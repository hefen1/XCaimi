/*******************************************************
  Դ��������:TradeCoreIntf.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100720
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
	//�ͷ�
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