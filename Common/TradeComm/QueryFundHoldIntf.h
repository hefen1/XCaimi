/*******************************************************
  Դ��������:QueryFundHoldIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����ݶ�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundHoldItem : virtual public IDataItem
{
	//����˾
	virtual CString GetFundCompany() = 0;
	//�������
	virtual CString GetFundCode() = 0;
	//��������
	virtual CString GetFundName() = 0;
	//��ֵ
	virtual CString GetNav() = 0;
	//״̬
	virtual CString GetStatus() = 0;
	//�շ����
	virtual CString GetChargeType() = 0;
	//�깺����
	virtual CString GetAllotDate() = 0;
	//�깺���
	virtual CString GetAllotNo() = 0;
	//�����������
	virtual CString GetEnableShares() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundHoldList : virtual public IDataList
{
	virtual IFundHoldItem * GetItems(const int i) = 0; 
	virtual IFundHoldItem * NewItem() = 0;
	virtual IFundHoldItem * GetByFundCode(CString sFundCode) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundHold : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//�������
	virtual void SetFundCode(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundHoldList* GetDataList() = 0;
};