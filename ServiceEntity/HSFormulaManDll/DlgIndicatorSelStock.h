/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgIndicatorSelStock.h
*	文件标识：	指标选股类定义
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#if !defined(AFX_TECHSELECT_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)
#define AFX_TECHSELECT_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HSCondition.h : header file --added by HS
//
#include "resource.h"
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "DlgUnionList.h"
#include "ExpPublic.h"
// CDlgIndicatorSelStock dialog

class CDlgIndicatorSelStock : public CDialog
{	
	DECLARE_DYNAMIC(CDlgIndicatorSelStock)

public:
	CDlgIndicatorSelStock(CWnd* pParent = NULL,HSOpenExpressUseData* pData = NULL,
		long* pThis = NULL);   // standard constructor
	virtual ~CDlgIndicatorSelStock();

// Dialog Data
	enum { IDD = IDD_TECHDIALOG };
	CStatic	m_wndUnionDlgPos;
	CButton	m_wndExpNotes;
	CStatic	m_wndStaticCon;
	CStatic	m_wndMiddle;
	CComboBox	m_wndData2;
	CComboBox	m_wndData1;
	CStatic		m_wndConNotes;
	CComboBox	m_wndCon;
	CComboBox	m_wndCompare;
	CStatic		m_wndAndOr;
	CExpTabCtrl	m_wndTab;
	CStatic		m_wndSelectStock;
	CStatic		m_wndTotalStock;
	CStatic		m_wndIncludeStock;

public:
//	CProgressCtrl	m_wndProgress;

public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CHSCondition)
	// "范围"按钮处理
	afx_msg void OnAdvance();
	// 取消处理
	virtual void OnCancel();
	// "注释"按钮处理
	afx_msg void OnUsernotes();
	// "执行"铵钮处理
	virtual void OnOK();
	// "选股到板块"按钮处理
	afx_msg void OnToblock();
	// 比较方式改变消息处理
	afx_msg void OnSelchangeCompare();
	// 比较指标改变消息处理
	afx_msg void OnSelchangeCon();
	// "修改公式"按钮处理
	afx_msg void OnButexpress();
	// "修改参数"按钮处理
	afx_msg void OnButparam();
	// "关闭"按钮处理
	afx_msg void OnBnClickedClose();
	// 公式列表选择改变消息处理
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	// 查找按钮处理
	afx_msg void OnBnClickedButfind();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
	//long    LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	//virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	/******************************************************************
	*	函数功能：	公式树操作时,所激发消息的响应函数
	*	函数参数：	WPARAM wParam = 0 : [in] TEMPUNION_NEWCONDITION -- 新增条件处理 lParam为CDlgUnionList*类型数据, 返回0
												TEMPUNION_SAVEUNION -- 保存组合处理 lParam为pTempUnion*类型数据, 返回0
												 TEMPUNION_DELUNION -- 删除组合处理 lParam为UnionSelCondition*类型, 返回0
												   HX_UNION_EXPRESS -- 组合公式输入 没有正确的起做用, lParam为UnionSelCondition*类型, 返回0 
											CTreeCtrlFormula::HSBrowser -- 组合公式输入,同时隐藏修改指标,修改公式,注释说明按钮, lParam为CTreeGroup*类型数据, 返回0
									CCompileEditView::UpdateExpress -- 公式被更新消息处理, 同时向自己发送HX_EXPRESSSENTENCE型消息, lParam无用, 返回0
												 HX_EXPRESSSENTENCE -- 选中公式处理,lParam为CExpression*数据, 返回0
	*				LPARAM lParam = 0 : [in] 与wParam相关
	*	返 回 值：	long : 恒为0
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月9日
	*	修改说明：	
	*****************************************************************/
	long    LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	/******************************************************************
	*	函数功能：	HX_USER_DRAGDATA用户消息处理
	*	函数参数：	WPARAM wParam : [in] 必须为HX_USER_DRAGDATA-1才做处理
	*				LPARAM lParam : [in] 为CExpression*数据,当前选 中的公式 
	*	返 回 值：	LRESULT : 做了处理(1), 不做处理(0)
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月9日
	*	修改说明：	
	*****************************************************************/
	virtual LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	/******************************************************************
	*	函数功能：	HX_USER_COMPILEDATA用户消息响应
	*	函数参数：	WPARAM wp : [in] 2	--	改变板块名设置,同时停止或开始选股(取原状态的返状态), 
											lp 为CString*型数据,指新设置的板块名
											返回0
								     3	--	返回当前选中股票
											lp 无意义
									 4	--	选股结束,返回0
									 5	--	设置当前股票数
											lp 为CNI_ReceiveData*型数据,pInfo2->m_nStatus为选中数, pInfo2->m_lDataTotal为总股票数
											返回0
									 6	--	设置所有被查找的股票数
											lp 为int型数据, 指所有被查找的股票数
											返回0
	*				LPARAM lp : [in] 与WP相关,请看WP说明
	*	返 回 值：	LRESULT : 与WP相关,请看WP说明
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月9日
	*	修改说明：	
	*****************************************************************/
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);

protected:
	HSOpenExpressUseData* m_pData; // 这里没用( ??? )
	bool m_bSelStockState; // 选股状态

public:

	CDlgUnionList			  m_wndTempUnionDlg; // 组合条件列表控件
	CArray<StockArea*,StockArea*> m_ayCheck; // 股票范围
	CString				  m_strBlock;              // 板块名

	SingleSelCondition		  m_sSingleSel;			   // 单个
	UnionSelCondition			  m_pHSUnion;			   // 组合
	SelStockCondition  m_curSelStockCondition;  // 条件选股结构
	// 是否使用除权后数据计算
	BOOL m_bCheckChuQuan;

protected:
	// 开始或是停止选股
	int  Select();
	// 初始化公式树
	void Init(int nCopy = CTreeCtrlFormula::CopyDelete);
	int  DoSelectStock( SelStockCondition*, int style);

public:
	// 初始化对话框
	virtual BOOL OnInitDialog();
	// 没用
	virtual void MyNcDestroy();
	/******************************************************************
	*	函数功能：	基本面时使用,从基本面数据ID得到列名(列定义表中)
	*	函数参数：	CString& strFindKeyName : [in] 应是一个数字字串,对应基本面数据的ID号
	*	返 回 值：	CString : 得到相应ID数据的名字
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月9日
	*	修改说明：	
	*****************************************************************/
	static CString  GetNameByKey(CString& strFindKeyName);
	// 初始化比较方式数据
	static void		InitCondition(CComboBox* pBox,int nType);

public:
	// 保存公式并删除所有私有空间
	void Delete();
	BOOL ShowWindow( int nCmdShow );
	//void SetSize();

public:
	/******************************************************************
	*	函数功能：	装入或是写出组合条件公式数据
	*	函数参数：	UINT nMode : [in] 装入还是写出
	*				CString strFileName = "" : [in] 文件名
	*				DWORD dStyle = 0 : [in] 风格 
	*	返 回 值：	BOOL 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年5月9日
	*	修改说明：	
	*****************************************************************/
	BOOL SaveLoad(UINT nMode,CString strFileName = "",DWORD dStyle = 0);	
	// 存所有组合公式
	virtual void Serialize(CArchive& ar,int& nCountRef);
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHSELECT_H__748885AF_D8EB_4424_A231_DA0BC35204A0__INCLUDED_)

