/*******************************************************
  源程序名称:ModifyUserInfoIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改用户信息接口定义
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqModifyUserInfo : virtual public ITrading
{
	//证件类型
	virtual void SetIdKind(const CString sValue) = 0;
	//证件号码
	virtual void SetIdNo(const CString sValue) = 0;
	//邮政编码
	virtual void SetZipCode(const CString sValue) = 0;
	//联系地址
	virtual void SetAddress(const CString sValue) = 0;
	//身份证地址
	virtual void SetIdAddress(const CString sValue) = 0;
	//联系电话
	virtual void SetPhoneCode(const CString sValue) = 0;
	//移动电话
	virtual void SetMobiletelePhone(const CString sValue) = 0;
	//传真
	virtual void SetFax(const CString sValue) = 0;
	//电子邮箱
	virtual void SetEMail(const CString sValue) = 0;
	//性别
	virtual void SetSex(const CString sValue) = 0;
	//校验信息
	virtual void SetLoginCheckInfo(const CString sValue) = 0;
	//备注
	virtual void SetRemark(const CString sValue) = 0;
};