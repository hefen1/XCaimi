/*******************************************************
  Դ��������:QueryUserInfoIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�û���Ϣ�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IUserInfoItem : virtual public IDataItem
{
	//֤������
	virtual CString GetIdKind() = 0;
	//֤������
	virtual CString GetIdNo() = 0;
	//��������
	virtual CString GetZipCode() = 0;
	//��ϵ��ַ
	virtual CString GetAddress() = 0;
	//���֤��ַ
	virtual CString GetIdAddress() = 0;
	//��ϵ�绰
	virtual CString GetPhoneCode() = 0;
	//�ƶ��绰
	virtual CString GetMobiletelePhone() = 0;
	//����
	virtual CString GetFax() = 0;
	//��������
	virtual CString GetEMail() = 0;
	//�Ա�
	virtual CString GetSex() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IUserInfoList : virtual public IDataList
{
	virtual IUserInfoItem * GetItems(const int i) = 0; 
	virtual IUserInfoItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryUserInfo : virtual public IReqQueryBase
{
	virtual IUserInfoList* GetDataList() = 0;
};