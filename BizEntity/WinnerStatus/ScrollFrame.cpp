// ScrollFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "ScrollFrame.h"
#include "StatuBar.h"
#include "HSMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CScrollFrame 对话框

IMPLEMENT_DYNAMIC(CScrollFrame, CDialog)

CScrollFrame::CScrollFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CScrollFrame::IDD, pParent)
{
	//m_rectScrollFloat.SetRectEmpty();//当跑马灯不是固定在状态栏上 记录在窗口的位置，大小
	m_pScrollWnd = NULL;
}

CScrollFrame::~CScrollFrame()
{
}

void CScrollFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CScrollFrame, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCHITTEST()
    ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()


// CScrollFrame 消息处理程序

int CScrollFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	long lExStyle = ::GetWindowLong(m_hWnd,GWL_EXSTYLE); 
	lExStyle &= ~WS_EX_APPWINDOW; 
	lExStyle |=  WS_EX_TOOLWINDOW ;
	
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, lExStyle);
	SetParent(NULL);
	SetWindowPos(&wndTopMost,0,0,SCROLL_FLOAT_WIDTH,SCROLL_FLOAT_HEIGHT,SWP_NOSIZE|SWP_NOACTIVATE);

	return 0;
}

void CScrollFrame::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_pScrollWnd)
	{//如果跑马灯已经创建
		CRect rect;
		GetClientRect(&rect);
		m_pScrollWnd->MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
		m_pScrollWnd->Invalidate(FALSE);
	}
	
}

CRect CScrollFrame::getPosition()
{
	CRect rect;
	GetWindowRect(&rect);
	
	return rect;
}
void CScrollFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = SCROLL_FLOAT_WIDTH / 2;
	lpMMI->ptMinTrackSize.y = SCROLL_FLOAT_HEIGHT + 6;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

LRESULT CScrollFrame::OnNcHitTest(CPoint point)
{
	UINT hit = CDialog::OnNcHitTest(point);
	if (!m_pScrollWnd->bScrollFixed && !m_pScrollWnd->bScrollMultiLine)
	{//如果浮动 单行显示 
		switch(hit)
		{
		case HTBOTTOM:
		case HTTOP:        return HTCAPTION;

		case HTTOPLEFT:
		case HTBOTTOMLEFT: return HTLEFT;

		case HTTOPRIGHT:
		case HTBOTTOMRIGHT: return HTRIGHT;
		}

	}
	return hit;
}

void CScrollFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{

	CRect rect;
	GetWindowRect(&rect);
	if( rect.bottom-point.y < 5)
	{
		if( point.x-rect.left < 5)//左下
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		else if (point.x-rect.left < rect.Width()-5)//下
	        ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		else//右下
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	}
	else if (rect.bottom-point.y < rect.Height()-5)
	{
		if(point.x-rect.left < 5)//左中
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		else if(point.x-rect.left < rect.Width()-5)//中
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		else//右
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	}
	else
	{
		if(point.x-rect.left < 5)//左上
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		else if(point.x-rect.left < rect.Width()-5)//上
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		else//右上
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));

	}
	CDialog::OnNcMouseMove(nHitTest, point);
}
