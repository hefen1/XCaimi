/*******************************************************
  源程序名称:QueryStockFundInfo.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  查询基金盘后业务信息接口
  			1、请求类定义
  			2、返回类口定义
  作    者:  shenglq
  开发日期:  20110328
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeDataBase.h"
#include "QueryStockFundInfoIntf.h"
#pragma warning (disable:4250)


class CStockFundInfoItem : public CDataItem, public IStockFundInfoItem
{
public:	
	//委托编号
    CString GetStockCode();
	//市场
	CString GetExchangeType();
	//股东账号
	CString GetStockAccount();
	//基金状态
	CString GetFundStatus();
	//转换状态
	CString GetTransStatus();
};

class CStockFundInfoList : public CDataList, public IStockFundInfoList
{
public:
	IStockFundInfoItem * GetItems(const int i); 
	IStockFundInfoItem * NewItem();
protected:
private:	
};



class CReqQueryStockFundInfo : public CTrading, public IReqQueryStockFundInfo
{
public:	
	//证券代码
	void SetStockCode(const CString sValue);
	//市场
	void SetExchangeType(const CString sValue);
	IStockFundInfoList* GetDataList();
protected:
	IStockFundInfoList* NewDataList();
};