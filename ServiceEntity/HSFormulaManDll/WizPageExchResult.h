/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchResult.h
*	�ļ���ʶ��	���ײ�������������ʾҳ
*	ժ	  Ҫ��	���ײ��Խ������,�Ժ���д
*
*	��ǰ�汾��	2.0
*	��	  �ߣ�	����ʯ
*	�������ڣ�	2003��4��26��
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
#include "WizPageExchBase.h"
#include "RichEditCtrlResult.h"

#define IDC_RESULT_ID	50000
// CWizPageExchResult dialog

class CWizPageExchResult : public CWizPageExchBase
{
//	DECLARE_DYNAMIC(CWizPageExchResult)

public:
	CWizPageExchResult(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchResult();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	void OnSetActive();

public:
	CRichEditCtrlResult	m_wndResult;
	TExchOptimize		m_sLocalOptimize;

	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1);
	long Initial();
	long PlayData();
	afx_msg void OnBnClickedModefy();
};
