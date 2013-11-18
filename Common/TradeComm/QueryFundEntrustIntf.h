/*******************************************************
  Դ��������:QueryFundEntrustIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������ί�нӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundEntrustItem : virtual public IDataItem
{
	//����˾
	virtual CString GetFundCompany() = 0;
	//�������
	virtual CString GetFundCode() = 0;
	//ί�б��
	virtual CString GetEntrustNo() = 0;
	//״̬
	virtual CString GetEntrustStatus() = 0;
	//�깺����
	virtual CString GetAllotDate() = 0;
	//�깺���
	virtual CString GetAllotNo() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundEntrustList : virtual public IDataList
{
	virtual IFundEntrustItem * GetItems(const int i) = 0; 
	virtual IFundEntrustItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundEntrust : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//�������
	virtual void SetFundCode(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundEntrustList* GetDataList() = 0;
};