// QuoteReport.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportDlg.h"
#include "WinnerApplication.h"
#include "BlockAdscriptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CQuoteReportApp

BEGIN_MESSAGE_MAP(CQuoteReportApp, CWinApp)
END_MESSAGE_MAP()


// CQuoteReportApp 构造

CQuoteReportApp::CQuoteReportApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CQuoteReportApp 对象

CQuoteReportApp theApp;


// CQuoteReportApp 初始化

BOOL CQuoteReportApp::InitInstance()
{
	CWinApp::InitInstance();

	HINSTANCE hRes = (HINSTANCE)CWinnerApplication::GetModuleHandle(_T("QuoteLang.dll"));
	if (hRes)
	{
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价模块-重设资源句柄为：%x",hRes);
		AfxSetResourceHandle(hRes);
	}
	return TRUE;
}

int PASCAL InitEntity()
{
	
	return TRUE;
}

//释放插件
int PASCAL UninitEntity()
{
	return TRUE;
}

//
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow* pWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndQuoteReport")) == 0)
	{
		CQuoteReportDlg *pDlg = new CQuoteReportDlg(pParentWnd);
		pDlg->Create(CQuoteReportDlg::IDD,pParentWnd);
		pDlg->ShowWindow(SW_SHOW);
		pWnd = (IBizWindow*)pDlg;
	}
	else if (_tcscmp(name, _T("WndBlockAdscription")) == 0)
	{
		CBlockAdscriptionDlg* pDLg= new CBlockAdscriptionDlg(pParentWnd);
		pDLg->Create(CBlockAdscriptionDlg::IDD, pParentWnd);
		pDLg->ShowWindow(SW_SHOW);
		pWnd = (IBizWindow*)pDLg;
	}
	return pWnd;
}
