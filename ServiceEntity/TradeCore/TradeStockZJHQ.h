/*******************************************************
  源程序名称:TradeStockZJHQ.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  证券直接还券类定义
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "Trading.h"
#include "TradeStockZJHQIntf.h"
#pragma warning (disable:4250)

class CReqStockZJHQ : public CTrading, public IReqStockZJHQ
{
public:	
	//市场
	void SetExchangeType(const CString sValue);
	//股东账号
	void SetStockAccount(const CString sValue);
	//证券代码
	void SetStockCode(const CString sValue);
	//委托数量
	void SetEntrustAmount(const int Value);
};