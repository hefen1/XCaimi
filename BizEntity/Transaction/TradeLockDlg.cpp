/*******************************************************
  源程序名称:TradeLockDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  锁屏
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "TradeLockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TradeLockDlg 对话框

IMPLEMENT_DYNAMIC(CTradeLockDlg, CHSBizDialog)

CTradeLockDlg::CTradeLockDlg(CWnd* pParent /*=NULL*/)
: CHSBizDialog(CTradeLockDlg::IDD, pParent)
{
	m_pTradeCoreObj = (ITradeCoreObj*) CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_pAccountMng = m_pTradeCoreObj->GetAccountMng();
	m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
}

CTradeLockDlg::~CTradeLockDlg()
{
}

void CTradeLockDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BT_REFRESH, m_btRefresh);
	//DDX_Control(pDX, IDC_ED_ACCOUNT, m_edAccount);
	DDX_Control(pDX, IDC_ED_PSW, m_edPsw);
	DDX_Control(pDX, IDC_ST_FUNDACCOUNT, m_stFundAccount);
	DDX_Control(pDX, IDC_CB_FUNDACCOUNT, m_cbFundAccount);
	//DDX_Control(pDX, IDC_BT_MIN, m_btMin);
	//DDX_Control(pDX, IDC_BT_MAX, m_btMax);
	//DDX_Control(pDX, IDC_BT_CLOSE, m_btClose);
	DDX_Control(pDX, IDC_ST_HINT, m_stHint);
	DDX_Control(pDX, ID_BT_EXIT, m_btExit);
	DDX_Control(pDX, ID_BT_OK, m_btOK);
	DDX_Control(pDX, ID_BT_CANCEL, m_btCancel);
	DDX_Control(pDX, IDC_ST_PSW, m_stPsw);
	DDX_Control(pDX, IDC_ST_ERRORHINT, m_stErrorHint);
}

BEGIN_MESSAGE_MAP(CTradeLockDlg, CHSBizDialog)
	//
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_BT_EXIT, &CTradeLockDlg::OnBnClickedBtExit)
	ON_BN_CLICKED(ID_BT_CANCEL, &CTradeLockDlg::OnBnClickedBtCancel)
	ON_BN_CLICKED(ID_BT_OK, &CTradeLockDlg::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeLockDlg::OnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (__super::OnInit())
	{
		GetDlgItem(IDC_GB_LOCK)->ShowWindow(SW_HIDE);
		/*CString strBmpPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
		CImage img;
		CString strFile = "";
		strFile = strBmpPath +  _T("close_main.bmp");
		img.Load(strFile);
		if( !img.IsNull() )
		{
			HBITMAP hBitmap = img.Detach();
			m_btClose.SetImage(hBitmap);
			m_btClose.SetAutorepeatMode();
			m_btClose.SizeToContent();
		}

		//strFile = strBmpPath +  _T("restore_main.bmp");
		strFile = strBmpPath +  _T("max_main.bmp");
		img.Load(strFile);
		if( !img.IsNull() )
		{
			HBITMAP hBitmap = img.Detach();
			m_btMax.SetImage(hBitmap);
			m_btMax.SetAutorepeatMode();
			m_btMax.SizeToContent();
		}

		strFile = strBmpPath +  _T("min_main.bmp");
		img.Load(strFile);
		if( !img.IsNull() )
		{
			HBITMAP hBitmap = img.Detach();
			m_btMin.SetImage(hBitmap);
			m_btMin.SetAutorepeatMode();
			m_btMin.SizeToContent();
		}*/
		
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeLockDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_edPsw.SetWindowText("");
		m_cbFundAccount.ResetContent();
		IAccount* pCurrAccount = m_pAccountMng->GetCurrentAccount(atTrade);
		int nCurrIndex = 0;
		for (int i = 0; i < m_pAccountMng->GetCount(atTrade); i++)
		{
			IAccount* pAccount = m_pAccountMng->GetAccount(i, atTrade);
			CString sTmp = "";
			sTmp.Format(_T("股票%s(%s)"), pAccount->GetAccountName(), pAccount->GetUserName());
			int nIndex = m_cbFundAccount.AddString(sTmp);
			m_cbFundAccount.SetItemDataPtr(nIndex, pAccount);
			if (pAccount == pCurrAccount)
			{
				nCurrIndex = nIndex;
			}
		}
		if (m_cbFundAccount.GetCount() > 0)
		{
			m_cbFundAccount.SetCurSel(nCurrIndex);
		}
		if (pCurrAccount->GetConnectHandle() == 0)
		{
			m_stErrorHint.SetWindowText("由于未知的原因连接被断开，请检查网络后重新连接！");
		}
		else
		{
			m_stErrorHint.SetWindowText("");
		}
	}
}

void CTradeLockDlg::OnWindowFocused()
{
	if (IsShow())
		m_edPsw.SetFocus();
}

void CTradeLockDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (m_cbFundAccount && m_cbFundAccount.GetSafeHwnd() && m_cbFundAccount.IsWindowEnabled())
	{
		CRect rectParent; 
		GetDlgItem(IDC_GB_LOCK)->GetParent()->GetClientRect(rectParent); 
		CRect rectControl; 
		GetDlgItem(IDC_GB_LOCK)->GetWindowRect(rectControl); 
		int iGroupX = (rectParent.Width()-rectControl.Width())/2;
		int iGroupY = (rectParent.Height()-rectControl.Height())/2;
		GetDlgItem(IDC_GB_LOCK)->MoveWindow(iGroupX, iGroupY, rectControl.Width(), rectControl.Height()); 

		m_stHint.GetWindowRect(rectControl);
		m_stHint.MoveWindow(iGroupX + 20, iGroupY + 10, rectControl.Width(), rectControl.Height());

		m_stFundAccount.GetWindowRect(rectControl);
		m_stFundAccount.MoveWindow(iGroupX + 5, iGroupY + 35, rectControl.Width(), rectControl.Height());

		m_cbFundAccount.GetWindowRect(rectControl);
		m_cbFundAccount.MoveWindow(iGroupX + 35, iGroupY + 30, rectControl.Width(), rectControl.Height());

		m_stPsw.GetWindowRect(rectControl);
		m_stPsw.MoveWindow(iGroupX + 5, iGroupY + 60, rectControl.Width(), rectControl.Height());

		m_edPsw.GetWindowRect(rectControl);
		m_edPsw.MoveWindow(iGroupX + 35, iGroupY + 55, rectControl.Width(), rectControl.Height());

		m_btOK.GetWindowRect(rectControl);
		m_btOK.MoveWindow(iGroupX + 5, iGroupY + 90, rectControl.Width(), rectControl.Height());

		m_btCancel.GetWindowRect(rectControl);
		m_btCancel.MoveWindow(iGroupX + 73, iGroupY + 90, rectControl.Width(), rectControl.Height());

		m_btExit.GetWindowRect(rectControl);
		m_btExit.MoveWindow(iGroupX + 141, iGroupY + 90, rectControl.Width(), rectControl.Height());

		m_stErrorHint.GetWindowRect(rectControl);
		m_stErrorHint.MoveWindow(iGroupX + 5, iGroupY + 120, rectControl.Width(), rectControl.Height());
	}	
}


void CTradeLockDlg::OnBnClickedBtExit()
{
	// TODO: Add your control notification handler code here
	CString sHint = m_pTradeLang->LoadStr(GVN_TRADE_EXIT);
	if (MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pAccountMng->FreeCache(atTrade);
		IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
		if(pMainWnd)
		{
			pMainWnd->ClosePanel(PN_TRADE);
		}
	}
}

void CTradeLockDlg::OnBnClickedBtCancel()
{
	// TODO: Add your control notification handler code here
	IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
	if(pMainWnd)
	{
		pMainWnd->ShowPanel(PN_TRADE,SP_HIDE,_T(""));
	}
}

void CTradeLockDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	CString sTmp;
	m_edPsw.GetWindowText(sTmp);
	//IAccount* pAccount = m_pAccountMng->GetCurrentAccount(atTrade);
	int nIndex = m_cbFundAccount.GetCurSel();
	IAccount* pAccount = (IAccount*)m_cbFundAccount.GetItemDataPtr(nIndex);
	if (pAccount->GetConnectHandle() == 0)
	{
		pAccount->SetPassword(sTmp);
		if(pAccount->Login(sTmp))
		{
			pAccount->SetIsLocked(FALSE);
			this->GetLayoutWindow()->OpenLayout(CURRENT_LAYOUT_TARGET,LN_TRADINGPANEL,PUBLIC_OWNER);
			m_pAccountMng->SetCurrentAccount(pAccount);
		}
		else
			MessageBox(sTmp, "提示", MB_ICONINFORMATION);
	}
	else
	{
		if (pAccount->GetPassword() == sTmp)
		{
			pAccount->SetIsLocked(FALSE);
			this->GetLayoutWindow()->OpenLayout(CURRENT_LAYOUT_TARGET,LN_TRADINGPANEL,PUBLIC_OWNER);
			m_pAccountMng->SetCurrentAccount(pAccount);
		}
		else
		{
			MessageBox("密码错误！", "提示", MB_ICONINFORMATION);
			m_edPsw.SetFocus();
			m_edPsw.SetSel(0, -1);
		}
	}
	
}

LRESULT CTradeLockDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_KEYDOWN && wParam == VK_RETURN)   
	{
		CWnd* pFocus = GetFocus();
		if ((&m_btOK) == pFocus)
		{
			OnBnClickedBtOk();
			return 0;
		}
	}
	return __super::WindowProc(message, wParam, lParam);
}