/*******************************************************
  源程序名称:QueryFund.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询资金类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
//#include "TradeDataBase.h"
#include "QueryFundIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)


class CFundItem : public CDataItem, public IFundItem
{
public:
	//币种类别
	EMoneyType GetMoneyType();
	void SetMoneyType(const EMoneyType mtValue);
	//当前余额
	double GetCurrentBalance();
	void SetCurrentBalance(const double Value);
	//可用金额
	double GetEnableBalance();
	void SetEnableBalance(const double Value);
	//资金资产
	double GetFundAsset();
	void SetFundAsset(const double value);
	//资产总值
	double GetAssetBalance();
	void SetAssetBalance(const double value);
	//证券市值
	double GetMarketValue();
	void SetMarketValue(const double value);
	//融资负债
	double GetFinanceDebit();
};

class CFundList : public CDataList, public IFundList
{
public:
	IFundItem * GetItems(const int i);
	IFundItem * NewItem();
protected:
private:
};

class CReqQueryFund : public CReqQueryBase, virtual public IReqQueryFund
{
public:   
	//币种
	void SetMoneyType(const EMoneyType mtValue);
    IFundList* GetDataList();
protected:
	IFundList* NewDataList();
};