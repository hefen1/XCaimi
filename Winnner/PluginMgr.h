#pragma once

#include "..\..\Common\HSWinner.h"

typedef int(WINAPI *InitEntity)();
typedef int(WINAPI *UninitEntity)();
typedef IBizWindow*(WINAPI *GetEntityInstance)(const char*, HWND);
typedef void*(WINAPI *GetServiceObj)(const char*,const char*);
typedef int(WINAPI *OnFrameNotify)(int nFlag);

struct BizPlugin
{
	CString            m_strName;
	HINSTANCE          m_handle;  

	InitEntity          pInit;
	UninitEntity        pUninit;
	GetEntityInstance   pGetInstance;
	OnFrameNotify       pNotify;

	BizPlugin()
	{
		m_strName.Empty();
		m_handle = 0;
		pInit = 0;
		pUninit = 0;
		pGetInstance = 0;
		pNotify = 0;
	}
};

class CBizPluginMgr: public CObject
{
protected:
	CList<BizPlugin*,BizPlugin*> m_plugList;
	BizPlugin* GetPlugin(CString strName);
public:
	static const CString m_strClassName;
	CBizPluginMgr();
	~CBizPluginMgr();	
	IBizWindow* CreateBizWnd(CString strLocation,HWND hParent);
	/********************************************************************************
	* 函数功能 :通知业务插件
	* 函数参数 :int nFlag: 通知标识
	*  返回值  :
	*******************************************************************************/
	int NotifyToPlugin(int nFlag);
};


struct ServicePlugin
{
	CString            m_strName;
	HINSTANCE          m_handle;  

	InitEntity         pInit;
	UninitEntity       pUninit;
	GetServiceObj      pGetServiceObject;
	OnFrameNotify      pNotify;

	ServicePlugin()
	{
		m_strName.Empty();
		m_handle = 0;
		pInit = 0;
		pUninit = 0;
		pGetServiceObject = 0;
		pNotify = 0;
	}
};

class CServicePluginMgr: public CObject
{
protected:
	CList<ServicePlugin*,ServicePlugin*> m_plugList;
	CMapStringToPtr  m_mapObj;
	ServicePlugin* GetPlugin(CString strName);

public:
	static const CString m_strClassName;
	CServicePluginMgr();
	~CServicePluginMgr();	
	BOOL  LoadInitServicePlugin(CString strFileName);
	void* GetServiceObject(CString strLocation,CString strPrefix = _T(""));
	void FinalizePlugin();
	
	/********************************************************************************
	* 函数功能 :通知服务插件
	* 函数参数 :int nFlag: 通知标识
	*  返回值  :
	*******************************************************************************/
	int NotifyToPlugin(int nFlag);
};