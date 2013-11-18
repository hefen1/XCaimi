/*******************************************************
  Դ��������:QueryStockFundInfo.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�����̺�ҵ����Ϣ�ӿ�
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20110328
*********************************************************/
#include "stdafx.h"
#include "QueryStockFundInfo.h"


CString CStockFundInfoItem::GetStockCode()
{
	return CDataItem::GetStrByName("stock_code");
}

CString CStockFundInfoItem::GetExchangeType()
{
	return CDataItem::GetStrByName("exchange_type");
}

CString CStockFundInfoItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}

CString CStockFundInfoItem::GetFundStatus()
{
	return CDataItem::GetStrByName("fund_status");
}

CString CStockFundInfoItem::GetTransStatus()
{
	return CDataItem::GetStrByName("trans_status");
}

//////////////////////////////////////////////////////////////////////////

IStockFundInfoItem* CStockFundInfoList::GetItems(const int i)
{

	IStockFundInfoItem* pItem = NULL;
    pItem = dynamic_cast<IStockFundInfoItem*>(CDataList::GetItems(i));
	return pItem;
}

IStockFundInfoItem* CStockFundInfoList::NewItem()
{
	CStockFundInfoItem* pItem = new CStockFundInfoItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////

void CReqQueryStockFundInfo::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqQueryStockFundInfo::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type", sValue);
}

IStockFundInfoList* CReqQueryStockFundInfo::GetDataList()
{
	return dynamic_cast<IStockFundInfoList*>(m_DataList);
}

IStockFundInfoList* CReqQueryStockFundInfo::NewDataList()
{
	CStockFundInfoList* pStockHoldList = new CStockFundInfoList;
	return pStockHoldList;
}