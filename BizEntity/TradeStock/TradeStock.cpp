/*******************************************************
  源程序名称:TradeStock.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  证券交易
  功能说明:  
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
// TradeStock.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TradeStock.h"
#include "HSWinner.h"
#include "TradeStockBuyDlg.h"
#include "TradeStockSellDlg.h"
#include "TradeQueryBaseDlg.h"
#include "TradeAction.h"
#include "QueryHoldDlg.h"
#include "QueryEntrustDlg.h"
#include "QueryDealDlg.h"
#include "TradeStockSJBuyDlg.h"
#include "TradeStockSJSellDlg.h"
#include "TradeStockAllotDlg.h"
#include "TradeStockRedeemDlg.h"
#include "TradeStockSubcribeDlg.h"
#include "TradeZQToStockDlg.h"
#include "TradeZQHSDlg.h"
#include "TradeWarrantEntrustDlg.h"
#include "TradeStockSetCostPriceDlg.h"
#include "TradeStockSBBuyADlg.h"
#include "TradeStockSBBuyBDlg.h"
#include "TradeStockSBBuyCDlg.h"
#include "TradeStockSBSellADlg.h"
#include "TradeStockSBSellBDlg.h"
#include "TradeStockSBSellCDlg.h"
#include "QuerySBQuoteDlg.h"
#include "QuerySBEntrustDlg.h"
#include "TradeStockFundMergeSplitDlg.h"
#include "TradeStockFundTransDlg.h"
#include "QueryStockFundInfoDlg.h"
#include "QueryStockFundEntrustDlg.h"
#include "TradeRZBuyDlg.h"
#include "TradeRQSellDlg.h"
#include "TradeMQHQDlg.h"
#include "TradeMQHKDlg.h"
#include "TradeZJHKDlg.h"
#include "TradeZJHQDlg.h"
#include "TradeStock9835Dlg.h"
#include "TradeStock9837Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
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

// CTradeStockApp

BEGIN_MESSAGE_MAP(CTradeStockApp, CWinApp)
END_MESSAGE_MAP()


// CTradeStockApp construction

CTradeStockApp::CTradeStockApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTradeStockApp object

CTradeStockApp theApp;


// CTradeStockApp initialization

BOOL CTradeStockApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pBizWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndStockBuy")) == 0)
	{
		CTradeStockBuyDlg *pDlg = new CTradeStockBuyDlg();
		pDlg->Create(CTradeStockBuyDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSell")) == 0)
	{
		CTradeStockSellDlg *pDlg = new CTradeStockSellDlg();
		pDlg->Create(CTradeStockSellDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockFund")) == 0)
	{
		CTradeQueryBaseDlg *pDlg = new CTradeQueryBaseDlg();
		pDlg->Create(CTradeQueryBaseDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_QUERYFUND);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockHold")) == 0)
	{
		CQueryHoldDlg *pDlg = new CQueryHoldDlg();
		pDlg->Create(CQueryHoldDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYHOLD);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockEntrust")) == 0)
	{
		CQueryEntrustDlg *pDlg = new CQueryEntrustDlg();
		pDlg->Create(CQueryEntrustDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockDeal")) == 0)
	{
		CQueryDealDlg *pDlg = new CQueryDealDlg();
		pDlg->Create(CQueryDealDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYDEAL);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSJBuy")) == 0)
	{
		CTradeStockSJBuyDlg *pDlg = new CTradeStockSJBuyDlg();
		pDlg->Create(CTradeStockSJBuyDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSJSell")) == 0)
	{
		CTradeStockSJSellDlg *pDlg = new CTradeStockSJSellDlg();
		pDlg->Create(CTradeStockSJSellDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSubcribe")) == 0)
	{
		CTradeStockSubcribeDlg *pDlg = new CTradeStockSubcribeDlg();
		pDlg->Create(CTradeStockSubcribeDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockAllot")) == 0)
	{
		CTradeStockAllotDlg *pDlg = new CTradeStockAllotDlg();
		pDlg->Create(CTradeStockAllotDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockRedeem")) == 0)
	{
		CTradeStockRedeemDlg *pDlg = new CTradeStockRedeemDlg();
		pDlg->Create(CTradeStockRedeemDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndZQToStock")) == 0)
	{
		CTradeZQToStockDlg *pDlg = new CTradeZQToStockDlg();
		pDlg->Create(CTradeZQToStockDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndZQHS")) == 0)
	{
		CTradeZQHSDlg *pDlg = new CTradeZQHSDlg();
		pDlg->Create(CTradeZQHSDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndWarrantEntrust")) == 0)
	{
		CTradeWarrantEntrustDlg *pDlg = new CTradeWarrantEntrustDlg();
		pDlg->Create(CTradeWarrantEntrustDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSetCostPrice")) == 0)
	{
		CTradeStockSetCostPriceDlg *pDlg = new CTradeStockSetCostPriceDlg();
		pDlg->Create(CTradeStockSetCostPriceDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SETCOSTPRICE);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBBuyA")) == 0)
	{
		CTradeStockSBBuyADlg *pDlg = new CTradeStockSBBuyADlg();
		pDlg->Create(CTradeStockSBBuyADlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBBuyB")) == 0)
	{
		CTradeStockSBBuyBDlg *pDlg = new CTradeStockSBBuyBDlg();
		pDlg->Create(CTradeStockSBBuyBDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBBuyC")) == 0)
	{
		CTradeStockSBBuyCDlg *pDlg = new CTradeStockSBBuyCDlg();
		pDlg->Create(CTradeStockSBBuyCDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBSellA")) == 0)
	{
		CTradeStockSBSellADlg *pDlg = new CTradeStockSBSellADlg();
		pDlg->Create(CTradeStockSBSellADlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBSellB")) == 0)
	{
		CTradeStockSBSellBDlg *pDlg = new CTradeStockSBSellBDlg();
		pDlg->Create(CTradeStockSBSellBDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockSBSellC")) == 0)
	{
		CTradeStockSBSellCDlg *pDlg = new CTradeStockSBSellCDlg();
		pDlg->Create(CTradeStockSBSellCDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_SBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQuerySBQuote")) == 0)
	{
		CQuerySBQuoteDlg *pDlg = new CQuerySBQuoteDlg();
		pDlg->Create(CQuerySBQuoteDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYSBQUOTE);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQuerySBEntrust")) == 0)
	{
		CQuerySBEntrustDlg *pDlg = new CQuerySBEntrustDlg();
		pDlg->Create(CQuerySBEntrustDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYSBENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockFundMerge")) == 0)
	{
		CTradeStockFundMergeSplitDlg *pDlg = new CTradeStockFundMergeSplitDlg();
		pDlg->Create(CTradeStockFundMergeSplitDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_FUNDMERGE);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockFundSplit")) == 0)
	{
		CTradeStockFundMergeSplitDlg *pDlg = new CTradeStockFundMergeSplitDlg();
		pDlg->Create(CTradeStockFundMergeSplitDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_FUNDSPLIT);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockFundTrans")) == 0)
	{
		CTradeStockFundTransDlg *pDlg = new CTradeStockFundTransDlg();
		pDlg->Create(CTradeStockFundTransDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_FUNDTRANS);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockFundInfo")) == 0)
	{
		CQueryStockFundInfoDlg *pDlg = new CQueryStockFundInfoDlg();
		pDlg->Create(CQueryStockFundInfoDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYFUNDINFO);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryStockFundEntrust")) == 0)
	{
		CQueryStockFundEntrustDlg *pDlg = new CQueryStockFundEntrustDlg();
		pDlg->Create(CQueryStockFundEntrustDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_QUERYFUNDENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockRZBuy")) == 0)
	{
		CTradeRZBuyDlg *pDlg = new CTradeRZBuyDlg();
		pDlg->Create(CTradeRZBuyDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockRQSell")) == 0)
	{
		CTradeRQSellDlg *pDlg = new CTradeRQSellDlg();
		pDlg->Create(CTradeRQSellDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockMQHQ")) == 0)
	{
		CTradeMQHQDlg *pDlg = new CTradeMQHQDlg();
		pDlg->Create(CTradeMQHQDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockMQHK")) == 0)
	{
		CTradeMQHKDlg *pDlg = new CTradeMQHKDlg();
		pDlg->Create(CTradeMQHKDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockZJHK")) == 0)
	{
		CTradeZJHKDlg *pDlg = new CTradeZJHKDlg();
		pDlg->Create(CTradeZJHKDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ZJHK);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockZJHQ")) == 0)
	{
		CTradeZJHQDlg *pDlg = new CTradeZJHQDlg();
		pDlg->Create(CTradeZJHQDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_RZRQ_ZJHQ);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStock9835")) == 0)
	{
		CTradeStock9835Dlg *pDlg = new CTradeStock9835Dlg();
		pDlg->Create(CTradeStock9835Dlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_9835);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStock9837")) == 0)
	{
		CTradeStock9837Dlg *pDlg = new CTradeStock9837Dlg();
		pDlg->Create(CTradeStock9837Dlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_STOCK_9837);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}
