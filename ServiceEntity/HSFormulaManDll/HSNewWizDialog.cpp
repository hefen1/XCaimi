// NewWizDialog.cpp : implementation file
//

#include "stdafx.h"

#include "HSNewWizDialog.h"
#include "HSNewWizPage.h"

#include <afxpriv.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CHSNewWizDialog dialog


CHSNewWizDialog::CHSNewWizDialog(CWnd* pParent /*= NULL*/) 
{
	Init();
}

CHSNewWizDialog::CHSNewWizDialog(LPCTSTR lpszTemplateName, 
								   CWnd* pParent):CHSBaseDialog(lpszTemplateName,pParent)
{
	Init();
}

CHSNewWizDialog::CHSNewWizDialog(UINT nIDTemplate, 
								   CWnd* pParent):CHSBaseDialog(nIDTemplate,pParent)
{
	Init();
}

CHSNewWizDialog::~CHSNewWizDialog()
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		delete pPage;
	}
}


INT_PTR CHSNewWizDialog::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	// can be constructed with a resource template or InitModalIndirect
	ASSERT(m_lpszTemplateName != NULL || m_hDialogTemplate != NULL ||
		m_lpDialogTemplate != NULL);

	// load resource as necessary
	LPCDLGTEMPLATE lpDialogTemplate = m_lpDialogTemplate;
	HGLOBAL hDialogTemplate = m_hDialogTemplate;
	HINSTANCE hInst = AfxGetResourceHandle();
	if (m_lpszTemplateName != NULL)
	{
		hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
		HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
		hDialogTemplate = LoadResource(hInst, hResource);
	}
	if (hDialogTemplate != NULL)
		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

	// return -1 in case of failure to load the dialog template resource
	if (lpDialogTemplate == NULL)
		return -1;

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);
		if (CreateDlgIndirect(lpDialogTemplate,
			CWnd::FromHandle(hWndParent), hInst))
		{
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
					dwFlags |= MLF_NOIDLEMSG;
				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
				SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary
	if (m_lpszTemplateName != NULL || m_hDialogTemplate != NULL)
		UnlockResource(hDialogTemplate);
	if (m_lpszTemplateName != NULL)
		FreeResource(hDialogTemplate);

	return m_nModalResult;

	//return CHSBaseDialog::DoModal();
}

void CHSNewWizDialog::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHSNewWizDialog)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHSNewWizDialog, CHSBaseDialog)
//{{AFX_MSG_MAP(CHSNewWizDialog)
ON_WM_DESTROY()
ON_BN_CLICKED(ID_WIZFINISH, OnWizardFinish)
ON_BN_CLICKED(ID_WIZBACK, OnWizardBack)
ON_BN_CLICKED(ID_WIZNEXT, OnWizardNext)
ON_BN_CLICKED(IDCANCEL, OnCancel)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHSNewWizDialog message handlers


void CHSNewWizDialog::Init()
{
	m_nPlaceholderID = 0;
}


BOOL CHSNewWizDialog::DestroyPage(CHSNewWizPage* pPage)
{
	if (pPage->m_bCreated)
	{
		if (pPage->OnKillActive() == FALSE) return FALSE;
		pPage->OnDestroyPage();
		pPage->DestroyWindow();
		pPage->m_bCreated = FALSE;
	}
	
	// return TRUE even if the page was never created
	return TRUE;
}



CHSNewWizPage* CHSNewWizDialog::GetFirstPage()
{
	return (CHSNewWizPage*)m_PageList.GetHead();	
}


CHSNewWizPage* CHSNewWizDialog::GetLastPage()
{
	return (CHSNewWizPage*)m_PageList.GetTail();	
}


CHSNewWizPage* CHSNewWizDialog::GetActivePage() const
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			return pPage;
		}
	}
	return NULL;
}


// function to get the next page
CHSNewWizPage* CHSNewWizDialog::GetNextPage()
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			if (Pos == NULL) return NULL;
			return (CHSNewWizPage*)m_PageList.GetAt(Pos);
		}
	}
	return NULL;
}


void CHSNewWizDialog::SetPlaceholderID(int nPlaceholderID)
{
	m_nPlaceholderID = nPlaceholderID;
}


// returns TRUE if the new page is activated
BOOL CHSNewWizDialog::ActivatePage(CHSNewWizPage* pPage,int nPos /*= -1*/)
{
	ASSERT(m_nPlaceholderID != 0);
	ASSERT(pPage != NULL);
	ASSERT(::IsWindow(m_hWnd));
	
	// if the page has not been created, then create it
	if (pPage->m_bCreated == FALSE)
	{
		if (pPage->Create(pPage->m_nDialogID, this) == FALSE) return FALSE;
		pPage->m_bCreated = TRUE;
		pPage->m_pParent = this;
		
		if (pPage->OnCreatePage() == FALSE) return FALSE;
	}

	CHSNewWizPage* pOldPage = GetActivePage();

	// deactivate the current page
	if (!DeactivatePage(nPos)) return FALSE;
	
	CRect rect;
	CWnd *pWnd = GetDlgItem(m_nPlaceholderID);
	ASSERT(pWnd != NULL);
	ASSERT(IsWindow(pWnd->m_hWnd) != FALSE);
	
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pPage->MoveWindow(rect);
	//pPage->SetWindowPos(NULL, rect.left, rect.top, 0, 0, 
	//                     SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
	pPage->EnableWindow(TRUE);
	
	pPage->ShowWindow(SW_SHOW);
	pPage->InvalidateRect(NULL);
	pPage->UpdateWindow();
	pPage->OnSetActive();
	
	pPage->OnSetActive(pOldPage,nPos); //

	pPage->m_bActive = TRUE;
	return TRUE;
}


BOOL CHSNewWizDialog::DeactivatePage(int nPos /*= -1*/)
{
	CHSNewWizPage* pPage = GetActivePage();
	if (pPage == NULL) return TRUE;
	
	ASSERT(pPage->m_bCreated != FALSE);
	
	if (!pPage->OnKillActive(nPos)) 
		return FALSE;

	pPage->ShowWindow(SW_HIDE);
	pPage->m_bActive = FALSE;
	return TRUE;
}


BOOL CHSNewWizDialog::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();
	ModifyStyleEx (0, WS_EX_CONTROLPARENT);	
	
	ASSERT(m_nPlaceholderID != 0); // Be sure to call SetPlaceholderID from
	// your dialogs OnInitDialog
	
	// make the first page of the wizard active
	SetFirstPage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHSNewWizDialog::OnDestroy() 
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		
		// this function could return FALSE, but if it happens here
		// it is too late to do anything about it!
		VERIFY(DestroyPage(pPage));
	}
	
	CHSBaseDialog::OnDestroy();
}


// function to add a page to the the list of pages
// nID is the reource ID of the page we are adding
void CHSNewWizDialog::AddPage(CHSNewWizPage* pPage, UINT nID)
{
	m_PageList.AddTail(pPage);
	pPage->m_nDialogID = nID;
}



// Activate the page with the specified dialog resource
void CHSNewWizDialog::SetActivePageByResource(UINT nResourceID)
{
	CHSNewWizPage* pPage = GetPageByResourceID(nResourceID);
	if (pPage == NULL) return;
	
	if (!DeactivatePage()) return;
	
	ActivatePage(pPage);
}


// function to return a page object based on it's dialog resource ID
CHSNewWizPage* CHSNewWizDialog::GetPageByResourceID(UINT nResourceID)
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_nDialogID == nResourceID)
		{
			return pPage;
		}
	}
	return NULL;
}

BOOL CHSNewWizDialog::SetLastPage()
{
	CHSNewWizPage* pPage = GetLastPage();
	
	if (!DeactivatePage()) return FALSE;
	
	EnableFinish(TRUE);
	EnableNext(FALSE);
	if (m_PageList.GetCount() > 1)
	{
		EnableBack(TRUE);
	}
	else // wizard has only one page
	{
		EnableBack(FALSE);
	}
	
	return ActivatePage(pPage);
}

// Set first page as active page
BOOL CHSNewWizDialog::SetFirstPage()
{
	CHSNewWizPage* pPage = GetFirstPage();
	
	if (!DeactivatePage()) return FALSE;
	
	EnableBack(FALSE);
	
	if (m_PageList.GetCount() > 1)
	{
		EnableFinish(FALSE);
		EnableNext(TRUE);
	}
	else // wizard has only one page
	{
		EnableFinish(TRUE);
		EnableNext(FALSE);
	}
	
	if (ActivatePage(pPage)) return TRUE;
	return FALSE;
}


// Set next page as active page
void CHSNewWizDialog::SetNextPage()
{
	CHSNewWizPage* pPage = GetNextPage();
	if (ActivatePage(pPage))
	{
		EnableBack(TRUE);
	}
}


/////////////////////////////////////////////////////////////
//  CNewWizDlg Button Processing


// user pressed the Finish button
void CHSNewWizDialog::OnWizardFinish() 
{
	CHSNewWizPage* pPage;
	
	pPage = GetActivePage();
	
	// can we kill the active page?
	if (!pPage->OnKillActive()) return;
	
	
	// notify all pages that we are finishing
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bCreated)
		{
			if (!pPage->OnWizardFinish())
			{
				// data validation failed for one of the pages so we can't close
				return;
			}
		}
	}
	
	// The only reason this line would be needed is if you had controls
	// place in your main wizard dialog outside of the wizard pages.
	// In most "normal" implementations, this line does nothing
	UpdateData(TRUE);
	
	// close the dialog and return ID_WIZFINISH
	CHSBaseDialog::EndDialog(ID_WIZFINISH);
}




void CHSNewWizDialog::OnWizardBack() 
{
	CHSNewWizPage* pPage = GetActivePage();
	ASSERT(pPage);
	
	LRESULT lResult = pPage->OnWizardBack();
	if (lResult == -1) return; // don't change pages
	
	if (lResult == 0)
	{
		POSITION Pos = m_PageList.Find(pPage);
		ASSERT(Pos);
		m_PageList.GetPrev(Pos);
		if (Pos == NULL) return; // the current page was the first page
		pPage = (CHSNewWizPage*)m_PageList.GetAt(Pos);
	}
	else
	{
		pPage = GetPageByResourceID(lResult);
		if (pPage == NULL) return;
	}
	
	if (!ActivatePage(pPage,0)) return;	
	
	// if we are on the first page, then disable the back button
	if (pPage == GetFirstPage())
	{
		EnableBack(FALSE);
		EnableFinish(FALSE);
	}
	
	// enable the next button
	EnableNext(TRUE);
}


void CHSNewWizDialog::OnWizardNext() 
{
	CHSNewWizPage* pPage = GetActivePage();
	ASSERT(pPage);
	
	LRESULT lResult = pPage->OnWizardNext();
	if (lResult == -1) return; // don't change pages
	
	if (lResult == 0)
	{
		POSITION Pos = m_PageList.Find(pPage);
		ASSERT(Pos);
		m_PageList.GetNext(Pos);
		if (Pos == NULL) return; // the current page was the last page
		pPage = (CHSNewWizPage*)m_PageList.GetAt(Pos);
	}
	else
	{
		pPage = GetPageByResourceID(lResult);
		if (pPage == NULL) return;
	}
	
	if (!ActivatePage(pPage,1)) return;	
	
	// if we are on the last page, then disable the next button
	if (pPage == GetLastPage())
	{
		EnableNext(FALSE);
		EnableFinish(TRUE);
	}
	EnableBack(TRUE);
}


void CHSNewWizDialog::EnableFinish(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZFINISH);
	ASSERT(pWnd); // You must have an ID_WIZFINISH on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}

/*****************************************************
* 编写: 熊钟彬 2002 06 14
* 注释: 显示向导,并且切换到初始页面 
*       public
******************************************************/
BOOL CHSNewWizDialog::ShowWindow(int nCmdShow)
{
	SetActivePage(0);
	EnableBack(FALSE);
	EnableFinish(FALSE);
	//::ShowWindow(m_hWnd,/*nCmdShow*/SW_SHOWNORMAL);

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CHSBaseDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CHSBaseDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CHSBaseDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CHSBaseDialog::ShowWindow(nCmdShow);
	}

	return 1;
}

void CHSNewWizDialog::EnableBack(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZBACK);
	ASSERT(pWnd); // You must have an ID_WIZBACK on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}


void CHSNewWizDialog::EnableNext(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZNEXT);
	ASSERT(pWnd); // You must have an ID_WIZNEXT on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}


///////////////////////////////////////////////////////////////
//  Functions to mimic the behavior of CPropertySheet


// returns the index of the currently active page
int CHSNewWizDialog::GetActiveIndex() const
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	int nIndex = 0;
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			return nIndex;
		}
		++nIndex;
	}
	return -1;
}


int CHSNewWizDialog::GetPageIndex(CHSNewWizPage* pPage) const
{
	CHSNewWizPage* pTestPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	int nIndex = 0;
	while (Pos)
	{
		pTestPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pTestPage == pPage)
		{
			return nIndex;
		}
		++nIndex;
	}
	return -1;
}


int CHSNewWizDialog::GetPageCount()
{
	return m_PageList.GetCount();
}


// get a page based on it's placement index in the list
CHSNewWizPage* CHSNewWizDialog::GetPage(int nPage) const
{
	POSITION Pos = m_PageList.FindIndex(nPage);
	if (Pos == NULL) return NULL;
	return (CHSNewWizPage*)m_PageList.GetAt(Pos);
}



// activate a page based on its place in the list
BOOL CHSNewWizDialog::SetActivePage(int nPage)
{
	CHSNewWizPage* pPage = GetPage(nPage);
	if (pPage == NULL) return FALSE;
	ActivatePage(pPage);
	return TRUE;
}


BOOL CHSNewWizDialog::SetActivePage(CHSNewWizPage* pPage)
{
	ActivatePage(pPage);
	return TRUE;
}


// set the title of the main wizard window
void CHSNewWizDialog::SetTitle(LPCTSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	SetWindowText(lpszText);
}


void CHSNewWizDialog::SetTitle(UINT nIDText)
{
	CString s;
	s.LoadString(nIDText);
	SetTitle(s);
}


// set the text on the Finish button
void CHSNewWizDialog::SetFinishText(LPCTSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZFINISH);
	ASSERT(pWnd); // You must have an ID_WIZFINISH on your dialog
	if (pWnd)
	{
		pWnd->SetWindowText(lpszText);		
	}
}

void CHSNewWizDialog::SetFinishText(UINT nIDText)
{
	CString s;
	s.LoadString(nIDText);
	SetFinishText(s);	
}


// user pressed the cancel button
void CHSNewWizDialog::OnCancel()
{
	CHSNewWizPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bCreated)
		{
			// can we cancel?
			if (pPage->OnQueryCancel() == FALSE) return;
		}
	}
	
	CHSBaseDialog::OnCancel();
}