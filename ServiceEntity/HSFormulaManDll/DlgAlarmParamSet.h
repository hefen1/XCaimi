/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgAlarmParamSet.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	��ʽԤ�����,�Ժ���д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "AdvancedAlarmData.h"
#include "resource.h"

// CDlgAlarmParamSet dialog
#define HS_MAX_CYCLE	86400
#define HS_MIN_CYCLE	1
#define	HS_ALARM_STYLE_ALARMINFO_SAVE  0x0001	// ���澯����Ϣ
#define HS_ALARM_STYLE_SOUNDFILE_SHARE 0x0002  // �����ļ�����


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

	// ������ָ��
	CAdvancedAlarmData* m_pSingulAlarmData;

	// ��������ָ��
	CArray<CAdvancedAlarmData*, CAdvancedAlarmData*>* m_pAyAlarmData;

	// ��ǰ��ѡ��������λ��,���Ϊ-1��Ϊ������,����Ϊ��������
	int	m_nCurSelAlarmData;

	// ����
	int	m_nCycle;

	// �����ļ���
	CString m_strSoundFileName;

	// �Ƿ���ʾ<��������ťΪ����>
	BOOL m_bCheck;

	// �Ƿ�ѡ��������
	BOOL m_bCheckAllSet;

	// �Ƿ񱣴�Ԥ������
	BOOL m_wndSaveCheck;
public:
	void Init();
	afx_msg void OnBnClickedCheck1();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheck2();

};
