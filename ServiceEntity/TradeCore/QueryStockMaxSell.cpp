/*******************************************************
  Դ��������:QueryStockMaxSell.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
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