#include "StdAfx.h"
#include "HsGraph.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsGraph::CHsGraph(void)
{
	m_bInited = FALSE;
	m_pConfigBase = NULL;
}

CHsGraph::~CHsGraph(void)
{
	Release(TRUE);
}

void CHsGraph::Init()
{
	m_pConfigBase = gFileManager.GetSystemCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsGraph::Release( BOOL bSaveToFile /*= TRUE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
	return TRUE;
}

BOOL CHsGraph::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}

	return TRUE;
}

int CHsGraph::GetIndexTrendAxisDecimal()
{
	CString strPath = "//HsGraph/IndexAxis/TrendDecimal";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

int CHsGraph::GetIndexKLineAxisDecimal()
{
	CString strPath = "//HsGraph/IndexAxis/KLineDecimal";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

CString CHsGraph::GetTabTechIndexDispOrder()
{
	CString strPath = "//HsGraph/TabTechIndex/IndexDispOrder";
	return m_pConfigBase->GetNodeAttrString(strPath, "value");
}
