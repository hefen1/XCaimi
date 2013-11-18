// DlgJYInput.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "DlgJYInput.h"
#include "CreateValidate.h"
#include "..\HelperLib\GeneralHelper.h"
#include "WinnerApplication.h"
#include "DataSourceDefine.h"
#include "DoLogin.h"
//#include "LoginWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void WINAPI FSetProgressMsg(CDataSourceInitDataStauts *msg, CWnd* pWnd);
// CDlgJYInput 对话框

IMPLEMENT_DYNAMIC(CDlgJYInput, CDialogEx)

CDlgJYInput::CDlgJYInput(CWnd* pParent /*=NULL*/)
: CDialogEx(CDlgJYInput::IDD, pParent)
{
	m_BitHandle[0] = m_BitHandle[1] = m_BitHandle[2] = m_BitHandle[3] = NULL;
	strPreValidate = "first";
	strValidate = "first";
	m_keyboardDlg = NULL;
	m_pAccountTypeList = NULL;
	m_pCellList = NULL;
	m_ayAccInfo.RemoveAll();
	m_bAccUpdate = FALSE;
}

CDlgJYInput::~CDlgJYInput()
{
	if(m_keyboardDlg)
	{
		delete m_keyboardDlg;
		m_keyboardDlg = NULL;
	}
	for(int i=0; i<4; i++)
	{
		if (m_BitHandle[i])
		{
			DeleteObject(m_BitHandle[i]);
			m_BitHandle[i] = NULL;
		}
	}
}

void CDlgJYInput::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX,IDC_EDIT_JYPWD,m_sEdit);
	DDX_Control(pDX,IDC_COMBO_ACCOUNT,m_comBoxAcc);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgJYInput, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SoftKeyboard, &CDlgJYInput::OnBnClickedButtonSoftkeyboard)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_HIDENO, &CDlgJYInput::OnBnClickedCheckHide)
	ON_CBN_SELCHANGE(IDC_COMBO_Cell, &CDlgJYInput::OnCbnSelchangeComboCell)
	ON_CBN_SELCHANGE(IDC_COMBO_ACCOUNT, &CDlgJYInput::OnCbnSelchangeComboAccount)
	ON_BN_CLICKED(IDC_CHECK_REMEMBER, &CDlgJYInput::OnBnClickedCheckRemember)
	ON_CBN_SELCHANGE(IDC_COMBO_AccountType, &CDlgJYInput::OnCbnSelchangeComboAccounttype)
	ON_CBN_SELENDOK(IDC_COMBO_AccountType, &CDlgJYInput::OnCbnSelendokComboAccounttype)
END_MESSAGE_MAP()


// CDlgJYInput 消息处理程序
BOOL CDlgJYInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Init();
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, "face/Login_bkJY.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	this->SetBackgroundImage(bitmap);
	CreateValidate();
	if(GetDlgItem(IDC_COMBO_ACCOUNT) && GetDlgItem(IDC_COMBO_ACCOUNT)->m_hWnd)
	{
		GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
	}
	return FALSE;
}
void CDlgJYInput::Init()
{
	if(g_pDoLogin)
	{
		//////////////////////////////////////////////////////////////////////////
		// 获取营业部信息
		m_pCellList = g_pDoLogin->m_pTradeConfig->GetCellList(); //获取营业部
		if(m_pCellList)
		{
			CComboBox* pComboCell = (CComboBox*)GetDlgItem(IDC_COMBO_Cell);
			for(int i = 0; i < m_pCellList->GetCount(); i++)
			{
				ICellItem* pItem = m_pCellList->GetItems(i);
				CString sItem = pItem->GetCaption();
				pComboCell->AddString(sItem); //将营业部添加到下拉框中
				pItem->Release();
			}
		}
		else
		{
			CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Login_Log_Name, "登录框读取营业部失败！");
		}
		//////////////////////////////////////////////////////////////////////////
		// 获取账号信息
		g_pDoLogin->m_pCommCfg->GetAccount(m_ayAccInfo);
		// 获取自动保存账号开关
		m_bSaveAcc = g_pDoLogin->m_pCommCfg->GetSaveAccount();
		((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->SetCheck(m_bSaveAcc);
		// 把账号信息填充上控件上
		if (m_bSaveAcc && m_ayAccInfo.GetCount() > 0)
		{
			CString strAcc(""), strType(""), strCell("");
			CString strFormat;
			
			for (int i = 0; i < m_ayAccInfo.GetCount(); i=i+3)
			{
				strAcc  = m_ayAccInfo.GetAt(i);
				strType = m_ayAccInfo.GetAt(i+1);
				strCell = m_ayAccInfo.GetAt(i+2);
				
				ITradeAccountTypeList* pTypeList = m_pCellList->GetAccountTypeList(strCell);
				ITradeAccountTypeItem* pItem = pTypeList->GetItemsByName(strType);
				strFormat.Format("%s%s", pItem->GetShortName(), strAcc);
				m_comBoxAcc.AddString(strFormat);

				pItem->Release();
				pTypeList->Release();
			}	
			//////////////////////////////////////////////////////////////////////////
			// 当前用户账号的账号类型 以及 营业厅
			m_comBoxAcc.SetCurSel(0);
			strType = m_ayAccInfo.GetAt(1);
			strCell = m_ayAccInfo.GetAt(2);
			for (int i = 0; i < m_pCellList->GetCount(); i++)
			{
				ICellItem* pItem = m_pCellList->GetItems(i);
				if (pItem->GetCellID() == strCell)
				{
					((CComboBox*)GetDlgItem(IDC_COMBO_Cell))->SetCurSel(i);
					LoadAccountType(strType);
				}
				pItem->Release();
			}
		}
		else
		{
			CComboBox* pComboCell = (CComboBox*)GetDlgItem(IDC_COMBO_Cell);
			pComboCell->SetCurSel(0);
			LoadAccountType();
		}
		// 获取隐藏账号开关
		m_bHideAcc = g_pDoLogin->m_pCommCfg->GetHideAccount();
		((CButton*)GetDlgItem(IDC_CHECK_HIDENO))->SetCheck(m_bHideAcc);
		m_comBoxAcc.SetHide(m_bHideAcc);
	}	
	m_sEdit.SetLimitText(8); //需要从配置文件中获得
}

void CDlgJYInput::LoadAccountType(const CString& strType)
{
	if (g_pDoLogin)
	{
		if (m_pCellList)
		{
			int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_Cell))->GetCurSel();
			ICellItem* pItem = m_pCellList->GetItems(nIndex);
			if (m_pAccountTypeList)
			{
				m_pAccountTypeList->Release();
				m_pAccountTypeList = NULL;
			}
			m_pAccountTypeList = m_pCellList->GetAccountTypeList(pItem->GetCellID());
			pItem->Release();
			if(m_pAccountTypeList)
			{
				CComboBox* pComboAccType = ((CComboBox*)GetDlgItem(IDC_COMBO_AccountType));
				pComboAccType->ResetContent();
				int nIndex(0);
				for(int i = 0; i < m_pAccountTypeList->GetCount(); i++)
				{
					ITradeAccountTypeItem* pItem = m_pAccountTypeList->GetItems(i);
					CString sItem = pItem->GetAccountTypeName();
					pComboAccType->AddString(sItem); //将账号类型添加到下拉框中

					if (strType.CompareNoCase(sItem) == 0)
						nIndex = i;

					pItem->Release();
				}
				if (strType.IsEmpty())
					pComboAccType->SetCurSel(0);
				else
					pComboAccType->SetCurSel(nIndex);
				OnCbnSelchangeComboAccounttype();
			}
			else
			{
				CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"登录框读取账号类型失败！");
			}
		}
	}
}

void CDlgJYInput::SaveAccInfo( CString strAcc, CString strType, CString strCell)
{
	if (strAcc.IsEmpty() || strType.IsEmpty() || strCell.IsEmpty())
		return;

	int i(0);
	for (i = 0; i < m_ayAccInfo.GetCount(); i=i+3)
	{
		if (strAcc == m_ayAccInfo.GetAt(i))
			break;
	}
	if (i < m_ayAccInfo.GetCount())
	{// 存在相同的账号 再比较资金类型 以及营业部ID
		
		if (strType == m_ayAccInfo.GetAt(i+1) && strCell == m_ayAccInfo.GetAt(i+2))
		{// 都相同 就直接返回
			m_ayAccInfo.RemoveAt(i, 3); // 移出队列中的信息 
		}
	}

	// 添加到数列首部
	m_ayAccInfo.InsertAt(0, strCell);
	m_ayAccInfo.InsertAt(0, strType);
	m_ayAccInfo.InsertAt(0, strAcc);

	m_bAccUpdate = TRUE;
}

void CDlgJYInput::OnDestroy()
{
	//////////////////////////////////////////////////////////////////////////
	// 保存记录账号开关
	if (m_bSaveAcc && m_bAccUpdate)
	{// 如果需要记录账号 保存当前账号
		g_pDoLogin->m_pCommCfg->SaveAccount(m_ayAccInfo, m_ayAccInfo.GetCount()/3);
		g_pDoLogin->m_pCommCfg->SaveToFile();
	}

	// 释放交易配置对象
	if(m_pAccountTypeList)
	{
		m_pAccountTypeList->Release();
		m_pAccountTypeList = NULL;
	}
	if (m_pCellList)
	{
		m_pCellList->Release();
		m_pCellList = NULL;
	}

	CDialogEx::OnDestroy();
}

void CDlgJYInput::OnPaint()
{
	CDialog::OnPaint();

	CRect rect;
	GetDlgItem(IDC_STATIC_VALIDATE)->GetWindowRect(rect);
	ScreenToClient(rect);

	BOOL bRet = FALSE; 
	for(int i=0; i<strValidate.GetLength(); i++)
	{
		CRect textOutRect;
		textOutRect = rect;
		textOutRect.left = rect.left + rect.Width()/strValidate.GetLength()*i;
		textOutRect.right = textOutRect.left + rect.Width()/strValidate.GetLength();

		if(strValidate.Compare(strPreValidate) != 0) //如果前后两次比较不相等 则需要进行重新绘制验证码图片
		{
			bRet = TRUE;
			CCreateValidate m_CreateValidate;
			m_BitHandle[i] = m_CreateValidate.CreateBMP(this->GetSafeHwnd(),strValidate.Mid(i,1),textOutRect.Height(),textOutRect.Width());
		}

		CBitmap pBitmap;  
		pBitmap.Attach(m_BitHandle[i]);  
		CClientDC dc(this);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);       
		HBITMAP	hBitmapOld = (HBITMAP)memDC.SelectObject(pBitmap); 

		dc.BitBlt(textOutRect.left,textOutRect.top,textOutRect.Width(),textOutRect.Height(),&memDC,0,0,SRCCOPY);

		memDC.SelectObject(hBitmapOld);
		pBitmap.Detach();
		pBitmap.DeleteObject();
	}
	if(bRet = TRUE)
	{//已经将当前的验证码字符串 生成图片 
		strPreValidate = strValidate;
	}

}
BOOL CDlgJYInput::LoginJY()
{
	//TRACE("进入LoginJY函数\n");
	//int nTick = GetTickCount();
	EnableInput(FALSE);

	CString strText;
	CString strTitle;
	strTitle.LoadString(IDS_STRING_Error);

	CString sAccount = "";
	sAccount = m_comBoxAcc.GetEditText(); //获取账号
	if(sAccount.IsEmpty())
	{
		strText.LoadString(IDS_STRING_AccountError);
		MessageBox(strText,strTitle);
		EnableInput(TRUE);
		GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
		CreateValidate();
		//Invalidate(FALSE);
		return FALSE;
	}

	CString sPassward = "";
	sPassward = m_sEdit.GetRealText(); //获取密码
	if(sPassward.IsEmpty())
	{
		strText.LoadString(IDS_STRING_PasswardError);
		MessageBox(strText,strTitle);
		EnableInput(TRUE);
		GetDlgItem(IDC_EDIT_JYPWD)->SetFocus();
		CreateValidate();
		//Invalidate(FALSE);
		return FALSE;
	}

	CString str;
	GetDlgItem(IDC_EDIT_VALIDATE)->GetWindowText(str); //获取验证码
	if (str.Compare(strValidate))
	{
		strText.LoadString(IDS_STRING_ValidateError);
		MessageBox(strText,strTitle);
		EnableInput(TRUE);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VALIDATE);
		pEdit->SetFocus();
		pEdit->SetSel(0,pEdit->LineLength()); //验证码输入框选中

		CreateValidate();
		//Invalidate(FALSE);
		return FALSE;
	}

	IAccountMng *pTradeAccountMng = g_pDoLogin->m_pTradeCore->GetAccountMng();
	if (g_pDoLogin && pTradeAccountMng)
	{
		//IAccount* pAccount =  pTradeAccountMng->Add(atTrade);
		IAccount* pAccount =  pTradeAccountMng->NewAccount(atTrade);
		if (pAccount && m_pCellList)
		{
			int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_Cell))->GetCurSel();
			ICellItem* pItem = m_pCellList->GetItems(nIndex); 
			pAccount->SetLoginAccount(sAccount);
			pAccount->SetPassword(sPassward);
			CString strCell = pItem->GetCellID();   // 营业部ID
			pAccount->SetCellNo(strCell);  
		
			CString sItem; 
			((CComboBox*)GetDlgItem(IDC_COMBO_AccountType))->GetWindowText(sItem);
			if(m_pAccountTypeList)
			{
				// 设置账号登陆类型
				pAccount->SetLoginAccountType(m_pAccountTypeList->GetBackTypeByAccountType(sItem)); 
				// 设置账号市场类型
				pAccount->SetLoginMarketType(m_pAccountTypeList->GetMarketTypeByAccountType(sItem));	
			}
			else
				TRACE("获取pAccount失败,原因:m_pAccountTypeList指针为空\n");

			pAccount->SetServerID(pItem->GetServerID());
			pAccount->SetBranchNo(pItem->GetBranchNo());
			pItem->Release();
			CString sMsg = "";
			long handle = ConnectJYSrv();
			if (handle > 0)
			{
				IReqHeart *pTrade = g_pDoLogin->m_pTradeCore->NewReqHeart();
				pTrade->SetConnectHandle(handle);
				pTrade->TradingSync();
				CString version = pTrade->GetVersion();
				//升级
				pAccount->SetClientAddr(pTrade->GetClientAdd());
				CString sJrName = pTrade->GetJrName(); //获取接入服务器名称
				if(sJrName.IsEmpty()) //如果服务器那边没有配名称 则放入本地连接名称
				{
					sJrName = g_pDoLogin->m_pManager->GetServerName(handle);
				}
				pAccount->SetJrName(sJrName);

				pTrade->Release();
				pAccount->SetConnectHandle(handle);
				if(pAccount->Login(sMsg))
				{
					pTradeAccountMng->Add(pAccount);
					pTradeAccountMng->SetCurrentAccount(pAccount);
					if (m_bSaveAcc)
						SaveAccInfo(sAccount, sItem, strCell);
				}
				else
				{
					//pTradeAccountMng->DeleteAccount(pAccount);
					pAccount->Release();

					//int endTick = GetTickCount();
					//TRACE("%d",endTick-nTick);
					MessageBox(sMsg,strTitle);
					CreateValidate();
					//Invalidate(FALSE);
					EnableInput(TRUE);
					return FALSE;
				}
			}
			else
			{
				pTradeAccountMng->DeleteAccount(pAccount);
				CreateValidate();
				EnableInput(TRUE);

				CDataSourceInitDataStauts msg;
				strText.LoadString(IDS_STRING_JYServerFail);
				strcpy(msg.m_szMsg,strText.GetBuffer());
				FSetProgressMsg(&msg, GetParent());
				return FALSE;
			}
		}
	}
	return TRUE;
}
long CDlgJYInput::ConnectJYSrv()
{
	TradeCert tcInfo;
	tcInfo.m_Type = SSL;
	strncpy_s(tcInfo.m_ca, _T("hsca"), sizeof(tcInfo.m_ca));
	CString sCertFile =  CGeneralHelper::GetAppPath() + _T("Cert\\cert.pem");
	strncpy_s(tcInfo.m_cert, sCertFile, sizeof(tcInfo.m_cert));
	CString sCertKey =  CGeneralHelper::GetAppPath() + _T("Cert\\key.pem");
	strncpy_s(tcInfo.m_key, sCertKey, sizeof(tcInfo.m_key));
	strncpy_s(tcInfo.m_pwd, _T("111111"), sizeof(tcInfo.m_pwd));
	long m_lConnectHandle = g_pDoLogin->ConnectServer(CEV_Connect_JY, &tcInfo);
	return m_lConnectHandle;
}
void CDlgJYInput::CreateValidate()
{
	srand((unsigned int)GetTickCount());
	int a = rand()%10000;
	if(strValidate != "first") //如果不是第一次生成 则把前一次的验证码记录下来
	{
		strPreValidate = strValidate;
	}

	strValidate.Format("%04d",a);
	strValidate.Replace("4","8");
	GetDlgItem(IDC_STATIC_VALIDATE)->Invalidate();

}
void CDlgJYInput::EnableInput(BOOL en)
{
	GetDlgItem(IDC_EDIT_VALIDATE)->EnableWindow(en);
	GetDlgItem(IDC_EDIT_JYPWD)->EnableWindow(en);
	GetDlgItem(IDC_COMBO_ACCOUNT)->EnableWindow(en);
	GetDlgItem(IDC_COMBO1)->EnableWindow(en);
	GetDlgItem(IDC_COMBO2)->EnableWindow(en);
	GetDlgItem(IDC_CHECK_HIDENO)->EnableWindow(en);
	GetDlgItem(IDC_CHECK_REMEMBER)->EnableWindow(en);
	if(en)
		GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
}

BOOL CDlgJYInput::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_TAB) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_EDIT_VALIDATE:			
				this->GetParent()->GetDlgItem(IDC_BUTTON_LOGIN)->SetFocus();
				break;
			default:
				NextDlgCtrl(); 
			}
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN)
		{
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_EDIT_VALIDATE:
				this->GetParent()->GetDlgItem(IDC_BUTTON_LOGIN)->SetFocus();
				::PostMessage(GetParent()->m_hWnd, WM_KEYDOWN, VK_RETURN, 1); 
				break;
			default:
				NextDlgCtrl();
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			::PostMessage(GetParent()->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1);
			return TRUE;
		}
		else if (pMsg->wParam == 'D' && (::GetAsyncKeyState(VK_CONTROL) & 0x8000)  && (::GetAsyncKeyState(VK_MENU) & 0x8000))
		{
			IMainWnd* pMain = (IMainWnd*)CWinnerApplication::GetObject(_T("IMainWnd"));
			pMain->ShowPanel("TradeDebugView",SP_SHOW);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgJYInput::OnBnClickedCheckHide()
{
	m_bHideAcc = ((CButton*)GetDlgItem(IDC_CHECK_HIDENO))->GetCheck();
	m_comBoxAcc.SetHide(m_bHideAcc);

	g_pDoLogin->m_pCommCfg->SetHideAccount(m_bHideAcc);
}

void CDlgJYInput::OnBnClickedCheckRemember()
{
	BOOL bRet = IDNO;
	m_bSaveAcc = ((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->GetCheck();
	if (!m_bSaveAcc && m_comBoxAcc.GetCount() > 0)
	{
		bRet = MessageBox("你选择了不记住账号，原先记住的账号都会被清除，确定吗", "确认", MB_OKCANCEL | MB_ICONINFORMATION);	
		if (bRet == IDOK )
		{
			m_comBoxAcc.RemoveAllItem();
			m_ayAccInfo.RemoveAll();
			g_pDoLogin->m_pCommCfg->RemoveAllAccount();
			g_pDoLogin->m_pCommCfg->SaveToFile();
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->SetCheck(TRUE);
		}
	}
	m_bSaveAcc = ((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->GetCheck();

	g_pDoLogin->m_pCommCfg->SetSaveAccount(m_bSaveAcc);
}

void CDlgJYInput::OnBnClickedButtonSoftkeyboard()
{
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
		m_keyboardDlg->SetWnd(&m_sEdit);
		m_keyboardDlg->SetCharFilter(m_sEdit.GetCharFilter());
	}
	if(!::IsWindow(m_keyboardDlg->m_hWnd))
	{
		m_keyboardDlg->Create(NULL);
	}
	if(m_keyboardDlg->IsWindowVisible())
	{
		m_keyboardDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		MoveKeyboard();
		m_keyboardDlg->ShowWindow(SW_SHOW);
	}
}

BOOL CDlgJYInput::MoveKeyboard()
{
	CRect rectLogin; //登录框界面矩形
	CWnd* pWnd = GetParent();
	if(pWnd && ::IsWindow(pWnd->m_hWnd))
		pWnd->GetClientRect(&rectLogin);
	else
		return FALSE;

	CRect rectKeyboard; //软键盘矩形
	if(m_keyboardDlg && ::IsWindow(m_keyboardDlg->m_hWnd))
		m_keyboardDlg->GetClientRect(rectKeyboard);
	else
		return FALSE;

	int xScreen = ::GetSystemMetrics(SM_CXSCREEN); //获取当前屏幕宽度
	CPoint rb(rectLogin.right,rectLogin.bottom);   //获取登陆框右下角坐标
	CPoint lb(rectLogin.left,rectLogin.bottom);    //获取登陆框左下角坐标
	::ClientToScreen(pWnd->m_hWnd,&rb);
	::ClientToScreen(pWnd->m_hWnd,&lb);

	CPoint startPoint;
	if(rb.x + rectKeyboard.Width() < xScreen)
	{//显示在右边 可以不超过屏幕
		startPoint.x = rb.x;
		startPoint.y = rb.y - rectKeyboard.Height();
	}
	else if(lb.x - rectKeyboard.Width() > 0)
	{//显示在左边 可以不小于屏幕
		
		startPoint.x = lb.x - rectKeyboard.Width();
		startPoint.y = lb.y - rectKeyboard.Height();	
	}

	CPoint oldPoint(rectKeyboard.left,rectKeyboard.top);
	::ClientToScreen(m_keyboardDlg->m_hWnd,&oldPoint); //将软键盘矩形坐标转换为屏幕坐标
	if(startPoint == oldPoint) //不需要移动 
		return FALSE;

	m_keyboardDlg->MoveWindow(startPoint.x,startPoint.y,rectKeyboard.Width(),rectKeyboard.Height());
	return TRUE;
}

void CDlgJYInput::OnCbnSelchangeComboCell()
{
	// TODO: Add your control notification handler code here
	LoadAccountType();
	ChangeJYServer();
}

int CDlgJYInput::GetCurrentCellID()
{
	int nCellID(-1);
	if (m_pCellList)
	{
		int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_Cell))->GetCurSel();
		ICellItem* pItem = m_pCellList->GetItems(nIndex);
		nCellID = atoi(pItem->GetCellID());
		pItem->Release();
	}
	return nCellID;
}

void CDlgJYInput::ChangeJYServer()
{
	//////////////////////////////////////////////////////////////////////////
	// 系统地址
	int nCellID = GetCurrentCellID();
	CCommInfoArray* pAy = g_pDoLogin->m_commData.GetSysServerInfo();
	for (int i = 0; i < pAy->GetCount(); i++)
	{// 把原本交易的服务器地址删除
		CommInfo* pInfo = pAy->GetAt(i);
		if (pInfo->m_cType == CEV_Connect_JY)
		{
			delete pInfo;
			pAy->RemoveAt(i);
			i--;
		}
	}
	g_pDoLogin->m_pCommCfg->RemoveSysMap(CEV_Connect_JY, nCellID);
	g_pDoLogin->m_pCommCfg->GetSysServer(CEV_Connect_JY, pAy, nCellID);

	//////////////////////////////////////////////////////////////////////////
	// 用户自定义地址
	pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
	for (int i = 0; i < pAy->GetCount(); i++)
	{
		CommInfo* pInfo = pAy->GetAt(i);
		if (pInfo->m_cType == CEV_Connect_JY)
		{
			delete pInfo;
			pAy->RemoveAt(i);
			i--;
		}
	}
	g_pDoLogin->m_pCommCfg->RemoveUsrMap(CEV_Connect_JY, nCellID);
	g_pDoLogin->m_pCommCfg->GetUsrServer(CEV_Connect_JY, pAy, nCellID);

	//////////////////////////////////////////////////////////////////////////
	// 交易默认地址
	CString strDefault = g_pDoLogin->m_pCommCfg->GetDefaultSite(CEV_Connect_JY, nCellID);
	pAy = g_pDoLogin->m_commData.GetSysServerInfo();
	for (int i = 0; i < pAy->GetCount(); i++)
	{
		CommInfo* pInfo = pAy->GetAt(i);
		if (pInfo->m_cType == CEV_Connect_JY)
		{
			if (strDefault.CompareNoCase(pInfo->m_strServerID) == 0)
				pInfo->m_cIsDefault = 1;
			else
				pInfo->m_cIsDefault = 0;
		}
	}

	pAy = g_pDoLogin->m_commData.GetUsrServerInfo();
	for (int i = 0; i < pAy->GetCount(); i++)
	{
		CommInfo* pInfo = pAy->GetAt(i);
		if (pInfo->m_cType == CEV_Connect_JY)
		{
			if (strDefault.CompareNoCase(pInfo->m_strServerID) == 0)
				pInfo->m_cIsDefault = 1;
			else
				pInfo->m_cIsDefault = 0;
		}
	}
}

void CDlgJYInput::OnCbnSelchangeComboAccount()
{// 资金账户下拉框选择改变

	CString strAcc = m_comBoxAcc.GetEditText();
	int i(0);
	for (i = 0; i < m_ayAccInfo.GetCount(); i++)
	{
		if (m_ayAccInfo.GetAt(i).CompareNoCase(strAcc) == 0)
			break;
	}

	if (i != m_ayAccInfo.GetCount() && m_pCellList)
	{// 在当前保存的用户信息中找到
		CString strType   = m_ayAccInfo.GetAt(i+1);
		CString strCellID = m_ayAccInfo.GetAt(i+2);

		for (i = 0; i < m_pCellList->GetCount(); i++)
		{
			ICellItem* pItem = m_pCellList->GetItems(i);
			if (pItem->GetCellID() == strCellID)
			{
				((CComboBox*)GetDlgItem(IDC_COMBO_Cell))->SetCurSel(i);
				LoadAccountType(strType);
			}
			pItem->Release();
		}
	}
}

void CDlgJYInput::OnCbnSelchangeComboAccounttype()
{
	// TODO: Add your control notification handler code here
	if(m_pAccountTypeList)
	{
		CComboBox* pComboAccType = ((CComboBox*)GetDlgItem(IDC_COMBO_AccountType));
		CString sAccountType;
		int nIndex = pComboAccType->GetCurSel();
		ITradeAccountTypeItem* pItem = m_pAccountTypeList->GetItems(nIndex);
		if (pItem)
		{
			CStatic* pAccountType = ((CStatic*)GetDlgItem(IDC_ST_ACCOUNT));
			pAccountType->ShowWindow(SW_HIDE);
			pAccountType->SetWindowText(pItem->GetVisibleName());
			pAccountType->ShowWindow(SW_SHOW);
			pItem->Release();
		}
	}
}

void CDlgJYInput::OnCbnSelendokComboAccounttype()
{
	// TODO: Add your control notification handler code here
	
}
