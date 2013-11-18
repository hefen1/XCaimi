/*******************************************************
  源程序名称:TradeStockZJHQIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券直接还券接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockZJHQ : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//委托数量
	virtual void SetEntrustAmount(const int Value) = 0;
};