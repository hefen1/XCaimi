/*******************************************************
  源程序名称:QueryFundIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询资金接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IFundItem : virtual public IDataItem
{
	//币种类别
	virtual EMoneyType GetMoneyType() = 0;
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//当前余额
	virtual double GetCurrentBalance() = 0;
	virtual void SetCurrentBalance(const double Value) = 0;
	//可用金额
	virtual double GetEnableBalance() = 0;
	virtual void SetEnableBalance(const double Value) = 0;
	//资金资产
	virtual double GetFundAsset() = 0;
	virtual void SetFundAsset(const double value) = 0;
	//资产总值
	virtual double GetAssetBalance() = 0;
	virtual void SetAssetBalance(const double value) = 0;
	//证券市值
	virtual double GetMarketValue() = 0;
	virtual void SetMarketValue(const double value) = 0;
	//融资负债
	virtual double GetFinanceDebit() = 0;
};

struct IFundList : virtual public IDataList
{
	virtual IFundItem * GetItems(const int i) = 0; 
	virtual IFundItem * NewItem() = 0;
};

struct IReqQueryFund : virtual public IReqQueryBase
{
	//币种
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	virtual IFundList* GetDataList() = 0;
};