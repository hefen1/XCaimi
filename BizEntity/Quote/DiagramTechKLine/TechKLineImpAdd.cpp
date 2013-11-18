#include "stdafx.h"
#include "TechKLineImpAdd.h"
#include "GraphicsData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
CTechKLineImpAdd::CTechKLineImpAdd(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth) : CTechKLineImp(pMgr, pRegion, fHorizCellWidth)
{
	m_pColor = g_hsGraphicsData.GetDefTechColor(0);
}

CTechKLineImpAdd::~CTechKLineImpAdd()
{
	m_dataKLine.Release();
}


BOOL CTechKLineImpAdd::GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, int nPeriod, WORD wPeriodType)
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
	memcpy(pAskData->m_pCode, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo));
	memcpy(&pAskData->m_nPrivateKey, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo));

	ReqDayData* pRequest = (ReqDayData*)pAskData->m_pCode;
	pRequest->m_nDay = nPeriod;
	pRequest->m_lBeginPosition = 0;
	memcpy(&pRequest->m_ciCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));
	pRequest->m_nPeriodNum = 1;
	pRequest->m_cPeriod = wPeriodType;
	pRequest->m_nSize = 0;
	pAskData->m_lKey = wPeriodType;

	pReqCollector->AddRequest((char*)pAskData,lAskLen);

	delete[] pData;

	return TRUE;
}

int	CTechKLineImpAdd::OnUpdateData(DataHead* pData)
{
	if(pData->m_nType == RT_TECHDATA_EX )
	{
		AnsDayDataEx* pDayData = (AnsDayDataEx*)pData;
		if (pDayData->m_nSize <= 0 || memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return -1;
		}
		StockCompDayDataEx* pDataRecv = pDayData->m_sdData;
		int nCurrentDataLen = pDayData->m_nSize;
		m_lDataVauleLen = nCurrentDataLen;
		m_dataKLine.Refresh(nCurrentDataLen);

		if(!m_dataKLine.CopyData(pDataRecv, nCurrentDataLen))		
			return -1;
	}
	return 0;
}

int	CTechKLineImpAdd::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr == NULL || m_pLinear == NULL)
	{
		return -1;
	}

	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}

	StockDay* pStockDay = m_dataKLine.m_pStockDayData;
	int nStockDayLen = m_dataKLine.m_nCurrentDataLen;
	if (pStockDay == NULL || nStockDayLen <= 0 )
	{
		return -1;
	}

	if (!m_rcGird.IsRectEmpty() )
	{
		CHSBaseDC dc(pDC);
		m_pLinear->BeginDraw(&dc);

		m_pLinear->SetXNumOfTicks(0);
		m_pLinear->SetYNumOfTicks(8);
		HSDouble dMax, dMin, dOpen;	
		m_pTeckKLineMgr->GetMaxMinData(dMax, dMin, dOpen);
		float fVerCellHeight = m_rcGird.Height();
		float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;

		m_pLinear->SetYRefParam(dMin.m_dData , dMax.m_dData , fUnitValue, 0, 1000, 2, 1);

		int nStart = 0, nEnd = 0;//nStockCompDayLen - m_nHoriCellNum;
		m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
		if (nEnd > nStockDayLen)
		{
			nEnd = nStockDayLen;
		}
		m_pLinear->DrawKLine(&m_sChartParam,
			CTValues<DrawPointInfo>(m_pPointPos, m_nHoriCellNum),
			CTValues<StockDay>(pStockDay, nStart, (nEnd - nStart)),
			m_fKLineWidth, //　横向线与线的间距
			*m_pColor, *m_pColor, g_hsGraphicsData.clDayUpFillLine, *m_pColor);

		m_pLinear->EndDraw(&dc);
	}
	return 0;
}

void CTechKLineImpAdd::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = hx_DefMinValues; //pLine->m_lMinValue;
	dOpenPrice = 0;

	StockDay* pStockDay = m_dataKLine.m_pStockDayData;
	int nStockDayLen = m_dataKLine.m_nCurrentDataLen;
	if (pStockDay == NULL || nStockDayLen <= 0 )
	{
		return ;
	}
	int nStart = 0, nEnd = 0;
	m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
	if (nEnd > nStockDayLen)
	{
		nEnd = nStockDayLen;
	}
	StockDay* pStockDayDataCur = pStockDay + nStart;
	for (int i = nStart; i < nEnd; i ++)
	{
		StockDay* pData = pStockDayDataCur;
		if (pData != NULL && pData->m_lDate > 0 )
		{
			if((dblMin > pData->m_lMinPrice || dblMin == -1 ))
			{
				dblMin = pData->m_lMinPrice;
			}
			if (dblMax < pData->m_lMaxPrice)
			{
				dblMax = pData->m_lMaxPrice;
			}
		}
		pStockDayDataCur ++;
	}

	dMax = dblMax;
	dMin = dblMin;

}

BOOL CTechKLineImpAdd::GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex)
{
	if(m_pColor != NULL)
	{
		ayTitle.Add( new CToptextType<double>(m_stkInfo.GetName() + " ", 0, m_pColor->m_lColor, this, 0, 0, 1));
		return TRUE;
	}
	return FALSE;
}

short CTechKLineImpAdd::GetLineType()
{
	return etechLineType_stock;
}

int	CTechKLineImpAdd::SetColor(ColorProperty* pColor)
{
	m_pColor = pColor;
	return m_pColor->m_lColor;
}

BOOL CTechKLineImpAdd::IsMainLine()
{
	return FALSE;
}
