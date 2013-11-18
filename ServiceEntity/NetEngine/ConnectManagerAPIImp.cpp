#include "StdAfx.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#include "QuoteConnector.h"
#include "TradeConnector.h"
#include "ServerSpeedTest.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL WINAPI GConnectNotify(ConnectNotify *pnot);
int CConnectorManagerImp::TestProxy(ProxyInfo *pProxy)
{
	CQuoteConnector *pConn = new CQuoteConnector(NULL);
	if (pConn->TestProxy(pProxy))
	{
		delete pConn;
		return 0;
	}
	delete pConn;
	return -1;
}
int CConnectorManagerImp::SetSites(CommInfo *pComm, int count,ProxyInfo *pProxy,Server_type type)
{
	if (!pComm || count <= 0)
	{
		return -1;
	}
	if(type == CEV_Connect_ALL)
		m_siteInfoList.Free();
	else
		m_siteInfoList.RemoveSite(type);
	for (int i=0; i<count; i++)
	{
		ConnectInfo conn;
		conn.m_nPort = pComm[i].m_nPort;
		memcpy(conn.m_strServerID,pComm[i].m_strServerID,sizeof(pComm[i].m_strServerID));
		memcpy(conn.m_strServerIP,pComm[i].m_strServerIP,sizeof(pComm[i].m_strServerIP));
		m_siteInfoList.AddConn(pComm[i].m_cType,&conn,pComm[i].m_cIsDefault==0?FALSE:TRUE);
	}

	if(type == CEV_Connect_ALL || type == CEV_Connect_JY)
	{
		ConnectInfo tradeConnInfo;
		tradeConnInfo.m_nPort = 1234;
		memcpy(tradeConnInfo.m_strServerID,"inner_trade_test",strlen("inner_trade_test"));
		memcpy(tradeConnInfo.m_strServerIP,"127.0.0.1",strlen("127.0.0.1"));
		m_siteInfoList.AddConn(CEV_Connect_JY,&tradeConnInfo,FALSE);
	}

	if (pProxy)
	{
		if (!m_siteInfoList.m_pProxy)
		{
			m_siteInfoList.m_pProxy = new ProxyInfo;
			m_siteInfoList.m_pProxy->m_nOptions = pProxy->m_nOptions;
			m_siteInfoList.m_pProxy->m_nPort = pProxy->m_nPort;
			m_siteInfoList.m_pProxy->m_nUseChannel = pProxy->m_nUseChannel;
			strncpy(m_siteInfoList.m_pProxy->m_strProxyIP,pProxy->m_strProxyIP,sizeof(pProxy->m_strProxyIP));
			strncpy(m_siteInfoList.m_pProxy->m_strUser,pProxy->m_strUser,sizeof(pProxy->m_strUser));
			strncpy(m_siteInfoList.m_pProxy->m_strPwd,pProxy->m_strPwd,sizeof(pProxy->m_strPwd));
		}
	}
	DestroyRemovedConn();
	return 0;
}

long CConnectorManagerImp::CreateConnector(Server_type cType,CString strName,TradeCert *ptr) 
{
	ConnectInfo *pSrv;
	if (strName.IsEmpty())
	{
		pSrv = m_siteInfoList.GetDefult(cType);
	}
	else
		pSrv = m_siteInfoList.GetConnInfo(cType,strName.GetBuffer());
	ProxyInfo *pProxy = m_siteInfoList.m_pProxy;
	if (!pSrv)
	{
		return 0;
	}
	OnConnectNotify notify = GConnectNotify; 
	if (!IsHQServerType(cType))
	{
		CTradeConnector *pConn = new CTradeConnector(notify);
		if (pConn->CreateConn(cType,pSrv,pProxy,ptr))
		{
			CreateHeartBeat((long)pConn);
			RemoveDisConnect((long)pConn);

			CSingleLock lock(&m_cCritical_Conn);
			lock.Lock();
			m_connectedSiteList.Add((long)pConn);
			lock.Unlock();

			ConnectNotify not;
			not.m_cType = cType;
			not.m_lHandle = (long)pConn;
			not.m_result = Linked;
			Notify(&not);
						
			return (long)pConn;
		}
		else
		{
			delete pConn;
			//连接下一个站点
			return ConnectNextSite( cType, strName, ptr);
		}
		
	}
	else
	{
		CQuoteConnector *pConn = new CQuoteConnector(notify);
		if(pConn->CreateConn(cType,pSrv,pProxy,ptr))
		{
			CreateHeartBeat((long)pConn);
			RemoveDisConnect((long)pConn);

			CSingleLock lock(&m_cCritical_Conn);
			lock.Lock();
			m_connectedSiteList.Add((long)pConn);
			lock.Unlock();

			ConnectNotify not;
			not.m_cType = cType;
			not.m_lHandle = (long)pConn;
			not.m_result = Linked;
			Notify(&not);
						
			return (long)pConn;
		}
		else
		{
			delete pConn;
			//连接下一个站点
			return ConnectNextSite( cType, strName, ptr);
		}
	
	}
	return 0;
}

long CConnectorManagerImp::CreateBestConnector(Server_type cType,TradeCert *ptr)
{
	SiteInfo *pSite = m_siteInfoList.GetSite(cType);
	if (pSite)
	{
		if (pSite->m_ayConn.GetCount() == 1)//只有一个站点不测速
		{
			return CreateConnector(cType,pSite->m_ayConn.GetAt(0)->m_strServerID,ptr);
		}
		CServerSpeedTest *pValue;
		if (!m_mapSpeedTest.Lookup(cType,(void*&)pValue))
		{
			pValue = new CServerSpeedTest(pSite,NULL,ptr);
			m_mapSpeedTest.SetAt(cType,pValue);
			ConnectInfo *pSrv = pValue->GetBestConnector();
			if (pSrv)
			{
				return CreateConnector(cType,pSrv->m_strServerID,ptr);
			}
			else
				return CreateConnector(cType,"",ptr);
		}
		else//已经在测速或创建最优连接
		{

		}
	}
	return -1;
}
long CConnectorManagerImp::ConnectNextSite(Server_type cType,CString strName,TradeCert *ptr)
{
	//站点连不上的话，从第一个站点开始连接
	SiteInfo* pSite = m_siteInfoList.GetSite(cType);
	if ( pSite )
	{
		static int nCount = pSite->m_ayConn.GetCount();
		while( nCount > 0)
		{
			nCount--;
			ConnectInfo* pInfo = pSite->FindNext(NULL);
			long lRet =  CreateConnector(cType,(CString)pInfo->m_strServerID, ptr);
			if ( lRet > 0)
				return lRet;
		}
	}
	return 0;
}
int CConnectorManagerImp::DestroyConnect(long handle)
{
	BOOL bFind = FALSE;
	CSingleLock lock(&m_cCritical_Conn);
	lock.Lock();
	for (int i=0; i<m_connectedSiteList.GetCount(); i++)
	{
		if( handle == m_connectedSiteList.GetAt(i))
		{
			m_connectedSiteList.RemoveAt(i);
			bFind = TRUE;
			break;
		}
	}
	lock.Unlock();
	if (!bFind)
		return 0;
	RemoveDisConnect(handle);

#ifdef _DEBUG
	SYSTEMTIME time;
	GetSystemTime(&time);
	CString str;
	str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
	TRACE1(_T("删除链接！handle=%d; time=") + str +"\n",handle);
#endif // _DEBUG
	DeleteConnect(handle);
#ifdef _DEBUG
	GetSystemTime(&time);
	str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
	TRACE1(_T("删除链接完成！handle=%d; time=") + str +"\n",handle);
#endif // _DEBUG
	return 0;
}


int CConnectorManagerImp::RegisterNotify(OnConnectNotify notify)
{
	CSingleLock lock(&m_cCritical_Notify);
	lock.Lock();
	m_notifyList.Add(notify);
	lock.Unlock();
	return 0;
}

int CConnectorManagerImp::UnregisterNotify(OnConnectNotify notify)
{
	CSingleLock lock(&m_cCritical_Notify);
	lock.Lock();
	for (int i=0; i<m_notifyList.GetCount(); i++)
	{
		if (notify == m_notifyList.GetAt(i))
		{
			m_notifyList.RemoveAt(i);
			break;
		}
	}
	lock.Unlock();
	return 0;
}

int CConnectorManagerImp::StopSiteTest(Server_type cType)
{
	void *pValue;
	if (m_mapSpeedTest.Lookup(cType,pValue))
	{
		CServerSpeedTest *pTest = (CServerSpeedTest*)pValue;
		delete pTest;
		m_mapSpeedTest.RemoveKey(cType);
	}
	return 0;
}
int CConnectorManagerImp::TestSiteSpeed(Server_type cType,TestSpeedCallBack tsCall,void *ptr)
{
	StopSiteTest(cType);
	SiteInfo *pSite = m_siteInfoList.GetSite(cType);
	if (pSite)
	{
		CServerSpeedTest *pValue;
		if (!m_mapSpeedTest.Lookup(cType,(void*&)pValue))
		{
			pValue = new CServerSpeedTest(pSite,tsCall,(TradeCert*)ptr);
			m_mapSpeedTest.SetAt(cType,pValue);
			return 0;
		}
		else//已经在测速或创建最优连接
		{

		}
	}
	return -1;
}

void CConnectorManagerImp::SetOnlineRadioRange(Server_type cType,RangeItem *pOnline,int count)
{
	void* pItem = NULL;
	if (m_speedTestRange.Lookup(cType,pItem))
	{
		((BetsRange*)pItem)->SetOnlineRadioRange(pOnline,count);
	}
	else
	{
		BetsRange *pBets = new BetsRange;
		pBets->SetOnlineRadioRange(pOnline,count);
		m_speedTestRange.SetAt(cType,pBets);
	}
}

void CConnectorManagerImp::SetHandleTimeRange(Server_type cType,RangeItem *pTime,int count)
{
	void* pItem = NULL;
	if (m_speedTestRange.Lookup(cType,pItem))
	{
		((BetsRange*)pItem)->SetHandleTimeRange(pTime,count);
	}
	else
	{
		BetsRange *pBets = new BetsRange;
		pBets->SetHandleTimeRange(pTime,count);
		m_speedTestRange.SetAt(cType,pBets);
	}
}
