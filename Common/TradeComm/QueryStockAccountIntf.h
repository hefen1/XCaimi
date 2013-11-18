/*******************************************************
  Դ��������:QueryStockAccountIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�ɶ��˺Žӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IStockAccountItem : virtual public IDataItem
{
	//�г�
	virtual CString GetExchangeType() = 0;
	//�ɶ��˺�
	virtual CString GetStockAccount() = 0;
	//���ɶ��˺ű�ʶ
	virtual bool GetMainFlag() = 0;
	//ϯλ��
	virtual CString GetSeatNo() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockAccountList : virtual public IDataList
{
	virtual IStockAccountItem * GetItems(const int i) = 0; 
	virtual IStockAccountItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////\

struct IReqQueryStockAccount : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	virtual IStockAccountList* GetDataList() = 0;
};