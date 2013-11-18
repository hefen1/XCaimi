/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	CDrawTrendDetail.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  F1-�ɽ���ϸͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-03-08
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

#define TickCellWidth 150

class CDrawTrendDetail :
	public CDrawTableBase
{
public:
	CDrawTrendDetail(IDataSourceEx*	pDataSource);
	~CDrawTrendDetail(void);

	//interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo,BOOL bForce = FALSE);
	virtual int     OnUpdateData( DataHead* pData );
	virtual int     Draw(CDC* pDC);
	//enum ITEMID{TIMEITEM = 0, CHENGJIAOITME, ZHANGDIEITEM, MAIRUITEM, MAICHUITEM, XIANSHOUITEM};
protected:	
	void    AddRowData(StockTick* pStockTick, int nIndex = -1);    // ����һ������
	WORD    GetTextColorID(double newValue, double preValue); // ͨ��ǰ������ֵ�ıȽ� �õ���ɫID
private: 
	StockType                 m_StockType;     // ��ǰ��Ʊ��һЩ��Ϣ
	IDataSourceEx*	          m_pDataSource;   // ��������ӿ�ָ��

	long    m_lDate;            // ����������
	long    m_lTotalVol;        // �ܳɽ���
	unsigned long m_nTotalHand;	// �ֲܳ���

	long    m_lPreTime;       // ǰһ������ʱ�� 
	char    m_strPreTime[64]; // ǰһ������ʱ��
	long    m_lPreNewPrice;   // ǰһ�����ĳɽ���
	long    m_lPreSellPrice;  // ǰһ����
	long    m_lPreBuyPrice;   // ǰһ���

	BOOL    m_bDispFour;      // �Ƿ���ʾ�ɽ�(ֻ�������г�����)
	long    m_lStockTickCount;// tick��������

	long    m_lTickCount;
};
