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
	//����ģ��
	//1.appĿ¼(exe����Ŀ¼)
	CString strModuleFile = CGeneralHelper::GetAppPath() + strName;
	if( PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//2.app/tradeĿ¼
	strModuleFile = CGeneralHelper::GetAppPath() +  _T("trade\\") + strName;
	if( hModule == NULL && PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//3.app/quoteĿ¼
	strModuleFile = CGeneralHelper::GetAppPath() +  _T("quote\\") + strName;
	if( hModule == NULL && PathFileExists(strModuleFile) ) 
	{
		hModule = ::LoadLibrary(strModuleFile);
	}

	//4.ϵͳpathĿ¼
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
	//��������
	CMutexHelper lock(&m_mutex);

	//���ش��ڵľ��
	HINSTANCE hModule = FindModule(strName);
	if( hModule == NULL)
	{
		hModule = LoadModule(strName);
		if(hModule != NULL)
			AddModuleInfo(strName,hModule);
	}
	return hModule;
}