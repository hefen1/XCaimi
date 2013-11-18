// NewWizPage.cpp : implementation file
//

#include "stdafx.h"
#include "HSNewWizPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHSNewWizPage dialog

CHSNewWizPage::CHSNewWizPage(CWnd* pParent /*= NULL*/)
{
	m_bCreated = FALSE;
	m_bActive = FALSE;
	m_nDialogID = 0;
	m_pParent = NULL; 
}

CHSNewWizPage::CHSNewWizPage(LPCTSTR lpszTemplateName, CWnd* pParent)
:CDialog(lpszTemplateName,pParent)
{
	m_bCreated = FALSE;
	m_bActive = FALSE;
	m_nDialogID = 0;
	m_pParent = NULL; 
}


CHSNewWizPage::CHSNewWizPage(UINT nIDTemplate, CWnd* pParent)
:CDialog(nIDTemplate,pParent)
{
	m_bCreated = FALSE;
	m_bActive = FALSE;
	m_nDialogID = nIDTemplate;
	m_pParent = NULL; 
}

CHSNewWizPage::~CHSNewWizPage()
{
}



void CHSNewWizPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSNewWizPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHSNewWizPage, CDialog)
//{{AFX_MSG_MAP(CHSNewWizPage)
//	ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSNewWizPage message handlers


BOOL CHSNewWizPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// create the large font
	m_LargeFont.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));
	
	DWORD style = GetStyle();
	ASSERT((style & WS_CHILD) != 0);
	ASSERT((style & WS_BORDER) == 0);
	ASSERT((style & WS_DISABLED) != 0);
	
	//	m_Brush.CreateSolidBrush(RGB(255, 255, 255));
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CHSNewWizPage::OnCreatePage()
{
	return TRUE;
}

void CHSNewWizPage::OnDestroyPage()
{
}

// refer to CPropertyPage class members for a description of this function
void CHSNewWizPage::OnCancel()
{
}


// refer to CPropertyPage class members for a description of this function
BOOL CHSNewWizPage::OnKillActive(int nPos /*= -1*/)
{
	return TRUE;
}



// called when the page is set active
void CHSNewWizPage::OnSetActive()
{
}



// refer to CPropertyPage class members for a description of this function
BOOL CHSNewWizPage::OnQueryCancel()
{
	return TRUE;
}


// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the previous one, return the 
// Dialog resource ID of the page to display
LRESULT CHSNewWizPage::OnWizardBack()
{
	return 0;
}


// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the next one, return the 
// Dialog resource ID of the page to display
LRESULT CHSNewWizPage::OnWizardNext()
{
	return 0;
}



// refer to CPropertyPage class members for a description of this function
BOOL CHSNewWizPage::OnWizardFinish()
{
	return TRUE;
}

void CHSNewWizPage::OnSetActive(CHSNewWizPage* pOldPage,int nPos /*= -1*/)
{
}

BOOL CHSNewWizPage::ShowWindow(int nCmdShow)
{
// 	if( !::IsWindow(this->m_hWnd) )
// 		return 0;
// 
// 	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
// 		return CDialog::ShowWindow(nCmdShow);
// 
// 
// 	if( IsIconic() )
// 	{
// 		return CDialog::ShowWindow(SW_RESTORE);
// 	}
// 
// 	if(  IsZoomed() ) // max
// 		return CDialog::ShowWindow(nCmdShow);
// 
// 	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
// 	{
// 		this->Invalidate();
// 	}
// 	else
// 	{
// 		return CDialog::ShowWindow(nCmdShow);
// 	}
	return CWnd::ShowWindow(nCmdShow);
}
