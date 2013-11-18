// Transaction.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include ".\Transaction.h"
#include ".\GeneralEntrust.h"
#include ".\EntrustList.h"
#include ".\ShareHolderList.h"
#include ".\WinnerStatusBar.h"
#include ".\QuoteReport.h"
#include "..\..\Common\HSWinner.h"

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


	if(_tcscmp(name,_T("WndGeneralEntrust")) == 0)
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
	return pBizWnd;
}
