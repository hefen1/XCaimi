/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFunctionMan.h
*	文件标识：	函数管理窗口类
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_)
#define AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_

#include "ExpPublic.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFunctionMan.h : header file
//
//#include "ETSLayout.h"
#include "BaseListCtrl.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctionMan dialog

#define USERDEF_UPDATEDATA (WM_USER+101)

/*************************************************************
*	类 名 称：	CDlgFunctionMan
*	类 描 述：	函数管理窗口类
***************************************************************/
class CDlgFunctionMan : public CDialog
{
// Construction
public:

	/******************************************************************
	*	函数功能：	构造,初始化并调用CWnd类的Create(创建无模式对话框)
	*	函数参数：	CWnd* pParent = NULL : [in] 父窗口
	*				CMapVariabile*  pExternFunction = NULL : [in] 外部函数表
	*				CMapVariabile*	pExternExpression = NULL : [in]	外部公式表
	*				long*  pThis = NULL : [in] 对象指针的指针
	*	返 回 值：	无
	*	调用依赖：	CWnd::Create()
	*	作    者：	
	*	完成日期：	2003年5月6日
	*	修改说明：	
	*****************************************************************/
	CDlgFunctionMan(CWnd* pParent = NULL,CMapVariabile*  pExternFunction = NULL,
		CMapVariabile*	pExternExpression = NULL,
		long*  pThis = NULL );   // standard constructor

	/* DialogMgr: Add this: */
//	DECLARE_LAYOUT();
	/************************/

// Dialog Data
	//{{AFX_DATA(CDlgFunctionMan)
	enum { IDD = IDD_FUNCTIONMAN };
	// 注释文本框 
	CEdit			m_wndNotes;
	// 分类列表控件
	CBaseListCtrl   m_wndTypeList;//m_wndTabCtrl;
	// 当前分类所有公式表表控件
	CBaseListCtrl	m_wndListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFunctionMan)
public:
	// 消息传送预处理
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// 对话框数据交换
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CDlgFunctionMan)
	virtual void OnOK();		// "确定"按钮处理 ??? 
	virtual void OnCancel();	// "关闭"按钮处理
	afx_msg void OnNew();		// "新键"按钮处理 ???
	afx_msg void OnUpdate();	// ""
	afx_msg void OnInsert();	// "插入"按钮处理
	virtual BOOL OnInitDialog();// 对话框初始化处理
	// 函数分类列表选择改变消息处理
	afx_msg void OnItemchangedType(NMHDR* pNMHDR, LRESULT* pResult);
	// 函数列表选择改变消息处理
	afx_msg void OnItemchangedFunlist(NMHDR* pNMHDR, LRESULT* pResult);
	// 改变大小消息处理
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG


	/******************************************************************
	*	函数功能：	USERDEF_UPDATEDATA用户消息处理
	*	函数参数：	WPARAM wParam = 0 : [in] 无意义
	*				LPARAM lParam = 0 : [in] 内存指针,根据其大小可判断为CFunExpresion*或是CExpValue*型
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	long OnUserDefUpdateData(WPARAM wParam = 0, LPARAM lParam = 0);

	DECLARE_MESSAGE_MAP()
protected:

	/******************************************************************
	*	函数功能：	将指定类型的函数在函数列表控件中显示出来
	*	函数参数：	short nType : [in] 函数类型(0-11)
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void AddFunByType(short nType);

	/******************************************************************
	*	函数功能：	将指定类型的公式在公式列表控件中显示出来(目前好象不用了 ??? )
	*	函数参数：	short nType : [in] 公式类型
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月7日
	*	修改说明：	
	*****************************************************************/
	void AddExpressByType(short nType);

protected:
	class CDlgFormulaEdit* m_pParent;			// 调用本模块的公式编辑器窗口指针
	CMapVariabile*		  m_pExternFunction;	// 外部函数表
	CMapVariabile*		  m_pExternExpression;  // 外部表达式表

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONMAN_H__99136E56_137F_11D5_A10C_000000000000__INCLUDED_)
