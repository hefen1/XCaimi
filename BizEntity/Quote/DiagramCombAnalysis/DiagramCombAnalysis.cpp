// DiagramCombAnalysis.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DiagramCombAnalysis.h"
#include "HSMessage.h"
#include "WinnerApplication.h"
#include "HSBizDialog.h"
#include "DlgCombAnalysisReport.h"
#include "CombAnalysisTech.h"
#include "MultiCombAnalysisTech.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDiagramCombAnalysisApp

BEGIN_MESSAGE_MAP(CDiagramCombAnalysisApp, CWinApp)
END_MESSAGE_MAP()


// CDiagramCombAnalysisApp 构造

CDiagramCombAnalysisApp::CDiagramCombAnalysisApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDiagramCombAnalysisApp 对象

CDiagramCombAnalysisApp theApp;
ICombCacheSet*                g_pCacheSet  = NULL;
// CDiagramCombAnalysisApp 初始化

BOOL CDiagramCombAnalysisApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	CWinApp::InitInstance();
	AfxInitRichEdit();
	return TRUE;
}

ICombCacheSet* GetCombCacheSet()
{	
	return g_pCacheSet;
}

int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(g_pCacheSet == NULL)
	{
		g_pCacheSet = (ICombCacheSet*)CWinnerApplication::GetObject(OBJ_COMBCACHESET) ;
	}
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

void PASCAL OnMenuMessage (const char* szMsgId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(szMsgId == NULL)
		return;

	if( _tcscmp(szMsgId,"DiagramCombAnalysis.CombAnalysisTech") == 0 )
	{
		IMainWnd* pMainWnd = (IMainWnd*)CWinnerApplication::GetObject("IMainWnd");
		pMainWnd->ShowPanel("comb_anaysis",SP_SHOW);
	}
}

IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pBizWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);

	if(_tcscmp(name,_T("WndCombAnalysisReprot")) == 0)
	{
		CDlgCombAnalysisReport *pDlg = new CDlgCombAnalysisReport();
		pDlg->Create(CDlgCombAnalysisReport::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pBizWnd = (IBizWindow*)pDlg;
	}
	return pBizWnd;
}

extern "C" IDrawBase* WINAPI GetDiagramInstance(const char* name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(_tcscmp(name,_T("CombAnalysisTech")) == 0)
	{
		CCombAnalysisTech* pDraw = new CCombAnalysisTech();
		return pDraw;
	}
	else if(_tcscmp(name,_T("MultiCombAnalysisTech")) == 0)
	{
		CMultiCombAnalysisTech* pDraw = new CMultiCombAnalysisTech();
		return pDraw;
	}
	return NULL;
}