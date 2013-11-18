/*******************************************************
  Դ��������:QueryStockAccount.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɶ��˺�ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryStockAccount.h"

CString CStockAccountItem::GetExchangeType()
{
	return CDataItem::GetStrByName("exchange_type");
}

CString CStockAccountItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}

bool CStockAccountItem::GetMainFlag()
{
	CString sTmp = GetStrByName("main_flag");
	CString sTmp1 = "1";	
	return sTmp == sTmp1;
}

CString CStockAccountItem::GetSeatNo()
{
	return CDataItem::GetStrByName("seat_no");
}

//////////////////////////////////////////////////////////////////////////

IStockAccountItem* CStockAccountList::GetItems(const int i)
{

	IStockAccountItem* pItem = NULL;
    pItem = dynamic_cast<IStockAccountItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockAccountItem* CStockAccountList::NewItem()
{
	CStockAccountItem* pItem = new CStockAccountItem;
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryStockAccount::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

void CReqQueryStockAccount::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

IStockAccountList* CReqQueryStockAccount::GetDataList()
{
	return dynamic_cast<IStockAccountList*>(m_DataList);
}

IStockAccountList* CReqQueryStockAccount::NewDataList()
{
	CStockAccountList* pStockAccountList = new CStockAccountList;
	return pStockAccountList;
}