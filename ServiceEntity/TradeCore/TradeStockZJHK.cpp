/*******************************************************
  Դ��������:TradeStockZJHK.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֱ�ӻ�����ʵ��
  ��    ��:  shenglq
  ��������:  20110310
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