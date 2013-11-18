/*******************************************************
  Դ��������:TradeLogin.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���׵�½ʵ��
  			1������ʵ��
  			2������ʵ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#include "stdafx.h"
#include "TradeLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString CLoginItem::GetBranchNo()
{
	return CDataItem::GetStrByName("branch_no");
}
CString CLoginItem::GetFundAccount()
{
	return CDataItem::GetStrByName("fund_account");
}

CString CLoginItem::GetClientID()
{
	return CDataItem::GetStrByName("client_id");
}

CString CLoginItem::GetClientName()
{
	return CDataItem::GetStrByName("client_name");
}

CString CLoginItem::GetClientRights()
{
	return CDataItem::GetStrByName("client_rights");
}

CString CLoginItem::GetLoginDate()
{
	return CDataItem::GetStrByName("login_date");
}

CString CLoginItem::GetLoginTime()
{
	return CDataItem::GetStrByName("login_time");
}

CString CLoginItem::GetLastOpEntrustWay()
{
	return CDataItem::GetStrByName("last_op_entrust_way");
}

CString CLoginItem::GetLastOpIp()
{
	return CDataItem::GetStrByName("last_op_ip");
}

CString CLoginItem::GetTabconfirmFlag()
{
	return CDataItem::GetStrByName("tabconfirm_flag");
}

CString CLoginItem::GetInitDate()
{
	return CDataItem::GetStrByName("init_date");
}

CString CLoginItem::GetLastDate()
{
	return CDataItem::GetStrByName("last_date");
}

//////////////////////////////////////////////////////////////////////////
ILoginItem* CLoginList::GetItems(const int i)
{

	ILoginItem* pItem = NULL;
	pItem = dynamic_cast<ILoginItem*>(CDataList::GetItems(i));
	return pItem;
}

ILoginItem* CLoginList::NewItem()
{
	ILoginItem* pItem = new CLoginItem;
	return pItem;
}

//////////////////////////////////////////////////////////////////////////

void CReqLogin::SetOpBranchNo(const CString sValue)
{
	CTrading::SetRequestARData("op_branch_no", sValue);
}

void CReqLogin::SetOpStation(const CString sValue)\
{
	CTrading::SetRequestARData("op_station", sValue);
}

void CReqLogin::SetBranchNo(const CString sValue)
{
	CTrading::SetRequestARData("branch_no", sValue);
}

void CReqLogin::SetInputContent(const CString sValue)
{
	CTrading::SetRequestARData("input_content", sValue);
}

void CReqLogin::SetContentType(const CString sValue)
{
	CTrading::SetRequestARData("content_type", sValue);
}

void CReqLogin::SetAccountContent(const CString sValue)
{
	CTrading::SetRequestARData("account_content", sValue);
}

ILoginList* CReqLogin::GetDataList()
{
	return dynamic_cast<ILoginList*>(m_DataList);
}

ILoginList* CReqLogin::NewDataList()
{
	CLoginList* pList = new CLoginList;
	return pList;
}
