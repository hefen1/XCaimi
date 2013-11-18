// DiagramHolder.cpp : 定义 DLL 的初始化例程。
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


// CDiagramHolderApp 构造

CDiagramHolderApp::CDiagramHolderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDiagramHolderApp 对象

CDiagramHolderApp theApp;
CDiagramFactory* g_DiagramFactory = NULL;


// CDiagramHolderApp 初始化

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

//释放插件
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

	//公式管理
	if( _tcscmp(szMsgId,MA_OPENQUOTEREPORT) == 0)
	{
		IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject("IMainWnd");
		CString strId;
		strId.Format("%d", 0);
		CLinkageParam param;
		param.m_bPost = FALSE;
		param.m_nMsg = QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_QuoteReport); // 添加消息号
		param.AddParam(PARAM_DIAGRAMMSGID, strId); // 添加消息号
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
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_CombAnalysis); // 添加消息号
		param.AddParam(PARAM_DIAGRAMMSGID, strId); // 添加消息号
		pMainWnd->DispatchLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT,(WPARAM)&param,0,FALSE);
	}
}