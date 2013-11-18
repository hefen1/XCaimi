// WndKeyboard.cpp : implementation file
//

#include "stdafx.h"
#include "WndKeyboard.h"
#include "resource.h"
#include "InPlaceEdit.h"
#include "KeyboardDataList.h"
#include "GeneralHelper.h"
#include <atlbase.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndKeyboard

#define DETASTEP  8
#define LeftWidth 8
#define DispWidth 2


extern CString		  m_strKey;
extern CString		  m_OldstrKey;
extern HWND			  g_hEditUpdateParentWnd;
extern BOOL           g_bCharReturn;
static CWnd* m_pPreFocusWnd = NULL;
static char	 m_bHaveOneShow = FALSE;

static int	 m_nTopHeight  = 15;
static int	 m_nRightWidth = 15;

#define EDIT_HEIGHT	   16 
#define CONTROL_TREEID 100
#define CONTROL_LISTID 101
#define CONTROL_EDITID 102
#define CONTROL_SHOWBUTTONID  103
#define CONTROL_CLOSEBUTTONID 104

HHOOK CWndKeyboard::m_hookProc = NULL;
CWndKeyboard::CWndKeyboard()
{
	m_pFont = NULL;

	m_nAlign	  = CWndKeyboard::left;
	m_nOldAlign	  = CWndKeyboard::none;
	m_bOperator	  = FALSE;
	m_bMoveWindow = FALSE;
	m_pBrush	  = NULL;
	m_dwStyle     = DS_AUTOSHOWHIDE;//DS_ALWAYSSHOW;
	m_bEnter      = FALSE;

	m_oldRect.SetRectEmpty();
	m_floatRect.SetRectEmpty();


	m_pAlwaysShow  = NULL;
	m_pCloseBtn    = NULL;
	m_pKeyboard    = NULL;
	m_pEdit        = NULL;

	Initialise(_T("CWndKeyboard"));
}

CWndKeyboard::~CWndKeyboard()
{
	if( m_hookProc != NULL )
	{
		::UnhookWindowsHookEx(m_hookProc);
		m_hookProc = NULL;
	}

	if( m_pFont )
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	if ( m_pEditFont )
	{
		delete m_pEditFont;
		m_pEditFont = NULL;
	}
	if(m_pBrush != NULL)
	{
		m_pBrush->DeleteObject();
		delete m_pBrush;
		m_pBrush = NULL;
	}
	if(m_pKeyboard != NULL)
	{
		m_pKeyboard->DestroyWindow();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}
	if(m_pEdit != NULL)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
 	}
	if ( m_pAlwaysShow != NULL)
	{
		m_pAlwaysShow->DestroyWindow();
		delete m_pAlwaysShow;
		m_pAlwaysShow = NULL;
	}
	if ( m_pCloseBtn != NULL )
	{	m_pCloseBtn->DestroyWindow();
		delete m_pCloseBtn;
		m_pCloseBtn = NULL;
	}
}


BEGIN_MESSAGE_MAP(CWndKeyboard, CWnd)
	//{{AFX_MSG_MAP(CWndKeyboard)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_WM_TIMER()
	ON_WM_MOVING()
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONUP()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP

	ON_EN_CHANGE(CONTROL_EDITID, OnChangeEdit)
	ON_BN_CLICKED(CONTROL_SHOWBUTTONID, OnShowButton)
	ON_BN_CLICKED(CONTROL_CLOSEBUTTONID, OnCloseButton)
	ON_MESSAGE(hxUSER_EDITMSG,OnEditMsg)

	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndKeyboard message handlers

int CWndKeyboard::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pFont = CGeneralHelper::CreateFont("宋体",-13,FONT_BOLD);
	this->SetFont(m_pFont);

	CRect rect;
	rect.SetRectEmpty ();

	DWORD dwViewStyle;

 	m_pAlwaysShow = new CKeyButton;
 	m_pAlwaysShow->Create("",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |BS_OWNERDRAW,CRect(1,1,m_nTopHeight-4,m_nTopHeight-4),this,CONTROL_SHOWBUTTONID);
	HBITMAP hbmp = (HBITMAP)::LoadImage(GetModuleHandle(_T("KeyBoard.dll")),MAKEINTRESOURCE(IDB_SHOWKEYBTNUP),
 		IMAGE_BITMAP,0, 0,LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
	m_pAlwaysShow->SetImage(hbmp);
	m_pAlwaysShow->ShowWindow(SW_SHOW);
 
	m_pCloseBtn = new CKeyButton;
 	m_pCloseBtn->Create("",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|BS_OWNERDRAW ,CRect(0,0,m_nTopHeight-5,m_nTopHeight-5),this,CONTROL_CLOSEBUTTONID);
	hbmp = (HBITMAP)::LoadImage(GetModuleHandle(_T("KeyBoard.dll")),MAKEINTRESOURCE(IDB_CLOSEBTN),
		IMAGE_BITMAP,0, 0,	LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
	m_pCloseBtn->SetImage(hbmp);
	m_pCloseBtn->ShowWindow(SW_SHOW);

	if( IsStyle(DS_BYTREE) )
	{		
		dwViewStyle = WS_CHILD|WS_VISIBLE|WS_BORDER|TVS_HASLINES|TVS_LINESATROOT|
			TVS_HASBUTTONS|TVS_SHOWSELALWAYS|TVS_EDITLABELS; // TVS_TRACKSELECT
	}
	else if( IsStyle(DS_BYKEYBOARD) )
	{
		m_pKeyboard = new CKeyboardDataList;
		
		dwViewStyle = WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL /*| LBS_SORT*/ | LBS_USETABSTOPS;
		dwViewStyle |= LBS_OWNERDRAWFIXED; 
		dwViewStyle |= LBS_NOINTEGRALHEIGHT;
		dwViewStyle |=LBS_HASSTRINGS;

		m_pKeyboard->Create( dwViewStyle,rect,this,CONTROL_LISTID);
		
		m_pEdit = new CInPlaceEdit;
		m_pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_WANTRETURN | ES_UPPERCASE | ES_AUTOHSCROLL,rect,this,CONTROL_EDITID);
		m_pEdit->ModifyStyleEx(0,WS_EX_STATICEDGE,0); 
	
		m_pEditFont = CGeneralHelper::CreateFont("新宋体",-12,0);
		m_pKeyboard->SetFont(m_pEditFont);
		m_pEdit->SetFont(m_pEditFont);

		m_pEdit->SetLimitText(MAX_ENTER_CHARS);


		CSize size = m_pKeyboard->GetSize();
		MoveWindow(0,0,size.cx + 4,size.cy + EDIT_HEIGHT + m_nTopHeight);
		this->SetTimer((long)this,100,NULL);
	}

	return 0;
}

void CWndKeyboard::OnDestroy() 
{
	CWnd::OnDestroy();
	this->KillTimer((long)this);

	// TODO: Add your message handler code here
	
}

void CWndKeyboard::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnDropFiles(hDropInfo);
}

void CWndKeyboard::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if( IsStyle(DS_BYTREE) )
	{
		//CWnd::OnLButtonDown(nFlags, point);
		if( IsStyle(DS_ALWAYSSHOW) )
		{
			PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));	
		}
	}
	else
	{
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
	}
}

BOOL CWndKeyboard::MouseDown(UINT nFlags, CPoint point)
{	
	CRect rectMouseClip;
	GetClientRect(rectMouseClip);
  //  if ( !rectMouseClip.PtInRect(point) ) 
	//	return FALSE;	
	
	::SetCapture(m_hWnd);
//    ::ClipCursor(&rectMouseClip);	

	int     curX,curY;
	int     xDiff = 0,yDiff = 0;
	CRect   rectMove;
	rectMove = m_floatRect;
		
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			{
				curX = (int)(short)LOWORD(msg.lParam);
				curY = (int)(short)HIWORD(msg.lParam);				
			
				xDiff = curX - point.x;
				yDiff = curY - point.y;

				rectMove.OffsetRect(rectMouseClip.left + xDiff,rectMouseClip.top + yDiff);
				//if(msg.message == MK_LBUTTON)
				{
					AdjustPos(&rectMove);
					m_bMoveWindow = TRUE;
					MoveWindow(rectMove);
					m_bMoveWindow = FALSE;
					m_oldRect   = rectMove;
					m_floatRect = rectMove;
				}								
			}
	        break;
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:			
            goto ExitLoop;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
    ::ReleaseCapture();
//    ::ClipCursor(NULL);

	m_bOperator = FALSE;

	return TRUE;

}

void CWndKeyboard::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bOperator = FALSE;
	CWnd::OnLButtonUp(nFlags, point);
}

void CWndKeyboard::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);	
	// TODO: Add your message handler code here
	
}

void CWndKeyboard::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect,rcClient;
	GetClientRect(rcClient);
	if(!m_bMoveWindow)
	{
		GetWindowRect(rect);
		if( !rect.IsRectEmpty() )
		{
			m_floatRect = m_oldRect = rect;
		}
	}

	rect = rcClient;
	if( IsStyle(DS_BYTREE) )
	{
	}
	else if( IsStyle(DS_BYKEYBOARD) )
	{
		if( m_pKeyboard != NULL && 
			::IsWindow(m_pKeyboard->m_hWnd) )
		{
			rect.top	+= m_nTopHeight;
			rect.bottom -= EDIT_HEIGHT;
			m_pKeyboard->MoveWindow(rect);
			rect.bottom += EDIT_HEIGHT;
			rect.top     = rect.bottom - EDIT_HEIGHT;			
			m_pEdit->MoveWindow(rect);
		}
	}

 	if(m_pCloseBtn != NULL) // close
 	{
		
 		rcClient.left = rcClient.right - m_nRightWidth+3;
 		rcClient.bottom = rcClient.top + m_nTopHeight-2;
 		m_pCloseBtn->MoveWindow(rcClient);
 	}
}

void CWndKeyboard::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( IsStyle(DS_BYKEYBOARD) )
	{
		if(GetList() != NULL)
		{
			SendMessage(hxUSER_EDITMSG,nChar,(LPARAM)GetList());
		}
	}
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CWndKeyboard::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( IsStyle(DS_BYKEYBOARD) )
	{
		if(nChar == VK_ESCAPE)
		{
			if( !IsStyle(DS_ALWAYSSHOW) )
			{
				m_pPreFocusWnd = CWnd::FromHandle(g_hEditUpdateParentWnd);
				if( m_pPreFocusWnd )
					m_pPreFocusWnd->SetFocus();
				this->ShowWindow(SW_HIDE);
			}
			else
			{
				m_pEdit->SetWindowText("");
				m_OldstrKey.Empty();
				m_pEdit->SetFocus();
			}
			return;
		}
	}
	else if( IsStyle(DS_BYTREE) )
	{
		if(nChar == VK_ESCAPE)
		{
			m_pPreFocusWnd = CWnd::FromHandle(g_hEditUpdateParentWnd);
			if( m_pPreFocusWnd )
				m_pPreFocusWnd->SetFocus();
			return;
		}
		
		if(m_pPreFocusWnd != NULL)
		{
			m_pPreFocusWnd->PostMessage(WM_KEYDOWN,nChar,0);
		}
		else 
		{
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CWndKeyboard::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CClientDC clientDC(this);
	const int iGripperSize = 3;
	const int iGripperOffset = -1;
	const int iLinesNum = 2;
	
	CRect rectGripper;
 	GetClientRect(rectGripper);
 	rectGripper.bottom = rectGripper.top + m_nTopHeight;
 	clientDC.FillRect(rectGripper,m_pBrush);
	CRect rect = rectGripper;

	if( IsStyle(DS_BYKEYBOARD) )
	{
		CFont* oldFont;
		oldFont = clientDC.SelectObject(m_pFont);

		clientDC.SetBkMode(TRANSPARENT);
		clientDC.SetTextColor(RGB(0,0,255));
		clientDC.DrawText(_T("键盘精灵"),rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		clientDC.SelectObject(oldFont);
	}
	else
	{
		rectGripper.top += iGripperOffset;
		rectGripper.DeflateRect (2, 0);

		rectGripper.top = iGripperOffset + rectGripper.CenterPoint().y - 
			( iLinesNum*iGripperSize + (iLinesNum-1)) / 2;

		rectGripper.bottom = rectGripper.top + iGripperSize;

		for (int i = 0; i < iLinesNum; i ++)
		{
			clientDC.Draw3dRect(rectGripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),::GetSysColor(COLOR_BTNSHADOW));
			rectGripper.OffsetRect (0, iGripperSize+1);
		}
	}
	m_pCloseBtn->Invalidate(FALSE);
	m_pAlwaysShow->Invalidate(FALSE);
	// Do not call CWnd::OnPaint() for painting messages
}

void CWndKeyboard::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	
}

void CWndKeyboard::OnTimer(UINT nIDEvent) 
{
	if( !this->IsWindowVisible() )
		return;

	if( IsWindowVisible() &&
		IsStyle(DS_BYKEYBOARD) &&
		!IsStyle(DS_ALWAYSSHOW) &&
		IsStyle(DS_AUTOSHOWHIDE) )
	{		
		CWnd* pWnd = GetFocus();
		if( pWnd != this &&
			pWnd != m_pKeyboard &&
			pWnd != m_pEdit &&
 			pWnd != m_pAlwaysShow &&
 			pWnd != m_pCloseBtn)
		{
			m_pEdit->SetWindowText("");
			m_strKey = m_OldstrKey = "";
			ShowWindow(SW_HIDE);
			return;
		}
	}

 	if( IsStyle(DS_NotBerth) )
 	{
 		return;
 	}

	if( m_bOperator )
		return;

	CRect  rect;
	CPoint point;
	GetWindowRect(rect);
	::GetCursorPos(&point);
	if( !rect.PtInRect(point) )
	{
		if( !IsWindowVisible() )
		{
			ShowWindow(SW_SHOW);
		}
		SetSize(FALSE);

		m_bEnter = FALSE;
	}
	else if( !m_bEnter && m_nAlign != CWndKeyboard::none )
	{
		SetSize(TRUE);

		m_bEnter = TRUE;
	}


	CWnd::OnTimer(nIDEvent);
}

void CWndKeyboard::OnMoving(UINT fwSide, LPRECT pRect) 
{
	CWnd::OnMoving(fwSide, pRect);	
}

void CWndKeyboard::OnMove(int x, int y) 
{
	CWnd::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

void CWndKeyboard::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnMouseMove(nFlags, point);
}

void CWndKeyboard::Initialise(CString strClass)
{
	if(m_pBrush != NULL)
	{
		delete m_pBrush;
		m_pBrush = NULL;
	}
	m_pBrush = new CBrush(GetSysColor(COLOR_BTNFACE));//::GetSysColor(COLOR_BACKGROUND/*COLOR_INFOBK*/));
	
	HINSTANCE hInst = AfxGetInstanceHandle( );

	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
    wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
    wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
    wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = *m_pBrush;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = strClass;

    if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();
}

BOOL CWndKeyboard::Create(CWnd* pParentWnd,CRect rect)
{
	m_floatRect = m_oldRect = rect;
	return CWnd::CreateEx(WS_EX_LEFT | WS_EX_TOOLWINDOW,
                           _T("CWndKeyboard"),
                           NULL,
                           WS_POPUP | WS_BORDER | /*WS_CLIPSIBLINGS | WS_VISIBLE |*/ WS_THICKFRAME,
                           rect.left,rect.top,rect.Width(),rect.Height(),
						   pParentWnd==NULL?NULL:pParentWnd->m_hWnd, 
                           NULL);
}

CWndKeyboard::Align CWndKeyboard::AdjustPos(CRect* lpRect)
{
	CWnd* pMainWnd = GetParent();
	if(pMainWnd == NULL)
	{
		return CWndKeyboard::none;
	}

	RECT rWorkArea;
	pMainWnd->GetWindowRect(&rWorkArea);

	CRect rcWA;
	rcWA = rWorkArea;
 
	int iX = lpRect->left;
	int iY = lpRect->top;
 
	if( iX < rcWA.left + DETASTEP && iX != rcWA.left )
	{
		//调整左
		lpRect->OffsetRect(rcWA.left-iX,0);
		//AdjustPos(lpRect);
		return CWndKeyboard::left;
	}
	if( iY < rcWA.top + DETASTEP && iY != rcWA.top )
	{
		//调整上
		lpRect->OffsetRect(0,rcWA.top-iY);
		//AdjustPos(lpRect);
		return CWndKeyboard::top;
	}
	if( iX + lpRect->Width() > rcWA.right - DETASTEP && 
		iX != rcWA.right - lpRect->Width() )
	{
		//调整右
		lpRect->OffsetRect(rcWA.right-lpRect->right,0);
		//AdjustPos(lpRect);
		return CWndKeyboard::right;
	}
	if(iY + lpRect->Height() > rcWA.bottom - DETASTEP && iY !=rcWA.bottom-lpRect->Height())
	{ 
		//调整下
		//pWnd->SetWindowPos(NULL ,iX,rcWA.bottom-rcW.Height(),0,0,SWP_NOSIZE);
		lpRect->OffsetRect(0,rcWA.bottom-lpRect->bottom);
		return CWndKeyboard::bottom;
	}

	m_nAlign = CWndKeyboard::none;

	return CWndKeyboard::none;
}

BOOL CWndKeyboard::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CWndKeyboard::SetSize(BOOL bRestore)
{
	try
	{		
		if(bRestore)
		{
			if(m_bHaveOneShow)
				return;
			
			m_bHaveOneShow |= 0x1;
			

			m_bOperator	   = TRUE;

			m_pPreFocusWnd = this->GetFocus();
			if( /*(m_pPreFocusWnd == m_pContentTree) || */
				(m_pPreFocusWnd == this) )
			{
				m_pPreFocusWnd = CWnd::FromHandle(g_hEditUpdateParentWnd); 
			}
			
			m_bMoveWindow = TRUE;
			switch(m_nAlign)
			{
			case CWndKeyboard::left:
				if(m_oldRect.Width() <= LeftWidth*2)
				{
					m_oldRect.right = m_oldRect.left + LeftWidth*4;
				}
				break;
			case CWndKeyboard::top:
				if(m_oldRect.Height() <= LeftWidth*2)
				{
					m_oldRect.bottom = m_oldRect.top + LeftWidth*4;
				}
				break;
			case CWndKeyboard::right:
				if(m_oldRect.Width() <= LeftWidth*2)
				{
					m_oldRect.left = m_oldRect.right - LeftWidth*4;
				}
				break;
			case CWndKeyboard::bottom:
				if(m_oldRect.Height() <= LeftWidth*2)
				{
					m_oldRect.top = m_oldRect.bottom - LeftWidth*4;
				}
				break;
			}	
			RestoreWindow();
			BringWindowToTop();
			m_bMoveWindow = FALSE;
			m_bOperator   = FALSE;

			m_nOldAlign = m_nAlign;
			return;
		}
		m_bHaveOneShow &= ~(0x1);
		

		if(m_pPreFocusWnd != NULL)
		{
			if(::IsWindow(m_hWnd))
			{
				m_pPreFocusWnd->SetFocus();
			}
			m_pPreFocusWnd = NULL;
		}
		
		CRect rect;
		CRect parentRect;
		CWnd* pWnd = this->GetParent();
		GetWindowRect(rect);
		pWnd->GetWindowRect(parentRect);	
		Align nAlign;
		if( rect.left > parentRect.left - LeftWidth/2 &&
			rect.left < parentRect.left + LeftWidth/2 )
		{
			nAlign = CWndKeyboard::left;
		}
		else if( rect.top > parentRect.top - LeftWidth/2 &&
			rect.top < parentRect.top + LeftWidth/2 )
		{
			nAlign = CWndKeyboard::top;
		}
		else if( rect.right > parentRect.right - LeftWidth/2 &&
			rect.right < parentRect.right + LeftWidth/2 )
		{
			nAlign = CWndKeyboard::right;
		}
		else if( rect.bottom > parentRect.bottom - LeftWidth/2 &&
			rect.bottom < parentRect.bottom + LeftWidth/2 )
		{
			nAlign = CWndKeyboard::bottom;
		}
		else
		{
			nAlign = CWndKeyboard::none;
		}
		
		if(nAlign != CWndKeyboard::none)
		{
			m_nAlign = nAlign;
		}
		
		CRect oldRect = m_floatRect;
		int nPos1;
		switch(m_nAlign)
		{
		case CWndKeyboard::left:
			{
				rect.left  = parentRect.left;
				rect.right = parentRect.left + DispWidth;
				
				m_oldRect.left   = rect.left;
				m_oldRect.right  = m_oldRect.left + oldRect.Width();			
				if( IsStyle(DS_DependenceParent) )
				{
					rect.top = parentRect.top + LeftWidth;
					rect.bottom = parentRect.bottom - LeftWidth;
					m_oldRect.top    = parentRect.top + LeftWidth;
					m_oldRect.bottom = parentRect.bottom - LeftWidth;
				}
				else
				{
					nPos1 = rect.top - parentRect.top;			
					if(nPos1 < 0)
					{
						rect.OffsetRect(0,-nPos1 + LeftWidth);
						m_floatRect.OffsetRect(0,-nPos1 + LeftWidth);
						m_oldRect.OffsetRect(0,-nPos1 + LeftWidth);
					}
					nPos1 = rect.bottom - parentRect.bottom;
					if(nPos1 > 0)
					{
						rect.OffsetRect(0,-nPos1 - LeftWidth);
						m_floatRect.OffsetRect(0,-nPos1 - LeftWidth);
						m_oldRect.OffsetRect(0,-nPos1 - LeftWidth);
					}
					if( rect.Height() < m_floatRect.Height() )
					{
						rect.top = parentRect.top + LeftWidth;
						rect.bottom = rect.top + m_floatRect.Height();
						m_oldRect.top = rect.top;
						m_oldRect.bottom = m_oldRect.bottom;
					}
					if(rect.Height() > parentRect.Height())
					{
						rect.top  = parentRect.top + LeftWidth;
						rect.bottom = parentRect.bottom - LeftWidth;
						m_oldRect.top  = rect.top;
						m_oldRect.bottom = rect.bottom;
					}
				}
				
				m_floatRect.left  = m_oldRect.left;
				m_floatRect.right = m_oldRect.right;
			}
			break;
		case CWndKeyboard::top:
			{
				rect.top    = parentRect.top;
				rect.bottom = parentRect.top + DispWidth;
				
				m_oldRect.top    = rect.top;
				m_oldRect.bottom = m_oldRect.top + oldRect.Height();
				if( IsStyle(DS_DependenceParent) )
				{
					rect.left  = parentRect.left + LeftWidth;
					rect.right = parentRect.right - LeftWidth;
					m_oldRect.left   = parentRect.left + LeftWidth;
					m_oldRect.right  = parentRect.right - LeftWidth;
				}
				else
				{
					nPos1 = rect.left  - parentRect.left;				
					if(nPos1 < 0)
					{
						rect.OffsetRect(-nPos1 + LeftWidth,0);
						m_floatRect.OffsetRect(-nPos1 + LeftWidth,0);
						m_oldRect.OffsetRect(-nPos1 + LeftWidth,0);
					}
					nPos1 = rect.right - parentRect.right;
					if(nPos1 > 0)
					{
						rect.OffsetRect(-nPos1 - LeftWidth,0);
						m_floatRect.OffsetRect(-nPos1 - LeftWidth,0);
						m_oldRect.OffsetRect(-nPos1 - LeftWidth,0);
					}
					if( rect.Width() < m_floatRect.Width() )
					{
						rect.left  = parentRect.left + LeftWidth;
						rect.right = rect.left + m_floatRect.Width();
						m_oldRect.left  = rect.left;
						m_oldRect.right = rect.right;
					}
					if(rect.Width() > parentRect.Width())
					{
						rect.left  = parentRect.left + LeftWidth;
						rect.right = parentRect.right - LeftWidth;
						m_oldRect.left  = rect.left;
						m_oldRect.right = rect.right;
					}
				}
				
				m_floatRect.top    = m_oldRect.top;
				m_floatRect.bottom = m_oldRect.bottom;
			}
			break;
		case CWndKeyboard::right:
			{
				rect.right = parentRect.right - GetSystemMetrics(SM_CXEDGE)*3;
				rect.left  = rect.right - DispWidth;
				
				m_oldRect.right  = rect.right;
				m_oldRect.left   = rect.right - oldRect.Width();
				if( IsStyle(DS_DependenceParent) )
				{
					rect.top = parentRect.top + LeftWidth;
					rect.bottom = parentRect.bottom - LeftWidth;
					m_oldRect.top = parentRect.top + LeftWidth;
					m_oldRect.bottom = parentRect.bottom - LeftWidth;
				}
				else
				{
					nPos1 = rect.top - parentRect.top;			
					if(nPos1 < 0)
					{
						rect.OffsetRect(0,-nPos1 + LeftWidth);
						m_floatRect.OffsetRect(0,-nPos1 + LeftWidth);
						m_oldRect.OffsetRect(0,-nPos1 + LeftWidth);
					}
					nPos1 = rect.bottom - parentRect.bottom;
					if(nPos1 > 0)
					{
						rect.OffsetRect(0,-nPos1 - LeftWidth);
						m_floatRect.OffsetRect(0,-nPos1 - LeftWidth);
						m_oldRect.OffsetRect(0,-nPos1 - LeftWidth);
					}
					if( rect.Height() < m_floatRect.Height() )
					{
						rect.top = parentRect.top + LeftWidth;
						rect.bottom = rect.top + m_floatRect.Height();
						m_oldRect.top = rect.top;
						m_oldRect.bottom = m_oldRect.bottom;
					}
					if(rect.Height() > parentRect.Height())
					{
						rect.top  = parentRect.top + LeftWidth;
						rect.bottom = parentRect.bottom - LeftWidth;
						m_oldRect.top  = rect.top;
						m_oldRect.bottom = rect.bottom;
					}
				}
				
				m_floatRect.left  = m_oldRect.left;
				m_floatRect.right = m_oldRect.right;
			}
			break;
		case CWndKeyboard::bottom:
			{
				rect.bottom = parentRect.bottom - GetSystemMetrics(SM_CYEDGE)*3;
				rect.top    = rect.bottom - DispWidth;
				
				m_oldRect.bottom = rect.bottom;
				m_oldRect.top	 = rect.bottom - oldRect.Height();
				if( IsStyle(DS_DependenceParent) )
				{
					rect.left  = parentRect.left + LeftWidth;
					rect.right = parentRect.right - LeftWidth;
					m_oldRect.left   = parentRect.left + LeftWidth;
					m_oldRect.right = parentRect.right - LeftWidth;
				}			
				else
				{
					nPos1 = rect.left  - parentRect.left;				
					if(nPos1 < 0)
					{
						rect.OffsetRect(-nPos1 + LeftWidth,0);
						m_floatRect.OffsetRect(-nPos1 + LeftWidth,0);
						m_oldRect.OffsetRect(-nPos1 + LeftWidth,0);
					}
					nPos1 = rect.right - parentRect.right;
					if(nPos1 > 0)
					{
						rect.OffsetRect(-nPos1 - LeftWidth,0);
						m_floatRect.OffsetRect(-nPos1 - LeftWidth,0);
						m_oldRect.OffsetRect(-nPos1 - LeftWidth,0);
					}
					if( rect.Width() < m_floatRect.Width() )
					{
						rect.left = parentRect.left + LeftWidth;
						rect.right = rect.left + m_floatRect.Width();
						m_oldRect.left  = rect.left;
						m_oldRect.right = rect.right;
					}
					if(rect.Width() > parentRect.Width())
					{
						rect.left  = parentRect.left + LeftWidth;
						rect.right = parentRect.right - LeftWidth;
						m_oldRect.left  = rect.left;
						m_oldRect.right = rect.right;
					}
				}
				
				m_floatRect.top    = m_oldRect.top;
				m_floatRect.bottom = m_oldRect.bottom;
			}
			break;
		default:
			return;
		}
		m_bMoveWindow = TRUE;
		MoveWindow(rect);	
		m_bMoveWindow = FALSE;
	}
	catch(...)
	{
	}
}

void CWndKeyboard::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CWndKeyboard::OnNcRButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CWnd::OnNcRButtonUp(nHitTest, point);
}

void CWndKeyboard::RestoreWindow()
{
	CRect rect,oldRect;
	GetWindowRect(rect);
	oldRect = rect;
	if(rect == m_oldRect)
		return;

	float nPos;
	float nStep;
	float fSpeed = 100.0;
	switch(m_nAlign)
	{
	case CWndKeyboard::left:
		{
			nStep = m_oldRect.Width()/fSpeed;
			for(nPos = 0; nPos < m_oldRect.Width(); nPos += nStep )
			{
				rect.SetRect(m_oldRect.left,m_oldRect.top,
					m_oldRect.left+(int)nPos,m_oldRect.bottom);
				MoveWindow(rect);
			}
			m_oldRect.left = oldRect.left;
		}
		break;
	case CWndKeyboard::top:
		{
			nStep = m_oldRect.Height()/fSpeed;
			for(nPos = 0; nPos < m_oldRect.Height(); nPos += nStep )
			{
				rect.SetRect(m_oldRect.left,m_oldRect.top,
					m_oldRect.right,m_oldRect.top+(int)nPos);
				MoveWindow(rect);
			}
			m_oldRect.top = oldRect.top;
		}
		break;
	case CWndKeyboard::right:
		{
			nStep = m_oldRect.Width()/fSpeed;
			for(nPos = 0; nPos < m_oldRect.Width(); nPos += nStep )
			{
				rect.SetRect(m_oldRect.right-(int)nPos,m_oldRect.top,
						m_oldRect.right,m_oldRect.bottom);
				MoveWindow(rect);
			}
			m_oldRect.right = oldRect.right;
		}
		break;
	case CWndKeyboard::bottom:
		{
			nStep = m_oldRect.Height()/fSpeed;
			for(nPos = 0; nPos < m_oldRect.Height(); nPos += nStep )
			{
				rect.SetRect(m_oldRect.left,m_oldRect.bottom-(int)nPos,
					m_oldRect.right,m_oldRect.bottom);
				MoveWindow(rect);
			}
			m_oldRect.bottom = oldRect.bottom;
		}
		break;
	}
	
	MoveWindow(m_oldRect);	
	
}

void CWndKeyboard::OnChangeEdit()
{
	CString strText;
	 // 添加外部字符串
// 	if( strlen(m_sHSShowKeyboardInfo.m_cResever) > 0 )
// 	{
// 		CString str = CString(m_sHSShowKeyboardInfo.m_cResever,sizeof(m_sHSShowKeyboardInfo.m_cResever));
// 		//if( strText.IsEmpty() )
// 		{
// 			memset(m_sHSShowKeyboardInfo.m_cResever,0,sizeof(m_sHSShowKeyboardInfo.m_cResever));
// 			m_pEdit->SetWindowText(str);	
// 		}
// 	}

	m_pEdit->GetWindowText(strText);
	if(m_pKeyboard == NULL)
		return;
	m_pKeyboard->CreateDataThread(strText);
}


LRESULT CALLBACK CWndKeyboard::KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
	MSG* pMessage = (MSG*)lParam;
	if( pMessage != NULL )
	{
		if( pMessage->message >= WM_KEYFIRST && pMessage->message <= WM_KEYLAST )
		{
// 			if( pMessage->wParam == VK_RETURN && m_pFormulaMan != NULL )
// 			{
// 				CWndKeyboard* pCWndKeyboard = m_pFormulaMan->GetKeyboard();
// 				if( pCWndKeyboard != NULL && pCWndKeyboard->IsWindowVisible() )
// 				{
// 					CKeyboardDataList* pCKeyboardDataList = pCWndKeyboard->GetKeyboard();
// 					if( pCKeyboardDataList )
// 					{
// 						pCKeyboardDataList->SendMessage(WM_KEYDOWN,pMessage->wParam,pMessage->lParam);
// 						return 0;
// 					}
// 				}
			//}
		}
	}

    // chain to the next hook procedure
    return ::CallNextHookEx(m_hookProc, code, wParam, lParam);
}

LRESULT CWndKeyboard::OnEditMsg(WPARAM wParam, LPARAM lParam)
{
	if( wParam == VK_ESCAPE )
	{
		m_pEdit->SetWindowText("");
		m_OldstrKey.Empty();
		m_pPreFocusWnd = CWnd::FromHandle(g_hEditUpdateParentWnd);
		if( m_pPreFocusWnd )
			m_pPreFocusWnd->SetFocus();

		if( !IsStyle(DS_ALWAYSSHOW) )
		{
			ShowWindow(SW_HIDE);
		}
		else
		{
			m_pEdit->SetFocus();
		}
		return 1;
	}

	CWnd* pWnd = (CWnd*)lParam;
	if(pWnd == m_pEdit)
	{
		if(wParam == VK_RETURN)
		{	
			g_bCharReturn = TRUE;
			m_pEdit->SetWindowText("");			
			return 1;
		}
		else if( wParam == VK_PRIOR || wParam == VK_NEXT ||
				 wParam == VK_DOWN  || wParam == VK_UP   )
		{
			if( m_pKeyboard->GetCount() <= 0 )
				return 0;
			m_pKeyboard->SetFocus();
			m_pKeyboard->SendMessage(WM_KEYDOWN,wParam,0);
			return 1;
		}
		else if (wParam == VK_BACK)
		{
			CString strText;
			m_pEdit->GetWindowText(strText);
			if ( strText.GetLength() <= 1)
			{
// 				if ( m_pKeyboard )
// 					m_pKeyboard->RemoveArray();
				m_OldstrKey.Empty();
				m_strKey.Empty();
				ShowWindow(SW_HIDE);
				if(g_hEditUpdateParentWnd != NULL)
				{
					::SetFocus(g_hEditUpdateParentWnd);
				}
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else if(pWnd == m_pKeyboard)
	{	
		if(wParam == VK_RETURN)
		{
			m_pEdit->SetWindowText("");
			return 1;
		}
		if( ((CInPlaceEdit*)m_pEdit)->GetLastChar() == VK_RETURN)
		{
			m_pEdit->SetWindowText("");
		}

		if( wParam == VK_PRIOR || wParam == VK_NEXT ||
			wParam == VK_END   || wParam == VK_HOME ||
			wParam == VK_UP    || wParam == VK_DOWN)
		{
		}
		else if( wParam == VK_LEFT || wParam == VK_RIGHT )
		{
			m_pEdit->SetFocus();
		}
		else if( wParam == 0xA || wParam == 0xB )
		{
			;
		}
		else
		{
			CString strChar;
			if (wParam >= VK_NUMPAD0 && wParam<= VK_NUMPAD9)
			{
				wParam = wParam-48; 
			}
			strChar.Format("%c",wParam); // HS?
			if( wParam > 0x7f ) // 是汉字
			{
				static char cOne = 0;
				static CWordArray wordAy; 
				cOne ++;
				wordAy.Add((UINT)wParam);
				if( cOne%2 == 0 )
				{		
					strChar.Format("%c%c",wordAy.GetAt(0),wordAy.GetAt(1));
					wordAy.RemoveAll();
					cOne = 0;
				}
				else
				{
					strChar.Empty();
				}
			}
			if( !strChar.IsEmpty() )
			{
				CString strOldText;
				int nLen = 0;
				m_pEdit->GetWindowText(strOldText);
				if(wParam == VK_BACK)
				{
					if(strOldText.GetLength() > 0)
					{
						strOldText = strOldText.Left(strOldText.GetLength()-1);
						m_pEdit->SetWindowText(strOldText);
						nLen = strOldText.GetLength();
					}
				}
				else
				{		
					CString strText;
					strText.Format("%s%s",strOldText,strChar);
					if( strText.GetLength() <= MAX_ENTER_CHARS )
					{
						m_pEdit->SetWindowText(strText);
						nLen = strText.GetLength();
					}
					else 
					{
						m_pEdit->SetWindowText(strText);
						nLen = strText.GetLength();

						m_pEdit->SetFocus();
						m_pEdit->SetSel(nLen,nLen);
						return 1;
					}
				}
				m_pEdit->SetFocus();
				m_pEdit->SetSel(nLen,nLen);
			}
			return 1;
		}	
	}

	return 0;
}

void CWndKeyboard::OnShowButton()
{
	if( !IsStyle(DS_ALWAYSSHOW) )
	{		
		HBITMAP hbmp = (HBITMAP)::LoadImage(
			GetModuleHandle(_T("KeyBoard.dll")),
			MAKEINTRESOURCE(IDB_SHOWBTN),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);

		m_pAlwaysShow->SetImage(hbmp);
		AddStyle(DS_ALWAYSSHOW);
		
		if( IsStyle(DS_BYTREE) )
		{
			CRect rect,rc;
			this->GetWindowRect(rect);
			rc = rect;
			rc.top    += rect.Height() / 4;
			rc.bottom -= rect.Height() / 4;
			this->MoveWindow(rc);
		}
	}
	else
	{
		HBITMAP hbmp = (HBITMAP)::LoadImage(
			GetModuleHandle(_T("KeyBoard.dll")),
			MAKEINTRESOURCE(IDB_SHOWKEYBTNUP),
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
		m_pAlwaysShow->SetImage(hbmp);
		RemoveStyle(DS_ALWAYSSHOW);
		this->ShowWindow(SW_HIDE);
	}
	if(m_pEdit != NULL)
	{
		m_pEdit->SetFocus();
	}
}

void CWndKeyboard::OnCloseButton()
{
	if( IsStyle(DS_BYTREE) )
	{
		ShowWindow(SW_HIDE);
	}
	else if( IsStyle(DS_BYKEYBOARD) )
	{
		ShowWindow(SW_HIDE);
	}
	
	m_pPreFocusWnd = CWnd::FromHandle(g_hEditUpdateParentWnd);
	if(m_pPreFocusWnd != NULL)
	{
		m_pPreFocusWnd->SetFocus();
	}
	HBITMAP hbmp = (HBITMAP)::LoadImage(
		GetModuleHandle(_T("KeyBoard.dll")),
		MAKEINTRESOURCE(IDB_SHOWKEYBTNUP),
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION | LR_LOADMAP3DCOLORS);
	m_pAlwaysShow->SetImage(hbmp);
	RemoveStyle(DS_ALWAYSSHOW);
}

BOOL CWndKeyboard::ShowWindow( int nCmdShow )
{
	if(nCmdShow == SW_HIDE)
	{
		if(m_pEdit != NULL)
		{
			m_pEdit->SetWindowText("");
		}
	}
	BOOL bRet = 0;
	if(nCmdShow == SW_HIDE)
	{
		if( this->IsWindowVisible() )
		{
			bRet = CWnd::ShowWindow(nCmdShow);
		}
	}
	else if(nCmdShow == SW_SHOW)
	{
		if( !this->IsWindowVisible() )
		{
			bRet = CWnd::ShowWindow(nCmdShow);
		}
	}
	else
	{
		bRet = CWnd::ShowWindow(nCmdShow);
	}

	return bRet;
}

 BOOL CWndKeyboard::OnEraseBkgnd(CDC* pDC)
 {
 	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
 }
