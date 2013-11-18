/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAddStockToAlarm.h
*	文件标识：	加股票到预警界面
*	摘	  要：	公式预警相关,以后再写
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once

#include "AdvancedAlarmData.h"
#include "DlgAlarmMan.h"
#include "afxcmn.h"
// CDlgAddStockToAlarm dialog

class CDlgAddStockToAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddStockToAlarm)

public:
	CDlgAddStockToAlarm(void *pData = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAddStockToAlarm();

// Dialog Data
	enum { IDD = IDD_ADD_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:

	// 预警条件
	CDlgAlarmMan* m_pManagerDlg;
	CodeInfo			m_StockInfo;
	CArray<int, int>	m_ayDisplyIndex;
	CArray<StockUserInfo*,StockUserInfo*> m_ayStock;
	CListCtrl m_ListCondition;
	// 用于显示列表框中图标
	CImageList m_itemList;
	void ReDrawCondition();
	void InitList();
	int  GetCurStockArray();
	long OnMsgPress(WPARAM wp,LPARAM lp);
	int AddStock();
	void SetAddButton(BOOL nFlag);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk3();
	virtual int  DoModal();
	afx_msg void OnBnClickedView();
};
