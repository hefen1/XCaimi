/*******************************************************
  源程序名称:TradeCommAction.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易公共功能
  功能说明:  
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
// TradeCommAction.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TradeCommAction.h"
#include "HSWinner.h"
#include "TradeCommQueryDlg.h"
#include "TradeAction.h"
#include "TradeQueryWhereDlg.h"
#include "BankToStockDlg.h"
#include "StockToBankDlg.h"
#include "QueryBankBalanceDlg.h"
#include "ChangePasswordDlg.h"
#include "ModifyUserInfoDlg.h"

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

// CTradeCommActionApp

BEGIN_MESSAGE_MAP(CTradeCommActionApp, CWinApp)
END_MESSAGE_MAP()


// CTradeCommActionApp construction

CTradeCommActionApp::CTradeCommActionApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTradeCommActionApp object

CTradeCommActionApp theApp;


// CTradeCommActionApp initialization

BOOL CTradeCommActionApp::InitInstance()
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
	if(_tcscmp(name,_T("WndQueryStockFund")) == 0)
	{
		TradeCommQueryDlg *pDlg = new TradeCommQueryDlg();
		pDlg->Create(TradeCommQueryDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_QUERYFUND);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryCommWhere")) == 0)
	{
		CTradeQueryWhereDlg *pDlg = new CTradeQueryWhereDlg();
		pDlg->Create(CTradeQueryWhereDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryComm")) == 0)
	{
		TradeCommQueryDlg *pDlg = new TradeCommQueryDlg();
		pDlg->Create(TradeCommQueryDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndBankToStock")) == 0)
	{
		CBankToStockDlg *pDlg = new CBankToStockDlg();
		pDlg->Create(CBankToStockDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_BANKTRANSFER);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStockToBank")) == 0)
	{
		CStockToBankDlg *pDlg = new CStockToBankDlg();
		pDlg->Create(CStockToBankDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_BANKTRANSFER);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQueryBankBalance")) == 0)
	{
		CQueryBankBalanceDlg *pDlg = new CQueryBankBalanceDlg();
		pDlg->Create(CQueryBankBalanceDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_BANKTRANSFER);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndChangePassword")) == 0)
	{
		CChangePasswordDlg *pDlg = new CChangePasswordDlg();
		pDlg->Create(CChangePasswordDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_CHANGEPASSWORD);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndModifyUserInfo")) == 0)
	{
		CModifyUserInfoDlg *pDlg = new CModifyUserInfoDlg();
		pDlg->Create(CModifyUserInfoDlg::IDD,pParentWnd);
		pDlg->SetAction(TRADE_MODIFYUSERINFO);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}
