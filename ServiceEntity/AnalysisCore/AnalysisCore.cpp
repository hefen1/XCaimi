// AnalysisCore.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AnalysisCore.h"
#include "AnalysisTableImp.h"
#include "CombManager.h"
#include "CombCacheImp.h"
#include "IndicatrixObjImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAnalysisCoreApp

BEGIN_MESSAGE_MAP(CAnalysisCoreApp, CWinApp)
END_MESSAGE_MAP()


// CAnalysisCoreApp 构造

CAnalysisCoreApp::CAnalysisCoreApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAnalysisCoreApp 对象

CAnalysisCoreApp			theApp;
CAnalysisTableSet			g_AnalysisTableSet;
CCombManager			g_CombManager;
CCombCacheSet            g_CombCacheSet;

// CAnalysisCoreApp 初始化

BOOL CAnalysisCoreApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

void* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if (_tcscmp(szName, _T("CAnalysisTableSet")) == 0)
	{
		return &g_AnalysisTableSet;
	}
	else if (_tcscmp(szName, _T("CCombManager")) == 0)
	{
		return &g_CombManager;
	}
	else if (_tcscmp(szName, _T("CWatcherInfoManager")) == 0)
	{
		return (void*)new CWatcherInfoManager();
	}
	else if (_tcscmp(szName, _T("CIndicatrixObj")) == 0)
	{
		return new CIndicatrixObj();;
	}
	else if (_tcscmp(szName, _T("CCombCacheSet")) == 0)
	{
		return GetCombCacheSet();
	}
	return NULL;
}

CCombCacheSet* GetCombCacheSet()
{
	static BOOL bInit = FALSE;
	if(!bInit)
	{
		g_CombCacheSet.InitCacheSet();
		bInit = TRUE;
	}
	return &g_CombCacheSet;
}