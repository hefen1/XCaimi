/*******************************************************
  源程序名称:ChangePassword.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改密码类实现
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#include "stdafx.h"
#include "ChangePassword.h"

void CReqChangePassword::SetPasswordType(const CString sValue)
{
	CTrading::SetRequestARData("password_type", sValue);
}

void CReqChangePassword::SetOldPassword(const CString sValue)
{
	CTrading::SetRequestARData("old_password", sValue);
}

void CReqChangePassword::SetNewPassword(const CString sValue)
{
	CTrading::SetRequestARData("new_password", sValue);
}

