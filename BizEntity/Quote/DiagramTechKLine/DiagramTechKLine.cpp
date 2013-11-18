// DiagramTechKLine.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DiagramTechKLine.h"
#include "TechKLineMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDiagramTechKLineApp

BEGIN_MESSAGE_MAP(CDiagramTechKLineApp, CWinApp)
END_MESSAGE_MAP()


// CDiagramTechKLineApp 构造

CDiagramTechKLineApp::CDiagramTechKLineApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDiagramTechKLineApp 对象

CDiagramTechKLineApp theApp;


// CDiagramTechKLineApp 初始化

BOOL CDiagramTechKLineApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" int WINAPI InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

extern "C" int WINAPI UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

extern "C" IDrawBase* WINAPI GetDiagramInstance(const char* name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char szDiagram[] = OBJ_DIAGRAM_TECHKLINE;
	char* pszCompName = strchr(szDiagram, '.');
	if(pszCompName != NULL && strnicmp((char*)(pszCompName + 1), name, strlen(name)) == 0)
	{
		CTechKLineMgr* pTeckKLineNew = new CTechKLineMgr;
		return pTeckKLineNew;
	}
	return NULL;
}