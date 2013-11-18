/*******************************************************
  源程序名称:QueryStockMaxBuyIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询证券最大可买接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IStockMaxBuyItem : virtual public IDataItem
{
	//可买数量
	virtual long GetEnableAmount() = 0;
	//virtual void SetEnableAmount(const int Value) = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockMaxBuyList : virtual public IDataList
{
	virtual IStockMaxBuyItem * GetItems(const int i) = 0; 
	virtual IStockMaxBuyItem * NewItem() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IReqQueryStockMaxBuy : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//委托价格
	virtual void SetEntrustPrice(const double Value) = 0;
	//委托属性
	virtual void SetEntrustProp(const CString sValue) = 0;
	//委托类型
	virtual void SetEntrustType(const CString sValue) = 0;
	virtual IStockMaxBuyList* GetDataList() = 0;
};