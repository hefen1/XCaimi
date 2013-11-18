// Login.cpp : ���� DLL �ĳ�ʼ�����̡�
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


// CWinnerStatusApp ����

CLoginApp::CLoginApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLoginApp ����

CLoginApp theApp;


// CLoginApp ��ʼ��

BOOL CLoginApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// ���ʵ�ֺ���
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

//�ͷŲ��
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