/*******************************************************
  Դ��������:QueryFundStockAccount.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�����˺���
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundStockAccountIntf.h"
#pragma warning (disable:4250)


class CFundStockAccountItem : public CDataItem, public IFundStockAccountItem
{
public:	
	//����˾
	CString GetFundCompany();
	//�����˺�
	CString GetStockAccount();
	//ת���˺�
	CString GetTransAccount();
	//��˾����
	CString GetCompanyName();
	//״̬
	CString GetHolderStatus();
	//��������
	CString GetOpenDate();
	//��λ��
	CString GetPositionStr();
};

class CFundStockAccountList : public CDataList, public IFundStockAccountList
{
public:
	IFundStockAccountItem * GetItems(const int i);
	IFundStockAccountItem * NewItem();
	IFundStockAccountItem * GetByFundCompany(CString sFundCompany);
protected:
private:
};


class CReqQueryFundStockAccount : public CTrading, public IReqQueryFundStockAccount
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IFundStockAccountList* GetDataList();
protected:
	IFundStockAccountList* NewDataList();
};