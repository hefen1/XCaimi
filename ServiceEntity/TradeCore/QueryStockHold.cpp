/*******************************************************
  Դ��������:QueryStockHold.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ֲ�ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockHold.h"


CString CStockHoldItem::GetStockCode()
{
	return CDataItem::GetStrByName("stock_code");
}

CString CStockHoldItem::GetExchangeType()
{
	return CDataItem::GetStrByName("exchange_type");
}

CString CStockHoldItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}

long CStockHoldItem::GetCurrentAmount()
{
	return CDataItem::GetLongByName("current_amount");
}

long CStockHoldItem::GetEnableAmount()
{
	return CDataItem::GetLongByName("enable_amount");
}


double CStockHoldItem::GetLastPrice()
{
	return CDataItem::GetDoubleByName("last_price");
}

void CStockHoldItem::SetLastPrice(const double Value)
{
	CString sTmp;
	sTmp.Format("%d", Value);
	CDataItem::SetData("last_price", sTmp);
}

double CStockHoldItem::GetCostPrice()
{
	return CDataItem::GetDoubleByName("cost_price");
}


double CStockHoldItem::GetMarketBalance()
{
	return CDataItem::GetDoubleByName("market_balance");
}

void CStockHoldItem::SetMarketBalance(const double Value)
{
	CString sTmp;
	sTmp.Format("%d", Value);
	CDataItem::SetData("market_balance", sTmp);
}

double CStockHoldItem::GetIncomeBalance()
{
	return CDataItem::GetDoubleByName("income_balance");
}

void CStockHoldItem::SetIncomeBalance(const double Value)
{
	CString sTmp;
	sTmp.Format("%d", Value);
	CDataItem::SetData("income_balance", sTmp);
}

double CStockHoldItem::GetIncomeRatio()
{
	return CDataItem::GetDoubleByName("income_ratio");
}

void CStockHoldItem::SetIncomeRatio(const double Value)
{
	CString sTmp;
	sTmp.Format("%d", Value);
	CDataItem::SetData("income_ratio", sTmp);
}

CString CStockHoldItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

CString CStockHoldItem::GetSeat()
{
	return CDataItem::GetStrByName("seat");
}

//////////////////////////////////////////////////////////////////////////

IStockHoldItem* CStockHoldList::GetItems(const int i)
{

	IStockHoldItem* pItem = NULL;
    pItem = dynamic_cast<IStockHoldItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockHoldItem* CStockHoldList::NewItem()
{
	CStockHoldItem* pItem = new CStockHoldItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////

void CReqQueryStockHold::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqQueryStockHold::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

void CReqQueryStockHold::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqQueryStockHold::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IStockHoldList* CReqQueryStockHold::GetDataList()
{
	return dynamic_cast<IStockHoldList*>(m_DataList);
}

IStockHoldList* CReqQueryStockHold::NewDataList()
{
	CStockHoldList* pStockHoldList = new CStockHoldList;
	return pStockHoldList;
}