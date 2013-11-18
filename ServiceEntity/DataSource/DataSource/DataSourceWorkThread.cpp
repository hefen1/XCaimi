// DataSourceWorkThread.cpp : implementation file
//

#include "stdafx.h"
#include "DataSource.h"
#include "DataSourceWorkThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif 
// CDataSourceWorkThread

IMPLEMENT_DYNCREATE(CDataSourceWorkThread, CWinThread)

CDataSourceWorkThread::CDataSourceWorkThread()
{
	m_bAutoDelete = FALSE;
	m_bIsBusy = FALSE;
	m_pDataSourceImpl = NULL;
}

CDataSourceWorkThread::~CDataSourceWorkThread()
{
}

BOOL CDataSourceWorkThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CDataSourceWorkThread::ExitInstance()
{
	m_bIsBusy = TRUE;
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CDataSourceWorkThread, CWinThread)
	ON_THREAD_MESSAGE(WM_THREAD_DEAL, OnThreadDeal)
END_MESSAGE_MAP()


// CDataSourceWorkThread message handlers

void CDataSourceWorkThread::OnThreadDeal(WPARAM wParam, LPARAM lParam)
{
	CDataSourceReqestResponceData* pData = (CDataSourceReqestResponceData*)lParam;
	if (m_pDataSourceImpl != NULL && pData != NULL && !::IsBadWritePtr(pData, sizeof(CDataSourceReqestResponceData)))
	{
		BOOL bDealSuccess = FALSE;
		if (pData->m_uStatus == eDataSourceRequestResponceStauts_recvPorcess)
		{
			bDealSuccess = m_pDataSourceImpl->CtrolResponce(pData);
		}
		else if (pData->m_uStatus == eDataSourceRequestResponceStauts_processing)
		{
			bDealSuccess = m_pDataSourceImpl->CtrolRequest(pData);
		}	
		else
		{
			//
			TRACE("处理到一个异常的数据包\n");
		}
	}
	CSingleLock lock(&m_mutex);
	if(lock.Lock())
	{
		m_bIsBusy = FALSE;
		lock.Unlock();
	}
}


BOOL CDataSourceWorkThread::IsThreadBusy()
{
	BOOL bRet = TRUE;
	CSingleLock lock(&m_mutex);
	if (lock.Lock())
	{
		if (!m_bIsBusy)
		{
			bRet = FALSE;
			m_bIsBusy = TRUE;
		}
		lock.Unlock();
	}
	return bRet;
}

