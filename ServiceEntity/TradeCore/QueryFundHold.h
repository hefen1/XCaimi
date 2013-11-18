/*******************************************************
  源程序名称:QueryFundHold.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金份额类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20110222
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundHoldIntf.h"
#pragma warning (disable:4250)


class CFundHoldItem : public CDataItem, public IFundHoldItem
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
	//申购日期
	CString GetAllotDate();
	//申购编号
	CString GetAllotNo();
	//基金可用数量
	CString GetEnableShares();
	//定位串
	CString GetPositionStr();
};

class CFundHoldList : public CDataList, public IFundHoldList
{
public:
	IFundHoldItem * GetItems(const int i);
	IFundHoldItem * NewItem();
	IFundHoldItem * GetByFundCode(CString sFundCode);
protected:
private:
};


class CReqQueryFundHold : public CTrading, public IReqQueryFundHold
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//基金代码
	void SetFundCode(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IFundHoldList* GetDataList();
protected:
	IFundHoldList* NewDataList();
};