/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAddStockToAlarm.h
*	�ļ���ʶ��	�ӹ�Ʊ��Ԥ������
*	ժ	  Ҫ��	��ʽԤ�����,�Ժ���д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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

	// Ԥ������
	CDlgAlarmMan* m_pManagerDlg;
	CodeInfo			m_StockInfo;
	CArray<int, int>	m_ayDisplyIndex;
	CArray<StockUserInfo*,StockUserInfo*> m_ayStock;
	CListCtrl m_ListCondition;
	// ������ʾ�б����ͼ��
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
