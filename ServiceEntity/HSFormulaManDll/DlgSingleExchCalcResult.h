/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgSingleExchCalcResult.h
*	�ļ���ʶ��	��������ʾ
*	ժ	  Ҫ��	���ײ��Խ������,�Ժ���д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "RichEditCtrlResult.h"
#include "ExchCalcMain.h"
#include "afxcmn.h"
#include <afxwin.h>
#define IDC_TABLE_ID	50001
#define IDC_LIST_FLOW_ID	(IDC_TABLE_ID+1)
#define IDC_BMP_ID			(IDC_TABLE_ID+2)
#define IDC_RICH_ID			(IDC_TABLE_ID+3)

// CDlgSingleExchCalcResult dialog

class CDlgSingleExchCalcResult : public CDialog
{
	DECLARE_DYNAMIC(CDlgSingleExchCalcResult)

public:
	CDlgSingleExchCalcResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSingleExchCalcResult();

// Dialog Data
	enum { IDD = IDD_EXCH_CACLTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CRichEditCtrlResult	m_wndResult;
	CExchCalcMain		*m_pCaclData;
	CWnd				*m_pwndBMP;

	UINT				m_uParamID;

	void ShowData(UINT uID = 0);
	long PlayStock(UINT uID = -1);
	long Initial(CExchCalcMain* pData = NULL, UINT uID = 0);
	long SetSize();
	long PlayData(UINT uID = -1);
	void Open();
	virtual BOOL OnInitDialog();
	// �����л�
	CTabCtrl m_wndTab;
	CListCtrl m_wndFlowDeal;
	CListCtrl m_wndList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHdnItemclickStocklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickStocklist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSaveParam();
	afx_msg void OnBnClickedSave();
protected:
	virtual void OnCancel();
};
