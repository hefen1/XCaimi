/*******************************************************
  Դ��������:QueryFundInfo.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������Ϣ��
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundInfoIntf.h"
#pragma warning (disable:4250)


class CFundInfoItem : public CDataItem, public IFundInfoItem
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
	//���յȼ�
	CString GetFundRisklevel();
	//���յȼ�����
	CString GetFundRisklevelName();
	//��λ��
	CString GetPositionStr();
};

class CFundInfoList : public CDataList, public IFundInfoList
{
public:
	IFundInfoItem * GetItems(const int i);
	IFundInfoItem * NewItem();
	IFundInfoItem * GetByFundCode(CString sFundCode);
protected:
	virtual IFundInfoList* NewDataList();
private:
};


class CReqQueryFundInfo : public CTrading, public IReqQueryFundInfo
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//�������
	void SetFundCode(const CString sValue);
	//��λ��
	void SetPositionStr(const CString sValue);
	IFundInfoList* GetDataList();
protected:
	IFundInfoList* NewDataList();
};