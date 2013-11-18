/*******************************************************
  Դ��������:QueryFundInfoIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������Ϣ�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundInfoItem : virtual public IDataItem
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
	//���յȼ�
	virtual CString GetFundRisklevel() = 0;
	//���յȼ�����
	virtual CString GetFundRisklevelName() = 0;
	//��λ��
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundInfoList : virtual public IDataList
{
	virtual IFundInfoItem * GetItems(const int i) = 0; 
	virtual IFundInfoItem * NewItem() = 0;
	virtual IFundInfoItem * GetByFundCode(CString sFundCode) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundInfo : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//�������
	virtual void SetFundCode(const CString sValue) = 0;
	//��λ��
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundInfoList* GetDataList() = 0;
};