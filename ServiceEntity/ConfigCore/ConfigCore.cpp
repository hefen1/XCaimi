// ConfigCore.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "ConfigCore.h"
#include "TradeConfigObj.h"
#include "blockmanager.h"

//////////////////////////////////////////////////////////////////////////
#include "HsColor.h"
#include "HsFont.h"
#include "HsFileManager.h"
#include "HsScrollNews.h"
#include "HsColumnProperty.h"
#include "HsTabCfg.h"
#include "HsCommunication.h"
#include "HsMiscellany.h"
#include "HsQuoteSys.h"
#include "HsGraph.h"
#include "HsUserDataCfg.h"
#include "HsKeyboardCfg.h"
#include "HsInfo.h"
#include "HsKLineTab.h"
#include "HSKLinePositionCtrl.h"
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CConfigCoreApp

BEGIN_MESSAGE_MAP(CConfigCoreApp, CWinApp)
END_MESSAGE_MAP()


// CConfigCoreApp construction

CConfigCoreApp::CConfigCoreApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CConfigCoreApp object

CConfigCoreApp theApp;


CBlockManager gBlockManager;

/////////////////以下是新版本////////////////////////
//begin
CHsFileManager gFileManager;  
CHsColor       gColor;         
CHsFont        gFont;
CHsScrollNews  gScrollNew;
CHsColumnProperty gColunmProp;
CHsTabCfg         gTabCfgEx;
CHsCommunication  gCommCfg;
CHsMiscellany     gMiscellanyCfg;
CHsQuoteSys       gQuoteSysCfg;
CHsGraph          gGraphCfg;
CHsUserDataCfg    gUserDataCfg;
CHsKeyboardCfg    gKeyboardCfg;
CHsInfo           gInfoCfg;
CHsKLineTab       gKLineTab;
CHSLinePositionCtrl g_LinePostionCtrl;
//end
//////////////////////////////////////////////////////////////////////////
//交易配置
CTradeConfigObj gTradeConfigObj;


// CConfigCoreApp initialization

BOOL CConfigCoreApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


int PASCAL InitEntity()
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}
void* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	if (_tcscmp(szName,_T("TradeConfigObj")) == 0)
	{
		return (void*)&gTradeConfigObj;
	}
	if (_tcscmp(szName,_T("CBlockManager")) == 0)
	{
		if (!gBlockManager.IsInit())
		{
			gBlockManager.Init();
		}
		return (void*)&gBlockManager;
	}
//////////////////////////////////////////////////////////////////////////
	
	if (_tcscmp(szName,_T("CHsColor")) == 0)
	{
		if (!gColor.IsInit())
		{
			gColor.Init();
		}
		return (void*)&gColor;
	}

	if (_tcscmp(szName, _T("CHsFont")) == 0)
	{
		if (!gFont.IsInit())
		{
			gFont.Init();
		}
		return (void*)&gFont;
	}

	if (_tcscmp(szName, _T("CHsScrollNews")) == 0)
	{
		if (!gScrollNew.IsInit())
		{
			gScrollNew.Init();
		}
		return (void*)&gScrollNew;
	}

	if (_tcscmp(szName, _T("CHsColumnProperty")) == 0)
	{
		if (!gColunmProp.IsInit())
		{
			gColunmProp.Init();
		}
		return (void*)&gColunmProp;
	}

	if (_tcscmp(szName, _T("CHsTabCfg")) == 0)
	{
		if (!gTabCfgEx.IsInit())
		{
			gTabCfgEx.Init();
		}
		return (void*)&gTabCfgEx;
	}

	if (_tcscmp(szName, _T("CHsCommuncation")) == 0)
	{
		if (!gCommCfg.IsInit())
		{
			gCommCfg.Init();
		}
		return (void*)&gCommCfg;
	}

	if (_tcscmp(szName, _T("CHsMiscellany")) == 0)
	{
		if (!gMiscellanyCfg.IsInit())
		{
			gMiscellanyCfg.Init();
		}
		return (void*)&gMiscellanyCfg;
	}

	if (_tcscmp(szName, _T("CHsQuoteSys")) == 0)
	{
		if (!gQuoteSysCfg.IsInit())
		{
			gQuoteSysCfg.Init();
		}
		return (void*)&gQuoteSysCfg;
	}

	if (_tcscmp(szName, _T("CHsGraph")) == 0)
	{
		if (!gGraphCfg.IsInit())
		{
			gGraphCfg.Init();
		}
		return (void*)&gGraphCfg;
	}

	if (_tcscmp(szName, _T("CHsUserDataCfg")) == 0)
	{
		if (!gUserDataCfg.IsInit())
		{
			gUserDataCfg.Init();
		}
		return (void*)&gUserDataCfg;
	}

	if (_tcscmp(szName, _T("CHsKeyboardCfg")) == 0)
	{
		if (!gKeyboardCfg.IsInit())
		{
			gKeyboardCfg.Init();
		}
		return (void*)&gKeyboardCfg;
	}

	if (_tcscmp(szName, _T("CHsInfoCfg")) == 0)
	{
		if (!gInfoCfg.IsInit())
		{
			gInfoCfg.Init();
		}
		return (void*)&gInfoCfg;
	}

	if (_tcscmp(szName, _T("CHsKLineTab")) == 0)
	{
		if (!gKLineTab.IsInit())
		{
			gKLineTab.Init();
		}
		return (void*)&gKLineTab;
	}

	if (_tcscmp(szName, _T("CHsKLinePosCtrol")) == 0)
	{
		if (!g_LinePostionCtrl.IsInit())
		{
			g_LinePostionCtrl.Init();
		}
		return (void*)&g_LinePostionCtrl;
	}
//////////////////////////////////////////////////////////////////////////
	return NULL;
}