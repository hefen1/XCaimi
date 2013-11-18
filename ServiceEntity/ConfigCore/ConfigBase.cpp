#include "StdAfx.h"
#include "ConfigBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IQuoteLang*	CConfigBase::m_iQuoteLang = NULL;	// 行情资源库
ITradeLang*	CConfigBase::m_iTradeLang = NULL;	// 交易资源库

CConfigBase::CConfigBase()
{
	m_bInited = FALSE;
}

CConfigBase::~CConfigBase(void)
{
	BOOL bSave = m_UserDoc.GetModified();
	Release(bSave);
}

BOOL CConfigBase::InitResourceDoc(UINT nRcID)
{
	if (nRcID <= 0)
	{
		return FALSE;
	}

	CString strRcID;
	strRcID.Format("%d",nRcID);
	if ( m_ResourceDoc.LoadFromResource(nRcID))
	{
		return TRUE;
	}

	return FALSE;
	
}

BOOL CConfigBase::InitSysFileDoc(const CString& strDocFilePath)
{
	if (strDocFilePath.IsEmpty())
	{
		return FALSE;
	}

	if (m_SystemDoc.LoadFromFile(strDocFilePath.GetString()))
	{
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CConfigBase::InitUserFileDoc(const CString& strDocFilePath)
{
	if (strDocFilePath.IsEmpty())
	{
		return FALSE;
	}

	if ( !m_UserDoc.LoadFromFile(strDocFilePath.GetString()))
	{
		m_UserDoc.SetTabSize(2);
		CString strText;
		strText.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n\t<%s>\n\t</%s>", Root_Text, Root_Text);
		m_UserDoc.Parse(strText);
		m_UserDoc.SetLoaded(TRUE);
	}

	return TRUE;
}

BOOL CConfigBase::InsertNodes(TiXmlNode* pNewNode, CString strFindPath, TiXmlNode* pOtherNode, const CString strAttrName, const CString strAttrValue)
{
	if (!pNewNode || ::IsBadReadPtr(pNewNode, 1) || strFindPath.IsEmpty() || !pOtherNode || ::IsBadReadPtr(pOtherNode, 1))
	{
		return FALSE;
	}

	TiXmlNode* pNode = m_UserDoc.GetNode(strFindPath, strAttrName, strAttrValue);
	if (pNode)
	{//如果该节点已经存在 则立刻返回
		return TRUE;
	}

	TiXmlNode* pParentNode  = NULL;
	m_UserDoc.SetModified(TRUE);
	int nIndex = -1;
	while ( pOtherNode->Parent() && strncmp(pOtherNode->Parent()->Value(), Root_Text, strnlen(Root_Text, INT_MAX)) != 0)
	{
		nIndex = strFindPath.ReverseFind('/');
		if (nIndex != -1 && strFindPath.GetLength() > 2)
		{//解析路径 如: "//a/b/c" 将其分解为"//a/b"、"//a" 
			strFindPath = strFindPath.Left(nIndex);
		}

		if ( (m_UserDoc.GetNode(strFindPath)) != NULL)
		{
			pParentNode = (m_UserDoc.GetNode(strFindPath));
			pParentNode->LinkEndChild(pNewNode);
			m_UserDoc.SetModified(TRUE);
//			m_UserDoc.SaveFile(m_UserDoc.Value());
			return TRUE;
		}
		else
		{
			CString str = pOtherNode->Parent()->Value(); //for test
			pParentNode = CopyNodeFromAnther(pOtherNode->Parent());
			if (pParentNode)
			{
				pParentNode->LinkEndChild(pNewNode);
				pNewNode = pParentNode;
			}
		}

		pOtherNode = pOtherNode->Parent();
	}	
	m_UserDoc.RootElement()->LinkEndChild(pNewNode);
	m_UserDoc.SetModified(TRUE);
//	m_UserDoc.SaveFile(m_UserDoc.Value());
	return TRUE;
}

TiXmlNode* CConfigBase::CopyNodeFromAnther(TiXmlNode* pSourceNode)
{
	if (pSourceNode == NULL || ::IsBadReadPtr(pSourceNode, 1))
	{
		return NULL;
	}

	TiXmlElement* pTargetElement = new TiXmlElement(pSourceNode->Value());
	//清除目标节点多余属性(父亲节点以及孩子节点)
//	TiXmlElement* pElement = pTargetNode->ToElement();

	//清除孩子
	pTargetElement->Clear();

	//父节点暂时清理不了
	if (pTargetElement->Parent())
	{
		//pElement->Parent() = NULL;
	}
	
	return pTargetElement;
}


BOOL CConfigBase::Init(UINT nRcID, const CString &pSysDocFilePath, const CString &pUserDocFilePath)
{
	m_bInited = TRUE;

	// 加载资源文件
	BOOL bRet = FALSE;
	if (nRcID > 0)
	{
		bRet = InitResourceDoc(nRcID);
	
	}
	// 加载系统文件
	if (pSysDocFilePath && strlen(pSysDocFilePath)>0)
	{
		bRet = InitSysFileDoc(pSysDocFilePath);	
	}

	// 加载用户文件
	if (pUserDocFilePath && strlen(pUserDocFilePath)>0)
	{//从用户自定义路径查找

		bRet = InitUserFileDoc(pUserDocFilePath);
	}
	else if (pSysDocFilePath && strlen(pSysDocFilePath)>0)
	{//从系统配置文件目录下 查找对应的User文件

		CString strUserFile,strSysFile;
		strSysFile.Format("%s",pSysDocFilePath);
		int nIndex = strSysFile.ReverseFind('.');
		strUserFile.Format("%sUser.xml",strSysFile.Left(nIndex));
		
	 	bRet = InitUserFileDoc(strUserFile.GetString());
	}
	else if (nRcID > 0)
	{//从系统目录下的config目录中 查找对应的User文件

		CString strSysFile = m_iQuoteLang->LoadStr(nRcID);
		CString strUserFile;
		int nIndex = strSysFile.ReverseFind('.');
		strUserFile.Format("%sconfig\\%sUser.xml",CGeneralHelper::GetAppPath(),strSysFile.Left(nIndex));
		
		bRet = InitUserFileDoc(strUserFile.GetString());
	}

	return bRet;
}

BOOL CConfigBase::IsInit()
{
	return m_bInited;
}

BOOL CConfigBase::Release(BOOL bSaveToFile /* = FALSE */)
{
	if (bSaveToFile)
	{
		SaveToFile();
	}
	if (m_ResourceDoc.GetLoaded())
		m_ResourceDoc.Release();

	if (m_SystemDoc.GetLoaded())
		m_SystemDoc.Release();
	
	if (m_UserDoc.GetLoaded())
		m_UserDoc.Release();

	return TRUE;
}

void CConfigBase::SaveToFile(const CString &strFileName /* = "" */)
{
	const char* pFileName;
	if (strFileName.GetLength() <= 0)
	{
		pFileName = m_UserDoc.Value();
		m_UserDoc.SaveFile(pFileName);
	}
	else
		m_UserDoc.SaveFile(strFileName);

}


TiXmlNode* CConfigBase::GetRootNode(DocType nType /* = Defaule */)
{

	switch(nType)
	{
	case SystemDoc:
		{
			return m_SystemDoc.GetRootNode();
		}
	case ResourceDoc:
		{
			return m_ResourceDoc.GetRootNode();
		}
	default:
		{
			return m_UserDoc.GetRootNode();
		}
	}

}

BOOL CConfigBase::SetRootNode(TiXmlNode* pRootNode,DocType nType /* = Defaule */)
{
	if (!pRootNode || IsBadReadPtr(pRootNode,1))
	{
		return FALSE;
	}

	switch (nType)
	{
	case ResourceDoc:
		{
			m_ResourceDoc.SetRootNode(pRootNode);
			break;
		}
	case SystemDoc:
		{
			m_SystemDoc.SetRootNode(pRootNode);
			break;
		}
	default:
		{
			m_UserDoc.SetRootNode(pRootNode);
			break;
		}
	}

	return TRUE;
}

BOOL CConfigBase::SetRootNode(const CString& strFindPath, const CString& strAttrName/* =NULL */, const CString& strAttrValue/* =NULL */, DocType nType /* = Defaule */)
{
	if (strFindPath.IsEmpty())
	{
		return FALSE;
	}
	
	TiXmlNode* pNode = GetNode(strFindPath,strAttrName,strAttrValue,nType);
	if (pNode && !IsBadReadPtr(pNode,1))
	{
		return SetRootNode(pNode,nType);
	}
	return FALSE;
}

TiXmlNode* CConfigBase::GetNode(const CString& strFindPath, DocType nType /* = Default */ )
{
	if (strFindPath.IsEmpty())
	{
		return NULL;
	}

	TiXmlNode* pNode = NULL;
	// 无指定文档则三级查找
	if (!pNode && (nType==Default || nType==UserDoc))			// 用户文档
	{
		pNode = m_UserDoc.GetNode(strFindPath);
	} 
	if (!pNode && (nType==Default || nType==SystemDoc))			// 系统文档
	{
		pNode = m_SystemDoc.GetNode(strFindPath);
	}
	if (!pNode && (nType==Default || nType==ResourceDoc))		// 资源文档
	{
		pNode = m_ResourceDoc.GetNode(strFindPath);
	}

	return pNode;
}

TiXmlNode* CConfigBase::GetNode(const CString& strFindPath, const CString& strAttrName, const CString& strAttrValue, DocType nType /*= Default */ )
{
	if (strFindPath.IsEmpty())
	{
		return NULL;
	}

	if (!strAttrName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%s\"]",strAttrName,strAttrValue);
		return GetNode(strNewPath, nType);
	}
	return GetNode(strFindPath,nType);

}

TiXmlNode* CConfigBase::GetNode(const CString& strFindPath, const CString& strAttrName,int nNameValue, DocType nType /*= Default*/  )
{
	if (strFindPath.IsEmpty())
	{
		return NULL;
	}

	if (!strAttrName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%d\"]",strAttrName,nNameValue);
		return GetNode(strNewPath, nType);
	}
	return GetNode(strFindPath,nType);
}

TiXmlNode* CConfigBase::GetNode(const CString& strFindPath, const CString& strAttrName,double dNameValue, DocType nType /*= Default*/  )
{
	if (strFindPath.IsEmpty())
	{
		return NULL;
	}

	if (!strAttrName.IsEmpty())
	{
		CString strNewPath(strFindPath, strFindPath.GetLength());
		strNewPath.AppendFormat("[@%s=\"%f\"]",strAttrName,dNameValue);
		return GetNode(strNewPath, nType);
	}
	return GetNode(strFindPath,nType);
}

TiXmlNode* CConfigBase::SetNode(const CString& strPath, const CString& strAttrName /*= ""*/, const CString& strAttrValue /*= ""*/)
{
	if (strPath.IsEmpty())
	{
		return NULL;
	}

	TiXmlNode* pNode = m_UserDoc.GetNode(strPath, strAttrName, strAttrValue);
	if (pNode)
	{// 已经添加成功
		return pNode;
	}

	pNode = m_SystemDoc.GetNode(strPath);
	if (pNode == NULL)
	{
		pNode = m_ResourceDoc.GetNode(strPath);
		if (pNode == NULL)
		{// 
			return NULL;
		}
	}

	TiXmlElement* pNewElement = new TiXmlElement(pNode->Value());
	if (!strAttrName.IsEmpty())
	{
		pNewElement->SetAttribute(strAttrName, strAttrValue);
	}

	if (InsertNodes(pNewElement, strPath, pNode, strAttrName, strAttrValue))
	{
		return pNewElement;
	}
	else
		return NULL;
	
}

BOOL CConfigBase::RemoveNode(TiXmlNode* pNode)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1))
	{
		return FALSE;
	}

	pNode->Clear(); // 清除所有孩子
	TiXmlNode* pParentNode = pNode->Parent();
	if (pParentNode && !::IsBadReadPtr(pParentNode, 1))
	{
		pParentNode->RemoveChild(pNode);
	}
	
	return TRUE;
	
}
BOOL CConfigBase::RemoveNode(const CString& strPath, const CString& strAttrName /*= ""*/, const CString& strAttrValue /*= ""*/)
{
	if (strPath.IsEmpty())
	{
		return FALSE;
	}

	TiXmlNode* pNode = m_UserDoc.GetNode(strPath, strAttrName, strAttrValue);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		return RemoveNode(pNode);
	}
	return FALSE;
}


// CString CConfigBase::GetNodeData(TiXmlNode* pNode, DocType nType /* = Defaule */)
// {
// 
// 	CString strData = "";
// 	if (pNode && !IsBadReadPtr(pNode,1))
// 	{
// 		if (strData.IsEmpty() && (nType == Default || nType == UserDoc))
// 		{
// 			strData = m_ResourceDoc.GetNodeData(pNode);
// 		}
// 		if (strData.IsEmpty() && (nType == Default || nType == SystemDoc))
// 		{
// 			strData = m_SystemDoc.GetNodeData(pNode);
// 		}
// 		if (strData.IsEmpty() && (nType == Default || nType == ResourceDoc))
// 		{
// 			strData = m_ResourceDoc.GetNodeData(pNode);
// 		}
// 	}
// 
// 	return strData;
// }

CString CConfigBase::GetNodeData(const CString& strFindPath, const CString& strAttrName/* =NULL */, const CString& strAttrValue/* =NULL */, DocType nType /* = Defaule */)
{
	if (strFindPath.IsEmpty())
	{
		return "";
	}

	CString strData = "";
	if (strData.IsEmpty() && (nType == Default || nType == UserDoc))
	{
		strData = m_ResourceDoc.GetNodeData(strFindPath, strAttrName, strAttrValue);
	}
	if (strData.IsEmpty() && (nType == Default || nType == SystemDoc))
	{
		strData = m_SystemDoc.GetNodeData(strFindPath, strAttrName, strAttrValue);
	}
	if (strData.IsEmpty() && (nType == Default || nType == ResourceDoc))
	{
		strData = m_ResourceDoc.GetNodeData(strFindPath, strAttrName, strAttrValue);
	}
	return strData;
}

BOOL CConfigBase::SetNodeData(TiXmlNode* pNode, const CString& strNewValue)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1) || strNewValue.IsEmpty())
	{
		return FALSE;
	}

	return m_UserDoc.SetNodeData(pNode, strNewValue);
}

BOOL CConfigBase::SetNodeData(const CString& strFindPath, const CString& strNewValue, const CString& strAttrName /* = NULL */, const CString& strAttrValue /* = NULL */)
{
	if (strFindPath.IsEmpty() || strNewValue.IsEmpty())
	{
		return FALSE;
	}
	
	 if (!m_UserDoc.SetNodeData(strFindPath, strNewValue,strAttrName,strAttrValue))
	 {//用户文档不存在该节点 则根据系统文档或者资源文档中的路径在用户文档新建一个

		TiXmlNode* pNode = m_SystemDoc.GetNode(strFindPath, strAttrName, strAttrValue); //从系统文档里获取
		if (pNode == NULL)
		{
			pNode = m_ResourceDoc.GetNode(strFindPath, strAttrName, strAttrValue); //从资源文档里获取
			if (pNode == NULL)
			{
				return FALSE;
			}
		}

		TiXmlElement* pNewElement = new TiXmlElement(pNode->Value());	
		pNewElement->SetValue(strNewValue);

		return InsertNodes(pNewElement, strFindPath, pNode);
	 }

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//获取int
// int CConfigBase::GetNodeAttrInt( TiXmlNode* pNode, CString strAttrName, int nDefault /*= 0*/, DocType nType /*= Defaule*/ )
// {
// 
// 	int nData = nDefault;
// 	if (pNode && !IsBadReadPtr(pNode,1))
// 	{
// 		if (nData == nDefault && (nType == Default || nType == UserDoc))
// 		{
// 			nData = m_ResourceDoc.GetNodeAttrInt(pNode,strAttrName);
// 		}
// 		if (nData == nDefault && (nType == Default || nType == SystemDoc))
// 		{
// 			nData = m_SystemDoc.GetNodeAttrInt(pNode, strAttrName);
// 		}
// 		if (nData == nDefault && (nType == Default || nType == ResourceDoc))
// 		{
// 			nData = m_ResourceDoc.GetNodeAttrInt(pNode, strAttrName);
// 		}
// 	}
// 
// 	return nData;
// }

int CConfigBase::GetNodeAttrInt(const CString& strFindPath, const CString& strAttrName, int nDefault , const CString& strExtAttrName/*=_T("")*/, const CString& strExtAttrValue/*=_T("")*/, DocType nType /*= Defaule*/ )
{
	if (strFindPath.IsEmpty())
	{
		return nDefault;
	}

	int nData = nDefault;
	if (nData == nDefault && (nType == Default || nType == UserDoc))
	{
		nData = m_UserDoc.GetNodeAttrInt(strFindPath, strAttrName, nDefault ,strExtAttrName, strExtAttrValue);
	}
	if (nData == nDefault && (nType == Default || nType == SystemDoc))
	{
		nData = m_SystemDoc.GetNodeAttrInt(strFindPath, strAttrName, nDefault ,strExtAttrName, strExtAttrValue);
	}
	if (nData == nDefault && (nType == Default || nType == ResourceDoc))
	{
		nData = m_ResourceDoc.GetNodeAttrInt(strFindPath, strAttrName, nDefault ,strExtAttrName, strExtAttrValue);
	}
	return nData;
}

BOOL CConfigBase::SetNodeAttrInt(TiXmlNode* pNode, const CString& strNewAttrName, int nNewAttrValue)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1) || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	return m_UserDoc.SetNodeAttrInt(pNode, strNewAttrName, nNewAttrValue);
}

BOOL CConfigBase::SetNodeAttrInt( const CString& strFindPath, const CString& strNewAttrName, int nNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/)
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	if (!m_UserDoc.SetNodeAttrInt(strFindPath,strNewAttrName,nNewAttrValue,strAttrName,strAttrValue))
	{
		TiXmlNode* pNode = m_SystemDoc.GetNode(strFindPath, strAttrName, strAttrValue);
		if (pNode == NULL)
		{
			pNode = m_ResourceDoc.GetNode(strFindPath, strAttrName, strAttrValue);
			if (pNode == NULL)
			{
				return FALSE;
			}
		}

		TiXmlElement* pNewElement = new TiXmlElement(pNode->Value());
		pNewElement->SetAttribute(strNewAttrName, nNewAttrValue);
		return InsertNodes(pNewElement, strFindPath, pNode);
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取double
// double CConfigBase::GetNodeAttrDouble( TiXmlNode* pNode,CString strAttrName, double dDefault, DocType nType /*= Default*/ )
// {
// 
// 	double dData = dDefault;
// 	if (pNode && !IsBadReadPtr(pNode,1))
// 	{
// 		if (dData == dDefault && (nType == Default || nType == UserDoc))
// 		{
// 			dData = m_UserDoc.GetNodeAttrDouble(pNode, strAttrName, dDefault);
// 		}
// 		if (dData == dDefault && (nType == Default || nType == SystemDoc))
// 		{
// 			dData = m_SystemDoc.GetNodeAttrDouble(pNode, strAttrName, dDefault);
// 		}
// 		if (dData == dDefault && (nType == Default || nType == ResourceDoc))
// 		{
// 			dData = m_ResourceDoc.GetNodeAttrDouble(pNode, strAttrName, dDefault);
// 		}
// 	}
// 
// 	return dData;
// }

double CConfigBase::GetNodeAttrDouble( const CString& strFindPath, const CString& strAttrName/*=_T("")*/, double dDefault, const CString& strExtAttrName/*=_T("")*/, const CString& strExtAttrValue/*=_T("")*/, DocType nType /*= Default */ )
{
	if (strFindPath.IsEmpty())
	{
		return dDefault;
	}

	double dData = dDefault;
	if (dData == dDefault && (nType == Default || nType == UserDoc))
	{
		dData = m_UserDoc.GetNodeAttrDouble(strFindPath, strAttrName, dDefault, strExtAttrName, strExtAttrValue);
	}
	if (dData == dDefault && (nType == Default || nType == SystemDoc))
	{
		dData = m_SystemDoc.GetNodeAttrDouble(strFindPath, strAttrName, dDefault, strExtAttrName, strExtAttrValue);
	}
	if (dData == dDefault && (nType == Default || nType == ResourceDoc))
	{
		dData = m_ResourceDoc.GetNodeAttrDouble(strFindPath, strAttrName, dDefault, strExtAttrName, strExtAttrValue);
	}
	
	return dData;
}
BOOL CConfigBase::SetNodeAttrDouble(TiXmlNode* pNode, const CString& strNewAttrName, double dNewAttrValue)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1) || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	return m_UserDoc.SetNodeAttrDouble(pNode, strNewAttrName, dNewAttrValue);
}
BOOL CConfigBase::SetNodeAttrDouble( const CString& strFindPath, const CString& strNewAttrName, double dNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/)
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	if (!m_UserDoc.SetNodeAttrDouble(strFindPath,strNewAttrName,dNewAttrValue,strAttrName,strAttrValue))
	{
		TiXmlNode* pNode = m_SystemDoc.GetNode(strFindPath, strAttrName, strAttrValue);
		if (pNode == NULL)
		{
			pNode = m_ResourceDoc.GetNode(strFindPath, strAttrName, strAttrValue);
			if (pNode == NULL)
			{
				return FALSE;
			}
		}

		TiXmlElement* pNewElement = new TiXmlElement(pNode->Value());
		pNewElement->SetDoubleAttribute(strNewAttrName, dNewAttrValue);
		return InsertNodes(pNewElement, strFindPath, pNode);
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取CString
// CString CConfigBase::GetNodeAttrString( TiXmlNode* pNode,CString strAttrName, CString strDefault /*= _T("")*/, DocType nType /*= Default*/ )
// {
// 
// 	CString strData = strDefault;
// 	if (pNode && !IsBadReadPtr(pNode,1))
// 	{
// 		if (strData == strDefault && (nType == Default || nType == UserDoc))
// 		{
// 			strData = m_UserDoc.GetNodeAttrString(pNode, strAttrName, strDefault);
// 		}
// 		if (strData == strDefault && (nType == Default || nType == SystemDoc))
// 		{
// 			strData = m_SystemDoc.GetNodeAttrString(pNode, strAttrName, strDefault);
// 		}
// 		if (strData == strDefault && (nType == Default || nType == ResourceDoc))
// 		{
// 			strData = m_ResourceDoc.GetNodeAttrString(pNode, strAttrName, strDefault);
// 		}
// 	}
// 
// 	return strData;
// }

CString CConfigBase::GetNodeAttrString( const CString& strFindPath, const CString& strAttrName, const CString& strDefault/*=_T("")*/, const CString& strExtAttrName/*=_T("")*/, const CString& strExtAttrValue/*=_T("")*/, DocType nType /*= Default*/ )
{
	if (strFindPath.IsEmpty())
	{
		return strDefault;
	}

	CString strData = strDefault;
	if (strData == strDefault && (nType == Default || nType == UserDoc))
	{
		strData = m_UserDoc.GetNodeAttrString(strFindPath, strAttrName, strDefault, strExtAttrName, strExtAttrValue);
	}
	if (strData == strDefault && (nType == Default || nType == SystemDoc))
	{
		strData = m_SystemDoc.GetNodeAttrString(strFindPath, strAttrName, strDefault, strExtAttrName, strExtAttrValue);
	}
	if (strData == strDefault && (nType == Default || nType == ResourceDoc))
	{
		strData = m_ResourceDoc.GetNodeAttrString(strFindPath, strAttrName, strDefault, strExtAttrName, strExtAttrValue);
	}
	return strData;
}

BOOL CConfigBase::SetNodeAttrString(TiXmlNode* pNode, const CString& strNewAttrName, const CString& strNewAttrValue)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1) || strNewAttrName.IsEmpty())
	{
		return FALSE;
	}

	return m_UserDoc.SetNodeAttrString(pNode, strNewAttrName, strNewAttrValue);
}

BOOL CConfigBase::SetNodeAttrString( const CString& strFindPath, const CString& strNewAttrName, const CString& strNewAttrValue, const CString& strAttrName/*=_T("")*/, const CString& strAttrValue/*=_T("")*/)
{
	if (strFindPath.IsEmpty() || strNewAttrName.IsEmpty())
	{ 
		return FALSE;
	}
	
	if (!m_UserDoc.SetNodeAttrString(strFindPath,strNewAttrName,strNewAttrValue,strAttrName,strAttrValue))
	{
		TiXmlNode* pNode = m_SystemDoc.GetNode(strFindPath, strAttrName, strAttrValue);
		if (pNode == NULL)
		{
			pNode = m_ResourceDoc.GetNode(strFindPath, strAttrName, strAttrValue);
			if (pNode == NULL)
			{
				return FALSE;
			}
		}

		TiXmlElement* pNewElement = new TiXmlElement(pNode->Value());
		pNewElement->SetAttribute(strNewAttrName, strNewAttrValue);
		return InsertNodes(pNewElement, strFindPath, pNode);
	}
	return TRUE;

}

CMyXmlDocument* CConfigBase::GetXmlDocument( DocType nType /*= Default */)
{

	CMyXmlDocument* pXMLDoc = NULL;
	if (pXMLDoc == NULL && (nType == Default || nType == UserDoc))
	{
		if (m_UserDoc.GetLoaded())
		{
			pXMLDoc = &m_UserDoc;
		}
	}
	if (pXMLDoc == NULL && (nType == Default || nType == SystemDoc))
	{
		if (m_SystemDoc.GetLoaded())
		{
			pXMLDoc = &m_SystemDoc;
		}
	}
	if (pXMLDoc == NULL && (nType == Default || nType == ResourceDoc))
	{
		if (m_ResourceDoc.GetLoaded())
		{
			pXMLDoc = &m_ResourceDoc;
		}
	}

	return pXMLDoc;
}