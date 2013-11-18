/*******************************************************
  源程序名称:TradeDataCore.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据数据中心
        1、订阅、取消订阅数据类定义
  		2、读取、设置数据类定义 
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradeDataCoreIntf.h"

class CTradeDataCore : public ITradeDataCore
{
public:	
	CTradeDataCore();
	~CTradeDataCore();
	//释放
	void Release();
	//订阅数据
	void Subscribe(ITradeDataCallBack * pOwne);
	//取消订阅
	void CancelSubscribe(ITradeDataCallBack * pOwner);
	//读取、设置资金
	IFundList * GetFund();
	void SetFund(IFundList * pDataList);
	//读取、设置股票持仓
	IStockHoldList * GetStockHold();
	void SetStockHold(IStockHoldList * pDataList);
	//读取、设置股票委托
	IStockEntrustList * GetStockEntrust();
	void SetStockEntrust(IStockEntrustList * pDataList);
	//读取、设置股票成交
	IStockDealList * GetStockDeal();
	void SetStockDeal(IStockDealList * pDataList);

	//读取、设置基金持仓
	IFundHoldList * GetFundHold(void);
	void SetFundHold(IFundHoldList * pDataList);
	//读取、设置基金委托
	IFundEntrustList * GetFundEntrust(void);
	void SetFundEntrust(IFundEntrustList * pDataList);
private:
	IFundList* m_pFundList;
	IStockHoldList* m_pStockHoldList;
	IStockEntrustList* m_pStockEntrustList;
	IStockDealList* m_pStockDealList;
	CArray<ITradeDataCallBack*, ITradeDataCallBack*> m_SubscribeList;
	IFundHoldList* m_pFundHoldList;
	IFundEntrustList* m_pFundEntrustList;
};