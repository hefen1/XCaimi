/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	RichEditCtrlResult.h
*	�ļ���ʶ��	
*	ժ	  Ҫ��	�������ý����ʾ�ؼ���(Edit)
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�
*
*	�޸�˵����	
*
***************************************************************/

#pragma once
#include "RichEditCtrlEx.h"
#include "ExchData.h"
/*#include "XInfoTip.h"*/

class CRichEditCtrlResult :
	public CRichEditCtrlEx
{
public:
	CRichEditCtrlResult(void);
	~CRichEditCtrlResult(void);
	DECLARE_MESSAGE_MAP()

public:
	/******************************************************************
	*	�������ܣ�	��ʾ��������
	*	����������	void* pData		: [in] CExchCalcMain��������
	*				BOOL bSignal	: [in] ��ʾ����Ʊ���û���ȫ�г�����
	*				UINT uID = 0	: [in] ��ǰ����ID
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT Display(void* pData, BOOL bSignal, UINT uID = 0, TExchOptimize* pOptimize = NULL);

	/******************************************************************
	*	�������ܣ�	�õ��������������ִ�
	*	����������	void* pExchSys : [in] CExchCalcMain��������
	*	�� �� ֵ��	CString : �õ��������ִ�	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	CString GetBuyCon(void* pExchSys);

	/******************************************************************
	*	�������ܣ�	�õ��������������ִ�
	*	����������	void* pExchSys : [in] CExchCalcMain��������
	*	�� �� ֵ��	CString : �õ��������ִ�
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	CString GetSellCom(void* pExchSys);

	/******************************************************************
	*	�������ܣ�	�õ�ƽ�����������ִ�����
	*	����������	void* pExchSys		: [in] CExchCalcMain��������
	*				CStringArray& ayStr : [out] ƽ�������ִ�
	*	�� �� ֵ��	int : ƽ��������������
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	int GetPCCon(void* pExchSys, CStringArray& ayStr);

public:
	/******************************************************************
	*	�������ܣ�	��ʾ���Խ��
	*	����������	void* pData		: [in] CExchCalcMain��������
	*				int nIdx		: [in] Ҫ��ʾ�Ĺ�Ʊ����(-1ʱ��ʾ�ܵ�ͳ�ƽ��)
	*				UINT uID		: [in] Ҫ��ʾ����Ĳ���ID
	*				BOOL bIsSingal	: [in] "ժҪ"����"��ϸ"
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT Display(void* pData, int nIdx, UINT uID, BOOL bIsSingal);

	/******************************************************************
	*	�������ܣ�	��ʾ"ժҪ"���Խ��
	*	����������	void* pData		: [in] һ����Ʊ��ͳ�ƽ������
	*				void* pExchData : [in] CExchCalcMain��������
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT DisplaySingal(void* pData, void* pExchData);

	/******************************************************************
	*	�������ܣ�	��ʾ"��ϸ"���Խ��
	*	����������	void* pData		: [in] һ����Ʊ��ͳ�ƽ������
	*				void* pExchData	: [in] CExchCalcMain��������
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(-1)
	*	����������	 
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT DisplayMulti(void* pData, void* pExchData);

	/******************************************************************
	*	�������ܣ�	��һ���м�������Ŀ(�ĸ���,ÿ������һ��,�ֱ�Ϊ���⼰����)
	*	����������	CString str1 : [in] ��һ�������
	*				CString str2 : [in] ��һ�������
	*				CString str3 : [in] �ڶ��������
	*				CString str4 : [in] �ڶ��������
	*				COLORREF cl1 : [in] ������ʾɫ��
	*				COLORREF cl2 : [in] �����ݵ���ʾɫ��
	*				BOOL bLN = TRUE : [in] �Ƿ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddLineDoubleItem(CString str1, CString str2, CString str3, CString str4, COLORREF cl1, COLORREF cl2, BOOL bLN = TRUE);

	/******************************************************************
	*	�������ܣ�	��һ���м�һ����Ŀ(�ֱ�Ϊ����������),�������
	*	����������	CString str1 : [in] ����
	*				CString str2 : [in] ����
	*				COLORREF cl1 : [in] ������ʾɫ��
	*				COLORREF cl2 : [in] �����ݵ���ʾɫ��
	*				BOOL bLN = TRUE : [in] �Ƿ���
	*	�� �� ֵ��	void 
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	void AddLineSingleItem(CString str1, CString str2, COLORREF cl1, COLORREF cl2, BOOL bLN = TRUE);

public:
	/******************************************************************
	*	�������ܣ�	��ʾ�Ѻ���ʾ
	*	����������	LPCSTR lpMsg = NULL : [in] ��ʾ��Ϣ
	*	�� �� ֵ��	LRESULT : �ɹ�(1), ʧ��(-1)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	2003��4��26��
	*	�޸�˵����	
	*****************************************************************/
	LRESULT Display(LPCSTR lpMsg = NULL);

	// ��ʾ��Ϣ����
/*	CXInfoTip		m_pDatatip;*/
	// ��ʼ��TIP
	void InitialTip();
	CString	GetTipText(CString& strTitle,UINT nFlags, CPoint point,
		int* nBegin = NULL,int* nEnd = NULL);
	CString	GetKeyHelp(CString strKey);
	CString	FindWord(CString strText,int& nStart,int nCount);
	CString	CutWord(CString strText,BOOL bLeft);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
