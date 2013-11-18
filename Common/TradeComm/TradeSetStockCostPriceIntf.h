/*******************************************************
  源程序名称:TradeSetStockCostPriceIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  重置成本价接口定义
  作    者:  shenglq
  开发日期:  20110317
*********************************************************/
#pragma once
#include "TradingIntf.h"

struct IReqSetStockCostPrice : virtual public ITrading
{
	//市场
	virtual void SetExchangeType(const CString sValue) = 0;
	//股东账号
	virtual void SetStockAccount(const CString sValue) = 0;
	//证券代码
	virtual void SetStockCode(const CString sValue) = 0;
	//成本价
	virtual void SetCostPrice(const double Value) = 0;
	//委托席位
	virtual void SetSeat(const CString sValue) = 0;
	//金额
	virtual void SetEntrustBalance(const double Value) = 0;
	//备注
	virtual void SetRemark(const CString sValue) = 0;
};