/*******************************************************
  Դ��������:ChangePassword.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �޸�������ʵ��
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#include "stdafx.h"
#include "ChangePassword.h"

void CReqChangePassword::SetPasswordType(const CString sValue)
{
	CTrading::SetRequestARData("password_type", sValue);
}

void CReqChangePassword::SetOldPassword(const CString sValue)
{
	CTrading::SetRequestARData("old_password", sValue);
}

void CReqChangePassword::SetNewPassword(const CString sValue)
{
	CTrading::SetRequestARData("new_password", sValue);
}

