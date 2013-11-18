/*******************************************************
  Դ��������:TradeStockEntrust.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯί��ʵ��
  ��    ��:  shenglq
  ��������:  20100720
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