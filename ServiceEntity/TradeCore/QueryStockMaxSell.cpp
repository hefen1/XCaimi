/*******************************************************
  源程序名称:QueryStockMaxSell.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询最大可卖实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockMaxSell.h"

long CStockMaxSellItem::GetEnableAmount()
{
	return CDataItem::GetLongByName("enable_amount");
}

long CStockMaxSellItem::GetMergeAmount()
{
	return CDataItem::GetLongByName("merge_amount");
}

long CStockMaxSellItem::GetSplitAmount()
{
	return CDataItem::GetLongByName("split_amount");
}

long CStockMaxSellItem::GetShortsellDebitAmount()
{
	return CDataItem::GetLongByName("shortsell_debit_amount");
}

//////////////////////////////////////////////////////////////////////////

IStockMaxSellItem* CStockMaxSellList::GetItems(const int i)
{

	IStockMaxSellItem* pItem = NULL;
	pItem = dynamic_cast<IStockMaxSellItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockMaxSellItem* CStockMaxSellList::NewItem()
{
	CStockMaxSellItem* pItem = new CStockMaxSellItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////
void CReqQueryStockMaxSell::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqQueryStockMaxSell::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqQueryStockMaxSell::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqQueryStockMaxSell::SetEntrustProp(const CString sValue)
{
	CTrading::SetRequestARData("entrust_prop", sValue);
}

IStockMaxSellList* CReqQueryStockMaxSell::GetDataList()
{
	return dynamic_cast<IStockMaxSellList*>(m_DataList);
}


IStockMaxSellList* CReqQueryStockMaxSell::NewDataList()
{
	CStockMaxSellList* pCStockMaxSellList = new CStockMaxSellList;
	return pCStockMaxSellList;
}