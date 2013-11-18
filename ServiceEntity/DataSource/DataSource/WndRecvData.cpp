// WndRecvData.cpp : implementation file
//

#include "stdafx.h"
#include "DataSource.h"
#include "WndRecvData.h"
#include "DataSourceImpl.h"
#include <afximpl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 

// CWndRecvData

IMPLEMENT_DYNAMIC(CWndRecvData, CWnd)

CWndRecvData::CWndRecvData(CDataSourceImpl* pDataSourceImpl)
{
	m_pDataSourceImpl = pDataSourceImpl;
}

CWndRecvData::~CWndRecvData()
{
}


BEGIN_MESSAGE_MAP(CWndRecvData, CWnd)
	ON_MESSAGE(WM_THREAD_ASYNC, OnThreadAsync)
	ON_MESSAGE(WM_THREAD_STATUS, OnStatusNotify)
	ON_WM_COPYDATA()
	
	ON_WM_CLOSE()
END_MESSAGE_MAP()



// CWndRecvData message handlers



BOOL CWndRecvData::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{

//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	return CreateEx(0, lpszClassName, lpszWindowName,
		dwStyle,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, (HMENU)(UINT_PTR)nID, (LPVOID)pContext);
}

BOOL CWndRecvData::PreCreateWindow(CREATESTRUCT& cs)
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

LRESULT CWndRecvData::OnThreadAsync(WPARAM wParam, LPARAM lParam)
{
	if (m_pDataSourceImpl != NULL)
	{
//		TRACE("Thread Ansyc , time tick = %ld\n", GetTickCount());
		return m_pDataSourceImpl->OnAsyncNotify(wParam, lParam);
	}
	return 1;
}

LRESULT CWndRecvData::OnStatusNotify(WPARAM wParam, LPARAM lParam)
{
	if (m_pDataSourceImpl != NULL)
	{
		return m_pDataSourceImpl->OnStatusNotify(wParam, lParam);
	}
	return 1;
}

void CWndRecvData::OnClose()
{
	DestroyWindow();
}
