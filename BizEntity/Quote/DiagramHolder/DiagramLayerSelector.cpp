#include "StdAfx.h"
#include "DiagramLayerSelector.h"
#include "hsstructnew.h"
#include "..\..\HelperLib\GeneralHelper.h"

CDiagramLayerSelector::CDiagramLayerSelector()
{
	m_strSchemeName = _T("default");
}

CDiagramLayerSelector::~CDiagramLayerSelector(void)
{
	CDiagramLayer* pLayer = NULL;
	POSITION pos;
	CString key;
	for( pos = m_layerMap.GetStartPosition(); pos != NULL; )
	{
		m_layerMap.GetNextAssoc( pos, key, pLayer);
		if(pLayer)
		{
			delete pLayer;
		}
	}
}

CDiagramLayer* CDiagramLayerSelector::GetDiagramLayer(CString strLayerName,CodeInfo* pCodeInfo)
{
	if(pCodeInfo == NULL)
		return NULL;
	WORD wMarketType = pCodeInfo->m_cCodeType;
	CDiagramLayer* pLayer = NULL;
	CString strKey;
	strKey.Format(_T("%x_%s"),wMarketType,strLayerName);
	if( m_layerMap.Lookup(strKey,pLayer) )
	{
		return pLayer;
	}
	else
	{
		CString strStandardFile = GetStandardFileName(strLayerName,pCodeInfo);
		if(strStandardFile.IsEmpty())
			return NULL;
		pLayer = new CDiagramLayer(strLayerName);
		m_layerMap.SetAt(strKey,pLayer);
		pLayer->LoadLayout(m_pHostWnd,strStandardFile);
	}
	return pLayer;
}


CString CDiagramLayerSelector::GetStandardFileName(CString strLayerName,CodeInfo* pCodeInfo)
{
	CFileFind finder;
	CString strLayoutDir,strLayoutFileName;
	strLayoutDir.Format("%slayout\\diagram\\%s", CGeneralHelper::GetAppPath(),m_strSchemeName);

	if(pCodeInfo == NULL)
	{
		strLayoutFileName.Format(_T("%s\\Public\\%s.layout"),strLayoutDir,strLayerName);
		return strLayoutFileName;
	}
	
	//先找每个市场的指定目录
	WORD wMarketType = pCodeInfo->m_cCodeType;
	if( (wMarketType & 0x00FF) == KIND_INDEX && wMarketType != 0)//指数
	{
		switch(wMarketType & 0x0F00)
		{
		case SH_BOURSE:
			strLayoutFileName.Format(_T("%s\\SH\\Index\\%s.layout"),strLayoutDir,strLayerName);
			break;
		case SZ_BOURSE:
			strLayoutFileName.Format(_T("%s\\SZ\\Index\\%s.layout"),strLayoutDir,strLayerName);
			break;
		}

		if( !finder.FindFile(strLayoutFileName) )
		{
			strLayoutFileName.Format(_T("%s\\Public\\Index\\%s.layout"),strLayoutDir,strLayerName);
		}
	}
	else //个股
	{
		switch(wMarketType & 0x0F00)
		{
		case SH_BOURSE:
			strLayoutFileName.Format(_T("%s\\SH\\%s.layout"),strLayoutDir,strLayerName);
			break;
		case SZ_BOURSE:
			strLayoutFileName.Format(_T("%s\\SZ\\%s.layout"),strLayoutDir,strLayerName);
			break;
		}

		if( !finder.FindFile(strLayoutFileName) )
		{
			strLayoutFileName.Format(_T("%s\\Public\\%s.layout"),strLayoutDir,strLayerName);
		}
	}
	return strLayoutFileName;
}