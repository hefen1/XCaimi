#include "StdAfx.h"
#include "HsTabCfg.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsTabCfg::CHsTabCfg(void)
{
	m_pConfigBase = NULL;
	m_bInited = FALSE;
}

CHsTabCfg::~CHsTabCfg(void)
{
	Release(TRUE);
}

void CHsTabCfg::Init()
{
	m_pConfigBase = gFileManager.GetReportTabCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsTabCfg::Release( BOOL bSaveToFile /*= FALSE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}

	CString strName = "";
	CTabItemArray* pAy = NULL;
	POSITION pos = m_mapTabGroup.GetStartPosition();
	while (pos)
	{
		m_mapTabGroup.GetNextAssoc(pos, strName, (void*&)pAy);
		for (int i = 0; i < pAy->GetCount(); i++)
		{
			delete pAy->GetAt(i);
			pAy->GetAt(i) = NULL;
		}
		pAy->RemoveAll();
		delete pAy;
	}
	m_mapTabGroup.RemoveAll();

	return TRUE;
}

BOOL CHsTabCfg::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}
	return TRUE;
}

long CHsTabCfg::GetTabGroups(TiXmlNode* pNode, CStringArray& ayTabGroup)
{
	if (pNode == NULL)
	{
		return -1;
	}

	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		CString strName = pChildNode->Value();
		if ( strName.Compare("Group") == 0 && pChildNode->FirstChild())
		{
			ayTabGroup.Add(pChildNode->ToElement()->Attribute("name"));
		}

		pChildNode = pChildNode->NextSibling();
	}

	return ayTabGroup.GetCount();
}

long CHsTabCfg::GetTabList( TiXmlNode* pNode, CTabItemArray& ayTabItem )
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1))
	{
		return -1;
	}
	
	CTabItemArray* pItemAy = new CTabItemArray;
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode && !::IsBadReadPtr(pChildNode, 1))
	{
		TabItem* pItem = new TabItem;
		CString str = "";
		// id
		pChildNode->ToElement()->Attribute("id", (int *)&pItem->m_ID); 

		// 名称
		strncpy(pItem->m_szName, pChildNode->ToElement()->Attribute("name"), sizeof(pItem->m_szName)-1 );

		// 板块名称
		str = pChildNode->ToElement()->Attribute("BlockName");
		strncpy(pItem->m_szBlock, pChildNode->ToElement()->Attribute("name"), BLOCK_NAME_LENGTH - 1 );

		// 页面类型
		str = pChildNode->ToElement()->Attribute("PageType");
		pItem->m_lPageType = strtol(str.GetString(), NULL, 16);

		// 页面标签类型
		str = pChildNode->ToElement()->Attribute("PageTagType");
		pItem->m_lPageTag = strtol(str.GetString(), NULL, 16);

		// 市场类型
		str = pChildNode->ToElement()->Attribute("MarketType");
		CStringArray ayTemp;
		int nTemp = CGeneralHelper::AnalyseStr(str, _T(","), ayTemp);		// 多市场处理
		for (int i = 0; i < nTemp; i++)
		{
			const char *szAttr = (const char*)ayTemp.GetAt(i).GetString();
			if (szAttr)
			{
				pItem->m_sMarket[i] = (HSMarketDataType)strtol(szAttr, NULL,16);
			}
		}
		// 列名称
		strncpy(pItem->m_szColGroupName, pChildNode->ToElement()->Attribute("ColGroup"), BLOCK_NAME_LENGTH - 1);
		
		// 菜单掩码
		str = pChildNode->ToElement()->Attribute("MenuMask");
		pItem->m_lMenuMask = strtol(str.GetString(), NULL, 16);

		// 默认菜单
		pChildNode->ToElement()->Attribute("DefinedMenu", (int *)&pItem->m_lMenuID);

		pItemAy->Add(pItem);
		pChildNode = pChildNode->NextSibling();
	}

	m_mapTabGroup.SetAt(pNode->ToElement()->Attribute("name"), (void*)pItemAy);

	ayTabItem.Append(*pItemAy);
	return ayTabItem.GetCount();
}

CString CHsTabCfg::GetDefaultTabGroup()
{
	CString strPath = "//DefualtGroup";
	return m_pConfigBase->GetNodeAttrString(strPath, "name", "");
}

int CHsTabCfg::GetDefaultTabItemID()
{
	CString strPath = "//DefualtItem";
	return m_pConfigBase->GetNodeAttrInt(strPath, "id", 0);
}

long CHsTabCfg::GetAllTabGroups( CStringArray& ayTabGroup )
{
	CMapStringToPtr mapTemp;
	// 从用户文件读取所有Group的name属性值
	ayTabGroup.RemoveAll();
	GetUserTabGroups(ayTabGroup);
	for (int i = 0; i < ayTabGroup.GetCount(); i++)
	{
		mapTemp.SetAt(ayTabGroup.GetAt(i), NULL);
	}
	
	// 从系统文件中读取 去除重复的
	ayTabGroup.RemoveAll();
	GetSysTabGroups(ayTabGroup);
	for (int i = 0; i < ayTabGroup.GetCount(); i++)
	{
		mapTemp.SetAt(ayTabGroup.GetAt(i), NULL);
	}

	CString strName = "";
	void* pNull = NULL;
	ayTabGroup.RemoveAll();
	POSITION pos = mapTemp.GetStartPosition();
	while (pos)
	{
		mapTemp.GetNextAssoc(pos, strName, pNull);
		ayTabGroup.Add(strName);
	}
	return ayTabGroup.GetCount();
}

long CHsTabCfg::GetSysTabGroups( CStringArray& ayTabGroup )
{
	TiXmlNode* pRoot = m_pConfigBase->GetRootNode(SystemDoc);
	if (pRoot == NULL)
	{
		pRoot = m_pConfigBase->GetRootNode(ResourceDoc);
	}

	return GetTabGroups(pRoot, ayTabGroup);
}

long CHsTabCfg::GetUserTabGroups( CStringArray& ayTabGroup )
{
	TiXmlNode* pRoot = m_pConfigBase->GetRootNode(UserDoc);
	return GetTabGroups(pRoot, ayTabGroup);
}

long CHsTabCfg::GetUserDefMenuID( int nID )
{  
	return 0;
}

long CHsTabCfg::GetTabList( CString strGroupName,CTabItemArray &ayTabItem )
{	
	CTabItemArray* pItemAy = NULL;
	if (m_mapTabGroup.Lookup(strGroupName, (void*&)pItemAy))
	{
		ayTabItem.Append(*pItemAy);
		return ayTabItem.GetCount();
	}

	CString strPath = "//Group";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "name", strGroupName);
	
	return GetTabList(pNode, ayTabItem);
}

long CHsTabCfg::GetSysTabList(CString strGroupName,CTabItemArray& ayTabItem)
{
	CString strPath = "//Group";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "name", strGroupName, SystemDoc);
	return GetTabList(pNode, ayTabItem);
}

BOOL CHsTabCfg::SetTabList( CString strGroupName,CTabItemArray* ayTabItem )
{
	if (strGroupName.IsEmpty() || ayTabItem == NULL || ayTabItem->IsEmpty())
	{
		return FALSE;
	}

	CTabItemArray ayOldTabItem;
	GetTabList(strGroupName, ayOldTabItem);
	if (ayOldTabItem.GetCount() == ayTabItem->GetCount())
	{
		int i = 0;
		for (i = 0; i < ayOldTabItem.GetCount(); i++)
		{
			if ( memcmp(ayOldTabItem.GetAt(i), ayTabItem->GetAt(i), sizeof(TabItem)) )
				break;
		}
		if (i == ayOldTabItem.GetCount())
			return TRUE;
	}

	CString strPath = "//Group";
	TiXmlNode* pNode = m_pConfigBase->SetNode(strPath, "name", strGroupName);//m_pConfigBase->GetNode(strPath, "name", strGroupName, UserDoc);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		pNode->Clear(); // 清空所有孩子

		TabItem* pItem;
		CString strTemp = "";
		for (int i = 0; i < ayTabItem->GetCount(); i++)
		{
			TiXmlElement* pElement = new TiXmlElement("Item");
			pItem = ayTabItem->GetAt(i);

			pElement->SetAttribute("id",          pItem->m_ID);
			pElement->SetAttribute("name",        pItem->m_szName);
			pElement->SetAttribute("BlockName",   pItem->m_szBlock);
			strTemp.Format("%x", pItem->m_lPageType);
			pElement->SetAttribute("PageType",    strTemp);
			strTemp.Format("%x", pItem->m_lPageTag);
			pElement->SetAttribute("PageTagType", strTemp);
			
			strTemp.Format("%x", pItem->m_sMarket[0]);
			for (int j = 1; j < MARKETTYPE_COUNT; j++)
			{
				if (pItem->m_sMarket[j] > 0)
				{
					strTemp.AppendFormat(",%x",pItem->m_sMarket[j]);
				}
			}
			pElement->SetAttribute("MarketType",  strTemp);
			pElement->SetAttribute("ColGroup",    pItem->m_szColGroupName);
			strTemp.Format("%x", pItem->m_lMenuMask);
			pElement->SetAttribute("MenuMask",    strTemp);
			pElement->SetAttribute("DefinedMenu", pItem->m_lMenuID);

			pNode->LinkEndChild(pElement);
		}

		m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		
		//////////////////////////////////////////////////////////////////////////
		// 清空缓存中的数据
		for (int i = 0; i < ayOldTabItem.GetCount(); i++)
		{
			pItem = ayOldTabItem.GetAt(i);
			if (pItem)
			{
				delete pItem;
			}
		}
		m_mapTabGroup.RemoveKey(strGroupName);
		//////////////////////////////////////////////////////////////////////////
		return TRUE;
	}
	return FALSE;
}

BOOL CHsTabCfg::DelTabList( CString strGroupName )
{
	CString strPath = "//Group";
	if (m_pConfigBase->RemoveNode(strPath, "name", strGroupName))
	{
		CTabItemArray* pAyItem = NULL;
		if (m_mapTabGroup.Lookup(strGroupName, (void*&)pAyItem) && pAyItem)
		{
			TabItem* pItem = NULL;
			for (int i = 0; i < pAyItem->GetCount(); i++)
			{
				pItem = pAyItem->GetAt(i);
				if (pItem)
				{
					delete pItem;
					pItem = NULL;
				}
			}
			pAyItem->RemoveAll();
			m_mapTabGroup.RemoveKey(strGroupName);
		}

		m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		return TRUE;
	}
	else
		return FALSE;
	
}