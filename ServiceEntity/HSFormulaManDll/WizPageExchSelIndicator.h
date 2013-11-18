/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	WizPageExchSelIndicator.h
*	文件标识：	交易测试公式及公式参数设置页
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
#include "afxcmn.h"
//#include "ExpTabCtrl.h"
//#include "TreeCtrlFormula.h"
#include <afxwin.h>
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "WizPageExchBase.h"
#include "EditEidolon.h"
// CWizPageExchSelIndicator dialog

class CWizPageExchSelIndicator : public CWizPageExchBase
{
//	DECLARE_DYNAMIC(CWizPageExchSelIndicator)

public:
	CWizPageExchSelIndicator(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchSelIndicator();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	long    LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1);
	virtual void OnSetActive();
	virtual LRESULT OnWizardNext();
protected:
	CEditEidolon m_EditEidolon;

public:
	CComboBox    m_wndPeriod;
	CExpTabCtrl	 m_wndTab;

public:
	TExchExp	m_sExp;
	afx_msg void OnBnClickedButton2();
	virtual BOOL  InitialData(TExchHead* pData);
	// 是否计算除权
	CButton m_ctlExRight;
};
