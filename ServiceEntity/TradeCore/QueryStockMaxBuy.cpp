/*******************************************************
  源程序名称:QueryStockMaxBuy.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询最大可买实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockMaxBuy.h"

long CStockMaxBuyItem::GetEnableAmount()
{
	return CDataItem::GetLongByName("enable_amount");
}
//////////////////////////////////////////////////////////////////////////

IStockMaxBuyItem* CStockMaxBuyList::GetItems(const int i)
{

	IStockMaxBuyItem* pItem = NULL;
	pItem = dynamic_cast<IStockMaxBuyItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockMaxBuyItem* CStockMaxBuyList::NewItem()
{
	CStockMaxBuyItem* pItem = new CStockMaxBuyItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////
void CReqQueryStockMaxBuy::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqQueryStockMaxBuy::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqQueryStockMaxBuy::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqQueryStockMaxBuy::SetEntrustPrice(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("entrust_price", sTmp);
}

void CReqQueryStockMaxBuy::SetEntrustProp(const CString sValue)
{
	CTrading::SetRequestARData("entrust_prop", sValue);
}

void CReqQueryStockMaxBuy::SetEntrustType(const CString sValue)
{
	CTrading::SetRequestARData("entrust_type", sValue);
}

IStockMaxBuyList* CReqQueryStockMaxBuy::GetDataList()
{
	return dynamic_cast<IStockMaxBuyList*>(m_DataList);
}


IStockMaxBuyList* CReqQueryStockMaxBuy::NewDataList()
{
	CStockMaxBuyList* pCStockMaxBuyList = new CStockMaxBuyList;
	return pCStockMaxBuyList;
}