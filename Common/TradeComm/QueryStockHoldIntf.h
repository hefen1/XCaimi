/*******************************************************
  Դ��������:QueryStockHoldIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ֲֽӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"


struct IStockHoldItem : virtual public IDataItem
{
	//֤ȯ����
    virtual CString GetStockCode() = 0;
	//�г�
	virtual CString GetExchangeType() = 0;
	//�ɶ��˺�
	virtual CString GetStockAccount() = 0;
	//��Ʊ���
	virtual long GetCurrentAmount() = 0;
	//��������
	virtual long GetEnableAmount() = 0;
	//���¼�
	virtual double GetLastPrice() = 0;
	virtual void SetLastPrice(const double Value) = 0;
	//�ɱ���
	virtual double GetCostPrice() = 0;
	//��ֵ
	virtual double GetMarketBalance() = 0;
	virtual void SetMarketBalance(const double Value) = 0;
	//����ӯ��
	virtual double GetIncomeBalance() = 0;
	virtual void SetIncomeBalance(const double Value) = 0;
	//ӯ������
	virtual double GetIncomeRatio() = 0;
	virtual void SetIncomeRatio(const double Value) = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
	//ϯλ
	virtual CString GetSeat() = 0;
};

struct IStockHoldList : virtual public IDataList
{
	virtual IStockHoldItem * GetItems(const int i) = 0; 
	virtual IStockHoldItem * NewItem() = 0;
};


struct IReqQueryStockHold : virtual public ITrading
{
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IStockHoldList* GetDataList() = 0;
};