#pragma once
#include "QuoteDiagram.h"

typedef int(WINAPI *F_InitEntity)();
typedef int(WINAPI *F_UninitEntity)();
typedef IDrawBase*(WINAPI *F_GetDiagramInstance)(const char*);

class CDiagramCreater
{
public:
	friend class CDiagramFactory; 
	CDiagramCreater()
	{
		m_pInit = NULL;
		m_pUninit = NULL;
		m_pGetInstance = NULL;
	}

	~CDiagramCreater()
	{
		
	}
	void ReleaseModule()
	{
		if(m_handle)
			::FreeLibrary(m_handle);
	}

	BOOL Init(HINSTANCE h);
	IDrawBase* CreateDiagram(CString strName);
private:
	F_InitEntity           m_pInit;
	F_UninitEntity         m_pUninit;
	F_GetDiagramInstance   m_pGetInstance;
	HINSTANCE              m_handle;
	CString                 m_strModuleName;
};

class CDiagramFactory
{
public:
	CDiagramFactory(void);
	~CDiagramFactory(void);
	
	IDrawBase* CreateDiagram(CString strLocation);
private:
	CMapStringToPtr  m_createrMap;
};
