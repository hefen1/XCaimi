// TradeToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include <io.h>

#include "Transaction.h"
#include "TradeToolBar.h"
#include "HSMessage.h"
#include "..\..\HelperLib\GeneralHelper.h"

#define OPERAT_TIMEOUT		10*60


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//HHOOK hMsgBoxHook;
//
//LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	HWND hwnd;
//	if(nCode < 0)
//		return CallNextHookEx(hMsgBoxHook, nCode, wParam, lParam);
//	switch(nCode)
//	{
//	case HCBT_ACTIVATE:
//		// 现在wParam中就是message box的句柄
//		hwnd = (HWND)wParam;
//		// 我们已经有了message box的句柄，在这里我们就可以定制message box了!
//		return 0;
//	}
//	// Call the next hook, if there is one
//	return CallNextHookEx(hMsgBoxHook, nCode, wParam, lParam);
//}
//
//int MsgBoxEx(HWND hwnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
//{   
//	int ret;
//	// Install a thread hook, so we can customize it
//	 hMsgBoxHook = SetWindowsHookEx(
//		WH_CBT, 
//		CBTProc, 
//		NULL, 
//		GetCurrentThreadId()
//		);
//	// Display a standard message box
//	ret = MessageBox(hwnd, lpText, lpCaption, uType);
//	// remove the window hook
//	UnhookWindowsHookEx(hMsgBoxHook);
//	return ret;
//
//}



int CHSImgButton::GetImageHorzMargin() const 
{
	if(m_sizeImage.cx != 0)
		return 0;
	else
		return __super::GetImageHorzMargin();
}
int CHSImgButton::GetVertMargin() const 
{ 
	if(m_sizeImage.cx != 0)
		return 0;
	else
		return 3;
}
void CHSImgButton::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	CString strText;
	GetWindowText(strText);
	if( strText.IsEmpty())
		rectClient.InflateRect(2,2);
	__super::OnDrawBorder(pDC,rectClient,uiState);
}

void CHSImgButton::ChangeImg(CString strImgFile)
{
	CImage img;
	img.Load(strImgFile);
	if( !img.IsNull() )
	{
		HBITMAP hBitmap = img.Detach();
		this->SetImage(hBitmap);
	}
}


// CTradeToolBar 对话框

IMPLEMENT_DYNAMIC(CTradeToolBar, CHSBizDialog)

CTradeToolBar::CTradeToolBar(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CTradeToolBar::IDD, pParent)
{
	m_pTradeCoreObj = (ITradeCoreObj*) CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_pAccountMng = m_pTradeCoreObj->GetAccountMng();
	m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
	m_pDoLogin = (IDoLogin*)CWinnerApplication::GetObject(OBJ_LOGIN);
}

CTradeToolBar::~CTradeToolBar()
{
	m_pAccountMng->UnRegAccountChange(this);
	UnregisterLinkageMsg(TRADE_MESSAGE_ACTIONNAME);
	UnregisterHotKey(HK_NOOPERATNOTIFY, 0, OPERAT_TIMEOUT);
}

void CTradeToolBar::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_REFRESH, m_btRefresh);
	DDX_Control(pDX, IDC_ED_ACCOUNT, m_edAccount);
	DDX_Control(pDX, IDC_ED_PSW, m_edPsw);
	DDX_Control(pDX, IDC_ST_FUNDACCOUNT, m_stFundAccount);
	DDX_Control(pDX, IDC_CB_FUNDACCOUNT, m_cbFundAccount);
	DDX_Control(pDX, IDC_BT_MIN, m_btMin);
	DDX_Control(pDX, IDC_BT_MAX, m_btMax);
	DDX_Control(pDX, IDC_BT_CLOSE, m_btClose);
	DDX_Control(pDX, IDC_BT_LOCK, m_btLock);
	DDX_Control(pDX, IDC_ST_ACTIONNAME, m_stActionName);
	DDX_Control(pDX, IDC_BT_MOREACCOUNT, m_btMoreAccount);
}


BEGIN_MESSAGE_MAP(CTradeToolBar, CHSBizDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_LOGIN, &CTradeToolBar::OnBnClickedBtLogin)
	ON_BN_CLICKED(IDC_BT_REFRESH, &CTradeToolBar::OnBnClickedBtRefresh)
	ON_BN_CLICKED(IDC_BT_MIN, &CTradeToolBar::OnBnClickedBtMin)
	ON_BN_CLICKED(IDC_BT_MAX, &CTradeToolBar::OnBnClickedBtMax)
	ON_BN_CLICKED(IDC_BT_CLOSE, &CTradeToolBar::OnBnClickedBtClose)
	ON_BN_CLICKED(IDC_BT_LOCK, &CTradeToolBar::OnLockTrade)
	ON_COMMAND(ID_LOGIN, &CTradeToolBar::OnLogin)
	ON_COMMAND(ID_EXIT, &CTradeToolBar::OnExit)
	ON_BN_CLICKED(IDC_BT_MOREACCOUNT, &CTradeToolBar::OnBnClickedBtMoreaccount)
	ON_CBN_SELCHANGE(IDC_CB_FUNDACCOUNT, &CTradeToolBar::OnCbnSelchangeCbFundaccount)
END_MESSAGE_MAP()


// CTradeToolBar 消息处理程序

BOOL CTradeToolBar::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	CString strBmpPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH) +  _T("menu_bar_background.bmp");
	if (_taccess(strBmpPath,0) != -1)
	{
		HBITMAP hBmp = (HBITMAP)::LoadImage(GetModuleHandle(NULL), strBmpPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if(hBmp)
			SetBackgroundImage(hBmp);
	}
	m_menuMoreAccount.LoadMenu(IDR_MENU_MOREACCOUNT);
	//m_btnMenuControler.Create(_T("菜单树"),);
	//m_btnMenuControler.SetFaceColor(RGB(255,0,0),true);	
	CFont* pFont = CGeneralHelper::CreateFontEx("宋体", 20, FONT_BOLD, 0, 0, 0, GB2312_CHARSET); 
	m_stActionName.SetFont(pFont);
	return TRUE;
}

BOOL CTradeToolBar::OnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (__super::OnInit())
	{
		RegisterLinkageMsg(TRADE_MESSAGE_ACTIONNAME);
		CString strBmpPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
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
		}
		m_pAccountMng->RegAccountChange(this);
		for (int i = 0; i < m_pAccountMng->GetCount(atTrade); i++)
		{
			IAccount* pAccount = m_pAccountMng->GetAccount(i, atTrade);
			CString sTmp = "";
			sTmp.Format(_T("股票%s(%s)"), pAccount->GetAccountName(), pAccount->GetUserName());
			int nIndex = m_cbFundAccount.AddString(sTmp);
			m_cbFundAccount.SetItemDataPtr(nIndex, pAccount);
		}
		if (m_cbFundAccount.GetCount() > 0)
		{
			m_cbFundAccount.SetCurSel(0);
		}
		return TRUE;
	}
	else
		return FALSE;
	
}

void CTradeToolBar::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	if (m_cbFundAccount && m_cbFundAccount.GetSafeHwnd() && m_cbFundAccount.IsWindowEnabled())
	{
		CRect rcClient;
		GetClientRect(rcClient);
		CRect rcAccount;
		m_btClose.GetClientRect(rcAccount);
		CPoint ptBR = rcAccount.BottomRight();
		CPoint ptTL = rcAccount.TopLeft();
		int iWidth = rcAccount.Width();
		ptBR.x = rcClient.BottomRight().x - 5;
		ptTL.x = ptBR.x - iWidth;
		rcAccount.SetRect(ptTL, ptBR);
		m_btClose.MoveWindow(rcAccount);

		int ix = ptTL.x - 3;		
		m_btMax.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_btMax.MoveWindow(rcAccount);

		ix = ptTL.x - 3;		
		m_btMin.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_btMin.MoveWindow(rcAccount);

		ix = ptTL.x - 3;		
		m_cbFundAccount.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_cbFundAccount.MoveWindow(rcAccount);

		ix = ptTL.x - 3;		
		m_btMoreAccount.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_btMoreAccount.MoveWindow(rcAccount);

		ix = ptTL.x - 3;		
		m_btLock.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_btLock.MoveWindow(rcAccount);

		ix = ptTL.x - 3;		
		m_btRefresh.GetClientRect(rcAccount);
		iWidth = rcAccount.Width();
		ptBR = rcAccount.BottomRight();
		ptBR.x = ix;
		ptBR.y = ptBR.y;
		ptTL = rcAccount.TopLeft();
		ptTL.x = ptBR.x - iWidth;
		ptTL.y = ptTL.y;
		rcAccount.SetRect(ptTL, ptBR);
		m_btRefresh.MoveWindow(rcAccount);		
	}	
}

void CTradeToolBar::OnBnClickedBtLogin()
{
	// TODO: Add your control notification handler code here
	if (m_pTradeConfigObj)
	{
		ITradeTable* pTradeTable = m_pTradeConfigObj->GetTradeTableData(2501);
		if (NULL == pTradeTable)
		{
			return;
		}
		int iCount = pTradeTable->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			ITradeColumn* pTradeColumn = pTradeTable->GetItems(i);
			if (pTradeColumn)
			{
				CString sName = "";
				sName.Format("Name:%s,DispName:%s\n", pTradeColumn->GetColumnName(), pTradeColumn->GetDispName());
				OutputDebugString(sName);
				pTradeColumn->Release();
			}
		}
		pTradeTable->Release();
	}
    return;
	if (m_pTradeCoreObj)
	{
		IAccountMng* pAccountMng = m_pTradeCoreObj->GetAccountMng();
		if (pAccountMng)
		{
			//IAccount* pAccount =  pAccountMng->Add(atTrade);
			IAccount* pAccount =  pAccountMng->NewAccount(atTrade);
			if (pAccount)
			{
				CString sTmp = "";
				m_edAccount.GetWindowText(sTmp);
				pAccount->SetLoginAccount(sTmp);
				pAccount->SetCellNo("1");
				pAccount->SetLoginAccountType("1");
				m_edPsw.GetWindowText(sTmp);
				pAccount->SetPassword(sTmp);
				pAccount->SetServerID(1001);
				pAccount->SetBranchNo("1001");
				CString sMsg = "";
				if(pAccount->Login(sMsg))
				{
					pAccountMng->Add(pAccount);
					pAccountMng->SetCurrentAccount(pAccount);
				}
				else
				{
					pAccount->Release();
					//pAccountMng->DeleteAccount(pAccount);
				}
				AfxMessageBox(sMsg);
			}
		}
	}
}

void CTradeToolBar::OnBnClickedBtRefresh()
{
	// TODO: Add your control notification handler code here
	CString sParam = "";
	CLinkageParam param;
	this->PostLinkageMsg(TRADE_MESSAGE_REFRESH, &param);
	OutputDebugString("CTradeToolBar::OnBnClickedBtRefresh()\n");
}

void CTradeToolBar::OnBnClickedBtMin()
{
	IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
	if(pMainWnd)
	{
		pMainWnd->ShowPanel(PN_TRADE,SP_HIDE,_T(""));
	}
}

void CTradeToolBar::OnBnClickedBtMax()
{
	IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
	if(pMainWnd)
	{
		pMainWnd->ShowPanel(PN_TRADE,SP_MAX);		
	}
}

void CTradeToolBar::OnBnClickedBtClose()
{
   CString sHint = m_pTradeLang->LoadStr(GVN_TRADE_EXIT);
   if (MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
   {
	   m_pAccountMng->FreeCache(atTrade);
	   IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
	   if(pMainWnd)
	   {
		   
		   pMainWnd->ShowPanel(PN_TRADE,SP_HIDE,_T(""));
		   pMainWnd->ClosePanel(PN_TRADE);
	   }
   }	
}

void CTradeToolBar::OnLockTrade()
{
	m_pAccountMng->GetCurrentAccount(atTrade)->SetIsLocked(TRUE);
	this->GetLayoutWindow()->OpenLayout(_T("."),LN_TRADELOCKER,PUBLIC_OWNER);
}

void CTradeToolBar::OnAccountChange(IAccount* lpValue, EAccountChange nChangeType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (lpValue && (lpValue->GetAccountType() == atTrade))
	{
		switch (nChangeType)
		{
		case acChange:
			{
				int nIndex = m_cbFundAccount.GetCurSel();
				IAccount* pAccount = (IAccount*)m_cbFundAccount.GetItemDataPtr(nIndex);
				if (pAccount == lpValue)
				{
					break;
				}
				for (int i = 0; i < m_cbFundAccount.GetCount(); i++)
				{
					IAccount* pAccount = (IAccount*)m_cbFundAccount.GetItemDataPtr(i);
					if (pAccount == lpValue)
					{
						m_cbFundAccount.SetCurSel(i);
						break;
					}
				}
				break;
			}
		case acAdd:
			{
				CString sTmp = "";
				sTmp.Format(_T("股票%s(%s)"), lpValue->GetAccountName(), lpValue->GetUserName());
				int nIndex = m_cbFundAccount.AddString(sTmp);
				m_cbFundAccount.SetItemDataPtr(nIndex, lpValue);
				break;
			}	
		case acDel:
			{
				for (int i = 0; i < m_cbFundAccount.GetCount(); i++)
				{
					IAccount* pAccount = (IAccount*)m_cbFundAccount.GetItemDataPtr(i);
					if (pAccount == lpValue)
					{
						m_cbFundAccount.DeleteString(i);
						break;
					}
				}
				break;
			}
		}
	}
}

void CTradeToolBar::OnAccountStatus(IAccount* lpValue)
{
	if (IsShow() && lpValue)
	{
		//if(AfxGetApp()->m_nThreadID == GetCurrentThreadId())
		{
			if (lpValue->GetConnectHandle() == 0)
			{
				OnLockTrade();
			}	
		}			
	}
}

void CTradeToolBar::OnLayoutStateChange()
{
	IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
	if(pMainWnd == NULL)
		return;

	CString strBmpPath = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
	CImage img;
	CString strFile = "";
	if(pMainWnd->IsPanelZoomed(PN_TRADE))
		strFile = strBmpPath +  _T("restore_main.bmp");
	else
		strFile = strBmpPath +  _T("max_main.bmp");
	img.Load(strFile);
	if( !img.IsNull() )
	{
		HBITMAP hBitmap = img.Detach();
		m_btMax.SetImage(hBitmap);
		m_btMax.SetAutorepeatMode();
		m_btMax.SizeToContent();
		m_btMax.Invalidate(TRUE);
	}
}

void CTradeToolBar::OnLinkageMsg(CLinkageParam* pParam)
{
	switch (pParam->m_nMsg)
	{
	case TRADE_MESSAGE_ACTIONNAME:
		{
			m_stActionName.ShowWindow(SW_HIDE);
			m_stActionName.SetWindowText(pParam->GetStringField(PARAM_ACTIONNAME));
			m_stActionName.ShowWindow(SW_SHOW);
			break;
		}

	}
}
void CTradeToolBar::OnLogin()
{
	m_pDoLogin->Login(LoginType_OnlyTrade);
}

void CTradeToolBar::OnExit()
{
	CString sHint = "确定要注销当前账户吗？";//m_pTradeLang->LoadStr(GVN_TRADE_EXIT);
	if (MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	//if (MsgBoxEx(this->GetHandle(), sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
	{
		m_pAccountMng->DeleteAccount(m_pAccountMng->GetCurrentAccount(atTrade));
		//m_pAccountMng->FreeCache(atTrade);
		if (m_pAccountMng->GetCount(atTrade) == 0)
		{
			IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject(OBJ_MAINFRAME);
			if(pMainWnd)
			{
				pMainWnd->ShowPanel(PN_TRADE,SP_HIDE,_T(""));
				pMainWnd->ClosePanel(PN_TRADE);
			}
		}
	}		
}

void CTradeToolBar::OnBnClickedBtMoreaccount()
{
	// TODO: Add your control notification handler code here
	CMenu* pPopup = m_menuMoreAccount.GetSubMenu(0);
	CPoint point;
	GetCursorPos(&point);
	pPopup->TrackPopupMenu(TPM_LEFTBUTTON, point.x,point.y,this);
}

void CTradeToolBar::OnCbnSelchangeCbFundaccount()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_cbFundAccount.GetCurSel();
	IAccount* pAccount = (IAccount*)m_cbFundAccount.GetItemDataPtr(nIndex);
	m_pAccountMng->SetCurrentAccount(pAccount);
}

void CTradeToolBar::OnShow(BOOL bShow)
{
	if (bShow)
	{
		RegisterHotKey(HK_NOOPERATNOTIFY, 0, OPERAT_TIMEOUT);
	}
}

void CTradeToolBar::OnNoOperatTimeout()
{
	UnregisterHotKey(HK_NOOPERATNOTIFY, 0, OPERAT_TIMEOUT);
	OnLockTrade();
}