/*******************************************************
  源程序名称:ModifyUserInfo.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改用户信息类定义
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "Trading.h"
#include "ModifyUserInfoIntf.h"
#pragma warning (disable:4250)

class CReqModifyUserInfo : public CTrading, public IReqModifyUserInfo
{
public:
	//证件类型
	void SetIdKind(const CString sValue);
	//证件号码
	void SetIdNo(const CString sValue);
	//邮政编码
	void SetZipCode(const CString sValue);
	//联系地址
	void SetAddress(const CString sValue);
	//身份证地址
	void SetIdAddress(const CString sValue);
	//联系电话
	void SetPhoneCode(const CString sValue);
	//移动电话
	void SetMobiletelePhone(const CString sValue);
	//传真
	void SetFax(const CString sValue);
	//电子邮箱
	void SetEMail(const CString sValue);
	//性别
	void SetSex(const CString sValue);
	//校验信息
	void SetLoginCheckInfo(const CString sValue);
	//备注
	void SetRemark(const CString sValue);
};