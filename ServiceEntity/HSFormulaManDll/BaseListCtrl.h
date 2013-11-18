/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	BaseListCtrl.h
*	文件标识：	列表基类的派生类
*	摘	  要：	用于板块分析列表及函数管理器列表中
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
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
	// 图标列表
	CImageList	  m_imaSmall;

public:
	// 初始化,主要是创建m_imaSmall及设置列表的图标
	void Init();

	// header
protected:
	// 自己的列头控件
	CHeaderCtrlEx* m_pWndHeader;
public:

	/******************************************************************
	*	函数功能：	加自己的列头控件
	*	函数参数：	int nKey : [in] 列头控件的分类ID
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void AddHeader(int nKey);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASELISTCTRL_H__99136E57_137F_11D5_A10C_000000000000__INCLUDED_)
