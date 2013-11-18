/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAlarmAnalysis.h
*	文件标识：	预警分析界面
*	摘	  要：	
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "PubStruct.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "afxdtctl.h"
#include "resource.h"
#include <WinDef.h>

#define HS_ALARM_ALALYSE_STK	_T("股票代码")
#define HS_ALARM_ALALYSE_NAME	_T("预警名称")
#define HS_ALARM_ALALYSE_TIME	_T("预警时间(月-日 时:分)")
#define HS_ALARM_ALALYSE_DATA2	_T("五日平均")
#define HS_ALARM_ALALYSE_DATA3	_T("十日平均")
#define HS_ALARM_ALALYSE_DATA4	_T("二十日平均")
#define HS_ALARM_ALALYSE_DESC	_T("预警描述")

#define HS_ALL_CONDITION		_T("所有条件")
#define HS_ALL_STOCK			_T("所有股票")
// CDlgAlarmAnalysis dialog

class CDlgAlarmAnalysis : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmAnalysis)

public:
	CDlgAlarmAnalysis(void *pData = NULL, CWnd* pParent = NULL, LPCSTR szAlarmName = NULL);   // standard constructor
	virtual ~CDlgAlarmAnalysis();

// Dialog Data
	enum { IDD = IDD_ALARM_ANASIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();

public:

	// 预警数据
	CArray<AlarmDisplayData*, AlarmDisplayData*>*	m_pDisplayData;
	// 预警数据列表框
	CListCtrl m_wndConditionList;
	// 预警条件列表盒
	CComboBox m_wndAlarmCondition;
	// 股票列表盒
	CComboBox m_wndListStk;
	// 当前条件
	CString m_strCondition;
	// 当前股票
	CString m_stkCode;
	// 显示出的数据项索引
	CArray<int, int> m_ayDiaplayIndex;
	// 改变之前的时间
	int m_nOldBeginTime, m_nOldEndTime;

public:
	void InitList();

	void ReDrawListData(CString strName = HS_ALL_CONDITION, 
		CString strCode = HS_ALL_STOCK, 
		int nBeginTime = -1, 
		int nEndTime = -1);

	void ReDrawListBox();
	virtual BOOL OnInitDialog();
	BOOL GetBoxText(CComboBox* pt, CString& strTemp);
	void GetSetTime(int* nBegin, int* nEnd);
	void SetTime(int nBegin, int nEnd);

	afx_msg void OnCbnSelchangeConList();
	afx_msg void OnCbnSelchangeStkList();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedAnalyse();
	afx_msg void OnDtnCloseupBeginTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOutput();
	// 开始时间
	CDateTimeCtrl m_wndBeginTime;
	// 结束时间
	CDateTimeCtrl m_wndEndTime;
	afx_msg void OnDtnCloseupEndTime(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once
