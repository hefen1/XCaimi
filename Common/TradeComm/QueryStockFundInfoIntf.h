/*******************************************************
  源程序名称:QueryStockFundInfoIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金盘后业务信息接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20110328
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IStockFundInfoItem : virtual public IDataItem
{
	//证券代码
	virtual CString GetStockCode() = 0;
	//市场
	virtual CString GetExchangeType() = 0;
	//股东账号
	virtual CString GetStockAccount() = 0;
	//基金状态
	virtual CString GetFundStatus() = 0;
	//转换状态
	virtual CString GetTransStatus() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockFundInfoList : virtual public IDataList
{
	virtual IStockFundInfoItem * GetItems(const int i) = 0; 
	virtual IStockFundInfoItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////\

struct IReqQueryStockFundInfo : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	virtual IStockFundInfoList* GetDataList() = 0;
};