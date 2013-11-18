/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgError.h
*	�ļ���ʶ��	���������Ϣ�б���
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_ERRORDLG_H__D9011B33_16E1_11D5_A127_000000000000__INCLUDED_)
#define AFX_ERRORDLG_H__D9011B33_16E1_11D5_A127_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgError.h : header file
//
//#include "ETSLayout.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgError dialog

class CCompileEditView;
class CDlgError : public CDialog
{
// Construction
public:
	CDlgError(CWnd* pParent = NULL,CCompileEditView* pRichEdit = NULL);   // standard constructor

	/* DialogMgr: Add this: */
	//DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgError)
	enum { IDD = IDD_ERRORDLG };
	CListCtrl	m_wndList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgError)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgError)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	
	long AddCompileInfotoList(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()

public:
	CCompileEditView* m_pRichEdit;
	CWnd*			  m_pParentDlg;
protected:
	CImageList	m_imaSmall;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��մ�����Ϣ ����
	void ClearError();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORDLG_H__D9011B33_16E1_11D5_A127_000000000000__INCLUDED_)
