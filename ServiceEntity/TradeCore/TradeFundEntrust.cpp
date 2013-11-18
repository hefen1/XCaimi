/*******************************************************
  源程序名称:TradeFundEntrust.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  基金委托实现
  作    者:  shenglq
  开发日期:  201102016
*********************************************************/
#include "stdafx.h"
#include "TradeFundEntrust.h"

void CReqFundEntrust::SetFundCompany(const CString sValue)
{
	CTrading::SetRequestARData("fund_company",sValue);
}

void CReqFundEntrust::SetFundCode(const CString sValue)
{
	CTrading::SetRequestARData("fund_code", sValue);
}

void CReqFundEntrust::SetAmount(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("amount", sTmp);
}

void CReqFundEntrust::SetBalance(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("balance", sTmp);
}

void CReqFundEntrust::SetChargeType(const CString sValue)
{
	CTrading::SetRequestARData("charge_type", sValue);
}

void CReqFundEntrust::SetExceedFlag(const CString sValue)
{
	CTrading::SetRequestARData("exceedflag", sValue);
}

void CReqFundEntrust::SetAllotDate(const CString sValue)
{
	CTrading::SetRequestARData("allotdate", sValue);
}

void CReqFundEntrust::SetAllotNo(const CString sValue)
{
	CTrading::SetRequestARData("allotno", sValue);
}

void CReqFundEntrust::SetEntrustNo(const CString sValue)
{
	CTrading::SetRequestARData("entrust_no", sValue);
}

void CReqFundEntrust::SetDividendMethod(const CString sValue)
{
	CTrading::SetRequestARData("dividendmethod", sValue);
	CTrading::SetRequestARData("dividend_flag", sValue);
}

void CReqFundEntrust::SetStockAccount(const CString sValue)
{
	CTrading::SetRequestARData("stock_account",sValue);
}

void CReqFundEntrust::SetTransCode(const CString sValue)
{
	CTrading::SetRequestARData("trans_code", sValue);
}

void CReqFundEntrust::SetTransAccount(const CString sValue)
{
	CTrading::SetRequestARData("trans_account", sValue);
}

void CReqFundEntrust::SetTransAmount(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("trans_amount", sTmp);
}

void CReqFundEntrust::SetClientRiskLevel(const CString sValue)
{
	CTrading::SetRequestARData("client_risklevel", sValue);
}

void CReqFundEntrust::SetHopeDate(const CString sValue)
{
	CTrading::SetRequestARData("hopedate", sValue);
}

void CReqFundEntrust::SetBeginDate(const CString sValue)
{
	CTrading::SetRequestARData("begin_date", sValue);
	CTrading::SetRequestARData("start_date", sValue);

}

void CReqFundEntrust::SetEndDate(const CString sValue)
{
	CTrading::SetRequestARData("end_date", sValue);
}

void CReqFundEntrust::SetFundDate(const CString sValue)
{
	CTrading::SetRequestARData("fund_date", sValue);
}

void CReqFundEntrust::SetFundIntl(const CString sValue)
{
	CTrading::SetRequestARData("fund_intl", sValue);
}