// PluginWndContainer.cpp : 实现文件
//

#include "stdafx.h"
#include ".\Winner.h"
#include ".\HSPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPluginWndContainer 对话框

IMPLEMENT_DYNAMIC(CHSPanel, CDialog)

CHSPanel::CHSPanel(BOOL bSeflFree/* = TRUE*/,CWnd* pParent /*=NULL*/)
{
	m_hChildWnd = NULL;	
	m_bSeflFree = bSeflFree;
}

CHSPanel::~CHSPanel()
{
}

BEGIN_MESSAGE_MAP(CHSPanel, CView)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPluginWndContainer 消息处理程序

void CHSPanel::OnSize(UINT nType, int cx, int cy)
{
//	TRACE("CHSPanel OnSize函数执行\n");
	CWnd::OnSize(nType, cx, cy);
	if(m_hChildWnd != NULL && ::IsWindow(m_hChildWnd))
	{
		::SetWindowPos(m_hChildWnd, NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CHSPanel::PostNcDestroy()
{
	if(m_bSeflFree)
		delete this;
}

BOOL CHSPanel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CHSPanel::SetPluginHandle(HWND  hwnd)
{
	if(::IsWindow(hwnd))
	{
		m_hChildWnd = hwnd;
		CRect rc;
		GetClientRect(&rc);
		::SetWindowPos (m_hChildWnd,NULL, -1, -1, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CHSPanel::OnDraw(CDC* pDC)
{
//	TRACE("CHSPanel OnDraw执行\n");
}

int CHSPanel::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return TRUE;
}
