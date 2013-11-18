// InportPromptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InportPromptDlg.h"
#include "DlgFormulaImportExport.h"
#include "DlgFormulaMan.h"
#include "ExpPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInportPromptDlg dialog


CInportPromptDlg::CInportPromptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInportPromptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInportPromptDlg)
	m_nType = 1;
	//}}AFX_DATA_INIT
}


void CInportPromptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInportPromptDlg)
	DDX_Radio(pDX, IDC_RADIO_OVR, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInportPromptDlg, CDialog)
	//{{AFX_MSG_MAP(CInportPromptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInportPromptDlg message handlers

void CInportPromptDlg::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();
	CDlgFormulaImportExport* pWnd = (CDlgFormulaImportExport*)this->GetParent();
	if(pWnd != NULL)
	{
		pWnd->m_wExternalExpress &= ~(EXTERN_EXPRESS_OVR|EXTERN_EXPRESS_AUTO_NAME|EXTERN_EXPRESS_CHANGE_NAME);

		switch( m_nType )
		{
		case 0:
			pWnd->m_wExternalExpress |= EXTERN_EXPRESS_OVR;
			break;
		case 1:
			pWnd->m_wExternalExpress |= EXTERN_EXPRESS_AUTO_NAME;
			break;
		case 2:
			pWnd->m_wExternalExpress |= EXTERN_EXPRESS_CHANGE_NAME;
			break;
		default:
			return;
		}

	}

	CDialog::OnOK();
}

void CInportPromptDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CInportPromptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
