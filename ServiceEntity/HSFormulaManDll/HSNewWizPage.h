#if !defined(AFX_NEWWIZPAGE_H__915D4C8B_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_NEWWIZPAGE_H__915D4C8B_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewWizPage.h : header file
//



class CHSNewWizDialog;

/////////////////////////////////////////////////////////////////////////////
// CHSNewWizPage dialog

class  CHSNewWizPage : public CDialog
{
	friend class CHSNewWizDialog;
	
	// Construction
public:
	CHSNewWizPage(CWnd* pParent = NULL);   // standard constructor
	
	CHSNewWizPage(LPCTSTR lpszTemplateName, CWnd* pParent = NULL);
	CHSNewWizPage(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CHSNewWizPage();
	
	// Attributes
public:
	CFont m_LargeFont; // a large font for any titles we may want
	
protected:
	//CBrush m_Brush; // brush for white background
	CHSNewWizDialog *m_pParent; // Parent dialog
	
private:
	BOOL m_bCreated; // flag to tell us if the dialog has been created
	BOOL m_bActive;		// flag to tell is if the dialog is the active page
	UINT m_nDialogID; // resource ID for thie page
	
public:
	UINT GetDialogID() { return m_nDialogID; }
	// Operations
public:
	virtual BOOL OnCreatePage();
	virtual void OnDestroyPage();
	
	// these functions are the same as CPropertyPage
	virtual void	OnCancel();
	virtual BOOL	OnKillActive(int nPos = -1);
	virtual void	OnSetActive();
	virtual BOOL    OnQueryCancel( );
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
	virtual BOOL	OnWizardFinish();
	
	virtual void OnSetActive(CHSNewWizPage* pOldPage,int nPos = -1);
	BOOL ShowWindow( int nCmdShow );
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSNewWizPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CHSNewWizPage)
	virtual BOOL OnInitDialog();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWWIZPAGE_H__915D4C8B_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
