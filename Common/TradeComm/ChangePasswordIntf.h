/*******************************************************
  Դ��������:ChangePasswordIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �޸�����ӿڶ���
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqChangePassword : virtual public ITrading
{
	//��������
	virtual void SetPasswordType(const CString sValue) = 0;
	//ԭ����
	virtual void SetOldPassword(const CString sValue) = 0;
	//������
	virtual void SetNewPassword(const CString sValue) = 0;
};