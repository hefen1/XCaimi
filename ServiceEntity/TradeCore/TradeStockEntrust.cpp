/*******************************************************
  源程序名称:TradeStockEntrust.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券委托实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeStockEntrust.h"

void CReqStockEntrust::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqStockEntrust::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqStockEntrust::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqStockEntrust::SetEntrustBs(const EBSDirection Value)
{
	CString sTmp = "";
	sTmp.Format("%d", int(Value));
	CTrading::SetRequestARData("entrust_bs", sTmp);
}

void CReqStockEntrust::SetEntrustAmount(const int Value)
{
	CString sTmp = "";
	sTmp.Format("%d", Value);
	CTrading::SetRequestARData("entrust_amount", sTmp);
	CTrading::SetRequestARData("amount", sTmp);
}

void CReqStockEntrust::SetEntrustPrice(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("entrust_price", sTmp);
}

void CReqStockEntrust::SetEntrustProp(const CString sValue)
{
	CTrading::SetRequestARData("entrust_prop", sValue);
}

void CReqStockEntrust::SetOppoSeatNo(const CString sValue)
{
	CTrading::SetRequestARData("oppo_seatno", sValue);
}

void CReqStockEntrust::SetConferNo(const CString sValue)
{
	CTrading::SetRequestARData("confer_no", sValue);
}

void CReqStockEntrust::SetTransCode(const CString sValue)
{
	CTrading::SetRequestARData("trans_code", sValue);
}

void CReqStockEntrust::SetEntrustType(const CString sValue)
{
	CTrading::SetRequestARData("entrust_type", sValue);
}

void CReqStockEntrust::SetComponentCode(const CString sValue)
{
	CTrading::SetRequestARData("component_code", sValue);
}