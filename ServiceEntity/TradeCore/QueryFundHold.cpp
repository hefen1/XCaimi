/*******************************************************
  Դ��������:QueryFundHold.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����ݶ�ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#include "stdafx.h"
#include "QueryFundHold.h"

CString CFundHoldItem::GetFundCompany()
{
	return CDataItem::GetStrByName("fund_company");
}

CString CFundHoldItem::GetFundCode()
{
	return CDataItem::GetStrByName("fund_code");
}


CString CFundHoldItem::GetFundName()
{
	return CDataItem::GetStrByName("fund_name");
}

CString CFundHoldItem::GetNav()
{
	return CDataItem::GetStrByName("nav");
}

CString CFundHoldItem::GetStatus()
{
	return CDataItem::GetStrByName("status");
}

CString CFundHoldItem::GetChargeType()
{
	return CDataItem::GetStrByName("charge_type");
}

CString CFundHoldItem::GetAllotDate()
{
	return CDataItem::GetStrByName("allotdate");
}

CString CFundHoldItem::GetAllotNo()
{
	return CDataItem::GetStrByName("allotno");
}

CString CFundHoldItem::GetEnableShares()
{
	CString sValue = CDataItem::GetStrByName("enable_shares");
	if (sValue.IsEmpty())
	{
		sValue = CDataItem::GetStrByName("enable_amount");
	}
	return sValue;
}

CString CFundHoldItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////
IFundHoldItem* CFundHoldList::GetItems(const int i)
{

	IFundHoldItem* pItem = NULL;
    pItem = dynamic_cast<IFundHoldItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundHoldItem* CFundHoldList::NewItem()
{
	CFundHoldItem* pItem = new CFundHoldItem;
	return pItem;
}

IFundHoldItem* CFundHoldList::GetByFundCode(CString sFundCode)
{
	IFundHoldItem* pItem = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		pItem = dynamic_cast<IFundHoldItem*>(CDataList::GetItems(i));
		if (pItem->GetFundCode() == sFundCode)
		{
			break;
		}
		pItem = NULL;
	}	
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryFundHold::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company", sValue);
}

void CReqQueryFundHold::SetFundCode(const CString sValue)
{
	CTrading::SetRequestARData("fund_code", sValue);
}

void CReqQueryFundHold::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IFundHoldList* CReqQueryFundHold::GetDataList()
{
	return dynamic_cast<IFundHoldList*>(m_DataList);
}

IFundHoldList* CReqQueryFundHold::NewDataList()
{
	CFundHoldList* pList = new CFundHoldList;
	return pList;
}