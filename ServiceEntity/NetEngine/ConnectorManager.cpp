#include "StdAfx.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#include "QuoteConnector.h"
#include "TradeConnector.h"
#include "ServerSpeedTest.h"
#include "HeartBeat.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL WINAPI GConnectNotify(ConnectNotify *pnot);

CConnectorManagerImp::CConnectorManagerImp(void)
{
	m_hEventConnNotify = NULL;
	m_hThreadCheck = NULL;
	CreateCheckThread();
}

CConnectorManagerImp::~CConnectorManagerImp(void)
{
#ifdef _DEBUG
	TRACE0(_T("NetEngine:Manager 析构！\n"));
#endif // _DEBUG
	Free();
	if (m_hThreadCheck != NULL)
	{
		if(WaitForSingleObject(m_hThreadCheck,100L) == WAIT_TIMEOUT)
			TerminateThread(m_hThreadCheck,1L);
		CloseHandle(m_hThreadCheck);
		m_hThreadCheck = NULL;
	}
}

void CConnectorManagerImp::Free()
{
	POSITION pos = m_speedTestRange.GetStartPosition();
	WORD key;
	void* pvalue = NULL;
	while (pos)
	{
		m_speedTestRange.GetNextAssoc(pos,key,pvalue);
		delete (BetsRange*)pvalue;
	}
	m_speedTestRange.RemoveAll();
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		long handle = m_connectedSiteList.GetAt(i);
		m_connectedSiteList.RemoveAt(i);
		lock.Unlock();
		i--;
		DeleteConnect(handle);
		lock.Lock();
	}
	lock.Unlock();
	pos = m_mapSpeedTest.GetStartPosition();
	while (pos)
	{
		m_mapSpeedTest.GetNextAssoc(pos,key,pvalue);
		delete (CServerSpeedTest*)pvalue;
	}
	m_notifyList.RemoveAll();
	m_disConnectList.RemoveAll();
	m_siteInfoList.Free();
	CSingleLock lockN(&m_cCritical_ConnNotify);
	lockN.Lock();
	for(int i=0; i<m_ayConnNotify.GetCount(); i++)
	{
		ConnectNotify *pNoti = m_ayConnNotify.GetAt(i);
		delete pNoti;
	}
	m_ayConnNotify.RemoveAll();
	lockN.Unlock();
}

void CConnectorManagerImp::CreateHeartBeat(long handle)
{
	OnConnectNotify notify = GConnectNotify;
	CHeartBeat *pHeart = new CHeartBeat(handle,notify);
	CSingleLock lock(&m_cCritical_Heart);
	lock.Lock();
	m_mapHeartBeat.SetAt(handle,pHeart);
	lock.Unlock();
}

void CConnectorManagerImp::StopHeartBeat(long handle)
{
	void *pValue;
	CSingleLock lock(&m_cCritical_Heart);
	lock.Lock();
	if (m_mapHeartBeat.Lookup(handle,pValue))
	{
		delete (CHeartBeat*)pValue;
		m_mapHeartBeat.RemoveKey(handle);
	}
	lock.Unlock();
}

void CConnectorManagerImp::CreateCheckThread()
{
	UINT uiThreadId = 0;
	m_hThreadCheck = (HANDLE)_beginthreadex(NULL,	// Security attributes
		0,	// stack
		CheckThreadProc,	// Thread proc
		this,	// Thread param
		CREATE_SUSPENDED,	// creation mode
		&uiThreadId);	// Thread ID
	if ( NULL != m_hThreadCheck)
	{
		ResumeThread( m_hThreadCheck );
		m_hEventConnNotify = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
}

UINT WINAPI CConnectorManagerImp::CheckThreadProc(LPVOID pParam)
{
	CConnectorManagerImp *pThis = reinterpret_cast<CConnectorManagerImp*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunCheck();

	return 1L;
}

void CConnectorManagerImp::RunCheck()
{
	CSingleLock lock(&m_cCritical_ConnNotify);
	while(WaitForSingleObject(m_hEventConnNotify,INFINITE) == WAIT_OBJECT_0)
	{
		CArray<ConnectNotify*,ConnectNotify*>ayNoti;
		lock.Lock();
		for (int i=0; i<m_ayConnNotify.GetCount(); i++)
		{
			ConnectNotify *pNoti = m_ayConnNotify.GetAt(i);
			ayNoti.Add(pNoti);
		}
		m_ayConnNotify.RemoveAll();
		lock.Unlock();
 		for (int j=0; j<ayNoti.GetCount(); j++)
		{
			ConnectNotify *pNoti = ayNoti.GetAt(j);
			Notify(pNoti);
			delete pNoti;
		}
		ayNoti.RemoveAll();
	}
}
void CConnectorManagerImp::DestroyRemovedConn()
{
	CArray<long,long>ayDel;
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		CConnector *pConn = (CConnector*)m_connectedSiteList.GetAt(i);
		if (!m_siteInfoList.GetConnInfo(pConn->GetType(),pConn->GetServerInfo()->m_strServerID))
		{
			ayDel.Add((long)pConn);
		}
	}
	lock.Unlock();
	for (int j=0; j<ayDel.GetCount(); j++)
	{
		DestroyConnect(ayDel.GetAt(j));
	}
}

void CConnectorManagerImp::DeleteConnect(long handle)
{
	CConnector *pConn = (CConnector*)handle;

#ifdef _DEBUG
	SYSTEMTIME time;
	GetSystemTime(&time);
	CString str;
	str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
	TRACE1(_T("停止心跳！handle=%d; time=") + str +"\n",handle);
#endif // _DEBUG
	StopHeartBeat(handle);
#ifdef _DEBUG
	GetSystemTime(&time);
	str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
	TRACE1(_T("停止心跳完成！handle=%d; time=") + str +"\n",handle);
#endif // _DEBUG
	delete pConn;
}

void CConnectorManagerImp::RemoveDisConnect(long handle)
{
	CSingleLock lock(&m_cCritical_DisConn);
	lock.Lock();
	for (int i=0; i<m_disConnectList.GetCount(); i++)
	{
		if (handle == m_disConnectList.GetAt(i))
		{
			m_disConnectList.RemoveAt(i);
			break;
		}
	}
	lock.Unlock();
	return;
}

CString CConnectorManagerImp::GetMacAdd(long handle)
{
	CConnector *pConn = (CConnector*)handle;
	return pConn->GetMacAdd();
}


