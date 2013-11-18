/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageExchSell.h
*	文件标识：	交易测试卖出条件设置页
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
#include <afxwin.h>
#include "WizPageExchBase.h"

#define FORMULAMAN_EXCH_PROMPT				_T("评测系统提示")
// CWizPageExchSell dialog

class CWizPageExchSell : public CWizPageExchBase
{
	DECLARE_DYNAMIC(CWizPageExchSell)
public:
	CWizPageExchSell(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchSell();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_SELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1 );

public:
	CEdit m_wndEditSellPeriod;
	CEdit m_wndEditWin;
	CEdit m_wndEditLose;
	CEdit m_wndEditFallDay;
	CEdit m_wndEditFallRation;
	CEdit m_wndEditUpDay;
	CEdit m_wndEditUpRation;
	CButton m_wndChkBuy;
	CButton m_wndChkBuy1;
	CButton m_wndChkLose;
	CButton m_wndChkDown;
	CButton m_wndChkHp;

public:
	TExchSell	m_sExchSell;
	
public:
	virtual BOOL  SetData();
	BOOL InValidTest();
	void PlayData();
public:
	void OnSetActive();
	// 初始化设置数据
	virtual BOOL  InitialData(TExchHead* pData);
};
