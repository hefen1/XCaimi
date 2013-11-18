#include "stdafx.h"
#include "DrawTStockTrend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_TREND_YSPLIT_MAX	14
#define DEFAULT_TREND_XSPLIT		8
#define DEFAULT_TREND_YSPLIT_STEP	30	

//////////////////////////////////////////////////////////////////////////
CDrawTStockTrend::CDrawTStockTrend(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth) : CTechKLineImp(pMgr, pRegion, fHorizCellWidth)
{
	m_dwLinearStyle = AXIS_FENSHIPRICE | AXIS_FIRSTTEXT |AXIS_LASTTEXT | AXIS_SOLIDANDDASH | AXIS_LEFT | AXIS_RIGHT | AXIS_FENSHIHORI 
		| AXIS_LEFTMARGIN | AXIS_TOPMARGIN | AXIS_RIGHTMARGIN;
	if (m_pLinear != NULL)
	{
		m_pLinear->SetAxesStyle(m_dwLinearStyle);
	}
	m_nVertCellHeight = 0;
	m_bFenshiDrawType = TRUE;
	m_nHoriCellNum = 241;	//
	m_nSplitNums = 2;
}

CDrawTStockTrend::~CDrawTStockTrend()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CDrawTStockTrend::SetRect(CRect rc)
{
	if (rc.IsRectEmpty() )
	{
		return FALSE;
	}
	m_rect = rc;
	m_rcGird = m_rect;

	RecalcRect(m_rcGird);
	m_pLinear->SetXRefParam(0, 0, 0, 0, 30);
	m_pLinear->SetXNumOfTicks(DEFAULT_TREND_XSPLIT * (m_nHoriCellNum / 240));

	m_nSplitNums = rc.Height() / 2 / DEFAULT_TREND_YSPLIT_STEP;
	m_nSplitNums *= 2;
	if (m_nSplitNums > DEFAULT_TREND_YSPLIT_MAX)
	{
		m_nSplitNums = DEFAULT_TREND_YSPLIT_MAX;
	}
	if (m_nSplitNums <= 0)
	{
		m_nSplitNums = 2;
	}

	m_pLinear->SetYNumOfTicks(m_nSplitNums);

	return TRUE;
}

void CDrawTStockTrend::SetHorizCellNum(int nHorizCellNum)
{
	m_nHoriCellNum = nHorizCellNum;
	SetRect(m_rect);
}

BOOL CDrawTStockTrend::RecalcRect(CRect rcArea)
{
	if (m_pLinear == NULL || m_pTeckKLineMgr == NULL || m_pRegion == NULL)
	{
		return FALSE;
	}
	if (m_pPointPos != NULL)
	{
		delete []m_pPointPos;
		m_pPointPos = NULL;
	}

	CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();
	CClientDC dc(pWnd);

	CRect rcTemp;
	rcTemp.left = m_pTeckKLineMgr->GetStockUserInfo()->m_ciStockCode.m_cCodeType;
	CRect* pRect = m_pLinear->GetMargin(rcTemp, PKLINE_TYPE_FENSHI, &dc);

	m_nVertCellHeight = rcTemp.top;

	CRect rcDraw = rcArea;
	rcDraw.left   += pRect->left;
	rcDraw.right  -= pRect->right;
	rcDraw.top    += pRect->top;
	rcDraw.bottom -= pRect->bottom;

	if (rcDraw.IsRectEmpty())
	{
		return FALSE;
	}
	double fStep = (double)(rcDraw.Width()) / (m_nHoriCellNum - 1);
	m_fHoriCellWidth = (float)fStep;

	m_pPointPos = new DrawPointInfo[m_nHoriCellNum + 1];
	m_pPointPos[0] = (float)rcDraw.left;
	m_pPointPos[0] = 0;
	for (int i = 0; i < m_nHoriCellNum; i++)
	{
		m_pPointPos[i + 1] = float(m_pPointPos[i]) + m_fHoriCellWidth;
		m_pPointPos[i + 1] = i + 1;
	}

	m_pLinear->SetRange(0, 0, rcDraw.Width(), rcDraw.Height());
	m_pLinear->RecalcRects(rcArea);

	return TRUE;
}

void CDrawTStockTrend::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return;
	}
	DrawFenShiData* pDrawFenshiData = NULL;
	int nDataLen = m_pTeckKLineMgr->GetStockFenShiData(pDrawFenshiData);
	//if(nDataLen <= 0 || pDrawFenshiData == NULL )
	//{
	//	return;
	//}
	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = hx_DefMinValues; //pLine->m_lMinValue;
	if (pDrawFenshiData != NULL)
	{
		DrawFenShiData* pFenShiDataCur = pDrawFenshiData;
		for (int i = 0; i < nDataLen; i ++)
		{
			DrawFenShiData* pData = pFenShiDataCur;
			if (pData != NULL && pData->m_lValue > 0 )
			{
				if((dblMin > pData->m_lValue || dblMin == -1 ))
				{
					dblMin = pData->m_lValue;
				}
				if (dblMax < pData->m_lValue)
				{
					dblMax = pData->m_lValue;
				}
			}
			pFenShiDataCur ++;
		}
	}
	HSDouble dblMaxDefault, dblMinDefault;
	long lOpenTemp = m_pTeckKLineMgr->GetFenshiPrevClose();
	StockUserInfo* pStock = m_pTeckKLineMgr->GetStockUserInfo();
	if (pStock != NULL)
	{
		dblMaxDefault = lOpenTemp + m_nSplitNums / 2 * pStock->m_nPriceUnit / pow((double)10, (double)pStock->m_nDecimal);
		dblMinDefault = lOpenTemp - m_nSplitNums / 2 * pStock->m_nPriceUnit / pow((double)10, (double)pStock->m_nDecimal);
	}
	if (dblMax == hx_DefMaxValues || dblMin == hx_DefMinValues)
	{
		dblMax = dblMaxDefault;
		dblMin = dblMinDefault;
	}
	if (dblMax < dblMaxDefault)
	{
		dblMax = dblMaxDefault;
	}
	if (dblMin > dblMinDefault)
	{
		dblMin = dblMinDefault;
	}

	HSDouble dblTemp = max(abs(dblMax.m_dData - lOpenTemp), abs(lOpenTemp - dblMin.m_dData));
	dMax = lOpenTemp + dblTemp;
	dMin = lOpenTemp - dblTemp;
	dOpenPrice = lOpenTemp;
}

int	CDrawTStockTrend::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return -1;
	}
	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}

	DrawFenShiData* pDrawFenshiData = NULL;
	long* pAvePriceVaules = NULL;
	int nDataLen = m_pTeckKLineMgr->GetStockFenShiData(pDrawFenshiData);
	nDataLen = m_pTeckKLineMgr->GetStockAveFenshiPrice(pAvePriceVaules);
	CHSBaseDC dc(pDC);
	m_pLinear->BeginDraw(&dc);

	HSDouble dMax, dMin, dOpen;
//	m_pTeckKLineMgr->GetMaxMinData(dMax, dMin, dOpen);
	GetMaxMinData(dMax, dMin, dOpen);
	float fVerCellHeight = m_rcGird.Height();
	float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;
	ASSERT(dOpen.m_dData != 0);
	StockUserInfo* pStock = m_pTeckKLineMgr->GetStockUserInfo();
	int nDecimal = 2;
	if (pStock != NULL)
	{
		nDecimal = pStock->m_nDecimal;
	}
	m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, dOpen.m_dData / 1000, 1, nDecimal, 1);
	m_pLinear->Grid();
	m_pLinear->Axes();

	if(nDataLen > 0 && pDrawFenshiData != NULL && pAvePriceVaules != NULL)
	{
		m_pLinear->DrawCurve(CGraphics::NONE,
			CTValues<DrawPointInfo>(m_pPointPos, 0),
			CTValues<DrawFenShiData>((DrawFenShiData*)&pDrawFenshiData[0], nDataLen),
			dMax.m_dData , dMin.m_dData,
			g_hsGraphicsData.clJsPriceLine);

		m_pLinear->DrawCurve(CGraphics::NONE,
			CTValues<DrawPointInfo>(m_pPointPos, 0),
			CTValues<long>((long*)&pAvePriceVaules[0], nDataLen),
			dMax.m_dData , dMin.m_dData,
			g_hsGraphicsData.clJsAvgLine);
	}

	m_pTeckKLineMgr->DrawTitle(m_pLinear);
	m_pLinear->EndDraw(&dc);
	
	return 0;
}

BOOL CDrawTStockTrend::IsMainLine()
{
	return TRUE;
}

int CDrawTStockTrend::GetDataYPos(int nDataIndex)
{
	return 0;
}

int CDrawTStockTrend::GetMaxHorizCellNum()
{
	return m_nHoriCellNum;
}

short CDrawTStockTrend::GetLineType()
{
	return etechLineType_main;
}
//////////////////////////////////////////////////////////////////////////
//CDrawTStockTrendVolume
CDrawTStockTrendVolume::CDrawTStockTrendVolume(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth) : CDrawTStockTrend(pMgr, pRegion, fHorizCellWidth)
{
	m_dwLinearStyle = AXIS_BOTTOMMARGIN | AXIS_RIGHTMARGIN | AXIS_LEFTMARGIN | AXIS_FENSHIHORI | AXIS_SOLIDANDDASH | AXIS_BOTTOM | AXIS_LEFT;
	if (m_pLinear != NULL)
	{	
		m_pLinear->SetAxesStyle(m_dwLinearStyle);
	}
	m_nHoriCellNum = 241; 
}

CDrawTStockTrendVolume::~CDrawTStockTrendVolume()
{

}

//////////////////////////////////////////////////////////////////////////
BOOL CDrawTStockTrendVolume::SetRect(CRect rc)
{
	m_rect = rc;
	m_rcGird = m_rect;
	RecalcRect(m_rcGird);
	m_pLinear->SetXRefParam(0, 0, 0, 0, 30);
	m_pLinear->SetXNumOfTicks(DEFAULT_TREND_XSPLIT * (m_nHoriCellNum / 240));
	int nSplit = rc.Height() / DEFAULT_TREND_YSPLIT_STEP;
	if (nSplit > DEFAULT_TREND_YSPLIT_MAX)
	{
		nSplit = DEFAULT_TREND_YSPLIT_MAX;
	}
	if (nSplit <= 0)
	{
		nSplit = 1;
	}
	m_pLinear->SetYNumOfTicks(nSplit);
	return TRUE;
}

int	CDrawTStockTrendVolume::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return -1;
	}
	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}
	DrawFenShiData* pDrawFenshiData = NULL;
	long* pChengjiaoValues = NULL;
	int nDataLen = m_pTeckKLineMgr->GetStockFenShiData(pDrawFenshiData);
	m_pTeckKLineMgr->GetStockChengJiaoValues(pChengjiaoValues);
	HSDouble dMax, dMin, dOpen;
	GetMaxMinData(dMax, dMin, dOpen);
	float fVerCellHeight = m_rcGird.Height();
	float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;
	m_pLinear->SetYRefParam(dMin.m_dData, dMax.m_dData, 0, 0, 100, 0, 1);

	CHSBaseDC dc(pDC);
	m_pLinear->BeginDraw(&dc);
	m_pLinear->Grid();
	m_pLinear->Axes();

	if(nDataLen > 0 && pDrawFenshiData != NULL && pChengjiaoValues != NULL)
	{
		m_pLinear->Bar(CGraphics::NONE,
			CTValues<DrawPointInfo>(m_pPointPos, 0),
			CTValues<DrawFenShiData>(pDrawFenshiData, nDataLen),
			CTValues<long>((long*)&(pChengjiaoValues[0]), nDataLen),
			dMax.m_dData, 0,
			g_hsGraphicsData.clUp, g_hsGraphicsData.clDown,
			g_hsGraphicsData.clUp.m_lColor);
	}


	m_pLinear->EndDraw(&dc);
	return 0;
}

void CDrawTStockTrendVolume::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return;
	}

	long* pChengjiaoValues = NULL;
	int nDataLen = m_pTeckKLineMgr->GetStockChengJiaoValues(pChengjiaoValues);
	//if(nDataLen <= 0 || pChengjiaoValues == NULL)
	//{
	//	return;
	//}

	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = 0; 
	if (pChengjiaoValues != NULL)
	{
		long* pChengjiaoVauleCur = pChengjiaoValues;
		for (int i = 0; i < nDataLen; i ++)
		{
			long* pData = pChengjiaoVauleCur;
			if (pData != NULL && *pData > 0 )
			{
				if (dblMax < *pData || dblMax == hx_DefMaxValues)
				{
					dblMax = *pData;
				}
			}
			pChengjiaoVauleCur ++;
		}
	}
	dMax = dblMax;
	dMin = dblMin;
	if (hx_DefMaxValues == dMax)
	{
		dMax = 0;
	}
}

BOOL CDrawTStockTrendVolume::IsMainLine()
{
	return FALSE;
}

short CDrawTStockTrendVolume::GetLineType()
{
	return etechLineType_tech;
}

CString	CDrawTStockTrendVolume::GetTeckKLineName()
{
	return m_strName;
}
//////////////////////////////////////////////////////////////////////////
//CDrawTStockTrendAdd
CDrawTStockTrendAdd::CDrawTStockTrendAdd(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth) 
	: CDrawTStockTrend(pMgr, pRegion, fHorizCellWidth)
{
	m_pColor = g_hsGraphicsData.GetDefTechColor(0);
}

CDrawTStockTrendAdd::~CDrawTStockTrendAdd()
{
	m_dataFenshi.Release();
}

BOOL CDrawTStockTrendAdd::GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, int nPeriod, int nReqDays)
{
	if (pReqCollector == NULL )
	{
		return FALSE;
	}
	if(pStockInfo != NULL)
	{
		if( !m_stkInfo.m_ciStockCode.CompareCode(&pStockInfo->m_ciStockCode) )
		{//保存请求的股票信息
			m_stkInfo.Copy(pStockInfo);
		}
	}
	else if (m_stkInfo.GetCode() == "" )
	{
		return FALSE;
	}
	m_dataFenshi.m_nLastReqDay = nReqDays;
	if (m_dataFenshi.m_nLastReqDay >= 1 && m_dataFenshi.m_nLastRecvDay == 0) 
	{
		int lAskLen = sizeof(AskData) + sizeof(CodeInfo);
		AskData* pskData = (AskData*) new char[lAskLen];
		memset(pskData,0,lAskLen);

		pskData->m_nType  = RT_TREND_EXT;
		pskData->m_nSize  = 1;

		memcpy(pskData->m_pCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
		memcpy(&pskData->m_nPrivateKey, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));

		pReqCollector->AddRequest((char*)pskData,lAskLen);
		delete []pskData;

		lAskLen = sizeof(AskData);
		char* pData = new char[lAskLen]; 
		AskData*  pAskData = (AskData*)pData;
		memset(pAskData, 0, lAskLen);
		pAskData->m_nType  = RT_REALTIME_EXT;
		pAskData->m_nSize  = 1;
		memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
		pReqCollector->AddRequest((char*)pAskData,lAskLen);

		pAskData->m_nType  = RT_AUTOPUSH_EXT;
		pReqCollector->AddRequest((char*)pAskData,lAskLen);

		m_dataFenshi.Init(pStockInfo, nPeriod, FALSE);
	}
	if(m_dataFenshi.m_nLastReqDay > 1 && m_dataFenshi.m_nLastReqDay > m_dataFenshi.m_nLastRecvDay)
	{
		// 计算长度	
		int nSize = sizeof(ReqHisTrend) - sizeof(CodeInfo);
		if(nSize % sizeof(CodeInfo))
		{
			nSize += sizeof(CodeInfo);
		}
		int nAskLen = sizeof(AskData) + nSize;
		int nStart = m_dataFenshi.m_nLastRecvDay;
		if (nStart <= 0)
		{
			nStart = 1;
		}
		for (int i = nStart; i < m_dataFenshi.m_nLastReqDay; i ++)
		{
			AskData* pakData       = (AskData*)new char[nAskLen];
			memset(pakData, 0, nAskLen);
			pakData->m_nType	   = RT_HISTREND;
			pakData->m_nSize	   = (nSize / sizeof(CodeInfo)) + 1;
			// 设置请求包数据
			ReqHisTrend* pReqHisTrend = (ReqHisTrend*)pakData->m_pCode;
			if( pStockInfo != NULL )
			{
				memcpy(&pReqHisTrend->m_ciStockCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
			}
			else
			{
				memcpy(&pReqHisTrend->m_ciStockCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));
			}
			pReqHisTrend->m_lDate = - i;	
			pReqCollector->AddRequest((char*)pakData, nAskLen);
			delete []pakData;
		}
	}
	return TRUE;
}

BOOL CDrawTStockTrendAdd::IsMainLine()
{
	return FALSE;
}

short CDrawTStockTrendAdd::GetLineType()
{
	return etechLineType_stock;
}

int	CDrawTStockTrendAdd::SetColor(ColorProperty* pColor)
{
	m_pColor = pColor;
	return m_pColor->m_lColor;
}

int	CDrawTStockTrendAdd::OnUpdateData(DataHead* pData)
{
	if (RT_TREND_EXT == pData->m_nType)
	{
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return -1;
		}
		AnsTrendData_Ext *pTrenad = (AnsTrendData_Ext *)pData;
		m_dataFenshi.UpdataTreadEx(pTrenad);
	}
	else if (RT_AUTOPUSH_EXT == pData->m_nType)
	{
		AnsHSAutoPushData_Ext *pEverytimeData = (AnsHSAutoPushData_Ext *)pData;
		CommRealTimeData_Ext *pCommRealTimeData = NULL;
		CommRealTimeData_Ext* pRealTimeData = pEverytimeData->m_pstData;
		if (pRealTimeData != NULL && memcmp(&m_stkInfo.m_ciStockCode, &pRealTimeData->m_ciStockCode, sizeof(CodeInfo)) != 0)
		{
			return -1;
		}
		int nNowSize = 0;
		for (int i = 0; i < pEverytimeData->m_nSize; i ++)
		{
			CommRealTimeData_Ext* pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);;
			if(memcmp(&pRealTimeData->m_ciStockCode, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo)) == 0)
			{
				pCommRealTimeData = pRealTimeData;
				break;
			}
			pRealTimeData = pNext;
		}
		if (pCommRealTimeData != NULL)
		{
			m_dataFenshi.UpdateRealTimeData(pCommRealTimeData);
		}
	}
	else if (RT_HISTREND == pData->m_nType)
	{
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return -1;
		}
		AnsHisTrend * pTendData = (AnsHisTrend *)pData;
		int nDataValLen = 0;
		int nTrendNum = m_dataFenshi.UpdataHisTrend(pTendData, nDataValLen);
	}
	return 0;
}

int	CDrawTStockTrendAdd::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return -1;
	}
	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}

	DrawFenShiData* pDrawFenshiData = m_dataFenshi.m_pPriceValuesCur;
	int nDataLen = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
	if(nDataLen <= 0 || pDrawFenshiData == NULL )
	{
		return -1;
	}
	CHSBaseDC dc(pDC);
	m_pLinear->BeginDraw(&dc);

	HSDouble dMax, dMin, dOpen;
//	m_pTeckKLineMgr->GetMaxMinData(dMax, dMin, dOpen);
	GetMaxMinData(dMax, dMin, dOpen);
	float fVerCellHeight = m_rcGird.Height();
	float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;
	ASSERT(dOpen.m_dData != 0);

	m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, dOpen.m_dData / 1000, 1,2, 1);

	m_pLinear->DrawCurve(CGraphics::NONE,
		CTValues<DrawPointInfo>(m_pPointPos, 0),
		CTValues<DrawFenShiData>((DrawFenShiData*)&pDrawFenshiData[0], nDataLen),
		dMax.m_dData , dMin.m_dData,
		*m_pColor);

	m_pLinear->EndDraw(&dc);

	return 0;

}

void CDrawTStockTrendAdd::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return;
	}
	DrawFenShiData* pDrawFenshiData = m_dataFenshi.m_pPriceValuesCur;
	int nDataLen = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
	if(nDataLen <= 0 || pDrawFenshiData == NULL )
	{
		return;
	}
	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = hx_DefMinValues; //pLine->m_lMinValue;
	DrawFenShiData* pFenShiDataCur = pDrawFenshiData;
	for (int i = 0; i < nDataLen; i ++)
	{
		DrawFenShiData* pData = pFenShiDataCur;
		if (pData != NULL && pData->m_lValue > 0 )
		{
			if((dblMin > pData->m_lValue || dblMin == -1 ))
			{
				dblMin = pData->m_lValue;
			}
			if (dblMax < pData->m_lValue)
			{
				dblMax = pData->m_lValue;
			}
		}
		pFenShiDataCur ++;
	}
	long lOpenTemp = m_dataFenshi.GetPrevClose();
	HSDouble dblTemp = max(abs(dblMax.m_dData - lOpenTemp), abs(lOpenTemp - dblMin.m_dData));
	dMax = lOpenTemp + dblTemp;
	dMin = lOpenTemp - dblTemp;
	dOpenPrice = lOpenTemp;
}

BOOL CDrawTStockTrendAdd::GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex)
{
	if(m_pColor != NULL)
	{
		ayTitle.Add( new CToptextType<double>(m_stkInfo.GetName() + " ", 0, m_pColor->m_lColor, this, 0, 0, 1));
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawTStockTrendAdd::UpdateDataPos(int nReqDay, int nDataLen)
{
	return m_dataFenshi.UpdateFenshiDataPos(nReqDay, nDataLen);
}

BOOL CDrawTStockTrendAdd::CheckRecvComplete()
{
	return (m_dataFenshi.m_nLastReqDay <= m_dataFenshi.m_nLastRecvDay);
}