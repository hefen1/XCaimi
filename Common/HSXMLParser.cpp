/*******************************************************
  源程序名称:HSXMLParser.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  
  功能说明:  XMLParser类实现
  作    者:  shenglq
  开发日期:  20100927
*********************************************************/
#include "stdafx.h"
#include "HSXMLParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHSXMLParser::CHSXMLParser()
{
	m_bModifed = FALSE;
	m_pTiXmlDocument = NULL;
	m_pTiXmlDocument = new TiXmlDocument();
	m_pRoot = NULL;
}

CHSXMLParser::~CHSXMLParser()
{
	if (m_pTiXmlDocument)
	{
		delete m_pTiXmlDocument;
	}	
}
void CHSXMLParser::Release()
{
	delete this;
}

BOOL CHSXMLParser::LoadFile(CString sFileName)
{
	
	if (m_pTiXmlDocument->LoadFile(sFileName))
	//if (m_pTiXmlDocument->LoadFile("D:\\work\\client\\Sources\\Resource\\TradeLang\\Tables.xml"))
	{
		m_pRoot = m_pTiXmlDocument->RootElement();
		OutputDebugString("LoadFile True");
		return TRUE;
	}
	else
	{
		OutputDebugString("LoadFile False");	
		return FALSE;
	}
}

BOOL CHSXMLParser::LoadXML(const char* szXML)
{
	//m_pTiXmlDocument->SetTabSize(2);
	if (m_pTiXmlDocument->Parse(szXML))
	{
		m_pRoot = m_pTiXmlDocument->RootElement();
		OutputDebugString("LoadXml True");
		return TRUE;
	}
	else
	{
		m_pRoot = m_pTiXmlDocument->RootElement();
		//OutputDebugString(szXML);
		OutputDebugString("LoadXml False");
		return FALSE;
	}
}

BOOL CHSXMLParser::SaveFile(CString sFileName)
{
	if (sFileName.GetLength() > 0)
	{
		return  m_pTiXmlDocument->SaveFile(sFileName);
	}
	else
		return FALSE;
}

TiXmlNode* CHSXMLParser::GetRoot()
{
	return m_pRoot;
}

int CHSXMLParser::GetNodeCount(CXML xRoot, CXML xPath)
{
	TiXmlNode* pNode = TinyXPath::XNp_xpath_node(m_pRoot, xRoot);
	return GetNodeCount(xPath, pNode);
}

int CHSXMLParser::GetNodeCount(CXML xPath, TiXmlNode* pParent /* = NULL */)
{
	TiXmlNode* pNode = pParent;
	if (!pNode)
		pNode = m_pRoot;
	return TinyXPath::i_xpath_int(pNode, xPath);	
}

CString CHSXMLParser::GetNodeValue(CXML xRoot, CXML xPath, CString Default /* = */ )
{
	TiXmlNode* pNode = TinyXPath::XNp_xpath_node(m_pRoot, xRoot);
	return GetNodeValue(xPath, pNode, Default);
}

CString CHSXMLParser::GetNodeValue(CXML xPath, TiXmlNode* pParent /* = NULL */, CString Default /* = */ )
{
	CString sResult = Default;
	TIXML_STRING S_res;
	TiXmlNode* pNode = pParent;
	if (!pNode)
		pNode = m_pRoot;
	S_res = TinyXPath::S_xpath_string(pNode, xPath);
	sResult = S_res.c_str();
	return sResult;
}

TiXmlNode* CHSXMLParser::SelectSingleNode(CXML xRoot, CXML xPath, int iIndex /* = -1 */)
{
	TiXmlNode* pNode = TinyXPath::XNp_xpath_node(m_pRoot, xRoot);
	return SelectSingleNode(xPath, pNode, iIndex);
}

TiXmlNode* CHSXMLParser::SelectSingleNode(CXML xPath, TiXmlNode* pParent /* = NULL */, int iIndex /* = -1 */)
{
	TiXmlNode* pNode = pParent;
	if (!pNode)
		pNode = m_pRoot;

	if (-1 == iIndex)
	{
		return TinyXPath::XNp_xpath_node(pNode, xPath);
	}
	else
	{
		CXML xTmp = "";
		xTmp.Format("%s[%d]", xPath, iIndex + 1);
		return TinyXPath::XNp_xpath_node(pNode, xTmp);
	}
}

BOOL CHSXMLParser::GetModified()
{
	return m_bModifed;
}

void CHSXMLParser::SetModified(BOOL bValue)
{
	m_bModifed = bValue;
}