/*******************************************************
  源程序名称:QueryFundInfoIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金信息接口
  			1、请求接口定义
  			2、返回接口定义
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#pragma once
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"

struct IFundInfoItem : virtual public IDataItem
{
	//基金公司
	virtual CString GetFundCompany() = 0;
	//基金代码
	virtual CString GetFundCode() = 0;
	//基金名称
	virtual CString GetFundName() = 0;
	//净值
	virtual CString GetNav() = 0;
	//状态
	virtual CString GetStatus() = 0;
	//收费类别
	virtual CString GetChargeType() = 0;
	//风险等级
	virtual CString GetFundRisklevel() = 0;
	//风险等级名称
	virtual CString GetFundRisklevelName() = 0;
	//定位串
	virtual CString GetPositionStr() = 0;
};
//////////////////////////////////////////////////////////////////////////

struct IFundInfoList : virtual public IDataList
{
	virtual IFundInfoItem * GetItems(const int i) = 0; 
	virtual IFundInfoItem * NewItem() = 0;
	virtual IFundInfoItem * GetByFundCode(CString sFundCode) = 0;
};

//////////////////////////////////////////////////////////////////////////

struct IReqQueryFundInfo : virtual public ITrading
{
	//基金公司
	virtual void SetFundCompany(const CString sValue) = 0;
	//基金代码
	virtual void SetFundCode(const CString sValue) = 0;
	//定位串
	virtual void SetPositionStr(const CString sValue) = 0;
	virtual IFundInfoList* GetDataList() = 0;
};