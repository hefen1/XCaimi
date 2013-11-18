/*******************************************************
  Դ��������:QueryStockMaxSell.h
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
#include "QueryStockMaxSellIntf.h"
#pragma warning (disable:4250)

class CStockMaxSellItem : public CDataItem, public IStockMaxSellItem
{
public:	
	long GetEnableAmount();
	//�ɺϲ�����
	long GetMergeAmount();
	//�ɲ������
	long GetSplitAmount();
	//��ȯ��ծ����
	long GetShortsellDebitAmount();
};
//////////////////////////////////////////////////////////////////////////
class CStockMaxSellList : public CDataList, public IStockMaxSellList
{
public:
	IStockMaxSellItem * GetItems(const int i); 
	IStockMaxSellItem * NewItem();
protected:
private:	
};
//////////////////////////////////////////////////////////////////////////
class CReqQueryStockMaxSell : public CTrading, public IReqQueryStockMaxSell
{
public:	
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//ί������
	void SetEntrustProp(const CString sValue);
	IStockMaxSellList* GetDataList();
protected:
	IStockMaxSellList* NewDataList();
};