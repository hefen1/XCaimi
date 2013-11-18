/*******************************************************
  Դ��������:TradeFundEntrust.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����ί�нӿڶ���
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeFundEntrustIntf.h"
#pragma warning (disable:4250)

class CReqFundEntrust : public CTrading, public IReqFundEntrust
{
public:	
	//����˾
	void SetFundCompany(const CString sValue);
	//�������
	void SetFundCode(const CString sValue);
	//ί������
	void SetAmount(const double Value);
	//ί�н��
	void SetBalance(const double Value);
	//�շѷ�ʽ
	void SetChargeType(const CString sValue);
	//�޶����
	void SetExceedFlag(const CString sValue);
	//�깺����
	void SetAllotDate(const CString sValue);
	//�깺���
	void SetAllotNo(const CString sValue);
	//ί�б��
	void SetEntrustNo(const CString sValue);
	//�ֺ��־
	void SetDividendMethod(const CString sValue);
	//�����ʺ�
	void SetStockAccount(const CString sValue);
	//ת������
	void SetTransCode(const CString sValue);
	//ת���ʺ�
	void SetTransAccount(const CString sValue);
	//ת������
	void SetTransAmount(const double Value);
	//�ͻ����յȼ�
	void SetClientRiskLevel(const CString sValue);
	//ԤԼ����
	void SetHopeDate(const CString sValue);
	//��ʼ����
	void SetBeginDate(const CString sValue);
	//��ֹ����
	void SetEndDate(const CString sValue);
	//�ۿ���
	void SetFundDate(const CString sValue);
	//��������
	void SetFundIntl(const CString sValue);
};