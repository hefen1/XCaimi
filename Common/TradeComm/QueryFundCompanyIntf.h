/*******************************************************
  Դ��������:QueryFundCompanyIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����˾�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundCompanyItem : virtual public IDataItem
{
	//����˾
	virtual CString GetFundCompany() = 0;
	//��˾����
	virtual CString GetCompanyName() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundCompanyList : virtual public IDataList
{
	virtual IFundCompanyItem * GetItems(const int i) = 0; 
	virtual IFundCompanyItem * NewItem() = 0;
	virtual IFundCompanyItem * GetByFundCompany(CString sFundCompany) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundCompany : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundCompanyList* GetDataList() = 0;
};