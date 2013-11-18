/*******************************************************
  源程序名称:QueryFundInfo.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金信息实现
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20110217
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