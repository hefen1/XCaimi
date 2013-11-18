/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgExchOptimize.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	������ѡ���,Ŀǰ��ûʵ��
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/
#pragma once

#include "ExchCalcMain.h"
#include "resource.h"
#include "exchyouselcacldata.h"
#include "afxcmn.h"
#include <afxwin.h>
#include "afxdtctl.h"
// CDlgExchOptimize dialog

class CDlgExchOptimize : public CDialog
{
	DECLARE_DYNAMIC(CDlgExchOptimize)

public:
	CDlgExchOptimize(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExchOptimize();

// Dialog Data
	enum { IDD = IDD_EXCHAGNE_YOUSHUANG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

protected:
	// �������ݼ�����
	CExchYouSelCaclData		m_sCaclData;
	inline CExchYouSelCaclData* GetYouSelCaclData() { return &m_sCaclData; }

	// ���ؽ������
	void FailDispose(LPARAM lp);
	// �򿪼������ļ������ 
	BOOL OpenTableWnd(UINT uID = 0) {return m_sCaclData.OpenTableWnd(uID);}
	
	// �����̲߳���
	LRESULT Start() {return m_sCaclData.StartEx(); }
	LRESULT Stop() {return m_sCaclData.StopEx(); }

	void	SetExpression();
	// ����Ĭ�ϲ��Բ���
	void	SetDefTact() { m_sCaclData.SetDefTact(); }
	// �����Զ�����Բ���
	void	SetCommTact() { m_sCaclData.SetCommTact(); }

	// ���������Ϣ����
	LRESULT OnExchangeData(WPARAM wp,LPARAM lp);
public:
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedRadioComm();
	afx_msg void OnBnClickedCheckAllsel();
	afx_msg void OnBnClickedCheckAllloctldata();
	afx_msg void OnBnClickedOk();
	CListCtrl m_wndList;
	virtual BOOL OnInitDialog();
	CProgressCtrl m_wndProgress;
	CComboBox m_wndTestObj;
	CComboBox m_wndTestCyc;
	CDateTimeCtrl m_wndBeginDate;
	CDateTimeCtrl m_wndEndDate;
};
