////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "HSDataTip.h"
#include "shlwapi.h"

#include <mmsystem.h>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CHSDataTip* m_pTooltip = NULL;
struct _Tooltip_destructor
{
    ~_Tooltip_destructor( )
	{
		if( m_pTooltip )
		{
			m_pTooltip->m_hWnd = NULL;
			delete m_pTooltip;
			m_pTooltip = NULL;
		}
    }
};
static const _Tooltip_destructor _Tooltip_destructorer;

BOOL CHSDataTip::m_bIsRecordOpr = FALSE;

CHSDataTip* HSGetTipWnd()
{
	if( m_pTooltip != NULL && 
		::IsWindow(m_pTooltip->m_hWnd) )
		return m_pTooltip;

	if( m_pTooltip )
	{
		delete m_pTooltip;
		m_pTooltip = NULL;
	}

	m_pTooltip = new CHSDataTip();
	m_pTooltip->Create(AfxGetMainWnd());

	return m_pTooltip;
}

long HSOpenTip(CPoint point,CString strTitle,CString strDisp,
				DWORD dwMask /*= 0*/,
				CWnd* pWnd   /*= NULL*/,
				void* pData  /*= NULL*/)
{
	if( pData != NULL )
	{
		if( m_pTooltip != NULL && m_pTooltip->m_hWnd && 
			::IsWindow(m_pTooltip->m_hWnd) ) // hide
		{
			m_pTooltip->Hide();
		}
		return 0;
	}

	CHSDataTip* pTooltip = HSGetTipWnd();
	if(pTooltip != NULL)
	{		

		if( dwMask & CHSDataTip_Style_DlgOpenTip )
		{
			CString strSection,strEntry;
			if( pWnd )
			{
				// title
				if( strTitle.IsEmpty() )
				{
					pWnd->GetWindowText(strTitle);
				}
				// comment
				if( strDisp.IsEmpty() )
				{
					strEntry.Format("%i",pWnd->GetDlgCtrlID());

					CWnd* pParentWnd = pWnd->GetParent();
					if( pParentWnd )
					{
						pParentWnd->SendMessage(HS_OPEN_DATA_TIP_MSG,0,(long)&strSection);
						if( strSection.IsEmpty() )
							return 0;
					}

					static CString strFile = 
						CHSFilePath::GetPath(CHSFilePath::Setting) + FILENAME_HS_DIALOG_TIP_CFG;

					::GetPrivateProfileString(strSection,strEntry,"",
						strDisp.GetBuffer(256),255,	strFile);
					strDisp.ReleaseBuffer();

					if( strDisp.IsEmpty() )
					{
						::WritePrivateProfileString(strSection,strEntry,strTitle,strFile);						
					}
					if( !strDisp.CompareNoCase(strTitle) )
						strDisp.Empty();

				}
			}

			if( !strDisp.IsEmpty() )
			{
				pTooltip->Set( point,strTitle,strDisp,20,5,AfxGetMainWnd(),dwMask );
			}
		}
		else
		{
			//TRACE("OpenTip\n");
			pTooltip->Set( point,strTitle,strDisp,20,5,pWnd,dwMask );
		}
		return (long)pTooltip;
	}

	return 0;
}

void HSSetTipFontClr(CFont* pFont,CFont*	pTitleFont,
		COLORREF*      BackColorRef	     ,
		COLORREF*      TextColorRef	     ,
		COLORREF*      TitleBackColorRef ,
		COLORREF*      TitleTextColorRef )
{
	CHSDataTip* pTooltip = HSGetTipWnd();
	if(pTooltip != NULL)
	{
		pTooltip->SetCurFontClr(pFont,pTitleFont,
			BackColorRef,
			TextColorRef,
			TitleBackColorRef,
			TitleTextColorRef);
	}
}

class CTipEdit : public CEdit
{
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


#define TEXT_BITMAP_BUTTOM_SIZE	 (13)
#define TEXT_LEFT_RIGHT_SIZE	 (5)

UINT TRACEBOX_BUTTON_EXPLOYER = ::RegisterWindowMessage (HS_DATA_TIP_EXPLORER);
UINT TRACEBOX_BUTTON_PRE	  = ::RegisterWindowMessage (HS_DATA_TIP_Pre);
UINT TRACEBOX_BUTTON_NEXT	  = ::RegisterWindowMessage (HS_DATA_TIP_Next);
UINT TRACEBOX_BUTTON_CLOSE	  = ::RegisterWindowMessage (HS_DATA_TIP_Close);

UINT HS_OPEN_DATA_TIP_MSG	  = ::RegisterWindowMessage (HS_OPEN_DATA_TIP);

//extern void FillSolidRect(CDC * pDC, int x, int y, int cx, int cy, COLORREF clr);
//extern void Draw3dRect(CDC * pDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight);
//extern void Draw3dRect(CDC * pDC, LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);

// msg define begin
#define CHSDataTip_ButMsg_Always	(CHSDataTip_ButMsg_Begin + 1)

//	杨朝惠添加20040315
#define	CHSDataTip_ButMsg_Close		(CHSDataTip_ButMsg_Begin + 2)
//	杨朝惠添加结束
// end

// Draw 3d
void FillSolidRect(CDC * pDC, int x, int y, int cx, int cy, COLORREF clr)
{
	ASSERT(pDC != NULL);
	ASSERT(pDC->m_hDC != NULL);

	COLORREF oldColor = pDC->SetBkColor(clr);
	CRect rect(x, y, x + cx, y + cy);
	pDC->ExtTextOut(0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	pDC->SetBkColor(oldColor);
}

void Draw3dRect(CDC * pDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(pDC, x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(pDC, x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(pDC, x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(pDC, x, y + cy, cx, -1, clrBottomRight);
}

void Draw3dRect(CDC * pDC, LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	Draw3dRect(pDC, lpRect->left, lpRect->top, lpRect->right - lpRect->left, 
		lpRect->bottom - lpRect->top, clrTopLeft, clrBottomRight);
}
//

// define the offset above cursor
static const CPoint _defOffset(14,14);//(8, 6);

// border area around the text
static const short _border = 2;

// define the active border for data tips
//static const short _horizontal  = 2;
//static const short _vertical    = 2;

short m_topMargin     = 10;
short m_leftMargin    = 10;
short m_rightMargin   = 10;
short m_bottomMargin  = 10;

// class data members
BOOL  CHSDataTip::_registered      = FALSE;
short CHSDataTip::_delay		   = 500;
short CHSDataTip::m_nMaxDispRow	   = 8;
short CHSDataTip::m_nHidedelay     = 5000;

short CHSDataTip::_count = 0;


// GDI objects used for drawing operations in module
static CBrush* _brush = NULL;

// hook information
HHOOK CHSDataTip::_hookProc = NULL;
HHOOK CHSDataTip::_hookMouseProc = NULL;

CHSDataTip* CHSDataTip::_current = NULL;

CSize m_textsize = CSize(16,16);

static COLORREF      m_BackColor = ::GetSysColor(COLOR_INFOBK);
static COLORREF      m_TextColor = ::GetSysColor(COLOR_INFOTEXT);
static COLORREF      m_TitleBackColor = ::GetSysColor(COLOR_INFOBK);
static COLORREF      m_TitleTextColor = ::GetSysColor(COLOR_INFOTEXT);

static CBrush		 m_brush;

COLORREF*      m_BackColorRef	   = NULL;
COLORREF*      m_TextColorRef	   = NULL;
COLORREF*      m_TitleBackColorRef = NULL;
COLORREF*      m_TitleTextColorRef = NULL;

COLORREF* CHSDataTip::GetBkColor()		 { return (( m_BackColorRef != NULL )?m_BackColorRef:&m_BackColor); }
COLORREF* CHSDataTip::GetFgColor()		 { return (( m_TextColorRef != NULL )?m_TextColorRef:&m_TextColor); }
COLORREF* CHSDataTip::GetBkTitleColor() { return (( m_TitleBackColorRef != NULL )?m_TitleBackColorRef:&m_TitleBackColor); }
COLORREF* CHSDataTip::GetFgTitleColor() { return (( m_TitleTextColorRef != NULL )?m_TitleTextColorRef:&m_TitleTextColor); }

// special destructor object for clearing up GDI objects
struct _brush_destructor
{
    ~_brush_destructor( )
    {
		if( _brush )
		{
			delete _brush;
			_brush = NULL;
		}
    }
};
static const _brush_destructor _destroyer;

void CHSDataTip::RegisterWnd( )
{
    // check for prior registration
    if (_registered) return;

    // initialise the basic information before registration
    HINSTANCE hInst = AfxGetInstanceHandle( );

    // initialise the window class information
	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
    wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
    wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
    wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = *_brush;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = "CHSDataTip";

    // register the window class
    if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();

    _registered = TRUE;
}

void CHSDataTip::Initialise( )
{
    if (_brush == NULL)
    {
        _brush = new CBrush(::GetSysColor(COLOR_INFOBK));
    }

    // register the window class
    RegisterWnd( );

	//add by lxqi 20090617	配置是否记录用户操作
	//m_bIsRecordOpr = AfxGetApp()->GetProfileInt(_T("system"),_T("IsRecordOperate"),0);
	CfgValues::GetDetailCfgInt(m_CfgTopMapOb, _T("system"),_T("IsRecordOperate") , m_bIsRecordOpr,0);
	//end by lxqi

    // install the keyboard hook procedure
    if (_hookProc == NULL)
    {

        _hookProc = ::SetWindowsHookEx(WH_KEYBOARD, 
                                       (HOOKPROC)KeyboardHookCallback,
                                       NULL,
                                       ::GetCurrentThreadId( ));

    }

	//
	if (_hookMouseProc == NULL)
    {
        _hookMouseProc = ::SetWindowsHookEx(WH_MOUSE, 
                                       (HOOKPROC)MouseProcCallback,
                                       NULL,
                                       ::GetCurrentThreadId( ));
    }
}

LRESULT CALLBACK CHSDataTip::MouseProcCallback(int code, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT* pMouse = (MOUSEHOOKSTRUCT*)lParam;
	if( _current && ::IsWindow(_current->m_hWnd) && lParam )
	{

		CRect rect;
		_current->GetWindowRect(rect);

		if( wParam == WM_LBUTTONDBLCLK ) // 双击
		{
			if( rect.PtInRect(pMouse->pt) )
			{
//DEL BY YUHJ 2008.6
//				_current->Hide();
//END
			}
		}
		else if( !(_current->IsStyle(CHSDataTip_Style_NotCloseTip) ||
			      _current->IsStyle(CHSDATATIP_STYLE_OPRNTIP_ATONCE)) )
		{
			CRect rc = rect;
			if( !_current->IsStyle(CHSDATATIP_STYLE_OPRNTIP_JUMP) )
				rc.InflateRect( _defOffset.x + 1,//m_topMargin,   
					_defOffset.y + 1,//m_leftMargin,  
					m_rightMargin, 
					m_bottomMargin);
			else
			{
				if( _current->IsStyle(CHSDATATIP_STYLE_JUMP_ALIGIN_BOTTOM) )
					rc.bottom += 20;
				else if( _current->IsStyle(CHSDATATIP_STYLE_JUMP_ALIGIN_TOP) )
					rc.top -= 20;
			}
			switch( wParam )
			{
			case WM_NCMOUSEMOVE:
			case WM_MOUSEMOVE:
				{
					if( !rc.PtInRect(pMouse->pt) )
					{
						_current->Hide();
					}
				}
				break;
			case WM_NCLBUTTONDOWN:
			case WM_NCLBUTTONUP:
			case WM_NCLBUTTONDBLCLK:
			case WM_NCRBUTTONDOWN:
			case WM_NCRBUTTONUP:

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
				{
					if( !rect.PtInRect(pMouse->pt) )
					{
						_current->Hide();
					}
				}
				break;
			default:
				break;
			}
		}
	}

	return ::CallNextHookEx(_hookMouseProc, code, wParam, lParam);
}

LRESULT CALLBACK CHSDataTip::KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    // 任意键 hide
    if ( code >= 0 && wParam != VK_F1 && wParam != VK_SHIFT)
	{
		if(_current && _current->m_hWnd /*&& !_current->IsStyle(CHSDataTip_Style_NotCloseTip)*/ )
		{
			_current->Hide( );
		}
		else
		{
			/*CWnd* pWnd = AfxGetMainWnd();
			if( pWnd )
			{
				pWnd->SetFocus();
			}*/
		}
	}

    // chain to the next hook procedure
    return ::CallNextHookEx(_hookProc, code, wParam, lParam);
}

CHSDataTip::CHSDataTip()
{
	//	杨朝惠添加，20040315
	m_bTipLocked	= FALSE;
	//	杨朝惠添加结束

	m_pFont		 = CHSTools::CreateFont("Arial",12, 0);
	m_pTitleFont = CHSTools::CreateFont("Arial",12, FONT_BOLD);

	m_pEdit = NULL;

	// color
    Initialise( );

    m_ready  = FALSE;
    
	m_timer  = 0;
	m_Killtimer = 0;

    m_origin = CPoint(0, 0);
    m_offset = _defOffset;

	m_Text.Empty();
	memset(&m_TextRect,0,sizeof(CRect));
   
	m_bSoundOn = FALSE;
	m_strSoundFile.Empty();

    _count++;
}

CHSDataTip::~CHSDataTip()
{
    _count--;
    if ( _count == 0 ) 
    {
		if( _hookProc != NULL )
		{
			::UnhookWindowsHookEx(_hookProc);
			_hookProc = NULL;
		}
		if( _hookMouseProc != NULL )
		{
			::UnhookWindowsHookEx(_hookMouseProc);
			_hookMouseProc = NULL;
		}		
    }

	if(m_pEdit != NULL)
	{
		m_pEdit->m_hWnd = NULL;
		delete m_pEdit;
		m_pEdit = NULL;
	}

	RemoveAllButton();

	if( m_pTitleFont )
	{
		delete m_pTitleFont;
		m_pTitleFont = NULL;
	}
	if( m_pFont )
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

BOOL CHSDataTip::Create(CWnd* pParentWnd,DWORD dStyle /*= 0*/) 
{
	m_dStyle = dStyle;
	CWnd* pWnd;

	if(pParentWnd)
	{
		pWnd = pParentWnd;	
	}
	else
	{
		pWnd = GetDesktopWindow();
	}

	// this
	m_WinSize.cx = 130;
	m_WinSize.cy = 0;

	//
	if( m_brush.m_hObject )
	{
		m_brush.DeleteObject();
	}
	m_brush.CreateSolidBrush(*GetBkColor());

	m_ready = CWnd::CreateEx(WS_EX_LEFT | WS_EX_TOOLWINDOW,
                             "CHSDataTip", 
                             NULL,
                             WS_POPUP | WS_BORDER | WS_CLIPSIBLINGS,
                             0, 0, m_WinSize.cx, 0,
                             NULL,//pWnd->m_hWnd, 
                             NULL);
/*
	m_ready = CWnd::CreateEx(	0,
						AfxRegisterWndClass(0),
						"CHSDataTip",
						WS_POPUP | WS_SYSMENU,
						CRect(0,0,50,50),
						NULL,
						NULL,
						NULL );
*/
	// bitmap button

	m_pEdit = new CTipEdit;
	m_pEdit->Create(WS_CHILD | WS_VISIBLE | ES_MULTILINE |
		ES_AUTOVSCROLL | WS_VSCROLL | ES_WANTRETURN ,//| WS_DISABLED,
		CRect(0,0,m_WinSize.cx,0),this,10001);

	m_pEdit->SetFont(m_pFont);

	AddButton(CHSDataTip_ButMsg_Always);
	
	//	杨朝惠添加20040315
	//	增加一个关闭按钮
	AddButton(CHSDataTip_ButMsg_Close);
	//	杨朝惠修改结束

	//m_pEdit->ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

	return m_ready;
}

void CHSDataTip::Hide( )
{
    if (!m_ready) return;

	if( IsWindowVisible() )
	{
		ShowWindow(SW_HIDE);
    }
	if(m_timer != 0)
	{
		KillTimer(m_timer);
		m_timer = 0;
	}

	if(m_Killtimer != 0)
	{
		KillTimer(m_Killtimer);
		m_Killtimer = 0;
	}

	if( this->IsStyle(CHSDataTip_Style_NotCloseTip) )
	{
		CHSButton* pButton = GetButton(CHSDataTip_ButMsg_Always);
		if( pButton != NULL )
		{
			pButton->SetTooltip("打开总是显示锁定");
			pButton->SetImage(IDB_SHOWBTNUP);
			RemoveStyle(CHSDataTip_Style_NotCloseTip);
		}
	}

//  ::ReleaseCapture( );

    _current = NULL;

}

void CHSDataTip::Set(CPoint point, 
					  const CString& title,
					  const CString& tip,
					  int nWidChar,int nRow,
					  CWnd* pWnd /*= NULL*/,
					  DWORD dStyle /*= 0*/)
{
//	m_Title   = title;

	m_dStyle = dStyle | (m_dStyle & CHSDataTip_Style_NotCloseTip);

	// 打开钉子
	if( (m_dStyle & CHSDATATIP_STYLE_OPRNTIP_ATONCE) && 
		!(m_dStyle & CHSDataTip_Style_NotCloseTip) )
	{
		OnButtonMsg(CHSDataTip_ButMsg_Always);
	}

	// gbq add 20060310, 如果不是CHSDATATIP_STYLE_OPRNTIP_JUMP属性, 则获取当前鼠标点为当前点
	if( !(( m_dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP) && point.x > 0) )
		if( !::GetCursorPos( &point ) )
			return;
	// gbq end;

	/*************************************************************
	* 吕连新更改
	* 时间：  2003年9月19日
	* 描述：  下面的条件中，加入了(dStyle & CHSDATATIP_STYLE_OPRNTIP_ATONCE)
	*		  为了在按下左键时，不管点同不同，立刻弹出
	*************************************************************/
	if ( point != m_origin || (dStyle & CHSDATATIP_STYLE_OPRNTIP_ATONCE) || (dStyle &  CHSDataTip_Style_IgnoreOriginPoint))
		 //m_Text.CompareNoCase(tip) )
	{
		m_origin  = point;

		m_Title   = title;
		m_Text    = tip;  
		SetDispTextSize(m_Text,nWidChar,nRow);

		if( dStyle & CHSDATATIP_STYLE_OPRNTIP_ATONCE )
		{
			Display();
			this->m_Killtimer = SetTimer((long)this, 5000, NULL);
		}
		else if( this->IsWindowVisible() && IsStyle(CHSDataTip_Style_NotCloseTip) )
		{
			//20090626 YJT 修改帮助tip显示方式 begin
			Display();
			m_dStyle = dStyle | (m_dStyle & CHSDataTip_Style_NotCloseTip);
			//20090626 YJT 修改帮助tip显示方式 end
		}
		else
		{
			if( m_timer != 0 )
			{
				//return ;
				KillTimer(m_timer);
				m_timer = 0;
			}

			if( pWnd == NULL )
				pWnd = GetDesktopWindow();

			if( pWnd )
			{
				CRect rect,rectInter;
				pWnd->GetWindowRect(rect);
				
				if( !(dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP) )
				{
					CRect rcThis(m_origin.x, 
						m_origin.y, 
						m_origin.x + m_WinSize.cx + _defOffset.x, 
						m_origin.y + m_WinSize.cy + _defOffset.y); 
					rcThis.InflateRect(2 * _border, _border);
					
					rectInter.IntersectRect(rect,rcThis);
					if( rcThis.right > rect.right )
					{
						m_rightMargin = 10;
						m_offset.x = (rect.right - rcThis.right)   - m_rightMargin - 2;
					}
					else
					{
						m_rightMargin = 0;
						m_offset.x = _defOffset.x;
					}

					if( rcThis.bottom > rect.bottom )
					{
						m_bottomMargin = 10;
						m_offset.y = (rect.bottom - rcThis.bottom) - m_bottomMargin - 2;
					}
					else
					{
						m_bottomMargin = 0;
						m_offset.y = _defOffset.y;
					}
				}
			}

			if(m_Killtimer != 0)  
			{
				KillTimer(m_Killtimer);
				m_Killtimer = 0;
			}
			//开启时钟 显示TIP
			if( m_timer == 0 ) 
			{
				m_timer = SetTimer((long)this, 
					((dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP ) ? 250 : _delay), NULL);
			}
		}
	}
}
    
void CHSDataTip::Display( )
{
    if (!m_ready) 
		return;

    if (_current != NULL)
    {
        _current->Hide();
    }

	// gbq modify & add, 
	/* old
	CRect wndRect(m_origin.x, 
                  m_origin.y, 
                  0, 
                  0); 
    */
	CRect wndRect;
	if( m_dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP )
	{
		CRect rect,rectInter;
		CWnd *pWnd = AfxGetMainWnd();
		pWnd->GetClientRect(rect);
		DWORD   dStyle = m_dStyle;
		CRect rcThis;
		// 设置默认对齐方式
		if( !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_LEFT) 
			&& !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_RIGHT)
			&& !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_HCENTER) )
			dStyle |= CHSDATATIP_STYLE_JUMP_ALIGIN_LEFT;
		if( !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_TOP)
			&& !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_BOTTOM)
			&& !(dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_VCENTER) )
			dStyle |= CHSDATATIP_STYLE_JUMP_ALIGIN_TOP;

		if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_LEFT )
		{
			rcThis.left = m_origin.x;
			rcThis.right = m_origin.x + m_WinSize.cx;
		}
		else if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_RIGHT )
		{
			rcThis.right = m_origin.x;
			rcThis.left = rcThis.right - m_WinSize.cx;
		}
		else if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_HCENTER )
		{
			int nWidth = m_WinSize.cx;
			rcThis.left = m_origin.x - nWidth / 2;
			rcThis.right = rcThis.left + nWidth;
		}
		if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_TOP )
		{
			rcThis.top = m_origin.y;
			rcThis.bottom = m_origin.y + m_WinSize.cy ;
		}
		else if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_BOTTOM )
		{
			rcThis.bottom = m_origin.y;
			rcThis.top = m_origin.y - m_WinSize.cy;
		}
		else if( dStyle & CHSDATATIP_STYLE_JUMP_ALIGIN_VCENTER )
		{
			int nHeight = m_WinSize.cy;
			rcThis.top = m_origin.y - nHeight / 2;
			rcThis.bottom = rcThis.top + nHeight;
		}
		
		rectInter.IntersectRect(rect,rcThis);
		if( rcThis.left < rect.left )
			m_offset.x = rect.left - rcThis.left + 2;
		else if( rcThis.right > rect.right )
			m_offset.x = rect.right - rcThis.right - 2;
		else
			m_offset.x = 0;

		if( rcThis.top < rect.top )
			m_offset.y = rect.top - rcThis.top + 2;
		else if( rcThis.bottom > rect.bottom )
			m_offset.y = rect.bottom - rcThis.bottom - 2;
		else
			m_offset.y = 0;

		//m_origin.x = rcThis.left;
		//m_origin.y = rcThis.top;
		wndRect.SetRect(rcThis.left, rcThis.top, 0, 0);
		SetSize((rcThis.left, rcThis.top));				// gbq 
	}
	else
		wndRect.SetRect(m_origin.x, 
                  m_origin.y, 
                  0, 
                  0); 
	// gbq end;

    wndRect.InflateRect(2 * _border, _border);
    wndRect.OffsetRect(m_offset);

	int nCxScreen;
	int nCyScreen;

	CWnd *pWnd = AfxGetMainWnd();
	if( pWnd )
	{
		CRect rect;
		pWnd->GetClientRect(rect);
		nCxScreen = rect.Width();
		nCyScreen = rect.Height();
	}
	else
	{
		nCxScreen = ::GetSystemMetrics(SM_CXSCREEN);
		nCyScreen = ::GetSystemMetrics(SM_CYSCREEN);
	}

	wndRect.right = wndRect.left + m_WinSize.cx + _border;
	wndRect.bottom = wndRect.top + m_WinSize.cy + _border;

	if( wndRect.right > nCxScreen )
	{
		wndRect.left -= (wndRect.right-nCxScreen);
	}
	if( wndRect.bottom > nCyScreen )
	{
		wndRect.top -= (wndRect.bottom - nCyScreen);
	}
	//---------------------------yulx add
	if (pWnd)
	{
		pWnd->ClientToScreen(&wndRect);
	}
	//=============end
	SetWindowPos(NULL,//&wndTopMost, 
                 wndRect.left,
                 wndRect.top,
                 0, 
                 0, 
                 SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_FRAMECHANGED);


	this->Invalidate();
	_current = this;
 //   SetCapture( );
}

void CHSDataTip::SetSize(CPoint pt)
{
	if( pt.x == 0 && pt.y == 0 )
		pt = m_origin;
	if( IsStyle(CHSDataTip_Style_NotCloseTip) )
	{				
		SetWindowPos(NULL,//&wndTopMost, 
			0,
			0,
			m_WinSize.cx, 
			m_WinSize.cy + _border, 
			SWP_NOMOVE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

		GetClientRect(&m_TextRect);

		CRect rect = m_TextRect;
		m_TextRect.top += (TEXT_BITMAP_BUTTOM_SIZE + 2);
		rect.bottom  = m_TextRect.top;
		rect.left   += 15;

		m_pEdit->MoveWindow(m_TextRect);

		this->InvalidateRect(rect);
	}
	else
	{
		/* old
		CRect wndRect(m_origin.x, 
			m_origin.y, 
			m_origin.x + m_WinSize.cx, 
			m_origin.y + m_WinSize.cy); 
		*/
		CRect wndRect(pt.x, pt.y, pt.x + m_WinSize.cx, pt.y + m_WinSize.cy);

		wndRect.InflateRect(2 * _border, _border);
		wndRect.OffsetRect(m_offset);

		SetWindowPos(NULL,//&wndTopMost, 
			wndRect.left,
			wndRect.top,
			wndRect.Width( ), 
			wndRect.Height( ), 
			SWP_NOACTIVATE | SWP_FRAMECHANGED);

		GetClientRect(&m_TextRect);
		m_TextRect.top += (TEXT_BITMAP_BUTTOM_SIZE + 2);
		m_pEdit->MoveWindow(m_TextRect);
	}
}

BOOL CHSDataTip::DestroyWindow() 
{
	if( m_timer != 0 )
	{
		KillTimer(m_timer);
		m_timer = 0;
	}

	if(m_Killtimer != 0)
	{
		KillTimer(m_Killtimer);
		m_Killtimer = 0;
	}

	return CWnd::DestroyWindow();
}

BOOL CHSDataTip::PreTranslateMessage(MSG* pMsg) 
{
	//switch (pMsg->message)
 //   {
 //       case WM_RBUTTONDOWN:
	//		{
	//			if( GetFocus() == m_pEdit )
	//			{
	//				break;
	//			}
	//		}
	//	case WM_LBUTTONDBLCLK:
 //       case WM_MBUTTONDOWN:
 //       //case WM_RBUTTONDOWN:
 //       case WM_CANCELMODE:
 //           Hide( );
 //           break;
	//	default:
	//		break;
 //   }

	return CWnd::PreTranslateMessage(pMsg);
}

IMPLEMENT_DYNCREATE(CHSDataTip, CWnd)

BEGIN_MESSAGE_MAP(CHSDataTip, CWnd)
	//{{AFX_MSG_MAP(CHSDataTip)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()	
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(HS_OPEN_DATA_TIP_MSG, OpenDataTipMsg)

	ON_WM_CTLCOLOR()

	ON_CONTROL_RANGE(BN_CLICKED,CHSDataTip_ButMsg_Begin,CHSDataTip_ButMsg_End,OnButtonMsg)

	ON_WM_SIZE()
END_MESSAGE_MAP()

void CHSDataTip::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	COLORREF OldColor = dc.SetTextColor(*GetFgTitleColor());

	rc.bottom = rc.top + TEXT_BITMAP_BUTTOM_SIZE + 2;

	dc.FillSolidRect(rc,*GetBkTitleColor());

	CFont* pOldFont = dc.SelectObject(m_pTitleFont);
	dc.DrawText(m_Title,rc,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	dc.SelectObject(pOldFont);

	dc.SetTextColor( OldColor );
}

void CHSDataTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
}

void CHSDataTip::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
}

void CHSDataTip::OnLButtonUp(UINT nFlags, CPoint point) 
{
}

void CHSDataTip::OnTimer(UINT nIDEvent) 
{ 
	if( nIDEvent == m_timer )
	{
		KillTimer(m_timer); 
		m_timer = 0;
		
		// make sure that cursor hasn't moved before displaying the data tip
		// gbq modify & add 20060310, 原来部分为else框内
		if( this->m_dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP )
		{
			Display();
		}
		else
		{
			CPoint point;
			if ( ::GetCursorPos(&point) )
			{
				if (point == m_origin)
				{
					Display();
				}
			}
		}
		// gbq end;
	}
	else if(nIDEvent == m_Killtimer)
	{
		KillTimer(m_Killtimer);
		m_Killtimer = 0;
		Hide();
	}
}

void CHSDataTip::SetDispTextSize(CString text,int nWidChar,int nRow)
{
	m_Text = text;

	// 查找最长行
	CStringArray ay;
	int nCount = CHSTools::AnalyzeStr(text,"\r",ay);
	int nLen = 0;
	for( int i = 0; i < nCount; i++ )
	{
		if( nLen < ay.GetAt(i).GetLength() )
		{
			nLen = ay.GetAt(i).GetLength();
			text = ay.GetAt(i);
		}
	}

	// 得到最长行的宽度和高度
	if( m_pEdit && m_pFont )
	{
		CClientDC dc(m_pEdit);
		CFont* pOldFont = dc.SelectObject(m_pFont);
		m_textsize = dc.GetTextExtent(text);
		TEXTMETRIC tm;
		dc.GetTextMetrics(&tm);
		m_WinSize.cx = m_textsize.cx + tm.tmMaxCharWidth + 2;
		dc.SelectObject(pOldFont);
	}

	//if( m_WinSize.cx < m_textsize.cx * nLen )
	//	m_WinSize.cx = m_textsize.cx * nLen;
	if( m_WinSize.cx < 130 )
		m_WinSize.cx = 130;

	// 使能够看到
	CWnd* pWnd = GetDesktopWindow();
	if( pWnd )
	{
		CRect rc;
		pWnd->GetWindowRect(rc);
		if( m_WinSize.cx > rc.Width() / 2 )
			m_WinSize.cx = rc.Width() / 2;
	}

	// 按大小设定edit文本
	m_pEdit->SetWindowPos(NULL,0,0,m_WinSize.cx + 2 * _border,
						  m_TextRect.Height(),SWP_NOMOVE | SWP_NOACTIVATE);
	m_pEdit->SetWindowText( m_Text );

	// 得到edit文本显示行数
	int nRowCount = m_pEdit->GetLineCount();

	// 确定是否显示竖向滚动条
	DWORD dStyle = m_pEdit->GetStyle();
	if( nRowCount > m_nMaxDispRow )
	{
		dStyle |= WS_VSCROLL;
		nRowCount = m_nMaxDispRow;
	}
	else
	{
		dStyle &= ~WS_VSCROLL;
	}
	SetWindowLong(m_pEdit->m_hWnd,GWL_STYLE,dStyle);
	
	// 重设大小
	m_WinSize.cy = TEXT_BITMAP_BUTTOM_SIZE + m_textsize.cy * nRowCount + 2;

	if( !(m_dStyle & CHSDATATIP_STYLE_OPRNTIP_JUMP) )
		SetSize();
}

void CHSDataTip::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);	
	if (lpwndpos->flags & SWP_SHOWWINDOW)
		if(m_bSoundOn && !m_strSoundFile.IsEmpty())
			PlayPopSound();
}

// Plays a WAVE
BOOL CHSDataTip::PlayPopSound()
{	
	char path[128];
	GetCurrentDirectory(sizeof(path),path);
	CString str = path;
	str += "\\";
	str += m_strSoundFile;
//	BOOL bSucc = PlaySound(str,NULL,SND_FILENAME );
	return TRUE;
}

long CHSDataTip::OpenDataTipMsg(WPARAM wParam, LPARAM lParam)
{
	if( lParam != 0 )
	{
		MouseTipTextInfo* pInfo = (MouseTipTextInfo*)lParam;
		if( /*pInfo->m_pPoint != NULL &&*/ !pInfo->m_strData.IsEmpty())
		{
			//On(TRUE);
			//SetOffset(5,5);
			Set( CPoint(0,0),pInfo->m_strTitle,pInfo->m_strData,20,5,(CWnd*)wParam);
			return 1;
		}
		else
		{
			//Hide();
		}
	}
	else
	{
		//Hide();
	}
	return 0;
}

void CHSDataTip::SetCurFontClr(CFont* pFont,CFont* pTitleFont,
		COLORREF*      BackColorRef	     ,
		COLORREF*      TextColorRef	     ,
		COLORREF*      TitleBackColorRef ,
		COLORREF*      TitleTextColorRef )
{
	if( BackColorRef != NULL )
	{
		m_BackColorRef = BackColorRef;
	}
	if( TextColorRef != NULL )
	{
		m_TextColorRef = TextColorRef;
	}
	if( TitleBackColorRef != NULL )
	{
		m_TitleBackColorRef = TitleBackColorRef;
	}
	if( TitleTextColorRef != NULL )
	{
		m_TitleTextColorRef = TitleTextColorRef;
	}

	LOGFONT logfont;
	//
	if( pFont != NULL )
	{
		pFont->GetLogFont(&logfont);
		if( m_pFont )
		{
			delete m_pFont;
			m_pFont = NULL;
		}
		m_pFont = new CFont;
		if( !m_pFont->CreateFontIndirect(&logfont) )
		{
			delete m_pFont;
			m_pFont = NULL;
		}
	}

	//
	if( pTitleFont != NULL )
	{
		pTitleFont->GetLogFont(&logfont);
		if( m_pTitleFont )
		{
			delete m_pTitleFont;
			m_pTitleFont = NULL;
		}
		m_pTitleFont = new CFont;
		if( !m_pTitleFont->CreateFontIndirect(&logfont) )
		{
			delete m_pTitleFont;
			m_pTitleFont = NULL;
		}
	}

	if( m_pFont && pFont != NULL )
	{
		this->SetFont(m_pFont);
		if( m_pEdit )
		{
			m_pEdit->SetFont(m_pFont);

			CClientDC dc(m_pEdit);
			CFont* pOldFont = dc.SelectObject(m_pFont);
			m_textsize = dc.GetTextExtent("A");
			dc.SelectObject(pOldFont);
		}

		if( m_brush.m_hObject )
		{
			m_brush.DeleteObject();
		}
		m_brush.CreateSolidBrush(*GetBkColor());
	}
}

HBRUSH CHSDataTip::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		{
			pDC->SetTextColor(*GetFgColor());
			pDC->SetBkColor(*GetBkColor());
		}
		return m_brush;
	}

	return hbr;
}

CHSButton* CHSDataTip::GetButton(WORD key)
{
	CObject* pValue;
	if( m_ayBut.Lookup(key,pValue) )
	{
		return (CHSButton*)pValue;
	}
	return NULL;
}

void CHSDataTip::AddButton(WORD key)
{
	CHSButton* pButton = GetButton(key);

	if( pButton != NULL )
		return;

	CResManager resmanager;

	if( key == CHSDataTip_ButMsg_Always )
	{
		pButton = new CHSButton;
		pButton->Create("",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
			CRect(0,0,14,14),this,CHSDataTip_ButMsg_Always);
		pButton->SetImage(IDB_SHOWBTNUP);
		pButton->m_nFlatStyle = CHSButton::BUTTONSTYLE_FLAT;
		pButton->SetTooltip("打开总是显示锁定");

		m_ayBut.SetAt(key,pButton);
	}
	
	//	杨朝惠添加20040315
	if ( key== CHSDataTip_ButMsg_Close )
	{
		CRect	rect;
		GetClientRect(&rect);
		rect.left = rect.right - 14;
		rect.bottom	= rect.top + 14;

		pButton = new CHSButton;
		pButton->Create("",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
			rect,this,CHSDataTip_ButMsg_Close);
		pButton->SetImage(IDB_CLOSEBTN);
		pButton->m_nFlatStyle = CHSButton::BUTTONSTYLE_FLAT;
		pButton->SetTooltip("关闭提示");
	
		m_ayBut.SetAt(key,pButton);
	}
	//	杨朝惠添加结束
}

void CHSDataTip::RemoveAllButton()
{
	POSITION pos;
	WORD key;
	CObject* pValue;

	for( pos = m_ayBut.GetStartPosition(); pos != NULL; )
	{
		m_ayBut.GetNextAssoc( pos, key, pValue );
		if( pValue ) 
		{
			((CHSButton*)pValue)->GetToolTipCtrl().m_hWnd = NULL;
			((CHSButton*)pValue)->m_hWnd = NULL;
			delete (CHSButton*)pValue;
		}
	}
	m_ayBut.RemoveAll();
}

void CHSDataTip::OnButtonMsg(UINT nID)
{	
	CHSButton* pButton = GetButton(nID);
	if( pButton == NULL )
		return;

	switch(nID)
	{
	case CHSDataTip_ButMsg_Always:
		{
//	杨朝惠修改。20040315
			m_bTipLocked	= !m_bTipLocked;
			if ( m_bTipLocked )
			{
				pButton->SetImage(IDB_SHOWBTN);
				AddStyle(CHSDataTip_Style_NotCloseTip);
				pButton->SetTooltip("取消总是显示锁定");
				
			}
			else
			{
				pButton->SetTooltip("打开总是显示锁定");
				pButton->SetImage(IDB_SHOWBTNUP);
				RemoveStyle(CHSDataTip_Style_NotCloseTip);
				/*this->Hide();*/	//	杨朝惠取消，
			}
//	杨朝惠添加结束
/*
//	杨朝惠取消，20040315
			if( !IsStyle(CHSDataTip_Style_NotCloseTip) )
			{		
				pButton->SetImage(IDB_SHOWBTN);
				//AddStyle(CHSDataTip_Style_NotCloseTip);

				pButton->SetTooltip("取消总是显示");
			}
			else
			{
				pButton->SetTooltip("打开总是显示");
				pButton->SetImage(IDB_SHOWBTNUP);
				//RemoveStyle(CHSDataTip_Style_NotCloseTip);
				this->Hide();
			}
//	杨朝惠取消结束
*/
		}
		break;

	//	杨朝惠添加，20040315
	case	CHSDataTip_ButMsg_Close:
		//	取消可能存在的锁定
		if ( m_bTipLocked )
		{
			m_bTipLocked = FALSE;
		}
		//	关闭提示
		this->Hide();
		break;
	//	杨朝惠添加结束
	}
	pButton->Invalidate();
}

void CHSDataTip::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if( abs(cx) + abs(cy) <= 0 )
		return;
	
	//	杨朝惠添加20040315
	//	重新调整close按钮的位置
	CHSButton	*pCloseButton	= GetButton(CHSDataTip_ButMsg_Close);
	if ( pCloseButton != NULL && IsWindow(pCloseButton->GetSafeHwnd()))
	{
		CRect	rect;
		GetClientRect(&rect);
		rect.left = rect.right - 14;
		rect.bottom = rect.top + 14;
		pCloseButton->MoveWindow(&rect);
	}
	//	杨朝惠添加结束

	POSITION pos;
	WORD key;
	CObject* pValue;
	CHSButton* pBut;

	for( pos = m_ayBut.GetStartPosition(); pos != NULL; )
	{
		m_ayBut.GetNextAssoc( pos, key, pValue );
		pBut = (CHSButton*)pValue;
		if( pBut ) 
		{
			 
		}
	}

	// TODO: 在此添加消息处理程序代码
}


BEGIN_MESSAGE_MAP(CTipEdit, CEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CHAR()
//	ON_WM_CREATE()
ON_WM_KEYDOWN()
ON_WM_SETCURSOR()
END_MESSAGE_MAP()

void CTipEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CEdit::OnLButtonDown(nFlags, point);
	
	/*CWinApp* pApp = AfxGetApp();
	if( pApp )
	{
		SetCursor( pApp->LoadStandardCursor(IDC_ARROW) );
	}*/

	CWnd* pWnd = this->GetParent();
	if( pWnd )
	{
		pWnd->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
		//pWnd->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(point.x,point.y));
	}
}

void CTipEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*CWinApp* pApp = AfxGetApp();
	if( pApp )
	{
		SetCursor( pApp->LoadStandardCursor(IDC_ARROW) );
	}*/
	//CEdit::OnMouseMove(nFlags, point);
}

void CTipEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CEdit::OnRButtonDown(nFlags, point);
}

void CTipEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

//int CTipEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CEdit::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
//	// TODO:  在此添加您专用的创建代码
//
//	return 0;
//}

void CTipEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTipEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWinApp* pApp = AfxGetApp();
	if( pApp )
	{
		SetCursor( pApp->LoadStandardCursor(IDC_ARROW) );
	}
	return 1;

	//return CEdit::OnSetCursor(pWnd, nHitTest, message);
}
