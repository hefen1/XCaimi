#include "StdAfx.h"
#include "Winner.h"
#include "ObjectManager.h"
#include "PluginMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CObjectManager::CObjectManager(void)
{
}

CObjectManager::~CObjectManager(void)
{

}

void CObjectManager::ClearObjects()
{
	OutputDebugString(_T("[CObjectManager::~CObjectManage] freeing...\n"));
	POSITION pos;
	CString key;
	void* pObj = NULL;
	for( pos = m_objMap.GetStartPosition(); pos != NULL; )
	{
		m_objMap.GetNextAssoc( pos, key, pObj);
		if(pObj)
		{
			delete (CObject*)pObj;
		}
	}
	m_objMap.RemoveAll();
}


BOOL CObjectManager::AddObj(CString strName,CObject *pObj)
{
	void* pObjExist = NULL;
	if(m_objMap.Lookup(strName,pObjExist))
		return FALSE;
	m_objMap.SetAt(strName,pObj);
	return TRUE;
}

void* CObjectManager::GetObj(CString strObjectLocation,CString strPrefix)
{
	if(strObjectLocation.Compare(CModuleLoader::m_strClassName) == 0)
	{
		extern CModuleLoader g_moduleLoader;
		return (void*)&g_moduleLoader;
	}
	else if(strObjectLocation.Compare(_T("IMainWnd")) == 0)
	{
		return HsGetMainWnd();
	}
	else 
	{
		void* pObj = NULL;
		if( m_objMap.Lookup(strObjectLocation,pObj) ) 
			return pObj;

		//返回插件内的对象
		extern CServicePluginMgr g_ServicePluginMgr;
		pObj = g_ServicePluginMgr.GetServiceObject(strObjectLocation,strPrefix);
		return pObj;
	}
}

CModuleLoader* CObjectManager::GetModuleLoader()
{
	extern CModuleLoader g_moduleLoader;
	return &g_moduleLoader;
}