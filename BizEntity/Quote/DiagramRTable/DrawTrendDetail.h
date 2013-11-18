/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDrawTrendDetail.h
*	文件标识：
*	摘	  要：  F1-成交明细图元
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-03-08
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
	void    AddRowData(StockTick* pStockTick, int nIndex = -1);    // 增加一行数据
	WORD    GetTextColorID(double newValue, double preValue); // 通过前后两个值的比较 得到颜色ID
private: 
	StockType                 m_StockType;     // 当前股票的一些信息
	IDataSourceEx*	          m_pDataSource;   // 数据引擎接口指针

	long    m_lDate;            // 服务器日期
	long    m_lTotalVol;        // 总成交量
	unsigned long m_nTotalHand;	// 总持仓量

	long    m_lPreTime;       // 前一个包的时间 
	char    m_strPreTime[64]; // 前一个包的时间
	long    m_lPreNewPrice;   // 前一个包的成交价
	long    m_lPreSellPrice;  // 前一卖价
	long    m_lPreBuyPrice;   // 前一买价

	BOOL    m_bDispFour;      // 是否显示成交(只有深圳市场才有)
	long    m_lStockTickCount;// tick包中数量

	long    m_lTickCount;
};
