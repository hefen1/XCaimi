/*******************************************************
  源程序名称:QueryFund.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询资金实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryFund.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EMoneyType CFundItem::GetMoneyType()
{
	EMoneyType mtTmp = EMoneyType(CDataItem::GetIntByName("money_type"));
	return mtTmp;
}

void CFundItem::SetMoneyType(const EMoneyType mtValue)
{
	CString sValue = "";
	sValue.Format("%d", int(mtValue));
	CDataItem::SetData("money_type",sValue);
}

double CFundItem::GetCurrentBalance()
{
	return CDataItem::GetDoubleByName("current_balance");
}

void CFundItem::SetCurrentBalance(const double Value)
{
	CString sTmp;
	sTmp.Format("%.4f", Value);
	CDataItem::SetData("current_balance", sTmp);
}

double CFundItem::GetEnableBalance()
{
	return CDataItem::GetDoubleByName("enable_balance");
}

void CFundItem::SetEnableBalance(const double Value)
{
	CString sTmp;
	sTmp.Format("%.4f", Value);
	CDataItem::SetData("enable_balance", sTmp);
}

double CFundItem::GetFundAsset()
{
	return CDataItem::GetDoubleByName("fund_asset");
}

void CFundItem::SetFundAsset(const double Value)
{
	CString sTmp;
	sTmp.Format("%.4f", Value);
	CDataItem::SetData("fund_asset", sTmp);
}

double CFundItem::GetAssetBalance()
{
	return CDataItem::GetDoubleByName("asset_balance");
}

void CFundItem::SetAssetBalance(const double Value)
{
	CString sTmp;
	sTmp.Format("%.4f", Value);
	CDataItem::SetData("asset_balance", sTmp);
}

double CFundItem::GetMarketValue()
{
	return CDataItem::GetDoubleByName("market_value");
}

void CFundItem::SetMarketValue(const double Value)
{
	CString sTmp;
	sTmp.Format("%.4f", Value);
	CDataItem::SetData("market_value", sTmp);
}

double CFundItem::GetFinanceDebit()
{
	return CDataItem::GetDoubleByName("finance_debit");
}

//////////////////////////////////////////////////////////////////////////

IFundItem* CFundList::GetItems(const int i)
{

	IFundItem* pItem = NULL;
    pItem = dynamic_cast<IFundItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundItem* CFundList::NewItem()
{
	IFundItem* pItem = new CFundItem;
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryFund::SetMoneyType(const EMoneyType mtValue)
{
	CString sValue = "";
	sValue.Format("%d", int(mtValue));
	CTrading::SetRequestARData("money_type",sValue);
}

IFundList* CReqQueryFund::GetDataList()
{
	return dynamic_cast<IFundList*>(m_DataList);
}

IFundList* CReqQueryFund::NewDataList()
{
	CFundList* pFundList = new CFundList;
	return pFundList;
}