#include "StdAfx.h"
#include "HsUserDataCfg.h"
#include "HsFileManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsUserDataCfg::CHsUserDataCfg(void)
{
	m_bInited = FALSE;
	m_bSave = FALSE;
	m_pConfigBase = NULL;
}

CHsUserDataCfg::~CHsUserDataCfg(void)
{
	Release();
}

void CHsUserDataCfg::Init()
{
	m_pConfigBase = gFileManager.GetUserDataCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
		LoadFileData();
	}
}

BOOL CHsUserDataCfg::Release()
{
	if (m_bSave)
	{
		SaveToFile();
	}
	if (m_pConfigBase)
	{
		m_pConfigBase->Release();
	}

	CString str;
	HsCodeMark* pMark = NULL;
	POSITION pos = m_mapCodeMark.GetStartPosition();
	while (pos)
	{
		m_mapCodeMark.GetNextAssoc(pos, str, pMark);
		if (pMark)
		{
			delete pMark;
		}
	}
	m_mapCodeMark.RemoveAll();

	return TRUE;
}

BOOL CHsUserDataCfg::SaveToFile()
{
	if (m_pConfigBase)
	{
		TiXmlNode* pNode = m_pConfigBase->GetRootNode(UserDoc);
		pNode->Clear();

		CString str;
		HsCodeMark* pMark = NULL;
		POSITION pos = m_mapCodeMark.GetStartPosition();
		while (pos)
		{
			m_mapCodeMark.GetNextAssoc(pos, str, pMark);
			if (pMark)
			{
				TiXmlElement newNode("mark");
				CString strMarket = CString(pMark->m_szIndex);
				newNode.SetAttribute("market",strMarket.Left(4));
				newNode.SetAttribute("code",strMarket.Right(6));
				newNode.SetAttribute("type",pMark->m_nType);
				if (pMark->m_nType == -1)
				{
					TiXmlText text(pMark->m_strContent);
					newNode.InsertEndChild(text);
				}

				pNode->InsertEndChild(newNode);
			}
		}
		m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
	}

	return TRUE;
}

void CHsUserDataCfg::LoadFileData()
{
	TiXmlNode* pNode = m_pConfigBase->GetRootNode(UserDoc);
	TiXmlElement* pChild = pNode->FirstChildElement();

	HsCodeMark* pTemp = NULL;
	while (pChild)
	{
		pTemp = new HsCodeMark;
		pChild->Attribute(_T("type"),&pTemp->m_nType);
		CString strTemp = CString(pChild->Attribute(_T("market")));
		if (!strTemp.CompareNoCase(_T("block")))
		{
			strncpy(pTemp->m_szIndex,pChild->Attribute(_T("code")),BLOCK_NAME_LENGTH);
		}
		else
		{
			strTemp.AppendFormat("%s",pChild->Attribute(_T("code")));
			strncpy(pTemp->m_szIndex,strTemp.GetString(),BLOCK_NAME_LENGTH);
		}
		if (pTemp->m_nType == -1)
		{
			pTemp->m_strContent.Format("%s",pChild->GetText());
		}

		//		m_ayCodeMark.Add(pTemp);
		HsCodeMark* pRes = NULL;
		if (m_mapCodeMark.Lookup(strTemp,pRes))
		{
			delete pRes;
			m_mapCodeMark.RemoveKey(strTemp);
		}
		m_mapCodeMark.SetAt(strTemp,pTemp);

		pChild = pChild->NextSiblingElement();
	}
}

const HsCodeMark* CHsUserDataCfg::SearchMark( IN CString& strKeyWord )
{
	HsCodeMark* pTemp = NULL;
	m_mapCodeMark.Lookup(strKeyWord,pTemp);
	return pTemp;
	
}

BOOL CHsUserDataCfg::DelMark( IN CString& strKeyWord )
{
	HsCodeMark* pTemp = NULL;
	if (m_mapCodeMark.Lookup(strKeyWord,pTemp) && pTemp)
	{
		delete pTemp;
		m_mapCodeMark.RemoveKey(strKeyWord);
	}

	return TRUE;
}

BOOL CHsUserDataCfg::SetMark( IN const HsCodeMark& CodeMark )
{
	m_bSave = TRUE;
	CString strCode = CodeMark.m_szIndex;
	HsCodeMark* pTemp = NULL;
	if (m_mapCodeMark.Lookup(strCode,pTemp) && pTemp)
	{
		*pTemp = CodeMark;
	}
	else
	{
		pTemp = new HsCodeMark;
		*pTemp = CodeMark;
		m_mapCodeMark.SetAt(strCode, pTemp);
	}

	return TRUE;
}