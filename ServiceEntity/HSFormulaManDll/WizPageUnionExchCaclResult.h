/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageUnionExchCaclResult.h
*	�ļ���ʶ��	�����Ż�ʱ,�ܵ�ͳ�ƽ����ʾ
*	ժ	  Ҫ��	���ײ��Բ����Ż��������,�Ժ���д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "WizPageExchBase.h"
#include "resource.h"


struct _tagExchSortParam
{
	CExchCalcMain* m_pData;
	int m_colID;
};

class CWizPageUnionExchCaclResult :
	public CWizPageExchBase
{
public:
	CWizPageUnionExchCaclResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageUnionExchCaclResult();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_CACL_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()	
	void OnSetActive();
	virtual BOOL OnKillActive(int nPos = -1);

public:

	virtual BOOL OnInitDialog();
	BOOL Initial(CExchCalcMain* pData = NULL);
	BOOL PlayData(CExchCalcMain* pData = NULL);
	BOOL ClearCell();

	BOOL OpenDetailed();
	// �����б�
	CListCtrl m_wndList;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReCacl();
};
