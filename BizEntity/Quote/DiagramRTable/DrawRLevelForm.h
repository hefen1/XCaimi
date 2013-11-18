/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	DrawRLevelFrom.h
*	文件标识：
*	摘	  要：  十档行情图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-04-21
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
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

	BOOL	 CreateSelf();  // 创建十挡
	BOOL     UpdateData(LevelRealTime* pLevelData, LevelStockOtherData* pOtherData);
	WORD     GetTextColor(double newValue, double preValue);
//interface
	virtual	BOOL	GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce = FALSE);
	virtual int		OnUpdateData(DataHead* pData);
	virtual BOOL    MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData = NULL);
	virtual BOOL    RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData = NULL);
protected:
	void    InitPriceMap(CMap<long,long,long,long> &lMap, long lPrice[10]);

	WORD    GetSellChange(long lSellPrice, long lNowSellCount, char* strText); // 计算卖量变化
	WORD    GetBuyChange(long lBuyPrice, long lNowBuyCount, char* strText);    // 计算买量变化
private:
	CMap<long,long,long,long> m_buyMap;  // 买1~10量
	CMap<long,long,long,long> m_sellMap; // 卖1~10量

	IDataSourceEx*	           m_pDataSource;   // 数据引擎接口指针
	RealTimeDataLevel          m_sLevelData;    // 用来过滤相同的level2主推包
};
