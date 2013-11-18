// TradeCore.h : main header file for the TradeCore DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "TradeCoreIntf.h"
#include "TradeConfigObjIntf.h"
#include "WndTrading.h"


// CTradeCoreApp
// See TradeCore.cpp for the implementation of this class
//

class CTradeCoreApp : public CWinApp
{
public:
	CTradeCoreApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

class CTradeCoreObj : public ITradeCoreObj
{
public:
	CTradeCoreObj();
	~CTradeCoreObj();
	void Release();
	void FreeCache();
	IAccountMng*	GetAccountMng();
	ITrading*		NewTradingByAction(const int iAction);
	IReqHeart*		NewReqHeart();
	ITradeTable*	NewColumsByAtionEx(const int iAction);
	void			SetTradeTcpView(ITradeTcpView* pTradeTcpView /* = NULL */);
	ITradeTcpView*	GetTradeTcpView();
public:
	void ThreadSync(ITrading* pTrading);
	void AccountStatus(IAccount* pAccount);
public:
	CWndTrading*		m_pWndTrading;
private:
	IAccountMng*		m_AccountMng;
	ITradeConfigObj*	m_pTradeConfigObj;
	ITradeTcpView*		m_pTradeTcpView;
};
