/*******************************************************
  Դ��������:QueryFundHold.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ����ݶ���
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20110222
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundHoldIntf.h"
#pragma warning (disable:4250)


class CFundHoldItem : public CDataItem, public IFundHoldItem
{
public:	
	//����˾
	CString GetFundCompany();
	//�������
	CString GetFundCode();
	//��������
	CString GetFundName();
	//��ֵ
	CString GetNav();
	//״̬
	CString GetStatus();
	//�շ����
	CString GetChargeType();
	//�깺����
	CString GetAllotDate();
	//�깺���
	CString GetAllotNo();
	//�����������
	CString GetEnableShares();
	//��λ��
	CString GetPositionStr();
};

class CFundHoldList : public CDataList, public IFundHoldList
{
public:
	IFundHoldItem * GetItems(const int i);
	IFundHoldItem * NewItem();
	IFundHoldItem * GetByFundCode(CString sFundCode);
protected:
private:
};


class CReqQueryFundHold : public CTrading, public IReqQueryFundHold
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//�������
	void SetFundCode(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IFundHoldList* GetDataList();
protected:
	IFundHoldList* NewDataList();
};