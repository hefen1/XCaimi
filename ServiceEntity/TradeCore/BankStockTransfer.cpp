/*******************************************************
  源程序名称:BankStockTransfer.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  银证转帐类实现
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "BankStockTransfer.h"

void CReqBankStockTransfer::SetMoneyType(const EMoneyType mtValue)
{
	CString sValue = "";
	sValue.Format("%d", int(mtValue));
	CTrading::SetRequestARData("money_type",sValue);
}

void CReqBankStockTransfer::SetBankNo(const CString sValue)
{
	CTrading::SetRequestARData("bank_no", sValue);
}

void CReqBankStockTransfer::SetBankPassword(const CString sValue)
{
	CTrading::SetRequestARData("bank_password", sValue);
}

void CReqBankStockTransfer::SetFundPassword(const CString sValue)
{
	CTrading::SetRequestARData("fund_password", sValue);
}

void CReqBankStockTransfer::SetTransferDirection(const CString sValue)
{
	CTrading::SetRequestARData("transfer_direction", sValue);
}

void CReqBankStockTransfer::SetOccurBalance(const double Value)
{
	CString sTmp = "";
	sTmp.Format("%.4f", Value);
	CTrading::SetRequestARData("occur_balance", sTmp);
}