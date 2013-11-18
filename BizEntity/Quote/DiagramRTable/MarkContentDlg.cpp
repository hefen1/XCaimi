// MarkContentDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MarkContentDlg.h"


// CMarkContentDlg 对话框

IMPLEMENT_DYNAMIC(CMarkContentDlg, CDialog)

CMarkContentDlg::CMarkContentDlg(CPoint& point,CWnd* pParent /* = NULL */)
	: CDialog(CMarkContentDlg::IDD, pParent)
{
	m_point = point;
}

CMarkContentDlg::~CMarkContentDlg()
{
}

void CMarkContentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_CONTENT, m_wndContent);
	DDX_Control(pDX, IDOK, m_wndOK);
	DDX_Control(pDX, IDCANCEL, m_wndCancel);
}

CString CMarkContentDlg::GetContent()
{
	return m_strContent;
}

void CMarkContentDlg::SetSize()
{
	CRect rcClient,rcOK,rcCancel;
	GetClientRect(rcClient);
	if (m_wndOK.GetSafeHwnd())
	{
		m_wndOK.MoveWindow(rcClient.right-200,rcClient.bottom-40,80,30,FALSE);
	}
	if (m_wndCancel.GetSafeHwnd())
	{
		m_wndCancel.MoveWindow(rcClient.right-100,rcClient.bottom-40,80,30,FALSE);
	}
	if (m_wndContent.GetSafeHwnd())
	{
		m_wndContent.MoveWindow(0,0,rcClient.Width(),rcClient.Height()-50,FALSE);
	}
	Invalidate(FALSE);
}
BEGIN_MESSAGE_MAP(CMarkContentDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMarkContentDlg 消息处理程序

void CMarkContentDlg::OnOK()
{
	GetDlgItem(IDC_ED_CONTENT)->GetWindowText(m_strContent);
	CDialog::OnOK();
}

BOOL CMarkContentDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(NULL,m_point.x,m_point.y,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_wndContent.SetFocus();
	SetSize();
	return FALSE;  // return TRUE unless you set the focus to a control
}

void CMarkContentDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_wndOK.GetSafeHwnd())
	{
		SetSize();
	}
}
