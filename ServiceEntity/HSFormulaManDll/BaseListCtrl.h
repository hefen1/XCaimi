/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	BaseListCtrl.h
*	�ļ���ʶ��	�б�����������
*	ժ	  Ҫ��	���ڰ������б������������б���
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_BASELISTCTRL_H__99136E57_137F_11D5_A10C_000000000000__INCLUDED_)
#define AFX_BASELISTCTRL_H__99136E57_137F_11D5_A10C_000000000000__INCLUDED_

#include "HeaderCtrlEx.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseListCtrl window

#undef SubclassWindow

class CHeaderCtrlEx;
class CBaseListCtrl : public CListCtrl
{
// Construction
public:
	CBaseListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	// ͼ���б�
	CImageList	  m_imaSmall;

public:
	// ��ʼ��,��Ҫ�Ǵ���m_imaSmall�������б��ͼ��
	void Init();

	// header
protected:
	// �Լ�����ͷ�ؼ�
	CHeaderCtrlEx* m_pWndHeader;
public:

	/******************************************************************
	*	�������ܣ�	���Լ�����ͷ�ؼ�
	*	����������	int nKey : [in] ��ͷ�ؼ��ķ���ID
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��5��7��
	*	�޸�˵����	
	*****************************************************************/
	void AddHeader(int nKey);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASELISTCTRL_H__99136E57_137F_11D5_A10C_000000000000__INCLUDED_)
