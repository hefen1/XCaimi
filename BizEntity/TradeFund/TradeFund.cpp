/*******************************************************
  源程序名称:TradeFund.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  基金交易
  功能说明:  
  作    者:  shenglq
  开发日期:  20110215
*********************************************************/
// TradeFund.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TradeFund.h"
#include "HSWinner.h"
#include "TradeFundSubcribeDlg.h"
#include "TradeFundAllotDlg.h"
#include "TradeFundRedeemDlg.h"
#include "TradeAction.h"
#include "QueryHoldDlg.h"
#include "QueryEntrustDlg.h"
#include "QueryFundInfoDlg.h"
#include "TradeSetDividendDlg.h"
#include "TradeFundTransDlg.h"
#include "FundRiskTestDlg.h"
#include "TradeFundHSOpenDlg.h"
#include "TradeFundHopeAllotDlg.h"
#include "TradeFundHopeRedeemDlg.h"
#include "TradeFund7432Dlg.h"
#include "Query7431Dlg.h"

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

// CTradeFundApp

BEGIN_MESSAGE_MAP(CTradeFundApp, CWinApp)
END_MESSAGE_MAP()


// CTradeFundApp construction

CTradeFundApp::CTradeFundApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTradeFundApp object

CTradeFundApp theApp;


// CTradeFundApp initialization

BOOL CTradeFundApp::InitInstance()
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
	if(_tcscmp(name,_T("WndFundSubcribe")) == 0)
	{
		CTradeFundSubcribeDlg *pDlg = new CTradeFundSubcribeDlg();
		pDlg->Create(CTradeFundSubcribeDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_SUBSCRIBE);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndFundAllot")) == 0)
	{
		CTradeFundAllotDlg *pDlg = new CTradeFundAllotDlg();
		pDlg->Create(CTradeFundAllotDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_ALLOT);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndFundRedeem")) == 0)
	{
		CTradeFundRedeemDlg *pDlg = new CTradeFundRedeemDlg();
		pDlg->Create(CTradeFundRedeemDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_REDEEM);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryFundHold")) == 0)
	{
		CQueryHoldDlg *pDlg = new CQueryHoldDlg();
		pDlg->Create(CQueryHoldDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_QUERYHOLD);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryFundEntrust")) == 0)
	{
		CQueryEntrustDlg *pDlg = new CQueryEntrustDlg();
		pDlg->Create(CQueryEntrustDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_QUERYENTRUST);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryFundInfo")) == 0)
	{
		CQueryFundInfoDlg *pDlg = new CQueryFundInfoDlg();
		pDlg->Create(CQueryFundInfoDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_QUERYFUNDINFO);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndSetDividend")) == 0)
	{
		CTradeSetDividendDlg *pDlg = new CTradeSetDividendDlg();
		pDlg->Create(CTradeSetDividendDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_SETDIVIDEND);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndFundTrans")) == 0)
	{
		CTradeFundTransDlg *pDlg = new CTradeFundTransDlg();
		pDlg->Create(CTradeFundTransDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_TRANS);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndFundRiskTest")) == 0)
	{
		CFundRiskTestDlg *pDlg = new CFundRiskTestDlg();
		pDlg->Create(CFundRiskTestDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_TRANS);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndFundHSOpen")) == 0)
	{
		CTradeFundHSOpenDlg *pDlg = new CTradeFundHSOpenDlg();
		pDlg->Create(CTradeFundHSOpenDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_OPEN);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndFundHopeAllot")) == 0)
	{
		CTradeFundHopeAllotDlg *pDlg = new CTradeFundHopeAllotDlg();
		pDlg->Create(CTradeFundHopeAllotDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_HOPEALLOT);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndFundHopeRedeem")) == 0)
	{
		CTradeFundHopeRedeemDlg *pDlg = new CTradeFundHopeRedeemDlg();
		pDlg->Create(CTradeFundHopeRedeemDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_HOPEREDEEM);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndFund7432")) == 0)
	{
		CTradeFund7432Dlg *pDlg = new CTradeFund7432Dlg();
		pDlg->Create(CTradeFund7432Dlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_7432);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name,_T("WndQuery7431")) == 0)
	{
		CQuery7431Dlg *pDlg = new CQuery7431Dlg();
		pDlg->Create(CQuery7431Dlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_FUND_7431);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}
