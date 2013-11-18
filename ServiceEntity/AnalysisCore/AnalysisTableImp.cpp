#include "StdAfx.h"
#include "tinyxml.h"
#include "AnalysisTableImp.h"
#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//AnalysisColumnI 实现=======================================================

void CAnalysisColumnImp::Release()
{
	delete this;
}

CAnalysisColumnImp::CAnalysisColumnImp()
{
	m_nIndex = 0;
	m_nWidth = 0;
}

CAnalysisColumnImp::~CAnalysisColumnImp()
{

}

CString CAnalysisColumnImp::GetName()
{
	return m_strName;
}

void CAnalysisColumnImp::SetName(CString strName)
{
	m_strName = strName;
}

CString CAnalysisColumnImp::GetDispName()
{
	return m_strDispName;
}

void CAnalysisColumnImp::SetDispName(CString strDispName)
{
	m_strDispName = strDispName;
}

CString CAnalysisColumnImp::GetExpression()
{
	IExpressionInfo* pExpInfo = GetExpInfo();
	if(pExpInfo)
		return pExpInfo->GetExpression();
	else
		return _T("");
}

IExpressionInfo* CAnalysisColumnImp::GetExpInfo()
{
	CExpressionInfoImp* pExpInfo = GetExpressionInfoManager()->GetExpressionInfo(m_strExpName); 
	return pExpInfo;
}

void CAnalysisColumnImp::SetExpName(CString strExpName)
{
	m_strExpName = strExpName;
}

int CAnalysisColumnImp::GetColumIndex()
{
	return m_nIndex;
}

int    CAnalysisColumnImp::GetColWidth()
{
	return m_nWidth;
}

void CAnalysisColumnImp::SetColumIndex(int nIndex)
{
	m_nIndex = nIndex;
}

void CAnalysisColumnImp::SetColWidth(int nWidth)
{
	m_nWidth = nWidth;
}

CString CAnalysisColumnImp::GetDispFormat()
{
	return m_strDispFormat;
}

void CAnalysisColumnImp::SetDispFormat(CString strDispFormat)
{
	m_strDispFormat = strDispFormat;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

CAnalysisTableImp::CAnalysisTableImp(void)
{
}

CAnalysisTableImp::~CAnalysisTableImp(void)
{
	POSITION pos;
	CString key;
	IAnalysisColumn* pColumn = NULL;
	for (pos = m_columnMap.GetStartPosition(); pos != NULL;)
	{
		m_columnMap.GetNextAssoc(pos, key, (void*&)pColumn);
		if(pColumn)
			pColumn->Release();
	}
}

//AnalysisTable 实现=======================================================

void CAnalysisTableImp::Release()
{
	delete this;
}

BOOL CAnalysisTableImp::AddColumn(CString strColName,IAnalysisColumn* pCol)
{
	IAnalysisColumn* pColunm = NULL; 
	if(m_columnMap.Lookup(strColName, (void*&)pColunm))
	{
		pColunm->Release();
	}
	m_columnMap.SetAt(strColName,pCol);
	return TRUE;
}

int CAnalysisTableImp::GetColumnCount()
{
	return m_columnMap.GetCount();
}

IAnalysisColumn* CAnalysisTableImp::GetColumn(const int nColIndex)
{
	POSITION pos;
	CString key;
	IAnalysisColumn* pColumn = NULL;
	for (pos = m_columnMap.GetStartPosition(); pos != NULL;)
	{
		m_columnMap.GetNextAssoc(pos, key, (void*&)pColumn);
		if(pColumn && pColumn->GetColumIndex() == nColIndex )
			return pColumn;
	}
	return NULL;
}

IAnalysisColumn* CAnalysisTableImp::GetColumn(const CString strColName)
{
	IAnalysisColumn* pColunm = NULL; 
	if(m_columnMap.Lookup(strColName, (void*&)pColunm))
		return pColunm;
	else
		return NULL;
}


//AnalysisTable 实现=======================================================
CAnalysisTableSet::CAnalysisTableSet()
{

}

CAnalysisTableSet::~CAnalysisTableSet()
{
}

IAnalysisTable* CAnalysisTableSet::GetTable(CString strName)
{
	CString strFileName;
	CString strAppPath = CGeneralHelper::GetAppPath();
	strFileName.Format("%s%s",strAppPath, _T("combinfo\\Indicatrix.xml")); 
	TiXmlDocument xmldoc;
	if( !xmldoc.LoadFile(strFileName) )
	{
		return NULL;
	}
	TiXmlElement* pRoot = xmldoc.FirstChildElement(_T("Content"));
	if(!pRoot)
		return NULL;

	TiXmlElement* pTablesRoot = pRoot->FirstChildElement("Tables");
	if(!pTablesRoot)
		return NULL;

	TiXmlElement* pTableIte = pTablesRoot->FirstChildElement(strName);
	if(!pTableIte)
		return NULL;

	CAnalysisTableImp *pTable = new CAnalysisTableImp();
	TiXmlElement* pColItem = pTableIte->FirstChildElement("Column");
	while(pColItem)
	{
		CString strColName = pColItem->Attribute(_T("name"));
		CString strDispName = pColItem->Attribute(_T("disp_name"));
		CString strExpName= pColItem->Attribute(_T("expression"));
		CString strIndex = pColItem->Attribute(_T("index"));
		int nIndex = atoi(strIndex);
		CString strWidth = pColItem->Attribute(_T("width"));
		int nWidth = atoi(strWidth);
		CString strDispFormat = pColItem->Attribute(_T("disp_format"));

		CAnalysisColumnImp* pColumn = new CAnalysisColumnImp();
		pColumn->SetName(strColName);
		pColumn->SetDispName(strDispName);
		pColumn->SetExpName(strExpName);
		pColumn->SetColumIndex(nIndex);
		pColumn->SetColWidth(nWidth);
		pColumn->SetDispFormat(strDispFormat);
		pTable->AddColumn(strColName,pColumn);

		pColItem = pColItem->NextSiblingElement(_T("Column"));
	}
	return pTable;
}
