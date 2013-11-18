/*******************************************************
  Դ��������:QueryFundStockAccountIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�����˺Žӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundStockAccountItem : virtual public IDataItem
{
	//����˾
	virtual CString GetFundCompany() = 0;
	//�����˺�
	virtual CString GetStockAccount() = 0;
	//ת���˺�
	virtual CString GetTransAccount() = 0;
	//��˾����
	virtual CString GetCompanyName() = 0;
	//״̬
	virtual CString GetHolderStatus() = 0;
	//��������
	virtual CString GetOpenDate() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundStockAccountList : virtual public IDataList
{
	virtual IFundStockAccountItem * GetItems(const int i) = 0; 
	virtual IFundStockAccountItem * NewItem() = 0;
	virtual IFundStockAccountItem * GetByFundCompany(CString sFundCompany) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundStockAccount : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundStockAccountList* GetDataList() = 0;
};