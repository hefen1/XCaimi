// TradeCore.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HSWinner.h"
#include "TradeCore.h"
#include "TradeAccountMng.h"
#include "TradeStockWithDraw.h"
#include "TradeStockEntrust.h"
#include "QueryFund.h"
#include "TradeLogin.h"
#include "QueryStockAccount.h"
#include "TradeDataBase.h"
#include "QueryBase.h"
#include "QueryStockEntrust.h"
#include "QueryStockDeal.h"
#include "QueryStockHold.h"
#include "ReqHeart.h"
#include "QueryStockMaxBuy.h"
#include "QueryStockMaxSell.h"
#include "TradeAction.h"
#include "WinnerApplication.h"
#include "TradeFundEntrust.h"
#include "QueryFundInfo.h"
#include "QueryFundHold.h"
#include "QueryFundEntrust.h"
#include "QueryBank.h"
#include "BankStockTransfer.h"
#include "ChangePassword.h"
#include "TradeSetStockCostPrice.h"
#include "QueryUserInfo.h"
#include "ModifyUserInfo.h"
#include "QueryStockFundInfo.h"
#include "TradeStockZJHK.h"
#include "TradeStockZJHQ.h"
#include "QueryFundStockAccount.h"
#include "QueryFundCompany.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma   data_seg( "._tradecore_data") 
ITradeCoreObj*	pTradeCoreObj = NULL; 
#pragma   data_seg() 

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTradeCoreApp

BEGIN_MESSAGE_MAP(CTradeCoreApp, CWinApp)
END_MESSAGE_MAP()


// CTradeCoreApp construction

CTradeCoreApp::CTradeCoreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTradeCoreApp object

CTradeCoreApp theApp;


// CTradeCoreApp initialization

BOOL CTradeCoreApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


ITradeCoreObj* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	return pTradeCoreObj;
}

int PASCAL InitEntity()
{
	if (!pTradeCoreObj)
	{
		pTradeCoreObj = new CTradeCoreObj;
	}
	return TRUE;
}

int PASCAL UninitEntity()
{
	pTradeCoreObj->Release();
	pTradeCoreObj = NULL;
	return TRUE;
}

int PASCAL OnFrameNotify(int nFlag)
{
	if (SN_BEFORE_FRAME_CLOSE == nFlag)
	{
		pTradeCoreObj->FreeCache();
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
 CTradeCoreObj::CTradeCoreObj()
{
	m_AccountMng = new CAccountMng;
	m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_pWndTrading = NULL;
	m_pTradeTcpView = NULL;
}

CTradeCoreObj::~CTradeCoreObj()
{
	/*if (m_pWndTrading != NULL && m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd))
	{
		m_pWndTrading->SendMessage(WM_CLOSE);
	}
	if (m_pWndTrading != NULL )
	{
		int nCount = 0;
		while(m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd) && nCount ++ < 1000)
		{
			Sleep(10);
		}
		delete m_pWndTrading;
		m_pWndTrading = NULL;
	}*/
}

void CTradeCoreObj::Release()
{
	m_AccountMng->Release();
	delete this;
}

IAccountMng* CTradeCoreObj::GetAccountMng()
{
	if (!m_pWndTrading)
	{
		m_pWndTrading = new CWndTrading();
		if (m_pWndTrading)
		{
			if (m_pWndTrading->m_hWnd == NULL || !::IsWindow(m_pWndTrading->m_hWnd))
			{
				BOOL bCreated = m_pWndTrading->Create(NULL, _T("TradingWnd"), WS_OVERLAPPED | WS_EX_TOOLWINDOW, CRect(0, 0, 0, 0), NULL, 0);
				if(bCreated == FALSE)
				{
					OutputDebugString("Create WndTrading window failed.\n");
				}
			}
		}
	}	
	return m_AccountMng;
}

ITrading* CTradeCoreObj::NewTradingByAction(const int iAction)
{
	ITrading* pTmp = NULL;
	switch(iAction)
	{
	case TRDADE_LOGIN://登陆
		{
			pTmp = new CReqLogin;
			break;
		}
	case TRADE_STOCK_QUERYMAXSELL://最大可卖
	case TRADE_STOCK_QUERYMERGESPLIT://可合并拆分数量
	case TRADE_RZRQ_QUERYRQAMOUNT:
		{
			pTmp = new CReqQueryStockMaxSell;
			break;
		}
	case TRADE_STOCK_QUERYMAXBUY://最大可买
	case TRADE_RZRQ_QUERYMAXBUY://
	case TRADE_RZRQ_QUERYMAXSELL:
		{
			pTmp = new CReqQueryStockMaxBuy;
			break;
		}
	case TRADE_STOCK_ENTRUST://证券委托
	case TRADE_STOCK_SBENTRUST://三板委托
	case TRADE_STOCK_FUNDMERGE://基金盘后业务合并
	case TRADE_STOCK_FUNDSPLIT://基金盘后业务拆分
	case TRADE_STOCK_FUNDTRANS://基金盘后业务转换
	case TRADE_RZRQ_ENTRUST://融资融券委托
	case TRADE_STOCK_9835://ETF网下现金认购
	case TRADE_STOCK_9837://ETF网下股份换购
		{
			pTmp = new CReqStockEntrust;
			break;
		}
	case TRADE_STOCK_WITHDRAW://撤单
	case TRADE_STOCK_SBWITHDRAW://三板撤单
	case TRADE_STOCK_FUNDWITHDRAW://基金盘后业务撤单
		{
			pTmp = new CReqStockWithDraw;
			break;
		}
	case TRADE_QUERYFUND://查询资金
	case TRADE_RZRQ_QUERYDEBITTOTAL://融资融券负债汇总
		{
			pTmp = new CReqQueryFund;
			break;
		}
	case TRADE_STOCK_QUERYACCOUNT://查询股东账号
		{
			pTmp = new CReqQueryStockAccount;
			break;
		}
	case TRADE_STOCK_QUERYENTRUST://证券当日委托
	case TRADE_STOCK_303://可撤委托
	case TRADE_STOCK_QUERYSBENTRUST://三板当日委托
	case TRADE_STOCK_QUERYFUNDENTRUST://基金盘后业务当日委托
		{
			pTmp = new CReqQueryStockEntrust;
			break;
		}
	case TRADE_STOCK_QUERYDEAL://证券当日成交
		{
			pTmp = new CReqQueryStockDeal;
			break;
		}
	case TRADE_STOCK_QUERYHOLD://证券持仓
		{
			pTmp = new CReqQueryStockHold;
			break;
		}
	case TRADE_FUND_QUERYFUNDINFO://查询基金信息
		{
			pTmp = new CReqQueryFundInfo;
			break;
		}
	case TRADE_FUND_SUBSCRIBE://基金认购
	case TRADE_FUND_ALLOT://基金申购
	case TRADE_FUND_REDEEM://基金赎回
	case TRADE_FUND_WITHDRAW://基金撤单
	case TRADE_FUND_SETDIVIDEND://设置分红方式
	case TRADE_FUND_TRANS://基金转换
	case TRADE_FUND_SETRISKLEVEL://设置风险等级
	case TRADE_FUND_OPEN://基金开户
	case TRADE_FUND_HOPEALLOT://基金预约申购
	case TRADE_FUND_HOPEREDEEM://基金预约赎回
	case TRADE_FUND_7434://基金定投撤单
	case TRADE_FUND_7432://基金定投
		{
			pTmp = new CReqFundEntrust;
			break;
		}
	case TRADE_FUND_QUERYHOLD://查询基金持仓
		{
			pTmp = new CReqQueryFundHold;
			break;
		}
	case TRADE_FUND_QUERYENTRUST://查询基金当日委托
	case TRADE_FUND_7431://查询基金定投
		{
			pTmp = new CReqQueryFundEntrust;
			break;
		}
	case TRADE_QUERYBANK://查询银行信息
		{
			pTmp = new CReqQueryBank;
			break;
		}
	case TRADE_BANKTRANSFER://银证转帐
	case TRADE_QUERYBANKBALANCE://查询银行余额
		{
			pTmp = new CReqBankStockTransfer;
			break;
		}
	case TRADE_CHANGEPASSWORD://修改密码
		{
			pTmp = new CReqChangePassword;
			break;
		}
	case TRADE_STOCK_SETCOSTPRICE://重置成本价
		{
			pTmp = new CReqSetStockCostPrice;
			break;;
		}
	case TRADE_QUERYUSERINFO://查询用户信息
		{
			pTmp = new CReqQueryUserInfo;
			break;
		}
	case TRADE_MODIFYUSERINFO://修改用户信息
		{
			pTmp = new CReqModifyUserInfo;
			break;
		}
	case TRADE_STOCK_QUERYFUNDINFO://查询基金盘后业务信息
		{
			pTmp = new CReqQueryStockFundInfo;
			break;
		}
	case TRADE_RZRQ_ZJHK://直接还款
		{
			pTmp = new CReqStockZJHK;
			break;
		}
	case TRADE_RZRQ_ZJHQ://直接还券
		{
			pTmp = new CReqStockZJHQ;
			break;
		}
	case TRADE_FUND_QUERYSTOCKACCOUNT://基金账户查询
		{
			pTmp = new CReqQueryFundStockAccount;
			break;
		}
	case TRADE_FUND_QUERYFUNDCOMPANY://基金公司查询
		{
			pTmp = new CReqQueryFundCompany;
			break;
		}
	default:
		pTmp = new CReqQueryBase;
	}
	pTmp->SetAction(iAction);
	return pTmp;
}


IReqHeart* CTradeCoreObj::NewReqHeart()
{
	IReqHeart* pTmp = new CReqHeart;
	pTmp->SetAction(TRADE_HEART);
	return pTmp;
}

void CTradeCoreObj::FreeCache()
{
	m_AccountMng->FreeCacheAll();
	if (m_pWndTrading != NULL && m_pWndTrading->m_hWnd != NULL && ::IsWindow(m_pWndTrading->m_hWnd))
	{
		m_pWndTrading->SendMessage(WM_CLOSE);
	}
	if (m_pWndTrading != NULL )
	{
		int nCount = 0;
		delete m_pWndTrading;
		m_pWndTrading = NULL;
	}
}

ITradeTable* CTradeCoreObj::NewColumsByAtionEx(const int iAction)
{
	if (m_pTradeConfigObj)
	{
		return m_pTradeConfigObj->GetTradeTableData(iAction);
	}
	else
		return NULL;

}

void CTradeCoreObj::ThreadSync(ITrading* pTrading)
{
	m_pWndTrading->SendMessage(WM_TRADE_THREAD_ASYNC, 0, (LPARAM)pTrading);
}

void CTradeCoreObj::AccountStatus(IAccount* pAccount)
{
	
	m_pWndTrading->SendMessage(WM_ACCOUNT_STATUS, 0, (LPARAM)pAccount);
}

void CTradeCoreObj::SetTradeTcpView(ITradeTcpView* pTradeTcpView )
{
	m_pTradeTcpView = pTradeTcpView;
}

ITradeTcpView* CTradeCoreObj::GetTradeTcpView()
{
	return m_pTradeTcpView;
}