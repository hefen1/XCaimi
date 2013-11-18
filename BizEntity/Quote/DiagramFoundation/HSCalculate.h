/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CHSCalculate.h
*	文件标识：
*	摘	  要：  计算类
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-03-01
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "..\..\..\Common\hsstructnew.h"
#include "..\..\..\Common\FormulaManPublic.h"
#include "..\..\..\Common\DataSourceDefineEx.h"
#include "..\..\..\Common\QuoteComm\QuoteDefine.h"
#include "..\..\..\Common\QuoteComm\InfoStruct.h"
#include "DiagramFoundationDef.h"

#define STR_LENTH    64

class HS_EXT_CLASS CHSCalculate
{
public:
	CHSCalculate(void);
	~CHSCalculate(void);

	// 计算买卖量 lValue是返回包中买卖量的值
	static double GetBuySellCount(const StockUserInfo* pStockInfo, long lValue, char strText[64]); 
	// 计算委比   委比＝(委买手数－委卖手数)/(委买手数＋委卖手数)×100％
	static double GetWeiBi(const HSStockRealTime* pRealTime, char strText[64]);  
	static double GetWeiBi(const LevelRealTime* pLevel, char strText[64]);
	static double GetWeiBi(const HSIndexRealTime* pRealTime, char strText[64]);
	// 计算涨跌
	static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
    static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetZhangdie(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// 计算幅度 幅度(涨幅=(现价-昨收)/昨收)
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);   
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetFudu(BOOL bStop, const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// 计算总手
	static double GetZongshou(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
	static double GetZongshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	static double GetZongshou(const HSIndexRealTime* pRealTime, int nHand, short nMask, char strText[64]);
	// 计算现手
	static double GetXianshou(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]);
	static double GetXianshou(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]);
	// 计算市盈
	static double GetShiying(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);
	static double GetShiying(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);
	// 计算流通
	static double Getliutong(CHSFinanceData* pFinanceData, char strText[64]);
	// 计算市值
	static double GetShizhi(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] );  
	static double GetShizhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] );
	// 计算委差
	static double GetWeicha(const HSStockRealTime* pRealTime, int nHand, char strText[64]);
	static double GetWeicha(const LevelRealTime* pLevel, int nHand, char strText[64]);
	static double GetWeicha(const HSIndexRealTime* pRealTime, int nHand, char strText[64]);
	// 计算均价 均价 ＝（E分时成交的量×成交价）/总成交股数
	static double GetJunjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);      
	static double GetJunjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);      
	// 计算量比
	static double GetLiangbi(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, const StockOtherData* pOther, IDataSourceEx* pDataSource, char strText[64]);  
	static double GetLiangbi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, const LevelStockOtherData* pLevelOther, IDataSourceEx* pDataSource, char strText[64]);  
	// 计算换手
	static double GetHuanshou(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]); 
	static double GetHuanshou(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]); 
	// 计算全价
	static double GetQuanjia(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);   
	static double GetQuanjia(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);   
	// 计算市净
	static double GetShijing(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64] );  
	static double GetShijing(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64] );  
	// 计算总股
	static double GetZonggu(CHSFinanceData* pFinanceData, char strText[64]);  
	// 计算总值
	static double GetZongzhi(const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);  
	static double GetZongzhi(const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);  
	// 计算外盘
	static double GetWaipan(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]); 
	static double GetWaipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]); 
	// 计算内盘
	static double GetNeipan(const StockUserInfo* pStockInfo, const StockOtherData* pOther, char strText[64]);
	static double GetNeipan(const StockUserInfo* pStockInfo, const LevelStockOtherData* pLevelOther, char strText[64]);
	// 计算IOPV
	static double GetIOPV(const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64]); 
	// 计算净值
	static double GetJingZhi(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, CHSFinanceData* pFinanceData, char strText[64]);
	static double GetJingZhi(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, CHSFinanceData* pFinanceData, char strText[64]);
	// 计算反汇率
	static double GetFanHuiLv(const StockUserInfo* pStockInfo, const HSStockRealTime_Ext* pRealTimeExt, char strText[64]);
	static double GetFanHuiLv(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// 计算涨速
	static double GetZhangSu(const StockUserInfo* pStockInfo, const HSStockRealTimeOther* pRealOther, char strText[64]);
	// 计算振幅
	static double GetZhenFu(const StockUserInfo* pStockInfo, const HSStockRealTime* pRealTime, char strText[64]);
	static double GetZhenFu(const StockUserInfo* pStockInfo, const HSIndexRealTime* pIndex, char strText[64]);
	// 计算每笔
	static double GetMeiBi(const LevelRealTime* pLevel, char strText[64]);
	// 计算卖均
	static double GetSellAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);
	// 计算买均
	static double GetBuyAvg(const StockUserInfo* pStockInfo, const LevelRealTime* pLevel, char strText[64]);

	// 传入文本标题以及文本内容 制作一个html文本 并且返回本地URL地址
	static CString CreateHtml(const CString& strContent, const InfoIndex* pInfoIndex);
	//////////////////////////////////////////////////////////////////////////
	// 计算时间 lPreTime只有当bIsFullTime = TRUE时才起作用
	static CTime  GetTimer(const StockUserInfo* pStockInfo, const StockTick* pTick, StockType* pTypeInfo, BOOL bIsFullTime, long lDate, long lPreTime, char strText[64]);
	// 构造一个时间类型
	static CTime ConstructTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,int nDST = -1);
    //////////////////////////////////////////////////////////////////////////
	// 财务数据
	static void  GetTextByID(int nID, CurrentFinanceData* pFinanceData, char strText[64]); // 根据ID 从财务数据中取数据返回字符串
	static float GetDataByID(int nID, CurrentFinanceData* pFinanceData); // 根据ID 从财务数据中取数据
	static void  AutoAdjustUnit(WORD wKey, double dbData, CString &strUnit, int *pUnit); // 调整数据单位 
};