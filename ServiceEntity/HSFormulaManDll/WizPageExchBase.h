/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	WizPageExchBase.h
*	�ļ���ʶ��	���ײ��Լ�����ҳ�Ļ���
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
#include "ExchCalcMain.h"
#include "HSNewWizPage.h"
 class CWizPageExchBase:	public CHSNewWizPage
{
public:
	CWizPageExchBase(CWnd* pParent = NULL);   // standard constructor
	CWizPageExchBase(LPCTSTR lpszTemplateName, CWnd* pParent = NULL);
	CWizPageExchBase(UINT nIDTemplate, CWnd* pParent = NULL);
	~CWizPageExchBase(void);

public:
	void* m_pData;
	static CExchCalcMain	  *m_pMainTech;

public:
	static void SetMainTech(CExchCalcMain* pTech)
	{
		m_pMainTech = pTech;
	}

	virtual BOOL SaveOption(CExchCalcMain *pMainTech = NULL)
	{
		if( pMainTech == NULL )
		{
			if( m_pMainTech != NULL )
				return m_pMainTech->SetData((TExchHead*)m_pData);
		}
		else
		{
			return pMainTech->SetData((TExchHead*)m_pData);
		}
		return FALSE;
	}

	virtual BOOL SaveOptionEx(CExchCalcMain *pMainTech = NULL)
	{
		if( pMainTech == NULL )
		{
			if( m_pMainTech != NULL )
				return (m_pMainTech->SetData((TExchHead*)m_pData)&&m_pMainTech->SetOptimize(((TExchExp*)m_pData)->m_pMainTech));
		}
		else
		{
			return (pMainTech->SetData((TExchHead*)m_pData)&&m_pMainTech->SetOptimize(((TExchExp*)m_pData)->m_pMainTech));
		}
		return FALSE;
	}
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	virtual BOOL  SetData() { return FALSE; }
	virtual BOOL  InitialData(TExchHead* pData) { return FALSE; }
};
