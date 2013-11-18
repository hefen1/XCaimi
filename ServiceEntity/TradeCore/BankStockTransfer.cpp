/*******************************************************
  Դ��������:BankStockTransfer.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��֤ת����ʵ��
  ��    ��:  shenglq
  ��������:  20110310
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