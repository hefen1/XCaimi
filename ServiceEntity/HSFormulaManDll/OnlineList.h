/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	OnlineList.h
*	�ļ���ʶ��	����(ʵʱ)���빫ʽ����,Ŀǰûʵ��,�ݲ���˵��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#if !defined(AFX_ONLINELIST_H__EE6596E6_4471_479F_82B1_C39784A5022D__INCLUDED_)
#define AFX_ONLINELIST_H__EE6596E6_4471_479F_82B1_C39784A5022D__INCLUDED_

#include "ExpPublic.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnlineList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COnlineList window

class COnlineList : public CListCtrl
{
// Construction
public:
	COnlineList();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlineList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COnlineList();

	// Generated message map functions
protected:
	//{{AFX_MSG(COnlineList)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	BOOL AddToOnlineList(CString& strKey,BOOL bEdit = FALSE,int nBegin = 0,int nEnd = 0);
	void Add(CString& strKey,CMapVariabile* pMapVar);
	void AddParam(CString& strKey,CMapVariabile* pMapVar);

	CWnd* GetParent();

protected:
	int  m_nBegin,m_nEnd;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINELIST_H__EE6596E6_4471_479F_82B1_C39784A5022D__INCLUDED_)
