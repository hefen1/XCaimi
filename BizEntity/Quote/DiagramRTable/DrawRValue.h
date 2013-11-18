/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawRValue.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Сͼֵ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-01-07
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once
#include "DrawTableBase.h"

#define RValue_ROW_HEIGHT     20
#define RValue_ROW_COUNT      18
#define RValue_COL_COUNT       2

class CDrawRValue :
	public CDrawTableBase
{
public:
	CDrawRValue(IDataSourceEx* pDataSource);
	~CDrawRValue(void);

	void CreateSelf();
	void HandleValueData(AnsValueData* pData); // ����Ӧ��
	
	WORD GetColor(double dNewValue, double dPreValue);
// interface
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
private: 
	IDataSourceEx*          m_pDataSource;    // ��������ָ��
 	AnsValueData*			m_ptagAnsValueData; // �洢�ĵ�ǰ����
	StockType               m_stkType;
};
