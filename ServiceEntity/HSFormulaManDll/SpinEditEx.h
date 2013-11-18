#if !defined(AFX_SPINEDITEX_H__A0872224_F98B_11D4_906E_5254AB285D5A__INCLUDED_)
#define AFX_SPINEDITEX_H__A0872224_F98B_11D4_906E_5254AB285D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpinEditEx.h : header file
//
#include "HSNumSpinEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CSpinEditEx window

class CValue;
class CNumericValue;
class  CSpinEditEx : public CHSNumSpinEdit
{
// Construction
public:
	CSpinEditEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpinEditEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpinEditEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpinEditEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual long OnEditUpdate(WPARAM wParam, LPARAM lParam);

protected:
	CNumericValue* m_pParam;

public:
	void SetEditDef(CValue* pValue);
	BOOL Create(CWnd* pWnd,CRect rect,UINT nID);
	int	 Width();
	void OffSet(int x,int y);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINEDITEX_H__A0872224_F98B_11D4_906E_5254AB285D5A__INCLUDED_)
