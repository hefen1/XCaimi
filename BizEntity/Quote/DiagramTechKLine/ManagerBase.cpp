
#include "stdafx.h"
#include "ManagerBase.h"
#include "TechKLineMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double FLOATZERO = 0.00000001;
#define IDT_MINUTE_AUTO	60000
#define IDT_HARFHOUR_AUTO 5400000
//////////////////////////////////////////////////////////////////////////
//CFenshiData
CFenshiData::CFenshiData()
{
	m_pPriceValues = NULL;
	m_pChengJiaoValues = NULL;
	m_pAvePriceValues = NULL;
	m_lTotalVol = 0;
	m_lTotalPrice = 0;
	m_nCurrentDataLen = 0;		
	m_nLastReqDay = 0;			
	m_nLastRecvDay = 0;			
	m_nOneDayDataNum = 240;		
	m_nRecvDataLen = 0;			
	m_nCurFenshiPos = 0;

	m_pPriceValuesCur = NULL;		
	m_pChengJiaoValuesCur = 0;
	m_pAvePriceValuesCur = 0;
	m_nTimeID = 0;
	m_pMgr = NULL;
	memset(m_ayDayData, 0, sizeof(m_ayDayData));
	memset(m_ayPrevClose, 0, sizeof(m_ayPrevClose));
}

BOOL CFenshiData::Init(StockUserInfo* pInfo, int nSize, BOOL bForce)
{
	if (m_info.m_ciStockCode.CompareCode(&pInfo->m_ciStockCode) && !bForce)
	{
		return TRUE;
	}
	m_info.Copy(pInfo);
	
	if (nSize <= m_nOneDayDataNum)
	{
		nSize = m_nOneDayDataNum + 1;
	}
	if (nSize > 0)
	{
		if(m_pPriceValues == NULL)
		{
			TRACE("\r\n alloc m_pPriceValues size is %d", nSize);
			m_pPriceValues = new DrawFenShiData[nSize];
		}
		if(m_pChengJiaoValues == NULL)
		{
			m_pChengJiaoValues = new long[nSize];
		}
		if(m_pAvePriceValues == NULL)
		{
			m_pAvePriceValues = new long[nSize];
		}
		memset(m_pPriceValues, 0, sizeof(DrawFenShiData) * nSize);
		memset(m_pChengJiaoValues, 0, sizeof(long) * nSize);
		memset(m_pAvePriceValues, 0, sizeof(long) * nSize);

		m_nCurrentDataLen = nSize;
	}
	memset(m_ayDayData, 0, sizeof(m_ayDayData));
	memset(m_ayPrevClose, 0, sizeof(m_ayPrevClose));

	m_nRecvDataLen = 0;
	m_nCurFenshiPos = 0;
	m_nLastRecvDay = 0;
	m_pPriceValuesCur = NULL;
	m_pChengJiaoValuesCur = NULL;
	m_pAvePriceValuesCur = NULL;
	m_nLastReqDay = nSize / m_nOneDayDataNum;
	return FALSE;
}

BOOL CFenshiData::Release()
{
	if (m_pPriceValues != NULL)
	{
		delete []m_pPriceValues;
		m_pPriceValues = NULL;
	}
	if(m_pChengJiaoValues != NULL)
	{
		delete []m_pChengJiaoValues;
		m_pChengJiaoValues = NULL;
	}
	if(m_pAvePriceValues != NULL)
	{
		delete []m_pAvePriceValues;
		m_pAvePriceValues = NULL;
	}
	memset(m_ayDayData, 0, sizeof(m_ayDayData));	
	memset(m_ayPrevClose, 0, sizeof(m_ayPrevClose));
	m_minData.Release();
	return FALSE;
}

BOOL CFenshiData::Refresh()
{
	if (m_nCurrentDataLen > 0)
	{
		if (m_pPriceValues != NULL)
		{
			memset(m_pPriceValues, 0, sizeof(DrawFenShiData) * m_nCurrentDataLen);
		}
		if(m_pChengJiaoValues != NULL)
		{
			memset(m_pChengJiaoValues, 0, sizeof(long) * m_nCurrentDataLen);
		}
		if(m_pAvePriceValues != NULL)
		{
			memset(m_pAvePriceValues, 0, sizeof(long) * m_nCurrentDataLen);
		}
	}
	m_lTotalVol = 0;
	m_lTotalPrice = 0;
	m_nLastReqDay = m_nCurrentDataLen / m_nOneDayDataNum;
	m_nLastRecvDay = 0;
	m_nRecvDataLen = 0;
	m_nCurFenshiPos = 0;
	m_pPriceValuesCur = NULL;
	m_pChengJiaoValuesCur = NULL;
	m_pAvePriceValuesCur = NULL;
	memset(m_ayDayData, 0, sizeof(m_ayDayData));
	memset(m_ayPrevClose, 0, sizeof(m_ayPrevClose));
	m_minData.Release();

	return FALSE;
}

int CFenshiData::UpdataTreadEx(AnsTrendData_Ext* pExtData)
{
	Refresh();
	if (pExtData == NULL)
	{
		return 0;
	}
	int nLen = pExtData->m_nHisLen;
	ASSERT(nLen <= m_nOneDayDataNum + 1);
	if (nLen > m_nOneDayDataNum + 1)
	{
		nLen = m_nOneDayDataNum + 1;
	}

	int nStart = m_nCurrentDataLen - m_nOneDayDataNum - 1;
	for (int i = 0; i < nLen; i++)
	{
		TRACE("\r\n m_pPriceValues index is %d", i+nStart);
		m_pPriceValues[i + nStart].m_nMessageMask = 0;
		m_pPriceValues[i + nStart] = pExtData->m_pHisData[i].m_lNewPrice;
		m_pPriceValues[i + nStart].m_lTime = i;
		if (pExtData->m_pHisData[i].m_lNewPrice == 0)
		{
			m_pPriceValues[i + nStart] = m_info.m_lPrevClose;
		}

		m_pChengJiaoValues[i + nStart] = pExtData->m_pHisData[i].m_lTotal - m_lTotalVol;
		m_lTotalVol = pExtData->m_pHisData[i].m_lTotal;
		m_lTotalPrice += (m_pChengJiaoValues[i + nStart] * (double long)m_pPriceValues[i + nStart]);
		if(m_lTotalVol > 0)
		{
			m_pAvePriceValues[i + nStart] = (long)(m_lTotalPrice  / m_lTotalVol);
		}
		else
			m_pAvePriceValues[i + nStart] = pExtData->m_nowDataExt.m_stockRealTime.m_lOpen;
	}
	m_pPriceValuesCur = m_pPriceValues;
	m_pChengJiaoValuesCur = m_pChengJiaoValues;
	m_pAvePriceValuesCur = m_pAvePriceValues;
	m_nLastRecvDay = 1;
	m_nRecvDataLen += nLen;
	m_nCurFenshiPos = 0;
	if (nLen <= m_nOneDayDataNum)
	{
		StartTimer();
	}
	return nLen;
}

int CFenshiData::UpdataHisTrend(AnsHisTrend* pTrendData, int& nCurDataLen)
{
	if (pTrendData == NULL)
	{
		return 0;
	}
	m_nLastRecvDay ++;
	ASSERT(m_nLastRecvDay <= MAX_FENSHI_DAYS);
	int nNeedLen = m_nLastReqDay * m_nOneDayDataNum + 1;
	int nOldDataLen = m_nCurrentDataLen;
	DrawFenShiData* pPriceValues = NULL;
	long* pChengJiaoValues = NULL;
	long* pAvePriceValues = NULL;

	m_ayDayData[m_nLastRecvDay] = pTrendData->m_shTend.m_shHead.m_lDate;
	m_ayPrevClose[m_nLastRecvDay] = pTrendData->m_shTend.m_shHead.m_lPrevClose;

	TRACE("count = %d and current date = %d\n", m_nLastRecvDay, pTrendData->m_shTend.m_shHead.m_lDate);

	if (m_nCurrentDataLen < nNeedLen)
	{
		m_nCurrentDataLen = nNeedLen;

		pPriceValues = new DrawFenShiData[m_nCurrentDataLen];
		memset(pPriceValues, 0, sizeof(DrawFenShiData) * m_nCurrentDataLen);
		pChengJiaoValues = new long[m_nCurrentDataLen];
		memset(pChengJiaoValues, 0, sizeof(long) * m_nCurrentDataLen);
		pAvePriceValues = new long[m_nCurrentDataLen];
		memset(pAvePriceValues, 0, sizeof(long) * m_nCurrentDataLen);
		int nLastStartPos = m_nCurrentDataLen - nOldDataLen;
		if(m_pPriceValues != NULL)
		{
			DrawFenShiData* pPriceValuesPlus = pPriceValues + nLastStartPos;
			memcpy(pPriceValuesPlus, m_pPriceValues, sizeof(DrawFenShiData) * nOldDataLen);
			delete []m_pPriceValues;
			m_pPriceValues = NULL;
		}
		m_pPriceValues = pPriceValues;
		if(m_pChengJiaoValues != NULL)
		{
			long* pChengJiaoValuesPlus = pChengJiaoValues + nLastStartPos;
			memcpy(pChengJiaoValuesPlus, m_pChengJiaoValues, sizeof(long) * nOldDataLen);
			delete []m_pChengJiaoValues;
			m_pChengJiaoValues = NULL;
		}
		m_pChengJiaoValues = pChengJiaoValues;
		if(m_pAvePriceValues != NULL)
		{
			long* pAvePriceValuesPlus = pAvePriceValues + nLastStartPos;
			memcpy(pAvePriceValuesPlus, m_pAvePriceValues, sizeof(long) * nOldDataLen);
			delete []m_pAvePriceValues;
			m_pAvePriceValues = NULL;
		}
		m_pAvePriceValues = pAvePriceValues;
	}
	else
	{
		int nCurPos = (m_nCurrentDataLen - m_nOneDayDataNum *  m_nLastRecvDay - 1);
		pPriceValues = m_pPriceValues + nCurPos;
		pChengJiaoValues = m_pChengJiaoValues + nCurPos;
		pAvePriceValues = m_pAvePriceValues  + nCurPos;
	}
	if (pPriceValues != NULL && pChengJiaoValues != NULL && pAvePriceValues != NULL)
	{
		int nLen = pTrendData->m_shTend.m_shHead.m_nSize;
		if (nLen > 0)
		{
			double long lTotal = 0;
			double long lTotalPrice = 0;
			for (int i = 0; i < nLen; i ++)
			{
				pPriceValues[i].m_nMessageMask = 0;
				pPriceValues[i] = pTrendData->m_shTend.m_shData[i].m_lNewPrice;
				pPriceValues[i].m_lTime = i;
				pChengJiaoValues[i] = pTrendData->m_shTend.m_shData[i].m_lTotal - lTotal;
				lTotal = pTrendData->m_shTend.m_shData[i].m_lTotal;
				lTotalPrice += (pChengJiaoValues[i] * (double long)pPriceValues[i]);
				if(lTotal > 0)
				{
					pAvePriceValues[i] = (long)(lTotalPrice  / lTotal);
				}
				else
					pAvePriceValues[i] = pTrendData->m_shTend.m_shHead.m_lPrevClose;
			}
		}
		m_nRecvDataLen += (nLen - 1);
		m_pPriceValuesCur = pPriceValues;
		m_pChengJiaoValuesCur = pChengJiaoValues;
		m_pAvePriceValuesCur = pAvePriceValues;
		m_nCurFenshiPos = m_nCurrentDataLen - nNeedLen;
		nCurDataLen = m_nRecvDataLen;
		return m_nCurrentDataLen;
	}
	return 0;
}

int	CFenshiData::UpdateRealTimeData(CommRealTimeData_Ext* pCommRealTimeData)
{
	if (pCommRealTimeData == NULL)
	{
		return 0;
	}
	ShareRealTimeData_Ext *pShareData = pCommRealTimeData->GetShareData();
	int nTime = pCommRealTimeData->m_othData.m_nTime;
	HSStockRealTime *pRTH = &pShareData->m_nowDataExt.m_stockRealTime;;
	int nIndex = pCommRealTimeData->m_othData.m_nTime;

//	int nPurePos = m_nRecvDataLen % m_nOneDayDataNum;
	TRACE("\r\n m_nRecvDataLen is %d", m_nRecvDataLen);
	if(m_pPriceValues[m_nRecvDataLen - 1].m_lTime < nIndex )//m_pPriceValues[m_nRecvDataLen].m_lTime != 0 && )
		m_nRecvDataLen ++;
	nIndex = m_nRecvDataLen - 1;

	m_pPriceValues[nIndex].m_nMessageMask = 0;//SD_FENSHITRACEDATA;
	m_pPriceValues[nIndex].m_lTime = nTime;

	double long lNewVol =  pRTH->m_lTotal - m_lTotalVol;
	m_pChengJiaoValues[nIndex] += lNewVol;
	m_lTotalVol = pRTH->m_lTotal;
	m_pPriceValues[nIndex] = pRTH->m_lNewPrice;

	m_lTotalPrice += (pRTH->m_lNewPrice * lNewVol);

	if (m_lTotalVol > 0 )
	{
		m_pAvePriceValues[nIndex] = (long)(m_lTotalPrice/ m_lTotalVol);	
	}
	else if( nIndex > 0 )
	{
		m_pAvePriceValues[nIndex] = m_pAvePriceValues[nIndex-1];//m_pStock->m_lPrevClose;	
	}
	if( m_lTotalPrice < 0 )
		m_lTotalPrice = 0;
	StartTimer();
	return m_nRecvDataLen - m_nCurFenshiPos;
}

int CFenshiData::UpdateMinTechData(DataHead* pData)
{
	if (pData != NULL)
	{
		AnsDayDataEx* pMinData = (AnsDayDataEx*)pData;
		if (pMinData->m_nSize <= 0 || memcmp(&m_info.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return 0;
		}
		StockCompDayDataEx* pDataRecv = pMinData->m_sdData;
		int nCurrentDataLen = pMinData->m_nSize;
		TRACE("分时中收到分钟数据，长度=%d\n", nCurrentDataLen);

		if (nCurrentDataLen > m_nCurrentDataLen - m_nCurFenshiPos)
		{
			pDataRecv += (nCurrentDataLen -  (m_nCurrentDataLen - m_nCurFenshiPos));
			nCurrentDataLen = m_nCurrentDataLen - m_nCurFenshiPos;
			TRACE("分时中收到分钟数据，长度截断,保留长度=%d\n", nCurrentDataLen);
		}
		
		m_minData.Refresh(nCurrentDataLen);
		if(!m_minData.CopyData(pDataRecv, nCurrentDataLen))		
			return -1;

		return nCurrentDataLen;
	}
	return 0;
}

int	CFenshiData::UpdateFenshiDataPos(int nDayPosNew, int& nLeftLen)
{
	m_nLastReqDay = nDayPosNew;
	if (m_nLastReqDay < 1)
	{
		m_nLastReqDay = 1;
	}
	if (m_nLastReqDay > MAX_FENSHI_DAYS)
	{
		m_nLastReqDay = MAX_FENSHI_DAYS;
	}
	int nNewPos = m_nLastRecvDay - m_nLastReqDay;
	if (nNewPos < 0)
	{
		nNewPos = 0;
	}
	if (nNewPos > MAX_FENSHI_DAYS)
	{
		nNewPos = MAX_FENSHI_DAYS;
	}
	if (nNewPos <= m_nLastRecvDay)
	{
		int nPos = nNewPos * m_nOneDayDataNum;
		m_nCurFenshiPos = nPos;
		m_pPriceValuesCur = m_pPriceValues + nPos;
		m_pChengJiaoValuesCur = m_pChengJiaoValues + nPos;
		m_pAvePriceValuesCur = m_pAvePriceValues + nPos;

		nLeftLen = m_nRecvDataLen - nPos;
		return m_nCurrentDataLen - nPos;
	}
	return 0;
}

int	CFenshiData::GetPosTime(int nPos, CString& strText, BOOL bIntegral)
{
	int nRetPos = 0;
	if (m_nCurrentDataLen - m_nCurFenshiPos > m_nOneDayDataNum + 1)
	{
//		nRet = (m_nRecvDataLen - m_nCurFenshiPos) / m_nOneDayDataNum;// - 1;
		int nSearchPos = m_nCurrentDataLen - (m_nCurFenshiPos  + nPos);
		nRetPos = m_nCurrentDataLen - m_nCurFenshiPos - 1 - m_nOneDayDataNum;
		if(m_nCurFenshiPos + nPos < m_nLastRecvDay * m_nOneDayDataNum && m_nOneDayDataNum > 0)
		{
			int nReverseDayIndex = nSearchPos / m_nOneDayDataNum;
			if (nReverseDayIndex < 0)
			{
				nReverseDayIndex = 0;
			}
			if(bIntegral)
			{
				if ((m_nCurFenshiPos + nPos ) % m_nOneDayDataNum == 0 && nReverseDayIndex <= m_nLastRecvDay)
				{
					long lDate = m_ayDayData[nReverseDayIndex];
					CString strDate;
					strDate.Format("%d", lDate);
					strText = strDate;
				}
			}
			else
			{	
				if (nReverseDayIndex <= m_nLastRecvDay)
				{
					long lDate = m_ayDayData[nReverseDayIndex + 1];
					CString strDate;
					strDate.Format("%d", lDate);
					strText = strDate;
				}
			}
		}
	}
	return nRetPos;
}

long CFenshiData::GetPrevClose()
{
	if (m_nLastReqDay > 0 && m_ayPrevClose[m_nLastReqDay] > 0)
	{
		return m_ayPrevClose[m_nLastReqDay];
	}
	return m_info.m_lPrevClose;
}

BOOL CFenshiData::GetMinDataRequest(IRequestCollector* pReqCollector, int nReqLen)
{
	if(nReqLen > 0 && nReqLen > m_minData.m_nCurrentDataLen && pReqCollector != NULL)
	{
		short nCount = sizeof(ReqDayData) / sizeof(CodeInfo);
		if( sizeof(ReqDayData) % sizeof(CodeInfo) )
		{
			nCount++;
		}
		int lAskLen = sizeof(AskData) + sizeof(CodeInfo) * (nCount - 1);
		char* pData = new char[lAskLen]; 
		AskData*  pAskData = (AskData*)pData;
		memset(pAskData, 0, lAskLen);
		pAskData->m_nType  = RT_TECHDATA_EX;
		pAskData->m_nSize  = nCount;
		memcpy(pAskData->m_pCode, &m_info.m_ciStockCode,sizeof(CodeInfo));
		memcpy(&pAskData->m_nPrivateKey, &m_info.m_ciStockCode,sizeof(CodeInfo));

		ReqDayData* pRequest = (ReqDayData*)pAskData->m_pCode;
		pRequest->m_nDay = nReqLen;
		pRequest->m_lBeginPosition = 0;
		memcpy(&pRequest->m_ciCode, &m_info.m_ciStockCode, sizeof(CodeInfo));
		pRequest->m_nPeriodNum = 1;
		pRequest->m_cPeriod = PERIOD_TYPE_MINUTE1;
		pRequest->m_nSize = 0;
		pAskData->m_lKey = PERIOD_TYPE_MINUTE1;

		pReqCollector->AddRequest((char*)pAskData,lAskLen);
		delete[] pData;
		return TRUE;
	}
	else if (nReqLen > 0 )
	{
		m_minData.ResetPos(nReqLen);
	}

	return FALSE;
}

BOOL CFenshiData::GetNeedRefresh(StockUserInfo* pInfo)
{
	if(pInfo != NULL)
		return  !m_info.m_ciStockCode.CompareCode(&pInfo->m_ciStockCode);
	return FALSE;
}

CString CFenshiData::GetLastNewPrice(BOOL bBuy)
{
	CString strRet = "";
	if (m_nRecvDataLen > 0)
	{
		long lValue = m_pPriceValues[m_nRecvDataLen - 1].m_lValue;
		char szText[64];
		CGeneralHelper::LongToString(lValue, 0, m_info.m_nPriceUnit, m_info.m_nDecimal, szText, LTOA_MASK_NOTHING_IFZERO);
		strRet = szText;
	}
	return strRet;
}

void CFenshiData::UpdateOneMinuteEmptyData()
{
	m_nRecvDataLen ++;	
	if (m_nRecvDataLen >= m_nCurrentDataLen)
	{
		StopTimer();
		m_nRecvDataLen = m_nCurrentDataLen - 1;
		return;
	}

	m_pPriceValues[m_nRecvDataLen].m_nMessageMask = 0;//SD_FENSHITRACEDATA;
	m_pPriceValues[m_nRecvDataLen].m_lTime = m_nRecvDataLen;
	m_pPriceValues[m_nRecvDataLen].m_lValue = m_pPriceValues[m_nRecvDataLen - 1].m_lValue;
	m_pChengJiaoValues[m_nRecvDataLen] = 0;
	m_pAvePriceValues[m_nRecvDataLen] = m_pAvePriceValues[m_nRecvDataLen - 1];
}

void CFenshiData::SetTechMgr(CTechKLineMgr* pMgr)
{
	m_pMgr = pMgr;
}

void CFenshiData::StartTimer()
{
	StopTimer();
	if (m_pMgr != NULL)
	{
		if (m_nRecvDataLen % m_nOneDayDataNum == 120)		//半天
		{
			m_nTimeID = SetTimer(m_pMgr->GetParentHWnd(), (UINT)this, IDT_HARFHOUR_AUTO, &CFenshiData::MinuteProc);	
		}
		else
			m_nTimeID = SetTimer(m_pMgr->GetParentHWnd(), (UINT)this, IDT_MINUTE_AUTO, &CFenshiData::MinuteProc);
	}
}

void CFenshiData::StopTimer()
{
	if (m_nTimeID > 0 && m_pMgr != NULL)
	{
		KillTimer(m_pMgr->GetParentHWnd(), m_nTimeID);
		m_nTimeID = 0;
	}
}

VOID CALLBACK CFenshiData::MinuteProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CFenshiData* pFenshi = (CFenshiData*)idEvent;
	if (pFenshi != NULL)
	{
		pFenshi->UpdateOneMinuteEmptyData();
	}
}
//////////////////////////////////////////////////////////////////////////
//CTechKLineData
CTechKLineData::CTechKLineData()
{
	memset(this, 0, sizeof(CTechKLineData));
	//	m_afKLineHorizCellWidth = {(float)0.01,1,2,3,4,5,6,7,8,9,10,20,40,50,60,70,80};  // k线缩放比例。
	m_afKLineHorizCellWidth[0] = (float)0.01;
	m_afKLineHorizCellWidth[1] = (float)0.1;
	m_afKLineHorizCellWidth[2] = (float)0.5;
	for (int i = 3; i <= 12; i ++)
	{
		m_afKLineHorizCellWidth[i] = (float)i;
	}
	m_afKLineHorizCellWidth[13] = (float)20;
	m_afKLineHorizCellWidth[14] = (float)40;
	m_afKLineHorizCellWidth[15] = (float)50;
	m_afKLineHorizCellWidth[16] = (float)60;
	m_afKLineHorizCellWidth[17] = (float)70;
	m_afKLineHorizCellWidth[18] = (float)80;
	m_afKLineHorizCellWidth[19] = (float)0.000001;

	m_nWidthIndex = 11;
}

BOOL CTechKLineData::Init(int nSize)
{
	ASSERT(nSize > 0);
	if (m_nCurrentDataLen != nSize && nSize > 0)
	{
		m_pStockDayData = new StockDay[nSize + 1];
		m_nCurrentDataLen = nSize;
		if(m_pStockDayData != NULL)
		{
			memset(m_pStockDayData, 0, sizeof(StockDay) * (nSize + 1));
		}
		return TRUE;
	}
	if (nSize  < m_nCurrentRequestLen - 1)//可能需要滤除当天的
	{
		//		m_bDataFinished = TRUE;
	}
	return FALSE;
}

BOOL CTechKLineData::Refresh(int nNewSize)
{
	if (m_nCurrentDataLen != nNewSize)
	{
		Release();
		return Init(nNewSize);
	}
	else
	{
		memset(m_pStockDayData, 0, sizeof(StockDay) * nNewSize);
		m_nCurrentDataPos = 0;
		InitChuquanDayData(TRUE);
	}
	return TRUE;
}

BOOL CTechKLineData::Release()
{
	m_bDataFinished = FALSE;
	if(m_pStockDayData)
	{
		delete[] m_pStockDayData;
		m_pStockDayData = 0;
	}
	m_nCurrentDataLen = 0;
	m_nCurrentDataPos = 0;
	InitChuquanDayData(TRUE);
	return TRUE;
}

BOOL CTechKLineData::CopyData(StockCompDayDataEx* pData, int nSize)
{
	ASSERT(m_nCurrentDataLen == nSize);
	m_bNeedRefresh = FALSE;
	if(m_pStockDayData != NULL && m_nCurrentDataLen == nSize)
	{
		m_nCurrentDataPos = 0;
		//根据m_pStockCompDayData生成m_pStockDayData数据
		for(int i = 0; i < m_nCurrentDataLen; i++ )
		{
//			memcpy(&m_pStockDayData[i], &pData[i], sizeof(StockCompDayDataEx));
			//参考StockCompDayDataEx 数据结构 hsds_comudata.h;line2155
			memcpy(&m_pStockDayData[i], &pData[i], sizeof(long) * 5);		
			m_pStockDayData[i].m_lMoney = (float)pData[i].m_lMoney;
			m_pStockDayData[i].m_lTotal = (float)pData[i].m_lTotal;
			m_pStockDayData[i].m_lNationalDebtRatio = pData[i].m_lNationalDebtRatio;
		}
		if (nSize  < m_nCurrentRequestLen - 1) //可能需要滤除当天的
		{
//			m_bDataFinished = TRUE;
		}
		return TRUE;
	}
	return FALSE;
}

float CTechKLineData::GetKLineHorizCellWidth()
{
	if (m_nWidthIndex >= 0 && m_nWidthIndex < sizeof(m_afKLineHorizCellWidth) / sizeof(float))
	{
		return m_afKLineHorizCellWidth[m_nWidthIndex];
	}
	return 0;
}

float CTechKLineData::GetNextWidth(BOOL bNext)
{
	int nOldWidthIndex = m_nWidthIndex;
	if (bNext)
	{
		m_nWidthIndex ++;
	}
	else 
		m_nWidthIndex --;
	if (m_nWidthIndex < 0)
	{
		m_nWidthIndex = 0;
	}
	if (m_nWidthIndex > 0 && m_afKLineHorizCellWidth[m_nWidthIndex] < 0.001)
	{
		m_nWidthIndex --;
	}
	return GetKLineHorizCellWidth();
}

BOOL CTechKLineData::IsKLineDataFinished(int nReqireLen, int nCurIndex)
{
	if (nReqireLen <= m_nCurrentDataLen)
	{
		return TRUE;
	}
	else
		return m_bDataFinished;
}

long CTechKLineData::GetIndexDate(int nIndex)
{
	if (nIndex < m_nCurrentDataLen)
	{
		return m_pStockDayData[nIndex].m_lDate;
	}
	return 0;
}

BOOL CTechKLineData::SetRefreshFlag(BOOL bNeedRefresh)
{
	BOOL bRet = m_bNeedRefresh;
	m_bNeedRefresh = bNeedRefresh;
	return bRet;
}

int CTechKLineData::ResetPos(int nNewLen)
{
	int nRet = m_nCurrentDataPos;
	if (nNewLen <= m_nCurrentDataLen)
	{
		m_nCurrentDataPos = m_nCurrentDataLen - nNewLen;
	}
	return nRet;
}

StockDay*	CTechKLineData::GetStockDayData()
{
	StockDay* pRet = NULL;
	if (m_nCurrentDataPos < m_nCurrentDataLen)
	{
		pRet = m_pStockDayData + m_nCurrentDataPos;
	}
	return pRet;
}

int CTechKLineData::GetStockCompDayDataLen()
{
	int nRet = m_nCurrentDataLen;
	if (m_nCurrentDataPos < m_nCurrentDataLen)
	{
		nRet = m_nCurrentDataLen - m_nCurrentDataPos;
	}
	return nRet;
}

long CTechKLineData::GetChuquanMask()
{
	return m_lChuquanMask;
}

BOOL CTechKLineData::CalcChuQuan(int nDayType, CArray<ChuQuanData*,ChuQuanData*>& ayChuQuanData, 
								long lMask, long nPriceUnit)
{
	long lOldMask = m_lChuquanMask;
	if (m_nCurrentDataLen > 0 && m_pStockDayData != NULL)
	{
		//分钟线也除权
		unsigned long lDate = 0 ;
		if ( m_pStockDayData->m_lDate > 99999999 ) //分钟线
		{
			lDate = m_pStockDayData->m_lDate / 10000;
			long lYear = lDate / 10000 - 10 + 2000 ;
			lDate = lYear * 10000 + lDate % 10000;
		}
		BOOL bFirstDayPower = FALSE;  //本周期第一天是否是除权日
		BOOL bMoreDays = (nDayType >= PERIOD_TYPE_WEEK && nDayType <= PERIOD_TYPE_MONTH)?TRUE:FALSE;
		if (bMoreDays == TRUE)
		{
			if (m_pStockDayData->m_lOpenPrice > FLOATZERO)
			{
				bFirstDayPower = (m_pStockDayData->m_lPrevClose/m_pStockDayData->m_lOpenPrice > 1.051)?TRUE:FALSE;
			}
		}
		// 除权
		ChuQuanData* pData = NULL;
		if ( lMask & HsTechLine_Left_ChuQuan )	 // 向前除权
		{
			if(!(m_lChuquanMask & HsTechLine_Left_ChuQuan ))	//除权操作
			{
				InitChuquanDayData(FALSE);
				m_lChuquanMask = lMask;
				for(int nIndex = 0; nIndex < m_nCurrentDataLen; nIndex ++)
				{
					StockDay* pDay = m_pStockDayDataChuquan + nIndex;
					long lPreDate = pDay->m_lDate;

					for(int i = 0; i < ayChuQuanData.GetSize(); i++)
					{
						pData = ayChuQuanData.GetAt(i);
						if( pData != NULL )
						{
							if (bMoreDays == TRUE)
							{
								if (nDayType == PERIOD_TYPE_WEEK)
								{
									if (GetSpanDays(lPreDate,pData->m_lDate) == 3)   //当周除权日期和上周只差两天则判断为本周第一天
										bFirstDayPower = TRUE;
								}
							}
							if( (pData->m_cMask == ChuQuanData::Calc) && ( (pDay->m_lDate < pData->m_lDate) || ( ( lDate > 0 ) && (lDate < pData->m_lDate) )) )
							{
								pDay->m_lOpenPrice  = ((pDay->m_lOpenPrice/(double)nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
								pDay->m_lMaxPrice   = ((pDay->m_lMaxPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
								pDay->m_lMinPrice   = ((pDay->m_lMinPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
								pDay->m_lClosePrice = ((pDay->m_lClosePrice/(double)nPriceUnit + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
								pDay->m_lTotal	   = (long)(pDay->m_lTotal*pData->m_fMulit);
								pDay->m_lMoney	   = (long)(pDay->m_lMoney*pData->m_fMulit);
							}
							else if((bMoreDays == TRUE) && (bFirstDayPower == FALSE) &&(pData->m_cMask == ChuQuanData::Calc) && (pDay->m_lDate >= pData->m_lDate) && (lPreDate < pData->m_lDate))
							{
								pDay->m_lOpenPrice  = ((pDay->m_lOpenPrice/(double)nPriceUnit  + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
								pDay->m_lMaxPrice   = ((pDay->m_lMaxPrice/(double)nPriceUnit   + pData->m_fAdd) / pData->m_fMulit)*nPriceUnit;
							}
						}
					}
				}
			}
			else	//复权
			{
				m_lChuquanMask = 0;
				InitChuquanDayData(TRUE);
			}
		}
		else if( lMask & HsTechLine_Back_ChuQuan ) // 向后除权
		{
			if(!(m_lChuquanMask & HsTechLine_Back_ChuQuan ))
			{
				InitChuquanDayData(FALSE);
				m_lChuquanMask = lMask;
				for(int nIndex = 0; nIndex < m_nCurrentDataLen; nIndex ++)
				{
					StockDay* pDay = m_pStockDayDataChuquan + nIndex;
					long lPreDate = pDay->m_lDate;

					for(int i = 0; i < ayChuQuanData.GetSize(); i++)
					{
						pData = ayChuQuanData.GetAt(i);
						if(pData != NULL)
						{
							if (bMoreDays == TRUE)
							{
								if (nDayType == PERIOD_TYPE_WEEK)
								{
									if (GetSpanDays(lPreDate,pData->m_lDate) == 3)   //当周除权日期和上周只差两天则判断为本周第一天
										bFirstDayPower = TRUE;
								}
							}
							if( (pData->m_cMask == ChuQuanData::Calc) && ( ( ( pDay->m_lDate >= pData->m_lDate ) && ( lDate == 0 ) ) ||  ( lDate >= pData->m_lDate ) ) )
							{
								if((bMoreDays == TRUE) && (bFirstDayPower == FALSE) 
									&& (pDay->m_lDate >= pData->m_lDate) && (lPreDate < pData->m_lDate))
								{
									pDay->m_lOpenPrice  = pDay->m_lOpenPrice;
									pDay->m_lMaxPrice   = pDay->m_lMaxPrice;
								}
								else
								{
									pDay->m_lOpenPrice  = ((pDay->m_lOpenPrice/(double)nPriceUnit  * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
									pDay->m_lMaxPrice   = ((pDay->m_lMaxPrice/(double)nPriceUnit   * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
								}
								pDay->m_lMinPrice   = ((pDay->m_lMinPrice/(double)nPriceUnit   * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;
								pDay->m_lClosePrice = ((pDay->m_lClosePrice/(double)nPriceUnit * pData->m_fMulit) - pData->m_fPGBL * pData->m_fPGJ + pData->m_fXJHL)*nPriceUnit;

								pDay->m_lTotal	   = (long)(pDay->m_lTotal*pData->m_fMulit);
								pDay->m_lMoney	   = (long)(pDay->m_lMoney*pData->m_fMulit);
							}
						}
					}
				}
			}
			else
			{
				InitChuquanDayData(TRUE);
				m_lChuquanMask = 0;
			}
		}
		else
		{
			InitChuquanDayData(TRUE);
			m_lChuquanMask = 0;
		}
	}
	return (m_lChuquanMask != lOldMask);
}

long  CTechKLineData::GetSpanDays(long lBgnDate,long lEndDate)
{
	long lSpanDays = 0;
	CTime tBgnDate = CGeneralHelper::LongToDate(lBgnDate);
	CTime tEndDate = CGeneralHelper::LongToDate(lEndDate);
	CTimeSpan tsSpan = tEndDate - tBgnDate;
	lSpanDays = tsSpan.GetDays();
	return lSpanDays;
}

BOOL CTechKLineData::InitChuquanDayData(BOOL bRelease)
{
	if(m_pStockDayDataChuquan != NULL)
	{
		delete m_pStockDayDataChuquan;
		m_pStockDayDataChuquan = NULL;
		m_lChuquanMask = 0;
	}
	if (!bRelease)
	{
		m_pStockDayDataChuquan = new StockDay[m_nCurrentDataLen + 1];
		memcpy(m_pStockDayDataChuquan, m_pStockDayData, sizeof(StockDay) * (m_nCurrentDataLen + 1));
	}
	return TRUE;
}

StockDay* CTechKLineData::GetCurrentDrawData()
{
	if (m_lChuquanMask != 0)
	{
		return m_pStockDayDataChuquan;
	}
	return m_pStockDayData;
}
//////////////////////////////////////////////////////////////////////////
//CManagerBase
CManagerBase::CManagerBase()
{
	m_nAxesType = CGraphics::CAxesType::XY;
}

CManagerBase::~CManagerBase()
{
	m_dataFenshi.Release();
	m_dataKLine.Release();
}

int	CManagerBase::GetStockDayData(StockDay*& pStockDaya)
{
	pStockDaya = m_dataKLine.m_pStockDayData;
	return m_lDataVauleLen;
}

int	 CManagerBase::GetStockDay(StockDay*& pStockDay)
{
	pStockDay = m_dataKLine.m_pStockDayData;
	return m_lDataVauleLen;
}

int	CManagerBase::GetStockFenShiData(DrawFenShiData*& pDrawFenshiData)
{
	pDrawFenshiData = m_dataFenshi.m_pPriceValuesCur;
	return m_lDataVauleLen;
}

int	CManagerBase::GetStockChengJiaoValues(long*& pChengJiaoValues)
{
	pChengJiaoValues = m_dataFenshi.m_pChengJiaoValuesCur;
	return m_lDataVauleLen;
}

int	CManagerBase::GetStockAveFenshiPrice(long*& pAvePriceValues)
{
	pAvePriceValues = m_dataFenshi.m_pAvePriceValuesCur;
	return m_lDataVauleLen;
}

BOOL CManagerBase::GetStartEndPos(int& nStartPos, int& nEndPos)
{
	nStartPos = m_nStartPos;
	nEndPos = m_nEndPos;
	return TRUE;
}

IFormulaMan* CManagerBase::GetFormulaMan()
{
	return m_pFormulaMan;
}

CString	CManagerBase::GetTimeByPos(int nCursorPos,CString& strText,int nType/* = 0*/)
{
	strText.Empty();

	if(nCursorPos < 0 )
		return "";

	int nRetPos = m_dataFenshi.GetPosTime(nCursorPos, strText);
	if (nRetPos >= 0)
	{
		int nCurrentPos = nCursorPos - nRetPos;//m_dataFenshi.m_nOneDayDataNum * nRet;
		if (nType == 5)
		{
			if (nCursorPos == 240 && m_dataFenshi.m_nRecvDataLen == nCursorPos + 1)
			{
			}
			else
				nCurrentPos = nCursorPos % 240;
			nType = 3;
		}
		if (nCurrentPos >= 0 && m_lDate > 0 )
		{
			int nSize = sizeof(m_stockType.m_union.m_nTimes) / sizeof(HSTypeTime) + 2;

			HSTypeTime* pTypeTime = (HSTypeTime*)&m_stockType.m_union.m_nAheadOpenTime;
			if( nType == 4 && ( nCurrentPos > m_stockType.m_nTotalTime || nCurrentPos > m_stockType.m_nTotalTime - 10 ) )
				nCurrentPos = m_stockType.m_nTotalTime;

			int nCount = 0;
			int nPreCount = 0;

			for( int i = 0; i < nSize; i++)
			{
				nCount += nPreCount;
				if( nCurrentPos /*- 1*/ <= (pTypeTime[i].m_nCloseTime - pTypeTime[i].m_nOpenTime + nCount) )
				{
					nCurrentPos -= nCount;
					nCurrentPos  = pTypeTime[i].m_nOpenTime + nCurrentPos;

					CTime tm = CTime( m_lDate / 10000, 
						m_lDate % 10000 / 100,
						m_lDate % 10000 % 100 , 
						nCurrentPos/60, nCurrentPos%60, 0 );

					if( nType == 4 || nType == 3 ) // 分时底部显示的时间
					{
						strText.Format("%02d:%02d", tm.GetHour(), tm.GetMinute());
					}
					else if( nType == 1 ) // 鼠标底部显示的时间
					{
						strText.Format( "%02d/%02d-%02d:%02d", tm.GetMonth(), tm.GetDay(), 
							tm.GetHour(), tm.GetMinute());
					}
					else // 弹出显示时间
					{
						strText.Format( "%02d%02d%02d%02d", tm.GetMonth(), tm.GetDay(), 
							tm.GetHour(), tm.GetMinute());
					}
					break;
				}
				nPreCount = pTypeTime[i].m_nCloseTime - pTypeTime[i].m_nOpenTime;
			}
		}
	}
	return strText;
}

long CManagerBase::GetFenshiPrevClose()
{
	return m_dataFenshi.GetPrevClose();
}

CGraphics::CAxesType CManagerBase::GetAxesType()
{
	return m_nAxesType;
}

int	CManagerBase::DelayDeleteTechLine(CTechKLineImp* pTechLine)
{
	return 0;
}