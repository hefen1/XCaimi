#include "StdAfx.h"
#include <process.h>
#include "ConnectorImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void to64frombits(unsigned char *out, const unsigned char *in, int inlen)
{
	const char base64digits[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	for (; inlen >= 3; inlen -= 3)
	{
		*out++ = base64digits[in[0] >> 2];
		*out++ = base64digits[((in[0] << 4) & 0x30) | (in[1] >> 4)];
		*out++ = base64digits[((in[1] << 2) & 0x3c) | (in[2] >> 6)];
		*out++ = base64digits[in[2] & 0x3f];
		in += 3;
	}
	if (inlen > 0)
	{
		unsigned char fragment;

		*out++ = base64digits[in[0] >> 2];
		fragment = (in[0] << 4) & 0x30;
		if (inlen > 1)
			fragment |= in[1] >> 4;
		*out++ = base64digits[fragment];
		*out++ = (inlen < 2) ? '=' : base64digits[(in[1] << 2) & 0x3c];
		*out++ = '=';
	}
	*out = '\0';
};
BOOL IsDomainName(const char *pszAddr, char *pszIP)
{
	try
	{
		int nDot = 0;
		for (int i = 0; pszAddr[i] != '\0'; i++)
		{
			if (pszAddr[i] >= '0' && pszAddr[i] <= '9')
			{
			}
			else if (pszAddr[i] == '.')
			{
				nDot++;
			}
			else
			{
				return TRUE;
			}
		}
		if (nDot != 3)
		{
			return TRUE;
		}
		int arrIP[4];
		sscanf(pszAddr, "%d.%d.%d.%d", &arrIP[0], &arrIP[1], 
			&arrIP[2], &arrIP[3]);
		for (int i = 0; i < 4; i++)
		{
			pszIP[i] = (char)arrIP[i];
		}
	}
	catch(...){}
	return FALSE;
}

char* CConnector::UnZipData(const char *pChar,BOOL& bZip,DWORD &newLen)
{
	bZip = FALSE;
	DataHead *pHead = (DataHead*) pChar;
	if( pHead == NULL )
		return NULL;
	if(pHead->m_nType != RT_ZIPDATA)
		return (char*)pHead;
	try
	{
		TransZipData* pZipData = (TransZipData*)pHead;
		pHead = NULL;
		if(pZipData->m_lOrigLen > 0 && pZipData->m_lZipLen > 0)
		{
			Bytef* pTemp = new Bytef[pZipData->m_lOrigLen];
			uLongf pRetLen = (uLongf)pZipData->m_lOrigLen;
			if( uncompress( pTemp, &pRetLen,
				(Bytef*)pZipData->m_cData, (uLongf)pZipData->m_lZipLen) == Z_OK &&
				pRetLen == pZipData->m_lOrigLen )
			{
				newLen = pRetLen;
				pHead = (DataHead*)pTemp;
				bZip = TRUE;
			}
			else
			{
				delete[] pTemp;
			}
		}
	}
	catch(...)
	{
	}
	return (char*)pHead;
}

int CConnector::NoBlockWait(HANDLE handle, DWORD ntimeout)
{
	if (handle == NULL)
	{
		return -1;
	}
	MSG msg;
	DWORD dRet;
	DWORD tm = GetTickCount();
	DWORD span,wait;
	while(true)
	{	
		if(ntimeout != INFINITE)
		{
			span = GetTickCount() - tm;
			if (span >= ntimeout)//time out
			{
				return 0;
			}
			wait = ntimeout - span;
		}
		else
		{
			wait = INFINITE;
		}
		dRet = MsgWaitForMultipleObjects(1,&handle,FALSE,wait,QS_INPUT);
		if (dRet == WAIT_OBJECT_0 + 1)//msg
		{
			while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if (dRet == WAIT_OBJECT_0)//event occur
		{
			return 1;
		}
		else if (dRet == WAIT_TIMEOUT)//time out
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
}



bool CConnector::IsOpen() 
{
	CSingleLock lock(&m_cCritical_handle);
	lock.Lock();
	bool bOpen = ( INVALID_HANDLE_VALUE != m_hConn );
	lock.Unlock();
	return bOpen;
}

UINT CConnector::CreatePacketID()
{
	UINT ID;
	CSingleLock lock(&m_cCritical_Packet);
	lock.Lock();
	m_packetID ++;
	if (m_packetID == (UINT)-1)
	{
		m_packetID = 1;
	}
	ID = m_packetID;
	lock.Unlock();
	return ID;
}

Server_type CConnector::GetType()
{
	return m_cSrvType;
}
void CConnector::FreeSendData()
{
	CSingleLock lock(&m_cCritical_Send);
	lock.Lock();
	while (!m_listSendDataAll.IsEmpty())
	{
		CDataBuffer *wData = (CDataBuffer*)m_listSendDataAll.RemoveHead();
		delete wData;
	}
	lock.Unlock();
}
void CConnector::Init()
{
	m_ayProcessor.RemoveAll();
	m_packetID = 0;
	m_listSyncPacket.clear();
	m_listSendDataAll.RemoveAll();
	m_listReceiveDataAsync.RemoveAll();
	m_listReceiveDataSync.RemoveAll();  
}

void CConnector::AddAsyncTimeOut(UINT id,DWORD time)
{
	if(id != 0 && id != (UINT)-1)
	{
		DWORD tm = GetTickCount();
		DWORD tmEnd = tm + time;
		CSingleLock lock(&m_cCritical_Async);
		lock.Lock();
		m_mapAsyncTimeout.SetAt(id,tmEnd);
		lock.Unlock();
		SetEvent(m_hEventCallBack);
	}
}

void CConnector::RemoveAsyncTimeOut(UINT id)
{
	CSingleLock lock(&m_cCritical_Async);
	lock.Lock();
	DWORD time;
	if (m_mapAsyncTimeout.Lookup(id,time))
	{
		m_mapAsyncTimeout.RemoveKey(id);
	}
	lock.Unlock();
}

void CConnector::SetSessionID(UINT ID)
{
	CSingleLock lock(&m_cCritical_Session);
	lock.Lock();
	m_sessionID = ID;
	lock.Unlock();
}
UINT CConnector::GetSessionID()
{
	CSingleLock lock(&m_cCritical_Session);
	lock.Lock();
	UINT ID = m_sessionID;
	lock.Unlock();
	return ID;
}
CString CConnector::GetServerName()
{
	return (CString)(m_SrvInfo.m_strServerID);
}
int CConnector::RegisterProcessor(OnPacketReceive processor)
{
	CSingleLock lock(&m_cCritical_Processor);
	lock.Lock();
	m_ayProcessor.Add(processor);
	lock.Unlock();
	return 0;
}
int CConnector::UnRegisterProcessor(OnPacketReceive processor)
{
	CSingleLock lock(&m_cCritical_Processor);
	lock.Lock();
	for (int i=0; i<m_ayProcessor.GetCount(); i++)
	{
		if (m_ayProcessor.GetAt(i) == processor)
		{
			m_ayProcessor.RemoveAt(i);
			break;
		}
	}
	lock.Unlock();
	return 0;
}
void CConnector::DoProcessor(const char *pchar,UINT len)
{
	CSingleLock lock(&m_cCritical_Processor);
	lock.Lock();
	for (int i=0; i<m_ayProcessor.GetCount(); i++)
	{
		OnPacketReceive fProcessor = m_ayProcessor.GetAt(i);
		fProcessor(pchar,len);
	}
	lock.Unlock();
}


void CConnector::FreeMem(char *pBuf)
{
	if (pBuf)
	{
		delete[] pBuf;
	}
}

void CConnector::CloseConn()		
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
		shutdown((SOCKET)m_hConn, SD_BOTH);
		closesocket( (SOCKET)m_hConn );
		m_hConn = INVALID_HANDLE_VALUE;
		lock.Unlock();
#ifdef _DEBUG
		GetSystemTime(&time);
		str.Format("%d:%d.%d",time.wMinute,time.wSecond,time.wMilliseconds);
		TRACE1(_T("断开连接完成！handle=%d; time=") + str +"\n",(long)this);
#endif // _DEBUG
	}
	StopThreads();
}