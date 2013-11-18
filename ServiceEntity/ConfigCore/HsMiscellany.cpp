#include "StdAfx.h"
#include "HsMiscellany.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsMiscellany::CHsMiscellany(void)
{
	m_bInited = FALSE;
	m_pConfigBase = NULL;
}

CHsMiscellany::~CHsMiscellany(void)
{
	Release(TRUE);
}

void CHsMiscellany::Init()
{
	m_pConfigBase = gFileManager.GetSystemCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsMiscellany::Release( BOOL bSaveToFile /*= TRUE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}
	return TRUE;
}

BOOL CHsMiscellany::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile("");
	}

	return TRUE;
}

CString CHsMiscellany::GetSectionValue( CString lpszSection )
{
	if (lpszSection.IsEmpty())
	{
		return "";
	}

	CString strPath = "//Miscellany/Section/Item";
	CString strData = "";
	return m_pConfigBase->GetNodeAttrString(strPath, "value", "", "id", lpszSection);
}

int CHsMiscellany::SetSectionValue( CString lpszSection, CString lpszValue )
{

	CString strPath = "";
	TiXmlNode* pNode = NULL;

	if (lpszSection.IsEmpty())
	{// 删除所有<Section>下子节点

		strPath = "//Miscellany/Section";
		pNode = m_pConfigBase->GetNode(strPath, "", "", UserDoc);
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{
			pNode->Clear(); // 删除所有子节点
			m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		}
		return 1;
	}
	else if (lpszValue.IsEmpty())
	{// 删除<Section>下 id=lpszSection的节点
		
		strPath = "//Miscellany/Section";
		pNode = m_pConfigBase->GetNode(strPath, "", "", UserDoc);
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{
			strPath = "//Miscellany/Section/Item";
			TiXmlNode* pChildNode = m_pConfigBase->GetNode(strPath, "id", lpszSection, UserDoc);
			if (pChildNode && !::IsBadReadPtr(pChildNode, 1))
			{
				pNode->RemoveChild(pChildNode);
			}
		}
		return 1;
	}
	else
	{// 都不为空 更新节点

		strPath = "//Miscellany/Section";
		pNode = m_pConfigBase->SetNode(strPath);
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{
			TiXmlNode* pChildNode = pNode->FirstChild();
			while(pChildNode)
			{
				if ( pChildNode->ToElement()->Attribute("id") == lpszSection)
				{
					m_pConfigBase->SetNodeAttrString(pChildNode, "value", lpszValue);
					return lpszValue.GetLength();
				}

				pChildNode = pChildNode->NextSibling();
			}

			TiXmlElement* pElement = new TiXmlElement("Item");
			pElement->SetAttribute("id", lpszSection);
			pElement->SetAttribute("value", lpszValue);
			pNode->LinkEndChild(pElement);
		}


		return lpszValue.GetLength();
	}

}