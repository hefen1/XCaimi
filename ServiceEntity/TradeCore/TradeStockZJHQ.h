/*******************************************************
  Դ��������:TradeStockZJHQ.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯֱ�ӻ�ȯ�ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockZJHQIntf.h"
#pragma warning (disable:4250)

class CReqStockZJHQ : public CTrading, public IReqStockZJHQ
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//ί������
	void SetEntrustAmount(const int Value);
};