/*******************************************************
  源程序名称:QueryFundStockAccountIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金账号接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundStockAccountItem : virtual public IDataItem
{
	//基金公司
	virtual CString GetFundCompany() = 0;
	//基金账号
	virtual CString GetStockAccount() = 0;
	//转换账号
	virtual CString GetTransAccount() = 0;
	//公司名称
	virtual CString GetCompanyName() = 0;
	//状态
	virtual CString GetHolderStatus() = 0;
	//开户日期
	virtual CString GetOpenDate() = 0;
	//定位串
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundStockAccountList : virtual public IDataList
{
	virtual IFundStockAccountItem * GetItems(const int i) = 0; 
	virtual IFundStockAccountItem * NewItem() = 0;
	virtual IFundStockAccountItem * GetByFundCompany(CString sFundCompany) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundStockAccount : virtual public ITrading
{
	//基金公司
	virtual void SetFundCompany(const CString sValue) = 0;
	//定位串
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundStockAccountList* GetDataList() = 0;
};