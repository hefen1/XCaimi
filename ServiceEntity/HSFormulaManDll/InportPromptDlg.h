/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	InportPromptDlg.h
*	文件标识：	引入公式时,存在重名的引入方式选择窗口类
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_)
#define AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InportPromptDlg.h : header file
//
#include "resource.h"
#include "HSBizDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CInportPromptDlg dialog

class CDlgFormulaImportExport;
class CInportPromptDlg : public CDialog
{
// Construction
public:
	CInportPromptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInportPromptDlg)
	enum { IDD = IDD_PROMPTDLG };
	// 重名时的操作方式(0覆盖,1自动命名,2改名)
	int		m_nType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInportPromptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInportPromptDlg)
	// 确定处理
	virtual void OnOK();
	// 退出处理
	virtual void OnCancel();
	// 对话框初始化处理
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPORTPROMPTDLG_H__C66A57EC_65BE_4A91_B1F3_C93E91544C06__INCLUDED_)
