/*******************************************************
  源程序名称:QueryStockAccountIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询股东账号接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IStockAccountItem : virtual public IDataItem
{
	//市场
	virtual CString GetExchangeType() = 0;
	//股东账号
	virtual CString GetStockAccount() = 0;
	//主股东账号标识
	virtual bool GetMainFlag() = 0;
	//席位号
	virtual CString GetSeatNo() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockAccountList : virtual public IDataList
{
	virtual IStockAccountItem * GetItems(const int i) = 0; 
	virtual IStockAccountItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////\

struct IReqQueryStockAccount : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	virtual IStockAccountList* GetDataList() = 0;
};