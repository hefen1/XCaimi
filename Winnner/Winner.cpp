
// Winnner.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "Winner.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "LayoutView.h"
#include "PluginMgr.h"
#include "BizWindowContext.h"
#include "ModuleLoader.h"

#include "..\..\HelperLib\GeneralHelper.h"
#include "..\Common\LogService.h"
#include "..\Common\WinnerApplication.h"

//#define Visual_Leak_Detector
#ifdef Visual_Leak_Detector
	#include "..\..\Controls\vld-10\vld.h"
	#pragma comment(lib,"vldmtdll.lib")
	#pragma message("Automatically linking with vldmtdll.lib") 
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CObjectManager g_objectManager;
CModuleLoader g_moduleLoader;
CServicePluginMgr g_ServicePluginMgr;
CLinkageMsgManager g_LinkageMsgManager;
CHotKeyManager g_HotKeyManager;
CWinnerApp theApp;

CLogTimeSapn::CLogTimeSapn(CString strFlag)
{
	m_strFlag = strFlag;
	m_lBegin = GetTickCount();
}

CLogTimeSapn::~CLogTimeSapn()
{
	long lSpan = GetTickCount() - m_lBegin;
	CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,_T("winner"),_T("[%s]span time = %d"),m_strFlag,lSpan);
}

CShellCommandInfo::CShellCommandInfo()
{
	for (int i = 1; i < __argc; i++)
	{
		LPCTSTR pszParam = __targv[i];
		CString str = pszParam;
		int nPos = str.Find("=");
		CString strName = str.Left(nPos);
		CString strKey = str.Right(str.GetLength() - nPos - 1);
		m_cmdInfoMap.SetAt(strName,strKey);		
	}
}

CString CShellCommandInfo::GetStringParam(CString strParamName,CString strDefault)
{
	CString strValue;
	if ( m_cmdInfoMap.Lookup(strParamName,strValue) )
		return strValue;
	else
		return strDefault;
}

int CShellCommandInfo::GetIntegerParam(CString strParamName,int nDefault)
{
	CString strValue;
	if ( m_cmdInfoMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atoi(strValue);
	}
	return nDefault;
}

double CShellCommandInfo::GetDoubleParam(CString strParamName,double dDefault)
{
	CString strValue;
	if ( m_cmdInfoMap.Lookup(strParamName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atof(strValue);
	}
	return dDefault;
}

BEGIN_MESSAGE_MAP(CWinnerApp, CWinAppEx)
END_MESSAGE_MAP()

CWinnerApp::CWinnerApp()
{
	m_hUnique = NULL;
}


void CWinnerApp::CreateGlobalObjec()
{
	//业务窗口上下文对象
	CBizWindowContext* pBizWndContext = new CBizWindowContext();
	CString strBizWndFile = CGeneralHelper::GetAppPath() + _T("config\\biz_windows.xml");
	pBizWndContext->BuildDesignMenu(strBizWndFile);
	HsGetObjectManager()->AddObj(CBizWindowContext::m_strClassName,pBizWndContext);

	//加载初始服务插件
	CString strInitLoadSvrPluginFile = CGeneralHelper::GetAppPath() + _T("config\\sysconfig.xml");
	HsGetServicePluginMgr()->LoadInitServicePlugin(strInitLoadSvrPluginFile);

	//创建插件管理器
	CBizPluginMgr  *plugMgr = new CBizPluginMgr();
	HsGetObjectManager()->AddObj(CBizPluginMgr::m_strClassName,plugMgr);
}

//typedef int ( CALLBACK * FUNC ) ( PCHAR );
BOOL CWinnerApp::InitInstance()
{
// 	m_hUnique = ::CreateMutex(NULL, FALSE, UNIQUE_ID);
// 	if (GetLastError() == ERROR_ALREADY_EXISTS) 
// 	{
// 		::MessageBox(NULL,_T("同样的程序已经有一个被打开了"),
// 			_T("提示"),MB_OK|MB_ICONEXCLAMATION|MB_SETFOREGROUND);
// 		return FALSE;
// 	}
	/*m_hModule  = LoadLibrary ( "Skin.dll" );
	CString strPath;

	CHAR szPath [ MAX_PATH + 1 ];
	if ( GetModuleFileName ( NULL, szPath, MAX_PATH ) )
	{
		strPath = szPath;

		int nPos = strPath.ReverseFind ( '\\' );

		if ( nPos >= 0 ) strPath = strPath.Left ( nPos + 1 );
	}

	CHAR szName [ MAX_PATH ];
	CString strName = strPath + "skins\\DefaultSkin.skn";
	FUNC pFunc = ( FUNC ) GetProcAddress ( theApp.m_hModule, "LoadSkin" );
	strcpy ( szName, strName );
	pFunc ( szName );*/
	LoginRet loginResult = LoginRet_Error;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// initializes Microsoft Windows GDI+
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	CWinAppEx::InitInstance();

	LOGFONT logfont = {0};
	::SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &logfont, 0);
	afxGlobalData.SetMenuFont(&logfont,true);


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		return FALSE;
	}
	AfxEnableControlContainer();

	SetRegistryKey(_T("HSWinner"));

	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	//创建全局对象
	CreateGlobalObjec();

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	pMainFrame->ShowWindow(SW_HIDE);

	CShellCommandInfo cmdInfo;
	CString strNotLogin = cmdInfo.GetStringParam(_T("notlogin"));
	if(strNotLogin != _T("true"))
	{
		loginResult = DoLogin(LoginType_Normal);
		if( loginResult == LoginRet_Error)
			return FALSE;
	}	
	else
	{
		loginResult = LoginRet_Quote;
	}

	pMainFrame->OpenPanel(PN_STATUSBAR);
	if(loginResult == LoginRet_Quote || loginResult == LoginRet_QuoteAndTrade )
	{
		CString strMainPage = cmdInfo.GetStringParam(_T("mainpage"));
		if ( !strMainPage.IsEmpty())
			pMainFrame->OpenPanel(strMainPage);
		else
			//pMainFrame->OpenPanel(PN_QUOTEDIAGRAM);
			pMainFrame->OpenPanel("comb_anaysis");
	}
	if(loginResult == LoginRet_Trade || loginResult == LoginRet_QuoteAndTrade)
	{
		if(pMainFrame->IsTradeLogin())
		{
			pMainFrame->OpenPanel(PN_TRADE);
			if(loginResult == LoginRet_Trade)
				(CMainFrame*)pMainFrame->ShowPanel(PN_TRADE,SP_MAX);
		}
	}

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	
	return TRUE;
}

LoginRet CWinnerApp::DoLogin(LoginType eType)
{
	IDoLogin *pLogin = (IDoLogin*)HsGetObjectManager()->GetObj(OBJ_LOGIN);
	ASSERT(pLogin!=NULL);
	return pLogin->Login(eType);
}

int CWinnerApp::ExitInstance()
{
	g_ServicePluginMgr.FinalizePlugin();
	HsGetObjectManager()->ClearObjects();

	//cleans up resources used by Microsoft Windows GDI+
	GdiplusShutdown(m_pGdiToken);

	if(m_hUnique)
		CloseHandle(m_hUnique);

	return CWinAppEx::ExitInstance();
}

BOOL CWinnerApp::LoadState(LPCTSTR lpszSectionName, CFrameImpl* pFrameImpl)
{
	return TRUE;
}

BOOL CWinnerApp::SaveState(LPCTSTR lpszSectionName, CFrameImpl* pFrameImpl)
{
	return TRUE;
}

BOOL CWinnerApp::LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd)
{
// 	 __super::LoadWindowPlacement(rectNormalPosition,nFflags,nShowCmd);
// 	 int nWidth = rectNormalPosition.Width();
// 	 int nHeight = rectNormalPosition.Height();
// 	 //当窗口大小异常时作最大化处理
// 	 if(  nWidth <= 100 || nHeight <= 100 )
// 	 {
// 		rectNormalPosition.SetRectEmpty();
// 		nShowCmd = SW_MAXIMIZE;
// 	 }
// 	 AfxGetApp()->m_nCmdShow = nShowCmd;
	AfxGetApp()->m_nCmdShow = SW_MAXIMIZE;
	 return TRUE;
}

BOOL CWinnerApp::StoreWindowPlacement(const CRect& rectNormalPosition, int nFflags, int nShowCmd)
{
	return TRUE;
	//__super::StoreWindowPlacement(rectNormalPosition,nFflags,nShowCmd);
}

CObjectManager *HsGetObjectManager()
{
	return &g_objectManager;
}

CServicePluginMgr *HsGetServicePluginMgr()
{
	return &g_ServicePluginMgr;
}

CLinkageMsgManager* HsGetLinkageMsgManager()
{
	return &g_LinkageMsgManager;
}

CHotKeyManager* HsGetHotKeyManager()
{
	return &g_HotKeyManager; 
}

IMainWnd* HsGetMainWnd()
{
	return (CMainFrame*)theApp.m_pMainWnd; 
}

extern "C" 
{
	HMODULE  PASCAL HswGetModuleHandle(const char* szModuleName)
	{
		AFX_MANAGE_STATE(AfxGetAppModuleState());
		return g_moduleLoader.GeModuleHandle(szModuleName);
	}

	void*  PASCAL HswGetObject(const char* szObjectLocation,const char* szPrefix = NULL)
	{
		AFX_MANAGE_STATE(AfxGetAppModuleState());
		return g_objectManager.GetObj(szObjectLocation,szPrefix);
	}
}

BOOL CWinnerApp::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_KEYDOWN)
	{
		//以下代码为了解决在dll弹出的非模态对话框上不接受按键消息的问题。
		char szClassName[128];
		// 看当前接收消息的窗口是不是对话框
		HWND hwnd = pMsg->hwnd;
		::GetClassName(hwnd, szClassName, 128);
		if (strcmp(szClassName, "#32770") == 0) // #32770 是对话框的类名
		{
			if (::IsDialogMessage(hwnd, pMsg)) 
				return TRUE;
		}
		else
		{
			// 看当前接收消息的窗口的父窗口是不是对话框
			::GetClassName(::GetParent(hwnd), szClassName, 128);
			if (strcmp(szClassName, "#32770") == 0)
			{
 				if (::IsDialogMessage(::GetParent(hwnd), pMsg)) 
 				{
 					//if(pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN )					
 					::SendMessage(::GetParent(hwnd),pMsg->message,pMsg->wParam,pMsg->lParam);
					if(pMsg->wParam != VK_ESCAPE)
 						return TRUE;
 				}	
			}
		}
	}
	return CWinAppEx::PreTranslateMessage(pMsg);
}
