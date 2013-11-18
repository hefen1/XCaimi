/*******************************************************
  Դ��������:TradeStockEntrust.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯί���ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockEntrustIntf.h"
#pragma warning (disable:4250)

class CReqStockEntrust : public CTrading, public IReqStockEntrust
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//��������
	void SetEntrustBs(const EBSDirection Value);
	//ί������
	void SetEntrustAmount(const int Value);
	//ί�м۸�
	//void SetEntrustPrice(const COleCurrency Value);
	void SetEntrustPrice(const double Value);
	//ί������
	void SetEntrustProp(const CString sValue);
	//�Է�ϯλ
	void SetOppoSeatNo(const CString sValue);
	//Э����
	void SetConferNo(const CString sValue);
	//ת������
	void SetTransCode(const CString sValue);
	//ί������
	void SetEntrustType(const CString sValue);
	//�ɷֹɴ���
	void SetComponentCode(const CString sValue);
};