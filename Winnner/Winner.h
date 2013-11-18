
// Winnner.h : Winnner 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "ObjectManager.h"
#include "PluginMgr.h"
#include "LinkageMsgManager.h"
#include "HotKeyManager.h"

#include "LoginInterface.h"
#include "..\..\Common\HSWinner.h"
#include "..\..\Common\HSMessage.h"

#define  UNIQUE_ID      _T("APP_ID")
class CShellCommandInfo
{
protected:
	CMapStringToString  m_cmdInfoMap;
public:
	CShellCommandInfo();
	CString GetStringParam(CString strParamName,CString strDefault = _T(""));
	int GetIntegerParam(CString strParamName,int nDefault = 0  );
	double GetDoubleParam(CString strParamName,double dDefault = 0 );
};

class CLogTimeSapn
{
public:
	CLogTimeSapn(CString strFlag);
	~CLogTimeSapn();
private:
	long   m_lBegin;
	CString m_strFlag;
};

class CWinnerApp : public CWinAppEx
{
public:
	CWinnerApp();
	HMODULE m_hModule;
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual BOOL LoadState(LPCTSTR lpszSectionName = NULL, CFrameImpl* pFrameImpl = NULL);
	virtual BOOL SaveState(LPCTSTR lpszSectionName = NULL, CFrameImpl* pFrameImpl = NULL);
	virtual BOOL LoadWindowPlacement(CRect& rectNormalPosition, int& nFflags, int& nShowCmd);
	virtual BOOL StoreWindowPlacement(const CRect& rectNormalPosition, int nFflags, int nShowCmd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LoginRet DoLogin(LoginType eType);

	DECLARE_MESSAGE_MAP()
private:
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	HANDLE    m_hUnique;
	void CreateGlobalObjec();	
};

extern CWinnerApp theApp;
CObjectManager* HsGetObjectManager();
CServicePluginMgr* HsGetServicePluginMgr();
CLinkageMsgManager* HsGetLinkageMsgManager();
CHotKeyManager* HsGetHotKeyManager();
IMainWnd* HsGetMainWnd();
