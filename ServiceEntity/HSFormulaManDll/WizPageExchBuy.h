/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchBuy.h
*	�ļ���ʶ��	���ײ����������������뷽������ҳ
*	ժ	  Ҫ��	���ײ��Խ������,�Ժ���д
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once
//#include "afxdtctl.h"
//#include <afxwin.h>
#include "TreeCtrlFormula.h"



#include "WizPageExchBase.h"

// CWizPageExchBuy dialog

class CWizPageExchBuy : public CWizPageExchBase
{
public:
	CWizPageExchBuy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchBuy();
	virtual BOOL OnKillActive(int nPos = -1);
	virtual void	OnSetActive();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_BUY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComcon1();
	virtual BOOL OnInitDialog();
	virtual BOOL SetData();
	void         InitCondition();

public:
	CDateTimeCtrl m_wndTimeBegin;
	CDateTimeCtrl m_wndTimeEnd;
	CButton m_wndButtonDecount;
	CComboBox m_wndConditionA;
	CComboBox m_wndConditionR;
	CComboBox m_wndConditionB1;
	CComboBox m_wndConditionB2;
	CStatic m_wndStaticRelation;
	CComboBox m_wndBuyMethod;
	CStatic m_wndStaticBuyMethod;
	CComboBox m_wndAddMethod;
	CStatic m_wndStaticAddMethod;
	CEdit m_wndEditBuyMount;
	CEdit m_wndEditAddMount;
	CButton m_wndCheckSkip;
	CEdit m_wndEditSkipPeriod;
	CStatic m_wndStaticMiddle;
	
public:
	TExchBuy		  m_TExchangeBuy;
	
	afx_msg void OnCbnSelchangeComuse();
	afx_msg void OnCbnSelchangeComfollow();

	// nIdx �������뷽��
	void ShowLSWin(int nIdx);
	// nCmd ��ʾ����
	void ShowLastLsWin(int nCmd);
	// nIdx �������
	void ShowBuyWin(int nIdx);
	// nCmd ��ʾ����
	void ShowLastBuyWin(int nCmd);
	// nIdx ���뷽��
	void EnableLSWin(int nIdx);
	// nIdx���뷽��
	void ShowBuyMedthWin(int nIdx);
	void ShowMedthWin(int nCmd);
	// nIdx �������뷽��
	void EnableSkipWin(int nIdx);
	// ����һ�������б�
	void FullConOne(int nIdx);
	// ���ڶ��������б�
	void FullConTwo(BOOL bIsTech = TRUE);
	// ��似��ָ��
	void FullTech();
	// ��ʾ����
	void PlayData();
	// ��ʾ����
	void SetSelectData(CString& str, CComboBox& wndCom);
	// �õ���ʽ
	CExpression* GetExp();

	afx_msg void OnCbnSelchangeComcond();
	afx_msg void OnCbnSelchangeComcon2();
	afx_msg void OnCbnKillfocusComcon2();
	virtual BOOL  InitialData(TExchHead* pData);
};
