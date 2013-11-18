/*******************************************************
  源程序名称:QueryStockMaxBuy.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询证券最大可买类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockMaxBuyIntf.h"
#pragma warning (disable:4250)

class CStockMaxBuyItem : public CDataItem, public IStockMaxBuyItem
{
public:	
	long GetEnableAmount();
};
//////////////////////////////////////////////////////////////////////////
class CStockMaxBuyList : public CDataList, public IStockMaxBuyList
{
public:
	IStockMaxBuyItem * GetItems(const int i); 
	IStockMaxBuyItem * NewItem();
protected:
private:	
};
//////////////////////////////////////////////////////////////////////////
class CReqQueryStockMaxBuy : public CTrading, public IReqQueryStockMaxBuy
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//委托价格
	void SetEntrustPrice(const double Value);
	//委托属性
	void SetEntrustProp(const CString sValue);
	//委托类型
	void SetEntrustType(const CString sValue);
	IStockMaxBuyList* GetDataList();
protected:
	IStockMaxBuyList* NewDataList();
};