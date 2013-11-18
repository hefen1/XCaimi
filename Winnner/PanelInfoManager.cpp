#include "StdAfx.h"
#include "tinyxml.h"
#include "PanelInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPanelInfoManager::CPanelInfoManager()
{
	
}

BOOL CPanelInfoManager::LoadAllPanelInfo(CString strFileName)
{
	TiXmlDocument xmldoc;
	if( !xmldoc.LoadFile(strFileName) )
	{
		//m_strErrorInfo.Format(_T("分析系统文件失败.\r\n行:%d 列:%d\r\n错识原因:%s"),
		//	m_doc.ErrorRow(),m_doc.ErrorCol(),m_doc.ErrorDesc());
		return FALSE;
	}

	TiXmlElement* pRoot = xmldoc.FirstChildElement("SysConfig");
	TiXmlElement* pActionElem = pRoot->FirstChild("Panels")->ToElement();

	TiXmlElement* pPanel = pActionElem->FirstChild(_T("Panel"))->ToElement();
	while(pPanel != NULL)
	{
		CPanelInfo *pPanelInfo = new CPanelInfo();
		pPanelInfo->m_strName = pPanel->Attribute(_T("name")); 
		pPanelInfo->m_strPostion = pPanel->Attribute(_T("postion")); 
		pPanelInfo->m_strCaption = pPanel->Attribute(_T("caption")); 
		pPanelInfo->m_strLayoutName = pPanel->Attribute(_T("layout"));
		pPanelInfo->m_strGrant = pPanel->Attribute(_T("grant"));
		CString str = pPanel->Attribute(_T("maximize"));
		pPanelInfo->m_bMaximize = str == "1";

		CString strSize = pPanel->Attribute(_T("size"));
		int nPos = strSize.Find(',');
		CString strWidth = strSize.Left(nPos);
		CString strHeight = strSize.Right(strSize.GetLength() - nPos - 1);
		if(!strWidth.IsEmpty())
			pPanelInfo->m_size.cx = atoi(strWidth);
		if(!strHeight.IsEmpty())
			pPanelInfo->m_size.cy = atoi(strHeight);

		m_mapPanelInfo.SetAt(pPanelInfo->m_strName,(CObject*&)pPanelInfo);

		pPanel = pPanel->NextSiblingElement();
	}
	return TRUE;
}

CPanelInfoManager::~CPanelInfoManager()
{
	POSITION pos;
	CString key;
	CPanelInfo *pInfo = NULL;
	for( pos = m_mapPanelInfo.GetStartPosition(); pos != NULL; )
	{
		m_mapPanelInfo.GetNextAssoc( pos, key, (CObject*&)pInfo);
		if(pInfo)
			delete pInfo;
	}
	m_mapPanelInfo.RemoveAll();
}

void CPanelInfoManager::AddPanelInfo(CString strPanelName,CPanelInfo* pInfo)
{
	CPanelInfo *pExistItem = NULL;
	if(m_mapPanelInfo.Lookup(strPanelName,(CObject*&)pExistItem) != 0)
	{
		if(pExistItem)
			delete pExistItem;
	}
	m_mapPanelInfo.SetAt(strPanelName,(CObject*)pInfo);
}

CPanelInfo* CPanelInfoManager::GetPanelInfo(CString strPanelName)
{
	CPanelInfo *pExistItem = NULL;
	m_mapPanelInfo.Lookup(strPanelName,(CObject*&)pExistItem);
	return pExistItem;
}