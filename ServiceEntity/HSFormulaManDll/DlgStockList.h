/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgStockList.h
*	文件标识：	
*	摘	  要：	股票列表窗口
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：	2003年4月29日
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "afxcmn.h"
#include "resource.h"

// CDlgStockList dialog

class CDlgStockList : public CDialog
{
	DECLARE_DYNAMIC(CDlgStockList)

public:
	CDlgStockList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgStockList();

// Dialog Data
	enum { IDD = IDD_STOCKLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	// 股票数据数组
	CArray<StockUserInfo*, StockUserInfo*> m_ayStock;
	// 固定的股票
	StockUserInfo* m_pFixeStock;
	// 股票列表
	CHSListCtrlBase m_wndList;

	// 加一个股票到股票数组
	BOOL AddStock(StockUserInfo* pStock);
	// 从股票数组删除一支股票
	BOOL DelStock(int nIdx);
	// 看股票pStock是否在股票数组及固定股票中存在
	BOOL IsHave(StockUserInfo* pStock);
	// 显示股票信息
	BOOL Display();
	// 初始化股票显示列表控件
	BOOL InitCtrl();
	// 键盘精灵消息处理
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);
	// 向键盘精灵发加股票消息
	BOOL SendAddStockMsg(MSG *pMsg);

public:
	/******************************************************************
	*	函数功能：	设置原始股票信息
	*	函数参数：	StockUserInfo* pFixeStock : [in] 固定股票项
	*				CArray<StockUserInfo*     
	*				StockUserInfo*> &ayStock  : [in] 股票数组
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void SetStock(StockUserInfo* pFixeStock, CArray<StockUserInfo*, StockUserInfo*> &ayStock);

	/******************************************************************
	*	函数功能：	将更改后的股票信息赋值给ayStock
	*	函数参数：	CArray<StockUserInfo*
	*				StockUserInfo*> &ayStock : [out] 得到更改后的股票信息
	*	返 回 值：	void 
	*	调用依赖：	
	*	作    者：	
	*	完成日期：	2003年4月29日
	*	修改说明：	
	*****************************************************************/
	void CopyStockTo(CArray<StockUserInfo*, StockUserInfo*> &ayStock);
	// "增加"按钮处理
	afx_msg void OnBnClickedAdd();
	// "删除"按钮处理
	afx_msg void OnBnClickedDel();
	// "确定"按钮处理
	afx_msg void OnBnClickedOk4();

	// 消息预发向处理,主要是截出KEY及CHAR消息,将其发送给键盘精灵
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// 对话框初始化
	virtual BOOL OnInitDialog();
};
