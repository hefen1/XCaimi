/*******************************************************
  Դ��������:TradeDataCore.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����������������
        1�����ġ�ȡ�����������ඨ��
  		2����ȡ�����������ඨ�� 
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradeDataCoreIntf.h"

class CTradeDataCore : public ITradeDataCore
{
public:	
	CTradeDataCore();
	~CTradeDataCore();
	//�ͷ�
	void Release();
	//��������
	void Subscribe(ITradeDataCallBack * pOwne);
	//ȡ������
	void CancelSubscribe(ITradeDataCallBack * pOwner);
	//��ȡ�������ʽ�
	IFundList * GetFund();
	void SetFund(IFundList * pDataList);
	//��ȡ�����ù�Ʊ�ֲ�
	IStockHoldList * GetStockHold();
	void SetStockHold(IStockHoldList * pDataList);
	//��ȡ�����ù�Ʊί��
	IStockEntrustList * GetStockEntrust();
	void SetStockEntrust(IStockEntrustList * pDataList);
	//��ȡ�����ù�Ʊ�ɽ�
	IStockDealList * GetStockDeal();
	void SetStockDeal(IStockDealList * pDataList);

	//��ȡ�����û���ֲ�
	IFundHoldList * GetFundHold(void);
	void SetFundHold(IFundHoldList * pDataList);
	//��ȡ�����û���ί��
	IFundEntrustList * GetFundEntrust(void);
	void SetFundEntrust(IFundEntrustList * pDataList);
private:
	IFundList* m_pFundList;
	IStockHoldList* m_pStockHoldList;
	IStockEntrustList* m_pStockEntrustList;
	IStockDealList* m_pStockDealList;
	CArray<ITradeDataCallBack*, ITradeDataCallBack*> m_SubscribeList;
	IFundHoldList* m_pFundHoldList;
	IFundEntrustList* m_pFundEntrustList;
};