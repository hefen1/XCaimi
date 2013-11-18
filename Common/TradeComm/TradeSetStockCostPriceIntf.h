/*******************************************************
  Դ��������:TradeSetStockCostPriceIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���óɱ��۽ӿڶ���
  ��    ��:  shenglq
  ��������:  20110317
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqSetStockCostPrice : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//�ɱ���
	virtual void SetCostPrice(const double Value) = 0;
	//ί��ϯλ
	virtual void SetSeat(const CString sValue) = 0;
	//���
	virtual void SetEntrustBalance(const double Value) = 0;
	//��ע
	virtual void SetRemark(const CString sValue) = 0;
};