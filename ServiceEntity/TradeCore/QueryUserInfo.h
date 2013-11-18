/*******************************************************
  源程序名称:QueryUserInfo.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询用户信息类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "QueryUserInfoIntf.h"
#include "QueryBase.h"
#pragma warning (disable:4250)

class CUserInfoItem : public CDataItem, public IUserInfoItem
{
public:	
	//证件类型
	CString GetIdKind();
	//证件号码
	CString GetIdNo();
	//邮政编码
	CString GetZipCode();
	//联系地址
	CString GetAddress();
	//身份证地址
	CString GetIdAddress();
	//联系电话
	CString GetPhoneCode();
	//移动电话
	CString GetMobiletelePhone();
	//传真
	CString GetFax();
	//电子邮箱
	CString GetEMail();
	//性别
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