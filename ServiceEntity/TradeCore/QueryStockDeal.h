/*******************************************************
  Դ��������:QueryStockDeal.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɽ���
  			1�������ඨ��
  			2��������ڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "QueryBase.h"
#include "TradeDataBase.h"
#include "QueryStockDealIntf.h"
#pragma warning (disable:4250)


class CStockDealItem : public CDataItem, public IStockDealItem
{
public:
	//֤ȯ����
	CString GetStockCode();
	//�г�
	CString GetExchangeType();
	//�ɶ��˺�
	CString GetStockAccount();
	//��������
	EBSDirection GetEntrustBS();
	//��λ��
	CString GetPositionStr();
};

class CStockDealList : public CDataList, public IStockDealList
{
public:
	IStockDealItem * GetItems(const int i);
	IStockDealItem * NewItem();
protected:
private:	
};


class CReqQueryStockDeal : public CReqQueryBase, public IReqQueryStockDeal
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
	IStockDealList* GetDataList();
protected:
	IStockDealList* NewDataList();

};