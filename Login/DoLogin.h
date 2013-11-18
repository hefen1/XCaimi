#pragma once
#include "resource.h"
#include "../Common/NetEngineInterface.h"
#include "../Common/TradeComm/TradeCoreIntf.h"
#include "ConfigInterface.h"
#include "ConfigStruct.h"
#include "DataSourceDefine.h"
#include "DoLogin.h"
#include "DlgSpeedTest.h"
#include "LoginInterface.h"
#include "KeyboardInterface.h"

#define Login_Log_Name _T("login")

class CCommData
{
protected:
	CCommInfoArray m_aySiteSys;
	CCommInfoArray m_aySiteUsr;
	ProxyInfo *m_pProxy;
	BOOL m_bAuto;//�Ƿ��Զ�ѡ�����վ��
	CMap<Server_type,Server_type,BOOL,BOOL> m_aySwitch;
public:
	CCommData()
	{
		m_pProxy = NULL;
		m_bAuto = FALSE;
	}
	~CCommData()
	{
		for (int i=0; i<m_aySiteSys.GetCount(); i++)
		{
			CommInfo *pC = m_aySiteSys.GetAt(i);
			if (pC)
			{
				delete pC;
				pC = NULL;
			}
		}
		m_aySiteSys.RemoveAll();
		for (int i=0; i<m_aySiteUsr.GetCount(); i++)
		{
			CommInfo *pC = m_aySiteUsr.GetAt(i);
			if (pC)
			{
				delete pC;
				pC = NULL;
			}
		}
		m_aySiteUsr.RemoveAll();

		if (m_pProxy)
		{
			delete m_pProxy;
			m_pProxy = NULL;
		}
	}
	BOOL InitServerInfo();
	CArray<CommInfo*,CommInfo*> *GetSysServerInfo(){return &m_aySiteSys;}
	CArray<CommInfo*,CommInfo*> *GetUsrServerInfo(){return &m_aySiteUsr;}
	int GetCount(){return m_aySiteUsr.GetCount() + m_aySiteSys.GetCount();}
	ProxyInfo *GetProxy(){return m_pProxy;}
	BOOL IsAuto(){return m_bAuto;}
	void SetAuto(BOOL ba){ m_bAuto = ba; }
	void Save(int nCellID);
	void SetDefault(Server_type type,char*pName);
	void SetProxy(ProxyInfo*proxy);
	CMap<Server_type,Server_type,BOOL,BOOL> *GetSwitch(){return &m_aySwitch;}
};

class CDoLogin : IDoLogin
{
public:
	CDoLogin(void);
	~CDoLogin(void);
	
	BOOL Init(); // ��ʼ������

	virtual LoginRet Login(LoginType eType);   // ��¼
	virtual void TestSpeed();                  // ����
	virtual void ConnManager();                // ͨѶ����

public:
	CCommData m_commData;
	IChannels *m_pChannels;
	IChannelManager *m_pManager;
	ITradeCoreObj *m_pTradeCore;
	ITradeConfigObj *m_pTradeConfig;
	IDataSource *m_pDataSource;
	IHsCommunication *m_pCommCfg;
	IHsQuoteSys *m_pQSysConf;
	IKeyboard *m_pKeyboard;
	HANDLE m_hThread; //���׵�¼�߳�

	long ConnectServer(Server_type cType, TradeCert* pCert = NULL);
	void SetSites();
	BOOL IsLevel2Support();

};

//////////////////////////////////////////////////////////////////////////
// ��ֵ�д��
extern CDoLogin*      g_pDoLogin;

class CDlgSpeedTest;
extern CDlgSpeedTest* g_pDlgSpeedTest;