// DlgJYInput.cpp : ʵ���ļ�
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
// CDlgJYInput �Ի���

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


// CDlgJYInput ��Ϣ�������
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
		// ��ȡӪҵ����Ϣ
		m_pCellList = g_pDoLogin->m_pTradeConfig->GetCellList(); //��ȡӪҵ��
		if(m_pCellList)
		{
			CComboBox* pComboCell = (CComboBox*)GetDlgItem(IDC_COMBO_Cell);
			for(int i = 0; i < m_pCellList->GetCount(); i++)
			{
				ICellItem* pItem = m_pCellList->GetItems(i);
				CString sItem = pItem->GetCaption();
				pComboCell->AddString(sItem); //��Ӫҵ����ӵ���������
				pItem->Release();
			}
		}
		else
		{
			CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL, Login_Log_Name, "��¼���ȡӪҵ��ʧ�ܣ�");
		}
		//////////////////////////////////////////////////////////////////////////
		// ��ȡ�˺���Ϣ
		g_pDoLogin->m_pCommCfg->GetAccount(m_ayAccInfo);
		// ��ȡ�Զ������˺ſ���
		m_bSaveAcc = g_pDoLogin->m_pCommCfg->GetSaveAccount();
		((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->SetCheck(m_bSaveAcc);
		// ���˺���Ϣ����Ͽؼ���
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
			// ��ǰ�û��˺ŵ��˺����� �Լ� Ӫҵ��
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
		// ��ȡ�����˺ſ���
		m_bHideAcc = g_pDoLogin->m_pCommCfg->GetHideAccount();
		((CButton*)GetDlgItem(IDC_CHECK_HIDENO))->SetCheck(m_bHideAcc);
		m_comBoxAcc.SetHide(m_bHideAcc);
	}	
	m_sEdit.SetLimitText(8); //��Ҫ�������ļ��л��
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
					pComboAccType->AddString(sItem); //���˺�������ӵ���������

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
				CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"��¼���ȡ�˺�����ʧ�ܣ�");
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
	{// ������ͬ���˺� �ٱȽ��ʽ����� �Լ�Ӫҵ��ID
		
		if (strType == m_ayAccInfo.GetAt(i+1) && strCell == m_ayAccInfo.GetAt(i+2))
		{// ����ͬ ��ֱ�ӷ���
			m_ayAccInfo.RemoveAt(i, 3); // �Ƴ������е���Ϣ 
		}
	}

	// ��ӵ������ײ�
	m_ayAccInfo.InsertAt(0, strCell);
	m_ayAccInfo.InsertAt(0, strType);
	m_ayAccInfo.InsertAt(0, strAcc);

	m_bAccUpdate = TRUE;
}

void CDlgJYInput::OnDestroy()
{
	//////////////////////////////////////////////////////////////////////////
	// �����¼�˺ſ���
	if (m_bSaveAcc && m_bAccUpdate)
	{// �����Ҫ��¼�˺� ���浱ǰ�˺�
		g_pDoLogin->m_pCommCfg->SaveAccount(m_ayAccInfo, m_ayAccInfo.GetCount()/3);
		g_pDoLogin->m_pCommCfg->SaveToFile();
	}

	// �ͷŽ������ö���
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

		if(strValidate.Compare(strPreValidate) != 0) //���ǰ�����αȽϲ���� ����Ҫ�������»�����֤��ͼƬ
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
	{//�Ѿ�����ǰ����֤���ַ��� ����ͼƬ 
		strPreValidate = strValidate;
	}

}
BOOL CDlgJYInput::LoginJY()
{
	//TRACE("����LoginJY����\n");
	//int nTick = GetTickCount();
	EnableInput(FALSE);

	CString strText;
	CString strTitle;
	strTitle.LoadString(IDS_STRING_Error);

	CString sAccount = "";
	sAccount = m_comBoxAcc.GetEditText(); //��ȡ�˺�
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
	sPassward = m_sEdit.GetRealText(); //��ȡ����
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
	GetDlgItem(IDC_EDIT_VALIDATE)->GetWindowText(str); //��ȡ��֤��
	if (str.Compare(strValidate))
	{
		strText.LoadString(IDS_STRING_ValidateError);
		MessageBox(strText,strTitle);
		EnableInput(TRUE);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_VALIDATE);
		pEdit->SetFocus();
		pEdit->SetSel(0,pEdit->LineLength()); //��֤�������ѡ��

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
			CString strCell = pItem->GetCellID();   // Ӫҵ��ID
			pAccount->SetCellNo(strCell);  
		
			CString sItem; 
			((CComboBox*)GetDlgItem(IDC_COMBO_AccountType))->GetWindowText(sItem);
			if(m_pAccountTypeList)
			{
				// �����˺ŵ�½����
				pAccount->SetLoginAccountType(m_pAccountTypeList->GetBackTypeByAccountType(sItem)); 
				// �����˺��г�����
				pAccount->SetLoginMarketType(m_pAccountTypeList->GetMarketTypeByAccountType(sItem));	
			}
			else
				TRACE("��ȡpAccountʧ��,ԭ��:m_pAccountTypeListָ��Ϊ��\n");

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
				//����
				pAccount->SetClientAddr(pTrade->GetClientAdd());
				CString sJrName = pTrade->GetJrName(); //��ȡ�������������
				if(sJrName.IsEmpty()) //����������Ǳ�û�������� ����뱾����������
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
	if(strValidate != "first") //������ǵ�һ������ ���ǰһ�ε���֤���¼����
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
		bRet = MessageBox("��ѡ���˲���ס�˺ţ�ԭ�ȼ�ס���˺Ŷ��ᱻ�����ȷ����", "ȷ��", MB_OKCANCEL | MB_ICONINFORMATION);	
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
	CRect rectLogin; //��¼��������
	CWnd* pWnd = GetParent();
	if(pWnd && ::IsWindow(pWnd->m_hWnd))
		pWnd->GetClientRect(&rectLogin);
	else
		return FALSE;

	CRect rectKeyboard; //����̾���
	if(m_keyboardDlg && ::IsWindow(m_keyboardDlg->m_hWnd))
		m_keyboardDlg->GetClientRect(rectKeyboard);
	else
		return FALSE;

	int xScreen = ::GetSystemMetrics(SM_CXSCREEN); //��ȡ��ǰ��Ļ���
	CPoint rb(rectLogin.right,rectLogin.bottom);   //��ȡ��½�����½�����
	CPoint lb(rectLogin.left,rectLogin.bottom);    //��ȡ��½�����½�����
	::ClientToScreen(pWnd->m_hWnd,&rb);
	::ClientToScreen(pWnd->m_hWnd,&lb);

	CPoint startPoint;
	if(rb.x + rectKeyboard.Width() < xScreen)
	{//��ʾ���ұ� ���Բ�������Ļ
		startPoint.x = rb.x;
		startPoint.y = rb.y - rectKeyboard.Height();
	}
	else if(lb.x - rectKeyboard.Width() > 0)
	{//��ʾ����� ���Բ�С����Ļ
		
		startPoint.x = lb.x - rectKeyboard.Width();
		startPoint.y = lb.y - rectKeyboard.Height();	
	}

	CPoint oldPoint(rectKeyboard.left,rectKeyboard.top);
	::ClientToScreen(m_keyboardDlg->m_hWnd,&oldPoint); //������̾�������ת��Ϊ��Ļ����
	if(startPoint == oldPoint) //����Ҫ�ƶ� 
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
	// ϵͳ��ַ
	int nCellID = GetCurrentCellID();
	CCommInfoArray* pAy = g_pDoLogin->m_commData.GetSysServerInfo();
	for (int i = 0; i < pAy->GetCount(); i++)
	{// ��ԭ�����׵ķ�������ַɾ��
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
	// �û��Զ����ַ
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
	// ����Ĭ�ϵ�ַ
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
{// �ʽ��˻�������ѡ��ı�

	CString strAcc = m_comBoxAcc.GetEditText();
	int i(0);
	for (i = 0; i < m_ayAccInfo.GetCount(); i++)
	{
		if (m_ayAccInfo.GetAt(i).CompareNoCase(strAcc) == 0)
			break;
	}

	if (i != m_ayAccInfo.GetCount() && m_pCellList)
	{// �ڵ�ǰ������û���Ϣ���ҵ�
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
