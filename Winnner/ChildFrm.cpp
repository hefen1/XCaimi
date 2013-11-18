
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "Winner.h"
#include ".\WinnerView.h"
#include "ChildFrm.h"
#include ".\MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_CHAR()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{

}

CChildFrame::CChildFrame(CMDIFrameWndEx* pPrentWnd)
{
	
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style   &=   ~WS_SYSMENU;
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWinnerView* pView = new CWinnerView();
	if( !pView->CreateWinnerView(this) )
		return FALSE;
	this->SetActiveView(pView);
	m_pWinnerView = pView;
	return 0;
}

void CChildFrame::PostNcDestroy()
{
	CMDIChildWndEx::PostNcDestroy();
}

void CChildFrame::OnClose()
{
	CWnd* pWnd = AfxGetMainWnd();
	pWnd->SendMessage(WM_CHILDEWNDCLOSE,WPARAM(this),0);
	CMDIChildWndEx::OnClose();
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_CLOSE || nID == SC_MINIMIZE || nID == SC_MAXIMIZE || nID == SC_RESTORE)
	{
		HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();
		ASSERT(hWndMain);
		if(nID == SC_RESTORE || nID == SC_MAXIMIZE)
		{
			DWORD dwStyle = GetWindowLong(hWndMain, GWL_STYLE);
			nID = (dwStyle & WS_MAXIMIZE)? SC_RESTORE: SC_MAXIMIZE;
		}
		::PostMessage(hWndMain, WM_SYSCOMMAND, nID, lParam);
		return;
	}

	CMDIChildWndEx::OnSysCommand(nID, lParam);
}

void CChildFrame::SetLayoutInfo(CString strLayoutFileName,CString strMenuFileFname)
{
	m_strMenuFile = strMenuFileFname;
	m_strLayoutFile = strLayoutFileName;
}
void CChildFrame::OnUpdateFrameMenu(BOOL bActive, CWnd* pActiveWnd, HMENU hMenuAlt)
{
	//  无需更换菜单，禁用菜单更新
	//CMDIChildWndEx::OnUpdateFrameMenu(bActive, pActiveWnd, hMenuAlt);
}

void CChildFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CMDIChildWndEx::OnChar(nChar, nRepCnt, nFlags);
}
