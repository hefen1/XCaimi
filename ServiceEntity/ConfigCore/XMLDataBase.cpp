/*******************************************************
  源程序名称:XMLDataBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  XML配置文件基础数据实现
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#include "stdafx.h"
#include "XMLDataBase.h"
#include "TradeLangMngIntf.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNodeData::CNodeData()
{
	m_pRoot = NULL;
	m_pParent = NULL;
	m_pXMLParser = NULL;
}

CNodeData::~CNodeData()
{

}

void CNodeData::Release()
{
	delete this;
}


CString CNodeData::GetValue(const CString sPropName)
{
	CXML xPath = "";
	xPath.Format("@%s", sPropName);
	return m_pXMLParser->GetNodeValue(xPath, m_pRoot);
}

CString CNodeData::Name()
{
	return GetValue("Name");
}

CString CNodeData::Value()
{
	return GetValue("Value");
}

CString CNodeData::ValueEx()
{
	return GetValue("ValueEx");
}

void CNodeData::SetXMLParser(CHSXMLParser* pXMLParser)
{
	m_pXMLParser = pXMLParser;
}

void CNodeData::SetRoot(TiXmlNode* pRoot)
{
	m_pRoot = pRoot;
	m_pParent = m_pRoot->Parent();
}


CString CNodeData::GetIDPropName()
{
	return "Name";
}
//////////////////////////////////////////////////////////////////////////
CNodeTreeData::CNodeTreeData()
{

}

CNodeTreeData::~CNodeTreeData()
{

}

int CNodeTreeData::GetCount()
{
	//CXML xPath = "";
	//xPath.Format("count(//%s)", GetChildNodeName());
	//return m_pXMLParser->GetNodeCount(xPath, m_pRoot);
	int nCount = 0;
	if (m_pRoot)
	{
		TiXmlElement* pElement = m_pRoot->FirstChildElement(GetChildNodeName());
		while(pElement)
		{
			nCount++;
			pElement = pElement->NextSiblingElement();
		}
	}	
	return nCount;
}

INodeData* CNodeTreeData::GetItems(const int iIndex)
{
	return GetItemsFull(GetChildNodeName(), iIndex);
}

INodeData* CNodeTreeData::GetItemsByName(const CString sName)
{
	CXML xPath = "";
	xPath.Format("%s[@%s='%s']", GetChildNodeName(), GetIDPropName(), sName);
	return GetItemsFull(xPath);
}

INodeData* CNodeTreeData::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{	
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CNodeData* pNodeData = new CNodeData();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
	
}

CXML CNodeTreeData::GetChildNodeName()
{
	return "Item";
}

//////////////////////////////////////////////////////////////////////////

CNodeDataList::CNodeDataList()
{
	m_pRoot = NULL;
	m_pXMLParser = new CHSXMLParser();
}

CNodeDataList::~CNodeDataList()
{
	m_pXMLParser->Release();
}

void CNodeDataList::Release()
{
	delete this;
}


CXML CNodeDataList::GetChildNodeName()
{
	return "Item";
}

int CNodeDataList::GetCount()
{
	//CXML xPath = "";
	//xPath.Format("count(//%s)", GetChildNodeName());
	//return m_pXMLParser->GetNodeCount(GetRootPath(), xPath);
	int nCount = 0;
	if (m_pRoot)
	{
		TiXmlElement* pElement = m_pRoot->FirstChildElement(GetChildNodeName());
		while(pElement)
		{
			nCount++;
			pElement = pElement->NextSiblingElement();
		}
	}	
	return nCount;
}

INodeData* CNodeDataList::GetItems(const int iIndex)
{
	return GetItemsFull(GetChildNodeName(), iIndex);
}

INodeData* CNodeDataList::GetItemsByName(const CString sName)
{
	CXML xPath = "";
	xPath.Format("%s[@%s='%s']", GetChildNodeName(), GetIDPropName(), sName);
	return GetItemsFull(xPath);
}

INodeData* CNodeDataList::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath, iIndex);
	if (pNode)
	{
		CNodeData* pNodeData = new CNodeData();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	return NULL;	
}


CString CNodeDataList::GetIDPropName()
{
	return "Name";
}

CXML CNodeDataList::GetRootPath()
{
	return "//root";
}

BOOL CNodeDataList::InitData()
{	
	BOOL bResult = FALSE;
	if (GetIDResource())
	{
		ITradeLang* pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
		if (pTradeLang)
		{
			char* pXML = pTradeLang->LoadXML(GetIDResource());
			if (pXML)
			{
				bResult = m_pXMLParser->LoadXML(pXML);
				delete[] pXML;			
			}	
		}			
	}
	else
	{
		bResult = m_pXMLParser->LoadFile(GetFileName());
		if (!bResult)
		{
			CString strText;
			strText.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n\t<%s />", Root_Text);
			//if(m_pXMLParser->LoadXML(strText)) 不知道为什么解析返回失败，但能保存到文件中
			m_pXMLParser->LoadXML(strText);
			bResult = m_pXMLParser->SaveFile(GetFileName());
		}
	}
	if (bResult)
		m_pRoot = m_pXMLParser->GetRoot();	
	return bResult;
}

CString CNodeDataList::GetFileName()
{
	return "";
}

UINT CNodeDataList::GetIDResource()
{
	return 0;
}

INodeTreeData* CNodeDataList::GetNodeTreeData(const CString sName)
{
	CXML xPath = "";
	xPath.Format("%s", sName);	
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath);
	if (pNode)
	{
		CNodeTreeData* pNodeData = new CNodeTreeData();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
	
}