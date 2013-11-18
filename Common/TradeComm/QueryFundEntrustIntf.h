/*******************************************************
  源程序名称:QueryFundEntrustIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金当日委托接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20110222
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundEntrustItem : virtual public IDataItem
{
	//基金公司
	virtual CString GetFundCompany() = 0;
	//基金代码
	virtual CString GetFundCode() = 0;
	//委托编号
	virtual CString GetEntrustNo() = 0;
	//状态
	virtual CString GetEntrustStatus() = 0;
	//申购日期
	virtual CString GetAllotDate() = 0;
	//申购编号
	virtual CString GetAllotNo() = 0;
	//定位串
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundEntrustList : virtual public IDataList
{
	virtual IFundEntrustItem * GetItems(const int i) = 0; 
	virtual IFundEntrustItem * NewItem() = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundEntrust : virtual public ITrading
{
	//基金公司
	virtual void SetFundCompany(const CString sValue) = 0;
	//基金代码
	virtual void SetFundCode(const CString sValue) = 0;
	//定位串
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundEntrustList* GetDataList() = 0;
};