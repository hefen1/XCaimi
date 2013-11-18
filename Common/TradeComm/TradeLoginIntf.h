/*******************************************************
  源程序名称:TradeLoginIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易登陆接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20100720
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