/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchSell.h
*	�ļ���ʶ��	���ײ���������������ҳ
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
#include <afxwin.h>
#include "WizPageExchBase.h"

#define FORMULAMAN_EXCH_PROMPT				_T("����ϵͳ��ʾ")
// CWizPageExchSell dialog

class CWizPageExchSell : public CWizPageExchBase
{
	DECLARE_DYNAMIC(CWizPageExchSell)
public:
	CWizPageExchSell(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchSell();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_SELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1 );

public:
	CEdit m_wndEditSellPeriod;
	CEdit m_wndEditWin;
	CEdit m_wndEditLose;
	CEdit m_wndEditFallDay;
	CEdit m_wndEditFallRation;
	CEdit m_wndEditUpDay;
	CEdit m_wndEditUpRation;
	CButton m_wndChkBuy;
	CButton m_wndChkBuy1;
	CButton m_wndChkLose;
	CButton m_wndChkDown;
	CButton m_wndChkHp;

public:
	TExchSell	m_sExchSell;
	
public:
	virtual BOOL  SetData();
	BOOL InValidTest();
	void PlayData();
public:
	void OnSetActive();
	// ��ʼ����������
	virtual BOOL  InitialData(TExchHead* pData);
};
