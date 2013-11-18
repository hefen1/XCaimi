/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawRLevelTick.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  �������(LeveL2)
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-04-19
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
#include "DataSourceDefineEx.h"

#define RLEVELTICK_ROW   9

class CDrawRLevelTick :
	public CDrawTableBase
{
public:
	CDrawRLevelTick(IDataSourceEx* pDataSource);
	virtual ~CDrawRLevelTick(void);

	BOOL	CreateSelf(); 
// interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo);
	virtual int		OnUpdateData(DataHead* pData);
private:
	// ����ʵʱ
	void            HandleStockTrace(StockTick* pData);
// ����
private:
	BOOL m_bDispFour;           // �Ƿ���ʾ������ �ֽ׶��Ϻ�����ʾ ������ʾ
	BOOL m_bFullTime;           // ���ÿ���: �Ƿ���ʾ����ʱ��

	long m_lBuyPrice;           // ���
	long m_lSellPrice;          // ����
	unsigned long m_lCurrentTotal; // ��ǰ�ܱ�
	unsigned long m_nHand;         // ����
	long          m_lPreTime;	   // ǰһʱ��
	unsigned short m_nLineNum;     // ��ǰ���µ�����
	long	       m_lPreNewPrice; // ǰһ�ɽ���

	StockType       m_StockType;     // ��ȡ��ǰ��Ʊ��һЩ��Ϣ ���ڼ���ʱ��
	CodeInfo        m_preCodeInfo;   // ͼԪ֮ǰ�Ĺ�Ʊ��Ϣ   
	IDataSourceEx*	m_pDataSource;   // ��������ӿ�ָ��
};
