/*******************************************************
  Դ��������:QueryStockFundInfoIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�����̺�ҵ����Ϣ�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110328
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IStockFundInfoItem : virtual public IDataItem
{
	//֤ȯ����
	virtual CString GetStockCode() = 0;
	//�г�
	virtual CString GetExchangeType() = 0;
	//�ɶ��˺�
	virtual CString GetStockAccount() = 0;
	//����״̬
	virtual CString GetFundStatus() = 0;
	//ת��״̬
	virtual CString GetTransStatus() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IStockFundInfoList : virtual public IDataList
{
	virtual IStockFundInfoItem * GetItems(const int i) = 0; 
	virtual IStockFundInfoItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////\

struct IReqQueryStockFundInfo : virtual public ITrading
{
	//�г�
	virtual void SetExchangeType(const CString sValue) = 0;
	//֤ȯ����
	virtual void SetStockCode(const CString sValue) = 0;
	virtual IStockFundInfoList* GetDataList() = 0;
};