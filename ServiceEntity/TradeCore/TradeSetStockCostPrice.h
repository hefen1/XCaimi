/*******************************************************
  源程序名称:TradeSetStockCostPrice.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  重置成本价类定义
  作    者:  shenglq
  开发日期:  20110317
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeSetStockCostPriceIntf.h"
#pragma warning (disable:4250)

class CReqSetStockCostPrice : public CTrading, public IReqSetStockCostPrice
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//成本价
	void SetCostPrice(const double Value);
	//委托席位
	void SetSeat(const CString sValue);
	//金额
	void SetEntrustBalance(const double Value);
	//备注
	void SetRemark(const CString sValue);
};