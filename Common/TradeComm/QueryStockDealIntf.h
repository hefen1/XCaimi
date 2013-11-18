/*******************************************************
  Դ��������:QueryStockDealIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɽ��ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
//#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"
#include "QueryBaseIntf.h"

struct IStockDealItem : virtual public IDataItem
{
	//֤ȯ����
	virtual CString GetStockCode() = 0;
	//�г�
	virtual CString GetExchangeType() = 0;
	//�ɶ��˺�
	virtual CString GetStockAccount() = 0;
	//��������
	virtual EBSDirection GetEntrustBS() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};

struct IStockDealList : virtual public IDataList
{
	virtual IStockDealItem * GetItems(const int i) = 0;
	virtual IStockDealItem * NewItem() = 0;
};

struct IReqQueryStockDeal : virtual public IReqQueryBase
{
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IStockDealList* GetDataList() = 0;
};