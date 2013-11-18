/*******************************************************
  源程序名称:ModifyUserInfo.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  修改用户信息类实现
  作    者:  shenglq
  开发日期:  20110315
*********************************************************/
#include "stdafx.h"
#include "ModifyUserInfo.h"

void CReqModifyUserInfo::SetIdKind(const CString sValue)
{
	CTrading::SetRequestARData("id_kind", sValue);
}

void CReqModifyUserInfo::SetIdNo(const CString sValue)
{
	CTrading::SetRequestARData("id_no", sValue);
}

void CReqModifyUserInfo::SetZipCode(const CString sValue)
{
	CTrading::SetRequestARData("zipcode", sValue);
}

void CReqModifyUserInfo::SetAddress(const CString sValue)
{
	CTrading::SetRequestARData("address", sValue);
}

void CReqModifyUserInfo::SetIdAddress(const CString sValue)
{
	CTrading::SetRequestARData("id_address", sValue);
}

void CReqModifyUserInfo::SetPhoneCode(const CString sValue)
{
	CTrading::SetRequestARData("phonecode", sValue);
}

void CReqModifyUserInfo::SetMobiletelePhone(const CString sValue)
{
	CTrading::SetRequestARData("mobiletelephone", sValue);
}

void CReqModifyUserInfo::SetFax(const CString sValue)
{
	CTrading::SetRequestARData("fax", sValue);
}

void CReqModifyUserInfo::SetEMail(const CString sValue)
{
	CTrading::SetRequestARData("e_mail", sValue);
}

void CReqModifyUserInfo::SetSex(const CString sValue)
{
	CTrading::SetRequestARData("sex", sValue);
}

void CReqModifyUserInfo::SetLoginCheckInfo(const CString sValue)
{
	CTrading::SetRequestARData("logincheckinfo", sValue);
}

void CReqModifyUserInfo::SetRemark(const CString sValue)
{
	CTrading::SetRequestARData("remark", sValue);
}

