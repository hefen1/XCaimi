/*************************************************************
*	Copyright (c)2002, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	SheetExchMain.h
*	文件标识：	交易测试的向导页停靠窗口及计算数据的管理窗口
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
#include "ExchCalcMain.h"
#include "DlgSingleExchCalcResult.h"
#include "TabCtrlExch.h"
#include "WizPageUnionExchCaclResult.h"
#include "HSNewWizDialog.h"

// CSheetExchMain dialog
#define PROGRESS_ID		40000
#define TAB_ID			(PROGRESS_ID+1)

class CSheetExchMain : public CHSNewWizDialog
{
	
public:
	CSheetExchMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSheetExchMain();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_UNION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
	
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

public:
	int Select();
	int	Start();
	int Stop();
	BOOL IsRunCalc();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWizback();
	afx_msg void OnBnClickedWiznext();
	afx_msg void OnBnClickedWizfinish();
	afx_msg void OnBnClickedExit();

public:
	CExchCalcMain	m_pMainTech;
	CExchCalcMain*  m_pRefMainTech;
	CExchCalcMain* GetData() 
	{ 
		if( m_pRefMainTech ) 
			return m_pRefMainTech;
		return &m_pMainTech; 
	}

	// 保存数据
	int Apply();
	// 进度消息处理
	static LRESULT OnMsgPress(DWORD dwMsg, WPARAM wp, LPARAM lp);

	// 进度条
	CProgressCtrl	m_wndPress;
	CTabCtrlExch		m_wndPageTab;
	// 按钮状态
	void SetStartBom(BOOL bStop);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetCurTab(int nAdd);
	void SetCurTab();

public:
	CDlgSingleExchCalcResult*	m_dlgTable;

	BOOL OpenTableWnd(UINT uID = 0);

	void ShowExch();
	void FailDispose(LPARAM lp);

	// 删除第六页
	BOOL DelCaclPage(UINT nIDTemplate = IDD_EXCHANGE_CACL_RESULT);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
