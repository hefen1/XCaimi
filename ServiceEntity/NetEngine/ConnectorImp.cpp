#include "StdAfx.h"
#include <process.h>
#include "ConnectorImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void to64frombits(unsigned char *out, const unsigned char *in, int inlen);
extern BOOL IsDomainName(const char *pszAddr, char *pszIP);

CConnector::CConnector(OnConnectNotify noti):
m_hConn(INVALID_HANDLE_VALUE), m_hReceiveThread(NULL), m_hSendThread(NULL), m_hCallBackThread(NULL),
m_hEventSend(NULL),m_hEventSyncRcv(NULL),m_hEventCallBack(NULL),m_packetID(0),m_pProxy(NULL),m_pNotify(NULL)
{
	m_sessionID = 0;
	m_pNotify = noti;
	m_hEventSyncRcv= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventCallBack= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventSend= CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hEventQuit = CreateEvent(NULL,TRUE,FALSE,NULL);
}

CConnector::~CConnector(void)
{
	CloseConn();
	if (NULL != m_hEventSyncRcv)
	{
		CloseHandle( m_hEventSyncRcv );
		m_hEventSyncRcv = NULL;
	}
	if (NULL != m_hEventCallBack)
	{
		CloseHandle( m_hEventCallBack );
		m_hEventCallBack = NULL;
	}
	if (NULL != m_hEventSend)
	{
		CloseHandle( m_hEventSend );
		m_hEventSend = NULL;
	}
	if (NULL != m_hEventQuit)
	{
		CloseHandle( m_hEventQuit );
		m_hEventQuit = NULL;
	}
	FreeSendData();
}

BOOL CConnector::CreateConn(Server_type cSrvType,ConnectInfo *pSrv,ProxyInfo *proxy,void *ptr)
{
#ifdef _DEBUG
	if(cSrvType == CEV_Connect_JY && strcmp(pSrv->m_strServerID,"inner_trade_test") == 0)
	{
		OutputDebugString("connect inner_trade_test...\n");
	}
#endif

	if (!pSrv)
	{
		return FALSE;
	}
	m_SrvInfo.Copy(pSrv);
	m_pProxy = proxy;
	m_cSrvType = cSrvType;
	return Connect(ptr);
}

BOOL CConnector::Connect(void *ptr)
{
	if (IsOpen())
	{
		return TRUE;
	}
	if (m_pProxy && m_pProxy->m_nOptions != NoProxy)//代理
	{
		if(ConnectTo(m_pProxy->m_strProxyIP,m_pProxy->m_nPort) && ProxyConnect())
		{
			Open();
			return TRUE;
		}
		else
		{
			CloseConn();
			return FALSE;
		}
	}
	else
	{
		if(ConnectTo(m_SrvInfo.m_strServerIP,m_SrvInfo.m_nPort))
		{
			Open();
			return TRUE;
		}
		else
		{
			CloseConn();
			return FALSE;
		}
	}
	return FALSE;
}
bool CConnector::ConnectTo(char *strDestination,UINT port, int nProtocol/*=AF_INET*/, int nType/*=SOCK_STREAM*/)
{
	// Socket is already opened
	if ( IsOpen() )
		return false;

	SOCKET sock = socket(nProtocol, nType, 0);
	if (INVALID_SOCKET != sock)
	{
		sockaddr_in addr;
		memset(&addr,0,sizeof(SOCKADDR_IN));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(strDestination);
		if (addr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(strDestination);
			if (lphost != NULL)
				addr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				OnEvent(EVT_CONFAILURE);
				closesocket(sock);
				return false;
			}
		}
		addr.sin_port = htons(port);   
		if (SOCKET_ERROR == connect( sock, (sockaddr*)&addr, sizeof(addr)))
		{
			OnEvent(EVT_CONFAILURE);
			closesocket( sock );
			return false;
		}
		m_hConn = (HANDLE) sock;
	}
	return (INVALID_SOCKET != sock);
}

BOOL CConnector::TestProxy(ProxyInfo *pProxy)
{
	if (pProxy && pProxy->m_nOptions != NoProxy)
	{
		return ConnectTo(pProxy->m_strProxyIP,pProxy->m_nPort);
	}
	return FALSE;
}
BOOL CConnector::ProxyConnHttp()
{
	CString strSend;
	CString strPwd(m_pProxy->m_strPwd);
	if (!strPwd.IsEmpty())
	{
		//Proxy-Authorization: Basic
		char szAuth[1024 + 1] = {""};
		char szAuthT[1024 + 1] = {""};
		sprintf(szAuthT, "%s:%s", m_pProxy->m_strUser,m_pProxy->m_strPwd);
		to64frombits((UCHAR *)szAuth, (UCHAR *)szAuthT, strlen(szAuthT));
		strSend.Format("CONNECT %s:%d HTTP/1.0\r\nHost: %s:%d\r\nProxy-Authorization: Basic %s\r\nProxy-Connection: Keep-Alive\r\n\r\n",
			m_SrvInfo.m_strServerIP, m_SrvInfo.m_nPort,m_SrvInfo.m_strServerIP, m_SrvInfo.m_nPort, szAuth);
	}
	else
		strSend.Format("CONNECT %s:%d HTTP/1.0\r\nHost: %s:%d\r\nProxy-Connection: Keep-Alive\r\n\r\n",
			m_SrvInfo.m_strServerIP, m_SrvInfo.m_nPort, m_SrvInfo.m_strServerIP,m_SrvInfo.m_nPort);
	UINT dw = SendData(strSend, strSend.GetLength(),DEFAULT_TIMEOUT_MSEC);	
	if (dw == 0 || dw == (UINT)-1)
	{
		return FALSE;
	}
	char cBuf[1024] = {0};
	dw = ReadData(cBuf,1024,DEFAULT_TIMEOUT_MSEC);
	if (dw == (UINT)-1 || dw == 0)
	{
		return FALSE;
	}
	if (strstr(cBuf,"HTTP/1.0 200") != NULL
		|| strstr(cBuf,"HTTP/1.1 200") != NULL)
	{
		return TRUE;
	}
	else
	{
		m_strLastError = cBuf;
	}
	return FALSE;
}
BOOL CConnector::ProxyConnSocks4()
{
	TCHAR szIP[4];
	szIP[0]	= szIP[1] = szIP[2] = '\0';
	szIP[3] = (TCHAR)1;
	BOOL bDomainName = IsDomainName(m_SrvInfo.m_strServerIP, szIP);
	int cbBuffer = _tcslen(m_pProxy->m_strUser) + 1;
	int nAlloc = 8 + cbBuffer;
	if (bDomainName)
	{
		nAlloc += _tcslen(m_SrvInfo.m_strServerIP) + 1;
	}
	char *pszRequest = new char[nAlloc];
	memset(pszRequest, 0, nAlloc);
	// VN(1) CD(1) DSTPORT(2) DSTIP(4) USERID(Var) NULL(1)
	pszRequest[0] = 4; // SOCKS version 4
	pszRequest[1] = 1; // connect
	pszRequest[2] = m_SrvInfo.m_nPort / 256;
	pszRequest[3] = m_SrvInfo.m_nPort % 256;
	memcpy(&pszRequest[4], szIP, 4);
	memcpy(&pszRequest[8], m_pProxy->m_strUser, cbBuffer);
	if (bDomainName)
	{
		strcpy(&pszRequest[8 + cbBuffer], m_SrvInfo.m_strServerIP);
	}
	UINT dw = SendData(pszRequest, nAlloc, DEFAULT_TIMEOUT_MSEC);
	delete [] pszRequest;
	if (dw == 0 || dw == (UINT)-1)
	{
		return FALSE;
	}
	char cBuf[1024] = {0};
	dw = ReadData(cBuf,1024,DEFAULT_TIMEOUT_MSEC);
	if (dw == (UINT)-1 || dw == 0)
	{
		return FALSE;
	}
	if (cBuf[1] != 90 || cBuf[0] != 0)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CConnector::ProxyConnSocks5()
{
	char command[10];
	BOOL bUseSocks5Logon = 0;
	CString strPwd(m_pProxy->m_strPwd);
	CString strUsr(m_pProxy->m_strUser);
	if( strPwd.IsEmpty() )
		bUseSocks5Logon = 0;
	else
		bUseSocks5Logon = 1;
	memset(command,0,10);
	command[0] = 5;
	command[1] = bUseSocks5Logon?2:1;
	command[2] = bUseSocks5Logon?2:0;

	UINT num = SendData(command,bUseSocks5Logon?4:3,DEFAULT_TIMEOUT_MSEC);
	if (num == (UINT)-1 || num == 0)
	{
		return FALSE;
	}
	//Get initialization response
	num = ReadData(command,2,DEFAULT_TIMEOUT_MSEC);
	if (num != 2 || command[0] != 5)
	{
		return FALSE;
	}
	if (command[1]==0xFF)
	{
		return FALSE;
	}
	if (command[1])
	{
		if (command[1]!=2)
		{
			return FALSE;
		}
		if (bUseSocks5Logon)
		{
			//Send authentication
			int lLen = 3 + strUsr.GetLength()+strPwd.GetLength();
			char *pData = new char[lLen];
			char *buffer = pData;
			*buffer = 1; //身份验证版本号是1而不是5
			buffer ++;

			*buffer = strUsr.GetLength();
			buffer ++;
			memcpy(buffer,strUsr,strUsr.GetLength());
			buffer += strUsr.GetLength();

			*buffer = strPwd.GetLength();
			buffer ++;
			memcpy(buffer,strPwd,strPwd.GetLength());

			UINT res = SendData(pData,lLen,DEFAULT_TIMEOUT_MSEC);
			delete[] pData;

			if (res == (UINT)-1 || res < lLen)
			{
				return FALSE;
			}
			//Get auth response
			int num = ReadData(command,2,DEFAULT_TIMEOUT_MSEC);
			if (num!=2)
			{
				return FALSE;
			}
			if (command[1]!=0x00)
			{
				return FALSE;
			}
		}
	}
	{
		char szIP[4];
		szIP[0]	= szIP[1] = szIP[2] = '\0';
		szIP[3] = (char)1;
		BOOL bDomain = IsDomainName(m_SrvInfo.m_strServerIP, szIP);
		char *command = new char[10+_tcslen(m_SrvInfo.m_strServerIP)+1];
		memset(command,0,10+_tcslen(m_SrvInfo.m_strServerIP)+1);
		command[0] = 5;
		command[1] = 1;//connect
		command[2] = 0;
		command[3] = !bDomain?1:3;
		int len = 4;
		if (!bDomain)
		{
			memcpy(&command[len],szIP,4);
			len += 4;
		}
		else
		{
			command[len] = _tcslen(m_SrvInfo.m_strServerIP);
			strcpy(&command[len+1],m_SrvInfo.m_strServerIP);
			len += _tcslen(m_SrvInfo.m_strServerIP)+1;
		}
		command[len]   = m_SrvInfo.m_nPort / 256;
		command[len+1] = m_SrvInfo.m_nPort % 256;
		len += 2;

		UINT dw = SendData(command,len,DEFAULT_TIMEOUT_MSEC);
		if (dw == (UINT)-1 || dw == 0)
		{
			return FALSE;
		}
		//Get auth response
		int num = ReadData(command,len,DEFAULT_TIMEOUT_MSEC);
		if (command[1]!=0x00)
		{
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CConnector::ProxyConnect()
{
	CString str(m_pProxy->m_strUser);
	if(str.IsEmpty())
	{
		char szUID[64];
		szUID[0] = '\0';
		DWORD cbBuffer = sizeof(szUID);
		if (::GetUserName(szUID, &cbBuffer))
		{
			szUID[cbBuffer - 1] = '\0';
			strcpy(m_pProxy->m_strUser, szUID);
		}
	}
	if(m_pProxy->m_nOptions == Http) // http<Connect>
	{
		return ProxyConnHttp();
	}
	else if( m_pProxy->m_nOptions == Socks4 )
	{
		return ProxyConnSocks4();
	}
	else if( m_pProxy->m_nOptions == Socks5 )
	{
		return ProxyConnSocks5();
	}
	return FALSE;
}
void CConnector::Open()
{
	Init();
	StartThreads();
}


