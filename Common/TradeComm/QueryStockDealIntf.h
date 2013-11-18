/*******************************************************
  源程序名称:QueryStockDealIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询成交接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
//#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"
#include "QueryBaseIntf.h"

struct IStockDealItem : virtual public IDataItem
{
	//证券代码
	virtual CString GetStockCode() = 0;
	//市场
	virtual CString GetExchangeType() = 0;
	//股东账号
	virtual CString GetStockAccount() = 0;
	//买卖方向
	virtual EBSDirection GetEntrustBS() = 0;
	//定位串
	virtual CString GetPositionStr() = 0;
};

struct IStockDealList : virtual public IDataList
{
	virtual IStockDealItem * GetItems(const int i) = 0;
	virtual IStockDealItem * NewItem() = 0;
};

struct IReqQueryStockDeal : virtual public IReqQueryBase
{
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//定位串
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IStockDealList* GetDataList() = 0;
};