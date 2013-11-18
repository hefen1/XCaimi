/*******************************************************
  Դ��������:QueryStockEntrustIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯί�нӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
//#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"
#include "QueryBaseIntf.h"


struct IStockEntrustItem : virtual public IDataItem
{
	virtual CString GetStockCode() = 0;
	//�г�
	virtual CString GetExchangeType() = 0;
	//�ɶ��˺�
	virtual CString  GetStockAccount() = 0;
	//��������
	virtual EBSDirection GetEntrustBS() = 0;
	//��λ
	virtual CString GetPositionStr() = 0;
	//ί��״̬
	virtual CString GetEntrustStatus() = 0;
};

struct IStockEntrustList : virtual public IDataList
{
	virtual IStockEntrustItem * GetItems(const int i) = 0; 
	virtual IStockEntrustItem * NewItem() = 0;
};


struct IReqQueryStockEntrust : virtual public IReqQueryBase
{
	//ί�б��
	virtual void SetEntrustNo(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//�ɶ��˺�
	virtual void SetStockAccount(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IStockEntrustList* GetDataList() = 0;
};