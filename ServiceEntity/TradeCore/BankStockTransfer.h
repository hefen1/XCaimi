/*******************************************************
  Դ��������:BankStockTransfer.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��֤ת���ඨ��
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#pragma once
#include "Trading.h"
#include "BankStockTransferIntf.h"
#pragma warning (disable:4250)

class CReqBankStockTransfer : public CTrading, public IReqBankStockTransfer
{
	
public:
	//����
	void SetMoneyType(const EMoneyType mtValue);
	//���д���
	void SetBankNo(const CString sValue);
	//��������
	void SetBankPassword(const CString sValue);
	//�ʽ�����
	void SetFundPassword(const CString sValue);
	//ת�ʷ���
	void SetTransferDirection(const CString sValue);
	//ת�ʽ��
	void SetOccurBalance(const double Value);
};