/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawRFinance.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ��Сͼ��������
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


#define RFinance_ROW_HEIGHT     22
#define RFinance_COL            2
#define RFinance_Text_Color     RGB(128, 128, 128)

class CDrawRFinance :
	public CDrawTableBase
{
public:
	CDrawRFinance(IDataSourceEx* pDataSource);
	~CDrawRFinance(void);

	BOOL	CreateSelf(); 
	virtual	BOOL    GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
protected:
	void    OnUpdateFinaceData(); // �����������
private:
	CArray<CurrentFinanceData> m_ayFinanceData; // ������������Ĳ�������
	CUIntArray                 m_ayTitleInt;    // ��Ҫ��ʾ�Ĳ�����궨��
	CStringArray               m_ayTitleStr;    // ��Ҫ��ʾ�Ĳ������ַ��� 

	IDataSourceEx*	           m_pDataSource;   // ��������ӿ�ָ��
};
