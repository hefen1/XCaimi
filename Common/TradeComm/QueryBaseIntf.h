/*******************************************************
  Դ��������:QueryBaseIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���ײ�ѯ���ӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqQueryBase : virtual public ITrading
{
	//��ʼ����
	virtual void SetBeginDate(const CString sValue) = 0;
	//��������
	virtual void SetEndDate(const CString sValue) = 0;
};