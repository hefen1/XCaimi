// PopupWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "PopupWindow.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPopupWindow 对话框

IMPLEMENT_DYNAMIC(CPopupWindow, CDialog)

CPopupWindow::CPopupWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CPopupWindow::IDD, pParent)
{
	m_pWinnerView = NULL;
}

CPopupWindow::~CPopupWindow()
{
}

void CPopupWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPopupWindow, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CPopupWindow 消息处理程序

BOOL CPopupWindow::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pWinnerView = new CWinnerView();
	if( !m_pWinnerView->CreateWinnerView(this) ) 
		return FALSE;
	this->SendMessage(WM_SETICON, (WPARAM)TRUE, (LPARAM)LoadIcon(GetModuleHandle(NULL), (LPCTSTR)IDR_MAINFRAME));
	SetWindowPos(NULL,0,0,m_wndSize.Width(),m_wndSize.Height(),SWP_NOZORDER|SWP_NOMOVE);
	AdjustLayout();
	SetWindowText(m_strCaption);	

	CenterWindow();
	return TRUE;
}


void CPopupWindow::AdjustLayout()
{
	if(m_pWinnerView != NULL && IsWindow(m_pWinnerView->m_hWnd) )
	{
		CRect rect;
		GetClientRect(&rect);
		m_pWinnerView->SetWindowPos(NULL,-1,-1,rect.Width(),
			rect.Height(),SWP_NOZORDER);
		RedrawWindow();
	}
}

void CPopupWindow::OnSize(UINT nType, int cx, int cy)
{
	AdjustLayout();
}

int CPopupWindow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	LONG lStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
	if (!m_panelInfo.m_bMaximize)
	{
		lStyle &= ~WS_MAXIMIZEBOX;
		lStyle &= ~WS_THICKFRAME;
	}
	SetWindowLong(m_hWnd,GWL_STYLE,lStyle);

	return 0;
}

void CPopupWindow::PostNcDestroy()
{
	delete this;
}

void CPopupWindow::OnCancel()
{
	CMainFrame* pMain = (CMainFrame*)m_pParentWnd;
	ASSERT(pMain != NULL);
	pMain->RemovePopupPanel(m_panelInfo.GetPanelName());
	DestroyWindow();
}
