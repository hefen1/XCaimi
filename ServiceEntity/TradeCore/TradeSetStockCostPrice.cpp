/*******************************************************
  Դ��������:TradeStockEntrust.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���óɱ���ʵ��
  ��    ��:  shenglq
  ��������:  20110317
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