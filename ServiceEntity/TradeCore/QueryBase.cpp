/*******************************************************
  Դ��������:QueryBase.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���ײ�ѯ����ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CReqQueryBase::SetBeginDate(const CString sValue)
{
	CTrading::SetRequestARData("begin_date",sValue);
	CTrading::SetRequestARData("start_date",sValue);
}

void CReqQueryBase::SetEndDate(const CString sValue)
{
	CTrading::SetRequestARData("end_date", sValue);
}