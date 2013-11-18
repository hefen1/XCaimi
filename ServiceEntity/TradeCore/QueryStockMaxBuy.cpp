/*******************************************************
  Դ��������:QueryStockMaxBuy.cpp
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