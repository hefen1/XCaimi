// QuoteSumGeneralSort.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "QuoteGeneralSort.h"
#include "QuoteGeneralSortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
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

// CQuoteSumGeneralSortApp

BEGIN_MESSAGE_MAP(CQuoteGeneralSortApp, CWinApp)
END_MESSAGE_MAP()


// CQuoteSumGeneralSortApp 构造

CQuoteGeneralSortApp::CQuoteGeneralSortApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CQuoteSumGeneralSortApp 对象

CQuoteGeneralSortApp theApp;


// CQuoteSumGeneralSortApp 初始化

BOOL CQuoteGeneralSortApp::InitInstance()
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
IBizWindow* PASCAL GetEntityInstance(const char* name, HWND  hOwner)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(name == NULL)
		return NULL;

	IBizWindow *pQuoteGeneralSortWnd = NULL;
	CWnd *pParentWnd = NULL;
	if(::IsWindow(hOwner))
		pParentWnd = CWnd::FromHandle(hOwner);
	if(_tcscmp(name,_T("WndQuoteGeneralSort")) == 0)
	{
		QuoteGeneralSortDlg *pDlg = new QuoteGeneralSortDlg();
		//pDlg->CreateStatic(pParentWnd,3,3);
		pDlg->Create(QuoteGeneralSortDlg::IDD,pParentWnd);
	/*	pDlg->ShowWindow(SW_SHOW);*/
		pQuoteGeneralSortWnd = (IBizWindow*)pDlg;
	}
	return pQuoteGeneralSortWnd;
}
