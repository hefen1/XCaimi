// DemoDiagram.cpp : ���� DLL �ĳ�ʼ�����̡�
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


// CDemoDiagramApp ����

CDemoDiagramApp::CDemoDiagramApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDemoDiagramApp ����

CDemoDiagramApp theApp;


// CDemoDiagramApp ��ʼ��

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

//�ͷŲ��
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
