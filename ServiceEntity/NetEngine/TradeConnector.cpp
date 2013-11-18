#include "StdAfx.h"
#include "TradeConnector.h"
#include "TradePacker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern NetEngineJYAPI g_NetJYAPI;

CTradeConnector::CTradeConnector(OnConnectNotify noti)
:CConnector(noti)
{
	m_conn = INVALID_HANDLE;
	m_pTradeCert = NULL;
}

CTradeConnector::~CTradeConnector(void)
{
	if (m_pTradeCert)
	{
		delete m_pTradeCert;
	}
}

bool CTradeConnector::IsOpen() 
{
	CSingleLock lock(&m_cCritical_handle);
	lock.Lock();
	bool bOpen = ( INVALID_HANDLE != m_conn );
	lock.Unlock();
	return bOpen;
}

BOOL CTradeConnector::Connect(void *ptr)
{
	if (IsOpen())
	{
		return TRUE;
	}
	if (ptr)
	{
		if (!m_pTradeCert)
		{
			m_pTradeCert = new TradeCert;
		}
		m_pTradeCert->m_Type = ((TradeCert*)ptr)->m_Type;
		memcpy(m_pTradeCert->m_ca,((TradeCert*)ptr)->m_ca,256);
		memcpy(m_pTradeCert->m_cert,((TradeCert*)ptr)->m_cert,256);
		memcpy(m_pTradeCert->m_key,((TradeCert*)ptr)->m_key,256);
		memcpy(m_pTradeCert->m_pwd,((TradeCert*)ptr)->m_pwd,256);
	}
	if(!m_pTradeCert)
	{
		return FALSE;
	}
	m_conn = g_NetJYAPI.JYCreateEI( m_pTradeCert->m_Type, m_SrvInfo.m_strServerIP, m_SrvInfo.m_nPort, 
		m_pTradeCert->m_ca, m_pTradeCert->m_cert, m_pTradeCert->m_key, m_pTradeCert->m_pwd );
	if (m_conn == INVALID_HANDLE)
	{
		OnEvent(EVT_CONFAILURE);
		return FALSE;
	}
	if (m_pProxy && m_pProxy->m_nOptions != NoProxy)//代理
	{
		if( 0 > g_NetJYAPI.JYSetProxy(m_conn,m_pProxy->m_nOptions,m_pProxy->m_strProxyIP,m_pProxy->m_nPort,
			m_pProxy->m_strUser,m_pProxy->m_strPwd,0))
		{
			OnEvent(EVT_PROXYERROR);
			CloseConn();
			return FALSE;
		}
	}
	if( 0 <= g_NetJYAPI.JYConnect( m_conn, DEFAULT_TIMEOUT_SEC ) )
	{
		Open();
		return TRUE;
	}
	else
	{
		OnEvent(EVT_CONFAILURE);
		CloseConn();
		return FALSE;
	}
	return FALSE;
}

void CTradeConnector::CloseConn()		
{
	if (IsOpen())
	{
#ifdef _DEBUG
		SYSTEMTIME time;
		GetSystemTime(&time);
		CString str;
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("断开连接！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
		CSingleLock lock(&m_cCritical_handle);
		lock.Lock();
		g_NetJYAPI.JYDestroyEI(m_conn);
		m_conn = INVALID_HANDLE;
		lock.Unlock();
#ifdef _DEBUG
		GetSystemTime(&time);
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("断开连接完成！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
	}
	CConnector::CloseConn();
}
UINT CTradeConnector::GetPacketID(const char* szBuf,UINT nBufLen)
{
	if (!szBuf || nBufLen <= 0)
	{
		return -1;
	}
	// 输出调试信息
	CTradePacker packer;
	if( 0 != packer.ParseData(szBuf,nBufLen) )
	{
		return -1;
	}
	UINT packid;
	packer.GetHeader( phSenderID, packid );
	return packid;
}

UINT CTradeConnector::SyncSend(const char* ReqBuf,UINT nReqLen)
{
	UINT packetID;
	packetID = GetPacketID(ReqBuf,nReqLen);
	if (packetID == 0 || packetID == (UINT)-1)//同步包的id不能为0
	{
		return (UINT)-1;
	}
	CSingleLock lock(&m_cCritical_SyncPacket);
	lock.Lock();
	m_listSyncPacket.push_back(packetID);
	lock.Unlock();

	CDataBuffer *pData = new CDataBuffer;
	pData->Alloc(nReqLen);
	memcpy(pData->m_pszBuffer, ReqBuf, nReqLen);
	
	CSingleLock locks(&m_cCritical_Send);
	locks.Lock();
	m_listSendDataAll.AddTail(pData);
	locks.Unlock();
	SetEvent(m_hEventSend);
	return packetID;
}

int  CTradeConnector::Receive(char*& RcvBuf,UINT& nRcvLen,DWORD nTimeOut,UINT packID)
{
	HANDLE handles[2];
	handles[0]=m_hEventSyncRcv;
	handles[1]=m_hEventQuit;
	DWORD tm = GetTickCount();
	DWORD span;
	while (true)
	{
		span = GetTickCount() - tm;
		if (span >= nTimeOut)//time out
		{
			nRcvLen = 0;
			OnEvent(EVT_TIMEOUT);
			return -1;
		}
		else
		{
			CSingleLock lock(&m_cCritical_Sync);
			lock.Lock();
			if (!m_listReceiveDataSync.IsEmpty())
			{ 
				POSITION  pos = m_listReceiveDataSync.GetHeadPosition();
				POSITION  prePos;
				while(pos)
				{
					prePos = pos;
					CDataBuffer *wData = (CDataBuffer*)(m_listReceiveDataSync.GetNext(pos));
					if (wData && wData->IsValid() && GetPacketID(wData->m_pszBuffer,wData->m_cbBuffer) == packID)
					{
						RcvBuf = new char[wData->m_cbBuffer];
						nRcvLen = wData->m_cbBuffer;
						memcpy(RcvBuf,wData->m_pszBuffer,nRcvLen);
						m_listReceiveDataSync.RemoveAt(prePos);
						lock.Unlock();
						delete wData;
						return 0;
					}
				}
				lock.Unlock();
			}
			else
			{
				lock.Unlock();
				if(WaitForMultipleObjects(2,handles,FALSE,nTimeOut - span) != WAIT_OBJECT_0)//CConnector::NoBlockWait(m_hEventSyncRcv,nTimeOut - span);
					return -1;
			}
		}
	}
}

int CTradeConnector::SendRcv(const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen)
{
	if (!ReqBuf || nReqLen <= 0)
	{
		return -1;
	}
	UINT packID = SyncSend(ReqBuf,nReqLen);
	if (packID == (UINT)-1)
	{
		nRcvLen = 0;
		return -1;
	}
	return Receive(RcvBuf,nRcvLen,nTimeOut,packID);
}

UINT CTradeConnector::AsyncSend(const char* szBuf,UINT nBufLen,DWORD TimeOut)
{
	if (!szBuf || nBufLen <= 0)
	{
		return -1;
	}

	CDataBuffer *pData = new CDataBuffer;
	pData->Alloc(nBufLen);	
	memcpy(pData->m_pszBuffer, szBuf, nBufLen);

	CSingleLock lock(&m_cCritical_Send);
	lock.Lock();
	m_listSendDataAll.AddTail(pData);
	lock.Unlock();
	SetEvent(m_hEventSend);

	if (TimeOut != (DWORD)-1)//异步超时
	{
		UINT id = GetPacketID(szBuf,nBufLen);
		AddAsyncTimeOut(id,TimeOut);
	}

	return 0;
}

void CTradeConnector::DoReceive(const char* pBuffer,UINT dwCount)
{
	UINT dwPacketID = GetPacketID(pBuffer,dwCount);
	BOOL bAsync = TRUE;
	CSingleLock lock(&m_cCritical_SyncPacket);
	lock.Lock();
	if (!m_listSyncPacket.empty())
	{
		std::list<UINT>::iterator ite = m_listSyncPacket.begin();
		for ( ; ite != m_listSyncPacket.end(); )
		{
			if ((UINT)(*ite) == dwPacketID)
			{
				m_listSyncPacket.erase(ite);
				lock.Unlock();
				CDataBuffer *pData = new CDataBuffer;
				pData->Alloc(dwCount);
				memcpy(pData->m_pszBuffer, pBuffer, dwCount);
				CSingleLock locks(&m_cCritical_Sync);
				locks.Lock();
				m_listReceiveDataSync.AddTail(pData);
				locks.Unlock();
				SetEvent(m_hEventSyncRcv);
				bAsync = FALSE;
				break;
			}
			++ite;
		}
	}
	lock.Unlock();
	if (bAsync)
	{
		CDataBuffer *pData = new CDataBuffer;
		pData->Alloc(dwCount);
		memcpy(pData->m_pszBuffer, pBuffer, dwCount);
		CSingleLock lockas(&m_cCritical_Async);
		lockas.Lock();
		m_listReceiveDataAsync.AddTail(pData);
		lockas.Unlock();
		SetEvent(m_hEventCallBack);
	}
}
// Event function - override to get data
void CTradeConnector::OnEvent(UINT uEvent)
{
	switch( uEvent )
	{
	case EVT_CONSUCCESS:
		m_strLastError =  _T("连接成功！\r\n") ;
		break;
	case EVT_CONFAILURE:
		m_strLastError =  _T("连接超时！\r\n") ;
		break;
	case EVT_CONDROP:
		m_strLastError =  _T("连接断开！\r\n") ;
		if (m_pNotify)
		{
			ConnectNotify not;
			not.m_cType = m_cSrvType;
			not.m_lHandle = (long)this;
			not.m_result = Drop;
			m_pNotify(&not);
		}
		break;
	case EVT_PROXYERROR:
		m_strLastError =  _T("设置代理失败！\r\n") ;
		break;
	case EVT_CREATETHREADERR:
		m_strLastError = _T("创建线程失败！\r\n");
		break;
	case	 EVT_THREADEXIST:  
		m_strLastError = _T("线程已存在！\r\n");
		break;
	case EVT_NOTCONNECTED:
		m_strLastError = _T("未连接！\r\n");
		break;
	case EVT_TIMEOUT:
		m_strLastError = _T("收发数据超时！\r\n");
		break;
	default:
		TRACE("Unknown Socket event\n");
		break;
	}
}

// Run function - override to implement a new behaviour
void CTradeConnector::RunReceive()
{
	UINT	dwBytes  = 0;
	UINT	dwTimeout = DEFAULT_TIMEOUT_MSEC;
	int ret = 0;
	while (true)
	{
		try
		{
			ret = g_NetJYAPI.JYReceive(m_conn,0,dwBytes,dwTimeout);//获得包长度
			if (ret < 0)// lost connection
			{
				OnEvent( EVT_CONDROP ); 
				break;
			}
			else if (dwBytes > 0)
			{
				char *pBuffer = new char[dwBytes];
				ret = g_NetJYAPI.JYReceive(m_conn,pBuffer,dwBytes,dwTimeout);
				if(ret > 0)
				{
#ifdef _DEBUG
					UINT id = GetPacketID(pBuffer,dwBytes);
					long tm = GetTickCount();
					TRACE2(_T("收到交易包ID：%d;Tick=%ld！\n"),id,tm);
#endif // _DEBUG
					DoReceive(pBuffer,dwBytes);
					delete pBuffer;
				}
				else if (ret < 0)
				{
					OnEvent( EVT_CONDROP ); // lost connection
					delete pBuffer;
					break;
				}
			}
		}
		catch(...)
		{
			OnEvent(EVT_RECEIVEEXCEPTION);
		}
	}
}
void CTradeConnector::RunSend()
{
	HANDLE handles[2];
	handles[0]=m_hEventSend;
	handles[1]=m_hEventQuit;
	while( true )
	{
		CSingleLock lock(&m_cCritical_Send);
		lock.Lock();
		if (!m_listSendDataAll.IsEmpty())
		{
			lock.Unlock();
			try
			{
				CDataBuffer *wData;
				lock.Lock();
				wData = (CDataBuffer*)m_listSendDataAll.RemoveHead();
				lock.Unlock();
#ifdef _DEBUG
				UINT id = GetPacketID(wData->m_pszBuffer,wData->m_cbBuffer);
				TRACE1(_T("发送交易包ID：%d！\n"),id);
#endif // _DEBUG
				int dwBytes = g_NetJYAPI.JYSend(m_conn,wData->m_pszBuffer,wData->m_cbBuffer,DEFAULT_TIMEOUT_MSEC);
				delete wData;
				if (dwBytes <= 0)
				{
					FreeSendData();
					OnEvent( EVT_CONDROP ); // lost connection
					break;
				}
			}
			catch(...)
			{
				OnEvent(EVT_SENDEXCEPTION);
			}
		}
		else
		{
			lock.Unlock();
			//WaitForSingleObject(m_hEventSend,INFINITE);
			if(WaitForMultipleObjects(2,handles,FALSE,INFINITE) == WAIT_OBJECT_0+1)
				break;
		}
	}
}
void CTradeConnector::RunCallBack()
{
	HANDLE handles[2];
	handles[0]=m_hEventCallBack;
	handles[1]=m_hEventQuit;
	while(true)
	{
		try
		{
			CSingleLock lock(&m_cCritical_Async);
			lock.Lock();
			if(!m_listReceiveDataAsync.IsEmpty() || !m_mapAsyncTimeout.IsEmpty())
			{
				if (!m_listReceiveDataAsync.IsEmpty())
				{
					CDataBuffer *wData;
					wData = (CDataBuffer*)m_listReceiveDataAsync.RemoveHead();
					lock.Unlock();
					PacketInfo info;
					info.m_lHandle = (long)this;
					info.m_lDataTotal = wData->m_cbBuffer;
					info.m_lDataTransmited = wData->m_cbBuffer;
					info.m_pszData = wData->m_pszBuffer;
#ifdef _DEBUG
					UINT id = GetPacketID(wData->m_pszBuffer,wData->m_cbBuffer);
					long tm = GetTickCount();
					TRACE2(_T("异步回调交易包ID：%d;Tick=%ld！\n"),id,tm);
#endif // _DEBUG
					DoProcessor((char*)&info,sizeof(info));//回调处理
					delete wData;
					lock.Lock();
				}
				if (!m_mapAsyncTimeout.IsEmpty())
				{
					CArray<UINT,UINT> ayID;
					POSITION pos = m_mapAsyncTimeout.GetStartPosition();
					UINT id;
					DWORD tmEnd,tmNow;
					tmNow = GetTickCount();
					while (pos)
					{
						m_mapAsyncTimeout.GetNextAssoc(pos,id,tmEnd);
						if (tmEnd >= tmNow)
						{
							RemoveAsyncTimeOut(id);
							ayID.Add(id);
						}
					}
					lock.Unlock();
					for (int j=0; j<ayID.GetCount(); j++)
					{
						UINT pID = ayID.GetAt(j);
						PacketInfo info;
						info.m_lHandle = (long)this;
						info.m_lDataTotal = pID;
						info.m_lDataTransmited = pID;
						info.m_pszData = NULL;
#ifdef _DEBUG
						TRACE1(_T("异步请求超时交易包ID：%d！\n"),pID);
#endif // _DEBUG
						DoProcessor((char*)&info,sizeof(info));//回调处理
					}
				}
				lock.Unlock();
			}
			else
			{
				lock.Unlock();
				//WaitForSingleObject(m_hEventCallBack,INFINITE);
				if(WaitForMultipleObjects(2,handles,FALSE,INFINITE) == WAIT_OBJECT_0+1)
					break;
			}
		}
		catch(...)
		{
			OnEvent(EVT_CALLBACKEXCEPTION);
		}
	}
}

CString CTradeConnector::GetMacAdd()
{
	if(g_NetJYAPI.JYGetMac == NULL)
		return _T("");

	char* pbuf = new char[128];
	int nLen = 128;
	CString sMac = "";
	int nReturn = g_NetJYAPI.JYGetMac(m_conn, 2, pbuf, nLen);
	if (nReturn > 0)
	{
		sMac = pbuf;
	}
	delete[] pbuf;
	return sMac;

}