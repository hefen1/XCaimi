/*******************************************************
  Դ��������:TradeStockEntrustIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ֤ȯί�нӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockEntrust : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//��������
	virtual void SetEntrustBs(const EBSDirection Value) =0;
	//ί������
	virtual void SetEntrustAmount(const int Value) = 0;
	//ί�м۸�
	//virtual void SetEntrustPrice(const COleCurrency Value) = 0;
	virtual void SetEntrustPrice(const double Value) = 0;
	//ί������
	virtual void SetEntrustProp(const CString sValue) = 0;
	//�Է�ϯλ
	virtual void SetOppoSeatNo(const CString sValue) = 0;
	//Э����
	virtual void SetConferNo(const CString sValue) = 0;
	//ת������
	virtual void SetTransCode(const CString sValue) = 0;
	//ί������
	virtual void SetEntrustType(const CString sValue) = 0;
	//�ɷֹɴ���
	virtual void SetComponentCode(const CString sValue) = 0;
};