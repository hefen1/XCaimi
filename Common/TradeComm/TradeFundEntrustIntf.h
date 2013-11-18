/*******************************************************
  Դ��������:TradeFundEntrustIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ����ί�нӿڶ���
  ��    ��:  shenglq
  ��������:  20110216
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqFundEntrust : virtual public ITrading
{
	//����˾
	virtual void SetFundCompany(const CString sValue) = 0;
	//�������
	virtual void SetFundCode(const CString sValue) = 0;
	//ί������
	virtual void SetAmount(const double Value) = 0;
	//ί�н��
	virtual void SetBalance(const double Value) = 0;
	//�շѷ�ʽ
	virtual void SetChargeType(const CString sValue) = 0;
	//�޶����
	virtual void SetExceedFlag(const CString sValue) = 0;
	//�깺����
	virtual void SetAllotDate(const CString sValue) = 0;
	//�깺���
	virtual void SetAllotNo(const CString sValue) = 0;
	//ί�б��
	virtual void SetEntrustNo(const CString sValue) = 0;
	//�ֺ��־
	virtual void SetDividendMethod(const CString sValue) = 0;
	//�����ʺ�
	virtual void SetStockAccount(const CString sValue) = 0;
	//ת������
	virtual void SetTransCode(const CString sValue) = 0;
	//ת���ʺ�
	virtual void SetTransAccount(const CString sValue) = 0;
	//ת������
	virtual void SetTransAmount(const double Value) = 0;
	//�ͻ����յȼ�
	virtual void SetClientRiskLevel(const CString sValue) = 0;
	//ԤԼ����
	virtual void SetHopeDate(const CString sValue) = 0;
	//��ʼ����
	virtual void SetBeginDate(const CString sValue) = 0;
	//��ֹ����
	virtual void SetEndDate(const CString sValue) = 0;
	//�ۿ���
	virtual void SetFundDate(const CString sValue) = 0;
	//��������
	virtual void SetFundIntl(const CString sValue) = 0;
};