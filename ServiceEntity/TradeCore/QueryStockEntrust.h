/*******************************************************
  Դ��������:QueryStockEntrust.h
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
#include "QueryBase.h"
#include "TradeDataBase.h"
#include "QueryStockEntrustIntf.h"
#pragma warning (disable:4250)


class CStockEntrustItem : public CDataItem, public IStockEntrustItem
//class CStockEntrustItem : public IStockEntrustItem
{
public:
	CString GetStockCode();
	CString GetExchangeType();
	CString GetStockAccount();
    EBSDirection GetEntrustBS();
	CString GetPositionStr();
	CString GetEntrustStatus();
protected:

private:
};

class CStockEntrustList : public CDataList, public IStockEntrustList
//class CStockEntrustList : public IStockEntrustList
{
public:
	IStockEntrustItem * GetItems(const int i);
	IStockEntrustItem * NewItem();
protected:
private:
};


class CReqQueryStockEntrust : public CReqQueryBase, public IReqQueryStockEntrust
{
public:	
	//ί�б��
	void SetEntrustNo(const CString sValue);
	//֤ȯ����
	void SetStockCode(const CString sValue);
	//�г�
	void SetExchangeType(const CString sValue);
	//�ɶ��˺�
	void SetStockAccount(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IStockEntrustList* GetDataList();
protected:
	IStockEntrustList* NewDataList();
};