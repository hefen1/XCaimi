/*******************************************************
  Դ��������:BankStockTransferIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��֤ת�ʽӿڶ���
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqBankStockTransfer : virtual public ITrading
{
	//����
	virtual void SetMoneyType(const EMoneyType mtValue) = 0;
	//���д���
	virtual void SetBankNo(const CString sValue) = 0;
	//��������
	virtual void SetBankPassword(const CString sValue) = 0;
	//�ʽ�����
	virtual void SetFundPassword(const CString sValue) = 0;
	//ת�ʷ���
	virtual void SetTransferDirection(const CString sValue) = 0;
	//ת�ʽ��
	virtual void SetOccurBalance(const double Value) = 0;
};