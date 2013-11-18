/*******************************************************
  Դ��������:ModifyUserInfo.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  �޸��û���Ϣ�ඨ��
  ��    ��:  shenglq
  ��������:  20110315
*********************************************************/
#pragma once
#include "Trading.h"
#include "ModifyUserInfoIntf.h"
#pragma warning (disable:4250)

class CReqModifyUserInfo : public CTrading, public IReqModifyUserInfo
{
public:
	//֤������
	void SetIdKind(const CString sValue);
	//֤������
	void SetIdNo(const CString sValue);
	//��������
	void SetZipCode(const CString sValue);
	//��ϵ��ַ
	void SetAddress(const CString sValue);
	//���֤��ַ
	void SetIdAddress(const CString sValue);
	//��ϵ�绰
	void SetPhoneCode(const CString sValue);
	//�ƶ��绰
	void SetMobiletelePhone(const CString sValue);
	//����
	void SetFax(const CString sValue);
	//��������
	void SetEMail(const CString sValue);
	//�Ա�
	void SetSex(const CString sValue);
	//У����Ϣ
	void SetLoginCheckInfo(const CString sValue);
	//��ע
	void SetRemark(const CString sValue);
};