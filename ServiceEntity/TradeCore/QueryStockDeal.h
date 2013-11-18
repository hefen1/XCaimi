/*******************************************************
  源程序名称:QueryStockDeal.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询成交类
  			1、请求类定义
  			2、返回类口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "QueryBase.h"
#include "TradeDataBase.h"
#include "QueryStockDealIntf.h"
#pragma warning (disable:4250)


class CStockDealItem : public CDataItem, public IStockDealItem
{
public:
	//证券代码
	CString GetStockCode();
	//市场
	CString GetExchangeType();
	//股东账号
	CString GetStockAccount();
	//买卖方向
	EBSDirection GetEntrustBS();
	//定位串
	CString GetPositionStr();
};

class CStockDealList : public CDataList, public IStockDealList
{
public:
	IStockDealItem * GetItems(const int i);
	IStockDealItem * NewItem();
protected:
private:	
};


class CReqQueryStockDeal : public CReqQueryBase, public IReqQueryStockDeal
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
	IStockDealList* GetDataList();
protected:
	IStockDealList* NewDataList();

};