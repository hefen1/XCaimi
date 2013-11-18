// LogService.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LogService.h"
#include "HSLogger.h"

#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLogServiceApp

BEGIN_MESSAGE_MAP(CLogServiceApp, CWinApp)
END_MESSAGE_MAP()


// CLogServiceApp 构造

CLogServiceApp::CLogServiceApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLogServiceApp 对象

CLogServiceApp theApp;
CHSLogger g_hsLogger;


// CLogServiceApp 初始化

BOOL CLogServiceApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int PASCAL InitEntity()
{
	CString strPath;
	DWORD dwSize=MAX_PATH;
	::GetModuleFileName(NULL,strPath.GetBuffer(MAX_PATH),dwSize); //AfxGetResourceHandle()
	strPath.ReleaseBuffer(dwSize);
	CString strAppPath=strPath.Left(strPath.ReverseFind('\\')+1);
	CString strLogCfg = strAppPath + _T("config\\log.config");
	g_hsLogger.LogInit(strLogCfg);
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	return TRUE;
}

void* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if(_tcscmp(szName,_T("HSLogger")) == 0)
		return (void*)&g_hsLogger;

	return NULL;
}	

