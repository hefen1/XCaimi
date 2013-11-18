/*******************************************************
  Դ��������:QueryUserInfo.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�û���Ϣ��
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryUserInfoIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)

class CUserInfoItem : public CDataItem, public IUserInfoItem
{
public:	
	//֤������
	CString GetIdKind();
	//֤������
	CString GetIdNo();
	//��������
	CString GetZipCode();
	//��ϵ��ַ
	CString GetAddress();
	//���֤��ַ
	CString GetIdAddress();
	//��ϵ�绰
	CString GetPhoneCode();
	//�ƶ��绰
	CString GetMobiletelePhone();
	//����
	CString GetFax();
	//��������
	CString GetEMail();
	//�Ա�
	CString GetSex();
};
//////////////////////////////////////////////////////////////////////////

class CUserInfoList : public CDataList, public IUserInfoList
{
public:	
	IUserInfoItem * GetItems(const int i); 
	IUserInfoItem * NewItem();
};

//////////////////////////////////////////////////////////////////////////

class CReqQueryUserInfo : public CReqQueryBase, virtual public IReqQueryUserInfo
{
	
public:
	IUserInfoList* GetDataList();
protected:
	IUserInfoList* NewDataList();
};