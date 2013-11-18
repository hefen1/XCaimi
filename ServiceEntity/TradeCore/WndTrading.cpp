/*******************************************************
  源程序名称:WndTrading.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  同步请求窗体类实现
  作    者:  shenglq
  开发日期:  20101025
*********************************************************/
#include "stdafx.h"
#include "WndTrading.h"
#include "Trading.h"
#include "TradeCoreIntf.h"
#include <afximpl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

extern ITradeCoreObj* pTradeCoreObj;

IMPLEMENT_DYNAMIC(CWndTrading, CWnd)

CWndTrading::CWndTrading()
{
}

CWndTrading::~CWndTrading()
{
}


BEGIN_MESSAGE_MAP(CWndTrading, CWnd)
	ON_MESSAGE(WM_TRADE_THREAD_ASYNC, OnThreadAsync)
	ON_MESSAGE(WM_ACCOUNT_STATUS, OnAccountStatus)
END_MESSAGE_MAP()

BOOL CWndTrading::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{

	//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return CreateEx(0, lpszClassName, lpszWindowName,
		dwStyle,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, (HMENU)(UINT_PTR)nID, (LPVOID)pContext);
}

BOOL CWndTrading::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	if (cs.lpszClass == NULL)
	{
		// make sure the default window class is registered
		VERIFY(AfxDeferRegisterClass(AFX_WND_REG));

		// no WNDCLASS provided - use child window default
		cs.lpszClass = AFX_WND;
	}
	return TRUE;
}

LRESULT CWndTrading::OnThreadAsync(WPARAM wParam, LPARAM lParam)
{
	ITrading* pTrading = (ITrading*)lParam;
	pTrading->CallBack();
	return 1;
}

LRESULT CWndTrading::OnAccountStatus(WPARAM wParam, LPARAM lParam)
{
	IAccount* pAccount = (IAccount*)lParam;
	pTradeCoreObj->GetAccountMng()->CallAccountStatus(pAccount);
	return 1;
}