/*******************************************************
  Դ��������:TradeStockZJHKIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯֱ�ӻ���ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockZJHK : virtual public ITrading
{
	//����
	virtual void SetMoneyType(const CString sValue) = 0;
	//ί�н��
	virtual void SetOccurBalance(const double Value) = 0;
};