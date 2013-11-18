/*******************************************************
  源程序名称:QueryStockMaxSell.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询证券最大可卖类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockMaxSellIntf.h"
#pragma warning (disable:4250)

class CStockMaxSellItem : public CDataItem, public IStockMaxSellItem
{
public:	
	long GetEnableAmount();
	//可合并数量
	long GetMergeAmount();
	//可拆分数量
	long GetSplitAmount();
	//融券负债数量
	long GetShortsellDebitAmount();
};
//////////////////////////////////////////////////////////////////////////
class CStockMaxSellList : public CDataList, public IStockMaxSellList
{
public:
	IStockMaxSellItem * GetItems(const int i); 
	IStockMaxSellItem * NewItem();
protected:
private:	
};
//////////////////////////////////////////////////////////////////////////
class CReqQueryStockMaxSell : public CTrading, public IReqQueryStockMaxSell
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//委托属性
	void SetEntrustProp(const CString sValue);
	IStockMaxSellList* GetDataList();
protected:
	IStockMaxSellList* NewDataList();
};