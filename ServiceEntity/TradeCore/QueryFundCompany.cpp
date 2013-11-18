/*******************************************************
  Դ��������:QueryFundCompany.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����˾ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#include "stdafx.h"
#include "QueryFundCompany.h"

CString CFundCompanyItem::GetFundCompany()
{
	return CDataItem::GetStrByName("fund_company");
}


CString CFundCompanyItem::GetCompanyName()
{
	return CDataItem::GetStrByName("company_name");
}

CString CFundCompanyItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////
IFundCompanyItem* CFundCompanyList::GetItems(const int i)
{

	IFundCompanyItem* pItem = NULL;
    pItem = dynamic_cast<IFundCompanyItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundCompanyItem* CFundCompanyList::NewItem()
{
	CFundCompanyItem* pItem = new CFundCompanyItem;
	return pItem;
}

IFundCompanyItem* CFundCompanyList::GetByFundCompany(CString sFundCompany)
{
	IFundCompanyItem* pItem = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		pItem = dynamic_cast<IFundCompanyItem*>(CDataList::GetItems(i));
		if (pItem->GetFundCompany() == sFundCompany)
		{
			break;
		}
		pItem = NULL;
	}	
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryFundCompany::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company", sValue);
}

void CReqQueryFundCompany::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IFundCompanyList* CReqQueryFundCompany::GetDataList()
{
	return dynamic_cast<IFundCompanyList*>(m_DataList);
}

IFundCompanyList* CReqQueryFundCompany::NewDataList()
{
	CFundCompanyList* pList = new CFundCompanyList;
	return pList;
}