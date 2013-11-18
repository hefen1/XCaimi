/*******************************************************
  Դ��������:QueryStockMaxBuyIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ֤ȯ������ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IStockMaxBuyItem : virtual public IDataItem
{
	//��������
	virtual long GetEnableAmount() = 0;
	//virtual void SetEnableAmount(const int Value) = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockMaxBuyList : virtual public IDataList
{
	virtual IStockMaxBuyItem * GetItems(const int i) = 0; 
	virtual IStockMaxBuyItem * NewItem() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IReqQueryStockMaxBuy : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//ί�м۸�
	virtual void SetEntrustPrice(const double Value) = 0;
	//ί������
	virtual void SetEntrustProp(const CString sValue) = 0;
	//ί������
	virtual void SetEntrustType(const CString sValue) = 0;
	virtual IStockMaxBuyList* GetDataList() = 0;
};