// NetEngine.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "NetEngine.h"
#include "./ConnectorManager.h"
#include "./ChannelsImp.h"
#include "../../Common/WinnerApplication.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CNetEngineApp

BEGIN_MESSAGE_MAP(CNetEngineApp, CWinApp)
END_MESSAGE_MAP()


// CNetEngineApp 构造

CNetEngineApp::CNetEngineApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CNetEngineApp 对象

CNetEngineApp theApp;
CConnectorManagerImp *g_ConManager = NULL;
CChannelsImp *g_Channel = NULL;
NetEngineJYAPI g_NetJYAPI;
// CNetEngineApp 初始化
CConnectorManagerImp *GetNetManager()
{
	if (!g_ConManager)
	{
		g_ConManager = new CConnectorManagerImp;
	}
	return g_ConManager;
}
CChannelsImp *GetNetChannel()
{
	if (!g_Channel)
	{
		g_Channel = new CChannelsImp;
	}
	return g_Channel;
}

BOOL CNetEngineApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	HINSTANCE hDll; //DLL句柄
	hDll = CWinnerApplication::GetModuleHandle(_T("HsJyCommd.dll"));
	if (hDll == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,_T("winner"),"NetEngineDll加载HsJyCommd.dll失败！");
		return FALSE;
	}
	g_NetJYAPI.JYConnect = (NetEngineJYConnect)GetProcAddress(hDll,"hjcei_connect"); 
	g_NetJYAPI.JYCreateEI = (NetEngineJYCreateEI)GetProcAddress(hDll,"hjcei_create_ei"); 
	g_NetJYAPI.JYCreatePM = (NetEngineJYCreatePM)GetProcAddress(hDll,"hjcpm_create_pm"); 
	g_NetJYAPI.JYGetHeader = (NetEngineJYGetHeader)GetProcAddress(hDll,"hjcpm_get_header_value");
	g_NetJYAPI.JYParse = (NetEngineJYParse)GetProcAddress(hDll,"hjcpm_parse_proto_mesg"); 
	g_NetJYAPI.JYReceive = (NetEngineJYReceive)GetProcAddress(hDll,"hjcei_read_mesg"); 
	g_NetJYAPI.JYSend = (NetEngineJYSend)GetProcAddress(hDll,"hjcei_write_mesg"); 
	g_NetJYAPI.JYSetProxy = (NetEngineJYSetProxy)GetProcAddress(hDll,"hjcei_set_proxy"); 
	g_NetJYAPI.JYDestroyEI = (NetEngineJYDestroyEI)GetProcAddress(hDll,"hjcei_destroy_ei"); 
	g_NetJYAPI.JYCompose = (NetEngineJYCompose)GetProcAddress(hDll,"hjcpm_compose");
	g_NetJYAPI.JYSetHeader = (NetEngineJYSetHeader)GetProcAddress(hDll,"hjcpm_set_header_value");
	g_NetJYAPI.JYGetProtoUnit = (NetEngineJYGetProtoUnit)GetProcAddress(hDll,"hjcpm_get_proto_unit");
	g_NetJYAPI.JYDestroyPM = (NetEngineJYDestroyPM)GetProcAddress(hDll,"hjcpm_destroy_pm");
	g_NetJYAPI.JYGetMac = (NetEngineJYGetMac)GetProcAddress(hDll, "HS_GetConnInfo");
	if (!g_NetJYAPI.Loaded())
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,_T("winner"),"NetEngineDll加载HsJyCommd.dll导出函数失败！");
		//return FALSE;
	}

	return TRUE;
}

int PASCAL InitEntity()
{
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	if (g_ConManager)
	{
		delete g_ConManager;
		g_ConManager = NULL;
	}
	if (g_Channel)
	{
		delete g_Channel;
		g_Channel = NULL;
	}
	return TRUE;
}

void* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if(_tcscmp(szName,_T("ChannelManager")) == 0)
	{
		return GetNetManager();
	}
	if(_tcscmp(szName,_T("Channels")) == 0)
	{
		return GetNetChannel();
	}
	return NULL;
}	


