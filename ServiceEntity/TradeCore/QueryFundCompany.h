/*******************************************************
  Դ��������:QueryFundCompany.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����˾��
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundCompanyIntf.h"
#pragma warning (disable:4250)


class CFundCompanyItem : public CDataItem, public IFundCompanyItem
{
public:	
	//����˾
	CString GetFundCompany();
	//��˾����
	CString GetCompanyName();
	//��λ��
	CString GetPositionStr();
};

class CFundCompanyList : public CDataList, public IFundCompanyList
{
public:
	IFundCompanyItem * GetItems(const int i);
	IFundCompanyItem * NewItem();
	IFundCompanyItem * GetByFundCompany(CString sFundCompany);
protected:
private:
};


class CReqQueryFundCompany : public CTrading, public IReqQueryFundCompany
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IFundCompanyList* GetDataList();
protected:
	IFundCompanyList* NewDataList();
};