/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgExchOptimize.h
*	文件标识：	
*	摘	  要：	交易优选相关,目前还没实现
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once

#include "ExchCalcMain.h"
#include "resource.h"
#include "exchyouselcacldata.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "afxdtctl.h"
// CDlgExchOptimize dialog

class CDlgExchOptimize : public CDialog
{
	DECLARE_DYNAMIC(CDlgExchOptimize)

public:
	CDlgExchOptimize(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExchOptimize();

// Dialog Data
	enum { IDD = IDD_EXCHAGNE_YOUSHUANG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected:
	// 计算数据及方法
	CExchYouSelCaclData		m_sCaclData;
	inline CExchYouSelCaclData* GetYouSelCaclData() { return &m_sCaclData; }

	// 返回结果处理
	void FailDispose(LPARAM lp);
	// 打开计算结果的计算过程 
	BOOL OpenTableWnd(UINT uID = 0) {return m_sCaclData.OpenTableWnd(uID);}
	
	// 计算线程操作
	LRESULT Start() {return m_sCaclData.StartEx(); }
	LRESULT Stop() {return m_sCaclData.StopEx(); }

	void	SetExpression();
	// 设置默认测试策略
	void	SetDefTact() { m_sCaclData.SetDefTact(); }
	// 设置自定义测试策略
	void	SetCommTact() { m_sCaclData.SetCommTact(); }

	// 计算进度消息管理
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
public:
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedRadioComm();
	afx_msg void OnBnClickedCheckAllsel();
	afx_msg void OnBnClickedCheckAllloctldata();
	afx_msg void OnBnClickedOk();
	CListCtrl m_wndList;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_wndProgress;
	CComboBox m_wndTestObj;
	CComboBox m_wndTestCyc;
	CDateTimeCtrl m_wndBeginDate;
	CDateTimeCtrl m_wndEndDate;
};
