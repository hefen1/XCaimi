/*******************************************************
  源程序名称:QueryStockHold.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询持仓类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockHoldIntf.h"
#pragma warning (disable:4250)


class CStockHoldItem : public CDataItem, public IStockHoldItem
{
public:	
	//委托编号
    CString GetStockCode();
	//市场
	CString GetExchangeType();
	//股东账号
	CString GetStockAccount();
	//股票余额
	long GetCurrentAmount();
	//可用数量
	long GetEnableAmount();
	//最新价
	double GetLastPrice();
	void SetLastPrice(const double Value);
	//成本价
	double GetCostPrice();
	//市值
	double GetMarketBalance();
	void SetMarketBalance(const double Value);
	//买卖盈亏
	double GetIncomeBalance();
	void SetIncomeBalance(const double Value);
	//盈亏比例
	double GetIncomeRatio();
	void SetIncomeRatio(const double Value);
	//定位串
	CString GetPositionStr();
	//席位
	CString GetSeat();
};

class CStockHoldList : public CDataList, public IStockHoldList
{
public:
	IStockHoldItem * GetItems(const int i); 
	IStockHoldItem * NewItem();
protected:
private:	
};



class CReqQueryStockHold : public CTrading, public IReqQueryStockHold
{
public:	
	//证券代码
	void SetStockCode(const CString sValue);
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IStockHoldList* GetDataList();
protected:
	IStockHoldList* NewDataList();
};