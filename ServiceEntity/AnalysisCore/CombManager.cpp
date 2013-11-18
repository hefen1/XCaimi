#include "StdAfx.h"
#include "xpath_static.h"
#include "CombManager.h"
#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCombComponent::CCombComponent(CString strCode ,CString strName,int nAmount)
{
		m_strCode = strCode;
		m_strName = strName;
		m_nAmount = nAmount;
}

CString CCombComponent::GetCode()
{
	return m_strCode;
}

int CCombComponent::GetAmount()
{
	return m_nAmount;
}


CCombInfo::CCombInfo(CString strName,int nAmount,CString strComponentFile)
{
	m_strCombName = strName;
	strComponentFile = strComponentFile;
	m_nAmount = nAmount;
}

CCombInfo::~CCombInfo()
{
	for(int i = 0; i < m_ayComponent.GetCount(); i++)
	{
		CCombComponent *p = m_ayComponent.GetAt(i);
		if(p)
			delete p;
	}
	m_ayComponent.RemoveAll();
}

CString    CCombInfo::GetCombName()
{
	return m_strCombName;
}

int   CCombInfo::GetAmount()
{
	return m_nAmount;
}
int   CCombInfo::GetComponentCount()
{
	return m_ayComponent.GetCount();
}

ICombComponent* CCombInfo::GetCombComponent(int nIndex)
{
	if( nIndex >= 0 && nIndex < m_ayComponent.GetCount() )
		return m_ayComponent.GetAt(nIndex);
	else
		return NULL;
}

void CCombInfo::AddComponent(CCombComponent *pComponent)
{
	m_ayComponent.Add(pComponent);
}

BOOL CCombInfo::LoadCombComponent(CString strComponentFile)
{
	if(strComponentFile.IsEmpty())
		return FALSE;

	CString strFileName;
	CString strAppPath = CGeneralHelper::GetAppPath();
	strFileName.Format("%s\\combinfo\\%s",strAppPath, strComponentFile);

	CStdioFile file;
	if( !file.Open(strFileName,CFile::modeNoTruncate | CFile::modeRead | CFile::typeText) )
		return FALSE;

	CString str;
	while(file.ReadString(str))
	{
		CStringArray ayRet;
		CGeneralHelper::AnalyseStr(str,",",ayRet);
		if(ayRet.GetCount() < 3)
			continue;

		CString strCode = ayRet.GetAt(0);
		CString strName = ayRet.GetAt(1);
		CString strAmount = ayRet.GetAt(2);
		int nAmount = atoi(strAmount);
		CCombComponent *pComponent = new CCombComponent(strCode,strName,nAmount);
		AddComponent(pComponent);
	}
	file.Close();
	return TRUE;
}

CCombManager::CCombManager(void)
{
}

CCombManager::~CCombManager(void)
{
}

BOOL CCombManager::LoadCombs()
{
	CString strFileName;
	CString strAppPath = CGeneralHelper::GetAppPath();
	strFileName.Format("%s%s",strAppPath, _T("combinfo\\combinfo.xml")); 
	TiXmlDocument xmldoc;
	if( !xmldoc.LoadFile(strFileName) )
	{
		return FALSE;
	}
	TiXmlElement* pRoot = xmldoc.FirstChildElement(_T("Content"));
	if(!pRoot)
		return FALSE;

	TiXmlElement* pCombInfoRoot = pRoot->FirstChildElement("Comb");
	if(!pCombInfoRoot)
		return FALSE;

	TiXmlElement* pCombItem = pCombInfoRoot->FirstChildElement(_T("Item"));
	while(pCombItem)
	{
		CString strCombName = pCombItem->Attribute(_T("name"));
		CString strAmount= pCombItem->Attribute(_T("amout"));
		CString strComponentFile = pCombItem->Attribute(_T("component_file"));
		int nAmount = atoi(strAmount);
		CCombInfo* pCombInfo = new CCombInfo(strCombName,nAmount,strComponentFile);
		if(!strComponentFile.IsEmpty())
			pCombInfo->LoadCombComponent(strComponentFile);
		m_ayCombInfo.Add(pCombInfo);
		pCombItem = pCombItem->NextSiblingElement(_T("Item"));
	}
	return TRUE;
}

BOOL CCombManager::AddComb(CString strName,int nAmount,CString strComponentFile)
{
	return TRUE;
}

BOOL CCombManager::DelComb(CString strName)
{
	return TRUE;
}

ICombInfo* CCombManager::GetComb(CString strName)
{
	for(int i = 0; i < m_ayCombInfo.GetCount(); i++)
	{
		CCombInfo* pCombInfo = m_ayCombInfo.GetAt(i);
		if(pCombInfo->GetCombName() == strName)
		{
			return pCombInfo;
		}
	}
	return NULL;
}

ICombInfo* CCombManager::GetComb(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_ayCombInfo.GetCount())
	{
		return m_ayCombInfo.GetAt(nIndex);
	}
	return NULL;
}

int CCombManager::GetCombCount()
{
	return m_ayCombInfo.GetCount();
}

//////////////////////////////////////////////////////////////////////////
//CParamInfo
//////////////////////////////////////////////////////////////////////////
CParamInfo::CParamInfo()
{

}

CString CParamInfo::GetParamName()
{
	return m_strParamName;
}

char     CParamInfo::GetParamType()
{
	return m_cParamType;
}	

CString CParamInfo::GetParamValue()
{
	return	m_strParamValue;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CWatcherParams::CWatcherParams()
{
	
}

CWatcherParams::~CWatcherParams()
{
	for (int i = 0; i < m_ayParams.GetCount(); i++)
	{
		CParamInfo* pParam = m_ayParams.GetAt(i);
		if(pParam)
			delete pParam;
	}
}

int CWatcherParams::GetParamCount()
{
	return m_ayParams.GetCount();
}

IParamInfo* CWatcherParams::GetParamInfo(int nIndex)
{
	if(nIndex <0 && nIndex >= m_ayParams.GetCount())
		return NULL;
	return m_ayParams.GetAt(nIndex);	
}

IParamInfo* CWatcherParams::GetParamInfo(CString strParmaName)
{
	for(int i = 0 ; i < m_ayParams.GetCount(); i++)
	{
		IParamInfo* p = m_ayParams.GetAt(i);
		if(p->GetParamName() == strParmaName )
			return p;
	}
	return NULL;
}

void CWatcherParams::UpdateParam(CString strParamName,char cType,CString strParamValue)
{
	CParamInfo* pParamInfo = NULL;
	for( int i = 0; i < m_ayParams.GetCount(); i ++)
	{
		CParamInfo* pTemp = m_ayParams.GetAt(i);
		if(pTemp->GetParamName() == strParamName )
		{
			pParamInfo = pTemp;
			pParamInfo->m_strParamName = strParamName;
			pParamInfo->m_cParamType = cType;
			pParamInfo->m_strParamValue = strParamValue;
			break;
		}
	}

	if(pParamInfo  == NULL)
	{
		pParamInfo = new CParamInfo();
		pParamInfo->m_strParamName = strParamName;
		pParamInfo->m_cParamType = cType;
		pParamInfo->m_strParamValue = strParamValue;
		m_ayParams.Add(pParamInfo);
	}
}
void CWatcherParams::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////////
//CWatcherInfo
//////////////////////////////////////////////////////////////////////////
CString CWatcherInfo::GetName()
{
	return m_strName;
}

IWatcherParams*  CWatcherInfo::GetParams()
{
	return &m_params;
}

void CWatcherInfo::UpdateParam(CString strParamName,char cType,CString strParamValue)
{
	m_params.UpdateParam(strParamName,cType,strParamValue);
}

IParamInfo* CWatcherInfo::GetParamInfo(CString strParamName)
{
	return m_params.GetParamInfo(strParamName);
}

void CWatcherInfo::Release()
{
	delete this;
}

//////////////////////////////////////////////////////////////////////////
//CWatcherInfoManager
//////////////////////////////////////////////////////////////////////////
CWatcherInfoManager::CWatcherInfoManager()
{

}

CWatcherInfoManager::~CWatcherInfoManager()
{
	for(int i = 0; i < m_ayWatcherInfo.GetCount(); i++)
	{
		CWatcherInfo* pInfo = m_ayWatcherInfo.GetAt(i);
		if(pInfo)
			delete pInfo;
	}
}

BOOL CWatcherInfoManager::LoadWatchers(CString strWatcherName)
{
	TiXmlDocument xmldoc;

	if( InitXmlDoc(xmldoc) == FALSE )
		return FALSE;

	TiXmlElement* pCombInfoRoot = GetWatcherRootItem(xmldoc,strWatcherName);
	if(!pCombInfoRoot)
		return FALSE;

	TiXmlElement* pWatchItem = pCombInfoRoot->FirstChildElement(_T("Item"));
	while(pWatchItem)
	{
		CWatcherInfo *pWatchInfo = new CWatcherInfo();
		pWatchInfo->m_strName = pWatchItem->Attribute(_T("name"));
		TiXmlElement* pParamItem = pWatchItem->FirstChildElement("Param");
		while(pParamItem)
		{
			CString strParamName = pParamItem->Attribute("name");
			CString strParamValue = pParamItem->Attribute("value");
			CString strParamType = pParamItem->Attribute("type");
			if (strParamType.IsEmpty())
				strParamType = _T("S");

			CParamInfo* pParam = new CParamInfo();
			pParam->m_strParamName = strParamName;
			pParam->m_strParamValue = strParamValue;
			pParam->m_cParamType = strParamType[0];
			pWatchInfo->m_params.m_ayParams.Add(pParam);

			pParamItem = pParamItem->NextSiblingElement("Param");
		}
		m_ayWatcherInfo.Add(pWatchInfo);
		pWatchItem = pWatchItem->NextSiblingElement(_T("Item"));
	}
	return TRUE;
}

BOOL CWatcherInfoManager::AddWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatcherName)
{
	if( pWatchInfo == NULL || pWatchInfo->GetName().IsEmpty() )
	{
		return FALSE;
	}
	TiXmlDocument xmldoc;
	if( InitXmlDoc(xmldoc) == FALSE )
		return FALSE;

	TiXmlElement* pCombInfoRoot = GetWatcherRootItem(xmldoc,strWatcherName);
	if(!pCombInfoRoot)
		return FALSE;

	TiXmlElement *pWatchItem = new TiXmlElement(_T("Item"));  
	pWatchItem->SetAttribute(_T("name"),pWatchInfo->GetName());
	IWatcherParams *pPamams =  pWatchInfo->GetParams();
	for(int i = 0; i < pPamams->GetParamCount(); i++)
	{
		IParamInfo* pInfo = pPamams->GetParamInfo(i);
		if(pInfo != NULL)
		{
			CString strType;
			strType.Format("%c",pInfo->GetParamType());
			TiXmlElement *p = new TiXmlElement(_T("Param"));
			p->SetAttribute(_T("name"),pInfo->GetParamName());
			p->SetAttribute(_T("type"),strType);
			p->SetAttribute(_T("value"),pInfo->GetParamValue());
			pWatchItem->LinkEndChild(p);
		}
	}
	pCombInfoRoot->LinkEndChild(pWatchItem);

	xmldoc.SaveFile();

	return TRUE;
}

BOOL CWatcherInfoManager::ModifyWatcherInfo(IWatcherInfo* pWatchInfo,CString strWatcherName)
 {
	 TiXmlDocument xmldoc;
	 if( InitXmlDoc(xmldoc) == FALSE )
		 return FALSE;

	 TiXmlElement* pCombInfoRoot = GetWatcherRootItem(xmldoc,strWatcherName);
	 if(!pCombInfoRoot)
		 return FALSE;
	 CString strXpath;
	 strXpath.Format("//Item[@name=\"%s\"]",pWatchInfo->GetName());
	 TiXmlNode* node = TinyXPath::XNp_xpath_node(pCombInfoRoot, strXpath);
	 if(node)
	 {
		 node->Clear();
		 TiXmlElement *pWatchItem = node->ToElement();  
		 pWatchItem->SetAttribute(_T("name"),pWatchInfo->GetName());
		 IWatcherParams *pPamams =  pWatchInfo->GetParams();
		 for(int i = 0; i < pPamams->GetParamCount(); i++)
		 {
			 IParamInfo* pInfo = pPamams->GetParamInfo(i);
			 if(pInfo != NULL)
			 {
				 CString strType;
				 strType.Format("%c",pInfo->GetParamType());
				 TiXmlElement *p = new TiXmlElement(_T("Param"));
				 p->SetAttribute(_T("name"),pInfo->GetParamName());
				 p->SetAttribute(_T("type"),strType);
				 p->SetAttribute(_T("value"),pInfo->GetParamValue());
				 pWatchItem->LinkEndChild(p);
			 }
		 }
	 }

	 xmldoc.SaveFile();

	 return TRUE;
 }

BOOL CWatcherInfoManager::DelWatcherInfo(CString strWatcherSetName,CString strWatcherName)
{
	TiXmlDocument xmldoc;
	if( InitXmlDoc(xmldoc) == FALSE )
		return FALSE;

	TiXmlElement* pCombInfoRoot = GetWatcherRootItem(xmldoc,strWatcherName);
	if(!pCombInfoRoot)
		return FALSE;
	CString strXpath;
	strXpath.Format("//Item[@name=\"%s\"]",strWatcherName);
	TiXmlNode* node = TinyXPath::XNp_xpath_node(pCombInfoRoot, strXpath);
	if(node)
		pCombInfoRoot->RemoveChild(node);

	xmldoc.SaveFile();

	return TRUE;
}

IWatcherInfo* CWatcherInfoManager::GetWatcherInfo(CString strName)
{
	for(int i = 0; i < m_ayWatcherInfo.GetCount(); i++)
	{
		if (m_ayWatcherInfo[i]->GetName() == strName)
			return m_ayWatcherInfo[i];
	}

	return NULL;
}

IWatcherInfo* CWatcherInfoManager::GetWatcherInfo(int nIndex)
{
	if(nIndex >=0 && nIndex < m_ayWatcherInfo.GetCount())
	{
		return m_ayWatcherInfo[nIndex];
	}
	return NULL;
}

IWatcherInfo* CWatcherInfoManager::CreateWathcerInfo()
{
	return new CWatcherInfo();	
}

int CWatcherInfoManager::GetWatcherInfoCount()
{
	return m_ayWatcherInfo.GetCount();
}

void CWatcherInfoManager::Release()
{
	delete this;
}

BOOL CWatcherInfoManager::InitXmlDoc(TiXmlDocument &xmldoc)
{
	CString strFileName;
	CString strAppPath = CGeneralHelper::GetAppPath();
	strFileName.Format("%s%s",strAppPath, _T("combinfo\\combinfo.xml")); 
	if( !xmldoc.LoadFile(strFileName) )
	{
		return FALSE;
	}	
	return TRUE;
}

TiXmlElement*  CWatcherInfoManager::GetWatcherRootItem(TiXmlDocument &xmldoc,CString strWatcherSetName)
{
	TiXmlElement* pRoot = xmldoc.FirstChildElement(_T("Content"));
	if(!pRoot)
		return NULL;
	TiXmlElement* pCombInfoRoot = pRoot->FirstChildElement(strWatcherSetName);
	return pCombInfoRoot;
}