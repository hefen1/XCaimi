#pragma once
/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	datacache.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	���ݻ���ͷ�ļ�
*	
*	��ǰ�汾��
*	��	  �ߣ�	xudan
*	������ڣ�  2010-4-6
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#include "HSDisk.h"

class CDataCache
{
public:
	CDataCache(void);
	virtual ~CDataCache(void);

	//�����ػ����ļ�
	void* ReadFile( AskData* pCurAsk ,long& lRetSize);
	//д���ػ����ļ�
	void WriteFile(void* pData, CodeInfo* pCode);

	//���ɰ�ͷ
	void MakeHeadData(DataHead*, AskData* );

	//�̺��������������
	void* MakeDayData(AskData*& pCurAsk, long& nSize,BOOL& bSend);
	//����ʷ��ʱ,
	void* MakeHisTraceData(AskData* pCurAsk,long& lSize);
	//����ʱ
	void* MakeTraceNewData(AskData* pCurAsk,long& lSize);
	//���ֱ�
	void* MakeTickData(AskData* pCurAsk,long& lSize);

	//�̺����д��������
	void WriteDayData(CodeInfo* info, int period,StockCompDayDataEx* pNewData,long nSize );
	//д��ʷ��ʱ��
	void WriteHisTraceData(void* pData, CodeInfo* pCode);
	//д��ʱ
	void WriteTraceNewData(void* pData, CodeInfo* pCode);
	void WriteTickData(void* pData,CodeInfo* pCode);
	//����Ѷ�ļ�
	void* MakeInfoData(AskData* pAsk, long& lRet);
	//д��Ѷ�ļ�
	void WriteInfoData(void* pData,CodeInfo* pCode);
	
	void SetDate(long lCurDate, long lSvrTime);
	//��ʼ��ʱɾ����ʱtick�ļ�
	BOOL DelTraceFile();
protected:
	CHSDisk m_disk;   //��д�ļ�
	long    m_nCurlate;  //����ʱ��
	long    m_nSvrTine;   //������ʱ��
};
