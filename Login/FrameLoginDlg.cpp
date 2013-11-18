// FrameLoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "FrameLoginDlg.h"
/*#include "DlgJYInput.h"*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CFrameLoginDlg 对话框

IMPLEMENT_DYNAMIC(CFrameLoginDlg, CDialog)

CFrameLoginDlg::CFrameLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameLoginDlg::IDD, pParent)
{

}

CFrameLoginDlg::~CFrameLoginDlg()
{

}

void CFrameLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CFrameLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CenterWindow();
	BOOL bRet = m_LoginWnd.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0),this,1000);
	if(!::IsWindow(m_LoginWnd.m_hWnd))
	{
		TRACE("登录框初始化失败");
		return False;
	}

	CRect rcClient;
	m_LoginWnd.GetClientRect(rcClient);
	m_LoginWnd.ShowWindow(SW_HIDE);
	MoveWindow(rcClient);

	m_LoginWnd.Init();
	m_LoginWnd.ShowWindow(SW_SHOW);

	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control

}
BEGIN_MESSAGE_MAP(CFrameLoginDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

void CFrameLoginDlg::OnDestroy()
{
	CDialog::OnDestroy();
	m_LoginWnd.DestroyWindow();
	
}

void CFrameLoginDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
// 	if(::IsWindow(m_LoginWnd.m_hWnd))
// 	{
// 		CDlgJYInput *pWnd = NULL;
// 		if((pWnd = m_LoginWnd.GetJYInput()) && ::IsWindow(pWnd->m_hWnd))
// 		{
// 			if(pWnd->IsWindowVisible())
// 				pWnd->PostMessage(WM_MOVEMESSAGE,0,0);
// 		}
// 	}

	CDialog::OnNcLButtonDown(nHitTest, point);
}
