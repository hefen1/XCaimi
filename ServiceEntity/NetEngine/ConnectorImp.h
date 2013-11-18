#pragma once
#include "NetEngineStructDefine.h"
#include "..\..\Common\hsds_comudata.h"
#include <list>
#include <afxmt.h>
// Event value
#define EVT_CONSUCCESS		0x0000	// Connection established
#define EVT_CONFAILURE		0x0001	// General failure - Wait Connection failed
#define EVT_CONDROP		0x0002	// Connection dropped
#define EVT_PROXYERROR		0x0003	// Zero length message
#define EVT_CREATETHREADERR 0x0005//create thread error
#define EVT_THREADEXIST        0x0006//thread already exist
#define EVT_NOTCONNECTED     0x0007//not connected
#define EVT_SENDEXCEPTION     0x0008//send exception
#define EVT_RECEIVEEXCEPTION  0x0009//receive exception
#define EVT_CALLBACKEXCEPTION 0x000A//callback exception
#define EVT_TIMEOUT             0x000B// time out
#define EVT_LOGINFAILE			0x000C//login faile
#define EVT_UNZIPFAILE			0x000D//unzip faile

#define PACKET_FLAG_NUM	4
const UINT DEFAULT_TIMEOUT_MSEC = 10000;
const UINT DEFAULT_TIMEOUT_SEC = 10;
const char PACKET_FLAG[] = "2003";

struct CDataBuffer
{
	char *m_pszBuffer; //��ǰ���ͺͽ��յ����ݰ�
	UINT m_cbBuffer; //���ܳ�����
	UINT m_nIndex; //�ѷ��ͺͽ��յ�����
	CDataBuffer()
	{
		m_pszBuffer = NULL;
		m_cbBuffer = 0;
		m_nIndex = 0;
	}

	~CDataBuffer()
	{
		Free();
	}

	int Alloc(UINT nSize)//�����ڴ�
	{
		Free();

		if( (long)nSize > 0 )
		{
			m_pszBuffer = new char[nSize+1];			
		}
		if (m_pszBuffer)
		{
			memset(m_pszBuffer,0,nSize+1);
			m_cbBuffer = nSize;
			m_nIndex = 0;
		}

		return IsValid();
	} 

	void Free()   //�ͷ��ڴ�
	{
		if (m_pszBuffer)
		{
			delete [] m_pszBuffer;
			m_pszBuffer = NULL;
			m_cbBuffer = 0;
			m_nIndex = 0;
		}
	}

	int IsValid() { return (m_cbBuffer > 0 && m_pszBuffer != NULL); }
	void cat(const char* lpsz,int nLen,const char* lpsz1,int nLen1)
	{
		if( (lpsz == NULL) && (lpsz1 == NULL) )
			return;

		if( nLen <= 0 && lpsz != NULL)
			nLen = strlen(lpsz);
		if( nLen1 <= 0 && lpsz1 != NULL)
			nLen1 = strlen(lpsz1);

		this->Alloc(nLen + nLen1);
		if( m_pszBuffer )
		{
			if( lpsz )
				memcpy(m_pszBuffer,lpsz,nLen);
			if( lpsz1 )
				memcpy(&m_pszBuffer[nLen],lpsz1,nLen1);
		}
	}

	void CopyStr(const char* lpsz,int nLen)
	{
		if( lpsz == NULL || nLen <= 0 )
			return;

		this->Alloc(nLen);
		if( m_pszBuffer )
		{
			memcpy(m_pszBuffer,lpsz,nLen);
		}
	}

	BOOL Copy(CDataBuffer* pData)
	{
		if( pData == NULL || !pData->IsValid() )
			return FALSE;

		if( !this->Alloc(pData->m_cbBuffer) )
			return FALSE;

		memcpy(this->m_pszBuffer,pData->m_pszBuffer,this->m_cbBuffer);

		return TRUE;
	}

	void AddLong(long lpsz)
	{
		CDataBuffer buffer;
		char szChar[128];
		sprintf(szChar,"%i",lpsz);
		buffer.cat(this->m_pszBuffer,this->m_cbBuffer,szChar,strlen(szChar));
		this->Copy(&buffer);
	}

	void AddStr(const char* lpsz,int nLen = 0)
	{
		if( lpsz == NULL )
			return;

		CDataBuffer buffer;
		if( nLen == 0 )
		{
			buffer.cat(this->m_pszBuffer,this->m_cbBuffer,lpsz,strlen(lpsz));
		}
		else
		{
			CDataBuffer bufferAdd;
			bufferAdd.CopyStr(lpsz,nLen);
			buffer.cat(this->m_pszBuffer,this->m_cbBuffer,bufferAdd.m_pszBuffer,bufferAdd.m_cbBuffer);
		}

		this->Copy(&buffer);
	}

	int Find(const char* str)
	{
		if( !IsValid() )
			return -1;

		char* pdest = strstr(m_pszBuffer,str);
		if( pdest )
			return (pdest - m_pszBuffer);

		return -1;
	}

	int GetLength()
	{
		if( !IsValid() )
			return 0;

		return strlen(m_pszBuffer);
	}

};

class CConnector
{
public:
	CConnector(OnConnectNotify noti=NULL);
	virtual ~CConnector(void);

	virtual int SendRcv(const char* ReqBuf,UINT nReqLen,DWORD nTimeOut,char*& RcvBuf,UINT& nRcvLen){return -1;}
	virtual UINT AsyncSend(const char* szBuf,UINT nBufLen,DWORD TimeOut = -1){return -1;}
	
	virtual void FreeMem(char *pBuf);//�ͷ��ڴ�
	virtual int RegisterProcessor(OnPacketReceive processor);//ע��ص�����
	virtual int UnRegisterProcessor(OnPacketReceive processor);
	virtual CString GetLastEorror(){return m_strLastError;}

	virtual Server_type GetType();//��ȡ��������
	
	virtual BOOL CreateConn(Server_type cSrvType,ConnectInfo *psrv,ProxyInfo *proxy,void *ptr); 
	virtual void CloseConn();//�ر����Ӽ��߳�
	virtual BOOL Connect(void *ptr);
	virtual UINT CreatePacketID();
	virtual UINT GetSessionID();
	virtual ConnectInfo *GetServerInfo(){ return &m_SrvInfo; }
	virtual BOOL TestProxy(ProxyInfo *pProxy);//���Դ���
	virtual CString GetServerName();
	virtual CString GetMacAdd(){ return ""; }

protected:
	void SetSessionID(UINT ID);
	void SetLastEorror(CString str){ m_strLastError = str; }
	
	Server_type      m_cSrvType;
	ConnectInfo   m_SrvInfo;
	ProxyInfo *m_pProxy;
	CString   m_strLastError;    
	OnConnectNotify m_pNotify;//֪ͨ����
	CArray<OnPacketReceive,OnPacketReceive> m_ayProcessor;//�첽����Ӧ��ص�����
	UINT m_packetID;
	UINT m_sessionID;
	CCriticalSection   m_cCritical_Packet;
	CCriticalSection   m_cCritical_Session;
	CCriticalSection   m_cCritical_Processor;
	virtual void DoProcessor(const char *pchar,UINT len);//�ص�
	
protected:
	std::list<UINT>  m_listSyncPacket; //������¼ͬ��Ӧ����Ϣ,SynSendʱ��������ͬ��Ӧ����
	CPtrList  m_listSendDataAll;				//���Ͷ���
	CPtrList  m_listReceiveDataAsync;      //����յ��������첽�����ͷŵ����б���
	CPtrList  m_listReceiveDataSync;       //����յ�������ͬ�������ͷŵ����б���
	CCriticalSection   m_cCritical_Send;
	CCriticalSection   m_cCritical_Async;
	CCriticalSection   m_cCritical_Sync;
	CCriticalSection   m_cCritical_SyncPacket;
	CMap<UINT,UINT,DWORD,DWORD> m_mapAsyncTimeout;//�첽��ʱ������ID��ʱ��
	virtual void AddAsyncTimeOut(UINT id,DWORD time);
	virtual void RemoveAsyncTimeOut(UINT id);

protected:
	//��Ӧ����л�ȡpacket id����Ҫÿ������ȥʵ��
	virtual UINT GetPacketID(const char* szBuf,UINT nBufLen) { return -1;}
	virtual void DoReceive(const char* szBuf,UINT nBufLen){}
	virtual void DoDivision(const char* szBuf,UINT nBufLen){}
	// Event function 
	virtual void OnEvent(UINT uEvent){}
	// Run function - Thread
	virtual void RunReceive(){}
	virtual void RunSend(){}
	virtual void RunCallBack(){}
	// Data function
	virtual UINT SyncSend(const char* ReqBuf,UINT nReqLen){return -1;}
	virtual int  Receive(char*& RcvBuf,UINT& nRcvLen,DWORD nTimeOut,UINT packID){return -1;}
	virtual UINT ReadData(char *pBuffer, UINT dwSize, DWORD dwTimeout){return 0;}
	virtual UINT SendData(const char *pBuffer, UINT dwCount, DWORD dwTimeout){return 0;}
	virtual void Init();
	virtual bool IsOpen();	        // Is Socket valid?
	virtual void FreeSendData();

protected:
	// Create a socket, connect to 
	bool ConnectTo(char *strDestination,UINT port, int nProtocol=AF_INET, int nType=SOCK_STREAM);
	void Open();//��ʼ�������߳�
	BOOL ProxyConnect();
	BOOL ProxyConnHttp();
	BOOL ProxyConnSocks4();
	BOOL ProxyConnSocks5();

protected:
	CCriticalSection  m_cCritical_handle;
	HANDLE		m_hConn;		        // socket handle
	HANDLE		m_hReceiveThread;		// receive Thread  handle
	HANDLE		m_hSendThread;		// send  Thread handle
	HANDLE        m_hCallBackThread;    // callback Thread handle
	HANDLE        m_hEventSend;			// ֪ͨ���Է����¼�
	HANDLE        m_hEventCallBack;	    // ֪ͨ���Իص��¼�
	HANDLE		 m_hEventSyncRcv;     // �յ�ͬ��������
	HANDLE        m_hEventQuit;

	bool StartThreads();		// Start  Socket thread
	void StopThreads();     // stop   socket thread
	bool IsRThreadStart() const;	    // Is receive Thread started?
	bool IsSThreadStart() const;	    // Is send Thread started?
	bool IsCBThreadStart() const;    // Is CallBack Thread started?
	bool CreateReceiveThread();		// Start Receive Socket thread
	void StopReceiveThread();		// Stop Receive Socket thread
	bool CreateSendThread();		// Start Send Socket thread
	void StopSendThread();		    // Stop Send Socket thread
	bool CreateCallBackThread();		// Start CallBack  thread
	void StopCallBackThread();		// Stop CallBack  thread
	int WaitCallBackThreadDone();	// wait "m_listReceiveDataAsync" is empty 

protected:
	static UINT WINAPI ReceiveThreadProc(LPVOID pParam);
	static UINT WINAPI SendThreadProc(LPVOID pParam);
	static UINT WINAPI CallBackThreadProc(LPVOID pParam);

public:
	static int NoBlockWait(HANDLE handle, DWORD ntimeout);//�¼���������1����ʱ����0��������-1����ʱʱ���Ǻ���
	static char* UnZipData(const char *pChar,BOOL& bZip,DWORD &newLen);
};