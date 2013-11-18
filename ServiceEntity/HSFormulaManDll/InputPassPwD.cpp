// InputPassPwD.cpp : implementation file
//

#include "stdafx.h"
#include "InputPassPwD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputPassPwD dialog


CInputPassPwD::CInputPassPwD(CWnd* pParent /*=NULL*/,CString strTitle /*= ""*/,
							 CString strPrompt /*= ""*/,
							 char cType /*= 0*/)
	: CDialog(CInputPassPwD::IDD, pParent)
{
	m_cType = cType;
	m_strTitle = strTitle;
	m_strPrompt = strPrompt;
	//{{AFX_DATA_INIT(CInputPassPwD)
	m_strCurPWD = _T("");
	//}}AFX_DATA_INIT
	m_pExp = NULL;
}


void CInputPassPwD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputPassPwD)
	DDX_Text(pDX, IDC_EDITPASSS, m_strCurPWD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputPassPwD, CDialog)
	//{{AFX_MSG_MAP(CInputPassPwD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputPassPwD message handlers

void CInputPassPwD::OnOK() 
{
	// TODO: Add extra validation here
	this->UpdateData();

	if( m_cType == 1 )
	{
		if( m_strCurPWD.IsEmpty() )
		{
			::MessageBox( m_hWnd,_T(" 名称不能为空，请重新输入... "),_T(" 提示 "), MB_OK);

			CWnd* pWnd = this->GetDlgItem(IDC_EDITPASSS);
			if( pWnd != NULL )
			{
				pWnd->SetFocus();
			}
			return;			
		}
		
		CDialog::OnOK();
		return;
	}

	if( !m_strFistPWD.IsEmpty() )
	{
		if(m_strFistPWD.Compare(m_strCurPWD))
		{
			::MessageBox( m_hWnd,_T(" 确认密码输入错误，请重新输入... "),_T(" 提示 "), MB_OK);
			
			CWnd* pWnd = this->GetDlgItem(IDC_EDITPASSS);
			if( pWnd != NULL )
			{
				pWnd->SetFocus();
			}
			return;
		}
		CDialog::OnOK();
	}
	else if( m_pExp != NULL && 
		 	 m_pExp->IsValidPWD(m_strCurPWD) )
	{
		CDialog::OnOK();
	}
	else
	{
		::MessageBox( m_hWnd,_T(" 密码输入不对，请重新输入... "),_T(" 提示 "), MB_OK);

		CWnd* pWnd = this->GetDlgItem(IDC_EDITPASSS);
		if( pWnd != NULL )
		{
			pWnd->SetFocus();
		}
	}
}

void CInputPassPwD::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CInputPassPwD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_cType == 1)
	{
		CEdit* pEdit = (CEdit*)this->GetDlgItem(IDC_EDITPASSS);
		if(pEdit != NULL)
		{
			pEdit->SetPasswordChar(0);
		}
	}
	this->SetWindowText(m_strTitle);
	CWnd* pWnd = GetDlgItem(IDC_PROMPT);
	if(pWnd != NULL)
	{
		pWnd->SetWindowText(m_strPrompt);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
