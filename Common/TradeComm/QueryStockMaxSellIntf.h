/*******************************************************
  源程序名称:QueryStockMaxSellIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询证券最大可卖接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IStockMaxSellItem : virtual public IDataItem
{
	//可买数量
	virtual long GetEnableAmount() = 0;
	//可合并数量
	virtual long GetMergeAmount() = 0;
	//可拆分数量
	virtual long GetSplitAmount() = 0;
	//融券负债数量
	virtual long GetShortsellDebitAmount() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockMaxSellList : virtual public IDataList
{
	virtual IStockMaxSellItem * GetItems(const int i) = 0; 
	virtual IStockMaxSellItem * NewItem() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IReqQueryStockMaxSell : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//委托属性
	virtual void SetEntrustProp(const CString sValue) = 0;
	virtual IStockMaxSellList* GetDataList() = 0;
};