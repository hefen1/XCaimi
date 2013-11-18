/*******************************************************
  Դ��������:QueryFundEntrust.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������ί����
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundEntrustIntf.h"
#pragma warning (disable:4250)


class CFundEntrustItem : public CDataItem, public IFundEntrustItem
{
public:	
	//����˾
	CString GetFundCompany();
	//�������
	CString GetFundCode();
	//ί�б��
	CString GetEntrustNo();
	//״̬
	CString GetEntrustStatus();
	//�깺����
	CString GetAllotDate();
	//�깺���
	CString GetAllotNo();
	//��λ��
	CString GetPositionStr();
};

class CFundEntrustList : public CDataList, public IFundEntrustList
{
public:
	IFundEntrustItem * GetItems(const int i);
	IFundEntrustItem * NewItem();
protected:
private:
};


class CReqQueryFundEntrust : public CTrading, public IReqQueryFundEntrust
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//�������
	void SetFundCode(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IFundEntrustList* GetDataList();
protected:
	IFundEntrustList* NewDataList();
};