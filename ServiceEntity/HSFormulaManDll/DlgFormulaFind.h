/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgFormulaFind.h
*	文件标识：	公式查找窗口类
*	摘	  要：	实现在公式查找的功能
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include <afxcmn.h>
#include <afxwin.h>

#include "HSBaseDialog.h"

class CDlgFormulaFind : public /*CDialog*/ CHSBaseDialog
{
	DECLARE_DYNAMIC(CDlgFormulaFind)

public:
	CDlgFormulaFind(DWORD dwMask = 0xFFFF,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFormulaFind();

	// Dialog Data
	//{{AFX_DATA(CDlgFormulaMan)
	enum { IDD = IDD_COMPFIND };
	// 动画
	CAnimateCtrl m_wndAnimate;
	// 要查找字串下拉框
	CComboBox m_wndKeyCombo;
//	CComboBox m_wndRangeCombo;
	// 查找结果列表
	CListCtrl m_wndResultList;
	
	// 查找范围
	BOOL m_bCompDiscribe; // 描述
	BOOL m_bCompEdolon;	  // 参数精灵	
	BOOL m_bCompName;	  // 名字
	BOOL m_bCompSource;	  // 定义字串(公式)
	BOOL m_bCompNotes;    // 注释
	BOOL m_bCompAll;	// 上述所有
	//}}AFX_DATA

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// "停止"铵钮处理
	afx_msg void OnBnClickedStopbut();
	// "开始"按钮处理
	afx_msg void OnBnClickedStartbutton();
	// 选择改变事件处理,这里没做什么事
	afx_msg void OnCbnSelchangeKeycombo();
	// 不会进行.不知道写这个函数有什么用 ??? 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 双击找到的公式列表,则在Ｋ线处显示消息
	afx_msg void OnLvnItemActivateResultlist(NMHDR *pNMHDR, LRESULT *pResult);	

public:
	DWORD m_dwMask;		// 查找范围(公式类型)
	CWnd* m_pParentWnd; // 父窗口,一般是主窗口
	short m_nFindMask;  // 查找范围(公式名,注释,描述,定义,参数说明等)
	BOOL m_bThreadRun;  // 线程运行状态(TRUE 运行中,FALSE 停止)

public:

	/******************************************************************
	*	函数功能：	查找线程处理函数
	*	函数参数：	LPVOID pParameter : [in] CDlgFormulaFind对象指针
	*	返 回 值：	DWORD : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月8日
	*	修改说明：	
	*****************************************************************/
	static DWORD WINAPI  FindComp(LPVOID pParameter);
	static BOOL bEscMsgOfDlgFind;
public:
	// 消息发送预处理
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	// 初始化,公式列表列名初始化
	virtual BOOL OnInitDialog();
	// OK处理
	virtual void OnOK();
	// "退出"处理
	virtual void OnCancel();
	// delete 对象自己
	virtual void PostNcDestroy();
	

public:
	// 从各BOOL值组织出m_nFindMask
	short GetFindMask(void);
	BOOL ShowWindow( int nCmdShow );	
	afx_msg void OnBnClickedCompall();
	afx_msg void OnBnClickedCompname();
	afx_msg void OnBnClickedCompdiscribe();
	afx_msg void OnBnClickedCompnotes();
	afx_msg void OnBnClickedCompedolon();
	afx_msg void OnBnClickedCompsource();
};
