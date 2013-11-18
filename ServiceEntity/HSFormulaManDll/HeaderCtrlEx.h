/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	HeaderCtrlEx.h
*	文件标识：	自定义列表头类
*	摘	  要：	主要是为了对右键单列表头的消息响应做处理而派生,用于CBaseListCtrl类
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_)
#define AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeaderCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx window

class CHeaderCtrlEx : public CHeaderCtrl
{
// Construction
public:
	CHeaderCtrlEx(int nKey = 0);
	// 分类ID(对消息做不同处理)
	// m_nKey == 0 : 弹出板块分析项目修改菜单
	// m_nKey == 1 : 弹出热门板块分析菜单( 目前此功能已不用 )
	int m_nKey; 

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeaderCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHeaderCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeaderCtrlEx)
	// 根据m_nKey弹出不同的菜单
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	// 以下全是调用了基类的处理函数
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEADERCTRLEX_H__F692F0CE_106C_4B59_A96A_B5041F077FF0__INCLUDED_)
