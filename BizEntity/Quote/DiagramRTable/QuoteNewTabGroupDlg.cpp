#include "StdAfx.h"
#include "QuoteNewTabGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CQuoteReportDlg ¶Ô»°¿ò
IMPLEMENT_DYNAMIC(CQuoteNewTabGroupDlg, CDialog)

CQuoteNewTabGroupDlg::CQuoteNewTabGroupDlg(CWnd* pParent /* = NULL */)
:CDialog(CQuoteNewTabGroupDlg::IDD,pParent)
{
}

CQuoteNewTabGroupDlg::~CQuoteNewTabGroupDlg(void)
{
}

void CQuoteNewTabGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
BOOL CQuoteNewTabGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_GROUPNAME)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CQuoteNewTabGroupDlg, CDialog)
END_MESSAGE_MAP()

void CQuoteNewTabGroupDlg::OnOK()
{
	GetDlgItem(IDC_GROUPNAME)->GetWindowText(m_strGroupName);

	CDialog::OnOK();
}
