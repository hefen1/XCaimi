/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAlarmMan.h
*	�ļ���ʶ��	"Ԥ��ϵͳ"������ģ��
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
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

#define		HS_CONDITION_CON	"Ԥ������"
#define		HS_CONDITION_TYPE	"Ԥ������"
#define		HS_CONDITION_STK	"Ԥ����Ʊ"
#define		HS_CONDITION_NUM	"Ԥ������"
#define		HS_CONDITION_ALARM	"Ԥ��״̬"
#define		HS_ALARM_CODE		"Ԥ������"
#define		HS_ALARM_TIME		"Ԥ��ʱ��(��-�� ʱ:��)"
#define		HS_ALARM_PRICE		"Ԥ���۸�"
#define		HS_ALARM_YK		"�ּ�/ӯ��"

#define		HS_BEGIN_ALARM_CODE		_T("����Ԥ��")
#define		HS_END_ALARM_CODE			_T("�ر�Ԥ��")

#define		HS_SHOW_CONDITION			_T("��ʾ����")
#define		HS_HIDE_CONDITION			_T("��������")

// CDlgAlarmMan dialog
// ������Ϣ��Ӧ
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
			
	// ��ǰԤ��״̬
	static BOOL m_bAlarmStateFlag;
	// �����б�
	CListCtrl m_ListCondition;								
	// ��ǰѡ�������б�����
	int		  m_nCurSelCondition;							
	// �����б�
	CListCtrl m_ListAlarm;									
	// ��ǰѡ�񾯱�����
	int		  m_nCurSelAlarm;								
	// Ԥ������
	CArray<CAdvancedAlarmData*, CAdvancedAlarmData*> m_ayAlarmData;		
	// Ԥ�������ļ���
	CString	m_strAlarmFileName;									
	// Ԥ����Ϣ�����ļ���
	CString	m_strAlarmInfoFileName;								
	// Ԥ����������
	CArray<AlarmCalcData*, AlarmCalcData*> m_CalculateData;		
	// ������ʾ����
	CArray<AlarmDisplayData*, AlarmDisplayData*> m_DisplayData;	
	// ���
	WORD	m_wStyle;	
	// ������ʾ�б����ͼ��
	CImageList m_itemList;

public:

	// ��ʼ��
	void	Init();		

	// ��ʼ���б�ͷ
	void	InitList();		

	// �ػ������б�
	void	ReDrawCondition();	

	// �ػ������б�, uMode == -1��ʾ׷��, uMode == 0�����ػ�����
	void	ReDrawAlarm(UINT uMode = -1);	
	
	//��ʼԤ��
	void    StartAlarm();

	// �õ���ʽ����
	CString	GetDescribe(UnionSelCondition* pHSUnion);				
	
	// ��д��ʽ���� m_ayAlarmData
	BOOL	ReadAlarmData();
	BOOL	WriteAlarmData();

	// ��дԤ����Ϣ m_DisplayData
	// ����ReadAlarmInfo������,һ��Ҫ����RemoveCalculateData() �� CreateCalculareData()
	// WriteAlarmInfo��������m_wStyle���һλ�������Ƿ�Ҫ����
	BOOL	ReadAlarmInfo();	
	BOOL	WriteAlarmInfo();

	// �򿪻�ر�Ԥ��---ע:����Ĭ��ֵΪ-1��ʾ��������
	BOOL	Start(int nIndex = -1);
	BOOL	Stop(int nIndex = -1);

	// �����趨ĳһ����Ԥ��,bFlag = TRUE ����,���������, 
	// ���bOnlyOnOffΪ��,��ֻ���������,����,ͬʱ�ı�
	BOOL	ReSetState(BOOL bFlag = TRUE, int nIndex = -1, BOOL bOnlyOnOff = FALSE);

	// ���³�ʼ����������
	void	ReInitCalData();
	
	// ɾ�������½�������
	BOOL	RemoveCalculateData(int nIndex = -1);
	BOOL	CreateCalculareData(int nIndex = -1);
	
	// ��������������������������������������ļ���һ��
	BOOL	ReSetCalculateCyclyAndSoundFile(int nIndex = -1);

	// ���ý���bFlag == TRUE ����ʾͶ������,��������
	void	ShowWindowStyle(BOOL bFlag = TRUE);
	
	// �������Ը����ڵ���Ϣ
	long	OnFramMsgPress(WPARAM wp,LPARAM lp);

	// �����༭
	void	AddCon(StockUserInfo* pData = NULL);
	void	DelCon();
	void	ChageCon(StockUserInfo* pData = NULL);

	// Ԥ������ɾ��
	void	DelAlarm(int nIndex = Del_Cur_Alarm );

	// �õ�����ϵͳ״̬
	BOOL	GetState();

	// ���ð�ť״̬
	BOOL	SetButtonState(BOOL bFalg);

	// ����������ʾ����Ϊ�Ѳ鿴
	int		SetDisplayDataOld(int nIndex = -1);

	// �����Ʊ
	BOOL AddStock(CArray<StockUserInfo*, StockUserInfo*>*pAyStock, StockUserInfo* pData);

	// ����ɾ����ť״̬
	void SetAlarmButton(BOOL bFlag);
	void SetConditionButton(BOOL bFlag);

	// �����������õ���������
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

