// DlgInputUnionName.cpp : implementation file
//

#include "stdafx.h"
#include "DlgInputUnionName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputUnionName dialog


CDlgInputUnionName::CDlgInputUnionName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputUnionName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputUnionName)
	m_strBlockName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInputUnionName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputUnionName)
	DDX_Text(pDX, IDC_BLOCKNAME, m_strBlockName);
	//DDV_MaxChars(pDX, m_strBlockName, 8);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CDlgInputUnionName, CDialog)
	//{{AFX_MSG_MAP(CDlgInputUnionName)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgInputUnionName message handlers

void CDlgInputUnionName::OnOK() 
{
	UpdateData();
	
	CDialog::OnOK();
}

void CDlgInputUnionName::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


BOOL CDlgInputUnionName::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDlgInputUnionName::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CDialog::ShowWindow(nCmdShow);
	}

	return 1;
}
