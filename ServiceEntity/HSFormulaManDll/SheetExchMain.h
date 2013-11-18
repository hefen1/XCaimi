/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	SheetExchMain.h
*	�ļ���ʶ��	���ײ��Ե���ҳͣ�����ڼ��������ݵĹ�����
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
#include "ExchCalcMain.h"
#include "DlgSingleExchCalcResult.h"
#include "TabCtrlExch.h"
#include "WizPageUnionExchCaclResult.h"
#include "HSNewWizDialog.h"

// CSheetExchMain dialog
#define PROGRESS_ID		40000
#define TAB_ID			(PROGRESS_ID+1)

class CSheetExchMain : public CHSNewWizDialog
{
	
public:
	CSheetExchMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSheetExchMain();

// Dialog Data
	enum { IDD = IDD_EXCHANGE_UNION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
	
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
	LRESULT OnUserDragData(WPARAM wParam, LPARAM lParam);

public:
	int Select();
	int	Start();
	int Stop();
	BOOL IsRunCalc();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedWizback();
	afx_msg void OnBnClickedWiznext();
	afx_msg void OnBnClickedWizfinish();
	afx_msg void OnBnClickedExit();

public:
	CExchCalcMain	m_pMainTech;
	CExchCalcMain*  m_pRefMainTech;
	CExchCalcMain* GetData() 
	{ 
		if( m_pRefMainTech ) 
			return m_pRefMainTech;
		return &m_pMainTech; 
	}

	// ��������
	int Apply();
	// ������Ϣ����
	static LRESULT OnMsgPress(DWORD dwMsg, WPARAM wp, LPARAM lp);

	// ������
	CProgressCtrl	m_wndPress;
	CTabCtrlExch		m_wndPageTab;
	// ��ť״̬
	void SetStartBom(BOOL bStop);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetCurTab(int nAdd);
	void SetCurTab();

public:
	CDlgSingleExchCalcResult*	m_dlgTable;

	BOOL OpenTableWnd(UINT uID = 0);

	void ShowExch();
	void FailDispose(LPARAM lp);

	// ɾ������ҳ
	BOOL DelCaclPage(UINT nIDTemplate = IDD_EXCHANGE_CACL_RESULT);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
