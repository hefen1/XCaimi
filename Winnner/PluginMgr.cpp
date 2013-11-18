#include "stdafx.h"
#include "Winner.h"
#include "PluginMgr.h"
#include "tinyxml.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString CBizPluginMgr::m_strClassName = _T("CBizPluginMgr");
const CString CServicePluginMgr::m_strClassName = _T("CServicePluginMgr");

CBizPluginMgr::CBizPluginMgr()
{
	
}

CBizPluginMgr::~CBizPluginMgr()
{
	while(m_plugList.GetCount() > 0)
	{
		BizPlugin* p = m_plugList.GetHead();
		ASSERT(p!=NULL);
#ifdef _DEBUG
		CString str;
		str.Format("释放服务插件:%s\n",p->m_strName);
		OutputDebugString(str);
#endif
		if(p->pUninit)
			p->pUninit();
		::FreeLibrary(p->m_handle);
		delete p;
		m_plugList.RemoveHead();
	}
}

BizPlugin* CBizPluginMgr::GetPlugin(CString strName)
{
	BizPlugin* p = NULL;
	POSITION pos = m_plugList.GetHeadPosition();
	for (int i=0;i < m_plugList.GetCount();i++)
	{
		p = m_plugList.GetNext(pos);
		ASSERT(p != NULL);
		if(p->m_strName == strName)
		{
			return p;	
		}	
	}

	p = new BizPlugin();
	CModuleLoader* pModuleLoader = HsGetObjectManager()->GetModuleLoader();
	ASSERT(pModuleLoader != NULL);
	p->m_handle = pModuleLoader->GeModuleHandle(strName);
	p->m_strName = strName;
	if(p->m_handle)
	{
		FARPROC proc = NULL;
		proc = GetProcAddress(p->m_handle,"InitEntity");
		p->pInit = reinterpret_cast<InitEntity>(proc);

		proc = GetProcAddress(p->m_handle,"UninitEntity");
		p->pUninit = reinterpret_cast<UninitEntity>(proc);

		proc = GetProcAddress(p->m_handle,"GetEntityInstance");
		p->pGetInstance = reinterpret_cast<GetEntityInstance>(proc);

		proc = GetProcAddress(p->m_handle,"OnFrameNotify");
		p->pNotify = reinterpret_cast<OnFrameNotify>(proc);

		p->pInit();

		m_plugList.AddHead(p);
	}
	else
	{
		delete p;
		p = NULL;
	}
	return p;
}

IBizWindow* CBizPluginMgr::CreateBizWnd(CString strLocation,HWND hParent)
{	
	CString strPlugin,strWndName;
	int nPos = strLocation.Find('.');
	strPlugin = strLocation.Left(nPos);
	strWndName = strLocation.Right(strLocation.GetLength() - nPos - 1);
    BizPlugin* pPlugin = GetPlugin(strPlugin + _T(".dll"));
	if(pPlugin)
	{
		return pPlugin->pGetInstance(strWndName,hParent);
	}
	return NULL;
}

int CBizPluginMgr::NotifyToPlugin(int nFlag)
{
	BizPlugin* p = NULL;
	POSITION pos = m_plugList.GetHeadPosition();
	for (int i=0;i < m_plugList.GetCount();i++)
	{
		p = m_plugList.GetNext(pos);
		ASSERT(p != NULL);
		if(p->pNotify != NULL)
			p->pNotify(nFlag);
	}
	return 0;
}


CServicePluginMgr::CServicePluginMgr()
{
}

CServicePluginMgr::~CServicePluginMgr()
{
	OutputDebugString(_T("[CServicePluginMgr::~CServicePluginMgr] freeing...\n"));	
}

void CServicePluginMgr::FinalizePlugin()
{
	while(m_plugList.GetCount() > 0)
	{
		ServicePlugin* p = m_plugList.GetHead();
		ASSERT(p!=NULL);
#ifdef _DEBUG
		CString str;
		str.Format("释放服务插件:%s\n",p->m_strName);
		OutputDebugString(str);
#endif
		p->pUninit();
		::FreeLibrary(p->m_handle);	
		delete p;
		m_plugList.RemoveHead();
	}
}

ServicePlugin* CServicePluginMgr::GetPlugin(CString strName)
{		
	ServicePlugin* p = NULL;
	POSITION pos = m_plugList.GetHeadPosition();
	for (int i=0;i < m_plugList.GetCount();i++)
	{
		p = m_plugList.GetNext(pos);
		ASSERT(p != NULL);
		if(p->m_strName == strName)
		{
			return p;	
		}
	}

	p = new ServicePlugin();
	CModuleLoader* pModuleLoader = HsGetObjectManager()->GetModuleLoader();
	ASSERT(pModuleLoader != NULL);
	p->m_handle = pModuleLoader->GeModuleHandle(strName);
	p->m_strName = strName;
	if(p->m_handle)
	{
		FARPROC proc = NULL;
		proc = GetProcAddress(p->m_handle,"InitEntity");
		p->pInit = reinterpret_cast<InitEntity>(proc);

		proc = GetProcAddress(p->m_handle,"UninitEntity");
		p->pUninit = reinterpret_cast<UninitEntity>(proc);

		proc = GetProcAddress(p->m_handle,"GetServiceObject");
		p->pGetServiceObject = reinterpret_cast<GetServiceObj>(proc);

		proc = GetProcAddress(p->m_handle,"OnFrameNotify");
		p->pNotify = reinterpret_cast<OnFrameNotify>(proc);

		p->pInit();
		m_plugList.AddHead(p);
	}
	else
	{
		delete p;
		p = NULL;
	}
	return p;
}

void* CServicePluginMgr::GetServiceObject(CString strLocation,CString strPrefix/* = _T("")*/)
{	
	CString strPlugin,strObjName;
	int nPos = strLocation.Find('.');
	strPlugin = strLocation.Left(nPos);
	strObjName = strLocation.Right(strLocation.GetLength() - nPos - 1);
	ServicePlugin* pPlugin = GetPlugin(strPlugin + _T(".dll"));
	if(pPlugin)
	{
		return pPlugin->pGetServiceObject(strObjName,strPrefix);
	}
	return NULL;
}

int CServicePluginMgr::NotifyToPlugin(int nFlag)
{
	ServicePlugin* p = NULL;
	POSITION pos = m_plugList.GetHeadPosition();
	for (int i=0;i < m_plugList.GetCount();i++)
	{
		p = m_plugList.GetNext(pos);
		ASSERT(p != NULL);
		if(p->pNotify != NULL)
			p->pNotify(nFlag);
	}
	return 0;
}

BOOL CServicePluginMgr::LoadInitServicePlugin(CString strFileName)
{
	TiXmlDocument xmldoc;
	if( !xmldoc.LoadFile(strFileName) )
	{
		CString strError;
		strError.Format(_T("分析布局文件(%s)失败.\r\n行:%d 列:%d\r\n错识原因:%s"),
			strFileName,xmldoc.ErrorRow(),xmldoc.ErrorCol(),xmldoc.ErrorDesc());
		CWinnerApplication::FormatLog(ERROR_LOG_LEVEL,LN_WINNER,strError);
		return FALSE;
	}
	
	TiXmlElement* pRoot = xmldoc.FirstChildElement(_T("SysConfig"));
	if(pRoot == NULL)
		return FALSE;
	TiXmlElement* pElem = pRoot->FirstChildElement(_T("InitLoadServicePlugin"));
	if(pElem)
	{
		pElem = pElem->FirstChildElement(_T("Item"));
		while(pElem)
		{
			CString strPluginName = pElem->Attribute(_T("name"));
			GetPlugin(strPluginName);
			pElem = pElem->NextSiblingElement(_T("Item"));
		}
	}

	return TRUE;
}