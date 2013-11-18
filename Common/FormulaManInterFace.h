/*************************************************************
*	Copyright (c)2010, �������ӹɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	FormulaManinterface.h
*	�ļ���ʶ��	��ʽ����ģ��ӿ�
*	ժ	  Ҫ��	
*
*	��ǰ�汾��	1.0
*	��	  �ߣ�	
*	�������ڣ�	
*
*	�޸�˵����	
*
***************************************************************/
#ifndef FORMULAMANINTERQFACE
#define FORMULAMANINTERQFACE

#include "hsds_comudata.h"
#include "FormulaManPublic.h"
/*#include "ExpPublic.h"*/
#define OBJ_FORMULAMAN	_T("FormulaMan.FormulaManager")

interface IFormulaMan
{
	/******************************************************************
	*	�������ܣ�	�����������ݵ�chsdatasorce�����б�m_ayDayData��
	*	����������	StockDay* pDay		: [in] ��������
	*				long lCurSize		: [in] �������ݳ���
	*				int nVolumeUnit		: [in] �ɽ�����λ
	*				int nMoneyUnit		: [in] �ɽ��λ
	*				short nMoneyDecimal	: [in] �ɽ���С��λ
	*				int nPriceUnit		: [in] �۸�λ
	*				short nPriceDecimal	: [in] �۸�С��λ
	*				WORD nKey = 0		: [in] Ϊ0ʱ,��ʾ�������е�K��,����,ֻ����һ����ӦnKey��ֵ������
	*	�� �� ֵ��	BOOL : �ɹ�(TRUE), ʧ��(FALSE)
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�	
	*	�޸�˵����	
	*****************************************************************/
	virtual BOOL SetDayData(StockDay* pDay,long lCurSize,
		int nVolumeUnit,
		int nMoneyUnit,short nMoneyDecimal,
		int nPriceUnit,short nPriceDecimal,CodeInfo* pCodeinfo = NULL,
		WORD nKey = 0) = 0;

	/******************************************************************
	*	�������ܣ�	��ʽ����
	*	����������	CString strname             : [in] ָ������
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] ���ؼ�����
	*				DWORD style					: [in] ָ������
	*				void* data                  : [out] ���ؽ��
	*				HSCurDataInfoStatus* pCurDiataStatus = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����
	*				CTValues<void>* pRefCalcValues = NULL : [out] ֻ���ڱ��ʽ����ʱ����,��ż�����λ��(��㼰�յ�)
	*	�� �� ֵ��	BOOL  : ��������ȷ(FALSE), ������Ϊ(TRUE)
	*	�� �� ֵ��	long : ������Ϣ�ķ���ֵ������ͬ,��������Ϣ��������˵��
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/
 	virtual BOOL CalcValue(  CString strName,DWORD style,CArray<CLineProperty*,CLineProperty*>* pAyData
		/*DWORD style ,*/
		/*,HSCurDataInfoStatus* pCurDataStatus = NULL,*/
		/*CTValues<void>* pRefCalcValues = NULL*/) = 0;
	/******************************************************************
	*	�������ܣ�	��ȡָ���߸���
	*	����������	CString strname             : [in] ָ������
	*				CArray<CLineProperty*,CLineProperty*>* pAyData  [out] ���ؼ�����
	*				DWORD style					: [in] ָ������
	*				
	*	�� �� ֵ��	int : �ߵĸ���
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual int GetLineNum(CString strName,DWORD style	) = 0;
	/******************************************************************
	*	�������ܣ�	��ȡָ�����
	*	����������	CString strname             : [in] ָ������
	*			
	*				DWORD style					: [in] ָ������
	*				
	*	�� �� ֵ��	char*   [out] ���ؼ�����
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual CString GetExpressVar(CString strName,DWORD style	) = 0;
	/******************************************************************
	*	�������ܣ�	�������ݴ���������߳�Ȩ,����Ȩ��
	*	����������	Dword style   : [in] ���ݴ�����
	*			
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual void	AddStyle(long dwStyle) =0;
	/******************************************************************
	*	�������ܣ�	�������Ƶ�ĳ���ֲ���ƥ�������ָ��,���ڼ��̾�����ҹ�ʽ
	*	����������	
	*				CArray<ExpPropery*,ExpPropery*> [out]��Ų��ҽ��
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual void	GetExpProp(CArray<ExpPropery*,ExpPropery*>& ayNameProp) =0;
	/******************************************************************
	*	�������ܣ�	������ʽ����
	*	����������	CString strname             : [in] ָ������
	*			    DWORD style					: [in] ָ������
	*				DWORD dwType ��������
	*	
	*					 hxOpenExpressByConditionStockType	 0x0900 // ����ѡ�ɴ�
	*					 hxOpenExpressByConditionDingZhi		 0x0910 // ����ѡ�ɴ�
	*					 hxOpenExpressByConditionZhiBiao		 0x0920 // ָ��ѡ�ɴ�
	*					 hxOpenExpressByConditionZhiNeng		 0x0930 // ����ѡ�ɴ�
	*					 hxOpenExpressByConditionChaJian		 0x0940 // ���ѡ�ɴ�
	*					 hxOpenExchangeSys            		 0x0950 // ����ϵͳ
	*					 hxOpenExchangeYouSel				 0x0960	// ������ѡ
	*					 hxOpenExpressBySortStockType		 0x0A00 // ָ�������
	*					 hxOpenBlockAnalysis					 0x0B00 // ������
	*					 hxOpenUniteAnalysis					 0x0C00 // ��������
	*					 hxOpenFindDataDlg					 0x0D00 // �򿪲���dlg
	*					 hxOpenAlarmManagerDlg				 0x0E00 // ��Ԥ������Ի���
	*					 hxOpenAddStkToAlarm					 0x1000 // �򿪼��뵽Ԥ���Ի���
	*				
	*	�� �� ֵ��	
	*	����������	
	*	��    �ߣ�	
	*	������ڣ�
	*	�޸�˵����	
	*****************************************************************/ 
	virtual void  OpenExpWindow(CString strName, DWORD dwStyle, DWORD dwType) = 0;
	
};


#endif