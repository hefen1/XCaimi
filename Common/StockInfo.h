/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	StockInfo.h
*	文件标识：
*	摘	  要：	数据引擎接口股票数据定义头文件
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-7-19
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
//HSDS--hundsun datasource
#include "hsds_comudata.h"

/*复合类型数据都有此数据头，m_lUpdateTime来指定数据引擎是否需要重新到服务端更新数据*/
struct CHSDSDataHead
{
	short					m_nSize;			//数据个数
	unsigned long			m_lUpdateTime;		//数据更新时间
};

/*实时其它数据*/
struct CHSDSStockOtherData : public CHSDSDataHead
{
	StockOtherData m_othData;
};

//时时数据
struct CHSDSStockRealTime : public CHSDSDataHead
{
	HSStockRealTime	m_stockRealTime;
};

//扩展数据
struct CHSDSShareRealTimeData : public CHSDSDataHead
{
	ShareRealTimeData_Ext	m_stockRealTime;	
};

//历史回忆、多日分时、右小图下分时走势
struct CHSDSHisTrendData : public CHSDSDataHead
{
//	StockCompHistoryData*		m_pHisData;
};

// 分笔记录
struct CHSDSStockTick : public CHSDSDataHead
{
//	StockTick*				m_pStockTick;
};

//分时走势
struct CHSDSPriceVolItem : public CHSDSDataHead
{
//	PriceVolItem*			m_pPriceVolItem;
};

//个股买卖盘
struct CHSDSBuySellOrder : public CHSDSDataHead
{
//	BuySellOrderData		m_buysellorderData;
};


//日线数据
struct CHSDSStockCompDayData : public CHSDSDataHead
{
//	StockCompDayDataEx*		m_pStockCompDayData;
};

/* 买卖力道数据项 */
struct CHSDSBuySellPowerData : public CHSDSDataHead
{
//	BuySellPowerData*		m_pBuySellPowerData;
};

//历史财务数据
struct CHSDSHisFinanceData : public CHSDSDataHead
{
//	HisFinanceData*			m_pHisfinanceData;
};

//右小图“值”的数据(股票)
struct CHSDSCalcData_Share : public CHSDSDataHead
{
	long			m_nTime;			// 时间，距离开盘分钟数
	unsigned long	m_lTotal;			// 总手
	float			m_fAvgPrice;		// 总金额
	long			m_lNewPrice;		// 最新价
	long			m_lTickCount;		// 成交笔数	
//	CalcData_Share	m_Share;			// 股票计算

};

//逐笔成交
struct CHSDSHSLevelTransaction : public CHSDSDataHead
{
	HSLevelTransaction*			m_pLeveltransaction;
};

//历史回忆分笔数据结构
struct CHSDSHistroyInfo_Deal : public CHSDSDataHead
{
//	HistroyInfo_Deal*		m_pHistroyInfoDeal;
};

/*股票、指数、期货、港股等*/
struct CHSDSStockInfoIntact
{
	StockUserInfo				m_stockInfo;

	CHSDSStockOtherData*		m_pOthData;			// 实时其它数据
	CHSDSStockRealTime*			m_pStockRealTime;	//时时数据
	CHSDSShareRealTimeData*		m_pStockOther;		//扩展数据
	CHSDSHisTrendData*			m_pHisTrend;		//历史回忆、多日分时、右小图下分时走势
	CHSDSStockTick*				m_pStockTick;		//分笔记录
	CHSDSPriceVolItem*			m_pPriceVolItem;	//分笔记录
	CHSDSBuySellOrder*			m_pBuySellOrder;	//个股买卖盘
	CHSDSStockCompDayData*		m_pStockCompDayData;//日线数据
	CHSDSBuySellPowerData*		m_pBuySellPowerData;//买卖力道数据项
	CHSDSHisFinanceData*		m_pHisFinanceData;	//历史财务数据
	CHSDSCalcData_Share*		m_pCalcDataShare;	//右小图“值”的数据(股票)
	CHSDSHSLevelTransaction*	m_pLevelTransaction;//逐笔成交
	CHSDSHistroyInfo_Deal*		m_pHistroyInfo;		//历史回忆分笔数据结构

	
	void*                       m_pData;              //本地缓存数据
	long                        m_lSize;              //缓存数据长度
};

