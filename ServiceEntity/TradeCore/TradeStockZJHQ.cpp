/*******************************************************
  Դ��������:TradeStockZJHQ.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯֱ�ӻ�ȯʵ��
  ��    ��:  shenglq
  ��������:  20100720
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