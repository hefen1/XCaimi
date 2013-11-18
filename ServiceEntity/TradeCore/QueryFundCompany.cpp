/*******************************************************
  源程序名称:QueryFundCompany.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金公司实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20110217
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