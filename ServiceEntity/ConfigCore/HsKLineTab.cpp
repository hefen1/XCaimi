#include "StdAfx.h"
#include "HsKLineTab.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsKLineTab::CHsKLineTab(void)
{
	m_bInited = FALSE;
	m_pConfigBase = NULL;
}

CHsKLineTab::~CHsKLineTab(void)
{
	Release();
}

void CHsKLineTab::Init()
{
	m_pConfigBase = gFileManager.GetKLineTab();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsKLineTab::Release()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release();
	}

	POSITION pos = m_mapIndex.GetStartPosition();
	CTechIndexArray* pAyIndex = NULL;
	CString strKey = "";
	while (pos)
	{
		m_mapIndex.GetNextAssoc(pos, strKey, (void*&)pAyIndex);
		if (pAyIndex)
		{
			// 清理数组里面的数据
			for (int i = 0; i < pAyIndex->GetCount(); i++)
			{
				TechIndex* pIndex = pAyIndex->GetAt(i);
				if (pIndex)
				{
					delete pIndex;
				}
			}
			pAyIndex->RemoveAll();
		}
		// 释放数组指针
		delete pAyIndex;
	}

	pos = m_mapTemplete.GetStartPosition();
	CTechTempleteArray* pAyTemplete = NULL;
	while (pos)
	{
		m_mapTemplete.GetNextAssoc(pos, strKey, (void*&)pAyTemplete);
		if (pAyTemplete)
		{
			// 清理数组里面的数据
			for (int i = 0; i < pAyTemplete->GetCount(); i++)
			{
				TechTemplete* pTemplete = pAyTemplete->GetAt(i);
				if (pTemplete)
				{
					delete pTemplete;
				}
			}
			pAyTemplete->RemoveAll();
		}
		// 释放数组指针
		delete pAyTemplete;
	}

	return TRUE;
}

CString CHsKLineTab::GetDefaultGroupName()   // 获取默认选中的名称
{
	CString strPath = "//TechKLineTabSet";
	return m_pConfigBase->GetNodeAttrString(strPath, "DefaultGroup");
}

int CHsKLineTab::GetTechIndexAyCount()                 // 获取指标项的数量
{
	CString strPath = "//Index";
	return m_pConfigBase->GetNodeAttrInt(strPath, "count");
}

CString CHsKLineTab::GetTechIndexName(IN int nIndex)    // 获取某一指标项的名称
{
	CString strPath = "//Index";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		int nValue(-1);
		pChildNode->ToElement()->Attribute("value", &nValue);
		if (nValue == nIndex)
		{
			return pChildNode->ToElement()->Attribute("name");
		}
		pChildNode = pChildNode->NextSibling();
	}
	return "";
}

int CHsKLineTab::GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex)          // 获取指标项
{
	//////////////////////////////////////////////////////////////////////////
	// 先从缓存文件里面寻找
	CString strKey;
	strKey.Format("%d", nIndex);
	CTechIndexArray* pAyIndex = NULL;
	if (m_mapIndex.Lookup(strKey, (void*&)pAyIndex) && pAyIndex)
	{
		ayTechIndex.Append(*pAyIndex);
		return pAyIndex->GetCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// 缓存中找不到
	CString strPath = "//Index";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		int nValue(-1);
		pChildNode->ToElement()->Attribute("value", &nValue);
		if (nValue == nIndex)
		{
			pAyIndex = new CTechIndexArray;
			TiXmlNode* pGrandChild = pChildNode->FirstChild();
			while (pGrandChild)
			{
				TechIndex* pIndex = new TechIndex;
				memset(pIndex, 0, sizeof(TechIndex));
				// 名称
				CString strName = pGrandChild->ToElement()->Attribute("name");
				strncpy(pIndex->m_szName, strName, min(strName.GetLength() + 1, sizeof(pIndex->m_szName)));
				// 指标名称
				strName = pGrandChild->ToElement()->Attribute("index_name");
				strncpy(pIndex->m_szIndexName, strName, min(strName.GetLength() + 1, sizeof(pIndex->m_szIndexName)));
				// 指标类型
				int nType(0);
				pGrandChild->ToElement()->Attribute("index_type", &nType);
				pIndex->m_nIndexType = nType;
				// 线类型
				pGrandChild->ToElement()->Attribute("line_type", &nType);
				pIndex->m_nLineType = nType;

				pAyIndex->Add(pIndex);
				pGrandChild = pGrandChild->NextSibling();
			}

			// 加入到map
			CString strKey;
			strKey.Format("%d", nValue);
			m_mapIndex.SetAt(strKey, (void*)pAyIndex);

			ayTechIndex.Append(*pAyIndex);
			return pAyIndex->GetCount();
		}
		pChildNode = pChildNode->NextSibling();
	}

	return 0;
}

int CHsKLineTab::GetTechTempleteAyCount()              // 获取模板项的数量
{
	CString strPath = "//Templete";
	return m_pConfigBase->GetNodeAttrInt(strPath, "count");
}

CString CHsKLineTab::GetTechTempleteName(IN int nIndex) // 获取某一模板项的名称
{
	CString strPath = "//Templete";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);

	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		int nValue(-1);
		pChildNode->ToElement()->Attribute("value", &nValue);
		if (nValue == nIndex)
		{
			return pChildNode->ToElement()->Attribute("name");
		}
		pChildNode = pChildNode->NextSibling();
	}
	return "";
}

int CHsKLineTab::GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete) // 获取模板项
{
	//////////////////////////////////////////////////////////////////////////
	// 先从缓存文件里面寻找
	CString strKey;
	strKey.Format("%d", nIndex);
	CTechTempleteArray* pAyTemplete = NULL;
	if (m_mapTemplete.Lookup(strKey, (void*&)pAyTemplete) && pAyTemplete)
	{
		ayTechTemplete.Append(*pAyTemplete);
		return pAyTemplete->GetCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// 缓存中找不到
	CString strPath = "//Templete";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		int nValue(-1);
		pChildNode->ToElement()->Attribute("value", &nValue);
		if (nValue == nIndex)
		{
			pAyTemplete = new CTechTempleteArray;
			TiXmlNode* pGrandChild = pChildNode->FirstChild();
			while (pGrandChild)
			{
				TechTemplete* pTemplete = new TechTemplete;
				memset(pTemplete, 0, sizeof(TechTemplete));
				// 名称
				CString strName = pGrandChild->ToElement()->Attribute("name");
				strncpy(pTemplete->m_szName, strName, min(strName.GetLength() + 1, sizeof(pTemplete->m_szName)));
				// 模板默认名称
				strName = pGrandChild->ToElement()->Attribute("Default_Index");
				strncpy(pTemplete->m_szDefaultIndex, strName, min(strName.GetLength() + 1, sizeof(pTemplete->m_szDefaultIndex)));
				// 线类型
				double dRaito(0.0);
				pGrandChild->ToElement()->Attribute("raito", &dRaito);
				pTemplete->m_dRaito = dRaito;

				pAyTemplete->Add(pTemplete);
				pGrandChild = pGrandChild->NextSibling();
			}

			// 加入到map
			CString strKey;
			strKey.Format("%d", nValue);
			m_mapTemplete.SetAt(strKey, (void*)pAyTemplete);

			ayTechTemplete.Append(*pAyTemplete);
			return pAyTemplete->GetCount();
		}
		pChildNode = pChildNode->NextSibling();
	}

	return 0;
}