#include "StdAfx.h"
#include "MyXmlDocument.h"
#include "xpath_static.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IQuoteLang* CMyXmlDocument::m_iQuoteLang = NULL;
ITradeLang* CMyXmlDocument::m_iTradeLang = NULL;

CMyXmlDocument::CMyXmlDocument():TiXmlDocument()
{
	m_bLoaded   = FALSE;
	m_bModified = FALSE;
	m_pRootNode = FALSE;
}

CMyXmlDocument::~CMyXmlDocument()
{
}

char* CMyXmlDocument::GetDataFromResource(UINT nFileID)
{
	char* szXMLContent = NULL;
	if (nFileID > 0)
	{
		if (!m_iQuoteLang)
		{
			m_iQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
		}
		if (m_iQuoteLang)
		{
			szXMLContent = m_iQuoteLang->LoadXML(nFileID);
			return szXMLContent;
		}

		if (!m_iTradeLang)
		{
			m_iTradeLang = (ITradeLang*)CWinnerApplication::GetObject(OBJ_TRADELANG);
		}
		if (m_iTradeLang)
		{
			szXMLContent = m_iTradeLang->LoadXML(nFileID);
			return szXMLContent;
		}

	}
 	return NULL;
}

BOOL CMyXmlDocument::LoadFromResource(UINT nFileID)
{
	if (nFileID <= 0)
	{
		return FALSE;
	}
	char* szContent = GetDataFromResource(nFileID);
	if (szContent && !IsBadReadPtr(szContent,1))
	{
		Parse(szContent);
		delete[] szContent;

		if (Error())
			return FALSE;
		else
		{
			m_bLoaded = TRUE;
			return TRUE;
		}
			
	}
 	return FALSE;
	
}

BOOL CMyXmlDocument::LoadFromFile(const char* pFilePath)
{
	if (!pFilePath || IsBadReadPtr(pFilePath,1))
	{
		return FALSE;
	}

	if (LoadFile(pFilePath,TIXML_ENCODING_UNKNOWN))
	{
		m_bLoaded = TRUE;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	 
}

void CMyXmlDocument::Release()
{
//	m_CurNodeMap.RemoveAll();
}

BOOL CMyXmlDocument::SaveFile(const CString& sFileName)
{
	if (m_bModified)
	{
		m_bModified = FALSE;
		return (BOOL)TiXmlDocument::SaveFile(sFileName.GetString());
	}
	return FALSE;
}

TiXmlNode* CMyXmlDocument::GetRootNode()
{
	if (!m_bLoaded)
		return NULL;

	if (m_pRootNode)
	{
		return m_pRootNode;
	}
	else
	{
		return TiXmlDocument::RootElement();

	}
}

int CMyXmlDocument::GetChildNodeCount(TiXmlNode* pParentNode /* = NULL */,const CString& strChildElement /* = _T */)
{
	if (!pParentNode || pParentNode->NoChildren() || !m_bLoaded)
	{
		return -1;
	}

	int nCount = 0;
	TiXmlNode* pChild = pParentNode->FirstChild();
	while(pChild)
	{
		if (strChildElement.IsEmpty() || (!strChildElement.IsEmpty() && !strChildElement.Compare( pChild->Value() )))	// 注意，标签名大小写敏感
		{ 
			nCount++;
		} 
		pChild = pChild->NextSibling();
	}
	return nCount;
}

int CMyXmlDocument::GetChildNodeCount(const CString& strFindPath, const CString& strAttrName/* =NULL */, const CString& strAttrValue/* =NULL */, const CString& strChildElement /* = _T */)
{
	if (strFindPath.IsEmpty() || !m_bLoaded)
	{
		return -1;
	}

	int nCount = 0;
	TiXmlNode* pNode = GetNode(strFindPath, strAttrName, strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		pNode = pNode->FirstChild();
		while(pNode)
		{
			if (strChildElement.IsEmpty() || (!strChildElement.IsEmpty() && !strChildElement.Compare( pNode->Value() )))	// 注意，标签名大小写敏感
			{
				nCount++;
			} 
			pNode = pNode->NextSibling();
		}
	}

	return nCount;
}

TiXmlNode* CMyXmlDocument::GetNode(const CString& strFindPath)
{
	if (strFindPath.IsEmpty() || !m_bLoaded)
	{
		return NULL;
	}

	TiXmlNode* pNode = NULL;
// 	if (m_CurNodeMap.Lookup(strFindPath, (void*&)pNode) && pNode && !::IsBadReadPtr(pNode, 1))
// 	{
// 		return pNode;
// 	}

	CString str;
	if ( GetRootNode() )
	{
		str = GetRootNode()->Value();
		pNode = TinyXPath::XNp_xpath_node(GetRootNode(), strFindPath.GetString());
	}
	if (pNode == NULL)
	{
//		str = this->Value();
//		str = RootElement()->Value();
		pNode = TinyXPath::XNp_xpath_node(RootElement(), strFindPath.GetString());
	}

// 	if (pNode && !::IsBadReadPtr(pNode, 1))
// 	{
// 		m_CurNodeMap.SetAt(strFindPath, (void*)pNode);
// 		return pNode;
// 	}
	return pNode;
	
}

TiXmlNode* CMyXmlDocument::GetNode(const CString& strFindPath,const CString& strName,const CString& strNameValue)
{
	if (strFindPath.IsEmpty() || !RootElement() || !m_bLoaded)
	{
		return NULL;
	}

	if (!strName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%s\"]",strName,strNameValue);
		return GetNode(strNewPath);
	}
	return GetNode(strFindPath);
}

TiXmlNode* CMyXmlDocument::GetNode(const CString& strFindPath,const CString& strName,int nNameValue)
{
	if (strFindPath.IsEmpty() || !RootElement() || !m_bLoaded)
	{
		return NULL;
	}

	if (!strName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%d\"]",strName,nNameValue);
		return GetNode(strNewPath);
	}
	return GetNode(strFindPath);
}

TiXmlNode* CMyXmlDocument::GetNode(const CString& strFindPath,const CString& strName,double dNameValue)
{
	if (strFindPath.IsEmpty() || !RootElement() || !m_bLoaded)
	{
		return NULL;
	}

	if (!strName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%f\"]",strName,dNameValue);
		return GetNode(strNewPath);
	}
	return GetNode(strFindPath);
}

BOOL CMyXmlDocument::SetRootNode(const CString& strFindPath, const CString& strAttrName /*=NULL*/, const CString& strAttrValue /*=NULL*/)
{
	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode)
	{
		if (SetRootNode(pNode))
		{
//			m_CurNodeMap.SetAt(strFindPath, (void*)pNode);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}

}
BOOL CMyXmlDocument::SetRootNode(TiXmlNode* pRootNode)
{
	if (pRootNode && IsBelongToMy(pRootNode))
	{
		m_pRootNode = pRootNode;
		return TRUE;
	}

	return FALSE;
	
}

///////////////////////////////////////////////////////////////////////////////////
CString CMyXmlDocument::GetNodeData(TiXmlNode* pNode)
{
	CString strData = "";
	if (pNode && !IsBadReadPtr(pNode,1) && m_bLoaded)
	{
		TiXmlText* pText = pNode->ToText();
		if (pText && !IsBadReadPtr(pText,1))
		{
			strData.Format("%s",pText->Value());
		}
	}
	return strData;
}

CString CMyXmlDocument::GetNodeData(const CString& strFindPath, const CString& strAttrName/* =NULL */, const CString& strAttrValue/* =NULL */)
{
	if (strFindPath.IsEmpty() || !RootElement() || !m_bLoaded)
	{
		return "";
	}

	CString strData = "";
	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		strData = GetNodeData(pNode);
	}
	return strData;
}

BOOL CMyXmlDocument::SetNodeData(const CString& strFindPath, const CString& strNewValue, const CString& strAttrName /* = NULL */, const CString& strAttrValue /* = NULL */)
{
	if (strFindPath.IsEmpty() || !RootElement())
	{
		return FALSE;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		return SetNodeData(pNode, strNewValue);
	}
	return FALSE;
}

BOOL CMyXmlDocument::SetNodeData(TiXmlNode* pNode, const CString& strNewValue)
{
	if (!RootElement())
	{
		return FALSE;
	}

	if (pNode && !::IsBadReadPtr(pNode,1))
	{
		TiXmlText* pText = pNode->ToText();
		if (pText && !IsBadReadPtr(pText,1))
		{
			pText->SetValue(strNewValue.GetString());
			SetModified(TRUE);
			return TRUE;
		}
	}

	return FALSE;

}

//////////////////////////////////////////////////////////////////////////////////
//获取int
int CMyXmlDocument::GetNodeAttrInt(TiXmlNode* pNode, const CString& strAttrName, int nDefault /* = 0 */)
{
	int nAttrValue = nDefault;
	if (pNode && !IsBadReadPtr(pNode,1) && !strAttrName.IsEmpty() && m_bLoaded)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement && !IsBadReadPtr(pElement,1))
		{
			pElement->Attribute(strAttrName.GetString(),&nAttrValue);
		}
	}
	
	return nAttrValue;
}

int CMyXmlDocument::GetNodeAttrInt(const CString& strFindPath, const CString& strAttrName, int nDefault /* = 0 */, const CString& strExtAttrName /* = _T("") */, const CString& strExtAttrValue /* =_T("") */)
{
	int nAttrValue = nDefault;

	if (strFindPath.IsEmpty() || strAttrName.IsEmpty() || !m_bLoaded)
	{
		return nAttrValue;
	}
	
	TiXmlNode* pNode = GetNode(strFindPath,strExtAttrName,strExtAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		nAttrValue = GetNodeAttrInt(pNode,strAttrName);
	}

	return nAttrValue;
}

BOOL CMyXmlDocument::SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue)
{
	if (strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement && !IsBadReadPtr(pElement,1))
		{
			pElement->SetAttribute(strNewAttrName,nNewAttrValue);
			SetModified(TRUE);
			return TRUE;
		}
	}

	return FALSE;

}

BOOL CMyXmlDocument::SetNodeAttrInt( const CString& strFindPath, const CString& strNewAttrName, int nNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/ )
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty() )
	{
		return FALSE;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		if (SetNodeAttrInt(pNode, strNewAttrName, nNewAttrValue))
		{
//			m_CurNodeMap.SetAt(strFindPath, (void*)pNode);
			return TRUE;
		}
	}
	
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////
//获取double
double CMyXmlDocument::GetNodeAttrDouble( TiXmlNode* pNode,const CString& strAttrName,double dDefault /*= 0.0*/ )
{
	double dAttrValue = dDefault;
	if (pNode && !IsBadReadPtr(pNode,1) && !strAttrName.IsEmpty() && m_bLoaded)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement && !IsBadReadPtr(pElement,1))
		{
			pElement->Attribute(strAttrName.GetString(), &dAttrValue);
		}
	}

	return dAttrValue;
}

double CMyXmlDocument::GetNodeAttrDouble( const CString& strFindPath, const CString& strAttrName, double dDefault /*= 0.0*/, const CString& strExtAttrName/*=_T("")*/, const CString& strExtAttrValue/*=_T("")*/ )
{
	double dAttrValue = dDefault;

	if (strFindPath.IsEmpty() || strAttrName.IsEmpty() || !m_bLoaded)
	{
		return dAttrValue;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strExtAttrName,strExtAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		dAttrValue = GetNodeAttrDouble(pNode,strAttrName);
	}

	return dAttrValue;
}

BOOL CMyXmlDocument::SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue)
{
	if (strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement && !IsBadReadPtr(pElement,1))
		{
			pElement->SetDoubleAttribute(strNewAttrName.GetString(), dNewAttrValue);
			SetModified(TRUE);
			return TRUE;
		}
	}

	return FALSE;

}

BOOL CMyXmlDocument::SetNodeAttrDouble( const CString& strFindPath, const CString& strNewAttrName, double dNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/ )
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		if (SetNodeAttrDouble(pNode, strNewAttrName, dNewAttrValue))
		{
//			m_CurNodeMap.SetAt(strFindPath, (void*)pNode);
			return TRUE;
		}
		
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//获取CString
CString CMyXmlDocument::GetNodeAttrString( TiXmlNode* pNode, const CString& strAttrName, const CString& strDefault/*=_T("")*/ )
{
	CString strAttrValue = strDefault;
	if (pNode && !IsBadReadPtr(pNode,1) && !strAttrName.IsEmpty() && m_bLoaded)
	{
		TiXmlElement* pElement = pNode->ToElement();
		if (pElement && !IsBadReadPtr(pElement,1))
		{
			strAttrValue = pElement->Attribute(strAttrName.GetString());
		}
	}
	return strAttrValue;
}

CString CMyXmlDocument::GetNodeAttrString( const CString& strFindPath, const CString& strAttrName, const CString& strDefault/*=_T("")*/, const CString& strExtAttrName/*=_T("")*/, const CString& strExtAttrValue/*=_T("")*/ )
{
	CString strAttrValue = strDefault;

	if (strFindPath.IsEmpty() || strAttrName.IsEmpty() || !m_bLoaded)
	{
		return strAttrValue;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strExtAttrName,strExtAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		strAttrValue = GetNodeAttrString(pNode,strAttrName);
	}

	return strAttrValue;
}

BOOL CMyXmlDocument::SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue)
{
	if (strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	if (pNode && !IsBadReadPtr(pNode,1))
	{
		TiXmlElement* pElement = pNode->ToElement();
		if(pElement && !IsBadReadPtr(pElement,1))
		{
			pElement->SetAttribute(strNewAttrName,strNewAttrValue);
			SetModified(TRUE);
			return TRUE;
		}
	}

	return FALSE;

}

BOOL CMyXmlDocument::SetNodeAttrString( const CString& strFindPath, const CString& strNewAttrName, const CString& strNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/ )
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		if (SetNodeAttrString(pNode, strNewAttrName, strNewAttrValue))
		{
//			m_CurNodeMap.SetAt(strFindPath, (void*)pNode);
			return TRUE;
		}
		
	}
	return FALSE;
}

BOOL CMyXmlDocument::IsBelongToMy(TiXmlNode* pNode)
{
	if (pNode == NULL || !m_bLoaded)
	{
		return FALSE;
	}
	
	TiXmlNode* pRoot = pNode;
	while (pRoot->Parent())
	{
		pRoot = pRoot->Parent();
	}

	return (pRoot->ToElement() == RootElement());
}