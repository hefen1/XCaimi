// PluginWndContainer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include ".\Winner.h"
#include ".\HSPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPluginWndContainer �Ի���

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


// CPluginWndContainer ��Ϣ�������

void CHSPanel::OnSize(UINT nType, int cx, int cy)
{
//	TRACE("CHSPanel OnSize����ִ��\n");
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
//	TRACE("CHSPanel OnDrawִ��\n");
}

int CHSPanel::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return TRUE;
}
