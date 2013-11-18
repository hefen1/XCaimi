/*******************************************************
  源程序名称:TradeDataBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易中心
  功能说明:  交易数据基础类实现
  作    者:  shenglq
  开发日期:  20100720
*********************************************************/

#include "stdafx.h"
#include "TradeDataBase.h"
#include "TradeDataBaseIntf.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//IColumn::~IColumn() {};
//
//CColumn::CColumn()
//{
//	m_sColumnName = "";
//	m_sDispName = "";
//	m_sDataDict = "";
//	m_sDispFormat = "";
//	m_eSumType = stNone;
//	m_bIsCalc = FALSE;
//	m_eAlign = alLeft;
//}
//
//CColumn::~CColumn()
//{
//}
//
//CString CColumn::GetColumnName()
//{  
//	return m_sColumnName; 
//}
//void CColumn::SetColumnName(const CString Value)
//{
//	m_sColumnName = Value;
//}
//
//CString CColumn::GetDispName()
//{
//	return m_sDispName; 
//}
//
//void CColumn::SetDispName(const CString Value)
//{
//	m_sDispName = Value;
//}
//CString CColumn::GetDataDict()
//{
//	return m_sDataDict; 
//}
//
//void CColumn::SetDataDict(const CString Value)
//{
//	m_sDataDict = Value;
//}
//
//CString CColumn::GetDispFormat()
//{
//	return m_sDispFormat; 
//}
//
//void CColumn::SetDispFormat(const CString Value)
//{
//	m_sDispFormat = Value;
//}
//
//ESumType CColumn::GetSumType()
//{
//	return m_eSumType;
//}
//
//void CColumn::SetSumType(const ESumType Value)
//{
//	m_eSumType = Value;
//}
//
//BOOL CColumn::GetIsCalc()
//{
//	return m_bIsCalc; 
//}
//
//void CColumn::SetIsCalc(const BOOL Value)
//{
//	m_bIsCalc = Value;
//}
//
//EAlign CColumn::GetAlign()
//{
//	return m_eAlign;
//}
//
//void CColumn::SetAlign(const EAlign Value)
//{
//	m_eAlign = Value;
//}
////////////////////////////////////////////////////////////////////////////
//IColumns::~IColumns(){};
//
//CColumns::CColumns()
//{
//	m_sLinkTable = "";
//	m_iDefaultSortIndex = 0;
//	m_eDefaultSortDirction = sdASC;
//}
//void CColumns::LoadColumns(const int iAction)
//{
//	
//}
//CColumns::~CColumns()
//{
//	for(int i = 0; i < m_ColumnList.GetCount(); ++i)
//	{
//		CColumn* pColumn = m_ColumnList[i];
//		delete pColumn;
//		pColumn = NULL;
//	}
//}
//int CColumns::GetCount()
//{
//	return m_ColumnList.GetCount();
//}
//
//IColumn* CColumns::GetColumns(const int i)
//{
//	if ((i >= 0) && (i < m_ColumnList.GetCount()))
//	{
//		CColumn* pColumn = m_ColumnList[i] ;	 
//		return pColumn;
//	}
//	else
//		return NULL;
//}
//
//IColumn* CColumns::GetColumnByName(const CString sName)
//{
//	CColumn* pColumn = NULL;
//	CString str1 = sName;
//	str1.MakeLower();
//	for (int i = 0; i < m_ColumnList.GetCount(); ++i)
//	{
//		CColumn* pTmp = m_ColumnList[i] ;
//		CString str2 = pTmp->GetColumnName();
//		str2.MakeLower();
//		if(str1 == str2)
//		{
//			pColumn = pTmp;
//			break;
//		}
//
//	}
//	return pColumn;
//}
//
//CString CColumns::GetLinkTable()
//{
//	return m_sLinkTable;
//}
//
//int CColumns::GetDefaultSortIndex()
//{
//	return  m_iDefaultSortIndex;
//}
//
//ESortDirection CColumns::GetDefaultSortDirction()
//{
//    return  m_eDefaultSortDirction;
//}


//INVData::~INVData(){};
//NV类
CNVData::CNVData()
{
	m_sName = "";
	m_sValue = "";
}

CNVData::~CNVData()
{
}

CString CNVData::GetName()
{
	return m_sName;
}
void CNVData::SetName(const CString Value)
{
     m_sName = Value;
}

CString CNVData::GetValue()
{
	return m_sValue;
}

void CNVData::SetValue(const CString Value)
{
	m_sValue = Value;
}

double CNVData::GetDouble(const double dDef)
{
	double dReturn = dDef;
	try
	{
		if (!m_sValue.IsEmpty())
		{
			dReturn = atof(m_sValue);
		}		
	}
	catch(...)
	{};
	return dReturn;
}

int CNVData::GetInt(const int iDef)
{
	int iReturn = iDef;
	try
	{
		if (!m_sValue.IsEmpty())
		{
			iReturn = atoi(m_sValue);
		}
	}
	catch(...)
	{};
	return iReturn;	
}

long CNVData::GetLong(const int iDef)
{
	long lReturn = iDef;
	try
	{
		if (!m_sValue.IsEmpty())
		{
			lReturn = atol(m_sValue);
		}
	}
	catch(...)
	{};
	return lReturn;	
}



IDataItem::~IDataItem(){};
// 每条数据访问接口
CDataItem::CDataItem()
{
	
}

CDataItem::~CDataItem()
{
	for(int i = 0; i < m_NVDataList.GetCount(); ++i)
	{
		CNVData* pNVData =  m_NVDataList[i];
		delete pNVData;
		pNVData = NULL;
	}
}

int CDataItem::GetIndexByName(const CString sName)
{
	int iIndex = -1;
	CString str1 = sName;
	str1.MakeLower();
	for (int i = 0; i < m_NVDataList.GetCount(); ++i)
	{		
		CNVData* pNVData =  m_NVDataList[i];
		CString str2 = pNVData->GetName();
		str2.MakeLower();
		if (_tcscmp(str1, str2) == 0)
		{
			iIndex = i;
			break;
		}
	}
	return iIndex;
}
/*
INVData* CDataItem::AddField()
{
	CNVData* pNVData = new CNVData;
	m_NVDataList.Add(pNVData);
	return pNVData;
}*/

void CDataItem::DeleteField(const int i)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData = m_NVDataList[i];
		delete pNVData;
		pNVData = NULL;
		m_NVDataList.RemoveAt(i);
	}
}

void CDataItem::DeleteFieldByName(const CString sName)
{
	if(sName.IsEmpty())
	{
		CNVData* pNVData = m_NVDataList[0];
		delete pNVData;
		pNVData = NULL;
		m_NVDataList.RemoveAt(0);
	}
}

CString CDataItem::GetStr(const int i)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		return pNVData->GetValue();
	}
	else
		return "";
}

char CDataItem::GetChar(const int i)
{
	char cChar[1];
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		CString str = pNVData->GetValue();
		strncpy_s(cChar, str,sizeof(cChar));

	};
	return cChar[0];
}

int CDataItem::GetInt(const int i, const int iDef /* = 0 */)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		return pNVData->GetInt(iDef);
	}
	else
		return iDef;
}

long CDataItem::GetLong(const int i, const int iDef /* = 0 */)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		return pNVData->GetLong(iDef);
	}
	else
		return iDef;
}

double CDataItem::GetDouble(const int i, const double dDef /* = 0 */)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		return pNVData->GetDouble(dDef);
	}
	else
		return dDef;
}

CString CDataItem::GetStrByName(CString sName)
{
	int iIndex = GetIndexByName(sName);
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetValue();
	}
	else
		return "";
}

char CDataItem::GetCharByName(const CString sName)
{
	char cChar[1];
	int iIndex = GetIndexByName(sName);
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		CString str = pNVData->GetValue();
		strncpy_s(cChar, str,sizeof(cChar));
		return cChar[0];
	}
	else
		return ' ';
}

int CDataItem::GetIntByName(const CString sName, const int iDef /* = 0 */)
{
	int iIndex = GetIndexByName(sName);
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetInt(iDef);
	}
	else
		return iDef;
}

long CDataItem::GetLongByName(const CString sName, const int iDef /* = 0 */)
{
	int iIndex = GetIndexByName(sName);
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetLong(iDef);
	}
	else
		return iDef;
}

double CDataItem::GetDoubleByName(const CString sName, const double dDef /* = 0 */)
{
	int iIndex = GetIndexByName(sName);
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetDouble(dDef);
	}
	else
		return dDef;
}

int CDataItem::GetErrorNo()
{
	int iIndex = GetIndexByName("error_no");
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetInt(0);
	}
	else
		return 0;
}

CString CDataItem::GetErrorInfo()
{
	int iIndex = GetIndexByName("error_info");
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetValue();
	}
	else
		return "";
}

CString CDataItem::GetEntrustNo()
{
	int iIndex = GetIndexByName("entrust_no");
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetValue();
	}
	else
		return "";
}
void CDataItem::SetData(const CString sName, const CString sValue)
{
	int iIndex = GetIndexByName(sName);
	if (iIndex == -1)
	{
		CNVData* pNVData = new CNVData;
		pNVData->SetName(sName);
		pNVData->SetValue(sValue);
	    m_NVDataList.Add(pNVData);
	}
	else
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->SetValue(sValue);
	}
}

CString CDataItem::GetSerialNo()
{
	int iIndex = GetIndexByName("serial_no");
	if (iIndex != -1)
	{
		CNVData* pNVData =  m_NVDataList[iIndex];
		return pNVData->GetValue();
	}
	else
		return "";
}

void CDataItem::AddData(const CString sName, const CString sValue)
{
	CNVData* pNVData = new CNVData;
	pNVData->SetName(sName);
	pNVData->SetValue(sValue);
	m_NVDataList.Add(pNVData);
}

int CDataItem::GetCount()
{
	return m_NVDataList.GetCount();
}

void CDataItem::GetFieldAndValue(const int i, CString &sField, CString &sValue)
{
	if((i >= 0) && (i < m_NVDataList.GetCount()))
	{
		CNVData* pNVData =  m_NVDataList[i];
		sField = pNVData->GetName();
	    sValue = pNVData->GetValue();
	}
}


//////////////////////////////////////////////////////////////////////////

CDataList::CDataList()
{
    InitializeCriticalSection(&m_cs);
	//m_pColumns = new CColumns;
}

CDataList::~CDataList()
{
	//delete m_pColumns;
	//m_pColumns = NULL;
	int iCount = GetCount();
	for(int i = 0; i < iCount; ++i)
	{
		IDataItem* pItem = m_DataItemList[i];
		delete pItem;
		pItem = NULL;
	}
	DeleteCriticalSection(&m_cs);
}

void CDataList::Release()
{
	delete this;
}

int CDataList::GetCount()
{
	return m_DataItemList.GetCount();
}

//void CDataList::LoadColumns()
//{
//	//m_pColumns->LoadColumns(401);
//}
//
//
//
//IColumn* CDataList::GetColumns(const int i)
//{
//	return  m_pColumns->GetColumns(i);
//}
//
//int CDataList::GetColumnCount()
//{
//	return  m_pColumns->GetCount();
//}

IDataItem* CDataList::GetItems(const int i)
{
	if ((i >= 0) &&(i < m_DataItemList.GetCount()))
	    return m_DataItemList[i];
	else
		return NULL;
}


void CDataList::AddItem(IDataItem * pItem)
{
	m_DataItemList.Add(pItem);
}

void CDataList::InsertItem(const int iIndex, IDataItem * pItem)
{
	if (iIndex >= 0)
	    m_DataItemList.InsertAt(iIndex, pItem);
}

void CDataList::DeleteItem(const int iIndex)
{
	if ((iIndex >= 0)&&(iIndex < m_DataItemList.GetCount()))
	{
		IDataItem* pItem = m_DataItemList[iIndex];
		delete pItem;
		pItem = NULL;
		m_DataItemList.RemoveAt(iIndex);
	}

}

IDataItem* CDataList::NewItem()
{
	CDataItem* pItem = new CDataItem;
	return pItem;
}

void CDataList::Lock()
{
	EnterCriticalSection(&m_cs);
}

void CDataList::UnLock()
{
	LeaveCriticalSection(&m_cs);
}

IDataList* CDataList::NewDataList()
{
	return new CDataList();
}

IDataList* CDataList::Clone()
{
	IDataList* pDataList = NewDataList();
	for (int i = 0; i < GetCount(); i++)
	{
		IDataItem* pItem = GetItems(i);
		IDataItem* pItemClone = NewItem();
		for (int j = 0; j < pItem->GetCount(); j++)
		{
			CString sField, sValue;
			pItem->GetFieldAndValue(j, sField, sValue);
			pItemClone->AddData(sField, sValue);
		}
		pDataList->AddItem(pItemClone);
	}
	return pDataList;
}