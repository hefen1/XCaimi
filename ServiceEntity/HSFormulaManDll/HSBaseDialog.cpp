// HSBaseDialog.cpp : implementation file
//

/*#define WINVER 0x0500*/

#include "stdafx.h"
#include "HSBaseDialog.h"
#include "shlwapi.h"
#include "resource.h"
#include "HSShadeButton.h"


//#include "HSEdit.h"

#define MAX_CLASSNAME 64

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSBaseDialog dialog

struct _AutoDeleteMemory
{
public:
	CImageList*	m_CloseImages;

	_AutoDeleteMemory()
	{
		m_CloseImages = NULL;
	}
	~_AutoDeleteMemory()
	{
		if( m_CloseImages )
		{
			delete m_CloseImages;
			m_CloseImages = NULL;
		}
	}
};

struct _AutoDeleteMemory g_CImageListEx_AutoDeleteMemory;

#define Support_HSCtrlManager 

/*extern UINT HS_OPEN_DATA_TIP_MSG;*/

IMPLEMENT_DYNAMIC(CHSBaseDialog, CDialog)
// IMPLEMENT_DYNCREATE

struct HSChangeWindowSize
{
	static int m_nWidth;
	static int m_nHeight;
	static int m_ndx;
	static int m_ndy;

	static void Init(HWND hWnd)
	{
		if( hWnd == NULL )
			return;

		::SetTimer(hWnd,(long)hWnd,100,TimerProc); 
	}

	static void CALLBACK EXPORT TimerProc(
				HWND hWnd,      // handle of CWnd that called SetTimer
				UINT nMsg,      // WM_TIMER
				UINT nIDEvent,   // timer identification
				DWORD dwTime    // system time
				)
	{
		if( hWnd == NULL )
			return;

		::KillTimer(hWnd, (long)hWnd);

		if( IsWindowVisible(hWnd) )
		{
			//this->CenterWindow();
			::AnimateWindow(hWnd,100,AW_CENTER);
		}
	}
};
int HSChangeWindowSize::m_nWidth   = 0;
int HSChangeWindowSize::m_nHeight  = 0;
int HSChangeWindowSize::m_ndx	    = 10;
int HSChangeWindowSize::m_ndy		= 10;

CHSBaseDialog::CHSBaseDialog() 
{
	m_hAccelTable = NULL;
	m_nFlags	  = 0; 
	m_cDeleteThis = 0;
	
	m_pThis = NULL;
	m_pArrayBut.RemoveAll();
}

CHSBaseDialog::CHSBaseDialog( LPCTSTR lpszTemplateName, CWnd* pParentWnd /*= NULL*/ )
	: CDialog(lpszTemplateName, pParentWnd)
{
	m_nFlags	  = 0;
	m_hAccelTable = NULL;
	m_cDeleteThis = 0;

	m_pThis = NULL;
	m_pArrayBut.RemoveAll();
	//{{AFX_DATA_INIT(CHSBaseDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CHSBaseDialog::CHSBaseDialog(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
{
	m_nFlags	  = 0;
	m_hAccelTable = NULL;
	m_cDeleteThis = 0;

	m_pThis = NULL;
	m_pArrayBut.RemoveAll();
	//{{AFX_DATA_INIT(CHSBaseDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CHSBaseDialog::~CHSBaseDialog()
{
	DeleteAllBut();
}

void CHSBaseDialog::Delete()
{
}

BEGIN_MESSAGE_MAP(CHSBaseDialog, CDialog)
	//{{AFX_MSG_MAP(CHSBaseDialog)
	ON_WM_CTLCOLOR()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

	ON_WM_NCLBUTTONDOWN()
	/*ON_WM_NCHITTEST()*/

	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDHELP, OnHelpButton)

/*	ON_REGISTERED_MESSAGE(HS_OPEN_DATA_TIP_MSG, HSUserMsg)*/
	
	ON_WM_NCACTIVATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CHSBaseDialog::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	/*mbClickDown = TRUE;
	CRect rc; 
	GetClientRect(rc); 
	ScreenToClient(&point);
	int nButton = GetButtonNumber(point);
    if (nButton != -1)
	{
		if (mbClickDown == TRUE)
		{
			SetCapture();
			mPushedButton[nButton] = TRUE; 
		}
		else
			mPushedButton[nButton] = FALSE; 
	}
	SendMessage(WM_NCPAINT, MAKEWPARAM(NULL, NULL), NULL);*/

	//ModifyStyleEx(0, WS_EX_CONTEXTHELP);

	if( (GetExStyle() & WS_EX_CONTEXTHELP) && nHitTest != HTHELP )
	{
		CRect rect;
		this->GetWindowRect(rect);
		rect.left  = rect.right = rect.right - (16 * 4) + 6;
		rect.left -= 16;

		if( rect.PtInRect(point) )
		{
			nHitTest = HTHELP;
		}
	}

	if( nHitTest == HTHELP )
	{
		CString strSection;
		/*SendMessage(HS_OPEN_DATA_TIP_MSG,0,(long)&strSection);*/
		if( strSection.IsEmpty() )
			return;

		strSection = "[" + strSection + "]";

// 		CString strDisp = 
// 			CWriteReadStandardConfig::GetContentsByHelpID(CHSFilePath::GetPath(CHSFilePath::Setting) + FILENAME_HS_HELPID_CFG,
// 			strSection);
// 
// 		HSOpenTip(point,"",
// 				strDisp,CHSDATATIP_STYLE_OPRNTIP_ATONCE,
// 				AfxGetMainWnd());

		return;
	}

	CDialog::OnNcLButtonDown(nHitTest, point);
}

UINT CHSBaseDialog::OnNcHitTest(CPoint point) 
{ 
	return CDialog::OnNcHitTest(point); 
 //   UINT ht = CDialog::OnNcHitTest(point); 
	//if (ht == HTCAPTION)
	//{
	//	/*ScreenToClient(&point);
	//	int nButton = GetButtonNumber(point);
	//	if (nButton != -1)
	//	{
	//		if (mbClickDown == TRUE)
	//		{
	//			SetCapture();
	//			mPushedButton[nButton] = TRUE; 
	//		}
	//		else
	//			mPushedButton[nButton] = FALSE; 
	//	}
	//	else
	//		mPushedButton[nButton] = FALSE;
	//	SendMessage(WM_NCPAINT, MAKEWPARAM(NULL, NULL), NULL);*/
	//}
 //   return ht; 
} 

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnHelpInfo
//	Desc:	Supply control-level context Help.
//	Args:	(See MFC doc)
//	Ret:	TRUE if handled.
/////////////////////////////////////////////////////////////////////////////
BOOL CHSBaseDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	//TRACE("CHelpDlg::OnHelpInfo iCtrlId = %x hItemHandle = %x dwContextId = %x\n", pHelpInfo->iCtrlId, pHelpInfo->hItemHandle, pHelpInfo->dwContextId);
	/*if (pHelpInfo->iContextType == HELPINFO_WINDOW)
	{
		AfxGetApp()->WinHelp(pHelpInfo->dwContextId, HELP_CONTEXTPOPUP);
	}*/

	return(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
//	Name:	OnHelpButton
//	Desc:	Handler for button with ID = IDHELP. You can place a button in your
//			dialog with this ID, and this will handle the routing to standard
//			Help for your dialog.
/////////////////////////////////////////////////////////////////////////////
void CHSBaseDialog::OnHelpButton() 
{
	// TODO: Add your control notification handler code here
	//OnCommandHelp(0, 0);
}

long CHSBaseDialog::HSUserMsg(WPARAM wParam, LPARAM lParam)
{
// 	if( wParam == HS_OPEN_DATA_TIP_MSG )
// 	{
// 		DWORD dwFlags = (long)lParam;
// 		RunModalLoop( dwFlags );
// 	}
// 	else
// 	{
		CString* strSection = (CString*)lParam;
		if( strSection )
		{
			static CString strShortName;
			if( strShortName.IsEmpty() )
			{
				AfxGetModuleShortFileName(AfxGetInstanceHandle(), strShortName);
				strShortName = ::PathFindFileName(strShortName);
			}

			strSection->Format("%s-%i",strShortName,m_nIDHelp);
			return (long)strSection;
		}
	/*}*/
	
	return 0;
}

void CHSBaseDialog::OnDestroy( )
{
	DeleteAllBut();
	CDialog::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CHSBaseDialog message handlers
BOOL CHSBaseDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//ModifyStyleEx(0, WS_EX_CONTEXTHELP);   

	HWND hCtrl = ::GetTopWindow(m_hWnd);
	while ( hCtrl )
	{
		if ( GetWindowLong( hCtrl, GWL_STYLE ) & WS_CHILD )
		{
			TCHAR szBuf[MAX_CLASSNAME];
			GetClassName( hCtrl, szBuf, MAX_CLASSNAME );
		
			DWORD dwStyle = GetWindowLong( hCtrl, GWL_STYLE );

			if ( !lstrcmpi( szBuf, _T( "Button" ) ) )
			{
				if ( ( dwStyle & BS_OWNERDRAW ) == BS_OWNERDRAW )         
				{
					hCtrl = ::GetNextWindow( hCtrl, GW_HWNDNEXT );
					continue;     // Do not subclass ownerdraw buttons
				}
				else if ( ( dwStyle & BS_GROUPBOX ) == BS_GROUPBOX ||
					( dwStyle & BS_FLAT ) == BS_FLAT ) 
				{
					hCtrl = ::GetNextWindow( hCtrl, GW_HWNDNEXT );
					continue;     // Skip all group boxes and flat buttons
				}
				else if ( ( dwStyle & BS_AUTOCHECKBOX ) == BS_AUTOCHECKBOX ||
					( dwStyle & BS_CHECKBOX ) == BS_CHECKBOX ||                
					( dwStyle & BS_3STATE ) == BS_3STATE )
				{
					hCtrl = ::GetNextWindow( hCtrl, GW_HWNDNEXT );
					continue;
				}
				else if ( ( dwStyle & BS_AUTORADIOBUTTON ) == BS_AUTORADIOBUTTON || 
					( dwStyle & BS_RADIOBUTTON ) == BS_RADIOBUTTON )
				{
					AddButton((int)hCtrl,-1,"",SHS_HARDBUMP);
				}
				//pCtl = new CCMRadioButton;
				else
				{
					AddButton((int)hCtrl,-1,"",SHS_HARDBUMP);
				}
				//pCtl = new CCMPushButton;     // If none of the above then it must be a pushbutton!
			}
		}
		hCtrl = ::GetNextWindow( hCtrl, GW_HWNDNEXT );
	}


	return TRUE;
}

BOOL CHSBaseDialog::ShowWindowEx( int nCmdShow )
{
	if( m_hWnd != NULL )
	{
		if( nCmdShow == WM_QUIT )
		{
			m_cDeleteThis = 1;

			if ( !(m_nFlags & WF_CONTINUEMODAL) )
			{
				CDialog::OnCancel();
				return 0;
			}

			EndModalLoop(IDCANCEL);
			return 0;
		}
		else if( nCmdShow == WM_COMMAND )
		{
			m_cDeleteThis = 1;
			EndModalLoop(IDOK);
			return 0;
		}
		else if(nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE)
		{
			m_nFlags &= ~WF_CONTINUEMODAL;
			m_nFlags &= ~WF_MODALLOOP;

			ShowWindow(nCmdShow);



			//SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
			//		SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
		else if( (nCmdShow >= SW_SHOWNORMAL && nCmdShow <= SW_SHOW) )
		{
			m_nFlags = WF_CONTINUEMODAL;

			//CWinApp* pApp = AfxGetApp();
			//if (pApp != NULL)
			//	pApp->EnableModeless(FALSE);
			//
			//// find parent HWND
			//HWND hWndParent = NULL;
			//BOOL bEnableParent = FALSE;
			//if (hWndParent != NULL && ::IsWindowEnabled(hWndParent))
			//{
			//	::EnableWindow(hWndParent, FALSE);
			//	bEnableParent = TRUE;
			//}

			m_nModalResult = -1;

			try
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				//if (GetStyle() & DS_NOIDLEMSG)
				dwFlags |= MLF_NOIDLEMSG;

				ShowWindow(nCmdShow);
				
				m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);

// 				this->PostMessage(HS_OPEN_DATA_TIP_MSG,(WPARAM)HS_OPEN_DATA_TIP_MSG,
// 					(LPARAM)dwFlags);

				this->SetFocus();

				//VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}
			catch(...)
			{
			}

		/*	if (bEnableParent)
				::EnableWindow(hWndParent, TRUE);
			if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
				::SetActiveWindow(hWndParent);*/

			if( m_cDeleteThis == 1 )
			{
				if( ::IsWindow(m_hWnd) )
				{
					DestroyWindow();
				}
			}

		}
		return 0;
	}

	m_nFlags = WF_CONTINUEMODAL;
	return DoModal();
}

BOOL CHSBaseDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == 0x4d)
	{
		// Magical mystical MFC F1 Help msg!
		if (GetKeyState(VK_SHIFT) >= 0)
		{
			// Shift key not down
			// Supply general dialog level help
			// 代码被注释 熊钟彬 不允许打开hlp文件查找
			// OnCommandHelp(0, 0);
			
			return(TRUE);		// Eat it
		}
	}
	// 快键
	else if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST )
	{
		// translate accelerators for frame and any children
		if (m_hAccelTable != NULL &&
			::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CHSBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	//return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
#ifndef _SUPPORT_CHANGE_CTLCOLOR
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
#else
	switch (nCtlColor)
	{	   
	case CTLCOLOR_STATIC:
		{
			pDC->SetTextColor(globalData.crStaticFgClr);
			pDC->SetBkColor(globalData.crStaticBkClr);
		}
		return globalData.brushStaticFgClr;
	case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(globalData.crEditFgClr);
			pDC->SetBkColor(globalData.crEditBkClr);
		}
		return globalData.brushEditFgClr;
	case CTLCOLOR_LISTBOX:
		{
			pDC->SetTextColor(globalData.crListBoxFgClr);
			pDC->SetBkColor(globalData.crListBoxBkClr);
		}
		return globalData.brushListBoxFgClr;
	case CTLCOLOR_SCROLLBAR:
		{
			pDC->SetTextColor(globalData.crScrollBarFgClr);
			pDC->SetBkColor(globalData.crScrollBarBkClr);
		}
		return globalData.brushScrollBarFgClr;
	case CTLCOLOR_BTN:
		{
			pDC->SetTextColor(globalData.crButtunFgClr);
			pDC->SetBkColor(globalData.crButtunBkClr);
		}
		return globalData.brushButtunFgClr;
	case CTLCOLOR_DLG:	    
		return globalData.m_Brush;
	}

	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
#endif
}

void CHSBaseDialog::OnNcPaint() 
{
	if( g_CImageListEx_AutoDeleteMemory.m_CloseImages == NULL )
	{
		g_CImageListEx_AutoDeleteMemory.m_CloseImages = new CImageList;
		g_CImageListEx_AutoDeleteMemory.m_CloseImages->Create(IDB_CLOSE, 16, 0, RGB(0,128,128));
	}

	//CDialog::OnNcPaint();
	// If you only change the menu bar, let the framework draw 
	// the window first
	Default();

	DWORD dwStyle = GetStyle();
	DWORD dwStyleEx = GetExStyle();

	if( (WS_MINIMIZEBOX & dwStyle)
		&& (WS_MAXIMIZEBOX & dwStyle)
		&& (WS_SYSMENU & dwStyle)
		&& (dwStyleEx&WS_EX_CONTEXTHELP) )
	{
		if( g_CImageListEx_AutoDeleteMemory.m_CloseImages == NULL )
		{
			return;
		}
		CWindowDC dc(this);

		CRect rc;
		GetWindowRect(rc);

		long lOffset = GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYSIZE);

		rc.bottom = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
		rc.right = rc.right - rc.left - GetSystemMetrics(SM_CXSIZE)*3 - 2;
		rc.left = rc.right - GetSystemMetrics(SM_CXSIZE) - 1;
		rc.top = rc.bottom - GetSystemMetrics(SM_CYSIZE);//GetSystemMetrics(SM_CYFRAME) + lOffset;
		g_CImageListEx_AutoDeleteMemory.m_CloseImages->Draw(&dc, 0, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
       // dc.DrawFrameControl(rc, DFC_CAPTION, DFCS_CAPTIONHELP );
	}
	else
	{
		long debug = 0;
		return;
	}
}

void CHSBaseDialog::MyNcDestroy()
{

}

void CHSBaseDialog::PostNcDestroy()
{
	MyNcDestroy();

	if( m_pThis && *m_pThis )
	{
		*m_pThis = 0;
		delete this;
	}
	else
	{
		CDialog::PostNcDestroy();
	}
}

void CHSBaseDialog::OnOK()
{
	if( m_pThis && *m_pThis )
	{
		this->DestroyWindow();
	}
	else
	{
		CDialog::OnOK();
	}
}

void CHSBaseDialog::OnCancel()
{
	//// 向父窗口发送，我要kill自己
	//CWnd* pWnd = AfxGetMainWnd();
	//if( pWnd && pWnd->m_hWnd )
	//{
	//	::SendMessage(pWnd->m_hWnd,HX_USER_COMPILEDATA,38,(LPARAM)this);
	//}

	Delete();

	if( m_pThis && *m_pThis )
	{
		this->DestroyWindow();
	}
	else
	{
		CDialog::OnCancel();
	}
}

BOOL CHSBaseDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*= NULL*/)
{
	return CDialog::Create(lpszTemplateName, pParentWnd);
}

BOOL CHSBaseDialog::Create(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
{
	return CDialog::Create(nIDTemplate, pParentWnd);
}

BOOL CHSBaseDialog::CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd /*= NULL*/,
					void* lpDialogInit /*= NULL*/)
{
	return CDialog::CreateIndirect(lpDialogTemplate, pParentWnd ,lpDialogInit);
}

BOOL CHSBaseDialog::CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd /*= NULL*/)
{
	return CDialog::CreateIndirect(hDialogTemplate, pParentWnd);
}

int CHSBaseDialog::RunModalLoop(DWORD dwFlags)
{
#ifdef VC_60
	return CDialog::RunModalLoop(dwFlags);
#else

	ASSERT(::IsWindow(m_hWnd)); // window must be created
	ASSERT(!(m_nFlags & WF_MODALLOOP)); // window must not already be in modal state

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	BOOL bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
	HWND hWndParent = ::GetParent(m_hWnd);
	m_nFlags |= (WF_MODALLOOP|WF_CONTINUEMODAL);
	MSG *pMsg = AfxGetCurrentMessage();

	try
	{

	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		//if( !ContinueModal() )
		//	goto ExitModal;

		//MSG msg;
		//VERIFY(::GetMessage(&msg, NULL, 0, 0));

		// PreTranslateMessage(&msg);
		// //::TranslateMessage(&msg);
		// ::DispatchMessage(&msg);

		ASSERT(ContinueModal());

		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			ASSERT(ContinueModal());

			// show the dialog when the message queue goes idle
			if (bShowIdle)
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			// call OnIdle while in bIdle state
			if (!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
			{
				// send WM_ENTERIDLE to the parent
				::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
			}
			if ((dwFlags & MLF_NOKICKIDLE) ||
				!SendMessage(WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
			{
				// stop idle processing next time
				bIdle = FALSE;
			}
		}

		// phase2: pump messages while available
		do
		{
			//ASSERT(ContinueModal());
			
			// pump message, but quit on WM_QUIT
			if (!AfxPumpMessage())
			{
				AfxPostQuitMessage(0);
				return -1;
			}

			// show the window when certain special messages rec'd
			if (bShowIdle &&
				(pMsg->message == 0x118 || pMsg->message == WM_SYSKEYDOWN))
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			if (!ContinueModal())
				goto ExitModal;

			// reset "no idle" state after pumping "normal" message
			if (AfxIsIdleMessage(pMsg))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} 
		while (::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
	}
	
	}
	catch(...)
	{
	}

ExitModal:
	m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);

#endif

	/*hWndParent = CWnd::GetSafeOwner_(m_pParentWnd->GetSafeHwnd(), &m_hWndTop);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		::SetActiveWindow(hWndParent);
	}*/

	return m_nModalResult;
}

void CHSBaseDialog::ExitRunModalLoop(DWORD dwFlags /*= 0*/)
{
	m_nFlags = 0;

	if( this->m_hWnd )
        SendMessage(WM_NULL);
}


INT_PTR CHSBaseDialog::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	// can be constructed with a resource template or InitModalIndirect
	ASSERT(m_lpszTemplateName != NULL || m_hDialogTemplate != NULL ||
		m_lpDialogTemplate != NULL);

	// load resource as necessary
	LPCDLGTEMPLATE lpDialogTemplate = m_lpDialogTemplate;
	HGLOBAL hDialogTemplate = m_hDialogTemplate;
	HINSTANCE hInst = AfxGetResourceHandle();
	if (m_lpszTemplateName != NULL)
	{
		hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
		HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
		hDialogTemplate = LoadResource(hInst, hResource);
	}
	if (hDialogTemplate != NULL)
		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

	// return -1 in case of failure to load the dialog template resource
	if (lpDialogTemplate == NULL)
		return -1;

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);
		if (CreateDlgIndirect(lpDialogTemplate,
						CWnd::FromHandle(hWndParent), hInst))
		{
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
					dwFlags |= MLF_NOIDLEMSG;
				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
					SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

	if (bEnableParent)
		::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary
	if (m_lpszTemplateName != NULL || m_hDialogTemplate != NULL)
		UnlockResource(hDialogTemplate);
	if (m_lpszTemplateName != NULL)
		FreeResource(hDialogTemplate);

	return m_nModalResult;

	//return CDialog::DoModal();
}

BOOL CHSBaseDialog::ShowWindow( int nCmdShow )
{
//	return CDialog::ShowWindow(nCmdShow);

	if( m_cDeleteThis == 2 )
		return CDialog::ShowWindow(nCmdShow);

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CDialog::ShowWindow(nCmdShow);

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( IsIconic() )
	{
		return CDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		/*CString strError;
		strError.Format("函数 \"BOOL CHSBaseDialog::ShowWindow( int nCmdShow )\",MFC标准返回的错误号: %i。",
			GetLastError());
		CHSTools::WriteError(strError);*/
		return CDialog::ShowWindow(nCmdShow);
	}

	return 1;//CDialog::ShowWindow(nCmdShow);
}

BOOL CHSBaseDialog::OnNcActivate(BOOL bActive)
{
	OnNcPaint();
	return TRUE;//CDialog::OnNcActivate(bActive);
}


void CHSBaseDialog::DeleteAllBut()
{
		CButton* pButton;
		for( int i = 0; i < m_pArrayBut.GetSize(); i++ )
		{
			pButton = m_pArrayBut.GetAt(i);
			if( pButton->IsKindOf(RUNTIME_CLASS(CHSShadeButton)) )
			{
				pButton->Detach();
				delete (CHSShadeButton*)pButton;
			}
		}
		m_pArrayBut.RemoveAll();
	
}

CButton* CHSBaseDialog::AddButton(int nID,int nImage /*= -1*/,CString strTip /*= ""*/,
									  long lMask /*= 0*/)
{
//	CWnd* pWnd = this->GetDlgItem(nID);
	HWND pWnd = (HWND)nID;
	if( pWnd )
	{
		if( lMask != 0 )
		{
			CHSShadeButton* pButton = new CHSShadeButton;

			pButton->Attach(pWnd);
			DWORD style = pButton->GetStyle();
			style |= BS_OWNERDRAW;
			SetWindowLong(pButton->m_hWnd,GWL_STYLE,style);

			m_pArrayBut.Add(pButton);

			pButton->SetShade(lMask);

			if( !strTip.IsEmpty() )
			{
				pButton->SetToolTipText(strTip);
			}
			return pButton;
		}
		else
		{
			CButton* pButton = new CButton;

			pButton->Attach(pWnd);
			DWORD style = pButton->GetStyle();
			style |= BS_OWNERDRAW;
			SetWindowLong(pButton->m_hWnd,GWL_STYLE,style);

			m_pArrayBut.Add(pButton);

			return pButton;
		}
	}

	return NULL;
}

void CHSBaseDialog::DelButton(int nID)
{
}


void CHSBaseDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	this->OnNcPaint();
}
