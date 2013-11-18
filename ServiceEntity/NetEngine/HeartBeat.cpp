#include "StdAfx.h"
#include "HeartBeat.h"
#include "ConnectorManager.h"
#include "ConnectorImp.h"
#include "QuoteConnector.h"
#include "TradeConnector.h"
#include "TradePacker.h"
#include "..\..\Common\WinnerApplication.h"
#include "..\..\Common\TradeComm\TradingIntf.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CConnectorManagerImp g_ConManager;
IHsCommunication * CHeartBeat::m_pCommCfg = NULL;
ITradeCoreObj* CHeartBeat::m_pTradeCore = NULL;
CHeartBeat::CHeartBeat(long handle, OnConnectNotify noti)
{
	if ( m_pCommCfg == NULL )
		m_pCommCfg = (IHsCommunication*)CWinnerApplication::GetObject(OBJ_COMM);
	if ( m_pTradeCore == NULL )
		m_pTradeCore = (ITradeCoreObj *)CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hThread = NULL;
	m_lHandle = handle;
	m_fNotify = noti;
	StartHeartBeat();
}

CHeartBeat::~CHeartBeat(void)
{
	StopHeartBeat();
	if(m_hEvent != NULL)
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}
	m_lHandle = 0;
}

void CHeartBeat::StartHeartBeat()
{
	UINT uiThreadId = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
		0,	// stack
		HeartBeatThreadProc,	// Thread proc
		this,	// Thread param
		CREATE_SUSPENDED,	// creation mode
		&uiThreadId);	// Thread ID
	if ( NULL != m_hThread)
	{
		ResumeThread( m_hThread );
	}
}

void CHeartBeat::StopHeartBeat()
{
	SetEvent(m_hEvent);
	if(m_hThread)
	{
		if(WaitForSingleObject(m_hThread,1000L) == WAIT_TIMEOUT)
			TerminateThread(m_hThread, 1L);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void CHeartBeat::RunHeartBeat()
{
	CConnector *pConn = (CConnector*)m_lHandle;
	if (CConnectorManagerImp::IsHQServerType(pConn->GetType()))
	{
		RunHeartBeatHQ();
	}
	else
	{
		RunHeartBeatJY();
	}
}
void CHeartBeat::RunHeartBeatHQ()
{
	CQuoteConnector *pConn = (CQuoteConnector*)m_lHandle;
	_ASSERTE( pConn != NULL );
	int nTimeout = m_pCommCfg->GetHeartBeat(CEV_Connect_HQ);
	UINT sessionID = 0;
	while (WaitForSingleObject(m_hEvent,nTimeout) != WAIT_OBJECT_0)
	{
		if (sessionID == 0)
		{
			sessionID = pConn->GetSessionID();
		}
		if( sessionID > 0)//已经登录
		{
			char *pRcv;
			UINT len,nTime = 10000;
			int nRetLen = sizeof(AskData) - sizeof(CodeInfo) + sizeof(ReqSrvSync);
			AskData *pData = (AskData*)new char[nRetLen];
			memset(pData, 0, nRetLen);

			pData->m_nType = RT_SRV_SYNC;
			pData->m_nIndex = 0;
			pData->m_lKey = pConn->CreatePacketID();
			pData->m_nSize = sizeof(ReqSrvSync);
			ReqSrvSync *req = (ReqSrvSync *)pData->m_pCode;
			req->client_sessionId = sessionID;
			TRACE1(_T("发送心跳包！Type=%d\n"),pConn->GetType());
			if(pConn->SendRcv((const char*)pData,nRetLen,nTime,pRcv,len) == -1)//收心跳超时
			{
				TRACE1(_T("收心跳包超时！Type=%d\n"),pConn->GetType());
				if(m_fNotify)
				{
					ConnectNotify noti;
					noti.m_cType = pConn->GetType();
					noti.m_lHandle = m_lHandle;
					noti.m_result = Drop;
					m_fNotify(&noti);
				}
			}
			else
				pConn->FreeMem(pRcv);
			delete[] pData;
		
			TRACE1(_T("收到心跳包！Type=%d\n"),pConn->GetType());
		}
	}
}
void CHeartBeat::RunHeartBeatJY()
{
	CTradeConnector *pConn = (CTradeConnector*)m_lHandle;
	_ASSERTE( pConn != NULL );
	int nTimeout = m_pCommCfg->GetHeartBeat(CEV_Connect_JY);
	UINT nTime = 10000;
	while (WaitForSingleObject(m_hEvent,nTimeout) != WAIT_OBJECT_0)
	{
		TRACE1(_T("发送交易心跳包！Type=%d\n"),pConn->GetType());
		IReqHeart *pTrade = m_pTradeCore->NewReqHeart();
		pTrade->SetConnectHandle(m_lHandle);
		if ( !pTrade->TradingSync(nTime) )
		{
			TRACE1(_T("收交易心跳包超时！Type=%d\n"),pConn->GetType());
			pTrade->Release();
			return ;
		}
		TRACE1(_T("收到交易心跳包！Type=%d\n"),pConn->GetType());
		pTrade->Release();
	}
}
UINT WINAPI CHeartBeat::HeartBeatThreadProc(LPVOID pParam)
{
	CHeartBeat *pThis = reinterpret_cast<CHeartBeat*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->RunHeartBeat();

	return 1L;
}