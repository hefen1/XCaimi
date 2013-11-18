/*******************************************************
  源程序名称:TradeTables.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  配置中心
  功能说明:  交易查询结果集表头类实现
  作    者:  shenglq
  开发日期:  20100926
*********************************************************/
#include "stdafx.h"
#include "TradeTables.h"
#include "TradeLangDefined.h"
#include "TradeConfigObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CTradeConfigObj gTradeConfigObj;

CTradeDict::CTradeDict()
{

}

CTradeDict::~CTradeDict()
{

}

CString CTradeDict::GetValueByName(CString sName)
{	
	CString sReturn = sName;
	TiXmlElement* pNode = m_pRoot->FirstChildElement("Column");
	while(pNode)
	{
		CString sTmp = pNode->Attribute(_T("Name"));
		if (sTmp == sName)
		{
			sReturn = pNode->Attribute(_T("DispName"));
			break;
		}
		pNode = pNode->NextSiblingElement();
	}
	return sReturn;
}

CXML CTradeDict::GetChildNodeName()
{
	return "Column";
}
//////////////////////////////////////////////////////////////////////////

CTradeColumn::CTradeColumn()
{
	m_pRoot = NULL;
	m_pXMLParser = NULL;
	m_sColumnName = "";
	m_sDispName = "";
	m_eAlign = alNone;
	m_pTradeDict = NULL;
	m_bHaveTradeDict = TRUE;
}

CTradeColumn::~CTradeColumn()
{
	if (m_pTradeDict)
	{
		m_pTradeDict->Release();
	}
}

void CTradeColumn::Release()
{
	delete this;
}

void CTradeColumn::SetRoot(TiXmlNode* pRoot)
{
	m_pRoot = pRoot;
}

void CTradeColumn::SetXMLParser(CHSXMLParser* pXMLParser)
{
	m_pXMLParser = pXMLParser;
}
CString CTradeColumn::GetColumnName()
{
	if ("" == m_sColumnName)
	{
		m_sColumnName = GetValue("Name");
	}
	return m_sColumnName;
}

CString CTradeColumn::GetDispName()
{
	if ("" == m_sDispName)
	{
		m_sDispName = GetValue("DispName");
	}
	return m_sDispName;	
}

EAlign CTradeColumn::GetAlign()
{
	if (alNone == m_eAlign)
	{
		CString sTmp = GetValue("Align");
		EAlign eTmp;
		if ("R" == sTmp)
		{
			eTmp = alRight;
		}
		else
			if ("M" == sTmp)
			{
				eTmp = alCenter;
			}
			else
				eTmp = alLeft;
		m_eAlign = eTmp;
	}
	return m_eAlign;	
}

CString CTradeColumn::GetValue(CString sPropName)
{
	CXML xPath = "";
	xPath.Format("@%s", sPropName);
	return m_pXMLParser->GetNodeValue(xPath, m_pRoot);
}

ITradeDict* CTradeColumn::GetTradeDict()
{
	if ((m_bHaveTradeDict) && (!m_pTradeDict))
	{
		CXML xPath = "";
		xPath.Format("@%s", "DataDict");	
		CString sDict = m_pXMLParser->GetNodeValue(xPath, m_pRoot);
		if ("" == sDict)
		{
			m_bHaveTradeDict = FALSE;
		}
		else
		  m_pTradeDict = gTradeConfigObj.GetTradeDict(sDict);
	}
	return m_pTradeDict;
}

CString CTradeColumn::GetDictValue(CString sName)
{
	ITradeDict* pTradeDict = GetTradeDict();
	if (pTradeDict)
	{
		return pTradeDict->GetValueByName(sName);
	}
	else
		return sName;
}

//////////////////////////////////////////////////////////////////////////
int CTradeTable::GetCount()
{
	CXML xPath = "";
	xPath.Format("count(//%s)", GetChildNodeName());
	return m_pXMLParser->GetNodeCount(xPath, m_pRoot);
}

ITradeColumn* CTradeTable::GetItems(const int iIndex)
{
	return GetItemsFull(GetChildNodeName(), iIndex);
}

ITradeColumn* CTradeTable::GetItemsByName(const CString sName)
{
	CXML xPath = "";
	xPath.Format("%s[@%s='%s']", GetChildNodeName(), GetIDPropName(), sName);
	return GetItemsFull(xPath);
}

ITradeColumn* CTradeTable::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{	
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(xPath, m_pRoot, iIndex);
	if (pNode)
	{
		CTradeColumn* pNodeData = new CTradeColumn();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;	
}

CXML CTradeTable::GetChildNodeName()
{
	return "Column";
}

//////////////////////////////////////////////////////////////////////////

CXML CTradeTables::GetRootPath()
{
	return "//Tables";
}

ITradeTable* CTradeTables::GetItems(const int iIndex)
{
	return dynamic_cast<ITradeTable*>(CNodeDataList::GetItems(iIndex));
}

ITradeTable* CTradeTables::GetItemsByName(const CString sName)
{
	return dynamic_cast<ITradeTable*>(CNodeDataList::GetItemsByName(sName));
}

ITradeTable* CTradeTables::GetItemsFull(CXML xPath, int iIndex /* = -1 */)
{	
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode(GetRootPath(), xPath, iIndex);
	if (pNode)
	{
		CTradeTable* pNodeData = new CTradeTable();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}

ITradeDict* CTradeTables::GetTradeDictByName(CString sName)
{
	CString sTmp = "";
	sTmp.Format("Item[@Name='%s']", sName);
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode("//DataDict", sTmp);
	if (pNode)
	{
		CTradeDict* pNodeData = new CTradeDict();
		pNodeData->SetXMLParser(m_pXMLParser);
		pNodeData->SetRoot(pNode);
		return pNodeData;
	}
	else
		return NULL;
}

INodeTreeData* CTradeTables::GetReqParamByAction(const int nAction)
{

	CString sTmp = "";
	sTmp.Format("Item[@Name='%d']", nAction);
	TiXmlNode* pNode = m_pXMLParser->SelectSingleNode("//ReqParam", sTmp);
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

UINT CTradeTables::GetIDResource()
{
	return 0;
	//return GNV_TRADE_XML_TABLES;
}
CString CTradeTables::GetFileName()
{
	return CGeneralHelper::GetAppPath() + _T("config\\Tables.xml");
}
