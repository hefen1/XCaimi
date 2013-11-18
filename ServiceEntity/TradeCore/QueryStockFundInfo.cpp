/*******************************************************
  源程序名称:QueryStockFundInfo.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金盘后业务信息接口
  			1、请求实现
  			2、返回实现
  作    者:  shenglq
  开发日期:  20110328
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