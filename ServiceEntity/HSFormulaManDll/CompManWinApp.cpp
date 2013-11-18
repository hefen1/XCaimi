// CompManWinApp.cpp: implementation of the CCompManWinApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CompManWinApp.h"
#include "DlgFormulaMan.h"
#include "FormulaExport.h"
#include "HSWinner.h"
#include "PubStruct.h"
#include "Express.h"
#include "HSDataSource.h"
#include "HSMessage.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDlgFormulaMan* g_pFormulaMan;
extern CFormulaExport* g_HSCompExport;
extern CHSDataSource*  g_pHxDataSource;
extern CMapStringToPtr* g_pTableHeadData;

extern BOOL g_bIsExeFile;
CCompManWinApp theApp;
 
 CCompManWinApp::CCompManWinApp()
 {
 }
 
 CCompManWinApp::~CCompManWinApp()
 {
 
 }
 
 BOOL CCompManWinApp::PreTranslateMessage(MSG* pMsg) 
 {	
 	return CWinApp::PreTranslateMessage(pMsg);
 }
 
 BOOL CCompManWinApp::InitInstance()
 {
 	// Install the CoolControls 
  //  GetCtrlManager().InstallHook();
 
 	static CString strProfile =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + "CompMan.INI";
 	m_pszProfileName = strdup(strProfile);
 
 	return CWinApp::InitInstance();
 }
 
 int CCompManWinApp::ExitInstance()
 {
 	// TODO: Add your specialized code here and/or call the base clas
 	if ( g_HSCompExport && !IsBadReadPtr(g_HSCompExport,1) )
 	{
 		delete g_HSCompExport;
 		g_HSCompExport = NULL;
 	}
 	if ( g_pFormulaMan && !IsBadReadPtr(g_pFormulaMan,1) )
 	{
 		g_pFormulaMan->DestroyWindow();
 		delete g_pFormulaMan;
 		g_pFormulaMan = NULL;
 	}
 	if ( g_pHxDataSource && !IsBadReadPtr(g_pHxDataSource,1) )
 		delete g_pHxDataSource;
 	g_pHxDataSource = NULL;
 	return CWinApp::ExitInstance();
 }
int PASCAL InitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd* pWnd = AfxGetMainWnd();
	if (g_HSCompExport == NULL)
	{
		g_HSCompExport = new CFormulaExport;
		g_HSCompExport->initExpress(NULL, 0);
	}
  
	return 1;
}

//释放插件
int PASCAL UninitEntity()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( g_HSCompExport && !IsBadReadPtr(g_HSCompExport,1) )
	{
		delete g_HSCompExport;
		g_HSCompExport = NULL;
	}
	if ( g_pFormulaMan && !IsBadReadPtr(g_pFormulaMan,1) )
	{
		g_pFormulaMan->DestroyWindow();
		delete g_pFormulaMan;
		g_pFormulaMan = NULL;
	}
	if ( g_pHxDataSource && !IsBadReadPtr(g_pHxDataSource,1) )
		delete g_pHxDataSource;
	g_pHxDataSource = NULL;

	return 1;
}

IFormulaMan* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if(_tcscmp(szName,_T("FormulaManager")) == 0)
	{
		if (g_HSCompExport == NULL)
			g_HSCompExport = new CFormulaExport;
		return g_HSCompExport;
	}
	return NULL;
}	


void PASCAL OnMenuMessage (const char* szMsgId)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(szMsgId == NULL)
		return;
	if (g_HSCompExport == NULL)
	{
		g_HSCompExport = new CFormulaExport;
		g_HSCompExport->initExpress(NULL, 0);
	}
	
	//公式管理
	if(_tcscmp(szMsgId,MA_FORMULAMANANAGER) == 0)
	{
		if ( g_HSCompExport )
			g_HSCompExport->OpenCompile(NULL, 0);
	}
	//公式查找
	else if(_tcscmp(szMsgId,MA_FORMULAFORMULAFINDER) == 0)
	{
		if ( g_HSCompExport )
		{
			/*g_HSCompExport->FindExpression(0);*/
			HSOpenExpressUseData data(hxOpenFindDataDlg);
			g_HSCompExport->OpenExpress(0,(LPARAM)&data);
		}
	}
	//选股
	else if(_tcscmp(szMsgId,MA_SELECTSTOCK) == 0)
	{
		if ( g_HSCompExport )
		{
			HSOpenExpressUseData* data = new HSOpenExpressUseData;
			data->m_wType = hxOpenExpressByConditionStockType;
			data->m_pData = NULL;
			g_HSCompExport->OpenExpress(0,(LPARAM)data);
			delete data;
		}
	}
	//交易测试
	else if(_tcscmp(szMsgId,MA_TRADETEST) == 0)
	{
		if ( g_HSCompExport )
		{
			HSOpenExpressUseData* data = new HSOpenExpressUseData;
			data->m_wType = hxOpenExchangeSys;
			data->m_pData = NULL;
			g_HSCompExport->OpenExpress(0,(LPARAM)data);
			delete data;
		}
	}

}