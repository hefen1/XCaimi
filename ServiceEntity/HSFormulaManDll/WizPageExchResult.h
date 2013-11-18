/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageExchResult.h
*	文件标识：	交易测试设置条件显示页
*	摘	  要：	交易测试界面相关,以后再写
*
*	当前版本：	2.0
*	作	  者：	俞礼石
*	更新日期：	2003年4月26日
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "WizPageExchBase.h"
#include "RichEditCtrlResult.h"

#define IDC_RESULT_ID	50000
// CWizPageExchResult dialog

class CWizPageExchResult : public CWizPageExchBase
{
//	DECLARE_DYNAMIC(CWizPageExchResult)

public:
	CWizPageExchResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchResult();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void OnSetActive();

public:
	CRichEditCtrlResult	m_wndResult;
	TExchOptimize		m_sLocalOptimize;

	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1);
	long Initial();
	long PlayData();
	afx_msg void OnBnClickedModefy();
};
