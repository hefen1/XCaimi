/*******************************************************
  Դ��������:ChangePassword.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �޸������ඨ��
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#pragma once
#include "Trading.h"
#include "ChangePasswordIntf.h"
#pragma warning (disable:4250)

class CReqChangePassword : public CTrading, public IReqChangePassword
{
public:
	//��������
	void SetPasswordType(const CString sValue);
	//ԭ����
	void SetOldPassword(const CString sValue);
	//������
	void SetNewPassword(const CString sValue);
};