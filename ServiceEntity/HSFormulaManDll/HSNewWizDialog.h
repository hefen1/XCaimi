#if !defined(AFX_NEWWIZDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_NEWWIZDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewWizDialog.h : header file
//


#include "HSBaseDialog.h"
class CHSNewWizPage;

/////////////////////////////////////////////////////////////////////////////
// CHSNewWizDialog dialog

class  CHSNewWizDialog : public CHSBaseDialog
{
	// Construction
public:
	CHSNewWizDialog(CWnd* pParent = NULL);
	CHSNewWizDialog (LPCTSTR lpszTemplate, CWnd* pParent = NULL);
	CHSNewWizDialog (UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CHSNewWizDialog ();
	
	virtual INT_PTR DoModal();
	
	// Attributes
public:
	
	
protected:
	CObList m_PageList; // this list of CHSNewWizPage objects
	UINT m_nPlaceholderID; // control ID for the palceholder
	
	
	// Operations
public:
	void AddPage(CHSNewWizPage* pPage, UINT nID);
	
	void SetActivePageByResource(UINT nResourceID);
	BOOL SetFirstPage();
	BOOL SetLastPage();
	void SetNextPage();
	
	void EnableFinish(BOOL bEnable);
	void EnableBack(BOOL bEnable);
	void EnableNext(BOOL bEnable);
	int GetActiveIndex() const;
	int GetPageIndex(CHSNewWizPage* pPage) const;
	int GetPageCount();
	CHSNewWizPage* GetPage(int nPage) const;
	BOOL SetActivePage(int nPage);
	BOOL SetActivePage(CHSNewWizPage* pPage);
	void SetTitle(LPCTSTR lpszText);
	void SetTitle(UINT nIDText);
	void SetFinishText(LPCTSTR lpszText);
	void SetFinishText(UINT nIDText);
	
public:
	BOOL ShowWindow(int nCmdShow);

public:
	BOOL ActivatePage(CHSNewWizPage* pPage,int nPos = -1);
	BOOL DeactivatePage(int nPos = -1);
	void SetPlaceholderID(int nPlaceholderID);
	CHSNewWizPage* GetPageByResourceID(UINT nResourceID);
	
	
protected:
	void Init();
	BOOL DestroyPage(CHSNewWizPage* pPage);
	CHSNewWizPage* GetFirstPage();
	CHSNewWizPage* GetLastPage();
	CHSNewWizPage* GetActivePage() const;
	CHSNewWizPage* GetNextPage();
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSNewWizDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CHSNewWizDialog)
	afx_msg void OnDestroy();
	virtual void OnWizardFinish();
	virtual void OnWizardBack(); 
	virtual void OnWizardNext(); 
	virtual void OnCancel(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWWIZDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
