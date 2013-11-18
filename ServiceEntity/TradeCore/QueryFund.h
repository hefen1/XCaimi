/*******************************************************
  Դ��������:QueryFund.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ʽ���
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
//#include "TradeDataBase.h"
#include "QueryFundIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)


class CFundItem : public CDataItem, public IFundItem
{
public:
	//�������
	EMoneyType GetMoneyType();
	void SetMoneyType(const EMoneyType mtValue);
	//��ǰ���
	double GetCurrentBalance();
	void SetCurrentBalance(const double Value);
	//���ý��
	double GetEnableBalance();
	void SetEnableBalance(const double Value);
	//�ʽ��ʲ�
	double GetFundAsset();
	void SetFundAsset(const double value);
	//�ʲ���ֵ
	double GetAssetBalance();
	void SetAssetBalance(const double value);
	//֤ȯ��ֵ
	double GetMarketValue();
	void SetMarketValue(const double value);
	//���ʸ�ծ
	double GetFinanceDebit();
};

class CFundList : public CDataList, public IFundList
{
public:
	IFundItem * GetItems(const int i);
	IFundItem * NewItem();
protected:
private:
};

class CReqQueryFund : public CReqQueryBase, virtual public IReqQueryFund
{
public:   
	//����
	void SetMoneyType(const EMoneyType mtValue);
    IFundList* GetDataList();
protected:
	IFundList* NewDataList();
};