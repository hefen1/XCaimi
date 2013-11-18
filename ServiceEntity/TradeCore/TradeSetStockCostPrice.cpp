/*******************************************************
  源程序名称:TradeStockEntrust.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  重置成本价实现
  作    者:  shenglq
  开发日期:  20110317
*********************************************************/
#include "stdafx.h"
#include "TradeSetStockCostPrice.h"

void CReqSetStockCostPrice::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqSetStockCostPrice::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqSetStockCostPrice::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqSetStockCostPrice::SetCostPrice(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.3f", Value);
	CTrading::SetRequestARData("cost_price", sTmp);
}

void CReqSetStockCostPrice::SetSeat(const CString sValue)
{
	CTrading::SetRequestARData("seat", sValue);
}

void CReqSetStockCostPrice::SetEntrustBalance(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("entrust_balance", sTmp);
}

void CReqSetStockCostPrice::SetRemark(const CString sValue)
{
	CTrading::SetRequestARData("remark", sValue);
}