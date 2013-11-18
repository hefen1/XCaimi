// NetEngine.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CNetEngineApp

BEGIN_MESSAGE_MAP(CNetEngineApp, CWinApp)
END_MESSAGE_MAP()


// CNetEngineApp ����

CNetEngineApp::CNetEngineApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CNetEngineApp ����

CNetEngineApp theApp;
CConnectorManagerImp *g_ConManager = NULL;
CChannelsImp *g_Channel = NULL;
NetEngineJYAPI g_NetJYAPI;
// CNetEngineApp ��ʼ��
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

	HINSTANCE hDll; //DLL���
	hDll = CWinnerApplication::GetModuleHandle(_T("HsJyCommd.dll"));
	if (hDll == NULL)
	{
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,_T("winner"),"NetEngineDll����HsJyCommd.dllʧ�ܣ�");
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
		CWinnerApplication::SimpleLog(DEBUG_LOG_LEVEL,_T("winner"),"NetEngineDll����HsJyCommd.dll��������ʧ�ܣ�");
		//return FALSE;
	}

	return TRUE;
}

int PASCAL InitEntity()
{
	return TRUE;
}

//�ͷŲ��
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


