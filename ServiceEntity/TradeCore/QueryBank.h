/*******************************************************
  Դ��������:QueryBank.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryBankIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)

class CBankItem : public CDataItem, public IBankItem
{
public:	
	//����
	EMoneyType GetMoneyType();
	//���д���
	CString GetBankNo();
	//��������
	CString GetBankName();
	//�����˺�
	CString GetBankAccount();
	CString GetShowPassword();
	CString GetBankPasswordRequire();
};
//////////////////////////////////////////////////////////////////////////

class CBankList : public CDataList, public IBankList
{
public:	
	IBankItem * GetItems(const int i); 
	IBankItem * NewItem();
};

//////////////////////////////////////////////////////////////////////////

class CReqQueryBank : public CReqQueryBase, virtual public IReqQueryBank
{
	
public:
	//����
	void SetMoneyType(const EMoneyType mtValue);
	//���к�
	void SetBankNo(const CString sValue);
	IBankList* GetDataList();
protected:
	IBankList* NewDataList();
};