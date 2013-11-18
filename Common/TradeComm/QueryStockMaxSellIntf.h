/*******************************************************
  Դ��������:QueryStockMaxSellIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ֤ȯ�������ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IStockMaxSellItem : virtual public IDataItem
{
	//��������
	virtual long GetEnableAmount() = 0;
	//�ɺϲ�����
	virtual long GetMergeAmount() = 0;
	//�ɲ������
	virtual long GetSplitAmount() = 0;
	//��ȯ��ծ����
	virtual long GetShortsellDebitAmount() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockMaxSellList : virtual public IDataList
{
	virtual IStockMaxSellItem * GetItems(const int i) = 0; 
	virtual IStockMaxSellItem * NewItem() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IReqQueryStockMaxSell : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//ί������
	virtual void SetEntrustProp(const CString sValue) = 0;
	virtual IStockMaxSellList* GetDataList() = 0;
};