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
			// �����������������
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
		// �ͷ�����ָ��
		delete pAyIndex;
	}

	pos = m_mapTemplete.GetStartPosition();
	CTechTempleteArray* pAyTemplete = NULL;
	while (pos)
	{
		m_mapTemplete.GetNextAssoc(pos, strKey, (void*&)pAyTemplete);
		if (pAyTemplete)
		{
			// �����������������
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
		// �ͷ�����ָ��
		delete pAyTemplete;
	}

	return TRUE;
}

CString CHsKLineTab::GetDefaultGroupName()   // ��ȡĬ��ѡ�е�����
{
	CString strPath = "//TechKLineTabSet";
	return m_pConfigBase->GetNodeAttrString(strPath, "DefaultGroup");
}

int CHsKLineTab::GetTechIndexAyCount()                 // ��ȡָ���������
{
	CString strPath = "//Index";
	return m_pConfigBase->GetNodeAttrInt(strPath, "count");
}

CString CHsKLineTab::GetTechIndexName(IN int nIndex)    // ��ȡĳһָ���������
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

int CHsKLineTab::GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex)          // ��ȡָ����
{
	//////////////////////////////////////////////////////////////////////////
	// �ȴӻ����ļ�����Ѱ��
	CString strKey;
	strKey.Format("%d", nIndex);
	CTechIndexArray* pAyIndex = NULL;
	if (m_mapIndex.Lookup(strKey, (void*&)pAyIndex) && pAyIndex)
	{
		ayTechIndex.Append(*pAyIndex);
		return pAyIndex->GetCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// �������Ҳ���
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
				// ����
				CString strName = pGrandChild->ToElement()->Attribute("name");
				strncpy(pIndex->m_szName, strName, min(strName.GetLength() + 1, sizeof(pIndex->m_szName)));
				// ָ������
				strName = pGrandChild->ToElement()->Attribute("index_name");
				strncpy(pIndex->m_szIndexName, strName, min(strName.GetLength() + 1, sizeof(pIndex->m_szIndexName)));
				// ָ������
				int nType(0);
				pGrandChild->ToElement()->Attribute("index_type", &nType);
				pIndex->m_nIndexType = nType;
				// ������
				pGrandChild->ToElement()->Attribute("line_type", &nType);
				pIndex->m_nLineType = nType;

				pAyIndex->Add(pIndex);
				pGrandChild = pGrandChild->NextSibling();
			}

			// ���뵽map
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

int CHsKLineTab::GetTechTempleteAyCount()              // ��ȡģ���������
{
	CString strPath = "//Templete";
	return m_pConfigBase->GetNodeAttrInt(strPath, "count");
}

CString CHsKLineTab::GetTechTempleteName(IN int nIndex) // ��ȡĳһģ���������
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

int CHsKLineTab::GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete) // ��ȡģ����
{
	//////////////////////////////////////////////////////////////////////////
	// �ȴӻ����ļ�����Ѱ��
	CString strKey;
	strKey.Format("%d", nIndex);
	CTechTempleteArray* pAyTemplete = NULL;
	if (m_mapTemplete.Lookup(strKey, (void*&)pAyTemplete) && pAyTemplete)
	{
		ayTechTemplete.Append(*pAyTemplete);
		return pAyTemplete->GetCount();
	}

	//////////////////////////////////////////////////////////////////////////
	// �������Ҳ���
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
				// ����
				CString strName = pGrandChild->ToElement()->Attribute("name");
				strncpy(pTemplete->m_szName, strName, min(strName.GetLength() + 1, sizeof(pTemplete->m_szName)));
				// ģ��Ĭ������
				strName = pGrandChild->ToElement()->Attribute("Default_Index");
				strncpy(pTemplete->m_szDefaultIndex, strName, min(strName.GetLength() + 1, sizeof(pTemplete->m_szDefaultIndex)));
				// ������
				double dRaito(0.0);
				pGrandChild->ToElement()->Attribute("raito", &dRaito);
				pTemplete->m_dRaito = dRaito;

				pAyTemplete->Add(pTemplete);
				pGrandChild = pGrandChild->NextSibling();
			}

			// ���뵽map
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