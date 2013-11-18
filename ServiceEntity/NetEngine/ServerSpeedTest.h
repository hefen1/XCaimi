#pragma once
#include <afxmt.h>
#include "NetEngineStructDefine.h"

class CServerSpeedTest
{
public:
	CServerSpeedTest(SiteInfo *psite,TestSpeedCallBack tsCallBack,TradeCert *pTrade);
	~CServerSpeedTest(void);
	ConnectInfo *GetBestConnector();

protected:
	Server_type m_srvType;
	DWORD m_lBeginWait;
	DWORD m_lWaitOut;
	CCriticalSection m_cCritical_Wait;       //临界区对象
	HANDLE m_hEventTestSpeed;           //测速事件
	HANDLE m_hThreadBest;           //计算最优站点线程
	ConnectInfo *m_pBestSite;				//最优站点
	CCriticalSection m_cCritical_Best;       //临界区对象
	SiteInfo *m_pSiteInfo;
	CCriticalSection m_cCritical_Sites;       //临界区对象
	TradeCert *m_pCert;
	TestSpeedCallBack fCallBack;
	int m_nNowTest;				//当前测速的站点
	CArray<HANDLE,HANDLE>    m_ayTestSpeedThreads;

protected:
	void CalBestConnector();//创建计算最优站点线程
	static UINT WINAPI CalBestConnThreadProc(LPVOID pParam);
	void RunCalBest();//计算最优站点线程函数
	void StopCalBest();//停止计算最优站点线程
	void SetSite(SiteInfo *psite,TestSpeedCallBack tsCallBack,TradeCert *pTrade);
	BOOL TestSpeed();//创建测速线程
	void StopTestSpeed();
	static UINT WINAPI TestSpeedThreadProc(LPVOID pParam);
	void  RunTestSpeed();
	void  RunTestSpeedHQ();
	void  RunTestSpeedJY();
	void  CalcResultHQ(char* pData,ConnectInfo *pSrv,DWORD sendTime);
	void  CalcResultJY(char* pData,ConnectInfo *pSrv,DWORD sendTime);
	BOOL  SetWaitOut(DWORD wait);
	void SetBestSite(ConnectInfo* pSrv);
	ConnectInfo *GetTestServer();
	ConnectInfo *GetBestSite();
};
