/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	DlgAlarmMan.h
*	文件标识：	"预警系统"功能主模块
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

#ifndef _HSWRANMANAGERDLG_HEAD_FILE_ 
#define _HSWRANMANAGERDLG_HEAD_FILE_

#include "stdafx.h"
#include "resource.h"
#include "afxcmn.h"
#include "AdvancedAlarmData.h"
#include "PubStruct.h"
#include "hsstructnew.h"

#define		HS_CONDITION_CON	"预警条件"
#define		HS_CONDITION_TYPE	"预警类型"
#define		HS_CONDITION_STK	"预警股票"
#define		HS_CONDITION_NUM	"预警次数"
#define		HS_CONDITION_ALARM	"预警状态"
#define		HS_ALARM_CODE		"预警代码"
#define		HS_ALARM_TIME		"预警时间(月-日 时:分)"
#define		HS_ALARM_PRICE		"预警价格"
#define		HS_ALARM_YK		"现价/盈亏"

#define		HS_BEGIN_ALARM_CODE		_T("启动预警")
#define		HS_END_ALARM_CODE			_T("关闭预警")

#define		HS_SHOW_CONDITION			_T("显示条件")
#define		HS_HIDE_CONDITION			_T("隐藏条件")

// CDlgAlarmMan dialog
// 定义消息响应
#define		Alarm_Save						0x0000
#define		Add_Stock_To_Cur_Alarm			0x0001
#define		Add_Stock_To_New_Alarm			0x0002
#define		Add_Stock_To_Alarm_CurSel		0x0003
#define		Check_Alarm_Name				0x0004
#define		Show_Alarm_Window				0x0005
#define		Begin_Alarm_Msg					0x0006
#define		Show_Alarm_Analyse				0x0007
#define		Flag_Alarm_Bebin				0x0008
#define		Flag_Alarm_End					0x0009

#define		Del_All_Alarm					0x0001
#define		Del_Cur_Alarm					0x0002

class CDlgAlarmMan : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarmMan)

public:
	CDlgAlarmMan(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAlarmMan();

// Dialog Data
	enum { IDD = IDD_ALARM_STOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClicked1014();
	afx_msg void OnBnClickedWranAnalyse();
	afx_msg void OnBnClickedChageCon();
	afx_msg void OnNMClickConList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedDelCon();
	afx_msg void OnBnClickedDelAlarm();
	afx_msg void OnBnClickedBeginAlarm();
	afx_msg void OnBnClickedHide();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnNMClickAlarmList(NMHDR *pNMHDR, LRESULT *pResult);

public:
			
	// 当前预警状态
	static BOOL m_bAlarmStateFlag;
	// 条件列表
	CListCtrl m_ListCondition;								
	// 当前选择条件列表索引
	int		  m_nCurSelCondition;							
	// 警报列表
	CListCtrl m_ListAlarm;									
	// 当前选择警报索引
	int		  m_nCurSelAlarm;								
	// 预警条件
	CArray<CAdvancedAlarmData*, CAdvancedAlarmData*> m_ayAlarmData;		
	// 预警保存文件名
	CString	m_strAlarmFileName;									
	// 预警信息保存文件名
	CString	m_strAlarmInfoFileName;								
	// 预警计算数据
	CArray<AlarmCalcData*, AlarmCalcData*> m_CalculateData;		
	// 报警显示数据
	CArray<AlarmDisplayData*, AlarmDisplayData*> m_DisplayData;	
	// 风格
	WORD	m_wStyle;	
	// 用于显示列表框中图标
	CImageList m_itemList;

public:

	// 初始化
	void	Init();		

	// 初始化列表头
	void	InitList();		

	// 重画条件列表
	void	ReDrawCondition();	

	// 重画警报列表, uMode == -1表示追加, uMode == 0表所重画所有
	void	ReDrawAlarm(UINT uMode = -1);	
	
	//开始预警
	void    StartAlarm();

	// 得到公式描述
	CString	GetDescribe(UnionSelCondition* pHSUnion);				
	
	// 读写公式数据 m_ayAlarmData
	BOOL	ReadAlarmData();
	BOOL	WriteAlarmData();

	// 读写预警信息 m_DisplayData
	// 调用ReadAlarmInfo函数后,一定要调用RemoveCalculateData() 及 CreateCalculareData()
	// WriteAlarmInfo函数根据m_wStyle最后一位来决定是否要定盘
	BOOL	ReadAlarmInfo();	
	BOOL	WriteAlarmInfo();

	// 打开或关闭预警---注:以下默认值为-1表示所有条件
	BOOL	Start(int nIndex = -1);
	BOOL	Stop(int nIndex = -1);

	// 重新设定某一条件预警,bFlag = TRUE 激活,否则就屏闭, 
	// 如果bOnlyOnOff为真,则只激活或屏闭,否则,同时改变
	BOOL	ReSetState(BOOL bFlag = TRUE, int nIndex = -1, BOOL bOnlyOnOff = FALSE);

	// 重新初始化计算数据
	void	ReInitCalData();
	
	// 删除与重新建立数据
	BOOL	RemoveCalculateData(int nIndex = -1);
	BOOL	CreateCalculareData(int nIndex = -1);
	
	// 重新设置条件数据与计算数据中周期与声音文件的一致
	BOOL	ReSetCalculateCyclyAndSoundFile(int nIndex = -1);

	// 设置界面bFlag == TRUE 则显示投机条件,否则隐藏
	void	ShowWindowStyle(BOOL bFlag = TRUE);
	
	// 处理来自付窗口的信息
	long	OnFramMsgPress(WPARAM wp,LPARAM lp);

	// 条件编辑
	void	AddCon(StockUserInfo* pData = NULL);
	void	DelCon();
	void	ChageCon(StockUserInfo* pData = NULL);

	// 预警数据删除
	void	DelAlarm(int nIndex = Del_Cur_Alarm );

	// 得到运行系统状态
	BOOL	GetState();

	// 设置按钮状态
	BOOL	SetButtonState(BOOL bFalg);

	// 设置所有显示数据为已查看
	int		SetDisplayDataOld(int nIndex = -1);

	// 加入股票
	BOOL AddStock(CArray<StockUserInfo*, StockUserInfo*>*pAyStock, StockUserInfo* pData);

	// 设置删除按钮状态
	void SetAlarmButton(BOOL bFlag);
	void SetConditionButton(BOOL bFlag);

	// 跟据条件名得到条件索引
	int  GetIdxByName(CString strName);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMRclickConList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAlarm1BeginAll();
	afx_msg void OnAlarm1EndAll();
	afx_msg void OnAlarm1BeginCur();
	afx_msg void OnAlarm1EndCur();
	afx_msg void OnAlarm1Exit();
	afx_msg void OnAlarm1ChgCon();
	afx_msg void OnAlarm1AddCon();
	afx_msg void OnAlarm1DelCon();
	afx_msg void OnAlarm1Begin();
	afx_msg void OnAlarm1End();
	afx_msg void OnNMDblclkConList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAlarm2Exit();
	afx_msg void OnAlarm2DelAll();
	afx_msg void OnAlarm2DelCur();
	afx_msg void OnNMRclickAlarmList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNMDblclkAlarmList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusConList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusConList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusAlarmList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusAlarmList(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif

