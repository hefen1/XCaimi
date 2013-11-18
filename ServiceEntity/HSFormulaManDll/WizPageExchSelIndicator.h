/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchSelIndicator.h
*	�ļ���ʶ��	���ײ��Թ�ʽ����ʽ��������ҳ
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
#include "afxcmn.h"
//#include "ExpTabCtrl.h"
//#include "TreeCtrlFormula.h"
#include <afxwin.h>
#include "TreeCtrlFormula.h"
#include "ExpTabCtrl.h"
#include "WizPageExchBase.h"
#include "EditEidolon.h"
// CWizPageExchSelIndicator dialog

class CWizPageExchSelIndicator : public CWizPageExchBase
{
//	DECLARE_DYNAMIC(CWizPageExchSelIndicator)

public:
	CWizPageExchSelIndicator(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWizPageExchSelIndicator();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	long    LoadCurExp(WPARAM wParam = 0, LPARAM lParam = 0);
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive(int nPos = -1);
	virtual void OnSetActive();
	virtual LRESULT OnWizardNext();
protected:
	CEditEidolon m_EditEidolon;

public:
	CComboBox    m_wndPeriod;
	CExpTabCtrl	 m_wndTab;

public:
	TExchExp	m_sExp;
	afx_msg void OnBnClickedButton2();
	virtual BOOL  InitialData(TExchHead* pData);
	// �Ƿ�����Ȩ
	CButton m_ctlExRight;
};
