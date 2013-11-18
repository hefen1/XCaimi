// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3D5699F3_380B_4D41_95A1_9B8AC1EA0963__INCLUDED_)
#define AFX_STDAFX_H__3D5699F3_380B_4D41_95A1_9B8AC1EA0963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT
  
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxrich.h>

#include "resource.h"

#pragma warning( disable : 4275 )  
#pragma warning( disable : 4251 )  
#pragma warning( disable : 4244 )  
#pragma warning( disable : 4006 )  
#pragma warning( disable : 4075 )  
   
// 写出日记信息 定义 2003-08-21
// 写出日记信息
#ifdef WRITE_LOG 
#undef WRITE_LOG 
#endif
#define WRITE_LOG (x,y,z)			WRITE_LOG_MACRO(_T("FormulaManDll"),x,y,z)
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)

#define  SUPPORT_OLD
extern CWnd* GetMainWnd();
/*#define AfxGetMainWnd() GetMainWnd();*/

extern HINSTANCE GetResourceHandle();
//#define AfxGetResourceHandle() GetResourceHandle();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3D5699F3_380B_4D41_95A1_9B8AC1EA0963__INCLUDED_)

#include <afxpriv.h>
#include "hscoldefine.h"
#include "ConfigStruct.h"
#include "GeneralHelper.h"
#include "..\..\BizEntity\Quote\QuoteComm\pageid.h"
#include "..\..\Controls\YtControl\GridCtrl\GridCtrl.h"
#include "hsothercoldefine.h"
#include "FormulaManPublic.h"
#include "FormulaCalc.h"
#include "KeyBoardStruct.h"
#include "hsds_comudata.h"
#include "HSMessage.h"
#include "QuoteComm/QuoteDefine.h"