/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAddProjectToBlock.h
*	文件标识：	"板块分析"功能模块--"项目管理"
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/

#if !defined(AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_)
#define AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddProjectToBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddProjectToBlock dialog
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"

class CDlgFormulaEdit;
class CExpTabCtrl;

class CDlgAddProjectToBlock : public CDialog
{
// Construction
public:
	enum { Condition,Sort };

	/******************************************************************
	*	函数功能：	
	*	函数参数：	CWnd* pParent = NULL : [in] 父窗口
	*				int iFormat = 1		 : [in] 显示格式 
	*				int iType = 0		 : [in] 统计方式
	*				LPCSTR strColName = "" : [in] 列名
	*				LPCSTR strDesc = ""	   : [in] 描述
	*				LPCSTR strWinText = "" : [in] 窗口名
    *	返 回 值：	无
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月26日
	*	修改说明：	
	*****************************************************************/
	CDlgAddProjectToBlock(CWnd* pParent = NULL, 
		int iFormat = 1, 
		int iType = 0, 
		LPCSTR strColName = "", 
		LPCSTR strDesc = "", 
		LPCSTR strWinText = ""
		);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddProjectToBlock)
	enum { IDD = IDD_ADDPROJECT };
	// 显示格式组合框控件
	CComboBox	m_comboFormat;
	// 统计方式组合框控件
	CComboBox	m_conboTotal;
	// 列名
	CString	m_name;
	// 列描述
	CString	m_desc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddProjectToBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddProjectToBlock)
	virtual BOOL OnInitDialog();
	// OK按键处理
	virtual void OnOK();
	// OnCancle处理
	afx_msg void OnExit();
	// 选择公式按键处理
	afx_msg void OnSelectExpress();
	// 格式组合框控件改变选择处理
	afx_msg void OnSelchangeComFormat();
	// 统计方式组合框控件改变选择处理
	afx_msg void OnSelchangeComTotal();
	// 改变列名处理
	afx_msg void OnChangeName();
	// 窗口销毁时处理
	afx_msg void OnDestroy();
	// 新建按钮处理
	afx_msg void OnNewExpress();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	// 指向公式管理窗口
	CWnd	*m_Form;
	// 当前列名
	CString m_strCurName;
	// 当前列描述
	CString m_strCurDesc;
	// 窗口标题
	CString m_strWinText;
	// 当前格式
	int		m_iCurFormat;
	// 当前统计类型
	int		m_iCurType;

	// ??? 没有什么用处,最好去除
	CExpTabCtrl	m_wndTab;
	// ??? 没有什么用处,最好去除
	void Init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSADDPROJECTDLG_H__F0CAF6BA_9733_4924_8918_DA94B3EBB806__INCLUDED_)
