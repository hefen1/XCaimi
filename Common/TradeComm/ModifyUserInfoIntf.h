/*******************************************************
  Դ��������:ModifyUserInfoIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �޸��û���Ϣ�ӿڶ���
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqModifyUserInfo : virtual public ITrading
{
	//֤������
	virtual void SetIdKind(const CString sValue) = 0;
	//֤������
	virtual void SetIdNo(const CString sValue) = 0;
	//��������
	virtual void SetZipCode(const CString sValue) = 0;
	//��ϵ��ַ
	virtual void SetAddress(const CString sValue) = 0;
	//���֤��ַ
	virtual void SetIdAddress(const CString sValue) = 0;
	//��ϵ�绰
	virtual void SetPhoneCode(const CString sValue) = 0;
	//�ƶ��绰
	virtual void SetMobiletelePhone(const CString sValue) = 0;
	//����
	virtual void SetFax(const CString sValue) = 0;
	//��������
	virtual void SetEMail(const CString sValue) = 0;
	//�Ա�
	virtual void SetSex(const CString sValue) = 0;
	//У����Ϣ
	virtual void SetLoginCheckInfo(const CString sValue) = 0;
	//��ע
	virtual void SetRemark(const CString sValue) = 0;
};