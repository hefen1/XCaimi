/*******************************************************
  Դ��������:QueryStockHold.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ֲ���
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockHoldIntf.h"
#pragma warning (disable:4250)


class CStockHoldItem : public CDataItem, public IStockHoldItem
{
public:	
	//ί�б��
    CString GetStockCode();
	//�г�
	CString GetExchangeType();
	//�ɶ��˺�
	CString GetStockAccount();
	//��Ʊ���
	long GetCurrentAmount();
	//��������
	long GetEnableAmount();
	//���¼�
	double GetLastPrice();
	void SetLastPrice(const double Value);
	//�ɱ���
	double GetCostPrice();
	//��ֵ
	double GetMarketBalance();
	void SetMarketBalance(const double Value);
	//����ӯ��
	double GetIncomeBalance();
	void SetIncomeBalance(const double Value);
	//ӯ������
	double GetIncomeRatio();
	void SetIncomeRatio(const double Value);
	//��λ��
	CString GetPositionStr();
	//ϯλ
	CString GetSeat();
};

class CStockHoldList : public CDataList, public IStockHoldList
{
public:
	IStockHoldItem * GetItems(const int i); 
	IStockHoldItem * NewItem();
protected:
private:	
};



class CReqQueryStockHold : public CTrading, public IReqQueryStockHold
{
public:	
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IStockHoldList* GetDataList();
protected:
	IStockHoldList* NewDataList();
};