#include "StdAfx.h"
#include "ExpressionInfoManagerImp.h"
#include "tinyxml.h"
#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CExpressionInfoManagerImp g_ExpInfoMgr; 

CString CExpParamInfoImp::GetName()
{
	return m_strName;
}

char     CExpParamInfoImp::GetType()
{
	return m_cType;
}

CString CExpParamInfoImp::GetNote()
{
	return m_strNote;
}

CString CExpParamInfoImp::GetFlag()
{
	return m_strFlag;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CExpressionInfoImp::~CExpressionInfoImp()
{
	for(int i = 0; i < m_ayParams.GetCount(); i++ )
	{
		CExpParamInfoImp* pInfo = m_ayParams.GetAt(i);
		if(pInfo)
			delete pInfo;
	}
	m_ayParams.RemoveAll();
}

void CExpressionInfoImp::GetParams(CMapStringToString &mapParams )
{
	CString strParamName,strParamNote;
	mapParams.RemoveAll();
	for(int i = 0; i < m_ayParams.GetCount(); i++ )
	{
		CExpParamInfoImp* pInfo = m_ayParams.GetAt(i);
		mapParams.SetAt(pInfo->GetName(),pInfo->GetNote());
	}
}

int  CExpressionInfoImp::GetParamCount()
{
	return m_ayParams.GetCount();
}

IExpParamInfo* CExpressionInfoImp::GetParamInfo(int nIndex)
{
	if(nIndex >= 0 && nIndex < m_ayParams.GetCount() )
		return m_ayParams.GetAt(nIndex);
	return NULL;
}

CExpressionInfoManagerImp::CExpressionInfoManagerImp()
{
	LoadExpression();
}

CExpressionInfoManagerImp::~CExpressionInfoManagerImp()
{
	CExpressionInfoImp* pExpInfo = NULL;
	CString strKey;
	for (POSITION pos = m_mapExpressionInfo.GetStartPosition(); pos != NULL;)
	{
		m_mapExpressionInfo.GetNextAssoc(pos,strKey,(CObject*&)pExpInfo);
		if (pExpInfo)
			delete pExpInfo;
	}
}

void CExpressionInfoManagerImp::LoadExpression()
{
	CString strFileName;
	CString strAppPath = CGeneralHelper::GetAppPath();
	strFileName.Format("%s%s",strAppPath, _T("combinfo\\Indicatrix.xml")); 
	TiXmlDocument xmldoc;
	if( !xmldoc.LoadFile(strFileName) )
		return;

	TiXmlElement* pRoot = xmldoc.FirstChildElement(_T("Content"));
	if(!pRoot)
		return;

	TiXmlElement* pCombInfoRoot = pRoot->FirstChildElement("Expression");
	if(!pCombInfoRoot)
		return;

	TiXmlElement* pExpItem = pCombInfoRoot->FirstChildElement(_T("Item"));
	CString strKey;
	CExpressionInfoImp* pExpInfoExist = NULL;
	while(pExpItem)
	{
		CExpressionInfoImp *pExpInfo = new CExpressionInfoImp();
		pExpInfo->m_strName = pExpItem->Attribute(_T("name"));
		pExpInfo->m_strExpression = pExpItem->Attribute(_T("exp"));
		if( pExpInfo->m_strExpression.IsEmpty())
		{
			pExpInfo->m_strExpression = pExpItem->GetText();
		}	
		TiXmlElement* pExpParam = pExpItem->FirstChildElement(_T("Param"));
		while(pExpParam)
		{
			CString strParamName = pExpParam->Attribute(_T("name"));
			CString strNote = pExpParam->Attribute(_T("note"));
			CString strFlag = pExpParam->Attribute(_T("note"));
			CString strType = pExpParam->Attribute(_T("type"));
			CExpParamInfoImp* pInfo = new CExpParamInfoImp();
			pInfo->m_strName = strParamName;
			pInfo->m_strNote = strNote;
			pInfo->m_strFlag = strFlag;
			if(strType.IsEmpty())
				pInfo->m_cType = PT_STRING;
			else
				pInfo->m_cType = strType[0];
			pExpInfo->m_ayParams.Add(pInfo);
			//pExpInfo->m_mapParams.SetAt(strParamName,strNote);
			pExpParam = pExpParam->NextSiblingElement(_T("Param"));
		}	
		//删除重名的，一般不会发生，除非配置重复
		if(m_mapExpressionInfo.Lookup(pExpInfo->m_strName,(CObject*&)pExpInfoExist))
		{
			if(pExpInfoExist)
				delete pExpInfoExist;
		}
		m_mapExpressionInfo.SetAt(pExpInfo->m_strName,(CObject*)pExpInfo);

		pExpItem = pExpItem->NextSiblingElement(_T("Item"));
	}
}

CExpressionInfoImp* CExpressionInfoManagerImp::GetExpressionInfo(CString strExpName)
{
	CExpressionInfoImp* pExpInfo = NULL;
	m_mapExpressionInfo.Lookup(strExpName,(CObject*&)pExpInfo);
	return pExpInfo;
}

CExpressionInfoManagerImp* GetExpressionInfoManager()
{
	return &g_ExpInfoMgr;
}