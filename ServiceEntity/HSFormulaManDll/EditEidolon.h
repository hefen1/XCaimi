#if !defined(AFX_MULWND1_H__1D786D53_DE15_4742_BC2A_EC2E829E6541__INCLUDED_)
#define AFX_MULWND1_H__1D786D53_DE15_4742_BC2A_EC2E829E6541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MulWnd1.h : header file
//
#include "ExportDll.h"
#include "ConfigStruct.h"
#include "ExpPublic.h"
#include "SpinEditEx.h"
/*#include "..\HSFormulaCalcLib\HxCompileOut.h" */ // ±‡“Î∆˜

/////////////////////////////////////////////////////////////////////////////
// CEditEidolon window
/*class CSpinEditEx;*/

class BCGCONTROLBARDLLEXPORT CEditEidolon : public CWnd
{
	// Construction
	DECLARE_DYNCREATE(CEditEidolon)
public:
	CEditEidolon();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditEidolon)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CEditEidolon();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditEidolon)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	LRESULT OnCompileUpdateData(WPARAM wp,LPARAM lp);

	DECLARE_MESSAGE_MAP()

protected:
	CString		  m_strOldText;
	CStringArray* m_pArray;

	CArray<CSpinEditEx*,CSpinEditEx*> m_payEditCtrl;
	CMapVariabile*				      m_pParamVar;
	CMapVariabile*					  m_pOldParamVar;//yulx add
protected:
	void	     CreateSpinEdit(int nCount);
	void		 DeleteSpinEdit();
	CSpinEditEx* GetSpinEdit(int nPos);

public:
	void SaveOldPar(CMapVariabile *pmap);//yulx add
	void ResetParam();//yulx add
	void SetText(CString strRefText,CMapVariabile* pParamVar,BOOL bFocus = TRUE);
	void SetText(CStringArray* pArray,CMapVariabile* pParamVar,BOOL bFocus = TRUE);

	void Empty();
	void MakeVisible();

	void MoveWindow(int x, int y, int nWidth, int nHeight,
		BOOL bRepaint = TRUE);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);

protected:
	struct DrawData
	{	
		CRect	  m_rcLeft;
		CRect	  m_rcRight;

		CString	  m_strLeft;		
		CString	  m_strRight;

		CSpinEditEx*	  m_pEdit;

		DrawData()
		{
			Empty();
		}
		void Empty()
		{
			m_rcLeft.SetRectEmpty();
			m_rcRight.SetRectEmpty();
			m_strLeft.Empty();		
			m_strRight.Empty();
			m_pEdit = NULL;
		}
		void Draw(CDC* pDC);
	};

	CArray<DrawData*,DrawData*> m_ayDrawData;
	BOOL  m_bDraw;
	int   m_nRowCount;
	int   m_nDrawHeight;
	int   m_nRowHeight;
	int   m_nScrollPos;
	CSize m_szEdit; // edit size

	char  m_cPrompt;

	FontProperty  m_ftChart;

	void DeleteData();
	DrawData* GetDrawData(CWnd* pWnd);

public:
	void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
	void SetFont(char * cFontName,int nFontSize, int nFontStyle);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULWND1_H__1D786D53_DE15_4742_BC2A_EC2E829E6541__INCLUDED_)
