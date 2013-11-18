#include "StdAfx.h"
#include "QuoteConnector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CQuoteConnector::CQuoteConnector(OnConnectNotify noti)
:CConnector(noti)
{
}

CQuoteConnector::~CQuoteConnector(void)
{
	m_ReceivedData.Free();
	m_PacketHead.Free();
}

void CQuoteConnector::Init()
{
	CConnector::Init();
	m_ReceivedData.Free();
	m_PacketHead.Free();
}

UINT CQuoteConnector::AsyncSend(const char* szBuf,UINT nBufLen,DWORD TimeOut)
{
	if (!szBuf || nBufLen <= 0)
	{
		return -1;
	}

#ifdef DEBUG
	DataHead *pHead = (DataHead*)szBuf;
	TRACE1(_T("发送包功能号：%x！\n"),pHead->m_nType);
	AskData *pAsk = (AskData*)szBuf;
	TRACE1(_T("发送包代码个数：%x！\n"),pAsk->m_nSize);
//  	for (int i=0; i<pAsk->m_nSize; i++)
//  	{
//  		TRACE1(_T("发送包代码：%s！\n"),pAsk->m_pCode[i].GetCode());
//  	}
#endif // _DEBUG

	CDataBuffer *pData = new CDataBuffer;
	long nAlloc = nBufLen + PACKET_FLAG_NUM + sizeof(int);
	pData->Alloc(nAlloc);	
	char *psz2 = pData->m_pszBuffer;
	memcpy(psz2, PACKET_FLAG, PACKET_FLAG_NUM*sizeof(char));//添加包头
	psz2 += PACKET_FLAG_NUM*sizeof(char);
	*(int*)psz2 = nBufLen;
	psz2 += sizeof(int);
	memcpy(psz2, szBuf, nBufLen);

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
int CQuoteConnector::SendRcv(const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen)
{
	if (!ReqBuf || nReqLen <= 0)
	{
		return -1;
	}
	DataHead *pHead = (DataHead*)ReqBuf;
	if (pHead->m_nType == RT_COMPASKDATA)//组合包
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
UINT CQuoteConnector::SyncSend(const char* ReqBuf,UINT nReqLen)
{
	UINT packetID;
	packetID = GetPacketID(ReqBuf,nReqLen);
	if (packetID == 0)//同步包的id不能为0
	{
		return (UINT)-1;
	}
	CSingleLock lock(&m_cCritical_SyncPacket);
	lock.Lock();
	m_listSyncPacket.push_back(packetID);
	lock.Unlock();

	CDataBuffer *pData = new CDataBuffer;
	long nAlloc = nReqLen + PACKET_FLAG_NUM + sizeof(int);
	pData->Alloc(nAlloc);
	char *psz2 = pData->m_pszBuffer;

	memcpy(psz2, PACKET_FLAG, PACKET_FLAG_NUM*sizeof(char));
	psz2 += PACKET_FLAG_NUM*sizeof(char);
	*(int*)psz2 = nReqLen;
	psz2 += sizeof(int);
	memcpy(psz2, ReqBuf, nReqLen);

	CSingleLock locks(&m_cCritical_Send);
	locks.Lock();
	m_listSendDataAll.AddTail(pData);
	locks.Unlock();
	SetEvent(m_hEventSend);
	return packetID;
}

int  CQuoteConnector::Receive(char*& RcvBuf,UINT& nRcvLen,DWORD nTimeOut,UINT packID)
{
	HANDLE handles[2];
	handles[0]=m_hEventSyncRcv;
	handles[1]=m_hEventQuit;
	DWORD tm = GetTickCount();
	DWORD span;
	CSingleLock lock(&m_cCritical_Sync);
	while (true)
	{
		span = GetTickCount() - tm;
		if (span >= nTimeOut)
		{
			nRcvLen = 0;
			OnEvent(EVT_TIMEOUT);
			return -1;
		}
		else
		{
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

UINT CQuoteConnector::GetPacketID(const char* szBuf,UINT nBufLen)
{
	if (!szBuf || nBufLen <= 0)
	{
		return 0;
	}
	DataHead *pHead = (DataHead*)szBuf;
	return pHead->m_lKey;
}
void CQuoteConnector::DoReceive(const char *pBuffer, UINT dwCount)
{
	BOOL bAdd;
	// 对每个接收到的字符进行处理
	for (UINT i = 0; i < dwCount; i++)
	{
		bAdd = TRUE;
		if(m_PacketHead.m_nIndex < PACKET_FLAG_NUM )//报头没收完
		{
			if (!m_PacketHead.IsValid())
			{
				m_PacketHead.Alloc(PACKET_FLAG_NUM + sizeof(int));
			}
			if (pBuffer[i] == PACKET_FLAG[m_PacketHead.m_nIndex])  // check 2003 	包头
			{
				int pkhead = 0;
				// 检查包头
				for( ; i + pkhead < dwCount && m_PacketHead.m_nIndex < PACKET_FLAG_NUM; )
				{
					if (pBuffer[i+pkhead] == PACKET_FLAG[m_PacketHead.m_nIndex] )
					{
						m_PacketHead.m_pszBuffer[m_PacketHead.m_nIndex] = pBuffer[i+pkhead];
						m_PacketHead.m_nIndex ++;
						pkhead ++;
					}
					else
						break;
				}
				if (m_PacketHead.m_nIndex == PACKET_FLAG_NUM)
				{
					i += pkhead;
				}
				else if(i + pkhead < dwCount)//包头不对
				{
					m_PacketHead.m_nIndex -= pkhead;
					bAdd = FALSE;
				}
				else
				{
					i += pkhead;
					bAdd = FALSE;
				}
			}
		}
		if (m_PacketHead.m_nIndex < PACKET_FLAG_NUM + sizeof(int) //包长度没收完
			&& m_PacketHead.m_nIndex >= PACKET_FLAG_NUM)		//包头收完
		{
			int pklen = 0;
			for ( ; i +pklen < dwCount && m_PacketHead.m_nIndex < PACKET_FLAG_NUM + sizeof(int);)
			{
				m_PacketHead.m_pszBuffer[m_PacketHead.m_nIndex] = pBuffer[i + pklen];
				m_PacketHead.m_nIndex ++;
				pklen++;
			}
			
			if (m_PacketHead.m_nIndex == PACKET_FLAG_NUM + sizeof(int))
			{
				// 包长度
				int  *pszStart = (int*)&m_PacketHead.m_pszBuffer[PACKET_FLAG_NUM];
				static int nMax = 100*1024*1024;   //100m
				if( *pszStart >= 0 && *pszStart < nMax ) 
				{
					i += pklen;
					m_ReceivedData.Alloc( *pszStart + 2 );
					m_ReceivedData.m_cbBuffer -= 2;
				}
				else//包长度不对
				{
					m_PacketHead.m_nIndex -= pklen;
					bAdd = FALSE;
				}
			}
			else
				i += pklen;
		}
		// 将数据内容放入接收缓冲区
		if ( m_ReceivedData.IsValid() && bAdd)
		{
			if( m_ReceivedData.m_nIndex <= m_ReceivedData.m_cbBuffer )
			{
				m_ReceivedData.m_pszBuffer[m_ReceivedData.m_nIndex] = pBuffer[i];
				m_ReceivedData.m_nIndex++;
			}
			if (m_ReceivedData.m_nIndex >= m_ReceivedData.m_cbBuffer)//一个完整包
			{		
				DoDivision(m_ReceivedData.m_pszBuffer,m_ReceivedData.m_cbBuffer);
				m_ReceivedData.Free();
				m_PacketHead.Free();
			}
		}
	}
	if (m_ReceivedData.m_cbBuffer > 0 && m_PacketHead.m_nIndex >= m_PacketHead.m_cbBuffer)//不是一个完整包,但包头已收完
	{
		//通知进度
	}
}

void CQuoteConnector::DoDivision(const char* szBuf,UINT nBufLen)
{
	BOOL bZip;
	DWORD len = nBufLen;
	char *pHead = CConnector::UnZipData(szBuf,bZip,len);
	if (!pHead)
	{
		OnEvent(EVT_UNZIPFAILE);
		return;
	}
#ifdef _DEBUG
	DataHead *pData = (DataHead*)pHead;
	TRACE1(_T("收到包功能号：%x！\n"),pData->m_nType);
	AskData *pAsk = (AskData*)pHead;
	TRACE1(_T("收到包代码个数：%x！\n"),pAsk->m_nSize);
#endif // _DEBUG
	if (((DataHead*)pHead)->m_nType == RT_SRV_SYNC)
	{
		AnsSrvSync *pData = (AnsSrvSync *)pHead;
		if(pData->session_stat == 1)
			SetSessionID(pData->srv_sessionid);
		else if(pData->session_stat == -1)//不一致，重新登录
		{
			SetSessionID(0);
			if (m_pNotify)
			{
				ConnectNotify not;
				not.m_cType = m_cSrvType;
				not.m_lHandle = (long)this;
				not.m_result = UnLogin;
				m_pNotify(&not);
			}
		}
	}
	else if (((DataHead*)pHead)->m_nType == RT_LOGIN)
	{
		((DataHead*)pHead)->m_lKey = LOGIN_KEY;
	}
	else if (((DataHead*)pHead)->m_nType == RT_LOGIN_FUTURES)
	{
		((DataHead*)pHead)->m_lKey = LOGIN_KEY;
	}
	else if ( ((DataHead*)pHead)->m_nType == RT_DISCONNLEVEL2 ) //level2连接失败处理
	{
		((DataHead*)pHead)->m_lKey = LEVEL2LOGIN_KEY;
	}
	UINT dwPacketID = GetPacketID(pHead,len);
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
				pData->Alloc(len);
				memcpy(pData->m_pszBuffer, pHead, len);
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
		pData->Alloc(len);
		memcpy(pData->m_pszBuffer, pHead, len);
		CSingleLock lockas(&m_cCritical_Async);
		lockas.Lock();
		m_listReceiveDataAsync.AddTail(pData);
		lockas.Unlock();
		SetEvent(m_hEventCallBack);
	}

	if (bZip)
	{
		delete[] pHead;
	}
}
// Event function - override to get data
void CQuoteConnector::OnEvent(UINT uEvent)
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
	case EVT_UNZIPFAILE:
		m_strLastError = _T("解压缩包失败！\r\n");
		break;
	default:
		TRACE("Unknown Socket event\n");
		break;
	}
	TRACE(m_strLastError);
}
// Run function - override to implement a new behaviour
void CQuoteConnector::RunReceive()
{
	UINT	dwBytes  = 0;
	DWORD	dwTimeout = DEFAULT_TIMEOUT_MSEC;
	char  pData[10240] = {0};
	while( true )
	{
		try
		{
			dwBytes = ReadData(pData, 10240, dwTimeout);
			if (dwBytes == (UINT)-1)// lost connection
			{ 
				OnEvent( EVT_CONDROP ); // lost connection
				break;
			}
			else if (dwBytes > 0)
			{
				DoReceive( pData, dwBytes);
			}
		}
		catch(...)
		{
			OnEvent(EVT_RECEIVEEXCEPTION);
		}
	}
}
void CQuoteConnector::RunSend()
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
				wData = (CDataBuffer*)m_listSendDataAll.GetHead();
				lock.Unlock();
				int nSend = 10240;
				nSend = (int)(wData->m_cbBuffer - wData->m_nIndex) > nSend ? nSend : (int)(wData->m_cbBuffer - wData->m_nIndex);
				UINT dwBytes = SendData(&wData->m_pszBuffer[wData->m_nIndex], nSend, DEFAULT_TIMEOUT_MSEC);
				if (dwBytes == (UINT)-1)
				{
					FreeSendData();
					OnEvent( EVT_CONDROP ); // lost connection
					break;
				}
				else if(dwBytes > 0)
				{
					wData->m_nIndex += nSend;
					if (wData->m_nIndex >= wData->m_cbBuffer)
					{
						lock.Lock();
						m_listSendDataAll.RemoveHead();
						lock.Unlock();
						delete wData;
					}
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
 			if(WaitForMultipleObjects(2,handles,FALSE,INFINITE) == WAIT_OBJECT_0+1)
				break;
		}
	}
}
void CQuoteConnector::RunCallBack()
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

					UINT packID = GetPacketID(wData->m_pszBuffer,wData->m_cbBuffer);
					RemoveAsyncTimeOut(packID);

					PacketInfo info;
					info.m_lHandle = (long)this;
					info.m_lDataTotal = wData->m_cbBuffer;
					info.m_lDataTransmited = wData->m_cbBuffer;
					info.m_pszData = wData->m_pszBuffer;
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
						DoProcessor((char*)&info,sizeof(info));//回调处理
					}
				}
				lock.Unlock();
			}
			else
			{
				lock.Unlock();
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
// Data function
UINT CQuoteConnector::ReadData(char *pBuffer, UINT dwSize, DWORD dwTimeout)
{
	if (pBuffer == NULL || dwSize < 1L)
		return 0L;

	fd_set	fdRead  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = dwTimeout/1000;
		stTime.tv_usec = dwTimeout % 1000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET) m_hConn;
	// Set Descriptor
	if ( !FD_ISSET( s, &fdRead ) )
		FD_SET( s, &fdRead );

	// Select function set read timeout
	UINT dwBytesRead = 0;
	int res = select( s+1, &fdRead, NULL, NULL, pstTime );
	if ( res > 0)
	{
		res = recv( s, pBuffer, dwSize, 0);
		dwBytesRead = (UINT)((res > 0)?(res) : (-1));
	}
	return dwBytesRead;
}
UINT CQuoteConnector::SendData(const char *pBuffer, UINT dwCount, DWORD dwTimeout)
{
	if (!IsOpen() || NULL == pBuffer)
		return 0L;

	fd_set	fdWrite  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = dwTimeout/1000;
		stTime.tv_usec = dwTimeout % 1000;
		pstTime = &stTime;
	}

	SOCKET s = (SOCKET) m_hConn;
	// Set Descriptor
	if ( !FD_ISSET( s, &fdWrite ) )
		FD_SET( s, &fdWrite );

	UINT dwBytesWritten = 0;
	int res = select( s+1, NULL, &fdWrite, NULL, pstTime );
	if ( res > 0)
	{
		res = send( s, pBuffer, dwCount, 0);
		dwBytesWritten = (UINT)((res >= 0)?(res) : (-1));
	}
	return dwBytesWritten;
}

