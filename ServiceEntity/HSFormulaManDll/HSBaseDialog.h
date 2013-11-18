#if !defined(AFX_HSBASEDIALOG_H__8C3ED726_E8F2_11D4_9060_5254AB285D5A__INCLUDED_)
#define AFX_HSBASEDIALOG_H__8C3ED726_E8F2_11D4_9060_5254AB285D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSShadeButton.h"
class  CHSBaseDialog : public CDialog
{
	DECLARE_DYNAMIC(CHSBaseDialog)
	// Construction
public:
	CHSBaseDialog();
	CHSBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	CHSBaseDialog( LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL );

	~CHSBaseDialog();

public:
	BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	BOOL CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd = NULL,
		void* lpDialogInit = NULL);
	BOOL CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd = NULL);


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHSBaseDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy( );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual void OnHelpButton();
	virtual UINT OnNcHitTest(CPoint point);
	virtual void OnNcLButtonDown(UINT nHitTest, CPoint point);

public:
	// Overridables (special message map entries)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	virtual void MyNcDestroy();

protected:
	virtual void OnOK();
	virtual void OnCancel();

public:
	virtual void Delete();
	virtual BOOL ShowWindowEx( int nCmdShow );

public:
	HACCEL m_hAccelTable;
	void   SetThisParam(long* pThis) { m_pThis = pThis;}

protected:
	char   m_cDeleteThis;
	long*  m_pThis;

public:
	virtual void ExitRunModalLoop(DWORD dwFlags = 0);
	int			 RunModalLoop(DWORD dwFlags = 0);

	virtual INT_PTR DoModal();

	BOOL ShowWindow( int nCmdShow );

protected:
	

protected:
	long HSUserMsg(WPARAM wParam, LPARAM lParam);



protected:
	CArray<CButton*,CButton*> m_pArrayBut;
public:
	void		DeleteAllBut();
	CButton*    AddButton(int nID,int nImage = -1,CString strTip = "",long lMask = 0);
	void	    DelButton(int nID);

	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnPaint();
};

/*#define DDX_Control HS_DDX_Control*/

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSBASEDIALOG_H__8C3ED726_E8F2_11D4_9060_5254AB285D5A__INCLUDED_)