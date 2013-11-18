/*******************************************************
  源程序名称:QueryUserInfoIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询用户信息接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "QueryBaseIntf.h"

struct IUserInfoItem : virtual public IDataItem
{
	//证件类型
	virtual CString GetIdKind() = 0;
	//证件号码
	virtual CString GetIdNo() = 0;
	//邮政编码
	virtual CString GetZipCode() = 0;
	//联系地址
	virtual CString GetAddress() = 0;
	//身份证地址
	virtual CString GetIdAddress() = 0;
	//联系电话
	virtual CString GetPhoneCode() = 0;
	//移动电话
	virtual CString GetMobiletelePhone() = 0;
	//传真
	virtual CString GetFax() = 0;
	//电子邮箱
	virtual CString GetEMail() = 0;
	//性别
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