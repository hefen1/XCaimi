/*******************************************************
  源程序名称:QueryFundStockAccount.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金账号实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#include "stdafx.h"
#include "QueryFundStockAccount.h"

CString CFundStockAccountItem::GetFundCompany()
{
	return CDataItem::GetStrByName("fund_company");
}

CString CFundStockAccountItem::GetStockAccount()
{
	return CDataItem::GetStrByName("stock_account");
}


CString CFundStockAccountItem::GetTransAccount()
{
	return CDataItem::GetStrByName("trans_account");
}

CString CFundStockAccountItem::GetCompanyName()
{
	return CDataItem::GetStrByName("company_name");
}

CString CFundStockAccountItem::GetHolderStatus()
{
	return CDataItem::GetStrByName("holder_status");
}

CString CFundStockAccountItem::GetOpenDate()
{
	return CDataItem::GetStrByName("open_date");
}

CString CFundStockAccountItem::GetPositionStr()
{
	return CDataItem::GetStrByName("position_str");
}

//////////////////////////////////////////////////////////////////////////
IFundStockAccountItem* CFundStockAccountList::GetItems(const int i)
{

	IFundStockAccountItem* pItem = NULL;
    pItem = dynamic_cast<IFundStockAccountItem*>(CDataList::GetItems(i));
	return pItem;
}

IFundStockAccountItem* CFundStockAccountList::NewItem()
{
	CFundStockAccountItem* pItem = new CFundStockAccountItem;
	return pItem;
}

IFundStockAccountItem* CFundStockAccountList::GetByFundCompany(CString sFundCompany)
{
	IFundStockAccountItem* pItem = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		pItem = dynamic_cast<IFundStockAccountItem*>(CDataList::GetItems(i));
		if (pItem->GetFundCompany() == sFundCompany)
		{
			break;
		}
		pItem = NULL;
	}	
	return pItem;
}

//////////////////////////////////////////////////////////////////////////
void CReqQueryFundStockAccount::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company", sValue);
}

void CReqQueryFundStockAccount::SetPositionStr(const CString sValue)
{
	CTrading::SetRequestARData("position_str", sValue);
}

IFundStockAccountList* CReqQueryFundStockAccount::GetDataList()
{
	return dynamic_cast<IFundStockAccountList*>(m_DataList);
}

IFundStockAccountList* CReqQueryFundStockAccount::NewDataList()
{
	CFundStockAccountList* pList = new CFundStockAccountList;
	return pList;
}