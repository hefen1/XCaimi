/*******************************************************
  Դ��������:TradeStockWithDrawIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯ�����ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockWithDraw : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//ί�б��
    virtual void SetEntrustNo(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
};