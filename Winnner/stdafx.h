
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��
#include <afxdhtml.h>

//�Զ�����￪��


//������lib��
#include <comdef.h>//��ʼ��һ��com��
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










