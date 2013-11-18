/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	dlgnotesAndEidolon.h
*	文件标识：	公式注释及参数精灵窗口类定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_)
#define AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTechExpress.h : header file
//

#include "EditEidolon.h"
#include "HSBaseDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgNotesAndEidolon dialog

//#include "EditEidolon.h"

class CDlgNotesAndEidolon : public /*CDialog*/ CHSBaseDialog
{
//	DECLARE_DYNCREATE(CDlgNotesAndEidolon)

// Construction
public:
	CDlgNotesAndEidolon(CWnd* pParent = NULL);
	~CDlgNotesAndEidolon();

	// 创建
	BOOL Create(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CDlgNotesAndEidolon)
	enum { IDD = IDD_NOTEANDEIDOLON };
	// 公式注释(文本)
	CEdit	m_pEditCtrl;
	// 参数精灵(文本)
	CEdit	m_pEidolonCtrl;
	// 参数修改(非文本)
	CEditEidolon m_EditEidolon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgNotesAndEidolon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgNotesAndEidolon)
	// 取消处理
	virtual void OnCancel();
	// 确定处理
	virtual void OnOK();
	// 初始化
	virtual BOOL OnInitDialog();
	// 改变大小处理
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 只调用了父类,优化时可删除 ??? 
	afx_msg void OnDestroy();
	// 只调用了父类,优化时可删除 ??? 
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG

	// 公式更新,同时更新公式相关的数据计算
	LRESULT OnCompileUpdateData(WPARAM wp,LPARAM lp);

	DECLARE_MESSAGE_MAP()

public:
	// 控件类型
	enum EditType {Notes/*注释*/,EidolonEdit/*参数精灵*/,Eidolon/*参数修改*/};

	// 设置控件类型
	void SetType(EditType nType) { m_nType = nType;}
	// 判断控件类型
	BOOL IsType (EditType nType) { return (m_nType == nType); }

	// 公式指针,控件所要显示的公式
	CExpression* m_pExpression;

	/******************************************************************
	*	函数功能：	设置公式注释信息
	*	函数参数：	CString strNotes	: [in] 注释内容
	*				CString strCaption  : [in] 窗口标题
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月13日
	*	修改说明：	
	*****************************************************************/
	void SetNotes(CString strNotes,CString strCaption);

	/******************************************************************
	*	函数功能：	设置参数精灵信息
	*	函数参数：	CString strNotes	: [in] 参数精灵信息
	*				CString strCaption	: [in] 窗口标题
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月13日
	*	修改说明：	
	*****************************************************************/
	void SetEidolonEdit(CString strNotes,CString strCaption);

	/******************************************************************
	*	函数功能：	设置参数修改界面(根据公式中的精数精灵文本信息及参数信息)
	*	函数参数：	CString strText/CStringArray* pArray : [in] 精数精灵文本信息
	*				CMapVariabile* pParamVar : [in] 参数信息
	*				CString strCaption : [in] 窗口标题
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月13日
	*	修改说明：	
	*****************************************************************/
	void SetEidolon(CString strText,CMapVariabile* pParamVar,CString strCaption);
	void SetEidolon(CStringArray* pArray,CMapVariabile* pParamVar,CString strCaption);
 
	// 清空当前显示内容
	void	Empty();
	// 得到当前显示内容
	CString GetText();

protected:
	// 控件类型
	EditType m_nType;
public:
	// 改变窗口大小时同时改变控件大小
	void SetSize();
	// 显示控件pWnd, 并设置窗口标题为strCaption
	void Show(CString strCaption,CWnd* pWnd);
	// 目前不起做用,优化时是否可删除 ??? 
	void Focus();

public:
	// 目前起做用的只是调用了基类的ShowWindow,优化时是否可删除 ??? 
	virtual BOOL ShowWindow( int nCmdShow );

	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTECHEXPRESS_H__A38EA223_35F4_11D3_97B1_0000E821282D__INCLUDED_)
