/*******************************************************
  Դ��������:TradeStockWithDraw.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯ�����ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockWithDrawIntf.h"
#pragma warning (disable:4250)

class CReqStockWithDraw : public CTrading, public IReqStockWithDraw
{
public:
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//ί�б��
	void SetEntrustNo(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue); 
};