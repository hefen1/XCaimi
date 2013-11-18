/*************************************************************
*	Copyright(c) 2010, ���ݺ����ɷ����޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	DrawRLevelFrom.h
*	�ļ���ʶ��
*	ժ	  Ҫ��  ʮ������ͼԪ
*	
*	��ǰ�汾��
*	��	  �ߣ�	����
*	������ڣ�  2011-04-21
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

class CDrawRLevelForm :
	public CDrawTableBase
{
public:
	CDrawRLevelForm(IDataSourceEx* pDataSource);
	~CDrawRLevelForm(void);

	BOOL	 CreateSelf();  // ����ʮ��
	BOOL     UpdateData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);
	WORD     GetTextColor(double newValue, double preValue);
//interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
protected:
	void    InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[10]);

	WORD    GetSellChange(long lSellPrice, long lNowSellCount, char* strText); // ���������仯
	WORD    GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText);    // ���������仯
private:
	CMap<long,long,long,long> m_buyMap;  // ��1~10��
	CMap<long,long,long,long> m_sellMap; // ��1~10��

	IDataSourceEx*	           m_pDataSource;   // ��������ӿ�ָ��
	RealTimeDataLevel          m_sLevelData;    // ����������ͬ��level2���ư�
};
