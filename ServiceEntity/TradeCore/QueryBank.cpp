/*******************************************************
  Դ��������:QueryBank.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ��ѯ������
  			1��������ʵ��
  			2��������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
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
