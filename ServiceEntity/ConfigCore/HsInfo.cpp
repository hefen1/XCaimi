#include "StdAfx.h"
#include "HsInfo.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsInfo::CHsInfo(void)
{
	m_pConfigBase = NULL;    // �ļ�������ָ��
	m_bInited = FALSE;       // �Ƿ��ʼ��
	m_strRootPath = "//Infomation"; // �߼����ڵ�
}

CHsInfo::~CHsInfo(void)
{
	Release(TRUE);
}

void CHsInfo::Init()
{
	m_pConfigBase = gFileManager.GetSystemCfg();
	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsInfo::Release( BOOL bSaveToFile /*= TRUE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
	return TRUE;
}

BOOL CHsInfo::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}

	return TRUE;
}

CString CHsInfo::GetInfoProvider()
{
	CString strPath  = m_strRootPath + "/InfoProvider";
	return m_pConfigBase->GetNodeAttrString(strPath, "value"); 
}
