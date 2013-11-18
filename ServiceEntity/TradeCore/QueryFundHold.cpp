/*******************************************************
  源程序名称:QueryFundHold.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金份额实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20110222
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