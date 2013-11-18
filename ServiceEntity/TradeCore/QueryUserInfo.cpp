/*******************************************************
  源程序名称:QueryUserInfo.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询用户信息类
  			1、请求类实现
  			2、返回类实现
  作    者:  shenglq
  开发日期:  20100720
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
