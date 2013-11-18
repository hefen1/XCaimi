// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "DlgLogin.h"
#include "TreePropSheet.h"
#include "PageSrvMan.h"
#include "PageProxyMan.h"
#include "DlgSpeedTest.h"
#include "PageLevel.h"
#include "Resource.h"
#include "DoLogin.h"
#include "DlgLevelFrame.h"
#include "HSWinner.h"
#include "WinnerApplication.h"
#include "HSSkinIntf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgLogin 对话框

extern CDlgSpeedTest* g_pDlgSpeedTest;

void WINAPI FSetProgressMsg(CDataSourceInitDataStauts *msg, CWnd* pWnd)
{
	if ( pWnd && ::IsWindow(pWnd->GetSafeHwnd()))
	{
		CDlgLogin* pDlg = (CDlgLogin*)pWnd;
		pDlg->SetProgressMsg(msg);
	}
}

IMPLEMENT_DYNAMIC(CDlgLogin, CDialogEx)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLogin::IDD, pParent)
{
	m_bLogin = FALSE;
	m_eDlgType = LoginType_Normal;
	m_nLoginType = LoginRet_QuoteAndTrade;
	m_pJYInputDlg    = NULL;
	m_pLevelInputDlg = NULL;
	m_nTimer = 0;
}

CDlgLogin::~CDlgLogin()
{
	if(m_pJYInputDlg)
	{
		if (::IsWindow(m_pJYInputDlg->GetSafeHwnd()))
		{
			m_pJYInputDlg->DestroyWindow();
		}
		delete m_pJYInputDlg;
		m_pJYInputDlg = NULL;
	}
	if (m_pLevelInputDlg)
	{
		if (::IsWindow(m_pLevelInputDlg->GetSafeHwnd()))
		{
			m_pLevelInputDlg->DestroyWindow();
		}
		delete m_pLevelInputDlg;
		m_pLevelInputDlg = NULL;
	}

	m_csProgress.DestroyWindow();
	m_cBtnQuit.DestroyWindow();
	m_cBtnLogin.DestroyWindow();
	m_cBtnGuide.DestroyWindow();
	m_cBtnComm.DestroyWindow();
	m_cBtnSpeed.DestroyWindow();
	m_cBtnOff.DestroyWindow();
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PROGRESS, m_csProgress);
	DDX_Control(pDX, IDCANCEL, m_cBtnQuit);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_cBtnLogin);
	DDX_Control(pDX, IDC_BUTTON_GUIDE, m_cBtnGuide);
	DDX_Control(pDX, IDC_BUTTON_COMM, m_cBtnComm);
	DDX_Control(pDX, IDC_BUTTON_TEST, m_cBtnSpeed);
	DDX_Control(pDX, IDC_BUTTON_TUOJI, m_cBtnOff);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_HQJY, &CDlgLogin::OnBnClickedRadioHqjy)
	ON_BN_CLICKED(IDC_RADIO_JY, &CDlgLogin::OnBnClickedRadioJy)
	ON_BN_CLICKED(IDC_RADIO_HQ, &CDlgLogin::OnBnClickedRadioHq)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CDlgLogin::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_TUOJI, &CDlgLogin::OnBnClickedButtonTuoji)
	ON_BN_CLICKED(IDCANCEL, &CDlgLogin::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_GUIDE, &CDlgLogin::OnBnClickedButtonGuide)
	ON_BN_CLICKED(IDC_BUTTON_COMM, &CDlgLogin::OnBnClickedButtonComm)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgLogin::OnBnClickedButtonTest)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序
void CDlgLogin::Init()
{
	BOOL bJY;
	if(m_eDlgType == LoginType_OnlyTrade)
		m_nLoginType = LoginRet_Trade;

	switch(m_nLoginType)
	{
	case LoginRet_Quote:
		bJY = FALSE;
		((CMyButton*)GetDlgItem(IDC_RADIO_HQ))->SetCheck(TRUE);
		break;
	case LoginRet_Trade:
		((CMyButton*)GetDlgItem(IDC_RADIO_JY))->SetCheck(TRUE);
		if(m_eDlgType == LoginType_OnlyTrade)
		{
			((CMyButton*)GetDlgItem(IDC_RADIO_HQ))->ShowWindow(FALSE);
			((CMyButton*)GetDlgItem(IDC_RADIO_JY))->ShowWindow(FALSE);
			((CMyButton*)GetDlgItem(IDC_RADIO_HQJY))->ShowWindow(FALSE);
		}
		bJY = TRUE;
		break;
	case LoginRet_QuoteAndTrade:
		((CMyButton*)GetDlgItem(IDC_RADIO_HQJY))->SetCheck(TRUE);
		bJY = TRUE;
		break;
	default:
		((CMyButton*)GetDlgItem(IDC_RADIO_HQ))->SetCheck(TRUE);
		m_nLoginType = LoginRet_Quote;
		bJY = FALSE;
		break;
	}
	if(HasLevel2() && !bJY)
	{
		if (!m_pLevelInputDlg)
		{
			m_pLevelInputDlg = new CDlgLevelInput;
		}
		if(!::IsWindow(m_pLevelInputDlg->m_hWnd))
		{
			m_pLevelInputDlg->Create(IDD_DIALOG_LEVELINPUT,this);
			CRect rc;
			GetDlgItem(IDC_STATIC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_pLevelInputDlg->MoveWindow(&rc);
		}
		if(m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd))
			m_pJYInputDlg->ShowWindow(FALSE);
		m_pLevelInputDlg->ShowWindow(TRUE);
	}
	else
	{
		if(!m_pJYInputDlg)
		{
			m_pJYInputDlg = new CDlgJYInput;
		}
		if(!::IsWindow(m_pJYInputDlg->m_hWnd))
		{
			m_pJYInputDlg->Create(IDD_DIALOG_JY,this);
			CRect rc;
			GetDlgItem(IDC_STATIC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_pJYInputDlg->MoveWindow(&rc);
		}
		m_pJYInputDlg->ShowWindow(TRUE);
		m_pJYInputDlg->EnableInput(TRUE);
		if (m_pLevelInputDlg && ::IsWindow(m_pLevelInputDlg->m_hWnd))
			m_pLevelInputDlg->ShowWindow(FALSE);
		if (!bJY)
		{
			m_pJYInputDlg->EnableInput(FALSE);
		}
	}
}

BOOL CDlgLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bLogin = FALSE;
	/*IHSSkinObj* pHSSkinObj = (IHSSkinObj*) CWinnerApplication::GetObject(OBJ_HSSKIN);
	if (pHSSkinObj)
	{
		pHSSkinObj->LoadSkin("");
		pHSSkinObj->SetActive(TRUE);
		pHSSkinObj->SetSkinFormType(2);		
		pHSSkinObj->SkinForm(this->GetSafeHwnd());
	}*/
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, "face/Login_bk.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	this->SetBackgroundImage(bitmap);
	Init();
	return FALSE;
}
void CDlgLogin::OnBnClickedRadioHqjy()//行情+交易
{
	m_nLoginType = LoginRet_QuoteAndTrade;
	if(!m_pJYInputDlg)
	{
		m_pJYInputDlg = new CDlgJYInput;
	}
	if(!::IsWindow(m_pJYInputDlg->m_hWnd))
	{
		m_pJYInputDlg->Create(IDD_DIALOG_JY,this);
		CRect rc;
		GetDlgItem(IDC_STATIC_INPUTZONE)->GetWindowRect(rc);
		ScreenToClient(rc);	
		m_pJYInputDlg->MoveWindow(&rc);
	}
	m_pJYInputDlg->ShowWindow(TRUE);
	m_pJYInputDlg->EnableInput(TRUE);
	m_pJYInputDlg->GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
	if (m_pLevelInputDlg && ::IsWindow(m_pLevelInputDlg->m_hWnd))
		m_pLevelInputDlg->ShowWindow(FALSE);
}

void CDlgLogin::OnBnClickedRadioJy()//交易
{
	m_nLoginType = LoginRet_Trade;
	if(!m_pJYInputDlg)
	{
		m_pJYInputDlg = new CDlgJYInput;
	}
	if(!::IsWindow(m_pJYInputDlg->m_hWnd))
	{
		m_pJYInputDlg->Create(IDD_DIALOG_JY,this);
		CRect rc;
		GetDlgItem(IDC_STATIC_INPUTZONE)->GetWindowRect(rc);
		ScreenToClient(rc);	
		m_pJYInputDlg->MoveWindow(&rc);
	}
	m_pJYInputDlg->ShowWindow(TRUE);
	m_pJYInputDlg->EnableInput(TRUE);
	m_pJYInputDlg->GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
	if (m_pLevelInputDlg && ::IsWindow(m_pLevelInputDlg->m_hWnd))
		m_pLevelInputDlg->ShowWindow(FALSE);
}

void CDlgLogin::OnBnClickedRadioHq()//行情
{
	m_nLoginType = LoginRet_Quote;
	if (HasLevel2())
	{
		if (!m_pLevelInputDlg)
		{
			m_pLevelInputDlg = new CDlgLevelInput;
		}
		if(!::IsWindow(m_pLevelInputDlg->m_hWnd))
		{
			m_pLevelInputDlg->Create(IDD_DIALOG_LEVELINPUT,this);
			CRect rc;
			GetDlgItem(IDC_STATIC_INPUTZONE)->GetWindowRect(rc);
			ScreenToClient(rc);	
			m_pLevelInputDlg->MoveWindow(&rc);
		}
		if(m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd))
			m_pJYInputDlg->ShowWindow(FALSE);
		m_pLevelInputDlg->ShowWindow(TRUE);
		m_pLevelInputDlg->GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
	}
	else
	{
		EnableButtens(FALSE);
		m_pJYInputDlg->EnableInput(FALSE);
		QuoteInit();
	}

	if(g_pDoLogin->m_pQSysConf->IsSupportFuture())
	{
		FutureInit();
	}
}

void CDlgLogin::OnBnClickedButtonLogin()//登陆
{
	BOOL bLoginJY;
	BOOL bLoginJYSuccess = FALSE;
	switch(m_nLoginType)
	{
	case LoginRet_Trade:
    case LoginRet_QuoteAndTrade:
		bLoginJY = TRUE;
		break;
	default:
		bLoginJY = FALSE;
	}

	CString strText;
	if (bLoginJY)
	{
		EnableButtens(FALSE);
		strText.LoadString(IDS_STRING_JYNow);

		CRect rc;
		m_csProgress.GetClientRect(&rc);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();
		if (m_pJYInputDlg && m_pJYInputDlg->LoginJY())//登录交易
		{
			bLoginJYSuccess = TRUE;
 			strText.LoadString(IDS_STRING_JYSuccess);
 			m_csProgress.SetWindowText(strText);
			RedrawWindow();

			if(m_eDlgType == LoginType_OnlyTrade)
			{
				m_bLogin = TRUE;	
				OnCancel();
				return;
			}
		}
		else//登录交易失败
		{
			bLoginJYSuccess = FALSE;
			strText.LoadString(IDS_STRING_JYFail);
			m_csProgress.SetWindowText(strText);
			RedrawWindow();

			EnableButtens(TRUE);
			m_bLogin = FALSE;
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Level2登录
	BOOL bRet(FALSE);
	if(HasLevel2())
	{
		if (bLoginJY)
		{
			CString strAccName(""), strAccPwd("");
			BOOL bExit = g_pDoLogin->m_pCommCfg->GetLevelAcc(strAccName, strAccPwd); // 获取配置文件中账号、密码
			if (bExit)
			{
				bRet = Level2Init(strAccName, strAccPwd);
			}
			else
			{// 没有配 则用户自己输入
				CDlgLevelFrame dlgFrame;
				if (dlgFrame.DoModal() == IDOK)
				{
					bRet = dlgFrame.IsLogin();
				}
			}
		}
		else
			bRet = Level2Init();	
	}
	//////////////////////////////////////////////////////////////////////////
	if(QuoteInit() == FALSE) //如果行情登录失败
	{
		if(bLoginJY == TRUE) //如果有交易登录
		{
			if(bLoginJYSuccess == TRUE)
			{
				m_bLogin = TRUE;
				OnCancel();
				return;
			}
			else
			{
				m_pJYInputDlg->CreateValidate();
				m_pJYInputDlg->EnableInput(TRUE);
				//m_cJYInput->Invalidate(FALSE);
			}
		}
	}	
}

void CDlgLogin::OnBnClickedButtonTuoji()//脱机
{

}

void CDlgLogin::OnBnClickedCancel()//退出
{
	m_bLogin = FALSE;
	OnCancel();
}

void CDlgLogin::OnBnClickedButtonGuide()//使用指南
{
	
}

void CDlgLogin::OnBnClickedButtonComm()//通讯设置
{
	CommMan();
}

void CDlgLogin::OnBnClickedButtonTest()//测速
{
	g_pDlgSpeedTest->DoModal();
}

void CDlgLogin::CommMan()
{
	int nCellID = m_pJYInputDlg->GetCurrentCellID();

	CMap<Server_type,Server_type,BOOL,BOOL> *pMap = g_pDoLogin->m_commData.GetSwitch();
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
	//期货
	CPageSrvMan PropertyPageAddressFutrue(CEV_Connect_QH);
	PropertyPageAddressFutrue.m_psp.dwFlags |= PSP_USETITLE;
	PropertyPageAddressFutrue.m_psp.pszTitle = _T("期货"); 
	if(pMap->Lookup(CEV_Connect_QH,bOpen) && bOpen)
	{
		dlgPropertySheet.AddPage(&PropertyPageAddressFutrue);
		bm.LoadBitmap(IDB_BITMAP1);
		DefaultImages.Add(&bm,RGB(255,0,255));
		bm.DeleteObject();
	}

	CPageSrvMan PropertyPageAddressJY(CEV_Connect_JY, nCellID);
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
	if(g_pDoLogin->IsLevel2Support())
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
			g_pDoLogin->m_commData.Save(nCellID);
			g_pDoLogin->SetSites();
		}
	}
	catch(...)
	{
		TRACE("Exception thrown: DoModal\n ");
	}   
}
BOOL CDlgLogin::HasLevel2()
{
	return g_pDoLogin->IsLevel2Support();
}
BOOL CDlgLogin::QuoteInit()
{
	CString strText;
	strText.LoadString(IDS_STRING_QuoteNow);
	m_csProgress.SetWindowText(strText);
	RedrawWindow();

	long handle = g_pDoLogin->ConnectServer(CEV_Connect_HQ);
 	if (handle <= 0)
 	{
		strText.LoadString(IDS_STRING_QuoteFail);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();

		if(m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd))
		{
			m_pJYInputDlg->EnableInput(TRUE);
		}
		EnableButtens(TRUE);
 	}

 	PFN_INITSTATUS_NOTIFY fNotify = FSetProgressMsg;
	IAccountMng *pTradeAccountMng = g_pDoLogin->m_pTradeCore->GetAccountMng();
	if(pTradeAccountMng && pTradeAccountMng->GetCount(atLevel1) == 0)
	{
		IAccount* pQHAccount  = pTradeAccountMng->NewAccount(atLevel1);
		if(pQHAccount)
		{
			pQHAccount->SetLoginAccount("hs-level1");
			pQHAccount->SetCellNo("");
			pQHAccount->SetLoginAccountType("");
			pQHAccount->SetPassword("hs-level1");
			pQHAccount->SetServerID(0);
			pQHAccount->SetBranchNo("0");
			pQHAccount->SetConnectHandle(handle);
			CString sMsg = "";
			if( pQHAccount->Login(sMsg) )
			{
				pTradeAccountMng->Add(pQHAccount);
				pTradeAccountMng->SetCurrentAccount(pQHAccount);
				g_pDoLogin->m_pDataSource->HSDataSource_SetDefaultConnectHandle(handle);
				g_pDoLogin->m_pDataSource->HSDataSource_ServerDataInit(handle, fNotify, this);
				return TRUE;
			}
			else
				pQHAccount->Release();
		}
	}
 	return FALSE;
}

BOOL CDlgLogin::FutureInit()
{
	CString strText;
	strText.LoadString(IDS_STRING_FutureNow);
	m_csProgress.SetWindowText(strText);
	RedrawWindow();

	long handle = g_pDoLogin->ConnectServer(CEV_Connect_QH);
	if (handle <= 0)
	{
		strText.LoadString(IDS_STRING_FutureFail);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();

		if(m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd))
		{
			m_pJYInputDlg->EnableInput(TRUE);
		}
		EnableButtens(TRUE);
	}

	PFN_INITSTATUS_NOTIFY fNotify = FSetProgressMsg;
	IAccountMng *pTradeAccountMng = g_pDoLogin->m_pTradeCore->GetAccountMng();
	if(pTradeAccountMng && pTradeAccountMng->GetCount(atFuture) == 0)
	{
		IAccount *pFutureAccount= pTradeAccountMng->NewAccount(atFuture);
		if(pFutureAccount)
		{
			pFutureAccount->SetLoginAccount("hs-future");
			pFutureAccount->SetCellNo("");
			pFutureAccount->SetLoginAccountType("");
			pFutureAccount->SetPassword("hs-future");
			pFutureAccount->SetServerID(0);
			pFutureAccount->SetBranchNo("0");
			pFutureAccount->SetConnectHandle(handle);
			CString sMsg = "";
			if( pFutureAccount->Login(sMsg) )
			{
				pTradeAccountMng->Add(pFutureAccount);
				pTradeAccountMng->SetCurrentAccount(pFutureAccount);
				g_pDoLogin->m_pDataSource->HSDataSource_ServerDataInit(handle, fNotify, this);
				return TRUE;
			}
			else
				pFutureAccount->Release();
		}
	}
	return FALSE;
}

BOOL CDlgLogin::Level2Init(const CString &strAccName, const CString &strAccPwd)
{
	CString strText;
	strText.LoadString(IDS_STRING_Level2Now);
	m_csProgress.SetWindowText(strText);
	RedrawWindow();
	
	if (!m_pLevelInputDlg)
	{
		m_pLevelInputDlg = new CDlgLevelInput;	
	}
	if (!::IsWindow(m_pLevelInputDlg->m_hWnd))
	{
		m_pLevelInputDlg->Create(IDD_DIALOG_LEVELINPUT, this);
	}
	
	BOOL bRet = m_pLevelInputDlg->LoginLevel(strAccName, strAccPwd);
	if (bRet == FALSE)
	{
		strText.LoadString(IDS_STRING_Level2Fail);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();
	}
	return bRet;	
}

void CDlgLogin::SetProgressMsg(CDataSourceInitDataStauts *msg)
{
	static int i = 0;
	if(msg)
	{
		m_csProgress.SetWindowText(msg->m_szMsg);
		RedrawWindow();
	}
	else
	{
		i++;
		if( i < 2 )
			return;
		//////////////////////////////////////////////////////////////////////////
		// 键盘精灵初始化
		CString strText("");
		strText.LoadString(IDS_STRING_KeyboardInit);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();
		g_pDoLogin->m_pKeyboard->InitKeyboard();

		strText.LoadString(IDS_STRING_KeyboardFinish);
		m_csProgress.SetWindowText(strText);
		RedrawWindow();
		//////////////////////////////////////////////////////////////////////////
		m_bLogin = TRUE;
		OnCancel();
	}
}	
void CDlgLogin::EnableButtens(BOOL en)
{
	GetDlgItem(IDC_RADIO_HQJY)->EnableWindow(en);
	GetDlgItem(IDC_RADIO_HQ)->EnableWindow(en);
	GetDlgItem(IDC_RADIO_JY)->EnableWindow(en);
	GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(en);
	GetDlgItem(IDC_BUTTON_TUOJI)->EnableWindow(en);
	GetDlgItem(IDC_BUTTON_GUIDE)->EnableWindow(en);
	GetDlgItem(IDC_BUTTON_COMM)->EnableWindow(en);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(en);
}
BOOL CDlgLogin::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_TAB) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_BUTTON_TEST:
				if (m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd) && m_pJYInputDlg->IsWindowVisible())
				{
					m_pJYInputDlg->GetDlgItem(IDC_COMBO_ACCOUNT)->SetFocus();
				}
				else if (m_pLevelInputDlg && ::IsWindow(m_pLevelInputDlg->m_hWnd) && m_pLevelInputDlg->IsWindowVisible())
				{
					m_pLevelInputDlg->GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
				}
				break;
			default:
				NextDlgCtrl(); 
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_BUTTON_LOGIN:
				OnBnClickedButtonLogin();
				break;
			case IDC_BUTTON_TUOJI:
				OnBnClickedButtonTuoji();
				break;
			case IDCANCEL:
				OnBnClickedCancel();
				break;
			case IDC_BUTTON_GUIDE:
				OnBnClickedButtonGuide();
				break;
			case IDC_BUTTON_COMM:
				OnBnClickedButtonComm();
				break;
			case IDC_BUTTON_TEST:
				OnBnClickedButtonTest();
				break;
			}
			return TRUE;
		}		
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgLogin::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nTimer == 0)
		m_nTimer = SetTimer((long)this,KeyBoardMoveTimer,NULL);

	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
}

void CDlgLogin::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == m_nTimer)
	{
		if(m_pJYInputDlg && ::IsWindow(m_pJYInputDlg->m_hWnd))
		{
			if(m_pJYInputDlg->MoveKeyboard() == FALSE && ::GetAsyncKeyState(VK_LBUTTON) == 0)
			{//移动错误 并且 鼠标左键没有按下 
				KillTimer(m_nTimer);
				m_nTimer = 0;
			}
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CDlgLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages
}
