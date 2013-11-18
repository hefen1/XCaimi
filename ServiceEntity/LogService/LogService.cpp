// LogService.cpp : ���� DLL �ĳ�ʼ�����̡�
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


// CLogServiceApp ����

CLogServiceApp::CLogServiceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLogServiceApp ����

CLogServiceApp theApp;
CHSLogger g_hsLogger;


// CLogServiceApp ��ʼ��

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

//�ͷŲ��
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

