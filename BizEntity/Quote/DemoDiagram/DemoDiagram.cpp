// DemoDiagram.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DemoDiagram.h"
#include "QuoteDiagram.h"
#include "DiagramTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDemoDiagramApp

BEGIN_MESSAGE_MAP(CDemoDiagramApp, CWinApp)
END_MESSAGE_MAP()


// CDemoDiagramApp 构造

CDemoDiagramApp::CDemoDiagramApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDemoDiagramApp 对象

CDemoDiagramApp theApp;


// CDemoDiagramApp 初始化

BOOL CDemoDiagramApp::InitInstance()
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

//
IDrawBase* PASCAL GetDiagramInstance(const char* name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(_tcscmp(name,_T("DiagramTest")) == 0)
	{
		return new CDiagramTest();
	}
	return NULL;
}
