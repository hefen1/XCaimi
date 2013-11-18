/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	ExpTabCtrl.h
*	文件标识：	公式TAB控件
*	摘	  要：	一般包括了四个TAB项"分组""所有""常用""自定义",
				TAB中包括了一个公式树控件
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
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
*	类 名 称：	CExpTabCtrl
*	类 描 述：	公式树TAB
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
	// 创建消息处理
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	// 消毁窗口处理
	afx_msg void OnDestroy();
	// 改变大小
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 相应键盘按下消息
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	// 相应单击消息
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	// TAB里的树控件
	CTreeCtrlFormula	m_wndTree;
public:
	// 得以树控件m_wndTree的指针
	CTreeCtrlFormula* GetTree();

	// TAB改变(开始)
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	// 以下不实现,只调用了父类函数
	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnFocusChange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);

	// 初始化树

	/******************************************************************
	*	函数功能：	根据m_wndTree里面的类型,从公式管理器中
					得到相应类型的公式并放到树m_wndTree中
	*	函数参数：	int nCopy = CTreeCtrlFormula::CopyDelete : [in] 公式的复制方式及是否只COPY常用及自定义公式
	*	返 回 值：	void 
	*	调用依赖：	CTreeCtrlFormula的服务
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void InitTree(int nCopy = CTreeCtrlFormula::CopyDelete);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPTABCTRL_H__AA3B03AE_5EC7_495B_9878_D25D2676AEB2__INCLUDED_)
