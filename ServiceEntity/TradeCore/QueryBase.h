/*******************************************************
  Դ��������:QueryBase.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���ײ�ѯ���ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryBaseIntf.h"
#pragma warning (disable:4250)

class CReqQueryBase : public CTrading, virtual public IReqQueryBase
{	
public:
	//��ʼ����
	void SetBeginDate(const CString sValue);
	//��������
	void SetEndDate(const CString sValue);
};