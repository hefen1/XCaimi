/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageExchBuy.h
*	文件标识：	交易测试买入条件及买入方法设置页
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
//#include "afxdtctl.h"
//#include <afxwin.h>
#include "TreeCtrlFormula.h"



#include "WizPageExchBase.h"

// CWizPageExchBuy dialog

class CWizPageExchBuy : public CWizPageExchBase
{
public:
	CWizPageExchBuy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchBuy();
	virtual BOOL OnKillActive(int nPos = -1);
	virtual void	OnSetActive();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_BUY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComcon1();
	virtual BOOL OnInitDialog();
	virtual BOOL SetData();
	void         InitCondition();

public:
	CDateTimeCtrl m_wndTimeBegin;
	CDateTimeCtrl m_wndTimeEnd;
	CButton m_wndButtonDecount;
	CComboBox m_wndConditionA;
	CComboBox m_wndConditionR;
	CComboBox m_wndConditionB1;
	CComboBox m_wndConditionB2;
	CStatic m_wndStaticRelation;
	CComboBox m_wndBuyMethod;
	CStatic m_wndStaticBuyMethod;
	CComboBox m_wndAddMethod;
	CStatic m_wndStaticAddMethod;
	CEdit m_wndEditBuyMount;
	CEdit m_wndEditAddMount;
	CButton m_wndCheckSkip;
	CEdit m_wndEditSkipPeriod;
	CStatic m_wndStaticMiddle;
	
public:
	TExchBuy		  m_TExchangeBuy;
	
	afx_msg void OnCbnSelchangeComuse();
	afx_msg void OnCbnSelchangeComfollow();

	// nIdx 连续买入方法
	void ShowLSWin(int nIdx);
	// nCmd 显示命令
	void ShowLastLsWin(int nCmd);
	// nIdx 买入规则
	void ShowBuyWin(int nIdx);
	// nCmd 显示命令
	void ShowLastBuyWin(int nCmd);
	// nIdx 买入方法
	void EnableLSWin(int nIdx);
	// nIdx买入方法
	void ShowBuyMedthWin(int nIdx);
	void ShowMedthWin(int nCmd);
	// nIdx 连续买入方法
	void EnableSkipWin(int nIdx);
	// 填充第一个条件列表
	void FullConOne(int nIdx);
	// 填充第二个条件列表
	void FullConTwo(BOOL bIsTech = TRUE);
	// 填充技术指标
	void FullTech();
	// 显示数据
	void PlayData();
	// 显示数据
	void SetSelectData(CString& str, CComboBox& wndCom);
	// 得到公式
	CExpression* GetExp();

	afx_msg void OnCbnSelchangeComcond();
	afx_msg void OnCbnSelchangeComcon2();
	afx_msg void OnCbnKillfocusComcon2();
	virtual BOOL  InitialData(TExchHead* pData);
};
