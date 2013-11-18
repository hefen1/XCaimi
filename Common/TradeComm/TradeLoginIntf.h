/*******************************************************
  Դ��������:TradeLoginIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ���׵�½�ӿ�
  			1������ӿڶ���
  			2�����ؽӿڶ���
  ��    ��:  shenglq
  ��������:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct ILoginItem : virtual public IDataItem
{
	virtual CString GetBranchNo() = 0;
	virtual CString GetFundAccount() = 0;
	virtual CString GetClientID() = 0;
	virtual CString GetClientName() = 0;
	virtual CString GetClientRights() = 0;
	virtual CString GetLoginDate() = 0;
	virtual CString GetLoginTime() = 0;
	virtual CString GetLastOpEntrustWay() = 0;
	virtual CString GetLastOpIp() = 0;
	virtual CString GetTabconfirmFlag() = 0;
	virtual CString GetInitDate() = 0;
	virtual CString GetLastDate() = 0;
};

struct ILoginList : virtual public IDataList
{
	virtual ILoginItem * GetItems(const int i) = 0; 
	virtual ILoginItem * NewItem() = 0;
};

struct IReqLogin : virtual public ITrading
{
	virtual void SetOpBranchNo(const CString sValue) = 0;
	virtual void SetOpStation(const CString sValue) = 0;
	virtual void SetBranchNo(const CString sValue) = 0;
	virtual void SetInputContent(const CString sValue) = 0;
	virtual void SetContentType(const CString sValue) = 0;
	virtual void SetAccountContent(const CString sValue) = 0;
	virtual ILoginList * GetDataList() = 0;
};