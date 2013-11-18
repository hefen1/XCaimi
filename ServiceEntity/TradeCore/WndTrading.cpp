/*******************************************************
  Դ��������:WndTrading.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ��������
  ����˵��:  ͬ����������ʵ��
  ��    ��:  shenglq
  ��������:  20101025
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