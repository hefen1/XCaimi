/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgInputUnionName.h
*	文件标识：	
*	摘	  要：	输入组合条件名窗口
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#include <afxwin.h>
#if !defined(AFX_HSNEWUNIONDLG_H__94F76B2D_0C21_4938_8769_137F037B7231__INCLUDED_)
#define AFX_HSNEWUNIONDLG_H__94F76B2D_0C21_4938_8769_137F037B7231__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInputUnionName.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgInputUnionName dialog

class CDlgInputUnionName : public CDialog
{
// Construction
public:
	CDlgInputUnionName(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CDlgInputUnionName)
	enum { IDD = IDD_NEWUNIONDLG };
	// 新输入的组和名称
	CString	m_strBlockName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInputUnionName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputUnionName)
	// "确定"按钮
	virtual void OnOK();
	// "取消"按钮
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	// 初始化
	virtual BOOL OnInitDialog();
	BOOL ShowWindow( int nCmdShow );
	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSNEWUNIONDLG_H__94F76B2D_0C21_4938_8769_137F037B7231__INCLUDED_)
