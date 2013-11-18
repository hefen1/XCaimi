/*******************************************************
  源程序名称:QueryStockHoldIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询持仓接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"


struct IStockHoldItem : virtual public IDataItem
{
	//证券代码
    virtual CString GetStockCode() = 0;
	//市场
	virtual CString GetExchangeType() = 0;
	//股东账号
	virtual CString GetStockAccount() = 0;
	//股票余额
	virtual long GetCurrentAmount() = 0;
	//可用数量
	virtual long GetEnableAmount() = 0;
	//最新价
	virtual double GetLastPrice() = 0;
	virtual void SetLastPrice(const double Value) = 0;
	//成本价
	virtual double GetCostPrice() = 0;
	//市值
	virtual double GetMarketBalance() = 0;
	virtual void SetMarketBalance(const double Value) = 0;
	//买卖盈亏
	virtual double GetIncomeBalance() = 0;
	virtual void SetIncomeBalance(const double Value) = 0;
	//盈亏比例
	virtual double GetIncomeRatio() = 0;
	virtual void SetIncomeRatio(const double Value) = 0;
	//定位串
	virtual CString GetPositionStr() = 0;
	//席位
	virtual CString GetSeat() = 0;
};

struct IStockHoldList : virtual public IDataList
{
	virtual IStockHoldItem * GetItems(const int i) = 0; 
	virtual IStockHoldItem * NewItem() = 0;
};


struct IReqQueryStockHold : virtual public ITrading
{
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//定位串
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IStockHoldList* GetDataList() = 0;
};