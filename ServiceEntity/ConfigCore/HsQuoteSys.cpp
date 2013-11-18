#include "StdAfx.h"
#include "HsQuoteSys.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsQuoteSys::CHsQuoteSys(void)
{
	m_bInited = FALSE;
	m_pConfigBase = NULL;
}

CHsQuoteSys::~CHsQuoteSys(void)
{
	Release(FALSE);
}

void CHsQuoteSys::Init()
{
	m_pConfigBase = gFileManager.GetSystemCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsQuoteSys::Release( BOOL bSaveToFile /*= TRUE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
	return TRUE;
}

BOOL CHsQuoteSys::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}

	return TRUE;
}

BOOL CHsQuoteSys::IsSupportLevel2()
{
	CString strPath = "//HsSystem/SupportLevel2";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

BOOL CHsQuoteSys::IsSupportFuture()
{
	CString strPath = "//HsSystem/SupportFuture";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

CString CHsQuoteSys::GetQuoteServerInitMarket()
{
	CString strPath = "//HsSystem/ServerIntiMarket/QuoteServer";
	return m_pConfigBase->GetNodeAttrString(strPath, "value");
}

CString CHsQuoteSys::GetFutureServerInitMarket()
{
	CString strPath = "//HsSystem/ServerIntiMarket/FutureServer";
	return m_pConfigBase->GetNodeAttrString(strPath, "value");
}