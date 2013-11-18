/*******************************************************
  源程序名称:TradeStockWithDraw.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券撤单接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockWithDrawIntf.h"
#pragma warning (disable:4250)

class CReqStockWithDraw : public CTrading, public IReqStockWithDraw
{
public:
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//委托编号
	void SetEntrustNo(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue); 
};