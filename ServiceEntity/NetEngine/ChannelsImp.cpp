#include "StdAfx.h"
#include "ChannelsImp.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CConnectorManagerImp *GetNetManager();

CChannelsImp::CChannelsImp(void)
{
}

CChannelsImp::~CChannelsImp(void)
{
}

int CChannelsImp::SendRcv(long handle,const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen)
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return -1;
	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->SendRcv(ReqBuf,nReqLen,nTimeOut,RcvBuf,nRcvLen);
}
int CChannelsImp::FreeMemory(long handle,char *pMem)
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return -1;
	}
	CConnector *pQconn = (CConnector*)handle;
	pQconn->FreeMem(pMem);
	return 0;
}
int CChannelsImp::AsyncSend(long handle,const char* szBuf,UINT nBufLen,DWORD TimeOut)
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return -1;
	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->AsyncSend(szBuf,nBufLen,TimeOut);
}

int CChannelsImp::RegisterProcessor(long handle,OnPacketReceive processor) 
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return -1;
	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->RegisterProcessor(processor);
}
int CChannelsImp::UnRegisterProcessor(long handle,OnPacketReceive processor)
{
  	if (!GetNetManager()->IsConnectHandle(handle))
  	{
  		return -1;
  	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->UnRegisterProcessor(processor);
}
UINT CChannelsImp::GetPacketID(long handle)
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return -1;
	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->CreatePacketID();
}

CString CChannelsImp::GetLastEorror(long handle)
{
	if (!GetNetManager()->IsConnected(handle))
	{
		return _T("Á¬½Ó¶Ï¿ª");
	}
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->GetLastEorror();
}

Server_type CChannelsImp::GetServerTypeByHandle(long handle)
{
	CConnector *pQconn = (CConnector*)handle;
	return pQconn->GetType();
}