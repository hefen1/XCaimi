/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFormulaImportExport.h
*	文件标识：	导入导出公式窗口定义 
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_)
#define AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFormulaImportExport.h : header file
//
#include "TreeCtrlFormula.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaImportExport dialog

class CDlgFormulaMan;
class CDlgFormulaImportExport : public CDialog
{
// Construction
public:
	CDlgFormulaImportExport(CWnd* pParent = NULL,long* pThis = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFormulaImportExport)
	enum { IDD = IDD_INPORTEXPORT };
	// 完全加密
	CButton	m_wndAllPWD;
	// 密码输入框
	CEdit	m_wndIdeaEdit;
	// 定向加密
	CButton	m_wndIdeaPWD;
	// 同名公式内容
	CButton	m_wndSameBut;
	// 公式定义体显示控件
	CEdit	m_wndComment;
	// 公式树控件
	CTreeCtrlFormula	m_wndExpTree;
	// 公式描述显示控件
	CStatic	m_wndName;
	// 公式注释显示控件
	CEdit	m_wndNote;
	// 是否使用密码
	BOOL	m_bIdeaPWD;
	// 定向密码字串
	CString	m_strIdeaPWD;
	// 是否完全加密
	BOOL	m_bAllPWD;
	// 提示说明
	CString	m_strWant;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFormulaImportExport)
	public:
	// 消息预处理,截下了VK_ESCAPE,VK_RETURN,VK_F2的KEY消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFormulaImportExport)
	// "确定"按钮处理,根据m_bInport状态,输入或是输出公式数据
	virtual void OnOK();
	// "取消"按钮处理, 删除输入输出私有公式表
	virtual void OnCancel();
	// "同名公式内容"按钮处理, 此功能没有使用
	afx_msg void OnFindsame();
	// 界面初始化
	virtual BOOL OnInitDialog();
	//}}AFX_MSG


	/******************************************************************
	*	函数功能：	好象没有使用,同名时,目前采用弹出"重名处理"框来选择处理方式
					设置公式相关信息,如果重名,则提示重名要求改名的提示
	*	函数参数：	WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] CTreeGroup*型数据
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：
	*	修改说明：	
	*****************************************************************/
	long OnSendAddStr(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()

public:
	// 公式管理器窗口指针
	CDlgFormulaMan*				     m_pFormulaMan;

	// 分析家格式公式数组
	CArray<FxjExpress*,FxjExpress*>* m_ayExpress;

	// 公式重名时的处理方法: 
	// EXTERN_EXPRESS_CHANGE_NAME	改名
	// EXTERN_EXPRESS_AUTO_NAME		自动命名
	// EXTERN_EXPRESS_OVR			覆盖已有公式
	WORD							 m_wExternalExpress;

public:
	// 如果是输出,则指向全局的公式表,如果是输入,则为本类的一个私有表的指针
	CMapVariabile* m_pExternExpression;
	// 输入或输出的公式文件名
	CString		   m_strFileName;
	// 传输方向: 输入 TRUE, 输出 FALSE
	BOOL		   m_bInport;

public:
	void AutoSetup();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTANDINPORTDLG_H__49D7F0DE_2849_45E4_A74F_569F4BB73C50__INCLUDED_)
