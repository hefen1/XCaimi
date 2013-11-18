/*************************************************************
*	Copyright (c)2002, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ExternTableAnalyze.h
*	�ļ���ʶ��	�����̺�����������ݴ���
*	ժ	  Ҫ��	Ŀǰ�Բ���.
*
*	��ǰ�汾��	2.0
*	��	  �ߣ�	���ӱ�
*	�������ڣ�	2003��4��18��
*
***************************************************************/

#if !defined(AFX_EXTERNTABLEANALYZE_H__6762347239057439875)
#define AFX_EXTERNTABLEANALYZE_H__6762347239057439875

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\comm\tableheadstruct.h"

class CExternTableAnalyze
{
public:
	CExternTableAnalyze();
	~CExternTableAnalyze();
protected:
	//ʱ������
	long m_lEnd;//��ʽ:20021010
	long m_lCount;//����
protected:
	//����������̺�����,�ֽ��������������(�����յ͵�)
	StockDay* GetPeriodStockDay(StockDay* pStockDay, short nPeriod);
	//���̺������ļ���ȡָ��ʱ���������������
	int GetData(RetDiskData* pData, StockUserInfo* pStock, long lEnd, long lCount);
	//���������
	inline long GetBeginDate(long lEnd, long lCount);
	//ȡ��ʱ���������ļ��е�λ��
	BOOL GetDataPosition(CFile *pSrcFile, long lSeekBeginDate, long *plBeginPos,
		long lSeekEndDate = -1, long *plEndPos  = NULL);
protected:
	void LongToFloat(long lData, long lClose, long lUnit, short nDecimal, float &fData);
};
#endif/*AFX_EXTERNTABLEANALYZE_H__6762347239057439875*/

long CExternTableAnalyze::GetBeginDate(long lEnd, long lCount)
{
	CTime tEnd(lEnd / 10000, (lEnd % 10000) / 100, lEnd % 100, 0, 0, 0);
	CTime tBegin = tEnd - CTimeSpan(lCount, 0, 0, 0);
	return (tBegin.GetYear() * 10000 + tBegin.GetMonth() * 100 + tBegin.GetDay()); 
}