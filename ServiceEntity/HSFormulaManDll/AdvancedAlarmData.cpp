#include "stdafx.h"
#include "AdvancedAlarmData.h"
#include "Express.h"
#include "GeneralHelper.h"
extern HWND			 g_hParentWnd;
extern BOOL GetStockByCode(CodeInfo* pInfo, StockUserInfo& stockInfo );
int CAdvancedAlarmData::m_nCycle = 5;

CAdvancedAlarmData::CAdvancedAlarmData(void)
{
	m_pUnionCondition = NULL;
	m_pSingleSel = NULL;

	Init();
}

CAdvancedAlarmData::~CAdvancedAlarmData(void)
{
	Free();
}

BOOL CAdvancedAlarmData::SetCondition(SingleSelCondition* sSingleSel)
{
	if( sSingleSel != NULL )
	{
		if( m_pSingleSel != NULL )
			delete m_pSingleSel;
		m_pSingleSel = sSingleSel;
		return TRUE;
	}
	return FALSE;
}

BOOL CAdvancedAlarmData::SetUnionCondition(UnionSelCondition* UnionSel)
{
	if( UnionSel != NULL )
	{
		if( m_pUnionCondition != NULL )
			delete m_pUnionCondition;
		m_pUnionCondition = UnionSel;
		return TRUE;
	}
	return FALSE;
}

void CAdvancedAlarmData::Init()
{
	m_uAlarmType = HS_Box | HS_NoBreak | HS_Ts | HS_Avi | HS_NoBreak | HS_ALARM_ON | HS_ALARM_ENABLE;
	m_nCount = 0;
	m_SingleOrUnion = TRUE;				//µ¥¹«Ê½
	m_strSoundFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH)  + "AlarmSound.wav";
}

void CAdvancedAlarmData::Free()
{
	m_ayStkList.RemoveAll();
	//m_ayAlarmConditionName.RemoveAll();
	if( m_pUnionCondition != NULL )
	{
		delete m_pUnionCondition;
		m_pUnionCondition = NULL;
	}

	if( m_pSingleSel != NULL )
	{
		delete m_pSingleSel;
		m_pSingleSel = NULL;
	}
}

void CAdvancedAlarmData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		StockUserInfo* pStock;
		ar << m_strName;
		ar << m_strAlarmBlock;
		ar << m_uAlarmType;
		ar << m_nCycle;
		ar << m_nCount;
		ar << m_strSoundFileName;
		ar << m_ayStkList.GetSize();
		for(int i = 0; i < m_ayStkList.GetSize(); i++ )
		{
			pStock = m_ayStkList.GetAt(i);
			CString strTemp;
			strTemp.Format("%s", pStock->m_ciStockCode.GetCode());
			ar << strTemp;
			ar << pStock->m_ciStockCode.m_cCodeType;
		}
		ar << m_SingleOrUnion;

		int nNullFlag;

		if( m_SingleOrUnion == FALSE )
		{
			if( m_pUnionCondition != NULL )
			{
				nNullFlag = 1;
				ar << nNullFlag;
				m_pUnionCondition->Serialize(ar);
			}
			else
			{
				nNullFlag = 0;
				ar << nNullFlag;
			}
		}
		else
		{
			if( m_pSingleSel != NULL )
			{
				nNullFlag = 1;
				ar << nNullFlag;
				m_pSingleSel->Serialize(ar);
			}
			else
			{
				nNullFlag = 0;
				ar << nNullFlag;
			}
		}
	}
	else
	{
		CodeInfo srtCode;
		StockUserInfo* pStock = new StockUserInfo;
		int count;
		CString strTemp;

		ar >> m_strName;
		ar >> m_strAlarmBlock;
		ar >> m_uAlarmType;
		ar >> m_nCycle;
		ar >> m_nCount;
		ar >> m_strSoundFileName;
		ar >> count;
		for(int i = 0; i < count; i++ )
		{
			CString strTemp;
			ar >> strTemp;
			strncpy(srtCode.m_cCode, strTemp, sizeof(char) * 6);
			ar >> srtCode.m_cCodeType;

			pStock = NULL;
			GetStockByCode(&srtCode, *pStock);		
			if( pStock != NULL )
			{
				m_ayStkList.Add(pStock);
			}

		}
		ar >> m_SingleOrUnion;
		int nNullFlag;
		ar >> nNullFlag;
		if( nNullFlag == 1 )
		{
			if( m_SingleOrUnion == FALSE )
			{
				if( m_pUnionCondition == NULL )
				{
					m_pUnionCondition= new UnionSelCondition;
				}
				m_pUnionCondition->Serialize(ar);
			}
			else
			{
				if( m_pSingleSel == NULL )
				{
					m_pSingleSel = new SingleSelCondition;
				}
				m_pSingleSel->Serialize(ar);
			}
		}
	}
}

void CAdvancedAlarmData::SetBlockName(CString block)
{
	if( block.IsEmpty() )
	{
		m_uAlarmType &= ~HS_Block;
		m_strAlarmBlock.Empty();
		return;
	}
	m_strAlarmBlock = block;
	m_uAlarmType |= HS_Block;
}

void CAdvancedAlarmData::AddCount(int nSize)
{
	m_nCount += nSize;
}

BOOL CAdvancedAlarmData::AddStock(StockUserInfo* pData)
{
	StockUserInfo* pTemp;
	for(int i = 0; i < m_ayStkList.GetSize(); i++ )
	{
		pTemp = m_ayStkList.GetAt(i);
		if( memcmp(pTemp->m_ciStockCode.m_cCode, pData->m_ciStockCode.m_cCode, sizeof(char) * 6) == 0 )
			return FALSE;
	}
	m_ayStkList.Add(pData);
	return TRUE;
}

BOOL CAdvancedAlarmData::DelStock(int nIndex)
{
	for ( int  i=0; i<m_ayStkList.GetCount(); i++)
	{
		StockUserInfo* pStock = m_ayStkList.GetAt(i);
		if ( pStock && !IsBadReadPtr(pStock, 1))
			delete pStock;
	}
	m_ayStkList.RemoveAt(nIndex);
	return TRUE;
}

int	CAdvancedAlarmData::GetStkCount()
{
	return m_ayStkList.GetSize();
}

BOOL CAdvancedAlarmData::CopyStkTo(CArray<StockUserInfo*, StockUserInfo*>* payStkList)
{
	if( payStkList == NULL )
		return FALSE;
	payStkList->Copy(m_ayStkList);
	return TRUE;
}

BOOL CAdvancedAlarmData::CopyStk(CArray<StockUserInfo*, StockUserInfo*>* payStkList)
{
	if( payStkList == NULL )
		return FALSE;
	m_ayStkList.Copy(*payStkList);
	return TRUE;
}

void CAdvancedAlarmData::RemoveAllStk()
{
	for ( int  i=0; i<m_ayStkList.GetCount(); i++)
	{
		StockUserInfo* pStock = m_ayStkList.GetAt(i);
		if ( pStock && !IsBadReadPtr(pStock, 1))
			delete pStock;
	}
	m_ayStkList.RemoveAll();
}