// DockableView.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include ".\DockableView.h"
#include ".\MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDockableView

IMPLEMENT_DYNAMIC(CDockableView, CDockablePane)

CDockableView::CDockableView(BOOL bFreeSelf)
{	
	m_pWinnerView = NULL;
    m_bFreeSelf = bFreeSelf;
}

CDockableView::~CDockableView()
{
}


BEGIN_MESSAGE_MAP(CDockableView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CDockableView 消息处理程序



int CDockableView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyle(WS_CAPTION,NULL,NULL);

	m_pWinnerView = new CWinnerView();
	m_pWinnerView->CreateWinnerView(this);

	return 0;
}

void CDockableView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_pWinnerView == NULL) 
		return;

	CRect rectClient;
	GetClientRect(rectClient);

	m_pWinnerView->MoveWindow (0, 0, rectClient.Width(),rectClient.Height());
	//m_pWinnerView->SetWindowPos (NULL, -1, -1, rectClient.Width(), rectClient.Height(), 
	//	SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void CDockableView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CDockableView::PostNcDestroy()
{
	if(m_bFreeSelf)
		delete this;
}

void CDockableView::OnClose()
{
	CDockablePane::OnClose();
}

void CDockableView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//__super::OnContextMenu(pWnd,point);
}

void CDockableView::EnableGripper(BOOL bEnable)
{
	return __super::EnableGripper(bEnable);
	if (bEnable && m_bHasGripper)
	{
		m_cyGripper = afxGlobalData.GetTextHeight();
	}
	else
	{
		m_cyGripper = 0;
	}
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}


CTabbedPane* CDockableView::CreateTabbedPane()
{
	CTabbedPane* pTabbedPane = __super::CreateTabbedPane();
	pTabbedPane->EnableGripper(TRUE);
	return pTabbedPane;
}