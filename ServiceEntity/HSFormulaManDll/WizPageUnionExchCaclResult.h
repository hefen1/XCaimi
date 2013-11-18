/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageUnionExchCaclResult.h
*	文件标识：	参数优化时,总的统计结果显示
*	摘	  要：	交易测试参数优化界面相关,以后再写
*
*	当前版本：	1.0
*	作	  者：
*	更新日期：	
*
*	修改说明：	
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
	// 计算列表
	CListCtrl m_wndList;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReCacl();
};
