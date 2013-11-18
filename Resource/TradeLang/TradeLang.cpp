/*******************************************************
  源程序名称:TradeLang.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易资源
  功能说明:  
  作    者:  shenglq
  开发日期:  20100921
*********************************************************/
// TradeLang.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "TradeLang.h"
#include "TradeLangMng.h"

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

// CTradeLangApp

BEGIN_MESSAGE_MAP(CTradeLangApp, CWinApp)
END_MESSAGE_MAP()


// CTradeLangApp 构造

CTradeLangApp::CTradeLangApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTradeLangApp 对象

CTradeLangApp theApp;

CTradeLang gTradeLang;


// CTradeLangApp 初始化

BOOL CTradeLangApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

ITradeLang* PASCAL GetServiceObject(const char* szName,const char* szPrefix)
{
	return &gTradeLang;
}

int PASCAL InitEntity()
{

	return TRUE;
}

int PASCAL UninitEntity()
{
	return TRUE;
}
