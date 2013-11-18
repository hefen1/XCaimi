/*******************************************************
  源程序名称:TradeStockZJHQ.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券直接还券实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeStockZJHQ.h"

void CReqStockZJHQ::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqStockZJHQ::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqStockZJHQ::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}

void CReqStockZJHQ::SetEntrustAmount(const int Value)
{
	CString sTmp = "";
	sTmp.Format("%d", Value);
	CTrading::SetRequestARData("occur_amount", sTmp);
}