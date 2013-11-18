/*******************************************************
  源程序名称:QueryFundStockAccount.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金账号类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundStockAccountIntf.h"
#pragma warning (disable:4250)


class CFundStockAccountItem : public CDataItem, public IFundStockAccountItem
{
public:	
	//基金公司
	CString GetFundCompany();
	//基金账号
	CString GetStockAccount();
	//转换账号
	CString GetTransAccount();
	//公司名称
	CString GetCompanyName();
	//状态
	CString GetHolderStatus();
	//开户日期
	CString GetOpenDate();
	//定位串
	CString GetPositionStr();
};

class CFundStockAccountList : public CDataList, public IFundStockAccountList
{
public:
	IFundStockAccountItem * GetItems(const int i);
	IFundStockAccountItem * NewItem();
	IFundStockAccountItem * GetByFundCompany(CString sFundCompany);
protected:
private:
};


class CReqQueryFundStockAccount : public CTrading, public IReqQueryFundStockAccount
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IFundStockAccountList* GetDataList();
protected:
	IFundStockAccountList* NewDataList();
};