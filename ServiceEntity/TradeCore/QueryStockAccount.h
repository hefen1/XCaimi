/*******************************************************
  Դ��������:QueryStockAccount.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɶ��˺���
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockAccountIntf.h"
#pragma warning (disable:4250)


class CStockAccountItem : public CDataItem, public IStockAccountItem
{
public:	
	//�г�
	CString GetExchangeType();
	//�ɶ��˺�
	CString GetStockAccount();
	//���ɶ��˺ű�ʶ
	bool GetMainFlag();
	//ϯλ��
	CString GetSeatNo();
};

class CStockAccountList : public CDataList, public IStockAccountList
{
public:
	IStockAccountItem * GetItems(const int i);
	IStockAccountItem * NewItem();
protected:
private:
};


class CReqQueryStockAccount : public CTrading, public IReqQueryStockAccount
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	IStockAccountList* GetDataList();
protected:
	IStockAccountList* NewDataList();
};