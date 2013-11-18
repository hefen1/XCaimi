/*******************************************************
  源程序名称:TradeLogin.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易登陆类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20100720
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