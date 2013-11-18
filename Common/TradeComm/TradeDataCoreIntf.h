/*******************************************************
  Դ��������:TradeDataCoreIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����������������
        1�����ġ�ȡ���������ݽӿڶ���
  	2����ȡ���������ݽӿڶ��� 
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#define TRADE_SUB_FUND						0x00000001 //�ʽ�
#define	TRADE_SUB_STOCK_HOLD				0x00000002 //��Ʊ�ֲ�
#define	TRADE_SUB_STOCK_ENTRUST				0x00000004 //��Ʊί��
#define	TRADE_SUB_STOCK_DEAL				0x00000008 //��Ʊ�ɽ�
#define	TRADE_SUB_FUND_HOLD					0x00000010 //����ֲ�
#define	TRADE_SUB_FUND_ENTRUST				0x00000020 //����ί��



#include "TradeDataBaseIntf.h"
#include "QueryStockEntrustIntf.h"
#include "QueryStockDealIntf.h"
#include "QueryFundIntf.h"
#include "QueryStockHoldIntf.h"
#include "QueryFundEntrustIntf.h"
#include "QueryFundHoldIntf.h"


enum EDataChangeType {icAdd, icModify, icDel, icReflash};

//��Ҫ�������ݵ������̳д���
struct ITradeDataCallBack
{
	virtual DWORD GetDataType() = 0;
	virtual void OnReflash(IDataList * pDataList, DWORD dDataType) = 0; 
	virtual void OnChange(IDataItem * pDataItem, EDataChangeType dtChangeType, DWORD dDataType) = 0; 
};

struct ITradeDataCore
{
	//�ͷ�
	virtual void Release() = 0;
	//��������
	virtual void Subscribe(ITradeDataCallBack * pOwner) = 0;
	//ȡ������
	virtual void CancelSubscribe(ITradeDataCallBack * pOwner) = 0;
	//��ȡ�������ʽ�
	virtual IFundList * GetFund(void) = 0;
	virtual void SetFund(IFundList * pDataList) = 0;
	//��ȡ�����ù�Ʊ�ֲ�
	virtual IStockHoldList * GetStockHold(void) = 0;
	virtual void SetStockHold(IStockHoldList * pDataList) = 0;
	//��ȡ�����ù�Ʊί��
	virtual IStockEntrustList * GetStockEntrust(void) = 0;
	virtual void SetStockEntrust(IStockEntrustList * pDataList) = 0;
	//��ȡ�����ù�Ʊ�ɽ�
	virtual IStockDealList * GetStockDeal(void) = 0;
	virtual void SetStockDeal(IStockDealList * pDataList) = 0;

	//��ȡ�����û���ֲ�
	virtual IFundHoldList * GetFundHold(void) = 0;
	virtual void SetFundHold(IFundHoldList * pDataList) = 0;
	//��ȡ�����û���ί��
	virtual IFundEntrustList * GetFundEntrust(void) = 0;
	virtual void SetFundEntrust(IFundEntrustList * pDataList) = 0;
};