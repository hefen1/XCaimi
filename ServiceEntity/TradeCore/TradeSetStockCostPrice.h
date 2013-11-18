/*******************************************************
  Դ��������:TradeSetStockCostPrice.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���óɱ����ඨ��
  ��    ��:  shenglq
  ��������:  20110317
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeSetStockCostPriceIntf.h"
#pragma warning (disable:4250)

class CReqSetStockCostPrice : public CTrading, public IReqSetStockCostPrice
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//�ɱ���
	void SetCostPrice(const double Value);
	//ί��ϯλ
	void SetSeat(const CString sValue);
	//���
	void SetEntrustBalance(const double Value);
	//��ע
	void SetRemark(const CString sValue);
};