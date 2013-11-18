/*******************************************************
  Դ��������:QueryFundIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ʽ�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IFundItem : virtual public IDataItem
{
	//�������
	virtual EMoneyType GetMoneyType() = 0;
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//��ǰ���
	virtual double GetCurrentBalance() = 0;
	virtual void SetCurrentBalance(const double Value) = 0;
	//���ý��
	virtual double GetEnableBalance() = 0;
	virtual void SetEnableBalance(const double Value) = 0;
	//�ʽ��ʲ�
	virtual double GetFundAsset() = 0;
	virtual void SetFundAsset(const double value) = 0;
	//�ʲ���ֵ
	virtual double GetAssetBalance() = 0;
	virtual void SetAssetBalance(const double value) = 0;
	//֤ȯ��ֵ
	virtual double GetMarketValue() = 0;
	virtual void SetMarketValue(const double value) = 0;
	//���ʸ�ծ
	virtual double GetFinanceDebit() = 0;
};

struct IFundList : virtual public IDataList
{
	virtual IFundItem * GetItems(const int i) = 0; 
	virtual IFundItem * NewItem() = 0;
};

struct IReqQueryFund : virtual public IReqQueryBase
{
	//����
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	virtual IFundList* GetDataList() = 0;
};