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
	CCriticalSection m_cCritical_Wait;       //�ٽ�������
	HANDLE m_hEventTestSpeed;           //�����¼�
	HANDLE m_hThreadBest;           //��������վ���߳�
	ConnectInfo *m_pBestSite;				//����վ��
	CCriticalSection m_cCritical_Best;       //�ٽ�������
	SiteInfo *m_pSiteInfo;
	CCriticalSection m_cCritical_Sites;       //�ٽ�������
	TradeCert *m_pCert;
	TestSpeedCallBack fCallBack;
	int m_nNowTest;				//��ǰ���ٵ�վ��
	CArray<HANDLE,HANDLE>    m_ayTestSpeedThreads;

protected:
	void CalBestConnector();//������������վ���߳�
	static UINT WINAPI CalBestConnThreadProc(LPVOID pParam);
	void RunCalBest();//��������վ���̺߳���
	void StopCalBest();//ֹͣ��������վ���߳�
	void SetSite(SiteInfo *psite,TestSpeedCallBack tsCallBack,TradeCert *pTrade);
	BOOL TestSpeed();//���������߳�
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
