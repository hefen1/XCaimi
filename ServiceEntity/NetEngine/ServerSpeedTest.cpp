#include "StdAfx.h"
#include "ServerSpeedTest.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#include "QuoteConnector.h"
#include "TradeConnector.h"
#include "TradePacker.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CConnectorManagerImp *GetNetManager();

CServerSpeedTest::CServerSpeedTest(SiteInfo *psite,TestSpeedCallBack tsCallBack,TradeCert *pTrade)
{
	m_hThreadBest = NULL;
	m_pBestSite = NULL;
	m_pCert = NULL;
	m_pSiteInfo = NULL;
	fCallBack = NULL;
	m_lBeginWait = ULONG_MAX;
	m_lWaitOut = ULONG_MAX;
	m_nNowTest =0;
	m_hEventTestSpeed = CreateEvent(NULL,FALSE,FALSE,NULL);
	SetSite(psite, tsCallBack,pTrade);
	if(tsCallBack != NULL)
		TestSpeed();
}

CServerSpeedTest::~CServerSpeedTest(void)
{
	StopTestSpeed();
	if(m_hEventTestSpeed != NULL)
	{
		CloseHandle(m_hEventTestSpeed);
		m_hEventTestSpeed = NULL;
	}
	StopCalBest();
}

void CServerSpeedTest::SetSite(SiteInfo *psite,TestSpeedCallBack tsCallBack,TradeCert *pTrade)
{
	m_pSiteInfo = psite;
	m_srvType = m_pSiteInfo?m_pSiteInfo->m_cType:CEV_Connect_HQ;
	m_pCert = pTrade;
	fCallBack = tsCallBack;
}

BOOL CServerSpeedTest::TestSpeed()
{
	if (!m_pSiteInfo)
	{
		return FALSE;
	}
	HANDLE hThread;
	UINT uiThreadId = 0;
	for(int i=0; i<m_pSiteInfo->m_ayConn.GetCount(); i++)
	{
		hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
			0,	// stack
			TestSpeedThreadProc,	// Thread proc
			this,	// Thread param
			CREATE_SUSPENDED,	// creation mode
			&uiThreadId);	// Thread ID
		if ( NULL != hThread)
		{
			ResumeThread( hThread );
			m_ayTestSpeedThreads.Add(hThread);
		}
	}
	return TRUE;
}
UINT WINAPI CServerSpeedTest::TestSpeedThreadProc(LPVOID pParam)
{
	CServerSpeedTest* pThis = reinterpret_cast<CServerSpeedTest*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunTestSpeed();
	
	return 1L;
}
void  CServerSpeedTest::RunTestSpeed()
{
	if (CConnectorManagerImp::IsHQServerType(m_srvType))
	{
		RunTestSpeedHQ();
	}
	else
		RunTestSpeedJY();
}
void  CServerSpeedTest::RunTestSpeedHQ()
{
	ConnectInfo *pSrv = GetTestServer();
	if (pSrv)
	{
		CQuoteConnector *pConn = new CQuoteConnector;
		if(pConn->CreateConn(m_srvType,pSrv,NULL,m_pCert))
		{
			//先发送登录包
			int nCodeCount = sizeof(ReqLogin) / sizeof(CodeInfo) + ((sizeof(ReqLogin) % sizeof(CodeInfo))?1:0);
			int lRetLen    = sizeof(AskData) + sizeof(CodeInfo)*(nCodeCount - 1);
			UINT lDataLen = 0;
			char *pData;
			AskData* pAskData = (AskData*)(new char[lRetLen]);
			if( pAskData == NULL )
			{
				return ;
			}
			memset(pAskData, 0, lRetLen);
			pAskData->m_nType = RT_LOGIN;
			pAskData->m_nSize = nCodeCount;
			ReqLogin* pLogin  = (ReqLogin*)pAskData->m_pCode;
			pAskData->m_nOption = Login_Option_Password;
			pAskData->m_lKey = LOGIN_KEY;  // 客户端内部版本

			strncpy(pLogin->m_szPWD, "1", min(sizeof(pLogin->m_szPWD), 1));
			strncpy(pLogin->m_szUser,"", sizeof(pLogin->m_szUser));
			if(pConn->SendRcv((char*)pAskData,lRetLen,DEFAULT_TIMEOUT_MSEC,pData,lDataLen) != -1)
			{
				delete[] pAskData;
				pAskData = NULL;
				pConn->FreeMem(pData);

				lRetLen = sizeof(AskData);
				pAskData = (AskData*)new char[lRetLen];
				memset(pAskData, 0, lRetLen);				
				pAskData->m_nType = RT_SERVERINFO;
				pAskData->m_cOperator = CEV_Connect_HQ;
				pAskData->m_lKey = pConn->CreatePacketID();
				DWORD sendTime = GetTickCount();
				if(pConn->SendRcv((char*)pAskData,lRetLen,DEFAULT_TIMEOUT_MSEC,pData,lDataLen) != -1)
				{
					DataHead *pHead = (DataHead *)pData;
					if (pHead->m_nType == RT_SERVERINFO)
					{
						CalcResultHQ(pData,pSrv,sendTime);
					}
					else if(fCallBack)
					{
						SpeedTestResult result;
						memcpy(result.m_strServerID,pSrv->m_strServerID,128);
						result.m_bError = TRUE;
						strcpy(result.m_strError,"测速应答没有返回！");
						fCallBack(&result);
					}
					pConn->FreeMem(pData);
				}
				else if(fCallBack)
				{
					SpeedTestResult result;
					memcpy(result.m_strServerID,pSrv->m_strServerID,sizeof(result.m_strServerID));
					result.m_bError = TRUE;
					strncpy_s(result.m_strError, pConn->GetLastEorror().GetBuffer(),sizeof(result.m_strError));
					fCallBack(&result);
				}
				delete[] pAskData;
				pAskData = NULL;
			}	
			if ( pAskData != NULL )
				delete[] pAskData;
			
		}
		else if(fCallBack)
		{
			SpeedTestResult result;
			memcpy(result.m_strServerID,pSrv->m_strServerID,sizeof(result.m_strServerID));
			result.m_bError = TRUE;
			strcpy(result.m_strError,"连接超时！");
			fCallBack(&result);
		}
		delete pConn;
	}
}
void  CServerSpeedTest::RunTestSpeedJY()
{
	ConnectInfo *pSrv = GetTestServer();
	if (pSrv)
	{
		CTradeConnector *pConn = new CTradeConnector;
		if(pConn->CreateConn(m_srvType,pSrv,NULL,m_pCert))
		{
			CTradePacker packer;
			packer.SetHeader( phAction, 60003 );//60003是心跳包
			packer.SetHeader( phCellIndex, 0 );
			packer.SetHeader( phOnlineID, 0 );
			packer.SetHeader( phOnlineToken, 0 );
			packer.SetHeader( phReturnCode, 0 );
			packer.SetHeader( phClientAddr, 0 );
			packer.SetHeader( phSenderID, pConn->CreatePacketID() );

			char* lpPackBuf = NULL;
			UINT iPackLen = 0;	

			lpPackBuf = packer.GetPackData(iPackLen);

			DWORD sendTime = GetTickCount();
			UINT lDataLen;
			char *pData = NULL;
			if(pConn->SendRcv(lpPackBuf,iPackLen,DEFAULT_TIMEOUT_MSEC,pData,lDataLen) != -1)
			{
				CalcResultJY(pData,pSrv,sendTime);
				pConn->FreeMem(pData);
			}
			else if(fCallBack)
			{
				SpeedTestResult result;
				memcpy(result.m_strServerID,pSrv->m_strServerID,sizeof(result.m_strServerID));
				result.m_bError = TRUE;

				strncpy_s(result.m_strError, pConn->GetLastEorror().GetBuffer(),sizeof(result.m_strError));
				fCallBack(&result);
			}	
			delete[] lpPackBuf;
		}
		else if(fCallBack)
		{
			SpeedTestResult result;
			memcpy(result.m_strServerID,pSrv->m_strServerID,sizeof(result.m_strServerID));
			result.m_bError = TRUE;
			strcpy(result.m_strError,"连接超时！");
			fCallBack(&result);
		}
		delete pConn;
	}
}

void  CServerSpeedTest::CalcResultHQ(char* pData, ConnectInfo *pSrv,DWORD sendTime)
{
	AnsServerInfo* pServerInfo = (AnsServerInfo*)pData;
	if (pServerInfo)
	{
		float online_ratio = float(pServerInfo->m_lTotalCount != 0 ? (pServerInfo->m_lNowCount * 1.0 / pServerInfo->m_lTotalCount) : 0);
		float online_ratio_bets = GetNetManager()->GetOnlineRadioRange(m_srvType,online_ratio);
		if(online_ratio_bets == -1)
			online_ratio_bets = 3;
		DWORD now = GetTickCount();
		long nTimeSpend = now -  sendTime;

		float req_time_bets = GetNetManager()->GetHandleTimeRange(m_srvType,(float)nTimeSpend);
		if(req_time_bets == -1)
			req_time_bets = 3;
		long waiteTime = long(nTimeSpend * req_time_bets + online_ratio * online_ratio_bets * 1000);
		if(fCallBack)
		{
			SpeedTestResult result;
			memcpy(result.m_strServerID,pSrv->m_strServerID,sizeof(result.m_strServerID));
			result.m_bError = FALSE;
			result.m_onlineRadio = online_ratio;
			result.m_timeSpan = nTimeSpend;
			result.m_result = waiteTime;
			fCallBack(&result);
		}
		else if(SetWaitOut(waiteTime))//目前为止是最优的
		{
			SetBestSite(pSrv);
		}
	}
}
void  CServerSpeedTest::CalcResultJY(char* pData,ConnectInfo *pSrv,DWORD sendTime)
{
	CTradePacker packer;
	UINT iPackLen = 0;	
	char *pValue;
	long onlineNum=0;
	long totalNum=0;
	pValue = packer.GetData("onlinenums",iPackLen);
	if ( pValue)
	{
		onlineNum = atol(pValue);
		delete pValue;
	}
	pValue = packer.GetData("maxnums",iPackLen);
	if ( pValue )
	{
		totalNum = atol(pValue);
		delete pValue;
	}

	float online_ratio = float(totalNum != 0 ? (onlineNum * 1.0 / totalNum) : 0);
	float online_ratio_bets = GetNetManager()->GetOnlineRadioRange(m_srvType,online_ratio);
	if(online_ratio_bets == -1)
		online_ratio_bets = 3;
	DWORD now = GetTickCount();
	long nTimeSpend = now -  sendTime;
	float req_time_bets = GetNetManager()->GetHandleTimeRange(m_srvType,(float)nTimeSpend);
	if(req_time_bets == -1)
		req_time_bets = 3;
	long waiteTime = long(nTimeSpend * req_time_bets + online_ratio * online_ratio_bets * 1000);
	if(fCallBack)
	{
		SpeedTestResult result;
		memcpy(result.m_strServerID,pSrv->m_strServerID,128);
		result.m_bError = FALSE;
		result.m_onlineRadio = online_ratio;
		result.m_timeSpan = nTimeSpend;
		result.m_result = waiteTime;
		fCallBack(&result);
	}
	else if(SetWaitOut(waiteTime))//目前为止是最优的
	{
		SetBestSite(pSrv);
	}
}
BOOL  CServerSpeedTest::SetWaitOut(DWORD wait)
{
	CSingleLock lock(&m_cCritical_Wait);
	lock.Lock();
	if(wait < m_lWaitOut)
	{
		m_lBeginWait = GetTickCount();
		m_lWaitOut = wait;
		lock.Unlock();
		SetEvent(m_hEventTestSpeed);
		return TRUE;
	}
	lock.Unlock();
	return FALSE;
}

void CServerSpeedTest::SetBestSite(ConnectInfo* pSrv)
{
	if (!pSrv)
	{
		return;
	}
	CSingleLock lock(&m_cCritical_Best);
	lock.Lock();
	m_pBestSite = pSrv;
	lock.Unlock();
}

ConnectInfo *CServerSpeedTest::GetBestSite()
{
	CSingleLock lock(&m_cCritical_Best);
	lock.Lock();
	ConnectInfo *pSrv = m_pBestSite;
	lock.Unlock();
	return pSrv;
}

ConnectInfo *CServerSpeedTest::GetTestServer()
{
	CSingleLock lock(&m_cCritical_Sites);
	lock.Lock();
	if(m_nNowTest < m_pSiteInfo->m_ayConn.GetCount())
	{
		ConnectInfo *pSrv = m_pSiteInfo->m_ayConn.GetAt(m_nNowTest);
		m_nNowTest ++;
		lock.Unlock();
		return pSrv;
	}
	lock.Unlock();
	return NULL;
}

void CServerSpeedTest::StopTestSpeed()
{
	for (int i=0; i<m_ayTestSpeedThreads.GetCount(); i++)
	{
		HANDLE handle = m_ayTestSpeedThreads.GetAt(i);
		if(WaitForSingleObject(handle,100L) == WAIT_TIMEOUT)
			TerminateThread(handle, 1L);
		CloseHandle(handle);
		m_ayTestSpeedThreads.RemoveAt(i);
		i--;
	}
	m_nNowTest = 0;
}

ConnectInfo *CServerSpeedTest::GetBestConnector()
{
	StopTestSpeed();
	if(!TestSpeed())
		return NULL;
	CalBestConnector();
	WaitForSingleObject(m_hThreadBest,INFINITE);//等待计算最优站点线程退出
	StopTestSpeed();
	ConnectInfo *pSrv = GetBestSite();
	return pSrv;
}
void CServerSpeedTest::CalBestConnector()
{
	UINT uiThreadId = 0;
	m_hThreadBest = (HANDLE)_beginthreadex(NULL,	// Security attributes
		0,	// stack
		CalBestConnThreadProc,	// Thread proc
		this,	// Thread param
		CREATE_SUSPENDED,	// creation mode
		&uiThreadId);	// Thread ID
	if ( NULL != m_hThreadBest)
	{
		ResumeThread( m_hThreadBest );
	}
}
UINT WINAPI CServerSpeedTest::CalBestConnThreadProc(LPVOID pParam)
{
	CServerSpeedTest* pThis = reinterpret_cast<CServerSpeedTest*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunCalBest();

	return 1L;
}
void CServerSpeedTest::RunCalBest()
{
	CSingleLock lock(&m_cCritical_Wait);
	while (TRUE)
	{
		lock.Lock();
		if (m_lWaitOut < ULONG_MAX && m_lBeginWait < ULONG_MAX)
		{
			DWORD tm = GetTickCount();	
			DWORD wait = tm - m_lBeginWait;
			if (wait < m_lWaitOut)
			{
				DWORD waitTime = m_lWaitOut - wait;
				lock.Unlock();
				WaitForSingleObject(m_hEventTestSpeed,waitTime);
			}
			else
			{
				lock.Unlock();
				break;
			}
		}
		else
		{
			lock.Unlock();
			if(WaitForSingleObject(m_hEventTestSpeed,DEFAULT_TIMEOUT_MSEC) != WAIT_OBJECT_0)
			{
				break;
			}
		}
	}
}
void CServerSpeedTest::StopCalBest()
{
	if (m_hThreadBest)
	{
		if(WaitForSingleObject(m_hThreadBest,100L) == WAIT_TIMEOUT)
			TerminateThread(m_hThreadBest, 1L);
		CloseHandle(m_hThreadBest);
		m_hThreadBest = NULL;
	}
}