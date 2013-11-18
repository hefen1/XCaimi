#pragma once
#include "afxcmn.h"
#include <afxwin.h>

// #ifndef CEdit
// #define CEdit CHSEdit
// #endif

class CKBEdit:public CEdit
{
	DECLARE_DYNAMIC(CKBEdit)
public:
	CKBEdit();
	virtual ~CKBEdit();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHSEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

protected:
	//{{AFX_MSG(CHSEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};