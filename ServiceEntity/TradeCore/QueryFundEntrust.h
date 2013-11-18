/*******************************************************
  源程序名称:QueryFundEntrust.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金当日委托类
  			1、请求类定义
  			2、返回类定义
  作    者:  shenglq
  开发日期:  20110222
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryFundEntrustIntf.h"
#pragma warning (disable:4250)


class CFundEntrustItem : public CDataItem, public IFundEntrustItem
{
public:	
	//基金公司
	CString GetFundCompany();
	//基金代码
	CString GetFundCode();
	//委托编号
	CString GetEntrustNo();
	//状态
	CString GetEntrustStatus();
	//申购日期
	CString GetAllotDate();
	//申购编号
	CString GetAllotNo();
	//定位串
	CString GetPositionStr();
};

class CFundEntrustList : public CDataList, public IFundEntrustList
{
public:
	IFundEntrustItem * GetItems(const int i);
	IFundEntrustItem * NewItem();
protected:
private:
};


class CReqQueryFundEntrust : public CTrading, public IReqQueryFundEntrust
{
public:	
	//基金公司
	void SetFundCompany(const CString sValue);
	//基金代码
	void SetFundCode(const CString sValue);
	//定位串
	void SetPositionStr(const CString sValue);
	IFundEntrustList* GetDataList();
protected:
	IFundEntrustList* NewDataList();
};