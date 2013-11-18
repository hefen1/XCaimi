/*******************************************************
  源程序名称:ChangePassword.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改密码类定义
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "Trading.h"
#include "ChangePasswordIntf.h"
#pragma warning (disable:4250)

class CReqChangePassword : public CTrading, public IReqChangePassword
{
public:
	//密码类型
	void SetPasswordType(const CString sValue);
	//原密码
	void SetOldPassword(const CString sValue);
	//新密码
	void SetNewPassword(const CString sValue);
};