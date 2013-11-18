/*******************************************************
  Դ��������:QueryFundEntrust.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������ί��ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#include "stdafx.h"
#include "QueryFundEntrust.h"

CString CFundEntrustItem::GetFundCompany()
{
	return CDataItem::GetStrByName("fund_company");
}

CString CFundEntrustItem::GetFundCode()
{
	return CDataItem::GetStrByName("fund_code");
}

CString CFundEntrustItem::GetEntrustNo()
{
	return CDataItem::GetStrByName("entrust_no");
}

CString CFundEntrustItem::GetEntrustStatus()
{
	return CDataItem::GetStrByName("entrust_status");
}

CString CFundEntrustItem::GetAllotDate()
{
	return CDataItem::GetStrByName("allotdate");
}

CString CFundEntrustItem::GetAllotNo()
{
	return CDataItem::GetStrByName("allotno");
}

CString CFundEntrustItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////
IFundEntrustItem* CFundEntrustList::GetItems(const int i)
{

	IFundEntrustItem* pItem = NULL;
    pItem = dynamic_cast<IFundEntrustItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundEntrustItem* CFundEntrustList::NewItem()
{
	CFundEntrustItem* pItem = new CFundEntrustItem;
	return pItem;
}



//////////////////////////////////////////////////////////////////////////
void CReqQueryFundEntrust::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company", sValue);
}

void CReqQueryFundEntrust::SetFundCode(const CString sValue)
{
	CTrading::SetRequestARData("fund_code", sValue);
}

void CReqQueryFundEntrust::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IFundEntrustList* CReqQueryFundEntrust::GetDataList()
{
	return dynamic_cast<IFundEntrustList*>(m_DataList);
}

IFundEntrustList* CReqQueryFundEntrust::NewDataList()
{
	CFundEntrustList* pList = new CFundEntrustList;
	return pList;
}