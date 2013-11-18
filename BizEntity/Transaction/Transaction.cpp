// Transaction.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include ".\Transaction.h"
#include ".\GeneralEntrust.h"
#include ".\EntrustList.h"
#include ".\ShareHolderList.h"
#include ".\MenuTreeDlg.h"
#include ".\WinnerStatusBar.h"
#include ".\TradeToolBar.h"
#include ".\QuoteReport.h"
#include ".\TradeLockDlg.h"

#include "..\..\Common\HSWinner.h"
#include "TradeTcpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTransactionApp

BEGIN_MESSAGE_MAP(CTransactionApp, CWinApp)
END_MESSAGE_MAP()

CTransactionApp::CTransactionApp()
{
}

CTransactionApp theApp;

BOOL CTransactionApp::InitInstance()
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

//
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pBizWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);


	if(_tcscmp(name,_T("WndTradeToolBar")) == 0)
	{
		CTradeToolBar *pDlg = new CTradeToolBar();
		pDlg->Create(CTradeToolBar::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndTradeLocker")) == 0)
	{
		CTradeLockDlg *pDlg = new CTradeLockDlg();
		pDlg->Create(CTradeLockDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndGeneralEntrust")) == 0)
	{
		CGeneralEntrust *pDlg = new CGeneralEntrust();
		pDlg->Create(CGeneralEntrust::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndEntrustList")) == 0)
	{
		CEntrustList *pDlg = new CEntrustList();
		pDlg->Create(CEntrustList::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndShareHolder")) == 0)
	{
		CShareHolderList *pDlg = new CShareHolderList();
		pDlg->Create(CShareHolderList::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndMenuTree")) == 0)
	{
		CMenuTreeDlg *pDlg = new CMenuTreeDlg();
		pDlg->Create(CMenuTreeDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndStatusBar")) == 0)
	{
		CWinnerStatusBar *pDlg = new CWinnerStatusBar();
		pDlg->Create(CWinnerStatusBar::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndQuoteReport")) == 0)
	{
		CQuoteReport *pDlg = new CQuoteReport();
		pDlg->Create(CQuoteReport::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	else if(_tcscmp(name,_T("WndTcpView")) == 0)
	{
		CTcpView *pDlg = new CTcpView();
		pDlg->Create(CTcpView::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}
