#include "StdAfx.h"
#include "ModuleLoader.h"

#include "..\HelperLib\GeneralHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


const CString CModuleLoader::m_strClassName = _T("CModuleLoader");

CModuleLoader::CModuleLoader(void)
{
}

CModuleLoader::~CModuleLoader(void)
{
	OutputDebugString(_T("[CModuleLoader::~CModuleLoader] freeing...\n"));
	while(m_moduleList.GetCount() > 0)
	{
		CModuleInfo* p = m_moduleList.GetHead();
		ASSERT(p!=NULL);
		delete p;
		m_moduleList.RemoveHead();
	}
}


HINSTANCE CModuleLoader::LoadModule(CString strName)
{
	HINSTANCE hModule = NULL;
	//加载模块
	//1.app目录(exe所在目录)
	CString strModuleFile = CGeneralHelper::GetAppPath() + strName;
	if( PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//2.app/trade目录
	strModuleFile = CGeneralHelper::GetAppPath() +  _T("trade\\") + strName;
	if( hModule == NULL && PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//3.app/quote目录
	strModuleFile = CGeneralHelper::GetAppPath() +  _T("quote\\") + strName;
	if( hModule == NULL && PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//4.系统path目录
	if(hModule == NULL)
		hModule = ::LoadLibrary(strModuleFile);

	return hModule;
}

HINSTANCE CModuleLoader::FindModule(CString strName)
{
	POSITION pos = m_moduleList.GetHeadPosition();
	for(int i = 0 ; i < m_moduleList.GetCount();i++)
	{
		CModuleInfo *pModuleInfo = m_moduleList.GetNext(pos);
		if(strName.Compare(pModuleInfo->GetName()) == 0)
		{
			return pModuleInfo->GetHandle();
		}
	}
	return NULL;
}


BOOL CModuleLoader::AddModuleInfo(CString strName,HINSTANCE hModule)
{
	if( hModule == NULL)
		return FALSE;
	CModuleInfo *pModuleInfo = new CModuleInfo(strName,hModule);
	m_moduleList.AddTail(pModuleInfo);
	return TRUE;
}

HINSTANCE CModuleLoader::GeModuleHandle(CString strName)
{
	//加锁保护
	CMutexHelper lock(&m_mutex);

	//返回存在的句柄
	HINSTANCE hModule = FindModule(strName);
	if( hModule == NULL)
	{
		hModule = LoadModule(strName);
		if(hModule != NULL)
			AddModuleInfo(strName,hModule);
	}
	return hModule;
}