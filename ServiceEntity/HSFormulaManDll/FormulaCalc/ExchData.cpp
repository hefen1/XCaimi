#include "stdafx.h"
#include "FormulaCalc.h"

#include "ExchData.h"
#include "pubstruct.h"
#include "ExpValue.h"
#include "pageid.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if( lParam1 > lParam2 )
	{
		return 1;
	}
	if( lParam1 < lParam2 )
	{
		return -1;
	}
	return 0;
} 

// 手续费
double EXCH_POUNDAGE = 0.0035;

long TDealFlowStock::m_lLastBuy = 0;
double TDealFlowStock::m_dAllMoney = 0;
double	TDealFlowStock::s_dLastMonery = 0;

long TExchBuy::m_lBeginDate = 0;
long TExchBuy::m_lEndDate = 0;
BOOL TDealFlowStock::m_bLastSell = FALSE;
// TExchHead

void TExchHead::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_bType;
	}
	else
	{
		ar >> m_bType;
	}
}

// TExchMsg
TExchMsg::TExchMsg()
{
	ZeroMemory(this, sizeof(TExchMsg));
	m_bNotify = TRUE;
}

BOOL TExchMsg::IsNotify()
{
	if( m_bNotify == FALSE )
		return false;
	if( !::IsWindow(m_hNotifyWnd) )
		return false;
	return true;
}

LRESULT TExchMsg::SendMsg(WPARAM wp, LPARAM lp)
{
	if( IsNotify() )
	{
		return ::SendMessage(m_hNotifyWnd, m_hNotifyMsg, wp, lp);
	}

	return TCaclReturn_Fail;
}

// TExchExp
TExchExp::TExchExp()
{
	m_bType = EXCH_DATA_TYPE_Exp;
	m_pMainTech = NULL;
	// 分析周期,default日线
	m_lCyc = AnalisysForDay;
	m_bExRights = TRUE;
}

void TExchExp::Serialize(CArchive& ar, CMapVariabile* pMapExpression/* = NULL*/)
{
	if( ar.IsStoring() )
	{
		if( m_pMainTech != NULL )
		{
			ar << m_pMainTech->GetName();
			ar << m_pMainTech->GetExpressType();
		}
		else
		{
			ar << CString("");
			ar << 0;
		}
		ar << m_bExRights;
		ar << m_lCyc;
	}
	else
	{
		CString strName;
		char	chType;

		ar >> strName;
		ar >> chType;
		ar >> m_bExRights;
		ar >> m_lCyc;

		strName.MakeUpper();
		CValue* valoare = NULL;
		if ( pMapExpression == NULL || 
			!pMapExpression->Lookup(strName,chType,valoare) )
		{
			m_pMainTech = NULL;
		}
		if ( valoare && !IsBadReadPtr(valoare, sizeof(CValue)))	
			m_pMainTech = ((CExpValue*)valoare)->GetExpByCompile();
	}
}

TExchExp& TExchExp::operator=(TExchExp& sExp)
{
	m_lCyc = sExp.m_lCyc;
	m_bExRights = sExp.m_bExRights;
	m_pMainTech = sExp.m_pMainTech;
	m_bType = sExp.m_bType;
	return *this;
}

CString TExchExp::GetExpTypeName()
{
	if( m_pMainTech == NULL )
		return CString("");

	switch( m_pMainTech->GetExpressType() )
	{
	case Tech:
		return CString(_T("技术指标"));
	case Exchange:
		return CString(_T("交易系统"));
	case Condition:
		return CString(_T("条件选股"));
	case MoreKLine:
		return CString(_T("五彩Ｋ线"));
	case Strategy: //20090302 YJT 增加 Strategy
		return CString(_T("策略交易"));
	}

	return CString("");
}

CString TExchExp::GetParamStr()
{
	if( m_pMainTech == NULL )
		return CString("");

	CMapVariabile* pParam = m_pMainTech->GetParamVar();
	if( pParam == NULL ) return CString("");

	CString strTemp;
	CString str("(");
	int lCount = pParam->GetSize();
	for( int i = 0; i < lCount; i++ )
	{
		CNumericValue* pNum = (CNumericValue*)pParam->GetAt(i);
		if( pNum != NULL )
		{
			strTemp.Format("%.2f", pNum->GetValue());
			
			if( strTemp.GetAt(strTemp.GetLength() - 1 ) == '0' )
			{
				int lSize = strTemp.GetLength();
				strTemp = strTemp.Left(lSize-1);
				if( strTemp.GetAt(strTemp.GetLength() - 1 ) == '0' )
				{
					lSize = strTemp.GetLength();
					strTemp = strTemp.Left(lSize-2);
				}
			}
			str += strTemp;
		}
		else
		{
			str += "0";
		}
		if( i != lCount - 1 )
			str += ",";
		else
			str += ")";
	}
	return str;
}

// TExchModel
TExchModel::TExchModel()
{
	m_bType = EXCH_DATA_TYPE_Mode;

	m_bSingle = 1;
	m_nHoleStockCount = 30;
	m_bBuyAll = 1;
	m_nOneBuyCount = 10;
	m_bBuyRandom = 1;
	m_bSToB = 0;
	m_lAllInvestCount = 100000;
	m_ProcGetStockUser = NULL;
}
TExchModel::~TExchModel()
{
	
}
void TExchModel::Clear()
{
	for ( int i=0; i<m_ayStkList.GetCount(); i++)
	{
		StockUserInfo* pStock = m_ayStkList.GetAt(i);
		if ( pStock && !IsBadReadPtr(pStock,1) )
			delete pStock;
		pStock = NULL;
	}
	m_ayStkList.RemoveAll();
}
void TExchModel::Copy( TExchModel &Model )
{
	m_bSingle = Model.m_bSingle;			
	m_nHoleStockCount = Model.m_nHoleStockCount;
	m_bBuyAll = Model.m_bBuyAll;			
	m_nOneBuyCount = Model.m_nOneBuyCount;
	m_bBuyRandom = Model.m_bBuyRandom;
	m_bSToB   = Model.m_bSToB;
	m_lAllInvestCount = Model.m_lAllInvestCount;
	m_sSortReason.Copy(&Model.m_sSortReason);
	m_ayStkList.Copy(Model.m_ayStkList);
	m_bType = Model.m_bType;
	m_ProcGetStockUser = Model.m_ProcGetStockUser;
}

TExchModel& TExchModel::operator=(TExchModel& Mode)
{
	Copy(Mode);
	return *this;
}

void TExchModel::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_bSingle;
		ar << m_nHoleStockCount;
		ar << m_bBuyAll;
		ar << m_nOneBuyCount;
		ar << m_bBuyRandom;
		ar << m_bSToB;
		ar << m_lAllInvestCount;
		m_sSortReason.Serialize(ar);
		ar << m_ayStkList.GetSize();
		for( int i = 0; i < m_ayStkList.GetSize(); i++ )
		{
			StockUserInfo* pStock = m_ayStkList.GetAt(i);
			if( pStock != NULL )
			{
				CString str(pStock->m_ciStockCode.m_cCode, sizeof(pStock->m_ciStockCode.m_cCode));
				ar << str;
				ar << pStock->m_ciStockCode.m_cCodeType;
			}
		}
		ar << EXCH_POUNDAGE;
	}
	else
	{
		ar >> m_bSingle;
		ar >> m_nHoleStockCount;
		ar >> m_bBuyAll;
		ar >> m_nOneBuyCount;
		ar >> m_bBuyRandom;
		ar >> m_bSToB;
		ar >> m_lAllInvestCount;
		m_sSortReason.Serialize(ar);
		int lCount;
		ar >> lCount;
		if( m_ProcGetStockUser != NULL )
		{
			for( int i = 0; i < lCount; i++ )
			{
				CString str;
				CodeInfo sCode;
				ar >> str;
				strncpy(sCode.m_cCode, str,  min(sizeof(sCode.m_cCode), str.GetLength()));			
				ar >> sCode.m_cCodeType;
				StockUserInfo* pStock = new StockUserInfo;
				(*m_ProcGetStockUser)(sCode,*pStock);
				m_ayStkList.Add(pStock);
			}
		}
		else
		{
			for( int i = 0; i < lCount; i++ )
			{
				CodeInfo sCode;
				CString str;
				ar >> str;
				ar >> sCode.m_cCodeType;
				StockUserInfo* pStock = NULL;
				m_ayStkList.Add(pStock);
			}
		}
		ar >> EXCH_POUNDAGE;
	}
}

// TExchBuy::
TExchBuy::TExchBuy()
{
	m_bType = EXCH_DATA_TYPE_Buy;
	m_lDateCount = 0;
	m_bNoCountIn = 0;
	m_SubData.m_cType = FormulaCompareSubData::DaYu;
	m_SubData.m_dData1 = _T("5");
	m_SubData.m_dData2 = _T("5");
	m_nBuyMethod = BuyMethod_All_Buy;
	m_fOneBuyCount = 30;
	m_nFollowBuyMethod = FollowBuyMethod_NO;
	m_fAddOrSubCount = 50;
	m_bSkip = 0;
	m_nInPeriod = 10;
}

TExchBuy& TExchBuy::operator=(TExchBuy& Buy)
{
	m_bType			= Buy.m_bType;

	m_lDateCount	= Buy.m_lDateCount;
	m_lBeginDate	= Buy.m_lBeginDate;
	m_lEndDate		= Buy.m_lEndDate;
	m_bNoCountIn	= Buy.m_bNoCountIn;
	m_strTechLeft	= Buy.m_strTechLeft;
	m_SubData.m_cType = Buy.m_SubData.m_cType;
	m_SubData.m_dData1 = Buy.m_SubData.m_dData1;
	m_SubData.m_dData2 = Buy.m_SubData.m_dData2;
	m_nBuyMethod = Buy.m_nBuyMethod;
	m_fOneBuyCount = Buy.m_fOneBuyCount;
	m_nFollowBuyMethod = Buy.m_nFollowBuyMethod;
	m_fAddOrSubCount = Buy.m_fAddOrSubCount;
	m_bSkip = Buy.m_bSkip;
	m_nInPeriod = Buy.m_nInPeriod;
	return *this;
}

void TExchBuy::Default(CExpression *pExp, CString strDefault)
{
	if( pExp == NULL )
	{
		return;
	}

	m_SubData.m_cType = FormulaCompareSubData::DaYu;
	m_SubData.m_dData1 = _T("5");
	m_SubData.m_dData2 = _T("5");

	CValue* valoare = NULL;
	if( CExpression::m_pExternExpression->Lookup(pExp->GetName(),pExp->GetExpressType(),valoare) ) // over
	{
		CExpValue* pExpValue = (CExpValue*)valoare;

		BOOL bFree = FALSE;
		CExpression* pCurExpression = pExpValue->GetExp();
		if( pCurExpression )
		{
			CArray<CFormulaDrawParam*,CFormulaDrawParam*> ayDraw;
			CString strParam;
			int nCount = pCurExpression->GetDraw(strParam,ayDraw);
			CFormulaDrawParam* pParam;
			if( nCount > 0 )
			{
				for(int i = 0; i < nCount; i++)
				{
					pParam = ayDraw.GetAt(i);
					if( pParam != NULL )
					{
						if( CString(pParam->m_strName).IsEmpty() )
						{
							m_strTechLeft = strDefault;
						}
						else
						{
							m_strTechLeft = pParam->m_strName;
						}
					}
				}
				_delArrayObj(ayDraw);
			}
			else
			{
				m_strTechLeft = strDefault;
			}
		}
	}				
}

void TExchBuy::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_lDateCount;
		ar << m_lBeginDate;
		ar << m_lEndDate;
		ar << m_bNoCountIn;
		ar << m_strTechLeft;
		m_SubData.Serialize(ar);
		ar << m_nBuyMethod;
		ar << m_fOneBuyCount;
		ar << m_nFollowBuyMethod;
		ar << m_fAddOrSubCount;
		ar << m_bSkip;
		ar << m_nInPeriod;
	}
	else
	{
		ar >> m_lDateCount;
		ar >> m_lBeginDate;
		ar >> m_lEndDate;
		ar >> m_bNoCountIn;
		ar >> m_strTechLeft;
		m_SubData.Serialize(ar);
		ar >> m_nBuyMethod;
		ar >> m_fOneBuyCount;
		ar >> m_nFollowBuyMethod;
		ar >> m_fAddOrSubCount;
		ar >> m_bSkip;
		ar >> m_nInPeriod;
	}
}

// TExchSell::
TExchSell::TExchSell()
{
	m_bType = EXCH_DATA_TYPE_Sell;

	m_bMaidPeriod = 1;
	m_bWinPeriod = 0;	
	m_bLosePeriod = 0;	
	m_bFallPeriod = 0;	
	m_bUpPeriod = 0;	

	m_nMaidPeriod = 20;
	m_nWinPeriod = 10;
	m_nLosePeriod = 4;
	m_nFallPeriod = 4;
	m_nUpPeriod = 4;
	m_nFallPeriodCount = 10;
	m_nUpPeriodCount = 20;
}
TExchSell& TExchSell::operator=(TExchSell& sell)
{
	m_bType				= sell.m_bType				;

	m_bMaidPeriod		= sell.m_bMaidPeriod		;	
	m_bWinPeriod		= sell.m_bWinPeriod			;	
	m_bLosePeriod		= sell.m_bLosePeriod		;	
	m_bFallPeriod		= sell.m_bFallPeriod		;	
	m_bUpPeriod			= sell.m_bUpPeriod			;	
	m_nMaidPeriod		= sell.m_nMaidPeriod		;	
	m_nWinPeriod		= sell.m_nWinPeriod			;	
	m_nLosePeriod		= sell.m_nLosePeriod		;	
	m_nFallPeriod		= sell.m_nFallPeriod		;	
	m_nUpPeriod			= sell.m_nUpPeriod			;	
	m_nFallPeriodCount	= sell.m_nFallPeriodCount	;
	m_nUpPeriodCount	= sell.m_nUpPeriodCount		;
	return *this;
}

void TExchSell::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_bMaidPeriod;	
		ar << m_bWinPeriod;	
		ar << m_bLosePeriod;	
		ar << m_bFallPeriod;	
		ar << m_bUpPeriod;	
		ar << m_nMaidPeriod;		
		ar << m_nWinPeriod;		
		ar << m_nLosePeriod;			
		ar << m_nFallPeriod;		
		ar << m_nUpPeriod;			
		ar << m_nFallPeriodCount;		
		ar << m_nUpPeriodCount;	
	}
	else
	{
		ar >> m_bMaidPeriod;	
		ar >> m_bWinPeriod;	
		ar >> m_bLosePeriod;	
		ar >> m_bFallPeriod;	
		ar >> m_bUpPeriod;	
		ar >> m_nMaidPeriod;		
		ar >> m_nWinPeriod;		
		ar >> m_nLosePeriod;			
		ar >> m_nFallPeriod;		
		ar >> m_nUpPeriod;			
		ar >> m_nFallPeriodCount;		
		ar >> m_nUpPeriodCount;	
	}
}

// TExchOptimizeItem::
TExchOptimizeItem::TExchOptimizeItem()
{
	ZeroMemory(this, sizeof(TExchOptimizeItem));
}

TExchOptimizeItem::~TExchOptimizeItem()
{
}

TExchOptimizeItem& TExchOptimizeItem::operator=(const TExchOptimizeItem& sItem)
{
	memcpy(this, &sItem, sizeof(TExchOptimizeItem));
	return *this;
}

void TExchOptimizeItem::ConvertToNumVal(CNumericValue& sValue)
{
	sValue.SetMaxValue(m_dMax);
	sValue.SetMinValue(m_dMin);
	sValue.SetTestStepValue(m_dAdd);
}

void TExchOptimizeItem::ConvertFromNumVal(CNumericValue* sValue, CMapVariabile* sMapVal, BOOL bValid)
{
	CString strName = sMapVal->GetObjectName(sValue);
	sprintf(m_szName, "%s", strName);
	m_szName[63] = 0;
	if( bValid == FALSE )
	{
		m_dMax = sValue->GetMaxValue();
		m_dMin = sValue->GetMinValue();
		m_dAdd = sValue->GetTestStepValue() == 0 ? ((m_dMax-m_dMin) / 10 == 0 ? 1 : (m_dMax-m_dMin) ): sValue->GetTestStepValue();
	}
	else
	{
		m_dMax = sValue->GetMaxValue();
		m_dMin = sValue->GetMinValue();
		double dTemp = sValue->GetValue();
		double dTempUpDown = m_dMax - m_dMin;
		long lDouble = (long)dTempUpDown;
		if( lDouble > 5 )
		{
			m_dMax = min(m_dMax, dTemp + 5);
			m_dMin = max(m_dMin, dTemp - 5);
			m_dAdd = 1;
		}
		else
		{
			m_dAdd = sValue->GetTestStepValue() == 0 ? ((m_dMax-m_dMin) / 10 == 0 ? 1 : (m_dMax-m_dMin) ): sValue->GetTestStepValue();
		}
	}
}

long TExchOptimizeItem::GetNum()
{
	return (long)((m_dMax - m_dMin)/(m_dAdd==0?1:m_dAdd)) + 1;
}

double TExchOptimizeItem::GetParam(int nIdx)
{
	if( nIdx < 0 )
	{
		return m_dMin;
	}
	else
	{
		return min(m_dMax, m_dMin + nIdx * m_dAdd);
	}
}

void TExchOptimizeItem::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << CString(m_szName);
		ar << m_dMin;
		ar << m_dMax;
		ar << m_dAdd;
	}
	else
	{
		CString strName;
		ar >> strName;
		sprintf(m_szName, "%s", strName);
		m_szName[63] = 0;
		ar >> m_dMin;
		ar >> m_dMax;
		ar >> m_dAdd;
	}
}

// TExchOptimize::
TExchOptimize::TExchOptimize()
{
	lValidLen = 0;
	lBufLen = 0;
	pItem = NULL;
	pIsValid = NULL;
	m_bType = EXCH_DATA_TYPE_Optimize;
}
TExchOptimize::~TExchOptimize()
{
	Free();
}

TExchOptimize::TExchOptimize(const TExchOptimize& sData)
{
	*this = sData;
}

TExchOptimize& TExchOptimize::operator=(const TExchOptimize& sData)
{
	if( !New( sData.lBufLen ) )
		return *this;
	lValidLen = sData.lValidLen;
	memcpy(pItem, sData.pItem, sizeof(TExchOptimizeItem) * lValidLen);
	memcpy(pIsValid, sData.pIsValid, lValidLen);
	m_bType = sData.m_bType;
	return *this;
}

TExchOptimizeItem* TExchOptimize::operator[](int iIdx)
{
	if( !IsValid() || iIdx > lValidLen || iIdx < 0 /*|| pIsValid[iIdx] == 'n'*/ )
		return NULL;
	return &pItem[iIdx];
}

// 转换到CMapVariabile
void TExchOptimize::ConvertToMapNumVal(CMapVariabile& sMapVal)
{
}

// 从CMapVariabile转来
BOOL TExchOptimize::ConvertFromMapNumVal(CMapVariabile* sMapVal, BOOL bValid)
{
	if( sMapVal == NULL )
	{
		Free();
		return TRUE;
	}

	if( !New(sMapVal->GetSize()) || !IsValid() )
		return false;

	for( int i = 0; i < sMapVal->GetSize(); i++ )
	{
		CNumericValue* pNumVal = (CNumericValue*)sMapVal->GetAt(i);
		TExchOptimizeItem* pSubItem = &pItem[i];

		if( pSubItem == NULL || pNumVal == NULL )
			return false;

		pSubItem->ConvertFromNumVal(pNumVal, sMapVal, bValid);
		if( bValid )
		{
			pIsValid[i] = 'y';
		}
		else
		{
			pIsValid[i] = 'n';
		}
	}
	lValidLen = sMapVal->GetSize();

	return true;
}

// 从公式数据转换
BOOL TExchOptimize::ConvertFromExpress(CExpression* pExpress, BOOL bValid)
{
	if( pExpress == NULL )
		return false;
	
	return ConvertFromMapNumVal(pExpress->GetParamVar(), bValid);
}

BOOL TExchOptimize::IsValid()
{
	if( pIsValid == NULL || pItem == NULL || lBufLen <= 0 || lValidLen > lBufLen || lValidLen < 0 )
		return false;
	return true;
}

BOOL TExchOptimize::IsValidItem(int nIdx)
{
	if( !IsValid() )
		return false;

	if( nIdx >= lValidLen || nIdx < 0 )
		return false;

	if( pIsValid[nIdx] == 'y' )
		return true;

	return false;
}

BOOL TExchOptimize::New(int lLen)
{
	Free();

	pItem = (TExchOptimizeItem*)new char[sizeof(TExchOptimizeItem) * lLen];
	if( pItem == NULL )
		return FALSE;

	pIsValid = new char[lLen];
	if( pIsValid == NULL )
	{
		char* pTemp = (char*)pItem;
		delete[] pTemp;
		pItem = NULL;
		return FALSE;
	}
	lValidLen = 0;
	lBufLen = lLen;
	return TRUE;
}

void TExchOptimize::Free()
{
	if( pItem != NULL )
		delete[] (char*)pItem;
	if( pIsValid != NULL )
		delete[] pIsValid;

	pIsValid = NULL;
	pItem = NULL;
	lBufLen = 0;
	lValidLen = 0;
}

void TExchOptimize::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << lValidLen;
		ar << lBufLen;

		for( int i = 0; i < lValidLen; i++ )
		{
			if( (pItem+i) != NULL )
			{
				pItem[i].Serialize(ar);
			}
		}
		CString str(pIsValid);
		ar << str;
	}
	else
	{
		int nLen;
		ar >> nLen;
		ar >> lBufLen;		

		if( New(lBufLen) )
		{
			lValidLen = nLen;
			for( int i = 0; i < lValidLen; i++ )
			{
				if( (pItem+i) != NULL )
				{
					pItem[i].Serialize(ar);
				}
			}
			CString str;
			ar >> str;
			strncpy(this->pIsValid, str.GetBuffer(), min(lBufLen, str.GetLength()) );
			str.ReleaseBuffer();
		}
		else
		{
			for( int i = 0; i < lValidLen; i++ )
			{
				TExchOptimizeItem sItem;
				sItem.Serialize(ar);
			}
			CString str;
			ar >> str;
		}
	}
}

int TExchOptimize::GetValidItemCount()
{
	int nRet = 0;

	if( IsValid() == FALSE )
		return nRet;
	for( int i = 0; i < lValidLen; i++ )
	{
		if( pIsValid[i] == 'y' )
			nRet++;
	}

	return nRet;
}

// 得到总共要计算的次数
long TExchOptimize::GetCaclUionCount()
{
	long lRet = 0;

	if( IsValid() == FALSE || GetValidItemCount() <= 0 )
		return lRet;

	lRet = 1;
	for( int i = 0; i < lValidLen; i++ )
	{
		if( pIsValid[i] == 'y' )
		{
			lRet *= pItem[i].GetNum();
		}
	}

	return lRet;
}

int TExchOptimize::ValidIdxToGlobalIdx(long nIdx)
{
	if( nIdx < 0 || nIdx >= GetValidItemCount() )
		return -1;

	int nValid = -1;
	for( int i = 0; i < lValidLen; i++ )
	{
		if( pIsValid[i] == 'y' )
		{
			nValid++;
		}
		if( nValid == nIdx )
			return i;
	}
	return -1;
}

TExchOptimizeItem* TExchOptimize::GetValidItem(long nIdx)
{
	nIdx = ValidIdxToGlobalIdx(nIdx);
	if( nIdx == -1 )
		return NULL;

	return &pItem[nIdx];
}

// 得到第N个能数组和
BOOL TExchOptimize::GetCaclUionItem(long nIdx, TExchParam& sRet)
{
	if( IsValid() == FALSE || nIdx < 0 || nIdx >= GetCaclUionCount() || sRet.IsValid() == FALSE )
	{
		return FALSE;
	}

	TExchOptimizeItem* pItemTemp;
	int nValidParam = GetValidItemCount();
	long lTemp = nIdx;
	long lCurNum;

	long lMode;

	for( int i = nValidParam-1; i >= 0, lTemp > 0; i-- )
	{
		pItemTemp = GetValidItem(i);

		lCurNum = pItemTemp->GetNum();
		lMode = lTemp % lCurNum;

		if( lMode < lCurNum )
		{
			sRet.m_pParam[i] = pItemTemp->GetParam(lMode);
		}
		lTemp = lTemp / lCurNum;
	}

	return TRUE;
}

// TCaclOneStockResult::
TCaclOneStockResult::TCaclOneStockResult()
{
	ZeroMemory(this, sizeof(TCaclOneStockResult));
}

TCaclOneStockResult& TCaclOneStockResult::operator=(TCaclOneStockResult& sData)
{
	memcpy(this, &sData, sizeof(TCaclOneStockResult));
	return *this;
}

TCaclOneStockResult& TCaclOneStockResult::operator+=(TCaclOneStockResult& sData)
{
	m_lStockNum ++;

	m_dNetProfit						+=	sData.m_dNetProfit;					// 净利润
	m_dNPPercent						+=	sData.m_dNPPercent;					// 净利润率NP=Net Profi
	m_dTotalProfit						+=	sData.m_dTotalProfit;					// 总盈利
	m_dTotalDificit						+=	sData.m_dTotalDificit;				// 总亏损
	m_lDealNum							+=	sData.m_lDealNum;						// 交易次数
	m_dWinPercent 						+=	sData.m_dWinPercent*sData.m_lDealNum;	// 胜率
	m_lYearDealNum 						+=	sData.m_lYearDealNum;					// 年均交易次数
	m_lProfitDealNum 					+=	sData.m_lProfitDealNum;				// 盈利交易次数
	m_lDificitDealNum 					+=	sData.m_lDificitDealNum;				// 亏损交易次数
	m_dMaxSingleProfit 					=	max(m_dMaxSingleProfit, sData.m_dMaxSingleProfit);				// 最大单次盈利
	m_dMaxSingleDificit 				=	min(m_dMaxSingleDificit, sData.m_dMaxSingleDificit);			// 最大单次亏损

	m_lMaxContinuProfitNum 				=	max(m_lMaxContinuProfitNum, sData.m_lMaxContinuProfitNum);			// 最大连续盈利次数
	m_lMaxContinuDificitNum 			=	max(m_lMaxContinuDificitNum, sData.m_lMaxContinuDificitNum);		// 最大连续亏损次数 
	m_dDealAverageCyc 					+=	sData.m_dDealAverageCyc;				// 交易平均周期数
	m_dProfiltAverageCyc 				+=	sData.m_dProfiltAverageCyc;			// 盈利平均交易周期
	m_dDiticitAverageCyc 				+=	sData.m_dDiticitAverageCyc;			// 亏损平均交易周期

	m_dMaxFloatProfit 					=	max(m_dMaxFloatProfit, sData.m_dMaxFloatProfit);				// 最大浮动盈利
	m_dMaxFloatDificit 					=	min(m_dMaxFloatDificit, sData.m_dMaxFloatDificit);				// 最大浮动亏损

	m_dAllJumpIn 						+=	sData.m_dAllJumpIn;					// 总投入
	m_dValidJumpIn 						+=	sData.m_dValidJumpIn;					// 有效投入
	m_dYearReturnPercent 				+=	sData.m_dYearReturnPercent;			// 年回报率
	m_dYearValidReturnPercent 			+=	sData.m_dYearValidReturnPercent;		// 年有效回报率
	m_dSimpleBuyHaveReturnPercent 		+=	sData.m_dSimpleBuyHaveReturnPercent;	// 简单买入持有回报
	m_dBuyHaveYearPercent 				+=	sData.m_dBuyHaveYearPercent;			// 买入持有年回报率
	m_dAllDealAmount 					+=	sData.m_dAllDealAmount;				// 总交易额
	m_dAllDealCost 						+=	sData.m_dAllDealCost;					// 总交易费

	if( m_lBeginDate == 0 )
	{
		m_lBeginDate 						=	sData.m_lBeginDate;
		m_lEndDate 							=	sData.m_lEndDate;
	}
	else 
	{
		m_lBeginDate						=	min(m_lBeginDate, sData.m_lBeginDate);
		m_lEndDate 							=	max(m_lEndDate, sData.m_lEndDate);						// 测试时间
	}
	m_lTestCycNum 						+=	sData.m_lTestCycNum;					// 测试周期数
	m_dAverageCW 						+=	sData.m_dAverageCW;					// 平均仓位
	m_dMaxCW 							=	max(sData.m_dMaxCW, m_dMaxCW);						// 最大仓位
	m_dAverageCCL 						+=	sData.m_dAverageCCL;					// 平均持仓量
	m_dMaxCCL 							=	max(sData.m_dMaxCCL, m_dMaxCCL);						// 最大持仓量
	m_dSucessPercent 					+=	sData.m_dSucessPercent;				// 成功率
	m_dSignalNum 						+=	sData.m_dSignalNum;					// 信号数量
	m_d5DayProfitProbability 			+=	sData.m_d5DayProfitProbability;		// 五日获利概率
	m_d10DayProfitProbability 			+=	sData.m_d10DayProfitProbability;		// 十获利概率
	m_d20DayProfitProbability 			+=	sData.m_d20DayProfitProbability;		// 二十获利概率
	m_dAimCycProfitProbability 			+=	sData.m_dAimCycProfitProbability;		// 目标周期获利概率

	return *this;
}

void TCaclOneStockResult::ToListCtrl(CListCtrl *pCtrl)
{
	LV_ITEM			lvitem;
	char			chTemp[256];
	char			*cTest = (char*)&m_lStockNum;


	lvitem.mask = LVIF_TEXT|LVIF_IMAGE;
	lvitem.iItem = pCtrl->GetItemCount();
	lvitem.iImage = (int)(m_dNetProfit >=  0);
	lvitem.pszText = chTemp;

	memset(chTemp, 0, sizeof(chTemp));
	if( *(cTest+sizeof(long)) == 0 )
	{
		sprintf(chTemp, _T("全市场"));
	}
	else
	{
		strncpy(chTemp, m_sCode.m_cCode, sizeof(m_sCode.m_cCode));
	}
	lvitem.iSubItem = 0;
	pCtrl->InsertItem(&lvitem);//返回表项插入后的索引号

	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 1;
	sprintf(chTemp, "%.2f%%", m_dNPPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 2;
	sprintf(chTemp, "%.2f%%", m_dYearReturnPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 3;
	sprintf(chTemp, "%.2f%%", m_dWinPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 4;
	sprintf(chTemp, "%d次", m_lDealNum);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 5;
	sprintf(chTemp, "%.2f%%", m_dSucessPercent);
	pCtrl->SetItem(&lvitem);
}

void TCaclOneStockResult::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_llData;
		ar << m_dNetProfit;		
		ar << m_dNPPercent;		
		ar << m_dTotalProfit;		
		ar << m_dTotalDificit;	
		ar << m_lDealNum;			
		ar << m_dWinPercent;		
		ar << m_lYearDealNum;		
		ar << m_lProfitDealNum;	
		ar << m_lDificitDealNum;	
		ar << m_dMaxSingleProfit;	
		ar << m_dMaxSingleDificit;
		ar << m_lMaxContinuProfitNum;	
		ar << m_lMaxContinuDificitNum;
		ar << m_dDealAverageCyc;		
		ar << m_dProfiltAverageCyc;	
		ar << m_dDiticitAverageCyc;	
		ar << m_dMaxFloatProfit;	
		ar << m_dMaxFloatDificit;
		ar << m_dAllJumpIn;					
		ar << m_dValidJumpIn;					
		ar << m_dYearReturnPercent;			
		ar << m_dYearValidReturnPercent;		
		ar << m_dSimpleBuyHaveReturnPercent;	
		ar << m_dBuyHaveYearPercent;			
		ar << m_dAllDealAmount;				
		ar << m_dAllDealCost;					
		ar << m_lBeginDate;
		ar << m_lEndDate;						
		ar << m_lTestCycNum;					
		ar << m_dAverageCW;					
		ar << m_dMaxCW;						
		ar << m_dAverageCCL;					
		ar << m_dMaxCCL;						
		ar << m_dSucessPercent;				
		ar << m_dSignalNum;					
		ar << m_d5DayProfitProbability;		
		ar << m_d10DayProfitProbability;		
		ar << m_d20DayProfitProbability;		
		ar << m_dAimCycProfitProbability;		
	}
	else
	{
		ar >> m_llData;
		ar >> m_dNetProfit;		
		ar >> m_dNPPercent;		
		ar >> m_dTotalProfit;		
		ar >> m_dTotalDificit;	
		ar >> m_lDealNum;			
		ar >> m_dWinPercent;		
		ar >> m_lYearDealNum;		
		ar >> m_lProfitDealNum;	
		ar >> m_lDificitDealNum;	
		ar >> m_dMaxSingleProfit;	
		ar >> m_dMaxSingleDificit;
		ar >> m_lMaxContinuProfitNum;	
		ar >> m_lMaxContinuDificitNum;
		ar >> m_dDealAverageCyc;		
		ar >> m_dProfiltAverageCyc;	
		ar >> m_dDiticitAverageCyc;	
		ar >> m_dMaxFloatProfit;	
		ar >> m_dMaxFloatDificit;
		ar >> m_dAllJumpIn;					
		ar >> m_dValidJumpIn;				
		ar >> m_dYearReturnPercent;			
		ar >> m_dYearValidReturnPercent;	
		ar >> m_dSimpleBuyHaveReturnPercent;
		ar >> m_dBuyHaveYearPercent;		
		ar >> m_dAllDealAmount;				
		ar >> m_dAllDealCost;				
		ar >> m_lBeginDate;
		ar >> m_lEndDate;					
		ar >> m_lTestCycNum;				
		ar >> m_dAverageCW;					
		ar >> m_dMaxCW;						
		ar >> m_dAverageCCL;				
		ar >> m_dMaxCCL;					
		ar >> m_dSucessPercent;				
		ar >> m_dSignalNum;					
		ar >> m_d5DayProfitProbability;		
		ar >> m_d10DayProfitProbability;	
		ar >> m_d20DayProfitProbability;	
		ar >> m_dAimCycProfitProbability;	
	}
}

// TDealFlowItem::
TDealFlowItem::TDealFlowItem()
{
	ZeroMemory(this, sizeof(TDealFlowItem));
}

void TDealFlowItem::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_wDealType;
		ar << m_lDealDate;
		ar << m_dDealPrice;
		ar << m_lDealAmount;
		ar << m_dDealProfit;
		ar << m_lCaclDays;
	}
	else
	{
		ar >> m_wDealType;
		ar >> m_lDealDate;
		ar >> m_dDealPrice;
		ar >> m_lDealAmount;
		ar >> m_dDealProfit;
		ar >> m_lCaclDays;
	}
}

TDealFlowItem& TDealFlowItem::operator=(TDealFlowItem& sData)
{
	memcpy(this, &sData, sizeof(TDealFlowItem));
	return *this;
}

TDealFlowItem& TDealFlowItem::operator++()
{
	m_lCaclDays++;
	return *this;
}

CString TDealFlowItem::GetDealType()
{
	CString strRet;

	switch( m_wDealType )
	{
	case TDealFlow_DT_PingChang	:
		strRet = _T(TDealFlow_DT_PingChang_Code);
		break;
	case TDealFlow_DT_Buy		:
		strRet = _T(TDealFlow_DT_Buy_Code);
		break;
	case TDealFlow_DT_Sell		:
		strRet = _T(TDealFlow_DT_Sell_Code);
		break;
	case TDealFlow_DT_ZhiShun	:
		strRet = _T(TDealFlow_DT_ZhiShun_Code);
		break;
	case TDealFlow_DT_ZhiYing	:
		strRet = _T(TDealFlow_DT_ZhiYing_Code);
		break;
	case TDealFlow_DT_HuiLuo:
		strRet = _T(TDealFlow_DT_HuiLuo_Code);
		break;
	case TDealFlow_DT_HenPang:
		strRet = _T(TDealFlow_DT_HenPang_Code);
		break;
	case TDealFlow_DT_QZPC:
		strRet = _T(TDealFlow_DT_QZPC_Code);
		break;
	default:
		strRet = _T(TDealFlow_DT_No_Code);
		break;
	}
	return strRet;
}

void TDealFlowItem::ToListCtrl(CListCtrl* pCtrl, CString& strName)
{
	if( pCtrl == NULL )
		return;

	LV_ITEM			lvitem;
	char			chTemp[256];

	lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	lvitem.iItem = pCtrl->GetItemCount();
	lvitem.iImage = (int)(m_wDealType == TDealFlow_DT_Buy);
	lvitem.pszText = chTemp;
	lvitem.lParam = (LPARAM)m_lDealDate;
	sprintf(chTemp, "%d", m_lDealDate);
	lvitem.iSubItem = 0;
	pCtrl->InsertItem(&lvitem);//返回表项插入后的索引号

	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 1;
	sprintf(chTemp, "%s", strName);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 2;
	sprintf(chTemp, "%s", GetDealType());
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 3;
	sprintf(chTemp, "%.2f", m_dDealPrice);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 4;
	sprintf(chTemp, "%d", m_lDealAmount);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 5;
	if( m_wDealType == TDealFlow_DT_Buy )
		sprintf(chTemp, "");
	else
		sprintf(chTemp, "%.2f", m_dDealProfit);
	pCtrl->SetItem(&lvitem);
}

// TDealFlowStock::
TDealFlowStock::TDealFlowStock() 
{
	Clear();
}

TDealFlowStock::TDealFlowStock(TDealFlowStock* pData)
{
	if( pData == NULL )
	{
		Clear();
	}
	else
	{
		*this = *pData;
	}
}

TDealFlowStock::TDealFlowStock(CodeInfo& sCode, CString& strName, double dLastMoney)
{
	Clear();
	memcpy(&m_sCode, &sCode, sizeof(m_sCode));
	m_strName.Format("%s", strName);
	m_lLastMoney = dLastMoney;
}

TDealFlowStock::~TDealFlowStock()
{
	Free();
}

TCaclOneStockResult* TDealFlowStock::TotalTo(TCaclOneStockResult* pItem, BOOL bSingle)
{
	if( pItem == NULL )
	{
		pItem = new TCaclOneStockResult;
	}
	if( pItem != NULL )
	{
		TDealFlowItem* pFlow = NULL;
		long lEnd = GetCount();
		// 当前周期
		long lMaxSeriesProfit = 0, lMaxSeriesDificit = 0;
		long lPreState = 0; // 0不定, 1 盈利, -1 亏损
		double dMaxValidJumpIn = 0;
		double dDealAmount;
		double dTotalBuy = 0, dTotalSell = 0;
		double dPreBuyAmount = 0;

		// 所有成交统计
		for( int i = 0; i < lEnd; i++ )
		{
			pFlow = GetItem(i);
			if( pFlow == NULL )
			{
				continue;
			}

			// 单笔成交金额
			dDealAmount = pFlow->m_dDealPrice * pFlow->m_lDealAmount;
			// 总交易额
			pItem->m_dAllDealAmount += dDealAmount;
			// 总交易费
			pItem->m_dAllDealCost += (dDealAmount * EXCH_POUNDAGE);

			// 买入
			if( pFlow->m_wDealType == TDealFlow_DT_Buy )
			{
				dMaxValidJumpIn += dDealAmount;
				// 买入所有的金额
				dTotalBuy += dDealAmount;
				dPreBuyAmount += dDealAmount;
				continue;
			}
			// 是强制平仓
			else if( pFlow->m_wDealType == TDealFlow_DT_QZPC )
			{
				if( !TDealFlowStock::m_bLastSell )
				{
					// 买入所有的金额	
					dTotalBuy -= dPreBuyAmount;
					dPreBuyAmount = 0;
					continue;
				}
				// 卖出所有的金额
				dTotalSell += dDealAmount;
				dPreBuyAmount = 0;
			}
			else
			{
				// 卖出所有的金额
				dTotalSell += dDealAmount;
				dPreBuyAmount = 0;
			}

			// 卖出
			// 最大有效投入
			pItem->m_dValidJumpIn =	max(pItem->m_dValidJumpIn, dMaxValidJumpIn);
			dMaxValidJumpIn = 0;
			// 平均仓位--计算过程
			pItem->m_dAverageCW += dMaxValidJumpIn;
			// 平均持仓量--计算过程
			pItem->m_dAverageCCL += pFlow->m_lDealAmount;
			// 最大持仓量
			pItem->m_dMaxCCL = max(pFlow->m_lDealAmount, pItem->m_dMaxCCL);

			if( pFlow->m_dDealProfit > 0 )
			{
				// 总盈利
				pItem->m_dTotalProfit += pFlow->m_dDealProfit;					
				// 盈利交易次数
				pItem->m_lProfitDealNum += 1;					
				// 最大单次盈利
				pItem->m_dMaxSingleProfit =	max(pItem->m_dMaxSingleProfit, pFlow->m_dDealProfit);	
				pItem->m_dProfiltAverageCyc	+= pFlow->m_lCaclDays;					
				if( lPreState == -1 )
				{
					lMaxSeriesProfit = 1;
					lPreState = 1;
					// 最大连续亏损次数 
					pItem->m_lMaxContinuDificitNum = max(lMaxSeriesDificit, pItem->m_lMaxContinuDificitNum);					
				}
				else if( lPreState == 1 )
				{
					lMaxSeriesProfit++;
				}
				else
				{	
					lMaxSeriesProfit = 1;
					lPreState = 1;
				}
			}
			else
			{
				// 总亏损
				pItem->m_dTotalDificit += pFlow->m_dDealProfit;					
				// 亏损交易次数
				pItem->m_lDificitDealNum +=	1;					
				// 最大单次亏损
				pItem->m_dMaxSingleDificit = min(pItem->m_dMaxSingleDificit, pFlow->m_dDealProfit);	
				pItem->m_dDiticitAverageCyc += pFlow->m_lCaclDays;	
				if( lPreState == 1 )
				{
					lMaxSeriesDificit = 1;
					lPreState = -1;
					// 最大连续盈利次数 
					pItem->m_lMaxContinuProfitNum = max(lMaxSeriesDificit, pItem->m_lMaxContinuProfitNum);					
				}
				else if( lPreState == -1 )
				{
					lMaxSeriesDificit++;
				}
				else
				{	
					lMaxSeriesDificit = 1;
					lPreState = -1;
				}					
			}
			// 交易次数
			pItem->m_lDealNum++	;				
			pItem->m_dDealAverageCyc +=	pFlow->m_lCaclDays;					
		}

		if( lPreState == 1 )
		{
			// 最大连续盈利次数 
			pItem->m_lMaxContinuProfitNum = max(lMaxSeriesProfit, pItem->m_lMaxContinuProfitNum);					
		}
		else if( lPreState == -1 )
		{
			// 最大连续亏损次数 
			pItem->m_lMaxContinuDificitNum = max(lMaxSeriesDificit, pItem->m_lMaxContinuDificitNum);					
		}

		// 平均仓位--计算过程
		pItem->m_dAverageCW = dTotalBuy / (pItem->m_lDealNum == 0 ? 1 : pItem->m_lDealNum) 
							  * 100 / (TDealFlowStock::m_dAllMoney != 0 ? TDealFlowStock::m_dAllMoney : 1);
		// 最大仓位--计算过程
		pItem->m_dMaxCW = pItem->m_dValidJumpIn * 100 / (TDealFlowStock::m_dAllMoney != 0 ? TDealFlowStock::m_dAllMoney : 1);
		// 平均持仓量--计算过程
		pItem->m_dAverageCCL /= (pItem->m_lDealNum == 0 ? 1 : pItem->m_lDealNum);
		// 最大有效投入
		pItem->m_dValidJumpIn *= (1+EXCH_POUNDAGE);
		// 时间
		pItem->m_lTestCycNum = m_lTestCycNum;
		pItem->m_lBeginDate = TExchBuy::m_lBeginDate;
		pItem->m_lEndDate = TExchBuy::m_lEndDate;
		// 代码
		pItem->m_sCode = m_sCode;
		// 净利润
		pItem->m_dNetProfit	= pItem->m_dTotalProfit+pItem->m_dTotalDificit;//(dTotalSell - dTotalBuy);				
		// 净利润率NP=Net Profi
		pItem->m_dNPPercent	= pItem->m_dNetProfit * 100/ (pItem->m_dValidJumpIn != 0 ? pItem->m_dValidJumpIn : 1);
		// 计算年数 一年的交易日约为250天
		float lYears = m_lTestCycNum *1.0 / 250;
		if( fabs(lYears) < 0.001 )
		{
			lYears = 1;
		}
		//long lYears = TExchBuy::m_lEndDate / 10000 - TExchBuy::m_lBeginDate / 10000;
		// 盈利平均交易周期
		pItem->m_dProfiltAverageCyc	/= (pItem->m_lProfitDealNum != 0 ? pItem->m_lProfitDealNum : 1);
		// 亏损平均交易周期
		pItem->m_dDiticitAverageCyc	/= (pItem->m_lDificitDealNum != 0 ? pItem->m_lDificitDealNum : 1 );
		// 交易平均周期数
		pItem->m_dDealAverageCyc /= (pItem->m_lDealNum != 0 ? pItem->m_lDealNum : 1);
		// 年均交易次数
		pItem->m_lYearDealNum =	pItem->m_lDealNum * 1.0/ lYears;					
		// 胜率
		pItem->m_dWinPercent = pItem->m_lProfitDealNum * 100.0 / (pItem->m_lDealNum != 0 ? pItem->m_lDealNum : 1);					
		// 总投入
		pItem->m_dAllJumpIn = TDealFlowStock::m_dAllMoney;	
		// 年回报率
		pItem->m_dYearReturnPercent = pItem->m_dNPPercent / lYears;					
		// 年有效回报率
		double dTemp1 = pItem->m_dValidJumpIn != 0 ? pItem->m_dValidJumpIn : 1;
		pItem->m_dYearValidReturnPercent = (m_lLastMoney-m_dAllMoney)*100 / (dTemp1*lYears);					
		// 简单买入持有回报
		pItem->m_dSimpleBuyHaveReturnPercent =	(m_dEndPrice - (m_dBeginPrice != 0 ? m_dBeginPrice : 1)) * 100 / (m_dBeginPrice != 0 ? m_dBeginPrice : 1);					
		// 买入持有年回报率
		pItem->m_dBuyHaveYearPercent = pItem->m_dSimpleBuyHaveReturnPercent / lYears;
		// 最大浮动盈利
		pItem->m_dMaxFloatProfit = m_dMaxFDProfit;					
		// 最大浮动亏损
		pItem->m_dMaxFloatDificit =	m_dMaxFDDificit;	
		// 信号数
		pItem->m_dSignalNum = m_lBuySignalNum;
		
		// 成功率
		pItem->m_dSucessPercent = 100.0 * m_lWinDays/ (m_lBuySignalNum == 0 ? 1 :m_lBuySignalNum);
		pItem->m_d5DayProfitProbability = 100.0 * m_lWin5Day/ (m_lBuySignalNum == 0 ? 1 :m_lBuySignalNum);
		pItem->m_d10DayProfitProbability = 100.0 * m_lWin10Day/ (m_lBuySignalNum == 0 ? 1 :m_lBuySignalNum);
		pItem->m_d20DayProfitProbability = 100.0 * m_lWin20Day/ (m_lBuySignalNum == 0 ? 1 :m_lBuySignalNum);
		pItem->m_dAimCycProfitProbability = 100.0 * m_lWinTabCyc/ (m_lBuySignalNum == 0 ? 1 :m_lBuySignalNum);
	}
	return pItem;
}

long TDealFlowStock::GetCount()
{
	return m_ayItem.GetSize();
}

void TDealFlowStock::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_lLastBuy;		
		ar << m_dAllMoney;	
		ar << m_bLastSell;	
		ar << s_dLastMonery;
		CString str(m_sCode.m_cCode, sizeof(m_sCode.m_cCode));
		ar << str;
		ar << m_sCode.m_cCodeType;		
		ar << m_strName;		
		ar << m_lLastStockNum;
		ar << m_lLastMoney;	
		ar << m_dBeginPrice;
		ar << m_dEndPrice;	
		ar << m_lTestCycNum;
		ar << m_lBuySignalNum;
		ar << m_lWinDays;		
		ar << m_lWin5Day;		
		ar << m_lWin10Day;	
		ar << m_lWin20Day;	
		ar << m_lWinTabCyc;	
		ar << m_dJumpInMoney; 
		ar << m_lSignalNum;	
		ar << m_dMaxFDProfit; 
		ar << m_dMaxFDDificit;

		ar << m_ayItem.GetSize();

		for( int i = 0; i < m_ayItem.GetSize(); i++ )
		{
			TDealFlowItem*pItem = m_ayItem.GetAt(i);
			ASSERT(pItem != NULL);
			if( pItem != NULL )
			{
				pItem->Serialize(ar);
			}
		}
	}
	else
	{
		Clear();

		ar >> m_lLastBuy;		
		ar >> m_dAllMoney;	
		ar >> m_bLastSell;	
		ar >> s_dLastMonery;
		CString str;
		ar >> str;
		sprintf(m_sCode.m_cCode, "%s", str);
		ar >> m_sCode.m_cCodeType;		
		ar >> m_strName;		
		ar >> m_lLastStockNum;
		ar >> m_lLastMoney;	
		ar >> m_dBeginPrice;
		ar >> m_dEndPrice;	
		ar >> m_lTestCycNum;
		ar >> m_lBuySignalNum;
		ar >> m_lWinDays;		
		ar >> m_lWin5Day;		
		ar >> m_lWin10Day;	
		ar >> m_lWin20Day;	
		ar >> m_lWinTabCyc;	
		ar >> m_dJumpInMoney; 
		ar >> m_lSignalNum;	
		ar >> m_dMaxFDProfit; 
		ar >> m_dMaxFDDificit;
		int iCount;
		ar >> iCount;
		TDealFlowItem* pItem;
		for(int i = 0; i < iCount; i++ )
		{
			pItem = new TDealFlowItem;
			ASSERT(pItem != NULL );
			if( pItem != NULL )
			{
				pItem->Serialize(ar);
				m_ayItem.Add(pItem);
			}
		}
	}
}

TDealFlowItem* TDealFlowStock::Append(TDealFlowItem* pItem)
{
	if( pItem == NULL )
		pItem = new TDealFlowItem;
	if( pItem != NULL )
		m_ayItem.Add(pItem);
	return pItem;
}

TDealFlowItem* TDealFlowStock::GetLastItem()
{
	if( m_ayItem.GetSize() <= 0 )
		return NULL;
	return m_ayItem.GetAt(m_ayItem.GetSize() -1 );
}
TDealFlowItem* TDealFlowStock::GetItem(int iIdx)
{
	if( iIdx >= 0 && iIdx < GetCount() )
		return m_ayItem.GetAt(iIdx);

	return NULL;
}

double TDealFlowStock::GetLastPrice()
{
	if( m_ayItem.GetSize() <= 0 )
		return -1;

	TDealFlowItem* pItem = m_ayItem.GetAt(m_ayItem.GetSize() -1 );
	if( pItem == NULL )
		return -1;
	return pItem->m_dDealPrice;
}

LONG TDealFlowStock::GetLastCaclDays()
{
	if( m_ayItem.GetSize() <= 0 )
		return -1;
	TDealFlowItem* pItem = m_ayItem.GetAt(m_ayItem.GetSize() -1 );
	if( pItem == NULL || pItem->m_wDealType != TDealFlow_DT_Buy )
		return -1;
	return pItem->m_lCaclDays;
}

BOOL TDealFlowStock::IsHaveMoney()
{
	return (m_lLastMoney > 0 );
}

BOOL TDealFlowStock::IsHaveStock()
{
	return (m_lLastStockNum > 0);
}

void TDealFlowStock::Clear()
{
	ZeroMemory(&m_sCode, sizeof(CodeInfo));		// 代码
	m_strName.Empty();		// 名称
	m_lLastStockNum = 0; // 最后一笔交易后所剩下的股票
	m_lLastMoney = TDealFlowStock::m_dAllMoney;	// 最后一笔交易后所剩下金钱

	m_dBeginPrice = 0;		// 开始时价格
	m_dEndPrice = 0;		// 结束时价格	
	m_lTestCycNum = 0;		// 测试周期
	m_lBuySignalNum = 0;	// 买入信号数
	m_lWin5Day = 0;			// 五日胜次
	m_lWin10Day = 0;		// 十日胜次
	m_lWin20Day = 0;		// 二十日胜次
	m_lWinTabCyc = 0;		// 目标周期胜次
	m_dJumpInMoney = 0; // 投入单次交易金额
	m_lSignalNum = 0;	// 信号数量
	m_dMaxFDProfit = 0; // 最大浮动盈利
	m_dMaxFDDificit = 0; // 最大浮动亏损
	m_lWinDays = 0;

	Free();
}

void TDealFlowStock::Free(void)
{
	for( int i = 0; i < m_ayItem.GetSize(); i++ )
		delete m_ayItem.GetAt(i);
	m_ayItem.RemoveAll();
}

TDealFlowStock& TDealFlowStock::operator=(TDealFlowStock& sData)
{
	memcpy(&m_sCode, &sData.m_sCode, sizeof(m_sCode));
	m_strName = sData.m_strName;
	m_lLastMoney = sData.m_lLastMoney;
	m_lLastStockNum = sData.m_lLastStockNum;
	m_dBeginPrice = sData.m_dBeginPrice;
	m_dEndPrice = sData.m_dEndPrice;

	m_lTestCycNum	= sData.m_lTestCycNum;			// 测试周期
	m_lBuySignalNum = sData.m_lBuySignalNum ;		// 买入信号数
	m_lWin5Day		= sData.m_lWin5Day		;		// 五日胜次
	m_lWin10Day		= sData.m_lWin10Day		;		// 十日胜次
	m_lWin20Day		= sData.m_lWin20Day		;		// 二十日胜次
	m_lWinTabCyc	= sData.m_lWinTabCyc	;		// 目标周期胜次

	m_dJumpInMoney	= sData.m_dJumpInMoney	;		// 投入单次交易金额
	m_lSignalNum	= sData.m_lSignalNum	;		// 信号数量
	m_dMaxFDProfit	= sData.m_dMaxFDProfit	;		// 最大浮动盈利
	m_dMaxFDDificit = sData.m_dMaxFDDificit ;		// 最大浮动亏损

	TDealFlowItem *pItemDesc;
	TDealFlowItem *pItemSour;

	Free();

	for( int i = 0; i < sData.m_ayItem.GetSize(); i++ )
	{
		pItemSour = sData.m_ayItem.GetAt(i);
		if( pItemSour == NULL )
			continue;
		pItemDesc = new TDealFlowItem;
		if( pItemDesc == NULL )
			continue;
		*pItemDesc = *pItemSour;
		m_ayItem.Add(pItemDesc);
	}
	return *this;
}

void TDealFlowStock::ToListCtrl(CListCtrl *pCtrl)
{
	int iEnd = GetCount();
	for( int i = 0; i < iEnd; i++ )
	{
		TDealFlowItem* pItem = GetItem(i);
		if( pItem != NULL )
		{
			pItem->ToListCtrl(pCtrl, m_strName);
		}
	}
}

// TDealFlow::
TDealFlow::~TDealFlow()
{
	Free();
}

void TDealFlow::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		ar << m_ayData.GetSize();

		for( int i = 0; i < m_ayData.GetSize(); i++ )
		{
			TDealFlowStock* pItem = m_ayData.GetAt(i);
			ASSERT(pItem != NULL );
			if( pItem != NULL )
				pItem->Serialize(ar);
		}
	}
	else
	{
		Free();

		int iCount;
		ar >> iCount;

		for( int i = 0; i < iCount; i++ )
		{
			TDealFlowStock* pItem = new TDealFlowStock;
			ASSERT(pItem != NULL );
			if( pItem != NULL )
			{
				pItem->Serialize(ar);
				m_ayData.Add(pItem);
			}
		}
	}
}

TDealFlowStock* TDealFlow::GetLastFlow()
{
	return m_ayData.GetAt(m_ayData.GetSize()-1);
}

TDealFlowStock* TDealFlow::GetItem(int iIdx)
{
	if( iIdx >= 0 && iIdx < GetCount() )
		return m_ayData.GetAt(iIdx);
	return NULL;
}

TDealFlowStock* TDealFlow::GetFlowFormCode(CodeInfo* pCode)
{
	if( pCode == NULL )
		return NULL;
	//CString strCode(pCode->m_cCode, sizeof(pCode->m_cCode));
	int iCount = GetCount();
	for( int i = 0; i < iCount; i++ )
	{
		TDealFlowStock* pItem = m_ayData.GetAt(i);
		//CString strCodeTemp(pItem->m_sCode.m_cCode, sizeof(pItem->m_sCode.m_cCode));
		//if( strCode.CompareNoCase(strCodeTemp) == 0 )
		if( strncmp(pCode->m_cCode, pItem->m_sCode.m_cCode, sizeof(pItem->m_sCode.m_cCode)) == 0 )
			return pItem;
	}

	return NULL;
}
void TDealFlow::AddStockItem(TDealFlowStock* pItem)
{
	m_ayData.Add(pItem);
}

int TDealFlow::GetCount()
{
	return m_ayData.GetSize();
}

void TDealFlow::Free(void)
{
	for( int i = 0; i < m_ayData.GetSize(); i++ )
	{
		delete m_ayData.GetAt(i);
	}
	m_ayData.RemoveAll();
}

TDealFlow& TDealFlow::operator=(TDealFlow& sData)
{
	TDealFlowStock *pItemDesc;
	TDealFlowStock *pItemSour;

	Free();
	for( int i = 0; i < sData.m_ayData.GetSize(); i++ )
	{
		pItemSour = sData.m_ayData.GetAt(i);
		if( pItemSour == NULL )
			continue;
		pItemDesc = new TDealFlowStock;
		if( pItemDesc == NULL )
			continue;
		*pItemDesc = *pItemSour;
		m_ayData.Add(pItemDesc);
	}
	return *this;
}

void TDealFlow::ToListCtrl(CListCtrl *pCtrl, int nIdx)
{
	if( pCtrl == NULL )
		return;

	pCtrl->DeleteAllItems();
	int iCount = GetCount();
	TDealFlowStock* pStock;

	if( nIdx < 0 || nIdx >= iCount )
	{
		for( int i = 0; i < iCount; i++ )
		{
			pStock = GetItem(i);
			if( pStock != NULL )
				pStock->ToListCtrl(pCtrl);
		}
		pCtrl->SortItems((PFNLVCOMPARE)SortFunc, (DWORD)pCtrl);
	}
	else
	{
		pStock = GetItem(nIdx);
		if( pStock != NULL )
			pStock->ToListCtrl(pCtrl);
	}
	pCtrl->SetSelectionMark(0);
}

void TDealFlow::Initital(CListCtrl* pCtrl, CImageList *pImage)
{
	if( pCtrl == NULL || pImage == NULL )
		return;

	pCtrl->SetImageList(pImage, LVSIL_SMALL);
	pCtrl->SetExtendedStyle(pCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	int i = 0;
	pCtrl->DeleteAllItems();
	while( pCtrl->DeleteColumn(0) && i++ < 300 );

	TCHAR _tagColName[][10] = {HS_EXCH_DEALFLOW_HANDLE_TIME	,
		HS_EXCH_DEALFLOW_HANDLE_STK	,
		HS_EXCH_DEALFLOW_HANDLE_TYPE	,
		HS_EXCH_DEALFLOW_HANDLE_PRICE	,
		HS_EXCH_DEALFLOW_HANDLE_AMOUNT	,
		HS_EXCH_DEALFLOW_HANDLE_PROFIT	
	};
	int _tabColWidth[] = { 80, 80, 70, 70, 70, 80};

	LV_COLUMN	lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中

	for(i = 0; i < _countof(_tagColName); i ++)
	{
		lvcol.pszText = _tagColName[i];
		lvcol.iSubItem = i;
		lvcol.cx = _tabColWidth[i];
		pCtrl->InsertColumn(i,&lvcol);
		lvcol.fmt=LVCFMT_RIGHT;
	}
}

// TCaclResult::
TCaclResult::TCaclResult()
{
	ZeroMemory(&m_sTotal, sizeof(m_sTotal));
}

TCaclResult::~TCaclResult()
{
	Free();
}

double TCaclResult::GetIDData(int nID)
{
	switch( nID )
	{
	default:
	case HS_EXCH_CACLRESULT_STOCK_ID_PROFIT:	
		return m_sTotal.m_dNPPercent;
	case HS_EXCH_CACLRESULT_STOCK_ID_YEAR	:
		return m_sTotal.m_dYearReturnPercent;
	case HS_EXCH_CACLRESULT_STOCK_ID_WIN	:	
		return m_sTotal.m_dWinPercent;
	case HS_EXCH_CACLRESULT_STOCK_ID_DEALNUM:	
		return m_sTotal.m_lDealNum;
	case HS_EXCH_CACLRESULT_STOCK_ID_SUCC	:	
		return m_sTotal.m_dSucessPercent;
	}

	return 0;
}

void TCaclResult::Add(TCaclOneStockResult* pResult)
{
	if( pResult != NULL )
		m_ayAllStock.Add(pResult);
}

BOOL TCaclResult::Total(BOOL bSingle)
{
	TCaclOneStockResult* pItem;
	
	ZeroMemory(&m_sTotal, sizeof(m_sTotal));

	int iCount = m_ayAllStock.GetSize();
	for( int i = 0; i < iCount; i++ )
	{
		pItem = m_ayAllStock.GetAt(i);
		if( pItem == NULL )
			continue;
		m_sTotal += *pItem;
	}

	if( !bSingle )
		m_sTotal.m_dValidJumpIn /= (iCount == 0 ? 1 : iCount);

	if( m_sTotal.m_lStockNum == 0 )
		m_sTotal.m_lStockNum = 1;
	//m_sTotal.m_dNetProfit;					// 净利润
	m_sTotal.m_dNPPercent /= m_sTotal.m_lStockNum;	// 净利润率NP=Net Profit
	//m_sTotal.m_dTotalProfit;					// 总盈利
	//m_sTotal.m_dTotalDificit;				// 总亏损
	//m_sTotal.m_lDealNum;						// 交易次数
	m_sTotal.m_dWinPercent /= (m_sTotal.m_lDealNum == 0 ? 1 : m_sTotal.m_lDealNum); // 胜率
	//m_sTotal.m_lYearDealNum;					// 年均交易次数
	//m_sTotal.m_lProfitDealNum;				// 盈利交易次数
	//m_sTotal.m_lDificitDealNum;				// 亏损交易次数
	//m_sTotal.m_dMaxSingleProfit;				// 最大单次盈利
	//m_sTotal.m_dMaxSingleDificit;			// 最大单次亏损
	//m_sTotal.m_lMaxContinuProfitNum;			// 最大连续盈利次数
	//m_sTotal.m_lMaxContinuDificitNum;		// 最大连续亏损次数 
	m_sTotal.m_dDealAverageCyc /= m_sTotal.m_lStockNum;				// 交易平均周期数
	m_sTotal.m_dProfiltAverageCyc /= m_sTotal.m_lStockNum;			// 盈利平均交易周期
	m_sTotal.m_dDiticitAverageCyc /= m_sTotal.m_lStockNum;			// 亏损平均交易周期
	//m_sTotal.m_dMaxFloatProfit;				// 最大浮动盈利
	//m_sTotal.m_dMaxFloatDificit;				// 最大浮动亏损
	//m_sTotal.m_dAllJumpIn;					// 总投入
	//m_sTotal.m_dValidJumpIn;					// 有效投入
	m_sTotal.m_dYearReturnPercent /= m_sTotal.m_lStockNum;			// 年回报率
	m_sTotal.m_dYearValidReturnPercent /= m_sTotal.m_lStockNum;		// 年有效回报率
	m_sTotal.m_dSimpleBuyHaveReturnPercent /= m_sTotal.m_lStockNum;	// 简单买入持有回报
	m_sTotal.m_dBuyHaveYearPercent /= m_sTotal.m_lStockNum;			// 买入持有年回报率
	//m_sTotal.m_dAllDealAmount;				// 总交易额
	//m_sTotal.m_dAllDealCost;					// 总交易费
	//m_sTotal.m_lBeginDate;
	//m_sTotal.m_lEndDate;						// 测试时间
	m_sTotal.m_lTestCycNum /= m_sTotal.m_lStockNum;					// 测试周期数
	m_sTotal.m_dAverageCW /= m_sTotal.m_lStockNum;					// 平均仓位
	//m_sTotal.m_dMaxCW;						// 最大仓位
	m_sTotal.m_dAverageCCL /= m_sTotal.m_lStockNum;					// 平均持仓量
	//m_sTotal.m_dMaxCCL;						// 最大持仓量
	m_sTotal.m_dSucessPercent /= m_sTotal.m_lStockNum;				// 成功率
	//m_sTotal.m_dSignalNum;					// 信号数量
	m_sTotal.m_d5DayProfitProbability /= m_sTotal.m_lStockNum;		// 五日获利概率
	m_sTotal.m_d10DayProfitProbability /= m_sTotal.m_lStockNum;		// 十获利概率
	m_sTotal.m_d20DayProfitProbability /= m_sTotal.m_lStockNum;		// 二十获利概率
	m_sTotal.m_dAimCycProfitProbability /= m_sTotal.m_lStockNum;		// 目标周期获利概率

	return TRUE;
}

void TCaclResult::Free(void)
{
	for( int i = 0; i < m_ayAllStock.GetSize(); i++ )
	{
		delete m_ayAllStock.GetAt(i);
	}
	m_ayAllStock.RemoveAll();
}

void TCaclResult::Serialize(CArchive& ar)
{
	if( ar.IsStoring() )
	{
		m_sTotal.Serialize(ar);
		ar << m_ayAllStock.GetSize();
		for( int i = 0 ; i < m_ayAllStock.GetSize(); i++ )
		{
			TCaclOneStockResult* pItem = m_ayAllStock.GetAt(i);
			ASSERT(pItem != NULL);
			if( pItem != NULL )
				pItem->Serialize(ar);
		}
	}
	else
	{
		Free();

		m_sTotal.Serialize(ar);
		int iCount;
		ar >> iCount;
		for( int i = 0 ; i < iCount; i++ )
		{
			TCaclOneStockResult* pItem = new TCaclOneStockResult;
			ASSERT(pItem != NULL);
			if( pItem != NULL )
			{
				pItem->Serialize(ar);
				m_ayAllStock.Add(pItem);
			}
		}
	}
}

TCaclResult& TCaclResult::operator=(TCaclResult& sData)
{
	m_sTotal = sData.m_sTotal;
	TCaclOneStockResult *pItemDesc;
	TCaclOneStockResult *pItemSour;

	Free();
	for( int i = 0; i < sData.m_ayAllStock.GetSize(); i++ )
	{
		pItemSour = sData.m_ayAllStock.GetAt(i);
		if( pItemSour == NULL )
			continue;
		pItemDesc = new TCaclOneStockResult;
		if( pItemDesc == NULL )
			continue;
		*pItemDesc = *pItemSour;
		m_ayAllStock.Add(pItemDesc);
	}
	return *this;
}

void TCaclResult::ToListCtrl(CListCtrl* pCtrl)
{
	if( pCtrl == NULL )
		return;

	TCaclOneStockResult* pItem;

	pCtrl->DeleteAllItems();
	m_sTotal.ToListCtrl(pCtrl);
	for( int i = 0; i < m_ayAllStock.GetCount(); i++ )
	{
		pItem = m_ayAllStock.GetAt(i);
		if( pItem != NULL )
			pItem->ToListCtrl(pCtrl);
	}
	pCtrl->SetFocus();
	pCtrl->SetSelectionMark(0);
}

void TCaclResult::Initital(CListCtrl* pCtrl, CImageList *pImage)
{
	if( pCtrl == NULL )
		return;

	if( pImage != NULL )
		pCtrl->SetImageList(pImage, LVSIL_SMALL);

	pCtrl->SetExtendedStyle(pCtrl->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);

	int i = 0;
	pCtrl->DeleteAllItems();
	while( pCtrl->DeleteColumn(0) && i++ < 300 );

	TCHAR _tagColName[][10] = {HS_EXCH_CACLRESULT_STOCK_HEADLE_STKCODE,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_PROFIT	,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_YEAR	,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_WIN	,	
		HS_EXCH_CACLRESULT_STOCK_HEADLE_DEALNUM,
		HS_EXCH_CACLRESULT_STOCK_HEADLE_SUCC	
	};

	int _tabColWidth[] = { 80, 80, 80, 80, 80, 80 };

	LV_COLUMN	lvcol;
	lvcol.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcol.fmt=LVCFMT_CENTER;//居中
    CString str;
	for(i = 0; i < _countof(_tagColName); i ++)
	{
		str = _T(_tagColName[i]);
		lvcol.pszText = (char*)LPCTSTR(str);//;_tagColName[i];
		lvcol.iSubItem = i;
		lvcol.cx = _tabColWidth[i];
		pCtrl->InsertColumn(i,&lvcol);
		lvcol.fmt=LVCFMT_RIGHT;
	}
}

// TCaclResultTemp::
TCaclResultTemp::TCaclResultTemp(TCaclResultTemp *pData)
{
	if( pData == NULL )
	{
		m_lDaysLen = 0;
		m_lBufLen = 0;
		m_lDescLen = m_lRithtLen = m_lLeftLen = 0;
		m_lDescReqult = m_lRightReqult = m_lLeftReqult = NULL;
	}
	else
	{
		Copy(pData);
	}
}
TCaclResultTemp::~TCaclResultTemp()
{
	Del();
}

BOOL TCaclResultTemp::Copy(TCaclResultTemp *pData)
{
	if( pData == NULL || !pData->IsValid() )
		return FALSE;
	m_lDaysLen = pData->m_lDaysLen;
	New(pData->m_lBufLen);
	if( IsValid() )
	{
		Copy( pData->m_lLeftReqult, pData->m_lLeftLen, TCaclResultTemp::ToLeft);
		Copy( pData->m_lRightReqult, pData->m_lRithtLen, TCaclResultTemp::ToRight);
		Copy( pData->m_lDescReqult, pData->m_lDescLen);
	}

	return TRUE;
}

BOOL TCaclResultTemp::IsValid()
{
	if( m_lLeftReqult != NULL && m_lRightReqult != NULL 
		&& m_lBufLen > 0 && m_lBufLen >= m_lLeftLen 
		&& m_lBufLen >= m_lRithtLen 
		&& m_lDescReqult != NULL )
	{
		return TRUE;
	}
	return FALSE;
}

void TCaclResultTemp::New(long lSize)
{
	Del();
	m_lDescReqult = (HSDouble*)new char[sizeof(HSDouble)*lSize];
	if( m_lDescReqult == NULL )
		return;

	m_lLeftReqult = (HSDouble*)new char[sizeof(HSDouble)*lSize];
	if( m_lLeftReqult == NULL )
	{
		delete[] (char*)m_lDescReqult;
		m_lDescReqult = NULL; //20090616 YJT 增加
		return;
	}
	m_lRightReqult = (HSDouble*)new char[sizeof(HSDouble)*lSize];
	if( m_lRightReqult == NULL )
	{
		delete[] (char*)m_lDescReqult;
		m_lDescReqult = NULL; //20090616 YJT 增加
		delete[] (char*)m_lLeftReqult;
		m_lLeftReqult = NULL;
		return;
	}
	m_lBufLen = lSize;
}

void TCaclResultTemp::Del()
{
	if( m_lDescReqult != NULL )
		delete[] (char*)m_lDescReqult;
	m_lDescReqult = NULL;
	if( m_lLeftReqult != NULL )
		delete[] (char*)m_lLeftReqult;
	m_lLeftReqult = NULL;
	if( m_lRightReqult != NULL )
		delete[] (char*)m_lRightReqult;
	m_lRightReqult = NULL;
	m_lDescLen = m_lBufLen = m_lLeftLen = m_lRithtLen = 0;
}

BOOL TCaclResultTemp::Copy(HSDouble* pData, long lCount, int nMask)
{
	HSDouble *pDesc = NULL;
	long *plLen = NULL;
	switch( nMask )
	{
	default:
	case ToDesc:
		pDesc = m_lDescReqult;
		plLen = &m_lDescLen;
		break;
	case ToLeft:
		pDesc = m_lLeftReqult;
		plLen = &m_lLeftLen;
		break;
	case ToRight:
		pDesc = m_lRightReqult;
		plLen = &m_lRithtLen;
		break;
	}
	if( pData != NULL && pDesc != NULL && plLen != NULL
		&& ::AfxIsValidAddress(pData, sizeof(HSDouble)*lCount, TRUE) 
		&& ::AfxIsValidAddress(pDesc, sizeof(HSDouble)*lCount, TRUE) )
	{
		for( int i = 0; i < lCount; i++ )
		{
			pDesc[i] = pData[i];
		}
		*plLen = lCount;
		return TRUE;
	}

	return FALSE;
}

// TCaclTemp::
TCaclTemp::TCaclTemp()
{
	lBeginDate = lEndDate = lCountDate = 0;
	nChengJiaoDiv = 1;
	pPress = NULL;
	pCode = NULL;
	nAnaPeriod = hx_PERIOD_TYPE_DAY_DATA;
	lCountDate = lBuffDiskData = 0;
	pRetDiskData = NULL;
	pRetCacl = NULL;
}

TCaclTemp::TCaclTemp(TCaclTemp& sTemp)
{
	pCode			= sTemp.pCode			;				
	pPress			= sTemp.pPress			;			
	szTechMain		= sTemp.szTechMain		;			
	szTechSideLeft	= sTemp.szTechSideLeft	;		
	szTechSideRitht	= sTemp.szTechSideRitht	;	
	lEndDate		= sTemp.lEndDate		;			
	lCountDate		= sTemp.lCountDate		;			
	nChengJiaoDiv	= sTemp.nChengJiaoDiv	;		
	nAnaPeriod		= sTemp.nAnaPeriod		;		
	bExRights       = sTemp.bExRights;
	lBeginDate		= sTemp.lBeginDate;

	New(lCountDate);		
	if( pRetDiskData != NULL )
	{
		memcpy(pRetDiskData, sTemp.pRetDiskData, sizeof(StockDay)*sTemp.pRetCacl->m_lDescLen);
	}
	if( pRetCacl != NULL )
	{
		pRetCacl->Copy(sTemp.pRetCacl);
	}
}

TCaclTemp::~TCaclTemp()
{
	Free();
}

void TCaclTemp::New(int iDiskDataLen)
{
	pRetCacl = new TCaclResultTemp;
	if( pRetCacl != NULL )
	{
		pRetCacl->New(lCountDate);
	}

	if( iDiskDataLen > 0 )
	{
		pRetDiskData = (StockDay*)new char[iDiskDataLen*sizeof(StockDay)];
	}
	lBuffDiskData = iDiskDataLen;
}

void TCaclTemp::ClearData()
{
	pRetDiskData = NULL;
	lBuffDiskData = 0;

	if( pRetCacl != NULL )
	{
		pRetCacl->Del();
	}
}

void TCaclTemp::Free(BOOL bAll)
{
	if( pRetCacl != NULL )
	{
		delete pRetCacl;
		pRetCacl = NULL;
	}
	if( bAll && pRetDiskData != NULL )
	{
		delete[] (char*)pRetDiskData;
		pRetDiskData = NULL;
	}
	lBuffDiskData = 0;
}

//TExchCaclResult
void TExchCaclResult::ToListCtrl(CListCtrl* pCtrl)
{
	LV_ITEM			lvitem;
	char			chTemp[256];

	lvitem.mask = LVIF_TEXT|LVIF_IMAGE;
	lvitem.iItem = pCtrl->GetItemCount();
	lvitem.iImage = (int)(m_sCaclResult.m_sTotal.m_dNetProfit >=  0);
	lvitem.pszText = chTemp;

	sprintf(chTemp, "%s", m_sCurParam.ToStr());
	lvitem.iSubItem = 0;
	pCtrl->InsertItem(&lvitem);//返回表项插入后的索引号

	lvitem.mask = LVIF_TEXT;
	lvitem.iSubItem = 1;
	sprintf(chTemp, "%.2f%%", m_sCaclResult.m_sTotal.m_dNPPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 2;
	sprintf(chTemp, "%.2f%%", m_sCaclResult.m_sTotal.m_dYearReturnPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 3;
	sprintf(chTemp, "%.2f%%", m_sCaclResult.m_sTotal.m_dWinPercent);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 4;
	sprintf(chTemp, "%d次", m_sCaclResult.m_sTotal.m_lDealNum);
	pCtrl->SetItem(&lvitem);

	lvitem.iSubItem = 5;
	sprintf(chTemp, "%.2f%%", m_sCaclResult.m_sTotal.m_dSucessPercent);
	pCtrl->SetItem(&lvitem);

	pCtrl->SetItemData(lvitem.iItem, (DWORD_PTR)m_sCurParam.m_uParamID);
}

double TExchCaclResult::GetIDData(int nID)
{
	if( nID == HS_EXCH_CACLRESULT_STOCK_ID_PARAM )
	{
		return (double)m_sCurParam.m_uParamID;
	}

	return m_sCaclResult.GetIDData(nID);
}