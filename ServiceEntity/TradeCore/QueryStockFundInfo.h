/*******************************************************
  Դ��������:QueryStockFundInfo.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�����̺�ҵ����Ϣ�ӿ�
  			1�������ඨ��
  			2��������ڶ���
  ��    ��:  shenglq
  ��������:  20110328
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockFundInfoIntf.h"
#pragma warning (disable:4250)


class CStockFundInfoItem : public CDataItem, public IStockFundInfoItem
{
public:	
	//ί�б��
    CString GetStockCode();
	//�г�
	CString GetExchangeType();
	//�ɶ��˺�
	CString GetStockAccount();
	//����״̬
	CString GetFundStatus();
	//ת��״̬
	CString GetTransStatus();
};

class CStockFundInfoList : public CDataList, public IStockFundInfoList
{
public:
	IStockFundInfoItem * GetItems(const int i); 
	IStockFundInfoItem * NewItem();
protected:
private:	
};



class CReqQueryStockFundInfo : public CTrading, public IReqQueryStockFundInfo
{
public:	
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//�г�
	void SetExchangeType(const CString sValue);
	IStockFundInfoList* GetDataList();
protected:
	IStockFundInfoList* NewDataList();
};