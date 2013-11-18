// DHtmlShowDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DHtmlShowDlg.h"

// CDHtmlShowDlg 对话框

IMPLEMENT_DYNCREATE(CDHtmlShowDlg, CDHtmlDialog)

CDHtmlShowDlg::CDHtmlShowDlg(CWnd* pParent /*=NULL*/)
//	: CDHtmlDialog(CDHtmlShowDlg::IDD, 0, pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strUserUrl = "";
	m_strUserCaption = "";

	Create(CDHtmlShowDlg::IDD, NULL);
}

CDHtmlShowDlg::~CDHtmlShowDlg()
{
}

void CDHtmlShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDHtmlShowDlg::OnInitDialog()
{
	//SetHostFlags(DOCHOSTUIFLAG_FLAT_SCROLLBAR);
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CDHtmlShowDlg, CDHtmlDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDHtmlShowDlg)
END_DHTML_EVENT_MAP()

void CDHtmlShowDlg::SetUserUrl( CString str )
{
	m_strUserUrl = str;
}

void CDHtmlShowDlg::SetCaption( CString str )
{
	m_strUserCaption = str;
}

void CDHtmlShowDlg::Show()
{
	this->ShowWindow(SW_HIDE);
	this->SetWindowText(m_strUserCaption.GetBuffer());
	this->MoveWindow(CRect(0,0,800,600), TRUE);
	this->CenterWindow(NULL);
	this->Navigate(m_strUserUrl.GetBuffer());
	this->ShowWindow(SW_SHOW);
}
BOOL CDHtmlShowDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc(0,0,0,0);
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, RGB(0,0,0));

	return TRUE;
	//return CDHtmlDialog::OnEraseBkgnd(pDC);
}
