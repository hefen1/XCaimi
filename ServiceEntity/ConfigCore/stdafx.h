// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#ifdef _DEBUG
	#pragma comment(lib,"HelperLibSD.lib")
	#pragma message("Automatically linking with HelperLibSD.lib") 

	//#pragma comment(lib,"tinyxmlSD.lib")
    #pragma comment(lib,"tinyxpathd.lib")
	#pragma message("Automatically linking with tinyxpathd.lib") 

	#pragma warning(disable:4099)
#else
	#pragma comment(lib,"HelperLibS.lib")
	#pragma message("Automatically linking with HelperLibS.lib") 

	//#pragma comment(lib,"tinyxml.lib")
	#pragma comment(lib,"tinyxpath.lib")
	#pragma message("Automatically linking with tinyxpath.lib") 


#endif

#include "..\..\HelperLib\GeneralHelper.h"