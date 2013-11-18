/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAlarmAnalysis.h
*	�ļ���ʶ��	Ԥ����������
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
#include "PubStruct.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "afxdtctl.h"
#include "resource.h"
#include <WinDef.h>

#define HS_ALARM_ALALYSE_STK	_T("��Ʊ����")
#define HS_ALARM_ALALYSE_NAME	_T("Ԥ������")
#define HS_ALARM_ALALYSE_TIME	_T("Ԥ��ʱ��(��-�� ʱ:��)")
#define HS_ALARM_ALALYSE_DATA2	_T("����ƽ��")
#define HS_ALARM_ALALYSE_DATA3	_T("ʮ��ƽ��")
#define HS_ALARM_ALALYSE_DATA4	_T("��ʮ��ƽ��")
#define HS_ALARM_ALALYSE_DESC	_T("Ԥ������")

#define HS_ALL_CONDITION		_T("��������")
#define HS_ALL_STOCK			_T("���й�Ʊ")
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

	// Ԥ������
	CArray<AlarmDisplayData*, AlarmDisplayData*>*	m_pDisplayData;
	// Ԥ�������б��
	CListCtrl m_wndConditionList;
	// Ԥ�������б��
	CComboBox m_wndAlarmCondition;
	// ��Ʊ�б��
	CComboBox m_wndListStk;
	// ��ǰ����
	CString m_strCondition;
	// ��ǰ��Ʊ
	CString m_stkCode;
	// ��ʾ��������������
	CArray<int, int> m_ayDiaplayIndex;
	// �ı�֮ǰ��ʱ��
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
	// ��ʼʱ��
	CDateTimeCtrl m_wndBeginTime;
	// ����ʱ��
	CDateTimeCtrl m_wndEndTime;
	afx_msg void OnDtnCloseupEndTime(NMHDR *pNMHDR, LRESULT *pResult);
};
#pragma once
