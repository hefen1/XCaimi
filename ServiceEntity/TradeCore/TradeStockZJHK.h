/*******************************************************
  源程序名称:TradeStockZJHK.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券直接还款类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockZJHKIntf.h"
#pragma warning (disable:4250)

class CReqStockZJHK : public CTrading, public IReqStockZJHK
{
public:	
	//币种
	void SetMoneyType(const CString sValue);
	//委托金额
	void SetOccurBalance(const double Value);
};