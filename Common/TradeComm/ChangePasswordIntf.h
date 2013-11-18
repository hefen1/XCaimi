/*******************************************************
  源程序名称:ChangePasswordIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改密码接口定义
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqChangePassword : virtual public ITrading
{
	//密码类型
	virtual void SetPasswordType(const CString sValue) = 0;
	//原密码
	virtual void SetOldPassword(const CString sValue) = 0;
	//新密码
	virtual void SetNewPassword(const CString sValue) = 0;
};