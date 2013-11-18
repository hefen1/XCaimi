#include "stdafx.h"
#include "pyjcmanager.h"
#include "DSWriteFile.h"
#include "WriteReadStandardConfig.h"

CPyjcManager::CPyjcManager(void)
{
}

CPyjcManager::~CPyjcManager(void)
{
	RemoveAll();
}

void CPyjcManager::Init(LPCTSTR pPath)
{
	CString strFileName;
	strFileName.Format("%s%s", pPath, PYJC_INI_FILE);
	Load(strFileName);
}

void CPyjcManager::RemoveAll()
{
	m_MultiMap.RemoveAll();

	POSITION pos = m_PyjcList.GetHeadPosition();
	while(pos != NULL)
	{
		PyjcTable *pyjc = m_PyjcList.GetNext(pos);
		delete pyjc;
	}

	m_PyjcList.RemoveAll();
}

bool CPyjcManager::Lookup(LPCTSTR key, CString &val)
{
	if(m_MultiMap.Lookup(key, val))
		return true;

	char buffer[2];
	USES_CONVERSION;
	CStringW wstr = A2W(key);
	wchar_t c = wstr.GetAt(0);

	buffer[1] = 0;

	if(iswdigit(c))
	{
		buffer[0] = _wtoi(wstr) + '0';
		val = buffer;
		return true;
	}
	else if((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z'))
	{
		val = W2T(wstr.MakeUpper());
		return true;
	}

	POSITION pos = m_PyjcList.GetHeadPosition();

	while(pos != NULL)
	{
		PyjcTable *pyjc = m_PyjcList.GetNext(pos);
		if(c >= pyjc->start && c < pyjc->final)
		{
			buffer[0] = pyjc->PyArray.GetAt(c - pyjc->start);
			val = buffer;
			return true;
		}
	}

	return false;
}

void CPyjcManager::Load(LPCTSTR pPath)
{
	int i;
	CString strKey;
	CString strValue;
	CStringArray ayRet, ayVal;
	USES_CONVERSION;

	char buffer[10];
	wchar_t wstr[2];
	CWriteReadStandardConfig::ReadConfigSectionEntry("MULTI", ayRet, pPath);
	CWriteReadStandardConfig::ReadConfigSectionString("MULTI", ayVal, pPath);

	wstr[1] = 0;
	for(i = 0; i < ayRet.GetSize(); i++)
	{
		strValue = ayRet.GetAt(i);
		wstr[0] = (wchar_t)atoi(strValue);
		strKey = W2T(wstr);
		m_MultiMap.SetAt(strKey, ayVal.GetAt(i));
	}

	lstrcpy(buffer, "£Á");
	buffer[8] = 'A';
	buffer[9] = 0;
	for(i = 0; i <= 'Z' - 'A'; i++)
	{
		m_MultiMap.SetAt(buffer, buffer + 8);
		buffer[1]++;
		buffer[8]++;
	}

	int start, final;
	PyjcTable *pPyjcItem;
	CWriteReadStandardConfig::ReadConfigInt("HEAD", "START", start, pPath);
	CWriteReadStandardConfig::ReadConfigInt("HEAD", "FINAL", final, pPath);
  
	for(i = start; i <= final; i++)
	{
		pPyjcItem = new PyjcTable;

		itoa(i, buffer, 10);
		CWriteReadStandardConfig::ReadConfigString(buffer, "RANGE", strValue, pPath);
		AnalyzeStr(strValue, ",", ayRet);
		pPyjcItem->start = atoi(ayRet.GetAt(0));
		pPyjcItem->final = atoi(ayRet.GetAt(1));
		CWriteReadStandardConfig::ReadConfigString(buffer, "TABLE", pPyjcItem->PyArray, pPath);
		ASSERT(pPyjcItem->PyArray.GetLength() == pPyjcItem->final - pPyjcItem->start);

		m_PyjcList.AddTail(pPyjcItem);
	}
}

int CPyjcManager::AnalyzeStr(CString strResource,const char* strDelimit,CStringArray& ayRet)
{
	ayRet.RemoveAll();

	CString strCur;
	strCur.Format("%s",strResource);
	CString tmp;
	char* token = strtok( (char*)LPCSTR(strCur), strDelimit);
	while(token != NULL)
	{
		tmp.Format("%s",token);
		ayRet.Add(tmp);
		token = strtok( NULL, strDelimit);
	}
	return ayRet.GetSize();
}