/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgExchParamSet.h
*	文件标识：	交易测试参数优选设置界面
*	摘	  要：	交易测试界面相关,以后再写
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include <afxwin.h>
#include "ExchData.h"
// CDlgExchParamSet dialog


class CDlgExchParamSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgExchParamSet)

public:
	CDlgExchParamSet(void* pData = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExchParamSet();

// Dialog Data
	enum { IDD = IDD_EXCH_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 参数一
	CButton m_wndCheckParam1;
	// 参数二
	CButton m_wndCheckParam2;
	// 参数三
	CButton m_wndCheckParam3;
	// 参数四
	CButton m_wndCheckParam4;

	BOOL m_bHaveChange;

protected:
	// 显示几行设计数据，最大四行
	void ShowLine(int nLineNum = 0);
	// 设置一行数据
	void SetLineData(int nLineIdx, CString strParamName, double dMin, double dMax, double dAdd);
	// 得到一行数据
	BOOL GetLineData(int nLineIdx, TExchOptimizeItem* pItem = NULL);
	// 使一行有效或无效
	void EnableLine(int nLineIdx, BOOL bValid = TRUE);
	// 得到最大参数数
	int GetMaxLine() { return 4; }
	// 显示统计计算次数
	void ShowTotal();

protected:
	// 优化数据
	TExchOptimize*	m_pData;
	TExchOptimize	m_localData;

public:
	void SetOptimizeData(TExchOptimize*	pData) 
	{ 
		if( pData == NULL )
		{
			return;
		}
		m_pData = pData; 
		m_localData = *pData;
	}
	TExchOptimize* GetOptimizeData() { return &m_localData; }

protected:
	// 显示数据
	BOOL PlayData();
	// 得到数据
	BOOL SaveData();

public:
	void OnChangeEdit(UINT uID);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel2();
};
