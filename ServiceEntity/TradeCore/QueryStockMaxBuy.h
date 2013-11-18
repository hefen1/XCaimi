/*******************************************************
  Դ��������:QueryStockMaxBuy.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ֤ȯ�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockMaxBuyIntf.h"
#pragma warning (disable:4250)

class CStockMaxBuyItem : public CDataItem, public IStockMaxBuyItem
{
public:	
	long GetEnableAmount();
};
//////////////////////////////////////////////////////////////////////////
class CStockMaxBuyList : public CDataList, public IStockMaxBuyList
{
public:
	IStockMaxBuyItem * GetItems(const int i); 
	IStockMaxBuyItem * NewItem();
protected:
private:	
};
//////////////////////////////////////////////////////////////////////////
class CReqQueryStockMaxBuy : public CTrading, public IReqQueryStockMaxBuy
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//ί�м۸�
	void SetEntrustPrice(const double Value);
	//ί������
	void SetEntrustProp(const CString sValue);
	//ί������
	void SetEntrustType(const CString sValue);
	IStockMaxBuyList* GetDataList();
protected:
	IStockMaxBuyList* NewDataList();
};