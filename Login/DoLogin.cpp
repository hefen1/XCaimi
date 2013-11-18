#include "StdAfx.h"
#include "DoLogin.h"
#include "DlgLogin.h"
#include "DlgSpeedTest.h"
#include "..\HelperLib\GeneralHelper.h"
#include "WinnerApplication.h"
#include "NetEngineInterface.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// extern CDlgLogin g_dlgLogin;
// extern CDoLogin* g_pDoLogin;

UINT WINAPI JYThreadProc(LPVOID pParam)
{
	TRACE("进入交易连接线程初始化 \n");
	TradeCert tcInfo;
	tcInfo.m_Type = SSL; 
	strncpy_s(tcInfo.m_ca, _T("hsca"), sizeof(tcInfo.m_ca)); 
	CString sCertFile =  CGeneralHelper::GetAppPath() + _T("Cert\\cert.pem");
	strncpy_s(tcInfo.m_cert, sCertFile, sizeof(tcInfo.m_cert));
	CString sCertKey =  CGeneralHelper::GetAppPath() + _T("Cert\\key.pem");
	strncpy_s(tcInfo.m_key, sCertKey, sizeof(tcInfo.m_key));
	strncpy_s(tcInfo.m_pwd, _T("111111"), sizeof(tcInfo.m_pwd));
	long handle = g_pDoLogin->m_pManager->CreateConnector(CEV_Connect_JY,"inner_trade_test", &tcInfo);
	if(handle > 0)
	{
		g_pDoLogin->m_pManager->DestroyConnect(handle);
	}

	TRACE("交易连接线程退出 \n");
	return 1L;
}
//////////////////////////////////////////////////////////////////////////

void CCommData::SetProxy(ProxyInfo*proxy)
{
	if (proxy)
	{
		if (!m_pProxy)
		{
			m_pProxy = new ProxyInfo;
		}
		memset(m_pProxy,0,sizeof(ProxyInfo));
		m_pProxy->m_nOptions = proxy->m_nOptions;
		m_pProxy->m_nPort = proxy->m_nPort;
		m_pProxy->m_nUseChannel = proxy->m_nUseChannel;
		strncpy(m_pProxy->m_strProxyIP,proxy->m_strProxyIP,128);
		strncpy(m_pProxy->m_strUser,proxy->m_strUser,64);
		strncpy(m_pProxy->m_strPwd,proxy->m_strPwd,64);
	}
}
void CCommData::SetDefault(Server_type type,char*pName)
{
	for (int i=0; i<m_aySiteSys.GetCount(); i++)
	{
		CommInfo *pComm = m_aySiteSys.GetAt(i);
		if(pComm->m_cType == type)
		{
			if (!strcmp(pComm->m_strServerID,pName))
			{
				pComm->m_cIsDefault = 1;
			}
			else
			{
				pComm->m_cIsDefault = 0;
			}
		}
	}
	for (int i=0; i<m_aySiteUsr.GetCount(); i++)
	{
		CommInfo *pComm = m_aySiteUsr.GetAt(i);
		if(pComm->m_cType == type)
		{
			if (!strcmp(pComm->m_strServerID,pName))
			{
				pComm->m_cIsDefault = 1;
			}
			else
			{
				pComm->m_cIsDefault = 0;
			}
		}
	}
}

BOOL CCommData::InitServerInfo()
{
	IHsCommunication *pComm = g_pDoLogin->m_pCommCfg;//theApp.m_pCommConf;
	ITradeConfigObj *pTradeCfg = g_pDoLogin->m_pTradeConfig;
	if (pComm && pTradeCfg)
	{	
		ICellList* pCellList = pTradeCfg->GetCellList();
		int nCellID(-1);
		if (pCellList && pCellList->GetCount() > 0)
		{
			ICellItem* pItem = pCellList->GetItems(0);
			nCellID = atoi(pItem->GetCellID().GetBuffer());
			pItem->Release();
			//////////////////////////////////////////////////////////////////////////
			// 释放从交易配置读取的节点
			pCellList->Release();
			//////////////////////////////////////////////////////////////////////////
		}
		pComm->GetAllSysServer(&m_aySiteSys, nCellID);
		pComm->GetAllUsrServer(&m_aySiteUsr, nCellID);
		ProxyInfo proxy;
		memset(&proxy,0,sizeof(ProxyInfo));
		proxy.m_nOptions = NoProxy;
		proxy.m_nUseChannel = 0;
		pComm->GetProxy(&proxy);
		if(proxy.m_nOptions != NoProxy || proxy.m_nUseChannel != 0)
			SetProxy(&proxy);
		m_bAuto = pComm->GetAuto();
		pComm->GetAllSwitch(&m_aySwitch);
		CMap<Server_type,Server_type,CString,CString> ayDefault;
		pComm->GetAllDefaultSite(&ayDefault, nCellID);
		POSITION pos = ayDefault.GetStartPosition();
		Server_type type;
		CString strName;
		while (pos)
		{
			ayDefault.GetNextAssoc(pos,type,strName);
			if (!strName.IsEmpty())
			{
				SetDefault(type,strName.GetBuffer());
			}
		}
	}
	return TRUE;
}

void CCommData::Save(int nCellID)
{
	IHsCommunication *pComm = g_pDoLogin->m_pCommCfg;
	if (pComm)
	{
		pComm->SetAutoChooseBest(m_bAuto);
		pComm->SaveProxy(m_pProxy);
		pComm->SaveAllUsrServer(&m_aySiteUsr, nCellID);
		pComm->SetAllDefaultSite(&m_aySiteSys, &m_aySiteUsr, nCellID);
		pComm->SaveToFile();
	}
}



//////////////////////////////////////////////////////////////////////////
//
CDoLogin::CDoLogin(void)
{

}

CDoLogin::~CDoLogin(void)
{
	if (WaitForSingleObject(m_hThread, 1000) == WAIT_TIMEOUT )
	{
		::TerminateThread(m_hThread, 0);
	}
	CloseHandle( m_hThread );
	m_hThread = NULL;
}

BOOL CDoLogin::Init()
{
	m_pChannels = (IChannels*)CWinnerApplication::GetObject(OBJ_CHANNELS);
	if (m_pChannels == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"LoginDll加载ichannels失败！");
		return FALSE;
	}
	m_pManager = (IChannelManager*)CWinnerApplication::GetObject(OBJ_CHANNELMANAGER);
	if (m_pManager == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"LoginDll加载ichannelmanager失败！");
		return FALSE;
	}
	m_pTradeCore = (ITradeCoreObj *)CWinnerApplication::GetObject(OBJ_TRADECORE);
	if (m_pTradeCore == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"LoginDll加载ITradeCoreObj失败！");
		return FALSE;
	}
	m_pTradeConfig = (ITradeConfigObj *)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	if(m_pTradeConfig == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"LoginDll加载ITradeConfigObj失败！");
		return FALSE;
	}
	m_pDataSource = (IDataSource*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	if (m_pDataSource == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name,"LoginDll加载IDataSource失败！");
		return FALSE;
	}
	m_pCommCfg = (IHsCommunication*)CWinnerApplication::GetObject(OBJ_COMM);
	if (m_pCommCfg == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name, "LoginDll加载IHsCommunication失败！");
		return FALSE;
	}
	m_pQSysConf = (IHsQuoteSys*)CWinnerApplication::GetObject(OBJ_QUOTESYS);
	if (m_pQSysConf == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name, "LoginDll加载IHsQuoteSys失败！");
		return FALSE;
	}

	m_pKeyboard = (IKeyboard *)CWinnerApplication::GetObject(OBJ_KEYBOARD);
	if (m_pKeyboard == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,Login_Log_Name, "LoginDll加载IKeyboard失败！");
		return FALSE;
	}

	m_commData.InitServerInfo();

	SetSites();
	//创建交易登录线程
	UINT uiThreadId = 0;
	m_hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
		0,	// stack
		JYThreadProc,	// Thread proc
		(void*)this,	// Thread param
		CREATE_SUSPENDED,	// creation mode
		&uiThreadId);	// Thread ID
	if ( NULL != m_hThread)
	{
		::ResumeThread( m_hThread );
	}

	// 	if(CUIMgr::SetUIPath("face\\Login\\login.ui") == E_SETUIPATH)
	// 	{//导入LibUIDK的界面 
	// 		AfxMessageBox("LoginDLL加载UI失败!");
	// 		return FALSE;
	// 	}
	return TRUE;
}

LoginRet CDoLogin::Login( LoginType eType )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDlgLogin dlgLogin;
	dlgLogin.SetSpecialLogin(eType);
	dlgLogin.DoModal();
	if (dlgLogin.IsLoginSuccess())
	{
		return dlgLogin.GetLoginType();
	}
	else
		return LoginRet_Error;
}

void CDoLogin::TestSpeed()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pDlgSpeedTest->DoModal();
}

void CDoLogin::ConnManager()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//g_dlgLogin.CommMan();
}

BOOL CDoLogin::IsLevel2Support()
{
	return m_pQSysConf->IsSupportLevel2();
}
void CDoLogin::SetSites()
{
	int count = m_commData.GetCount();
	if (count <= 0)
	{
		return;
	}
	CommInfo *pComm = new CommInfo[count];
	CArray<CommInfo*,CommInfo*> *SrvAy = m_commData.GetSysServerInfo();
	CArray<CommInfo*,CommInfo*> *SrvAyUsr = m_commData.GetUsrServerInfo();
	int index=0;
	for (int i=0; i<SrvAy->GetCount(); i++)
	{
		CommInfo *p = SrvAy->GetAt(i);
		pComm[index].m_cIsDefault = p->m_cIsDefault;
		pComm[index].m_cType = p->m_cType;
		pComm[index].m_nPort = p->m_nPort;
		memcpy(pComm[index].m_strServerID,p->m_strServerID,sizeof(p->m_strServerID));
		memcpy(pComm[index].m_strServerIP,p->m_strServerIP,sizeof(p->m_strServerIP));
		index ++;
	}
	for (int j=0; j<SrvAyUsr->GetCount(); j++)
	{
		CommInfo *p = SrvAyUsr->GetAt(j);
		pComm[index].m_cIsDefault = p->m_cIsDefault;
		pComm[index].m_cType = p->m_cType;
		pComm[index].m_nPort = p->m_nPort;
		memcpy(pComm[index].m_strServerID,p->m_strServerID,sizeof(p->m_strServerID));
		memcpy(pComm[index].m_strServerIP,p->m_strServerIP,sizeof(p->m_strServerIP));
		index++;
	}
	m_pManager->SetSites(pComm, count, m_commData.GetProxy());
	delete[] pComm;
}

long CDoLogin::ConnectServer( Server_type cType, TradeCert* pCert /*= NULL*/ )
{
	if (m_commData.IsAuto())
	{
		return m_pManager->CreateBestConnector(cType, pCert);
	}
	return m_pManager->CreateConnector(cType, "", pCert);
}
