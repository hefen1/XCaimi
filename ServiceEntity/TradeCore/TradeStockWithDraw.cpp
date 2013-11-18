/*******************************************************
  源程序名称:TradeStockWithDraw.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券撤单实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeStockWithDraw.h"

void CReqStockWithDraw::SetExchangeType(const CString sValue)
{
	CTrading::SetRequestARData("exchange_type",sValue);
}

void CReqStockWithDraw::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account", sValue);
}

void CReqStockWithDraw::SetEntrustNo(const CString sValue)
{
	CTrading::SetRequestARData("entrust_no", sValue);
}

void CReqStockWithDraw::SetStockCode(const CString sValue)
{
	CTrading::SetRequestARData("stock_code", sValue);
}