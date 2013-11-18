#include "afxtempl.h"

#pragma once

struct PyjcTable
{
	int start, final;
	CString PyArray;
};

class CPyjcManager
{
protected:
	CMapStringToString  m_MultiMap;
	CList<PyjcTable *>  m_PyjcList;

public:
	void RemoveAll();
	bool Lookup(LPCTSTR key, CString &val);
	void Load(LPCTSTR pPath);
	void Init(LPCTSTR pPath);
	int AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet);
public:
	CPyjcManager(void);
	virtual ~CPyjcManager(void);
};
