/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DlgExchParamSet.h
*	�ļ���ʶ��	���ײ��Բ�����ѡ���ý���
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
#include <afxwin.h>
#include "ExchData.h"
// CDlgExchParamSet dialog


class CDlgExchParamSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgExchParamSet)

public:
	CDlgExchParamSet(void* pData = NULL, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExchParamSet();

// Dialog Data
	enum { IDD = IDD_EXCH_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// ����һ
	CButton m_wndCheckParam1;
	// ������
	CButton m_wndCheckParam2;
	// ������
	CButton m_wndCheckParam3;
	// ������
	CButton m_wndCheckParam4;

	BOOL m_bHaveChange;

protected:
	// ��ʾ����������ݣ��������
	void ShowLine(int nLineNum = 0);
	// ����һ������
	void SetLineData(int nLineIdx, CString strParamName, double dMin, double dMax, double dAdd);
	// �õ�һ������
	BOOL GetLineData(int nLineIdx, TExchOptimizeItem* pItem = NULL);
	// ʹһ����Ч����Ч
	void EnableLine(int nLineIdx, BOOL bValid = TRUE);
	// �õ���������
	int GetMaxLine() { return 4; }
	// ��ʾͳ�Ƽ������
	void ShowTotal();

protected:
	// �Ż�����
	TExchOptimize*	m_pData;
	TExchOptimize	m_localData;

public:
	void SetOptimizeData(TExchOptimize*	pData) 
	{ 
		if( pData == NULL )
		{
			return;
		}
		m_pData = pData; 
		m_localData = *pData;
	}
	TExchOptimize* GetOptimizeData() { return &m_localData; }

protected:
	// ��ʾ����
	BOOL PlayData();
	// �õ�����
	BOOL SaveData();

public:
	void OnChangeEdit(UINT uID);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel2();
};
