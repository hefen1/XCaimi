#include "stdafx.h"
#include "FormulaManPublic.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void HSFree( void* pData, int nType /*= charType*/ )
{
// 	if ( pData && !IsBadReadPtr(pData,1))
// 	{
		switch( nType )
		{
		case HSCStringCopy:
		case HSCString:
			{
				delete (CString*)pData;
			}
			break;
		case HSDoubleType:
			{
				delete[] (HSDouble*)pData;
			}
			break;
		case charType:
			{
				delete[] (char*)pData;
			}
			break;
		case InfoIndexType:
			{
				//delete (InfoIndex*)pData;
			}
			break;
		case InfoIndexSubType:
			{
				//delete (InfoIndexSub*)pData;
			}
			break;
		case HSDrawOtherDataType:
			{
				delete (HSDrawOtherData*)pData;
			}
			break;
		}
	/*}*/
	
}

void* HSAlloc( long nCurSize,int nType /*= charType */  )
{		
	switch( nType )
	{
	case HSCStringCopy:
		{
			CString *pStr = (CString *)nCurSize;
			CString *pCurStr = (new CString);
			if( pStr )
			{
				pCurStr->Format("%s",*pStr);
			}
			return pCurStr;
		}
		break;
	case HSCString:
		{
			return (new CString);
		}
		break;
	case HSDoubleType:
		{
			if( nCurSize <= 0 )
			{
				return NULL;
			}
			HSDouble* pData = new HSDouble[nCurSize];
			for ( int i=0; i<nCurSize; i++)
				pData[i].init();	
			return pData;
		}
		break;
	case charType:
		{
			if( nCurSize <= 0 )
			{
				return NULL;
			}
			char* pData = new char[nCurSize];
			memset( pData,0,nCurSize);
			return pData;
		}
		break;
	case InfoIndexType:
		{
		}
		break;
	case InfoIndexSubType:
		{

		}
		break;
	case HSDrawOtherDataType:
		{
			HSDrawOtherData* pOld = (HSDrawOtherData*)nCurSize;
			HSDrawOtherData* pNew = new HSDrawOtherData;
			pNew->Copy(pOld);
			return pNew;
		}
		break;
	}

	return NULL;
}

long hxDefMaxValues()
{
	return 0x80000000;  // ��Сֵ
}

long hxDefMinValues()
{
	return 0x7fffffff;  // ���ֵ
}

long hxReturnPointValues()
{
	return 0x5fffffff;  // ������ֵ����ָ��
}

void SetDefaultValues(HSDouble* pData,long lSize,HSDouble values)
{
	if(pData == NULL || lSize <= 0)
		return;
	for(int i = 0; i < lSize; i++)
	{
		pData[i] = values;
	}
}

void SetDefaultValues(long* pData,long lSize)
{
	if(pData == NULL || lSize <= 0)
		return;
	for(int i = 0; i < lSize; i++)
	{
		pData[i] = hx_DefMinValues;
	}
}

// ��ͼ����
CLineProperty::CLineProperty()
{ 
	m_pData    = NULL;
	m_nSize    = 0;

	m_dwStyle  = 0;
 	m_pCurVal  = NULL;
 	m_pDataSource = NULL;

	memset(m_strName,0,sizeof(m_strName));
	memset(m_strFormat,0,sizeof(m_strFormat));

	this->m_pCalcParam = NULL;

	m_nColID = -1;
}

CLineProperty::~CLineProperty()
{
	Free();
	FreeStr();
}

void CLineProperty::AllocStr(CString* pStr)
{
	memcpy(m_strFormat, *pStr, min(sizeof(m_strFormat)-1,pStr->GetLength()));
}

void CLineProperty::FreeStr()
{
	return;
}

CString	CLineProperty::GetFormat(HSDouble& dValue)
{
	CString strRet; 
	CString strFormat="";
	strFormat.Format("%s", m_strFormat);
	if( dValue.m_pOtherData /*&& dValue.m_pOtherData->m_strText*/ )
	{ 
		strRet.Format("%s",dValue.m_pOtherData->m_cText);
	}
	else if( dValue.m_dData != hx_DefMinValues && 
		 strFormat.Find("%") != -1 )
	{
		if(strFormat.FindOneOf("DdIi") != -1)
		{
			strRet.Format(strFormat,(__int64)dValue.m_dData);
		}
		else
		{
			strRet.Format(strFormat,dValue.m_dData);
		}
	}
	else if( dValue.m_dData != hx_DefMinValues )
	{
		strRet.Format("%.3f",dValue.m_dData);
	}

	return strRet;
}


// ���������С�ٷֱ�
// ע�⣺ʹ�ô˺���ǰ�������ж�nBegin��nEnd�ĺϷ���
BOOL CLineProperty::CalcUpDownPercent(int nBegin, int nEnd, double& dUp, double& dDown, double dBaseIntput/*=(double)hx_DefMinValues*/)
{
	if( m_pData == NULL )
		return FALSE;

	double dMax, dMin;
	double dBase;
	int j;

	for( j = nBegin; j < nEnd; j++ )
	{
		if( m_pData[j].m_dData != hx_DefMinValues && m_pData[j].m_dData != hx_DefMaxValues )
		{
			break;
		}
	}
	if( j >= nEnd )
		return FALSE;

	dBase = m_pData[j].m_dData;

	//if( dBaseIntput != (double)hx_DefMinValues )
	//{
	//	dBaseIntput /= 1000;
	//	double dPercent = fabs(dBase/dBaseIntput);
	//	if( dPercent > 0.01 && dPercent < 100 )
	//	{
	//		dBase = dBaseIntput;
	//	}
	//}
	if( fabs(dBase) <= 0.001 )
	{
		return FALSE;
	}

	dMax = m_pData[j].m_dData;
	dMin = m_pData[j].m_dData;
	for( int i = j+1; i < nEnd; i++ )
	{
		if( m_pData[i].m_dData == hx_DefMinValues || m_pData[i].m_dData == hx_DefMaxValues )
			continue;
		dMax = max(dMax, m_pData[i].m_dData);
		dMin = min(dMin, m_pData[i].m_dData);
	}

	dUp = 100 * (dMax - dBase) / dBase;
	dDown = 100 * (dMin - dBase) / dBase;

	return TRUE;
}



BOOL CLineProperty::IsTrue(int nDataPos) 
{
	if( m_pData != NULL )
	{
		if( m_pData[nDataPos] == hx_DefMinValues )
			return FALSE;

		return (BOOL)m_pData[nDataPos];
	}
	return FALSE;
}

BOOL CLineProperty::IsEmpty() { return m_pData == NULL; }	
void CLineProperty::init()
{
	for (int i=0;i<m_nSize; i++)
		m_pData[i].init();
}
void CLineProperty::Alloc(int nCurSize,int nOldSize)
{
	if( nCurSize <= 0 )
	{
		Free();
		return;
	}
	if( (nCurSize != nOldSize) || (m_pData == NULL) )
	{			
		Free();

 		if( HSDrawData::m_pHSExternAlloc )
 		{
 			m_pData = (HSDouble*)(*HSDrawData::m_pHSExternAlloc)(nCurSize,HSDoubleType);
 			m_nSize = nCurSize;
 		}

	}
	if( m_pData != NULL )
	{
		SetDefaultValues(m_pData,nCurSize,hx_DefMinValues);
	}
}

void CLineProperty::Free()
{
	if ( m_pData != NULL && !IsBadReadPtr(m_pData,1))
	{	
		delete[] m_pData;
		m_pData = NULL;
	
		m_nSize = 0;
	}	
}

void CLineProperty::DefMinMax()
{
	m_lMaxValue  = hx_DefMaxValues;
	m_lMinValue  = hx_DefMinValues;
}

BOOL	CLineProperty::IsStyle(long dwStyle)		{ return (m_dwStyle & dwStyle); }
void	CLineProperty::AddStyle(long dwStyle)		{ m_dwStyle |= dwStyle;         }
void	CLineProperty::RemoveStyle(long dwStyle)	{ m_dwStyle &= (~dwStyle);      }
void    CLineProperty::SetStyle(long dwStyle)		{ m_dwStyle = dwStyle;          }

void HSDrawOtherData::Copy( HSDrawOtherData* pData )
{
	if( pData == NULL )
		return;

	m_bType1 = pData->m_bType1;
	m_bType2 = pData->m_bType2;
	m_bType3 = pData->m_bType3;
	m_bType4 = pData->m_bType4;

	m_dValue = pData->m_dValue;

	memcpy(m_cText,pData->m_cText, sizeof(m_cText));
	//m_strText = pData->m_strText;
	
}


HSExternAlloc HSDrawData::m_pHSExternAlloc = HSAlloc;
HSExternFree  HSDrawData::m_pHSExternFree  = HSFree;

HSDrawOtherData* HSDrawData::AllocOther(HSDrawOtherData* pData,BOOL bForce /*= FALSE*/)
{
	FreeOther();

	if( m_pHSExternAlloc )
	{
		if( bForce || pData )
		{
			m_pOtherData = (HSDrawOtherData*)(*m_pHSExternAlloc)
				( (long)pData,HSDrawOtherDataType );
		}
	}

	return m_pOtherData;
}

void HSDrawData::FreeOther()
{
	if( m_pOtherData != NULL && !IsBadReadPtr(m_pOtherData,1))
	{
		try
		{
			delete m_pOtherData;
			m_pOtherData = NULL;
		}
		catch (...)
		{
			OutputDebugString("HSDrawData::FreeOther()====>�����쳣��������ֱ������");
		}
	}
}

void HSDrawData::Copy(HSDrawData* pData,BOOL bMsg/* = FALSE*/)
{
	if( pData == NULL ) 
		return;

	m_dData = pData->m_dData;

	AllocOther(pData->m_pOtherData);
}

// �����Сֵ
CDispMaxMinValue::CDispMaxMinValue()
{
	Empty();
}
void CDispMaxMinValue::Empty()
{
	m_nPos   = -1;
	m_lValue = -1;
}

BOOL CDispMaxMinValue::IsFind(long nPos)
{
	return (m_nPos != -1) && (m_nPos == nPos) && 
		(m_lValue.m_dData != hx_DefMinValues && m_lValue.m_dData != hx_DefMaxValues);
}	

//
CNameProperty::CNameProperty()						{ m_dStyle = 0;}

BOOL CNameProperty::IsEmpty()						{ return m_strName.IsEmpty();			}
int  CNameProperty::CompareNoCase( LPCTSTR lpsz )   { return m_strName.CompareNoCase(lpsz); }

BOOL	CNameProperty::IsStyle(WORD dStyle)		    { return (m_dStyle & dStyle); }
void	CNameProperty::AddStyle(WORD dStyle)		{ m_dStyle |= dStyle;         }
void	CNameProperty::RemoveStyle(WORD dStyle)	    { m_dStyle &= (~dStyle);      }
void    CNameProperty::SetStyle(WORD dStyle)		{ m_dStyle = dStyle;          }

//////////////////////////////////////////////////////////////////////////
CHSFinanceData::CHSFinanceData()
{

}
CHSFinanceData::CHSFinanceData( CurrentFinanceData* pFinanceData)
{
	if (pFinanceData && !::IsBadReadPtr(pFinanceData, 1))
		memcpy(this, pFinanceData, sizeof(CurrentFinanceData));
}

HSDouble CHSFinanceData::CAPITAL(CodeInfo* pCode)
{
	if( pCode == NULL )
		return hx_DefMaxValues;

	HSDouble dRet = hx_DefMaxValues;

	if( pCode->IsType(KIND_STOCKB) )
		dRet = GetDataByID(COLUMN_CAPITALIZATION_B);
	else if( MakeIndexMarket( pCode->m_cCodeType ) )
		dRet = 0;
	else //if( pCode->IsType(KIND_STOCKA) ) 
		dRet = GetDataByID(COLUMN_CAPITALIZATION_PASS_A);	

	if((long)dRet != hx_DefMaxValues )
		dRet *= 100;

	return dRet;
}

HSDouble CHSFinanceData::GetDataByID(UINT nID)
{
	/*	    
	1	�ܹɱ�
	2	���ҹ�
	3	�����˷��˹�
	4	���˹�
	5	B��
	6	H��
	7	��ͨA��
	8	ְ����
	9	A2ת���

	10	���ʲ�
	11	�����ʲ�
	12	�̶��ʲ�
	13	�����ʲ�
	14	����Ͷ��
	15	������ծ
	16	���ڸ�ծ
	17	�ʱ�������
	18	ÿ�ɹ�����
	19	�ɶ�Ȩ��
	20	��Ӫ����
	21	��Ӫ����
	22	��������
	23	Ӫҵ����
	24	Ͷ������
	25	��������
	26	Ӫҵ����֧
	27	�����������
	28	�����ܶ�
	29	˰������ 
	30	������
	31	δ��������
	32	ÿ��δ����
	33	ÿ������
	34	ÿ�ɾ��ʲ�
	35	����ÿ�ɾ���
	36	�ɶ�Ȩ���
	37	����������
	*/

	if ( nID >= COLUMN_CAPITALIZATION_TOTAL && nID <= COLUMN_CAPITALIZATION_A2_GIVE )
	{
		nID = nID - COLUMN_CAPITALIZATION_TOTAL;
		return (HSDouble)m_fFinanceData[2+nID];
	}
	else if ( nID >= COLUMN_FINANCE_TOTAL_ASSETS && nID <= COLUMN_FINANCE_ASSETS_YIELD )
	{
		nID = COLUMN_CAPITALIZATION_A2_GIVE - COLUMN_CAPITALIZATION_TOTAL + 1 + nID - COLUMN_FINANCE_TOTAL_ASSETS;
		return (HSDouble)m_fFinanceData[2+nID];
	}
	else if ( nID >= 0 && nID < sizeof(m_fFinanceData)) //20090605 YJT ����
	{
		return (HSDouble)m_fFinanceData[nID];
	}

	return hx_DefMaxValues;
}

HSDouble CHSFinanceData::GetDataByPos(int nPos)
{
	nPos += 1;
	if ( nPos >= 0 && nPos < sizeof(m_fFinanceData)/sizeof(m_fFinanceData[0]) )
		return (HSDouble)m_fFinanceData[nPos];
	return hx_DefMaxValues;
}


CRealTimeDataSource::CRealTimeDataSource()
{
	Empty();
}

HSDouble CRealTimeDataSource::GetDataByPos(int nPos,double nPriceUnit,char cType /*= 0*/)
{
	if( nPriceUnit <= 0 )
		nPriceUnit = 1;

	long nHand = m_stStockData.m_nHand;
	if(nHand <= 0)
		nHand = 100;	

	switch( nPos ) 
	{
		// ����:  DYNAINFO(3) ��̬���麯��: ����
	case COLUMN_HQ_BASE_PRECLOSE:
	case 3: return this->m_stStockInfo.m_lPrevClose / nPriceUnit;
		// ����:  DYNAINFO(4) ��̬���麯��: ��
	case COLUMN_HQ_BASE_OPEN:
	case 4: return this->m_stStockData.m_lOpen / nPriceUnit;
		// ����:  DYNAINFO(5) ��̬���麯��: ���
	case COLUMN_HQ_BASE_MAX_PRICE:
	case 5: return this->m_stStockData.m_lMaxPrice / nPriceUnit;
		// ����:  DYNAINFO(6) ��̬���麯��: ���
	case COLUMN_HQ_BASE_MIN_PRICE:
	case 6: return this->m_stStockData.m_lMinPrice / nPriceUnit;
		// ����:  DYNAINFO(7) ��̬���麯��: ����
	case COLUMN_HQ_BASE_NEW_PRICE:
	case 7: return this->m_stStockData.m_lNewPrice / nPriceUnit;
		// ����:  DYNAINFO(8) ��̬���麯��: ����
	case COLUMN_HQ_BASE_TOTAL_HAND:
	case 8: return this->m_stStockData.m_lTotal / nHand;
		// ����:  DYNAINFO(9) ��̬���麯��: ����
	case COLUMN_HQ_BASE_HAND:
	case 9: return this->m_sOthData.m_lCurrent / nHand;
		// ����:  DYNAINFO(10) ��̬���麯��: �ܶ�
	case COLUMN_HQ_BASE_MONEY:
	case 10: return this->m_stStockData.m_fAvgPrice / 10000;
		// ����:  DYNAINFO(11) ��̬���麯��: ����
	case COLUMN_HQ_BASE_AVERAGE_PRICE:
	case 11: return (double)((double long)m_stStockData.m_fAvgPrice * nPriceUnit / m_stStockData.m_lTotal) / nPriceUnit;
		// ����:  DYNAINFO(12) ��̬���麯��: �ǵ�
	case COLUMN_HQ_BASE_RISE_VALUE:
	case 12: return (m_stStockData.m_lNewPrice - m_stStockInfo.m_lPrevClose) / nPriceUnit;
		// ����:  DYNAINFO(13) ��̬���麯��: ���(%)
	case COLUMN_HQ_BASE_RANGE:
	case 13: 
		if( m_stStockInfo.m_lPrevClose != 0 )
		{
			return (double)(m_stStockData.m_lMaxPrice - m_stStockData.m_lMinPrice) * 100 / 
				m_stStockInfo.m_lPrevClose; // *100
		}
		break;
		// ����:  DYNAINFO(14) ��̬���麯��: �Ƿ�(%)
	case COLUMN_HQ_BASE_RISE_RATIO:
	case 14: 
		if( m_stStockInfo.m_lPrevClose != 0 && 
			m_stStockData.m_lNewPrice  != 0 )
		{
			return (double)(m_stStockData.m_lNewPrice - m_stStockInfo.m_lPrevClose) * 100 / 
				m_stStockInfo.m_lPrevClose;// / nPriceUnit;
		}
		break;
		// ����:  DYNAINFO(15) ��̬���麯��: ί��
	case COLUMN_HQ_BASE_ORDER_RATIO:
	case 15: 
		{
			long BuyCount = m_stStockData.m_lBuyCount1  + 
				m_stStockData.m_lBuyCount2 + 
				m_stStockData.m_lBuyCount3 +
				m_stStockData.m_lBuyCount4 + 
				m_stStockData.m_lBuyCount5;
			long SellCount = m_stStockData.m_lSellCount1 + 
				m_stStockData.m_lSellCount2 + 
				m_stStockData.m_lSellCount3 +
				m_stStockData.m_lSellCount4 +
				m_stStockData.m_lSellCount5;
			long A = BuyCount - SellCount;
			long B = BuyCount + SellCount;

			if((B != 0) && (A != 0))
			{
				//long lBi = (long)(10000.0 * (double)A/(double)B);
				return (10000.0 * (double)A/(double)B) / 100 ;
			} 
		}
		break;
		// ����:  DYNAINFO(16) ��̬���麯��: ί��
	case COLUMN_HQ_BASE_ORDER_DIFF:
	case 16: 
		{
			long BuyCount  = m_stStockData.m_lBuyCount1  + 
				m_stStockData.m_lBuyCount2 + 
				m_stStockData.m_lBuyCount3 + 
				m_stStockData.m_lBuyCount4 +
				m_stStockData.m_lBuyCount5;
			long SellCount = m_stStockData.m_lSellCount1 + 
				m_stStockData.m_lSellCount2 + 
				m_stStockData.m_lSellCount3 +
				m_stStockData.m_lSellCount4 +
				m_stStockData.m_lSellCount5;

			long A = BuyCount - SellCount;
			return (A / nHand);
		}
		break;
		// ����:  DYNAINFO(17) ��̬���麯��: ����
	case COLUMN_HQ_BASE_VOLUME_RATIO:
	case 17:
		{
			if(m_stStockInfo.m_l5DayVol > 0)
			{
				double Bi = (double)((double long)m_stStockData.m_lTotal * 100  * 
					m_nTolalTime /  // CStockManager::GetBourseTotalTime(m_stStockInfo.m_ciStockCode.m_cCodeType)
					((double long)m_stStockInfo.m_l5DayVol * (m_sOthData.m_nTime + 1)));
				return (Bi / 100);
			}
		}
		break;
		// ����:  DYNAINFO(18) ��̬���麯��: ί��
	case COLUMN_HQ_BASE_ORDER_BUY_VOLUME:
	case 18: return (m_stStockData.m_lBuyCount1 / nHand);
		// ����:  DYNAINFO(19) ��̬���麯��: ί��
	case COLUMN_HQ_BASE_ORDER_SELL_VOLUME:
	case 19: return (m_stStockData.m_lSellCount1 / nHand);
		// ����:  DYNAINFO(20) ��̬���麯��: ί���
	case COLUMN_HQ_BASE_ORDER_BUY_PRICE:
	case 20: return (m_stStockData.m_lBuyPrice1 / nPriceUnit);
		// ����:  DYNAINFO(21) ��̬���麯��: ί����
	case COLUMN_HQ_BASE_ORDER_SELL_PRICE:
	case 21: return (m_stStockData.m_lSellPrice1 / nPriceUnit);
		// ����:  DYNAINFO(22) ��̬���麯��: ����
	case COLUMN_HQ_BASE_IN_HANDS:
	case 22: return (m_sOthData.m_lInside / nHand);
		// ����:  DYNAINFO(23) ��̬���麯��: ����
	case COLUMN_HQ_BASE_OUT_HANDS:
	case 23: return (m_sOthData.m_lOutside / nHand);
		// ����:  DYNAINFO(24) ��̬���麯��: ����
	case COLUMN_HQ_BASE_RISE_SPEED:
	case 24: break;
		// ����:  DYNAINFO(25) ��̬���麯��: ��һ��
		//case :
	case 25: return (m_stStockData.m_lBuyCount1 / nHand);
		// ����:  DYNAINFO(26) ��̬���麯��: �����
	case 26: return (m_stStockData.m_lBuyCount2 / nHand);
		// ����:  DYNAINFO(27) ��̬���麯��: ������
	case 27: return (m_stStockData.m_lBuyCount3 / nHand);
		// ����:  DYNAINFO(28) ��̬���麯��: ��һ��
	case 28: return (m_stStockData.m_lBuyPrice1 / nPriceUnit);
		// ����:  DYNAINFO(29) ��̬���麯��: �����
	case 29: return (m_stStockData.m_lBuyPrice2 / nPriceUnit);
		// ����:  DYNAINFO(30) ��̬���麯��: ������
	case 30: return (m_stStockData.m_lBuyPrice3 / nPriceUnit);
		// ����:  DYNAINFO(31) ��̬���麯��: ��һ��
	case 31: return (m_stStockData.m_lSellCount1 / nHand);
		// ����:  DYNAINFO(32) ��̬���麯��: ������
	case 32: return (m_stStockData.m_lSellCount2 / nHand);
		// ����:  DYNAINFO(33) ��̬���麯��: ������
	case 33: return (m_stStockData.m_lSellCount3 / nHand);
		// ����:  DYNAINFO(34) ��̬���麯��: ��һ��
	case 34: return (m_stStockData.m_lSellPrice1 / nPriceUnit);
		// ����:  DYNAINFO(35) ��̬���麯��: ������
	case 35: return (m_stStockData.m_lSellPrice2 / nPriceUnit);
		// ����:  DYNAINFO(36) ��̬���麯��: ������
	case 36: return (m_stStockData.m_lSellPrice3 / nPriceUnit);
		// ����:  DYNAINFO(37) ��̬���麯��: ������
	case COLUMN_HQ_EX_EXHAND_RATIO:
	case 37:
		{
			double dHuanShoulv = 0;
			if( m_dHuanShoulv > 10000 )
			{
				dHuanShoulv = m_stStockData.m_lTotal / (m_dHuanShoulv / 10000);
			}
			else if( m_dHuanShoulv != 0 )
			{
				dHuanShoulv = m_stStockData.m_lTotal / m_dHuanShoulv;
			}
			return (dHuanShoulv / 10000);
		}
		break;
		// ����:  DYNAINFO(38) ��̬���麯��: 5�վ���
	case COLUMN_HQ_EX_5DAY_AVGVOLUME:
	case 38: return (m_stStockInfo.m_l5DayVol / nHand);
		// ����:  DYNAINFO(39) ��̬���麯��: ��ӯ��
	case COLUMN_HQ_EX_PE_RATIO:
	case 39:
		{
			double nShiYing = m_dShiYinglv;
			nShiYing *= nPriceUnit;

			//20090605 YJT �޸���ӯ���㷨
			double nQuarter = m_dQuarter;
			if (abs(nQuarter - 1) < 0.0001)
				nQuarter = 0.25;
			else if (abs(nQuarter - 2) < 0.0001)
				nQuarter = 0.5;
			else if (abs(nQuarter - 3) < 0.0001)
				nQuarter = 0.75;
			else
				nQuarter = 1;

			if( nShiYing != 0 && m_stStockData.m_lNewPrice != 0 )
			{
				return ((m_stStockData.m_lNewPrice / nShiYing) * nPriceUnit / nPriceUnit * nQuarter);
			}
		}
		break;
		// ����:  DYNAINFO(40) ��̬���麯��: �ɽ�����
	case 40: break;
	//---------2009.11.06  litao���Ӷ�����4��5���Ĵ���
		// ����:  DYNAINFO(41) ��̬���麯��: ���ļ�
	case 41: return (m_stStockData.m_lBuyPrice4 / nPriceUnit);
		// ����:  DYNAINFO(42) ��̬���麯��: �����
	case 42: return (m_stStockData.m_lBuyPrice5 / nPriceUnit);
		// ����:  DYNAINFO(43) ��̬���麯��: ������
	case 43: return long(m_stStockData.m_lBuyCount4 / nHand);
		// ����:  DYNAINFO(44) ��̬���麯��: ������
	case 44: return long(m_stStockData.m_lBuyCount5 / nHand);
		// ����:  DYNAINFO(45) ��̬���麯��: ���ļ�
	case 45: return (m_stStockData.m_lSellPrice4 / nPriceUnit);
		// ����:  DYNAINFO(46) ��̬���麯��: �����
	case 46: return (m_stStockData.m_lSellPrice5 / nPriceUnit);
		// ����:  DYNAINFO(47) ��̬���麯��: ������
	case 47: return long(m_stStockData.m_lSellCount4 / nHand);
		// ����:  DYNAINFO(48) ��̬���麯��: ������
	case 48: return long(m_stStockData.m_lSellCount5 / nHand);	

	//--------------------------------------------------------
	}

	return hx_DefMaxValues;
}

BOOL CRealTimeDataSource::IsEmpty()
{
	return ( (m_stStockData.m_lNewPrice == 0) && 
		(m_stStockInfo.m_lPrevClose == 0) );
}
