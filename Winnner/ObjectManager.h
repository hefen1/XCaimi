#pragma once

#include "ModuleLoader.h"

class CObjectManager: public CObject
{
protected:
	CMapStringToPtr   m_objMap;
public:
	CObjectManager(void);
	~CObjectManager(void);

	void ClearObjects();

	BOOL AddObj(CString strName,CObject *pObj);
	void* GetObj(CString strObjectLocation,CString strPrefix = _T(""));

	CModuleLoader* GetModuleLoader();
};
