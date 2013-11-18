/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExpTabCtrl.h
*	�ļ���ʶ��	��ʽTAB�ؼ�
*	ժ	  Ҫ��	һ��������ĸ�TAB��"����""����""����""�Զ���",
				TAB�а�����һ����ʽ���ؼ�
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_EXPTABCTRL_H__AA3B03AE_5EC7_495B_9878_D25D2676AEB2__INCLUDED_)
#define AFX_EXPTABCTRL_H__AA3B03AE_5EC7_495B_9878_D25D2676AEB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpTabCtrl.h : header file
//
#include "TreeCtrlFormula.h"

/////////////////////////////////////////////////////////////////////////////
// CExpTabCtrl window

/*************************************************************
*	�� �� �ƣ�	CExpTabCtrl
*	�� �� ����	��ʽ��TAB
***************************************************************/
class CExpTabCtrl : public CTabCtrl
{
// Construction
public:
	CExpTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpTabCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExpTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExpTabCtrl)
	// ������Ϣ����
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// ���ٴ��ڴ���
	afx_msg void OnDestroy();
	// �ı��С
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��Ӧ���̰�����Ϣ
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	// ��Ӧ������Ϣ
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	// TAB������ؼ�
	CTreeCtrlFormula	m_wndTree;
public:
	// �������ؼ�m_wndTree��ָ��
	CTreeCtrlFormula* GetTree();

	// TAB�ı�(��ʼ)
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	// ���²�ʵ��,ֻ�����˸��ຯ��
	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnFocusChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);

	// ��ʼ����

	/******************************************************************
	*	�������ܣ�	����m_wndTree���������,�ӹ�ʽ��������
					�õ���Ӧ���͵Ĺ�ʽ���ŵ���m_wndTree��
	*	����������	int nCopy = CTreeCtrlFormula::CopyDelete : [in] ��ʽ�ĸ��Ʒ�ʽ���Ƿ�ֻCOPY���ü��Զ��幫ʽ
	*	�� �� ֵ��	void 
	*	����������	CTreeCtrlFormula�ķ���
	*	��    �ߣ�	
	*	������ڣ�	2003��4��29��
	*	�޸�˵����	
	*****************************************************************/
	void InitTree(int nCopy = CTreeCtrlFormula::CopyDelete);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPTABCTRL_H__AA3B03AE_5EC7_495B_9878_D25D2676AEB2__INCLUDED_)
