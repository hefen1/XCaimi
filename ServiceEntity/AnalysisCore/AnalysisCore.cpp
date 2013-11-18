// AnalysisCore.cpp : ���� DLL �ĳ�ʼ�����̡�
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


// CAnalysisCoreApp ����

CAnalysisCoreApp::CAnalysisCoreApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAnalysisCoreApp ����

CAnalysisCoreApp			theApp;
CAnalysisTableSet			g_AnalysisTableSet;
CCombManager			g_CombManager;
CCombCacheSet            g_CombCacheSet;

// CAnalysisCoreApp ��ʼ��

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

//�ͷŲ��
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