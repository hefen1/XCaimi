/*******************************************************
  Դ��������:TradeStockWithDraw.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯ����ʵ��
  ��    ��:  shenglq
  ��������:  20100720
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