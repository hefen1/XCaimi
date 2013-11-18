/*******************************************************
  源程序名称:QueryStockEntrust.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询委托接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "QueryBase.h"
#include "TradeDataBase.h"
#include "QueryStockEntrustIntf.h"
#pragma warning (disable:4250)


class CStockEntrustItem : public CDataItem, public IStockEntrustItem
//class CStockEntrustItem : public IStockEntrustItem
{
public:
	CString GetStockCode();
	CString GetExchangeType();
	CString GetStockAccount();
    EBSDirection GetEntrustBS();
	CString GetPositionStr();
	CString GetEntrustStatus();
protected:

private:
};

class CStockEntrustList : public CDataList, public IStockEntrustList
//class CStockEntrustList : public IStockEntrustList
{
public:
	IStockEntrustItem * GetItems(const int i);
	IStockEntrustItem * NewItem();
protected:
private:
};


class CReqQueryStockEntrust : public CReqQueryBase, public IReqQueryStockEntrust
{
public:	
	//委托编号
	void SetEntrustNo(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IStockEntrustList* GetDataList();
protected:
	IStockEntrustList* NewDataList();
};