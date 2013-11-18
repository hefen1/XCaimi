#include "StdAfx.h"
#include "DiagramFactory.h"
#include "WinnerApplication.h"

BOOL CDiagramCreater::Init(HINSTANCE h)
{
	m_handle = h;
	if ( h && m_pInit == NULL)
	{
		FARPROC proc = NULL;
		proc = GetProcAddress(h,"InitEntity");
		this->m_pInit = reinterpret_cast<F_InitEntity>(proc);

		proc = GetProcAddress(h,"UninitEntity");
		this->m_pUninit = reinterpret_cast<F_UninitEntity>(proc);

		proc = GetProcAddress(h,"GetDiagramInstance");
		this->m_pGetInstance = reinterpret_cast<F_GetDiagramInstance>(proc); 

		if( m_pInit)
			m_pInit();

		return TRUE;
	}
	else  if (m_pInit != NULL)
	{
		return TRUE;
	}

	return FALSE;
}

IDrawBase* CDiagramCreater::CreateDiagram(CString strName)
{
	if(m_pGetInstance)
		return m_pGetInstance(strName);
	else
		return NULL;
}

CDiagramFactory::CDiagramFactory(void)
{
	
}

CDiagramFactory::~CDiagramFactory(void)
{
	POSITION pos;
	CString key;
	CDiagramCreater* pCreater = NULL;
	for( pos = m_createrMap.GetStartPosition(); pos != NULL; )
	{
		m_createrMap.GetNextAssoc( pos, key, (void*&)pCreater);
		if(pCreater)
		{
			//对“DiagramCombAnalysis”模块特殊处理。因为DiagramCombAnalysis同时也是bizwindow插件，会造型多释放
			if(pCreater->m_pUninit &&  pCreater->m_strModuleName != "DiagramCombAnalysis")
			{
				pCreater->m_pUninit();
				pCreater->ReleaseModule();
			}
			delete pCreater;
		}
	}
	m_createrMap.RemoveAll();
}

IDrawBase* CDiagramFactory::CreateDiagram(CString strLocation)
{
	CString strPlugin,strDiagramName;
	int nPos = strLocation.Find('.');
	strPlugin = strLocation.Left(nPos);
	strDiagramName = strLocation.Right(strLocation.GetLength() - nPos - 1);

	CDiagramCreater* pCreater = NULL;
	if( !m_createrMap.Lookup(strPlugin,(void*&)pCreater) )
	{
		pCreater = new CDiagramCreater();
		HINSTANCE h = CWinnerApplication::GetModuleHandle(strPlugin + _T(".dll"));
		pCreater->m_strModuleName = strPlugin;
		pCreater->Init(h);
		m_createrMap.SetAt(strPlugin,pCreater);
	}
	return pCreater->CreateDiagram(strDiagramName);
}