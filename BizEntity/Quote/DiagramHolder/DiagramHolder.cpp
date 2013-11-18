// DiagramHolder.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DiagramHolder.h"
#include "DlgDiagramHolder.h"
#include "QuoteDefine.h"
#include "HSMessage.h"
#include "WinnerApplication.h"
#include "HSBizDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDiagramHolderApp

BEGIN_MESSAGE_MAP(CDiagramHolderApp, CWinApp)
END_MESSAGE_MAP()


// CDiagramHolderApp ����

CDiagramHolderApp::CDiagramHolderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDiagramHolderApp ����

CDiagramHolderApp theApp;
CDiagramFactory* g_DiagramFactory = NULL;


// CDiagramHolderApp ��ʼ��

BOOL CDiagramHolderApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CDiagramFactory* GetDiagramfactory()
{
	return g_DiagramFactory;
}

int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( g_DiagramFactory == NULL)
		g_DiagramFactory = new CDiagramFactory();
	return TRUE;
}

//�ͷŲ��
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_DiagramFactory)
	{
		delete g_DiagramFactory;
		g_DiagramFactory = NULL;
	}
	return TRUE;
}

//
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pBizWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);


	if(_tcscmp(name,_T("WndDiagramHolder")) == 0)
	{
		CDlgDiagramHolder *pDlg = new CDlgDiagramHolder();
		pDlg->Create(CDlgDiagramHolder::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}

void PASCAL OnMenuMessage (const char* szMsgId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(szMsgId == NULL)
		return;

	//��ʽ����
	if( _tcscmp(szMsgId,MA_OPENQUOTEREPORT) == 0)
	{
		IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject("IMainWnd");
		CString strId;
		strId.Format("%d", 0);
		CLinkageParam param;
		param.m_bPost = FALSE;
		param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_QuoteReport); // �����Ϣ��
		param.AddParam(PARAM_DIAGRAMMSGID, strId); // �����Ϣ��
		pMainWnd->DispatchLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,(WPARAM)&param,0,FALSE);
	}
	else if( _tcscmp(szMsgId,MA_OPENCOMBANALYSIS) == 0 )
	{
		IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject("IMainWnd");
		CString strId;
		strId.Format("%d", 0);
		CLinkageParam param;
		param.m_bPost = FALSE;
		param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_CombAnalysis); // �����Ϣ��
		param.AddParam(PARAM_DIAGRAMMSGID, strId); // �����Ϣ��
		pMainWnd->DispatchLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,(WPARAM)&param,0,FALSE);
	}
}