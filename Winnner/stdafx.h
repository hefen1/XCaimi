
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <afxdhtml.h>

//自定义编绎开关


//包含的lib库
#include <comdef.h>//初始化一下com口
#ifndef ULONG_PTR
#include "GdiPlus.h"
using namespace Gdiplus;
#endif
#pragma comment(lib,"gdiplus.lib")

#ifdef _DEBUG
	#pragma comment(lib,"tinyxpathd.lib")
	#pragma message("Automatically linking with tinyxpathd.lib") 

	#pragma comment(lib,"HelperLibSD.lib")
	#pragma message("Automatically linking with HelperLibSD.lib") 
#else
	#pragma comment(lib,"HelperLibS.lib")
	#pragma message("Automatically linking with HelperLibS.lib")

	#pragma comment(lib,"tinyxpath.lib")
	#pragma message("Automatically linking with tinyxmlS.lib") 
#endif










