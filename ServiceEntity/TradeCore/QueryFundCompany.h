/*******************************************************
  源程序名称:QueryFundCompany.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金公司类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundCompanyIntf.h"
#pragma warning (disable:4250)


class CFundCompanyItem : public CDataItem, public IFundCompanyItem
{
public:	
	//基金公司
	CString GetFundCompany();
	//公司名称
	CString GetCompanyName();
	//定位串
	CString GetPositionStr();
};

class CFundCompanyList : public CDataList, public IFundCompanyList
{
public:
	IFundCompanyItem * GetItems(const int i);
	IFundCompanyItem * NewItem();
	IFundCompanyItem * GetByFundCompany(CString sFundCompany);
protected:
private:
};


class CReqQueryFundCompany : public CTrading, public IReqQueryFundCompany
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IFundCompanyList* GetDataList();
protected:
	IFundCompanyList* NewDataList();
};