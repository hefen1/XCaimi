/*******************************************************
  Դ��������:TradeStockZJHK.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯֱ�ӻ����ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockZJHKIntf.h"
#pragma warning (disable:4250)

class CReqStockZJHK : public CTrading, public IReqStockZJHK
{
public:	
	//����
	void SetMoneyType(const CString sValue);
	//ί�н��
	void SetOccurBalance(const double Value);
};