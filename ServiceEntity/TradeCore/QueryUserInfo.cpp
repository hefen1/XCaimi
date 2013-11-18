/*******************************************************
  Դ��������:QueryUserInfo.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ�û���Ϣ��
  			1��������ʵ��
  			2��������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryUserInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString CUserInfoItem::GetIdKind()
{
	return CDataItem::GetStrByName("id_kind");
}

CString CUserInfoItem::GetIdNo()
{
	return CDataItem::GetStrByName("id_no");
}

CString CUserInfoItem::GetZipCode()
{
	return CDataItem::GetStrByName("zipcode");
}

CString CUserInfoItem::GetAddress()
{
	return CDataItem::GetStrByName("address");
}

CString CUserInfoItem::GetIdAddress()
{
	return CDataItem::GetStrByName("id_address");
}

CString CUserInfoItem::GetPhoneCode()
{
	return CDataItem::GetStrByName("phonecode");
}

CString CUserInfoItem::GetMobiletelePhone()
{
	return CDataItem::GetStrByName("mobiletelephone");
}

CString CUserInfoItem::GetFax()
{
	return CDataItem::GetStrByName("fax");
}

CString CUserInfoItem::GetEMail()
{
	return CDataItem::GetStrByName("e_mail");
}

CString CUserInfoItem::GetSex()
{
	return CDataItem::GetStrByName("sex");
}

//////////////////////////////////////////////////////////////////////////
IUserInfoItem* CUserInfoList::GetItems(const int i)
{

	IUserInfoItem* pItem = NULL;
	pItem = dynamic_cast<IUserInfoItem*>(CDataList::GetItems(i));
	return pItem;
}

IUserInfoItem* CUserInfoList::NewItem()
{
	CUserInfoItem* pItem = new CUserInfoItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////

IUserInfoList* CReqQueryUserInfo::GetDataList()
{
	return dynamic_cast<IUserInfoList*>(m_DataList);
}

IUserInfoList* CReqQueryUserInfo::NewDataList()
{
	CUserInfoList* pList = new CUserInfoList;
	return pList;
}
