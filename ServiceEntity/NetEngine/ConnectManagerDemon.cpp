#include "StdAfx.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#include "QuoteConnector.h"
#include "TradeConnector.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CConnectorManagerImp *GetNetManager();

BOOL WINAPI GConnectNotify(ConnectNotify *pnot)
{
	return GetNetManager()->FConnNotify(pnot);
}

BOOL CConnectorManagerImp::FConnNotify(ConnectNotify *pnot)
{
	CSingleLock lock(&m_cCritical_ConnNotify);
	ConnectNotify *noti = new ConnectNotify;
	noti->m_cType = pnot->m_cType;
	noti->m_lHandle = pnot->m_lHandle;
	noti->m_result = pnot->m_result;
	lock.Lock();
	m_ayConnNotify.Add(noti);
	lock.Unlock();
	SetEvent(m_hEventConnNotify);
	return TRUE;
}
float CConnectorManagerImp::GetOnlineRadioRange(Server_type cType,float fValue)
{
	void* pItem = NULL;
	if (m_speedTestRange.Lookup(cType,pItem))
	{
		return ((BetsRange*)pItem)->GetOnLineRatioBets(fValue);
	}
	return -1;
}

float CConnectorManagerImp::GetHandleTimeRange(Server_type cType,float fValue)
{
	void* pItem = NULL;
	if (m_speedTestRange.Lookup(cType,pItem))
	{
		return ((BetsRange*)pItem)->GetReqHandleTimeBets(fValue);
	}
	return -1;
}
CString CConnectorManagerImp::GetServerName(long handle)
{
	BOOL bFind = FALSE;
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		if( m_connectedSiteList.GetAt(i) == handle )
		{
			bFind = TRUE;
			break;
		}
	}
	lock.Unlock();
	CSingleLock lockDis(&m_cCritical_DisConn);
	lockDis.Lock();
	if ( !bFind)
	{
		for (int i=0; i<m_disConnectList.GetCount(); i++)
		{
			if (m_disConnectList.GetAt(i) == handle)
			{
				bFind = TRUE;
				break;
			}
		}
	}
	lockDis.Unlock();
	if ( !bFind )
		return "";
	CConnector* pConn = (CConnector*)handle;
	return pConn->GetServerName();
}
bool CConnectorManagerImp::IsConnected(long handle)
{
	BOOL bConn = FALSE;
	BOOL bDisconn = FALSE;
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		if( m_connectedSiteList.GetAt(i) == handle )
		{
			bConn = TRUE;
			break;
		}
	}
	lock.Unlock();
	CSingleLock lockDis(&m_cCritical_DisConn);
	lockDis.Lock();
	for (int i=0; i<m_disConnectList.GetCount(); i++)
	{
		if (m_disConnectList.GetAt(i) == handle)
		{
			bDisconn = TRUE;
			break;
		}
	}
	lockDis.Unlock();
	return (bConn && !bDisconn);
}

BOOL CConnectorManagerImp::IsConnectHandle( long handle )
{
	BOOL bConn = FALSE;
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		if( m_connectedSiteList.GetAt(i) == handle )
		{
			bConn = TRUE;
			break;
		}
	}
	lock.Unlock();
	return bConn;
}

BOOL  CConnectorManagerImp::Notify(ConnectNotify *pNot)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (pNot)
	{
		if (pNot->m_result == Drop)
		{
			BOOL bAdd = TRUE;
			CSingleLock lockDis(&m_cCritical_DisConn);
			lockDis.Lock();
			for (int i=0; i<m_disConnectList.GetCount(); i++)
			{
				if (m_disConnectList.GetAt(i) == pNot->m_lHandle)
				{
					bAdd = FALSE;
					break;
				}
			}
			if(bAdd)
				m_disConnectList.Add(pNot->m_lHandle);
			lockDis.Unlock();
		}
		CSingleLock lock(&m_cCritical_Notify);
		lock.Lock();
		for (int i=0; i<m_notifyList.GetCount(); i++)
		{
			OnConnectNotify notify = m_notifyList.GetAt(i);
			notify(pNot);
		}
		lock.Unlock();
	}
	return TRUE;
}

BOOL CConnectorManagerImp::IsHQServerType(Server_type type)
{
	if (type == CEV_Connect_JY || type == CEV_Connect_ZX || type == CEV_Connect_CENTER)
	{
		return FALSE;
	}
	return TRUE;
}


