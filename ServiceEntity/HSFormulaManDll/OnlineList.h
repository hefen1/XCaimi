/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	OnlineList.h
*	文件标识：	在线(实时)翻译公式定义,目前没实现,暂不做说明
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
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
