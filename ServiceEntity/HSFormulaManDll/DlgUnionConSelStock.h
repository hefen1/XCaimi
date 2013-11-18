/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgUnionConSelStock.h
*	�ļ���ʶ��	�������ѡ��,Ŀǰ��ʹ��,�ݲ�˵��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_HSCONUNION_H__1785DBEE_1652_4A18_9EAF_850448E139A0__INCLUDED_)
#define AFX_HSCONUNION_H__1785DBEE_1652_4A18_9EAF_850448E139A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DlgUnionConSelStock.h : header file
//
#include "ExpPublic.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgUnionConSelStock dialog

class CDlgUnionConSelStock : public CDialog
{
public:
// Construction
public:
	CDlgUnionConSelStock(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgUnionConSelStock)
	enum { IDD = IDD_CONDITION_UNION };
	CEdit	m_wndEndDate;
	CTreeCtrl	m_wndTree;
	CEdit	m_wndName;
	CEdit	m_wndBeginDate;
	CButton	m_wndAdvance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUnionConSelStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUnionConSelStock)
	afx_msg void OnNewset();
	afx_msg void OnNewadd();
	afx_msg void OnNew();
	afx_msg void OnDelete();
	afx_msg void OnAnd();
	afx_msg void OnAdvance();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdate();
	afx_msg void OnOr();
	afx_msg void OnClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	UnionSelCondition* m_pHSUnion;
	BOOL	  m_bCreate;

protected:
	void Delete();
	void Insert(CTreeCtrl* pTree,UnionSelCondition* pHSUnion);
	int	 DeleteItemData(UnionSelCondition* pHSUnion,void* pDelData);

	HTREEITEM AddUnionGroup(UnionSelConditionSub* pUnionGroup);
	BOOL ShowWindow( int nCmdShow );

public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSCONUNION_H__1785DBEE_1652_4A18_9EAF_850448E139A0__INCLUDED_)
