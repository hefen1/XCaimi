// LoginWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "LoginWnd.h"
#include "FrameLoginDlg.h"
#include "DlgSpeedTest.h"
#include "PageSrvMan.h"
#include "TreePropSheet.h"
#include "PageProxyMan.h"
#include "PageLevel.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CLoginWnd
extern CDlgSpeedTest g_dlgSpeedTest;
extern CFrameLoginDlg g_dlgFrameLogin;

// void WINAPI FSetProgressMsg(CDataSourceInitDataStauts *msg)
// {
// 	if(::IsWindow(g_dlgFrameLogin.GetLoginWnd()->m_hWnd))
// 		g_dlgFrameLogin.GetLoginWnd()->SetProgressMsg(msg);
// }

IMPLEMENT_DYNAMIC(CLoginWnd, CUIWnd)

CLoginWnd::CLoginWnd()
{
	m_bLogin = FALSE;
	m_bSpecialLogin = FALSE;
	m_nLoginType = HQJY;
	m_cJYInput = NULL;
	m_cLevelInput = NULL;
	m_nTimer = 0;
	
}

CLoginWnd::~CLoginWnd()
{
	if(m_cJYInput)
	{
		delete m_cJYInput;
		m_cJYInput = NULL;
	}
	if (m_cLevelInput)
	{
		delete m_cLevelInput;
		m_cLevelInput = NULL;
	}
}


void CLoginWnd::DoDataExchange(CDataExchange* pDX)
{
	CUIWnd::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_LOGIN, m_cBtnLogin);
	DDX_Control(pDX, IDC_BTN_GUIDE, m_cBtnGuide);
	DDX_Control(pDX, IDC_BTN_COMM, m_cBtnComm);
	DDX_Control(pDX, IDC_BTN_TEST, m_cBtnSpeed);
	DDX_Control(pDX, IDC_BTN_TUOJI, m_cBtnOff);


}

BEGIN_MESSAGE_MAP(CLoginWnd, CUIWnd)

	ON_BN_CLICKED(IDC_RAD_HQJY, OnBnClickedRadioHqjy)
	ON_BN_CLICKED(IDC_RAD_JY, OnBnClickedRadioJy)
	ON_BN_CLICKED(IDC_RAD_HQ, OnBnClickedRadioHq)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_EXIT,  OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BTN_TUOJI, OnBnClickedButtonTuoji)
	ON_BN_CLICKED(IDC_BTN_GUIDE, OnBnClickedButtonGuide)
	ON_BN_CLICKED(IDC_BTN_COMM,  OnBnClickedButtonComm)
	ON_BN_CLICKED(IDC_BTN_TEST,  OnBnClickedButtonTest)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CLoginWnd::Init()
{
	BOOL bJY;
	if(m_bSpecialLogin)
		m_nLoginType = JY;

	switch(m_nLoginType)
	{
	case HQ:
		bJY = FALSE;
		((CButton*)GetDlgItem(IDC_RAD_HQ))->SetCheck(TRUE);
		break;
	case JY:
		((CButton*)GetDlgItem(IDC_RAD_JY))->SetCheck(TRUE);
		if(m_bSpecialLogin)
		{
			((CButton*)GetDlgItem(IDC_RAD_HQ))->ShowWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_JY))->ShowWindow(FALSE);
			((CButton*)GetDlgItem(IDC_RAD_HQJY))->ShowWindow(FALSE);
		}
		bJY = TRUE;
		break;
	case HQJY:
		((CButton*)GetDlgItem(IDC_RAD_HQJY))->SetCheck(TRUE);
		bJY = TRUE;
		break;
	default:
		((CButton*)GetDlgItem(IDC_RAD_HQ))->SetCheck(TRUE);
		m_nLoginType = 0;
		bJY = FALSE;
		break;
	}
	if(HasLevel2() && !bJY)
	{
		if (!m_cLevelInput)
		{
			m_cLevelInput = new CDlgLevelInput;
		}
		if(!::IsWindow(m_cLevelInput->m_hWnd))
		{
			m_cLevelInput->Create(IDD_DIALOG_LEVELINPUT,this);
			CRect rc;
			GetDlgItem(IDC_STC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_cLevelInput->MoveWindow(rc);
		}
		if(m_cJYInput && ::IsWindow(m_cJYInput->m_hWnd))
		{
			m_cJYInput->ShowWindow(FALSE);
		}
		m_cLevelInput->ShowWindow(TRUE);
	}
	else
	{
		if(!m_cJYInput)
		{
			m_cJYInput = new CDlgJYInput;
		}
		if(!::IsWindow(m_cJYInput->m_hWnd))
		{
			m_cJYInput->Create(IDD_DIALOG_JY,this);
			CRect rc;
			GetDlgItem(IDC_STC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_cJYInput->MoveWindow(rc);

		}
		m_cJYInput->ShowWindow(TRUE);
		m_cJYInput->EnableInput(TRUE);

		if (m_cLevelInput && ::IsWindow(m_cLevelInput->m_hWnd))
			m_cLevelInput->ShowWindow(FALSE);
		if (!bJY)
		{
			m_cJYInput->EnableInput(FALSE);
		}
	}

}

void CLoginWnd::OnBnClickedRadioHqjy()//行情+交易
{
	m_nLoginType = HQJY;
	if(!m_cJYInput)
	{
		m_cJYInput = new CDlgJYInput;
	}
	if(!::IsWindow(m_cJYInput->m_hWnd))
	{
		m_cJYInput->Create(IDD_DIALOG_JY,this);
		CRect rc;
		GetDlgItem(IDC_STC_INPUTZONE)->GetWindowRect(rc);
		ScreenToClient(rc);	
		m_cJYInput->MoveWindow(&rc);
	}
	m_cJYInput->ShowWindow(TRUE);
	m_cJYInput->EnableInput(TRUE);
	
	//m_cJYInput->GetDlgItem(IDC_COMBO1)->SetFocus();
	if (m_cLevelInput && ::IsWindow(m_cLevelInput->m_hWnd))
		m_cLevelInput->ShowWindow(FALSE);
}

void CLoginWnd::OnBnClickedRadioJy()//交易
{
	m_nLoginType = JY;
	if(!m_cJYInput)
	{
		m_cJYInput = new CDlgJYInput;
	}
	if(!::IsWindow(m_cJYInput->m_hWnd))
	{
		m_cJYInput->Create(IDD_DIALOG_JY,this);
		CRect rc;
		GetDlgItem(IDC_STC_INPUTZONE)->GetWindowRect(rc);
		ScreenToClient(rc);	
		m_cJYInput->MoveWindow(&rc);
	}
	m_cJYInput->ShowWindow(TRUE);
	m_cJYInput->EnableInput(TRUE);
	//m_cJYInput->CreateValidate();
	//m_cJYInput->GetDlgItem(IDC_COMBO1)->SetFocus();
	if (m_cLevelInput && ::IsWindow(m_cLevelInput->m_hWnd))
		m_cLevelInput->ShowWindow(FALSE);
}

void CLoginWnd::OnBnClickedRadioHq()//行情
{
	m_nLoginType = HQ;
	if (HasLevel2())
	{
		if (!m_cLevelInput)
		{
			m_cLevelInput = new CDlgLevelInput;
		}
		if(!::IsWindow(m_cLevelInput->m_hWnd))
		{
			m_cLevelInput->Create(IDD_DIALOG_LEVELINPUT,this);
			CRect rc;
			GetDlgItem(IDC_STC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_cLevelInput->MoveWindow(&rc);
		}
		if(m_cJYInput && ::IsWindow(m_cJYInput->m_hWnd))
			m_cJYInput->ShowWindow(FALSE);
		m_cLevelInput->ShowWindow(TRUE);
		m_cLevelInput->GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
	}
	else
	{
		EnableButtens(FALSE);
		m_cJYInput->EnableInput(FALSE);
		QuoteInit();
	}
}

void CLoginWnd::OnBnClickedButtonLogin()//登陆
{

	BOOL bLoginJY;
	BOOL bLoginJYSuccess = FALSE;
	switch(m_nLoginType)
	{
	case HQ:
		bLoginJY = FALSE;
		break;
	case JY:
		bLoginJY = TRUE;
		break;
	case HQJY:
		bLoginJY = TRUE;
		break;
	default:
		bLoginJY = FALSE;
		break;
	}

	CString strText;
	if (bLoginJY)
	{
		EnableButtens(FALSE);
		strText.LoadString(IDS_STRING_JYNow);
		GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(strText);
		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(FALSE);
		//this->Invalidate();
		TRACE("开始登陆\n");
		if (m_cJYInput && m_cJYInput->LoginJY())//登录交易
		{
			bLoginJYSuccess = TRUE;
			TRACE("登陆之后 \n");
			GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
			strText.LoadString(IDS_STRING_JYSuccess);
			GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(strText);
			//this->Invalidate();
			if(m_bSpecialLogin)
			{
				m_bLogin=TRUE;
				Close();
				return;
			}
		}
		else//登录交易失败
		{
			bLoginJYSuccess = FALSE;
			GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
			strText.LoadString(IDS_STRING_JYFail);
			GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(strText);
			//this->Invalidate();
			EnableButtens(TRUE);
			m_bLogin = FALSE;
			return;
		}
	}
	if(HasLevel2())
	{
		if (m_cLevelInput && m_cLevelInput->LoginLevel())
		{
		}
	}
	if(QuoteInit() == FALSE) //如果行情登录失败
	{
		if(bLoginJY == TRUE) //如果有交易登录
		{
			if(bLoginJYSuccess == TRUE)
			{//交易登录成功

				CLoginApp * pApp = (CLoginApp*)AfxGetApp(); //将行情的连接信息放入账号管理器重
				IAccountMng *pTradeAccountMng = pApp->m_pTradeCore->GetAccountMng();
				if(pTradeAccountMng)
				{
					pQHAccount =  pTradeAccountMng->Add(atLevel1);
					if(pQHAccount)
					{
						pQHAccount->SetLoginAccount("hs-level1");
						pQHAccount->SetCellNo("");
						pQHAccount->SetLoginAccountType("");
						pQHAccount->SetPassword("hs-level1");
						pQHAccount->SetServerID(0);
						pQHAccount->SetBranchNo("0");
						pQHAccount->SetConnectHandle(0); //失败句柄为0
					}
					pTradeAccountMng->SetCurrentAccount(pQHAccount);
				}
				
				m_bLogin=TRUE;
				Close();
				return;
			}
			else
			{
				m_cJYInput->CreateValidate();
				m_cJYInput->EnableInput(TRUE);
				m_cJYInput->Invalidate();
			}
		}
	}
}
void CLoginWnd::OnBnClickedButtonTuoji()//脱机
{

}

void CLoginWnd::OnBnClickedCancel() //退出
{	
	m_bLogin = FALSE;
	Close();
}

void CLoginWnd::OnBnClickedButtonGuide()//使用指南
{

}

void CLoginWnd::OnBnClickedButtonComm()//通讯设置
{
	CommMan();
}

void CLoginWnd::OnBnClickedButtonTest()//测速
{
	g_dlgSpeedTest.DoModal();
}

void CLoginWnd::OnLButtonDown(UINT nFlags, CPoint point)
{

	if(m_nTimer == 0)
		m_nTimer = SetTimer((long)this,KeyBoardMoveTimer,NULL);

	CWnd* pDlg = GetParent();
	if(pDlg && ::IsWindow(pDlg->m_hWnd))
	{
		pDlg->PostMessage(WM_NCLBUTTONDOWN,HTCAPTION, MAKELPARAM(point.x,point.y));
	}

	CUIWnd::OnLButtonDown(nFlags, point);
}


void CLoginWnd::CommMan()
{
	CLoginApp * pApp = (CLoginApp*)AfxGetApp();
	CMap<Server_type,Server_type,BOOL,BOOL> *pMap = pApp->m_commData.GetSwitch();
	BOOL bOpen = FALSE;
	TreePropSheet::CTreePropSheet dlgPropertySheet(_T("通讯设置"));
	CImageList	DefaultImages;
	DefaultImages.Create(32,32,ILC_COLORDDB | ILC_MASK,0,0);
	CBitmap bm;
	dlgPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	CPageSrvMan PropertyPageAddress(CEV_Connect_HQ);
	PropertyPageAddress.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddress.m_psp.pszTitle = _T("行情"); 
	if(pMap->Lookup(CEV_Connect_HQ,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddress);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	CPageSrvMan PropertyPageAddressJY(CEV_Connect_JY);
	PropertyPageAddressJY.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddressJY.m_psp.pszTitle = _T("交易"); 
	if(pMap->Lookup(CEV_Connect_JY,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddressJY);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	CPageSrvMan PropertyPageAddressL(CEV_Connect_LEVEL2);
	PropertyPageAddressL.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddressL.m_psp.pszTitle = _T("Level2"); 
	if(pMap->Lookup(CEV_Connect_LEVEL2,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddressL);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	CPageSrvMan PropertyPageAddressC(CEV_Connect_CENTER);
	PropertyPageAddressC.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddressC.m_psp.pszTitle = _T("中心端"); 
	if(pMap->Lookup(CEV_Connect_CENTER,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddressC);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	CPageSrvMan PropertyPageAddressZ(CEV_Connect_ZX);
	PropertyPageAddressZ.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddressZ.m_psp.pszTitle = _T("资讯"); 
	if(pMap->Lookup(CEV_Connect_ZX,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddressZ);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	CPageProxyMan PropertyPageMan;
	PropertyPageMan.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageMan.m_psp.pszTitle = _T("高级设置"); 
	dlgPropertySheet.AddPage(&PropertyPageMan);
	bm.LoadBitmap(IDB_BITMAP3);
	DefaultImages.Add(&bm,RGB(255,0,255));
	bm.DeleteObject();

	CPageLevel PropertyPageLevel;
	PropertyPageLevel.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageLevel.m_psp.pszTitle = _T("level2帐号"); 
	if(pApp->IsLevel2Support())
	{
		dlgPropertySheet.AddPage(&PropertyPageLevel);
		bm.LoadBitmap(IDB_BITMAP3);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}
	dlgPropertySheet.SetTreeViewMode(TRUE, TRUE, TRUE);
	dlgPropertySheet.SetActivePage(&PropertyPageAddress);
	dlgPropertySheet.SetTreeWidth(120);
	dlgPropertySheet.SetTreeImages(&DefaultImages);

	try
	{
		if(IDOK == dlgPropertySheet.DoModal())
		{
			//保存修改后的通讯信息到配置模块
			//重新初始化通讯模块服务器列表
			pApp->m_commData.Save();
			pApp->SetSites();
		}
	}
	catch(...)
	{
		TRACE("Exception thrown: DoModal\n ");
	}   	

}
BOOL CLoginWnd::HasLevel2()
{
	CLoginApp * pApp = (CLoginApp*)AfxGetApp();
	return pApp->IsLevel2Support();
}

BOOL CLoginWnd::QuoteInit()
{
	long handle = ConnectServer();
	CString strText;
	if (handle <= 0)
	{
		strText.LoadString(IDS_STRING_QuoteFail);
		GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(strText);
		//this->Invalidate();

		if(m_cJYInput && ::IsWindow(m_cJYInput->m_hWnd))
		{
			m_cJYInput->EnableInput(TRUE);
			m_cJYInput->Invalidate();
		}
		EnableButtens(TRUE);
		return FALSE;
	}

// 	PFN_INITSTATUS_NOTIFY fNotify = FSetProgressMsg;
// 	CLoginApp * pApp = (CLoginApp*)AfxGetApp();
// 	if (pApp)
// 	{
// 	
// 		IAccountMng *pTradeAccountMng = pApp->m_pTradeCore->GetAccountMng();
// 		if(pTradeAccountMng)
// 		{
// 			pQHAccount =  pTradeAccountMng->Add(atLevel1);
// 			if(pQHAccount)
// 			{
// 				pQHAccount->SetLoginAccount("hs-level1");
// 				pQHAccount->SetCellNo("");
// 				pQHAccount->SetLoginAccountType("");
// 				pQHAccount->SetPassword("hs-level1");
// 				pQHAccount->SetServerID(0);
// 				pQHAccount->SetBranchNo("0");
// 				pQHAccount->SetConnectHandle(handle);
// 				CString sMsg = "";
// 				if( pQHAccount->Login(sMsg) )
// 				{
// 					pApp->m_pDataSource->HSDataSource_ServerDataInit(handle,fNotify);
// 					return TRUE;
// 				}
// 			}
// 		}
// 	}
	return FALSE;
}

void CLoginWnd::Close()
{
	
	CFrameLoginDlg* pDlg = (CFrameLoginDlg*)GetParent();
	if(pDlg)
	{
		pDlg->ShowWindow(FALSE);
		pDlg->SendMessage(WM_CLOSE);
	}
	SendMessage(WM_CLOSE);

}

long CLoginWnd::ConnectServer()
{
	CLoginApp * pApp = (CLoginApp*)AfxGetApp();
	CString strText;
	strText.LoadString(IDS_STRING_QuoteNow);
	GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(strText);
	if (pApp->m_commData.IsAuto())
	{
		return pApp->m_pManager->CreateBestConnector(CEV_Connect_HQ);
	}
	return pApp->m_pManager->CreateConnector(CEV_Connect_HQ,"");

}
void CLoginWnd::EnableButtens(BOOL en)
{
	GetDlgItem(IDC_RAD_HQJY)->EnableWindow(en);
	GetDlgItem(IDC_RAD_HQ)->EnableWindow(en);
	GetDlgItem(IDC_RAD_JY)->EnableWindow(en);
	GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(en);
	GetDlgItem(IDC_BTN_TUOJI)->EnableWindow(en);
	GetDlgItem(IDC_BTN_GUIDE)->EnableWindow(en);
	GetDlgItem(IDC_BTN_COMM)->EnableWindow(en);
	GetDlgItem(IDC_BTN_TEST)->EnableWindow(en);
}

BOOL CLoginWnd::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_TAB) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_BTN_TEST:
				if (m_cJYInput && ::IsWindow(m_cJYInput->m_hWnd) && m_cJYInput->IsWindowVisible())
				{
					m_cJYInput->GetDlgItem(IDC_COMBO1)->SetFocus();
				}
				else if (m_cLevelInput && ::IsWindow(m_cLevelInput->m_hWnd) && m_cLevelInput->IsWindowVisible())
				{
					m_cLevelInput->GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
				}
				break;
			default:
				GetNextDlgTabItem(GetFocus())->SetFocus();
				//NextDlgCtrl(); 
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_BTN_LOGIN:
				OnBnClickedButtonLogin();
				break;
			case IDC_BTN_TUOJI:
				OnBnClickedButtonTuoji();
				break;
			case IDC_BTN_EXIT:
				OnBnClickedCancel();
				break;
			case IDC_BTN_GUIDE:
				OnBnClickedButtonGuide();
				break;
			case IDC_BTN_COMM:
				OnBnClickedButtonComm();
				break;
			case IDC_BTN_TEST:
				OnBnClickedButtonTest();
				break;
			}
			return TRUE;
		}
	}
	return CUIWnd::PreTranslateMessage(pMsg);
}

void CLoginWnd::SetProgressMsg(CDataSourceInitDataStauts *msg)
{
	if(msg)
	{
		GetDlgItem(IDC_BTN_EXIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STC_PROGRESS)->SetWindowText(msg->m_szMsg);
		//this->Invalidate();
	}
	else
	{
		m_bLogin = TRUE;
		CLoginApp * pApp = (CLoginApp*)AfxGetApp();
		IAccountMng *pTradeAccountMng = pApp->m_pTradeCore->GetAccountMng();
		if(pTradeAccountMng)
			pTradeAccountMng->SetCurrentAccount(pQHAccount);
		
		Close();
	}
}

void CLoginWnd::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == m_nTimer)
	{
		if(m_cJYInput && ::IsWindow(m_cJYInput->m_hWnd))
		{
			if(m_cJYInput->MoveKeyboard() == FALSE && ::GetAsyncKeyState(VK_LBUTTON) == 0)
			{//移动错误 并且 鼠标左键没有按下 
				KillTimer(m_nTimer);
				m_nTimer = 0;
			}
		}
	}
	CUIWnd::OnTimer(nIDEvent);
}
