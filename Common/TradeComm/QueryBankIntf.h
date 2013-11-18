/*******************************************************
  Դ��������:QueryBankIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ���нӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IBankItem : virtual public IDataItem
{
	//����
	virtual EMoneyType GetMoneyType() = 0;
	//���д���
	virtual CString GetBankNo() = 0;
	//��������
	virtual CString GetBankName() = 0;
	//�����˺�
	virtual CString GetBankAccount() = 0;
	virtual CString GetShowPassword() = 0;
	virtual CString GetBankPasswordRequire() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IBankList : virtual public IDataList
{
	virtual IBankItem * GetItems(const int i) = 0; 
	virtual IBankItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryBank : virtual public IReqQueryBase
{
	//����
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//���к�
	virtual void SetBankNo(const CString sValue) = 0;
	virtual IBankList* GetDataList() = 0;
};