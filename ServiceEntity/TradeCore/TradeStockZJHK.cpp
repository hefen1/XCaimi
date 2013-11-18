/*******************************************************
  源程序名称:TradeStockZJHK.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  直接还款类实现
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "TradeStockZJHK.h"

void CReqStockZJHK::SetMoneyType(const CString sValue)
{
	CTrading::SetRequestARData("money_type",sValue);
}

void CReqStockZJHK::SetOccurBalance(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("occur_balance", sTmp);
}