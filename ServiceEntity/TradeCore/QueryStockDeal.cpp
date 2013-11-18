/*******************************************************
  Դ��������:QueryStockEntrust.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɽ�ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockDeal.h"

CString CStockDealItem::GetStockCode()
{
	return CDataItem::GetStrByName("stock_code");
}

CString CStockDealItem::GetExchangeType()
{
	return CDataItem::GetStrByName("exchange_type");
}

CString CStockDealItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}

EBSDirection CStockDealItem::GetEntrustBS()
{
	EBSDirection bsTmp = EBSDirection(CDataItem::GetIntByName("entrust_bs"));
	return bsTmp;
}

CString CStockDealItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////

IStockDealItem* CStockDealList::GetItems(const int i)
{

	IStockDealItem* pItem = NULL;
    pItem = dynamic_cast<IStockDealItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockDealItem* CStockDealList::NewItem()
{
	CStockDealItem* pItem = new CStockDealItem;
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryStockDeal::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqQueryStockDeal::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

void CReqQueryStockDeal::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqQueryStockDeal::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IStockDealList* CReqQueryStockDeal::GetDataList()
{
	return dynamic_cast<IStockDealList*>(m_DataList);
}


IStockDealList* CReqQueryStockDeal::NewDataList()
{
	CStockDealList* pStockDealList = new CStockDealList;
	return pStockDealList;
}
