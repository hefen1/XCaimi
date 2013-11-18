// Login.cpp : 定义 DLL 的初始化例程。
//
#include "stdafx.h"
#include "Login.h"
#include "DlgSpeedTest.h"
#include "DoLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CFrameLoginDlg g_dlgFrameLogin;
// CDlgLogin     g_dlgLogin;
CDlgSpeedTest* g_pDlgSpeedTest = NULL;
CDoLogin*      g_pDoLogin = NULL;

BEGIN_MESSAGE_MAP(CLoginApp, CWinApp)
END_MESSAGE_MAP()


// CWinnerStatusApp 构造

CLoginApp::CLoginApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLoginApp 对象

CLoginApp theApp;


// CLoginApp 初始化

BOOL CLoginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 插件实现函数
int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (g_pDoLogin == NULL)
	{
		g_pDoLogin = new CDoLogin;
		g_pDoLogin->Init();
	}
	if (g_pDlgSpeedTest == NULL)
		g_pDlgSpeedTest = new CDlgSpeedTest;
	
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (g_pDoLogin)
	{
		delete g_pDoLogin;
		g_pDoLogin = NULL;
	}
	if (g_pDlgSpeedTest)
	{
		if (::IsWindow(g_pDlgSpeedTest->GetSafeHwnd()))
		{
			g_pDlgSpeedTest->DestroyWindow();
		}
		delete g_pDlgSpeedTest;
		g_pDlgSpeedTest = NULL;
	}
	return TRUE;
}

void* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_tcscmp(szName, _T("LoginObj")) == 0 && g_pDoLogin != NULL)
	{
		return (void*)g_pDoLogin;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////