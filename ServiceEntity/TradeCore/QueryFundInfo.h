/*******************************************************
  源程序名称:QueryFundInfo.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金信息类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20110217
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundInfoIntf.h"
#pragma warning (disable:4250)


class CFundInfoItem : public CDataItem, public IFundInfoItem
{
public:	
	//基金公司
	CString GetFundCompany();
	//基金代码
	CString GetFundCode();
	//基金名称
	CString GetFundName();
	//净值
	CString GetNav();
	//状态
	CString GetStatus();
	//收费类别
	CString GetChargeType();
	//风险等级
	CString GetFundRisklevel();
	//风险等级名称
	CString GetFundRisklevelName();
	//定位串
	CString GetPositionStr();
};

class CFundInfoList : public CDataList, public IFundInfoList
{
public:
	IFundInfoItem * GetItems(const int i);
	IFundInfoItem * NewItem();
	IFundInfoItem * GetByFundCode(CString sFundCode);
protected:
	virtual IFundInfoList* NewDataList();
private:
};


class CReqQueryFundInfo : public CTrading, public IReqQueryFundInfo
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//基金代码
	void SetFundCode(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IFundInfoList* GetDataList();
protected:
	IFundInfoList* NewDataList();
};