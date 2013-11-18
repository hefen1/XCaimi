/*******************************************************
  源程序名称:QueryStockAccount.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询股东账号类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockAccountIntf.h"
#pragma warning (disable:4250)


class CStockAccountItem : public CDataItem, public IStockAccountItem
{
public:	
	//市场
	CString GetExchangeType();
	//股东账号
	CString GetStockAccount();
	//主股东账号标识
	bool GetMainFlag();
	//席位号
	CString GetSeatNo();
};

class CStockAccountList : public CDataList, public IStockAccountList
{
public:
	IStockAccountItem * GetItems(const int i);
	IStockAccountItem * NewItem();
protected:
private:
};


class CReqQueryStockAccount : public CTrading, public IReqQueryStockAccount
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	IStockAccountList* GetDataList();
protected:
	IStockAccountList* NewDataList();
};