/*******************************************************
  源程序名称:TradeStockZJHKIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券直接还款接口定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqStockZJHK : virtual public ITrading
{
	//币种
	virtual void SetMoneyType(const CString sValue) = 0;
	//委托金额
	virtual void SetOccurBalance(const double Value) = 0;
};