/*******************************************************
  源程序名称:TradeDataCoreIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据数据中心
        1、订阅、取消订阅数据接口定义
  	2、读取、设置数据接口定义 
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#define TRADE_SUB_FUND						0x00000001 //资金
#define	TRADE_SUB_STOCK_HOLD				0x00000002 //股票持仓
#define	TRADE_SUB_STOCK_ENTRUST				0x00000004 //股票委托
#define	TRADE_SUB_STOCK_DEAL				0x00000008 //股票成交
#define	TRADE_SUB_FUND_HOLD					0x00000010 //基金持仓
#define	TRADE_SUB_FUND_ENTRUST				0x00000020 //基金委托



#include "TradeDataBaseIntf.h"
#include "QueryStockEntrustIntf.h"
#include "QueryStockDealIntf.h"
#include "QueryFundIntf.h"
#include "QueryStockHoldIntf.h"
#include "QueryFundEntrustIntf.h"
#include "QueryFundHoldIntf.h"


enum EDataChangeType {icAdd, icModify, icDel, icReflash};

//需要订阅数据的类必须继承此类
struct ITradeDataCallBack
{
	virtual DWORD GetDataType() = 0;
	virtual void OnReflash(IDataList * pDataList, DWORD dDataType) = 0; 
	virtual void OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType) = 0; 
};

struct ITradeDataCore
{
	//释放
	virtual void Release() = 0;
	//订阅数据
	virtual void Subscribe(ITradeDataCallBack * pOwner) = 0;
	//取消订阅
	virtual void CancelSubscribe(ITradeDataCallBack * pOwner) = 0;
	//读取、设置资金
	virtual IFundList * GetFund(void) = 0;
	virtual void SetFund(IFundList * pDataList) = 0;
	//读取、设置股票持仓
	virtual IStockHoldList * GetStockHold(void) = 0;
	virtual void SetStockHold(IStockHoldList * pDataList) = 0;
	//读取、设置股票委托
	virtual IStockEntrustList * GetStockEntrust(void) = 0;
	virtual void SetStockEntrust(IStockEntrustList * pDataList) = 0;
	//读取、设置股票成交
	virtual IStockDealList * GetStockDeal(void) = 0;
	virtual void SetStockDeal(IStockDealList * pDataList) = 0;

	//读取、设置基金持仓
	virtual IFundHoldList * GetFundHold(void) = 0;
	virtual void SetFundHold(IFundHoldList * pDataList) = 0;
	//读取、设置基金委托
	virtual IFundEntrustList * GetFundEntrust(void) = 0;
	virtual void SetFundEntrust(IFundEntrustList * pDataList) = 0;
};