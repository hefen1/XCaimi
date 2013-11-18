//////////////////////////////////////////////////////////////////////////
//

#include "stdafx.h"
#include "ConfigManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////////

CConfigManager::CConfigManager()
{
	m_pSysMis = NULL;
	m_bInitSuccess = FALSE;
}

CConfigManager::~CConfigManager()
{
	if (m_pSysMis != NULL)
	{
		m_pSysMis->SaveToFile();
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL CConfigManager::InitEnv()
{
	if (!m_bInitSuccess)
	{
		m_pSysMis = (IHsMiscellany*)CWinnerApplication::GetObject(OBJ_MIS);
		m_bInitSuccess = (m_pSysMis != NULL);
	}
	return m_bInitSuccess;
}


int	CConfigManager::SetDownloadDate(CString strDownloadName, CString strDate)
{
	InitEnv();
	if (m_pSysMis != NULL && !strDownloadName.IsEmpty() && !strDate.IsEmpty())
	{
		return m_pSysMis->SetSectionValue(strDownloadName, strDate);
	}
	return 0;
}

long CConfigManager::GetInitDateByType(CString strSectionType)
{
	InitEnv();
	if (m_pSysMis != NULL && !strSectionType.IsEmpty())
	{
		CString strRet = m_pSysMis->GetSectionValue(strSectionType);
		return atol(strRet);
	}
	return 0;

}