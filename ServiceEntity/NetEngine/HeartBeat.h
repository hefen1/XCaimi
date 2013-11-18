#pragma once
#include <afxmt.h>
#include "../../Common/NetEngineInterface.h"
#include "ConnectorManager.h"
#include "../../Common/ConfigInterface.h"
#include "../../Common/TradeComm/TradeCoreIntf.h"
class CHeartBeat
{
public:
	CHeartBeat(long handle, OnConnectNotify noti);
	~CHeartBeat(void);

protected:
	long m_lHandle;
	HANDLE m_hThread;
	HANDLE m_hEvent;
	static IHsCommunication *m_pCommCfg;
	static ITradeCoreObj *m_pTradeCore;
	OnConnectNotify m_fNotify;

protected:
	void StartHeartBeat();
	void StopHeartBeat();
	static UINT WINAPI HeartBeatThreadProc(LPVOID pParam);
	void RunHeartBeat();
	void RunHeartBeatHQ();
	void RunHeartBeatJY();
};
