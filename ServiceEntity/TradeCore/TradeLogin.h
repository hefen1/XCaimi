/*******************************************************
  Դ��������:TradeLogin.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���׵�½��
  			1�������ඨ��
  			2�������ඨ��
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "TradeLoginIntf.h"
#pragma warning (disable:4250)


class CLoginItem : public CDataItem, public ILoginItem
{
public:
	CString GetBranchNo();
	CString GetFundAccount();
	CString GetClientID();
	CString GetClientName();
	CString GetClientRights();
	CString GetLoginDate();
	CString GetLoginTime();
	CString GetLastOpEntrustWay();
	CString GetLastOpIp();
	CString GetTabconfirmFlag();
	CString GetInitDate();
	CString GetLastDate();
};

class CLoginList : public CDataList, public ILoginList
{
public:
	ILoginItem * GetItems(const int i);
	ILoginItem * NewItem();
protected:
private:
};


class CReqLogin : public CTrading, public IReqLogin
{
public:   
	void SetOpBranchNo(const CString sValue);
	void SetOpStation(const CString sValue);
	void SetBranchNo(const CString sValue);
	void SetInputContent(const CString sValue);
	void SetContentType(const CString sValue);
	void SetAccountContent(const CString sValue);
	ILoginList * GetDataList();
protected:
	ILoginList* NewDataList();
};