
#include "stdafx.h"
#include "HSDataSource.h"
#include "express.h"
#include "Tools.h"
#include "RegMsg.h"
#include "FormulaStruct.h"
#include "WinnerApplication.h"
//#include "FormulaCalc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HWND CHSDataSource::m_wndMsg = NULL;

IMPLEMENT_DYNCREATE(CHSDataSource, CObject)

CHSDataSource::CHSDataSource(AskData* pCode /*= NULL*/,
							 DWORD dwStyle /*= HS_HxDataSource_Left_ChuQuan	| 
							 HS_HxDataSource_Read_CaiWu_Data				| 
							 HS_HxDataSource_Read_Now_Data*/)
{
	m_dwStyle	= 0;
	m_nDayCount = 0;
	m_lRefDay   = NULL;

	m_nVolumeUnit = 1;
	
	m_nMoneyUnit  = 1;
	m_nMoneyDecimal = 0;

	m_nPriceUnit  = 1;
	m_nPriceDecimal = 0;

	m_pAsk = new AskData;

	if( pCode != NULL )
	{
		memcpy(m_pAsk,pCode,sizeof(AskData));
	}
	else
	{
		memset(m_pAsk->m_pCode,0,sizeof(CodeInfo));
	}

	m_pSelStockSuccResult = NULL;
	m_pRefDataSource = NULL;	

	this->AddStyle(dwStyle);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
#ifdef SUPPORT_NETVALUE
	m_nNetVolueUint = 1000; // 净值及利率单位（目前均为1000)
#endif

	// 

}

CHSDataSource::~CHSDataSource()
{
	if(m_pAsk != NULL)
	{
		delete m_pAsk;
		m_pAsk = NULL;
	}

	//
	Free();
}

void CHSDataSource::Free()
{
	//
	POSITION pos = m_ayDayData.GetStartPosition();
	WORD nKey;
	CDataSourceUseData *pVal;
	while (pos != NULL)
	{
		m_ayDayData.GetNextAssoc (pos, nKey, pVal);
		if(pVal != NULL)
		{
			delete pVal;
		}
	}
	m_ayDayData.RemoveAll();
}

HSDouble* CHSDataSource::GetData(WORD nkey)
{
	CDataSourceUseData* pData;
	if ( m_ayDayData.Lookup(nkey,pData) && pData)
	{
		if( !pData->IsEmpty() )
		{
			return pData->GetData();
		}
	}
	return NULL;
}

CDataSourceUseData* CHSDataSource::GetDay(StockDay* pDay,long lCurSize,int nOldSize,WORD nkey,
										  StockDay* pLastDay /*= NULL*/)
{
	CDataSourceUseData* pData;
	if ( m_ayDayData.Lookup(nkey,pData) )
	{
		return pData->Alloc(lCurSize,nOldSize,(pLastDay != NULL)?1:0);
	}

	pData = new CDataSourceUseData();
	m_ayDayData.SetAt(nkey,pData);
	return pData->Alloc(lCurSize,nOldSize);	
}

unsigned long GetCurData(StockDay* pCurDay,int nSize)
{
	/*
	long	m_lDate;  //year-month-day ,example: 19960616
	//分钟数据的表示方法如下：yymmddhhnn(年月日时分)
	//yy指的是year - 1990，故年份表达范围：1990 - 2011
	//如0905131045，指的是：1999年5月13号10点45分。
	long	m_lOpenPrice;
	long	m_lMaxPrice;
	long	m_lMinPrice;
	long	m_lClosePrice;	
	long	m_lMoney;			//成交金额
	long	m_lTotal;			//成交量   单位：百股（手）
	long	m_lNationalDebtRatio; // 国债利率(单位为0.1分),基金净值
	*/

	return *(unsigned long*)( ((long*)pCurDay) + nSize );
}

BOOL CHSDataSource::SetDay(StockDay* pDay,long lCurSize,
						   int nVolumeUnit,
						   int nMoneyUnit,short nMoneyDecimal,
						   int nPriceUnit,short nPriceDecimal,
						   WORD nKey /*= 0*/,
						   StockDay* pLastDay /*= NULL*/)
{
	if(nVolumeUnit != 0)
		m_nVolumeUnit = nVolumeUnit;
	if(nMoneyUnit != 0)
		m_nMoneyUnit = nMoneyUnit;
	if(nMoneyDecimal >= 0)
		m_nMoneyDecimal = nMoneyDecimal;
	if(nPriceUnit != 0)
		m_nPriceUnit = nPriceUnit;
	if(nPriceDecimal >= 0)
		m_nPriceDecimal = nPriceDecimal;
	
	CDataSourceUseData* pSingleData = NULL;
	CDataSourceUseData* pOpen   = NULL;
	CDataSourceUseData* pHigh   = NULL;
	CDataSourceUseData* pLow    = NULL;
	CDataSourceUseData* pClose  = NULL;
	CDataSourceUseData* pVol    = NULL;
	CDataSourceUseData* pAmount = NULL;
	CDataSourceUseData* pDate   = NULL;
#ifdef SUPPORT_NETVALUE
	CDataSourceUseData* pNetValue  = NULL; // 净值
#endif

	double dUint = m_nPriceUnit;
	int nPos  = 0;
	if( nKey != 0 ) // 指定的
	{
		if( nKey == hx_OPEN_DATA )
		{
			nPos = 1;
		}
		if( nKey == hx_HIGH_DATA )
		{
			nPos = 2;
		}
		if( nKey == hx_LOW_DATA )
		{
			nPos = 3;
		}
		if( nKey == hx_CLOSE_DATA )
		{
			nPos = 4;
		}
		if( nKey == hx_VOL_DATA )
		{
			nPos  = 6;
			dUint = m_nVolumeUnit; 
		}
		if( nKey == hx_AMOUNT_DATA )
		{
			nPos  = 5;
			dUint = m_nMoneyUnit; 
		}
		if( nKey == hx_DATE_DATA )
		{
			nPos  = 0;
			dUint = 1;
		}

#ifdef SUPPORT_NETVALUE
		if( nKey == hx_NetValue_DATA )
		{
			nPos  = 7;
			dUint = m_nNetVolueUint;
		}
#endif
		pSingleData = GetDay(pDay,lCurSize,m_nDayCount,nKey,pLastDay);
	}
	else
	{
		pOpen   = GetDay(pDay,lCurSize,m_nDayCount,hx_OPEN_DATA,pLastDay);
		pHigh   = GetDay(pDay,lCurSize,m_nDayCount,hx_HIGH_DATA,pLastDay);
		pLow    = GetDay(pDay,lCurSize,m_nDayCount,hx_LOW_DATA,pLastDay);
		pClose  = GetDay(pDay,lCurSize,m_nDayCount,hx_CLOSE_DATA,pLastDay);
		pVol    = GetDay(pDay,lCurSize,m_nDayCount,hx_VOL_DATA,pLastDay);
		pAmount = GetDay(pDay,lCurSize,m_nDayCount,hx_AMOUNT_DATA,pLastDay);
		pDate   = GetDay(pDay,lCurSize,m_nDayCount,hx_DATE_DATA,pLastDay);

#ifdef SUPPORT_NETVALUE
		pNetValue = GetDay(pDay,lCurSize,m_nDayCount,hx_NetValue_DATA,pLastDay);
#endif
	}

	// 除权数据
	//if( IsStyle(HS_HxDataSource_Left_ChuQuan | HS_HxDataSource_Back_ChuQuan) )
	//{
	//	SetChuQuanData(m_pAsk->m_pCode);
	//}

	//// 财务数据
	//if( IsStyle(HS_HxDataSource_Read_CaiWu_Data) )
	//{
	//	SetFinaceData(m_pAsk->m_pCode);
	//}

	// 
	int i;
	StockDay* pCurDay;
	if( pLastDay != NULL && m_nDayCount < lCurSize )	// 只更新最后
	{
		pCurDay = pLastDay;
		i = m_nDayCount;

		if( i > 0 && i == lCurSize )
			i = lCurSize - 1;
		m_nDayCount = lCurSize;
	}
	else
	{
		i = 0;
		m_nDayCount = lCurSize;		
	}
	m_lRefDay   = (long*)pDay;

	//
	for(; i < lCurSize; i++)
	{
		if( pSingleData != NULL && pSingleData->IsEmpty() )
		{
			pCurDay = &pDay[i];
			if( GetCurData(pCurDay, nPos) == hx_DefMinValues )
			{
				pSingleData->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pSingleData->m_pData[i] = GetCurData(pCurDay, nPos)/dUint;
			}
			continue;
		}

		if(pOpen != NULL && !pOpen->IsEmpty())
		{
			if( pDay[i].m_lOpenPrice == hx_DefMinValues )
			{
				pOpen->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pOpen->m_pData[i] = pDay[i].m_lOpenPrice/m_nPriceUnit;
			}
		}
		if(pHigh != NULL && !pHigh->IsEmpty())
		{
			if( pDay[i].m_lMaxPrice == hx_DefMinValues )
			{
				pHigh->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pHigh->m_pData[i] = pDay[i].m_lMaxPrice/m_nPriceUnit;
			}
		}
		if(pLow != NULL && !pLow->IsEmpty())
		{
			if( pDay[i].m_lMinPrice == hx_DefMinValues )
			{
				pLow->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pLow->m_pData[i] = pDay[i].m_lMinPrice/m_nPriceUnit;
			}
		}
		if(pClose != NULL && !pClose->IsEmpty())
		{
			if( pDay[i].m_lClosePrice == hx_DefMinValues )
			{
				pClose->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pClose->m_pData[i] = pDay[i].m_lClosePrice/m_nPriceUnit;
			}
		}
		if(pVol != NULL && !pVol->IsEmpty()) // 成交量
		{
			if( pDay[i].m_lTotal == hx_DefMinValues )
			{
				pVol->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pVol->m_pData[i] = pDay[i].m_lTotal/(unsigned long)m_nVolumeUnit;
			}
		}
		if(pAmount != NULL && !pAmount->IsEmpty())
		{
			if( pDay[i].m_lMoney == hx_DefMinValues )
			{
				pAmount->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pAmount->m_pData[i] = (double)pDay[i].m_lMoney/m_nMoneyUnit;
			}
		}
		if(pDate != NULL && !pDate->IsEmpty())
		{
			if( pDay[i].m_lDate == hx_DefMinValues )
			{
				pDate->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pDate->m_pData[i] = pDay[i].m_lDate; // 日期
			}
		}
#ifdef SUPPORT_NETVALUE
		if(pNetValue != NULL && !pNetValue->IsEmpty())
		{
			if( pDay[i].m_lNationalDebtRatio == hx_DefMinValues 
				|| pDay[i].m_lNationalDebtRatio <= 0 )
			{
				pNetValue->m_pData[i] = hx_DefMinValues;
			}
			else
			{
				pNetValue->m_pData[i] = pDay[i].m_lNationalDebtRatio/m_nNetVolueUint; // 日期
			}
		}
#endif
	}
	m_nSize = lCurSize;
//	m_nDayCount = lCurSize;	
//	m_lRefDay   = (long*)pDay;

	return TRUE;
}

BOOL CHSDataSource::IsEmpty()
{
	return m_ayDayData.IsEmpty();
}

int CHSDataSource::ReadData(CString strCode /*= ""*/)
{
	if( m_pRefDataSource == NULL				|| 
		m_pRefDataSource->m_lRefDay == NULL || 
		m_pRefDataSource->m_nDayCount <= 0 )
		return -1;

	if( !::IsWindow(m_wndMsg) )
		return -1;

	if( m_pAsk == NULL )
		return -2;

	AskData* pAsk = m_pAsk;

	if( !strCode.IsEmpty() )
	{
		/*if( !IsEmpty() && !strCode.CompareNoCase(m_pAsk->m_pCode[0].m_cCode) )
		{
			return 1;
		}*/
		strncpy(m_pAsk->m_pCode[0].m_cCode,strCode,sizeof(m_pAsk->m_pCode[0].m_cCode));
	}
	else
	{
		//if( !IsEmpty() )
		//	return 1;
	}

	strCode = pAsk->m_pCode[0].GetCode();
	StockUserInfo* pStock = (StockUserInfo*)
							SendDataToWnd( &strCode,HS_COMPILE_DATA_WPARAM_GetStockUserInfo );
	if( pStock == NULL )
		return FALSE;

	m_pAsk->m_pCode[0].m_cCodeType = pStock->m_ciStockCode.m_cCodeType;

	RetDiskData* pDiskData = (RetDiskData*)
							 SendDataToWnd(pAsk,HS_COMPILE_DATA_WPARAM_GetDayData);
	if( pDiskData == NULL || pDiskData->IsEmpty() )
		return -3;

	int nDecimal   = pStock->m_nDecimal; 
	int nPriceUnit = pStock->m_nPriceUnit;
	int nChengJiaoDiv = 1;

	StockDay* pDay = (StockDay*)pDiskData->m_pData;

	// 处理和主图数据一样多。
	KQuickSort qsData;
	qsData.m_pData      = pDay;
	qsData.m_nDataLen   = sizeof(StockDay);
	qsData.m_nKeyOffset = 0;
	qsData.m_nKeyLen    = QS_UNLONG;
	qsData.m_nLow       = 0;
	qsData.m_nHigh      = (short)(pDiskData->m_lSize - 1);

	//
	StockDay* pRefDay = (StockDay*)m_pRefDataSource->m_lRefDay;

	StockDay* pTemp = new StockDay[m_pRefDataSource->m_nDayCount];
	memset(pTemp,0,sizeof(StockDay)*m_pRefDataSource->m_nDayCount);

	int nFindPos;
	StockDay* ppTemp = pTemp;
	for( int i = 0; i < m_pRefDataSource->m_nDayCount; i++,ppTemp++ )
	{
		nFindPos = BinarySearch(&qsData,&pRefDay[i].m_lDate);
		if( nFindPos != -1 )
		{
			memcpy(ppTemp,&pDay[nFindPos],sizeof(StockDay));
		}
		else
		{
			ppTemp->m_lDate		  = hx_DefMinValues;

			ppTemp->m_lOpenPrice  = hx_DefMinValues;
			ppTemp->m_lMaxPrice	  = hx_DefMinValues;
			ppTemp->m_lMinPrice   = hx_DefMinValues;
			ppTemp->m_lClosePrice = hx_DefMinValues;
			ppTemp->m_lTotal	  = hx_DefMinValues;
			ppTemp->m_lMoney	  = hx_DefMinValues;
		}
	}

	SetDay( pTemp,m_pRefDataSource->m_nDayCount,nChengJiaoDiv,0,0,nPriceUnit,nDecimal);
	//m_nDayCount = 0;
	m_lRefDay   = 0;

	delete[] pTemp;

	return 1;
}

BOOL CHSDataSource::SetDay(CObject* pDataOwnerObj,StockDay* pDay,long lCurSize,
						   int nVolumeUnit,
						   int nMoneyUnit,short nMoneyDecimal,
						   int nPriceUnit,short nPriceDecimal)
{
	if( m_pDataOwnerObj == pDataOwnerObj )
		return FALSE;
	m_pDataOwnerObj = pDataOwnerObj;

	return SetDay(pDay,lCurSize,nVolumeUnit,nMoneyUnit,
		nMoneyDecimal,nPriceUnit,nPriceDecimal);
}

void CHSDataSource::SetAsk(AskData* pAsk, int nAnaPeriod) //20080826 YJT 补充nAnaPeriod参数
{
	if( pAsk != NULL )
	{
		memcpy(m_pAsk,pAsk,sizeof(AskData));
	}
	else
	{
		memset(m_pAsk,0,sizeof(AskData));
	}
}

void CHSDataSource::SetChuQuanData(CodeInfo* pCode,CArray<ChuQuanData*,ChuQuanData*>* pAyChuQuanData)
{
	CArray<ChuQuanData*,ChuQuanData*>* pArray = 
		(CArray<ChuQuanData*,ChuQuanData*>*)SendDataToWnd(pCode,HS_COMPILE_DATA_WPARAM_GetChuQuanData);
	if( pArray )
	{
		m_ayChuQuanData.RemoveAll();
		for( int i = 0; i < pArray->GetSize(); i++ )
		{
			m_ayChuQuanData.Add(pArray->GetAt(i));
		}
	}
	if ( pAyChuQuanData )
	{
		m_ayChuQuanData.RemoveAll();
		for( int i = 0; i < pArray->GetSize(); i++ )
		{
			m_ayChuQuanData.Add(pAyChuQuanData->GetAt(i));
		}
	}
}
void CHSDataSource::SetPriceUnit(long lPrice)
{
	m_nPriceUnit = lPrice;
	return ;
}
void CHSDataSource::SetFinaceData(CodeInfo* pCode)
{
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFineace;
 	ayCode.Add(*pCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode,ayFineace);
	if ( ayFineace.GetCount() > 0)
		memcpy(&m_HSFinaceData,&ayFineace.GetAt(0),sizeof(CurrentFinanceData));
}	

HSDouble CHSDataSource::GetNowDataByCode(int nPos,
										 char cType /*= 0*/,char* pCode /*= NULL*/)
{
	if( pCode == NULL )
		pCode = m_pAsk->m_pCode[0].m_cCode;

	// need to modify 
	//数据引擎接口读取
	CRealTimeDataSource* pHSNowDataSource = 
		(CRealTimeDataSource*)SendDataToWnd(pCode,HS_COMPILE_DATA_WPARAM_GetNowData);
	if (pHSNowDataSource != NULL)
	{
		double nPriceUnit = (double)(int)SendDataToWnd(&pHSNowDataSource->m_stStockInfo.m_ciStockCode.m_cCodeType,
									HS_COMPILE_DATA_WPARAM_GetStockPriceUnit);
		return pHSNowDataSource->GetDataByPos(nPos,nPriceUnit,cType);
	}
	return hx_DefMaxValues;
}

void CHSDataSource::CalcChuQuan(ChuQuanData* pData,double& dwData)
{
	if( pData != NULL )
	{
		if( pData->m_fMulit != 0 )
		{
			dwData = (dwData + pData->m_fAdd) / pData->m_fMulit;
		}
		else
		{
			dwData = (dwData + pData->m_fAdd);
		}
	}
}

void CHSDataSource::CalcChuQuan(ChuQuanData* pData,long& lData,int& nPriceUnit)
{
	if( pData )
	{
		if( pData->m_fMulit != 0 )
		{
			lData = (long)((lData + pData->m_fAdd*nPriceUnit) / pData->m_fMulit*nPriceUnit);
		}
		else
		{
			lData = (long)(lData + pData->m_fAdd*nPriceUnit);
		}
	}
}

BOOL CHSDataSource::CalcChuQuan(StockDay& pDay,DWORD dwStyle /*= 0*/)
{
	ChuQuanData* pData;
	int i;

	if ( IsStyle(HS_HxDataSource_Left_ChuQuan) )	// 向前除权
	{
		for(i = 0; i < m_ayChuQuanData.GetSize(); i++)
		{
			pData = m_ayChuQuanData.GetAt(i);
			if(pData->m_cMask == ChuQuanData::Calc)
			{
				if( pDay.m_lDate < pData->m_lDate )
				{
					pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)m_nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)m_nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lMinPrice   = ((pDay.m_lMinPrice/(double)m_nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lClosePrice = ((pDay.m_lClosePrice/(double)m_nPriceUnit + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lTotal	   = (long)(pDay.m_lTotal*pData->m_fMulit);
					pDay.m_lMoney	   = (long)(pDay.m_lMoney*pData->m_fMulit);
				}
			}
		}
		return TRUE;
	}
	else if( IsStyle(HS_HxDataSource_Back_ChuQuan) ) // 向后除权
	{
		for(i = 0; i < m_ayChuQuanData.GetSize(); i++)
		{
			pData = m_ayChuQuanData.GetAt(i);
			if(pData->m_cMask == ChuQuanData::Calc)
			{
				if( pDay.m_lDate > pData->m_lDate )
				{
					pDay.m_lOpenPrice  = ((pDay.m_lOpenPrice/(double)m_nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lMaxPrice   = ((pDay.m_lMaxPrice/(double)m_nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lMinPrice   = ((pDay.m_lMinPrice/(double)m_nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lClosePrice = ((pDay.m_lClosePrice/(double)m_nPriceUnit + pData->m_fAdd) / pData->m_fMulit)*m_nPriceUnit;
					pDay.m_lTotal	   = (long)(pDay.m_lTotal*pData->m_fMulit);
					pDay.m_lMoney	   = (long)(pDay.m_lMoney*pData->m_fMulit);
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

CArray<SuccResult*,SuccResult*>* CHSDataSource::SetSeleckStockData(CodeInfo* pCode /*= NULL*/)
{
	if( pCode == NULL )
		pCode = m_pAsk->m_pCode;

	m_pSelStockSuccResult = AddSuccDataToMap(pCode);//CFormulaCalc::AddSuccDataToMap(pCode);

	return m_pSelStockSuccResult;
}

void CHSDataSource::CalcChuQuan(StockDay* pDay,long lCurSize,void* pData /*= NULL*/)
{
	if( !IsStyle(HS_HxDataSource_Left_ChuQuan | HS_HxDataSource_Back_ChuQuan) ) 
		return;

	SetDay(pDay,lCurSize,m_nVolumeUnit,m_nMoneyUnit,
		m_nMoneyDecimal,m_nPriceUnit,m_nPriceDecimal);
}

HSDouble CHSDataSource::GetFinaceDataByID(UINT nID)
{
	if( m_HSFinaceData.IsEmpty() ) // 没有数据，读取
	{
		SetFinaceData(m_pAsk->m_pCode);
	}
	if( !m_HSFinaceData.IsEmpty() )
	{
		return m_HSFinaceData.GetDataByID(nID);
	}
	return hx_DefMaxValues;
}

////////////////////////////////////////////////////////////////////////////////////
HSDouble CHSDataSource::CAPITAL()
{
	HSDouble dRet = hx_DefMaxValues;

	if( ((CodeInfo*)m_pAsk->m_pCode)->IsType(KIND_STOCKB) )
		dRet = GetFinaceDataByID(COLUMN_CAPITALIZATION_B);
	else if( MakeIndexMarket( ((CodeInfo*)m_pAsk->m_pCode)->m_cCodeType ) )//((CodeInfo*)m_pAsk->m_pCode)->IsType(KIND_INDEX) )
		dRet = 0;
	else //if( ((CodeInfo*)m_pAsk->m_pCode)->IsType(KIND_STOCKA) )
		dRet = GetFinaceDataByID(COLUMN_CAPITALIZATION_PASS_A);

	if(dRet.m_dData != hx_DefMaxValues)
		dRet *= 100;

	return dRet;
}

HSDouble CHSDataSource::GetFinaceDataByPos(int nPos)
{
	if( m_HSFinaceData.IsEmpty() ) // 没有数据，读取
	{
		SetFinaceData(m_pAsk->m_pCode);
	}
	if( !m_HSFinaceData.IsEmpty() )
	{
		return m_HSFinaceData.GetDataByPos(nPos);
	}
	return hx_DefMaxValues;
}

long CHSDataSource::SendDataToWnd( void* pBuffer, WPARAM wParam )
{
	if( ::IsWindow(m_wndMsg) )
	{
		return ::SendMessage( m_wndMsg, HX_USER_COMPILEDATA, wParam, (LPARAM)pBuffer );
	}
	return 0;
}

BOOL CHSDataSource::ReadFinancial(CString strCode,AllFinancialData* pAllFinancialData /*= NULL*/)
{
	if( strCode.IsEmpty() && m_pAsk )
		strCode = m_pAsk->m_pCode[0].GetCode();

	if( pAllFinancialData == NULL )
	{
		pAllFinancialData = &m_sAllFinancialData;
	}

	/*return CFinanceStatmentFile::Load(strCode,pAllFinancialData);*/
	return TRUE;
}
/********************************************************************************
* 函数功能 : 用于返回时间(段)内数据和偏移值同期的数据
* 函数参数 : CString strID  - 字段简称,或者下标索引,
*			 int nOffset	- 偏移年份的值,
*			 int Begin,int End, - 时间段 (0,0表示所有;-1,-1表示最后一期)
*			 CString code	- 代码,
*			HSCurDataInfoStatus* pCurDataStatus - 参数, int& nError - 错误值
*  返回值  : 数据指针
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-17
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
HSDouble CHSDataSource::PROFFIN(CString strID,
								HSCurDataInfoStatus* pRefDataStatus,
								/*int nOffset,*/
								int Begin,int End,CString code,
								HSCurDataInfoStatus* pCurDataStatus, int& nError)
{	

#if 0	/* 以下代码实现了读取时间段某期的数据 保留代码*/
	if(type < 1 || type > 4)
	{
        return PROFFINON(strID, Begin, End, code, pCurDataStatus, nError);
	}

	if( code.IsEmpty() && m_pAsk )
		code = m_pAsk->m_pCode[0].GetCode();

	if( m_sAllFinancialData.IsEmpty(code) )
	{
		ReadFinancial(code, &m_sAllFinancialData);
	}

	int nBeginPos	= 0;
	int nEndPos		= -1;

	if(Begin > 0 && End >= Begin)
	{
		nBeginPos	= m_sAllFinancialData.Find(Begin);
		nEndPos		= m_sAllFinancialData.Find(End, nBeginPos);
	}

	if(nBeginPos <= nEndPos)
	{
		int nID = m_sAllFinancialData.GetFinColProperty(strID);
		ASSERT(nID != -1);
		if(nID == -1)
		{
			strID += ":Cann't know ID!";
			WRITE_LOG(__FILE__, __LINE__, strID);
		}
		else
		{
			UpdatedFinancialData* pData = NULL;

			int nSize =  nEndPos - nBeginPos ;
			/* 临时 用于收集数据 */
			HSDouble* hdData = new HSDouble[nSize];
			int nExactlySize = 0;
			for(int i = 0; i < nSize; i++)
			{
				pData = m_sAllFinancialData.GetData(i + nBeginPos);
				if(pData == NULL)
				{
					continue;
				}
				/* 比较期数 */
				if(m_sAllFinancialData.GetType(pData->m_lBiuldDate) != type)
				{
					continue;
				}

				hdData[nExactlySize++] = m_sAllFinancialData.GetDataByID(pData, nID);	
			}

			pCurDataStatus->Alloc(nExactlySize);
			for(i = 0; i < nExactlySize; i++)
			{
				pCurDataStatus->m_pData[i].m_dData = hdData[i];
			}

			delete[] hdData;
		}
	}
#endif	

#if 0 // 用于返回同期比的数据 代码暂留 作为参考	
	pCurDataStatus->Free(); 

	if( code.IsEmpty() && m_pAsk )
		code = m_pAsk->m_pCode[0].GetCode();

	if( m_sAllFinancialData.IsEmpty(code) ) 
	{
		ReadFinancial(code, &m_sAllFinancialData);
	}
	
	int nBeginPos, nEndPos;
	m_sAllFinancialData.Find(Begin, End, nBeginPos, nEndPos);
	
	if(nBeginPos < nEndPos)
	{
		pCurDataStatus->Alloc(nEndPos - nBeginPos);

		int nID = m_sAllFinancialData.GetFinColProperty(strID);
		ASSERT(nID != -1);
		if(nID == -1)
		{
			strID += ":Cann't know ID!";
			WRITE_LOG(__FILE__, __LINE__, strID);
		}
		else
		{
			nOffset			*= 10000; /* 偏移年 */
			long lOffsetPos = 0;

			UpdatedFinancialData* pData			= NULL;
			UpdatedFinancialData* pOffsetData	= NULL;

			HSDouble hdData, hdOffset;
			
			for( int i = nBeginPos; i < nEndPos; i++ )
			{
				pData = m_sAllFinancialData.GetData(i);
				if(pData == NULL)
				{
					continue;
				}

				lOffsetPos = m_sAllFinancialData.Find(pData->m_lBiuldDate + nOffset,
					i, nEndPos, FALSE);
				pOffsetData = m_sAllFinancialData.GetData(lOffsetPos);
				if(pOffsetData == NULL)
				{
					continue;
				}
				
				hdData = m_sAllFinancialData.GetDataByID(pData, nID);
				hdOffset = m_sAllFinancialData.GetDataByID(pOffsetData, nID);
				pCurDataStatus->m_pData[i- nBeginPos].m_dData = ((hdData - hdOffset) / hdOffset) * (HSDouble)100;
			}
		}
	}
#endif
	return hx_ReturnPointValues;
	//return hx_DefMinValues;
}

HSDouble CHSDataSource::PROFFINON(CString strID,						// 字段						
								  int Begin,							// 开始日期	
								  int End,								// 终止日期
								  HSCurDataInfoStatus* pRefDataStatus,	// 参照数据
								  int nOffset,							// 偏移日期数
								  CString code,							// 代码
								  HSCurDataInfoStatus* pCurDataStatus,  // 返回数据
								  int& nError)
{
	ASSERT(pCurDataStatus);
	pCurDataStatus->Free(); 

	if( code.IsEmpty() && m_pAsk )
		code = m_pAsk->m_pCode[0].GetCode(); 

	m_sAllFinancialData.SetMyBlockData(-2, -2, "");
	// 读取数据
	if( m_sAllFinancialData.IsEmpty(code) 
		&& ReadFinancial(code, &m_sAllFinancialData) == FALSE) 
	{
		return hx_ReturnPointValues;	
	}

	// 取得字段ID,目前支持英文简称和索引下标
	int nID = m_sAllFinancialData.GetFinColProperty(strID); 
	if(nID == -1)
	{
		strID += ":Cann't know ID!";
		/*WRITE_LOG(__FILE__, __LINE__, strID);*/
		return hx_ReturnPointValues;	
	}

	int		nUseOffset	= 0; // 偏移年 
	int		nOffsetPos	= -1;// 偏移年的数据位置

	UpdatedFinancialData*	pData				= NULL; // 偏移数据
	HSDrawOtherData*		pHSDrawOtherData	= NULL; // 日期数据

	// 根据日期取得其偏移年的数据
	if(  pRefDataStatus && pRefDataStatus->m_pData && pRefDataStatus->m_lSize > 0)
	{
		nUseOffset	= nOffset * 10000; /* 偏移年 */
		pCurDataStatus->Alloc(pRefDataStatus->m_lSize);
		
		for( int i = 0; i < pRefDataStatus->m_lSize; i++ )
		{
			if(pRefDataStatus->m_pData[i].m_pOtherData == NULL)
			{
				continue;
			}

			nOffsetPos = m_sAllFinancialData.Find((long)pRefDataStatus->m_pData[i].m_pOtherData->m_dValue + nUseOffset,
				-1, -1, FALSE);
			
			if(nOffsetPos == -1)
			{
				continue;
			}

			pData = m_sAllFinancialData.GetData(nOffsetPos);
			if(pData == NULL)
			{
				continue;
			}

			pHSDrawOtherData = pCurDataStatus->m_pData[i].AllocOther(NULL,TRUE);
			if( pHSDrawOtherData )
			{
				pHSDrawOtherData->m_dValue = pData->m_lBiuldDate; // 日期
			}
			
			pCurDataStatus->m_pData[i].m_dData = m_sAllFinancialData.GetDataByID(pData, nID);			
		}
	}
	else
	{
		int nBeginPos = -1, nEndPos = -1;
		m_sAllFinancialData.Find(Begin, End, nBeginPos, nEndPos);
		if(nEndPos > nBeginPos && nBeginPos >= 0)
		{
			pCurDataStatus->Alloc(nEndPos - nBeginPos);
			for( int i = nBeginPos; i < nEndPos; i++ )
			{
				pData = m_sAllFinancialData.GetData(i);
				if(pData == NULL)
				{
					continue;
				}

				pHSDrawOtherData = pCurDataStatus->m_pData[i- nBeginPos].AllocOther(NULL,TRUE);
				if( pHSDrawOtherData )
				{
					pHSDrawOtherData->m_dValue = pData->m_lBiuldDate; // 日期
				}

				pCurDataStatus->m_pData[i- nBeginPos].m_dData = m_sAllFinancialData.GetDataByID(pData, nID);
			}

		}
	}

	return hx_ReturnPointValues;
	//return hx_DefMinValues;
}
/********************************************************************************
* 函数功能 : 与公式的接口
* 函数参数 : CString strID - 字段简称或者索引,
		CString strBlockName - 板块名称,
		int Begin,int End - 区间,
		HSCurDataInfoStatus* pRefDataStatus - 参考计算方式时的数据指针,
		int nOffset - 参考计算方式时的偏移值,
		HSCurDataInfoStatus* pCurDataStatus - 结果, int& nError - 错误,
	    int nOperator - 操作,目前未使用
*  返回值  : 数据指针
* 调用依赖 : 财务报表的板块分析
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-20
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
HSDouble CHSDataSource::BlockCalc(CString strID,
		CString strBlockName,
		int Begin,int End,
		HSCurDataInfoStatus* pRefDataStatus,
		int nOffset,
		HSCurDataInfoStatus* pCurDataStatus, int& nError,
	    int nOperator /*= 0*/)
{
	ASSERT(pCurDataStatus);
	pCurDataStatus->Free(); 

	// 取得字段ID,目前支持英文简称和索引下标
	int nID = m_sAllFinancialData.GetFinColProperty(strID); 
	ASSERT(nID != -1);
	if(nID == -1)
	{
		strID += ":Cann't know ID!";
		/*WRITE_LOG(__FILE__, __LINE__, strID);*/
		return hx_ReturnPointValues;	
	}

	TRY
	{
		UpdatedFinancialData*	pData				= NULL; // 偏移数据
		HSDrawOtherData*		pHSDrawOtherData	= NULL; // 日期数据

		int		nCalcOK = 0;	// 计算结果数据个数
		int		nDataPos = -1;	// 数据位置

		// 根据日期取得其偏移年的数据
		if(  pRefDataStatus && pRefDataStatus->m_pData && pRefDataStatus->m_lSize > 0)
		{
			int		nOffsetDate	= -1;				// 偏移年的数据日期
			int		nUseOffset	= nOffset * 10000;	// 偏移年 

			pCurDataStatus->Alloc(pRefDataStatus->m_lSize);			
			
			if(pRefDataStatus->m_pData[0].m_pOtherData && 
				pRefDataStatus->m_pData[pRefDataStatus->m_lSize -1].m_pOtherData )
			{
				// 请求的日期偏移区间
				Begin	= (long)pRefDataStatus->m_pData[0].m_pOtherData->m_dValue + nUseOffset;
				End		= (long)pRefDataStatus->m_pData[pRefDataStatus->m_lSize -1].m_pOtherData->m_dValue+ nUseOffset;

				// 
				nCalcOK = BlockCalc( strBlockName, Begin, End,  &m_sAllFinancialData, nOperator);
				if(nCalcOK)
				{
					for( int i = 0; i < pRefDataStatus->m_lSize; i++ )
					{
						if(pRefDataStatus->m_pData[i].m_pOtherData == NULL)
						{
							continue;
						}

						nOffsetDate = (long)pRefDataStatus->m_pData[i].m_pOtherData->m_dValue + nUseOffset;

						nDataPos = m_sAllFinancialData.Find(nOffsetDate, -1, -1, FALSE);
						pData	 = m_sAllFinancialData.GetData(nDataPos);
						if(pData == NULL)
						{
							continue;
						}

						pHSDrawOtherData = pCurDataStatus->m_pData[i].AllocOther(NULL,TRUE);
						if( pHSDrawOtherData )
						{
							pHSDrawOtherData->m_dValue = pData->m_lBiuldDate;
						}

						pCurDataStatus->m_pData[i].m_dData = m_sAllFinancialData.GetDataByID(pData, nID);			
					}
				}
			}
		}
		else
		{
			BlockCalc( strBlockName, Begin, End,  &m_sAllFinancialData, nOperator);
			
			int nBeginPos = -1, nEndPos = -1;
			m_sAllFinancialData.Find(Begin, End, nBeginPos, nEndPos);
			
			if(nBeginPos < nEndPos && nBeginPos >= 0)
			{
				pCurDataStatus->Alloc(nEndPos - nBeginPos);
				for(int i = nBeginPos; i < nEndPos; i++)
				{
					pData = m_sAllFinancialData.GetData(i);
					if(pData )
					{
						pHSDrawOtherData = pCurDataStatus->m_pData[i - nBeginPos].AllocOther(NULL,TRUE);
						if( pHSDrawOtherData )
						{
							pHSDrawOtherData->m_dValue = pData->m_lBiuldDate;
						}

						pCurDataStatus->m_pData[i - nBeginPos].m_dData = m_sAllFinancialData.GetDataByID(pData, nID);		
					}
				}
			}
		}
	}
	CATCH(CFileException, pEx)
	{
		/*HS_FILE_EXCEPTION(pEx);*/
		//pEx->ReportError(MB_ICONERROR|MB_OK);
	}
	AND_CATCH(CMemoryException, pEx)
	{
		/*WRITE_LOG(__FILE__, __LINE__,"MemoryException");*/
		//pEx->ReportError(MB_ICONERROR|MB_OK);
	}
	END_CATCH;

	return hx_ReturnPointValues;
}
/********************************************************************************
* 函数功能 : 取得板块财务数据，目前为求和
* 函数参数 :int nID,字段ID索引值
*			CStringArray* pAyStrCode - 股票代码的数组指针,非NULL,	
*			long lDate - 日期,绝对查找, 
*			int nOperator - 操作方式,目前未使用,求和处理 
*		    CTypedPtrMap<CMapStringToPtr, CString, AllFinancialData*>* pMapCodeData 
*				内存MAP, 如果外部指定,必须在外部删除内存,为NULL则使用内建MAP
*  返回值  : HSDouble 返回结果
* 调用依赖 : 
* 作    者 : 熊钟彬
* 完成日期 : 2004-04-20
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/
int CHSDataSource::BlockCalc(CString strBlockName,	
							 int Begin,int End,
							 AllFinancialData* pRetAllFinancialData,
							 int nOperator /*= 0*/)

{
	ASSERT( pRetAllFinancialData);
	// 指针 非NULL判断 此指针用于返回数据,必须非NULL
	if( !pRetAllFinancialData)
	{
		return 0;
	}
	
	/* 是否已经取过了数据, 直接使用内存数据 实际上,如果板块在计算期间发生了改变,将被忽略*/
	if(pRetAllFinancialData->IsMyBlockData(Begin, End, strBlockName))
	{
		return pRetAllFinancialData->GetSize();
	}
	
	pRetAllFinancialData->Free();
	/* 读取板块数据 */
	CStringArray AyStrCode; 
	//CHSTools::ReadBlock(NULL, strBlockName.GetBuffer(), FALSE, NULL, NULL, &AyStrCode);

	const int nSize = AyStrCode.GetSize();
	if(nSize <= 0)
	{
		return 0;
	}	

	CString strCode;
	UpdatedFinancialData*	pData = NULL; // 数据
	UpdatedFinancialData*	pRetUpdatedFinancialData = NULL;

	AllFinancialData		tagAllFinancialData ;
	
    CMap<int, int , int*, int*> m_ayCalcCount;
	
	int* pnCalcCount = NULL;
	int nKey = 0;

	int nRetPos = -1;
	
	int nBeginPos = -1, nEndPos = -1;
	for(int i = 0; i < nSize; i++)
	{
		strCode = AyStrCode.GetAt(i);		
		//CFinanceStatmentFile::Load(strCode, &tagAllFinancialData);
		
		if(tagAllFinancialData.IsEmpty(strCode))
		{
			continue;
		}		

		tagAllFinancialData.Find(Begin, End, nBeginPos, nEndPos);
		if(nEndPos <= nBeginPos || nBeginPos < 0)
		{
			continue;
		}


		for(int n = nBeginPos; n < nEndPos; n++) 
		{
			pData = tagAllFinancialData.GetData(n);
			if(pData == NULL)
			{
				continue;
			}

			nRetPos = pRetAllFinancialData->Find(pData->m_lBiuldDate, -1, -1, FALSE, FALSE) ;
			if(nRetPos == -1)
			{
				pRetAllFinancialData->Append((TCHAR*)pData, sizeof(UpdatedFinancialData));				
			}
			else
			{
				pRetUpdatedFinancialData = pRetAllFinancialData->GetData(nRetPos);
				if(pRetUpdatedFinancialData == NULL)
				{
					continue;
				}

				for(int j = 0; j < FINANCIAL_DATA_LENGTH; j++)
				{
					pRetUpdatedFinancialData->m_fData[j] += pData->m_fData[j];
				}
				pRetUpdatedFinancialData->m_lBiuldDate = pData->m_lBiuldDate;
			}
			
			// 统计实际参与计算的个数
			if(!m_ayCalcCount.Lookup(pData->m_lBiuldDate, pnCalcCount))
			{
				pnCalcCount = new int;
				(*pnCalcCount) = 0;
				m_ayCalcCount.SetAt(pData->m_lBiuldDate, pnCalcCount);
			}

			(*pnCalcCount) ++;
		}				
	}

	// 求均值
	for(int n = 0; n < pRetAllFinancialData->GetSize(); n++)
	{
		pRetUpdatedFinancialData = pRetAllFinancialData->GetData(n);
		if(pRetUpdatedFinancialData == NULL)
		{
			continue;
		}

		m_ayCalcCount.Lookup(pRetUpdatedFinancialData->m_lBiuldDate, pnCalcCount);
		for(int j = 0; j < FINANCIAL_DATA_LENGTH; j++)
		{
			pRetUpdatedFinancialData->m_fData[j] /= (*pnCalcCount);
		}
	}
	
	POSITION  pos;
	for( pos = m_ayCalcCount.GetStartPosition(); pos != NULL; )
	{
		m_ayCalcCount.GetNextAssoc( pos, nKey, pnCalcCount );
		if( pnCalcCount )
		{
			delete pnCalcCount;
		}
	}
	m_ayCalcCount.RemoveAll();	

	pRetAllFinancialData->ReSort();
	pRetAllFinancialData->SetMyBlockData(Begin, End, strBlockName);
	return pRetAllFinancialData->GetSize();
}

void CHSDataSource::EmptyData()
{
	m_sAllFinancialData.Free();
}