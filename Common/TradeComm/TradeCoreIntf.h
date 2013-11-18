/*******************************************************
  源程序名称:TradeCoreIntf.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/
#pragma once
#include "TradeAccountMngIntf.h"
#include "TradingIntf.h"
#include "TradeDataBaseIntf.h"
#include "ReqHeartIntf.h"
#include "TradeTablesIntf.h"

#define OBJ_TRADECORE   _T("TradeCore.TradeCoreObj")

struct ITradeTcpView
{
	virtual void CallBackData(char* pBuf, UINT nLen, long nConnectHandle, int nAction, int nType) = 0;
};

struct ITradeCoreObj
{
	//释放
	virtual void Release() = 0;
	virtual void FreeCache() = 0;
	virtual IAccountMng* GetAccountMng() = 0;
	virtual ITrading* NewTradingByAction(const int iAction) = 0;
	virtual IReqHeart* NewReqHeart() = 0;
	virtual ITradeTable* NewColumsByAtionEx(const int iAction) = 0;
	virtual void ThreadSync(ITrading* pTrading) = 0;
	virtual void AccountStatus(IAccount* pAccount) = 0;
	virtual void SetTradeTcpView(ITradeTcpView* pTradeTcpView = NULL) = 0;
	virtual ITradeTcpView* GetTradeTcpView() = 0;
};