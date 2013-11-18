/*******************************************************
  Դ��������:QueryStockEntrust.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯί��ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockEntrust.h"


CString CStockEntrustItem::GetStockCode()
{
	return CDataItem::GetStrByName("stock_code");
}

CString CStockEntrustItem::GetExchangeType()
{
	return CDataItem::GetStrByName("exchange_type");
}

CString CStockEntrustItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}

EBSDirection CStockEntrustItem::GetEntrustBS()
{
	CString sTmp = CDataItem::GetStrByName("entrust_bs");
	EBSDirection bsTmp = EBSDirection(CDataItem::GetIntByName("entrust_bs"));
	return bsTmp;
}

CString CStockEntrustItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

CString CStockEntrustItem::GetEntrustStatus()
{
	return CDataItem::GetStrByName("entrust_status");
}

//////////////////////////////////////////////////////////////////////////
IStockEntrustItem* CStockEntrustList::GetItems(const int i)
{

	IStockEntrustItem* pItem = NULL;
    pItem = dynamic_cast<IStockEntrustItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockEntrustItem* CStockEntrustList::NewItem()
{
	CStockEntrustItem* pItem = new CStockEntrustItem;
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryStockEntrust::SetEntrustNo(const CString sValue)
{
	CTrading::SetRequestARData("entrust_no", sValue);
}

void CReqQueryStockEntrust::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}
void CReqQueryStockEntrust::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

void CReqQueryStockEntrust::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqQueryStockEntrust::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IStockEntrustList* CReqQueryStockEntrust::GetDataList()
{
	return dynamic_cast<IStockEntrustList*>(m_DataList);
}


IStockEntrustList* CReqQueryStockEntrust::NewDataList()
{
	CStockEntrustList* pStockEntrustList = new CStockEntrustList;
	return pStockEntrustList;
}
