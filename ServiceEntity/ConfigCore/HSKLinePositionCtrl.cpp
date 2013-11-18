#include "stdafx.h"
#include "HSKLinePositionCtrl.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//CHSLinePositionCtrl
extern CHsFileManager gFileManager; 
CHSLinePositionCtrl::CHSLinePositionCtrl()
{
	m_pConfigBase = NULL;
	m_nCurrentPos = 0;
}

CHSLinePositionCtrl::~CHSLinePositionCtrl()
{
	Release();
}

void CHSLinePositionCtrl::Init()
{
	m_pConfigBase = gFileManager.GetKLinePostionCtrl();

	if (m_pConfigBase != NULL && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHSLinePositionCtrl::Release()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release();
	}
	return TRUE;
}

int CHSLinePositionCtrl::GetTechLines(int nType, CStringArray& ayStrRet, BOOL bResearch)
{
	if (bResearch)
	{
		m_nCurrentPos = 0;
	}
	CString strPath;
	if(nType == 1)
		strPath = "//Fenshi";
	else if (nType == 2)
	{
		strPath = "//KLine";
	}
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	TiXmlNode* pChildNode = NULL;
	if (pNode != NULL)
	{
		pChildNode =  pNode->FirstChild();
	}
	int nIndex = 0;
	while (pChildNode != NULL)
	{
		nIndex ++;
		if (nIndex > m_nCurrentPos)
		{
			CString strItem = pChildNode->ToElement()->Attribute("name");
			if (strItem == "POSCTROL")
			{
				break;
			}
			ayStrRet.Add(strItem);
			m_nCurrentPos ++;
		}
		pChildNode = pChildNode->NextSibling();
	}

	return ayStrRet.GetSize();
}

float CHSLinePositionCtrl::GetPosCtrolRatio(int nType)
{
	double dblRet = 0.0;
	CString strPath;
	if(nType == 1)
		strPath = "//Fenshi";
	else if (nType == 2)
	{
		strPath = "//KLine";
	}
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	TiXmlNode* pChildNode = NULL;
	if (pNode != NULL)
	{
		pChildNode = pNode->FirstChild();
	}
	int nIndex = 0;
	while (pChildNode != NULL)
	{
		nIndex ++;
		if (nIndex > m_nCurrentPos)
		{
			CString strItem = pChildNode->ToElement()->Attribute("name");
			if (strItem == "POSCTROL")
			{
				m_nCurrentPos ++;
				pChildNode->ToElement()->Attribute("ratio", &dblRet);
				break;
			}
		}
		pChildNode = pChildNode->NextSibling();
	}
	return (float)dblRet;
}

int CHSLinePositionCtrl::SetTechLines(int nType, CStringArray& strKLIneName, BOOL bFirst/* = FALSE*/, BOOL bLast/* = FALSE*/)
{
	if (m_pConfigBase == NULL)
	{
		return 0;
	}
	int nRet = 0;
	CString strPath;
	CString strType;
	strType.Format("%d", nType);
	if(nType == 1)
		strPath = "//Fenshi";
	else if (nType == 2)
	{
		strPath = "//KLine";
	}

	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "type", strType);
	if (pNode == NULL)
	{
		TiXmlNode* pRoot = m_pConfigBase->GetRootNode(UserDoc);
		if (pRoot == NULL)
		{// 初始化发生错误
			return FALSE;
		}
		else
		{
			strPath = strPath.Right(strPath.GetLength()-2);
			TiXmlElement* pElement = new TiXmlElement(strPath);
			pElement->SetAttribute("type", strType);
			
			pNode = pElement;
			pRoot->LinkEndChild(pNode);
		}
	}

	if (pNode != NULL)
	{
		if (bFirst)
		{
			pNode->Clear();
			m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		}
		for (int i = 0; i < strKLIneName.GetSize(); i ++)
		{
			CString strLineName = strKLIneName.GetAt(i);
			if(!strKLIneName.IsEmpty())
			{
				TiXmlElement* pElement = new TiXmlElement("Item");
				pElement->SetAttribute("name",  strLineName);
				pNode->LinkEndChild(pElement);
			}
		}
	}
	if (bLast && m_pConfigBase != NULL)
	{
		m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		m_pConfigBase->SaveToFile("");
	}
	return nRet;
}

int CHSLinePositionCtrl::SetPosCtrolRatio(int nType, float fRatio)
{
	int nRet = 0;
	CString strPath;
	CString strType;
	strType.Format("%d", nType);
	if(nType == 1)
		strPath = "//Fenshi";
	else if (nType == 2)
	{
		strPath = "//KLine";
	}

	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "type", strType);
	if (pNode == NULL)
	{
		TiXmlNode* pRoot = m_pConfigBase->GetRootNode(UserDoc);
		if (pRoot == NULL)
		{// 初始化发生错误
			return FALSE;
		}
		else
		{
			strPath = strPath.Right(strPath.GetLength()-2);
			TiXmlElement* pElement = new TiXmlElement(strPath);
			pElement->SetAttribute("type", strType);
			
			pNode = pElement;
			pRoot->LinkEndChild(pNode);
		}
	}

	if (pNode != NULL)
	{
		CString strPosName = "POSCTROL";
		TiXmlElement* pElement = new TiXmlElement("Item");
		pElement->SetAttribute("name",  strPosName);
		CString strRatio;
		strRatio.Format("%.3f", fRatio);
		pElement->SetAttribute("ratio",  strRatio);
		pNode->LinkEndChild(pElement);
	}
	return nRet;
}
