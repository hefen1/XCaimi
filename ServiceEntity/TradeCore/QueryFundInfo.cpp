/*******************************************************
  Դ��������:QueryFundInfo.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������Ϣʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#include "stdafx.h"
#include "QueryFundInfo.h"

CString CFundInfoItem::GetFundCompany()
{
	return CDataItem::GetStrByName("fund_company");
}

CString CFundInfoItem::GetFundCode()
{
	return CDataItem::GetStrByName("fund_code");
}


CString CFundInfoItem::GetFundName()
{
	return CDataItem::GetStrByName("fund_name");
}

CString CFundInfoItem::GetNav()
{
	return CDataItem::GetStrByName("nav");
}

CString CFundInfoItem::GetStatus()
{
	return CDataItem::GetStrByName("status");
}

CString CFundInfoItem::GetChargeType()
{
	return CDataItem::GetStrByName("charge_type");
}

CString CFundInfoItem::GetFundRisklevel()
{
	return CDataItem::GetStrByName("fund_risklevel");
}

CString CFundInfoItem::GetFundRisklevelName()
{
	return CDataItem::GetStrByName("fund_risklevel_name");
}

CString CFundInfoItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////
IFundInfoItem* CFundInfoList::GetItems(const int i)
{

	IFundInfoItem* pItem = NULL;
    pItem = dynamic_cast<IFundInfoItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundInfoItem* CFundInfoList::NewItem()
{
	CFundInfoItem* pItem = new CFundInfoItem;
	return pItem;
}

IFundInfoItem* CFundInfoList::GetByFundCode(CString sFundCode)
{
	IFundInfoItem* pItem = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		pItem = dynamic_cast<IFundInfoItem*>(CDataList::GetItems(i));
		if (pItem->GetFundCode() == sFundCode)
		{
			break;
		}
		pItem = NULL;
	}	
	return pItem;
}

IFundInfoList* CFundInfoList::NewDataList()
{
	return new CFundInfoList();
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryFundInfo::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company", sValue);
}

void CReqQueryFundInfo::SetFundCode(const CString sValue)
{
	CTrading::SetRequestARData("fund_code", sValue);
}

void CReqQueryFundInfo::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IFundInfoList* CReqQueryFundInfo::GetDataList()
{
	return dynamic_cast<IFundInfoList*>(m_DataList);
}

IFundInfoList* CReqQueryFundInfo::NewDataList()
{
	CFundInfoList* pList = new CFundInfoList;
	return pList;
}