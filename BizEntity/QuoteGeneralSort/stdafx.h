// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afx.h>

#include "Resource.h"
#include "..\..\Common\hsds_comudata.h"
#include "..\..\Common\hscoldefine.h"
#include "..\..\Common\hsothercoldefine.h"
#include "..\..\Common\configDefined.h"
#include "..\..\Common\hsstructnew.h"
#include "..\..\Common\WinnerApplication.h"
#include "..\..\Controls\CommControls\GridCtrl\GridCell.h"
#include "..\..\Controls\CommControls\HSGridCtrl.h"
#include "..\..\Controls\CommControls\GridCtrl\GridCellBase.h"
#include "..\..\Controls\\CommControls\GridCtrl\GridBaseDefine.h"
#include "..\..\Common\ConfigInterface.h"
#include "..\..\HelperLib\GeneralHelper.h"
#include <oledb.h>
#include <afxtempl.h>
#include <afxbutton.h>