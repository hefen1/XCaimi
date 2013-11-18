/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAlarmParamSet.h
*	文件标识：	
*	摘	  要：	公式预警相关,以后再写
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include "AdvancedAlarmData.h"
#include "resource.h"

// CDlgAlarmParamSet dialog
#define HS_MAX_CYCLE	86400
#define HS_MIN_CYCLE	1
#define	HS_ALARM_STYLE_ALARMINFO_SAVE  0x0001	// 保存警报信息
#define HS_ALARM_STYLE_SOUNDFILE_SHARE 0x0002  // 声音文件共响


class CDlgAlarmParamSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmParamSet)

public:
	CDlgAlarmParamSet(void* pData = NULL, int nMode = -1, WORD wStyle = 0, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAlarmParamSet();

// Dialog Data
	enum { IDD = IDD_ALARM_PROPER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:

	// 单条件指针
	CAdvancedAlarmData* m_pSingulAlarmData;

	// 条件数组指针
	CArray<CAdvancedAlarmData*, CAdvancedAlarmData*>* m_pAyAlarmData;

	// 当前所选择条件的位置,如果为-1则为单条件,否则为条件数组
	int	m_nCurSelAlarmData;

	// 周期
	int	m_nCycle;

	// 声音文件名
	CString m_strSoundFileName;

	// 是否显示<设置所按钮为可用>
	BOOL m_bCheck;

	// 是否选择了所有
	BOOL m_bCheckAllSet;

	// 是否保存预警数据
	BOOL m_wndSaveCheck;
public:
	void Init();
	afx_msg void OnBnClickedCheck1();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck2();

};
