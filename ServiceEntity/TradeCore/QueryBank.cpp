/*******************************************************
  源程序名称:QueryBank.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询银行类
  			1、请求类实现
  			2、返回类实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#include "stdafx.h"
#include "QueryBank.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EMoneyType CBankItem::GetMoneyType()
{
	EMoneyType mtTmp = EMoneyType(CDataItem::GetIntByName("money_type"));
	return mtTmp;
}

CString CBankItem::GetBankNo()
{
	return CDataItem::GetStrByName("bank_no");
}

CString CBankItem::GetBankName()
{
	return CDataItem::GetStrByName("bank_name");
}

CString CBankItem::GetBankAccount()
{
	return CDataItem::GetStrByName("bank_account");
}

CString CBankItem::GetShowPassword()
{
	return CDataItem::GetStrByName("showpassword");
}

CString CBankItem::GetBankPasswordRequire()
{
	return CDataItem::GetStrByName("bank_password_require");
}


//////////////////////////////////////////////////////////////////////////
IBankItem* CBankList::GetItems(const int i)
{

	IBankItem* pItem = NULL;
	pItem = dynamic_cast<IBankItem*>(CDataList::GetItems(i));
	return pItem;
}

IBankItem* CBankList::NewItem()
{
	CBankItem* pItem = new CBankItem;
	return pItem;
}
//////////////////////////////////////////////////////////////////////////
void CReqQueryBank::SetMoneyType(const EMoneyType mtValue)
{
	CString sValue = "";
	sValue.Format("%d", int(mtValue));
	CTrading::SetRequestARData("money_type",sValue);
}

void CReqQueryBank::SetBankNo(const CString sValue)
{
	CTrading::SetRequestARData("bank_no", sValue);
}

IBankList* CReqQueryBank::GetDataList()
{
	return dynamic_cast<IBankList*>(m_DataList);
}

IBankList* CReqQueryBank::NewDataList()
{
	CBankList* pList = new CBankList;
	return pList;
}
