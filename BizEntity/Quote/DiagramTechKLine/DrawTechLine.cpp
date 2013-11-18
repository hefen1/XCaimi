//
#include "stdafx.h"
#include "DrawTechLine.h"
#include "TechKLineMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLOSE_BUTTON_WIDTH	9
#define CLOSE_BUTTON_STEP	4
//////////////////////////////////////////////////////////////////////////

CDrawTechLine::CDrawTechLine(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth, int nCellNum ) : CTechKLineImp(pMgr, pRegion, fHorizCellWidth)
{
	m_nStockDayDataLen = 0;
	m_bAddLine = FALSE;
	m_bInitFormFenshi = FALSE;
	if (nCellNum > 0)
	{
		m_bInitFormFenshi = TRUE;
		m_nHoriCellNum = nCellNum;
	}
	m_rcClose = CRect(0, 0, 0, 0);
}

CDrawTechLine::~CDrawTechLine()
{
	while (m_arLine.GetSize() > 0)
	{
		CLineProperty* pLine = m_arLine.GetAt(0);
		m_arLine.RemoveAt(0);
		if (pLine != NULL)
		{
			delete pLine;
			pLine = NULL;
		}
	}
}

void CDrawTechLine::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return;
	}
	int nStart = 0, nEnd = 0;
	m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = hx_DefMinValues; //pLine->m_lMinValue;
	dOpenPrice = 0;
	for (int nLineIndex = 0; nLineIndex < m_arLine.GetSize(); nLineIndex ++)
	{
		CLineProperty* pLine = m_arLine.GetAt(nLineIndex);

		if (pLine != NULL && pLine->m_nSize > 0)
		{
			HSDouble* pValue = pLine->m_pData + nStart;
			int nMinEnd = min(nEnd, pLine->m_nSize);
			for (int i = nStart; i < nMinEnd; i ++)
			{
				HSDouble dblValue = *pValue;
				if ((long)dblValue != hx_DefMinValues)
				{
					if (dOpenPrice == 0)
					{
						dOpenPrice = dblValue;
					}
					if (dblValue < dblMin )
					{
						dblMin = dblValue;
					}
					if (dblValue > dblMax)
					{
						dblMax = dblValue;
					}
				}
				pValue ++;
			}
		}
	}
	if (dblMax == hx_DefMaxValues)
	{
		dblMax = 0;
	}
	if (dblMin == hx_DefMinValues)
	{
		dblMin = 0;
	}
	dMax = dblMax.m_dData * 1000;
	dMin = dblMin.m_dData * 1000;

}

BOOL CDrawTechLine::SetRect(CRect rc)
{
	if (m_nHoriCellNum > 0 && m_pLinear != NULL && m_bInitFormFenshi)
	{
//		m_bInitFormFenshi = FALSE;
		m_rect = rc;
		m_rcGird = m_rect;
		CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();
		CClientDC dc(pWnd);
		CRect rcTemp;
		rcTemp.left = m_pTeckKLineMgr->GetStockUserInfo()->m_ciStockCode.m_cCodeType;
		CRect* pRect = m_pLinear->GetMargin(rcTemp, PKLINE_TYPE_FENSHI, &dc);
		CRect rcDraw = m_rcGird;
		rcDraw.left   += pRect->left;
		rcDraw.right  -= pRect->right;
		rcDraw.top    += pRect->top;
		rcDraw.bottom -= pRect->bottom;
		if(rcDraw.IsRectEmpty())
		{
			return FALSE;
		}
		double fStep = (double)rcDraw.Width() / (m_nHoriCellNum - 1);
		m_fHoriCellWidth = (float)fStep;
		m_fKLineWidth = m_fHoriCellWidth * 3 / 4;
		if (m_fKLineWidth < 1.0)
		{
			m_fKLineWidth = 0;
		}
		m_pPointPos = new DrawPointInfo[m_nHoriCellNum + 1];
		m_pPointPos[0] = (float)rcDraw.left;
		m_pPointPos[0] = 0;
		for (int i = 0; i < m_nHoriCellNum; i++)
		{
			m_pPointPos[i + 1] = float(m_pPointPos[i]) + m_fHoriCellWidth;
			m_pPointPos[i + 1] = i + 1;
		}

		m_pLinear->SetRange(0, 0, rcDraw.Width(), rcDraw.Height());
		m_pLinear->RecalcRects(m_rect);
		return TRUE;
	}
	else
		return CTechKLineImp::SetRect(rc);
}

int CDrawTechLine::Draw(CDC* pDC)
{
	if( m_pLinear == NULL)
	{
		return -1;
	}
	if (m_pTeckKLineMgr == NULL )
	{
		return -1;
	}

	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}
	CHSBaseDC dc(pDC);
	m_pLinear->BeginDraw(&dc);
	if (!IsAddLine())
	{
		m_pLinear->SetXNumOfTicks(0);
		m_pLinear->SetYNumOfTicks(3);
	}
	HSDouble dMax, dMin, dOpen;
	if (!IsAddLine())
	{
		m_rcClose = CRect(m_rect.right - CLOSE_BUTTON_WIDTH - CLOSE_BUTTON_STEP, m_rect.top + CLOSE_BUTTON_STEP, 
			m_rect.right - CLOSE_BUTTON_STEP, m_rect.top + CLOSE_BUTTON_WIDTH + CLOSE_BUTTON_STEP);
		if (!m_rect.PtInRect(m_rcClose.TopLeft()) || !m_rect.PtInRect(m_rcClose.BottomRight()))
		{
			m_rcClose = CRect(0, 0, 0, 0);
		}
		
		GetMaxMinData(dMax, dMin, dOpen);

		float fVerCellHeight = m_rcGird.Height();
		float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;
		if (m_strName == _T("VOL"))
		{
			m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, 0, 10, 0, 1);
		}
		else
		{
			m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, 0, 1, 2, 1);
		}
		m_pLinear->Grid();
		m_pLinear->Axes();

		if (!m_rcClose.IsRectEmpty())
		{
			CPen pen(g_hsGraphicsData.clChartTick.m_nPenStyle, g_hsGraphicsData.clChartTick.m_nWidth, g_hsGraphicsData.clChartTick.m_lColor);
			CPen *pOldPen = (CPen*)pDC->SelectObject(&pen);
			pDC->MoveTo(m_rcClose.left, m_rcClose.top);
			pDC->LineTo(m_rcClose.right, m_rcClose.bottom);
			pDC->MoveTo(m_rcClose.right, m_rcClose.top);
			pDC->LineTo(m_rcClose.left - 1, m_rcClose.bottom);
			pDC->SelectObject(pOldPen);
		}
	}
	else
	{
		m_pTeckKLineMgr->GetMaxMinData(dMax, dMin, dOpen);
		float fVerCellHeight = m_rcGird.Height();
		float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight / 1000;
		if (m_strName == _T("VOL"))
		{
			m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, 0, 10, 0, 1);
		}
		else
		{
			m_pLinear->SetYRefParam(dMin.m_dData / 1000, dMax.m_dData / 1000, fUnitValue, 0, 1, 2, 1);
		}
	}
	int nStart = 0, nEnd = 0;//nStockCompDayLen - m_nHoriCellNum;
	m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);

//	ASSERT(m_nStockDayDataLen == (nEnd - nStart));
	if (m_nStockDayDataLen < (nEnd - nStart))
	{
		nEnd = nStart + m_nStockDayDataLen;
	}
	
	for (int nLineIndex = 0; nLineIndex < m_arLine.GetSize(); nLineIndex ++)
	{
		CLineProperty* pLine = m_arLine.GetAt(nLineIndex);
		if(pLine != NULL && pLine->IsStyle(HX_EXPRESSDRAW))
		{
			m_pLinear->DrawTechLine(pLine->m_dwStyle, 
			CTValues<DrawPointInfo>(m_pPointPos, nStart, m_nHoriCellNum),
			CTValues<HSDouble>((HSDouble*)&pLine->m_pData[nStart], 0, (nEnd - nStart)),
			m_fKLineWidth, pLine->m_lColor, g_hsGraphicsData.clDayUpLine, g_hsGraphicsData.clDayDownLine, g_hsGraphicsData.clDayUpFillLine, g_hsGraphicsData.clDayDownFillLine, nLineIndex, NULL
			);
		}
	}
	if (!IsAddLine())
	{
		m_pTeckKLineMgr->DrawTitle(m_pLinear);
	}
	m_pLinear->EndDraw(&dc);
	return 0;
}

void CDrawTechLine::SetTeckKLineName(CString strName)
{
	m_strName = strName;
}

CString	CDrawTechLine::GetTeckKLineName()
{
	return m_strName;
}

int	CDrawTechLine::OnUpdateData(DataHead* pData)
{
	IFormulaMan* pFormulaMan = NULL;
	if (m_pTeckKLineMgr != NULL)
	{
		pFormulaMan = m_pTeckKLineMgr->GetFormulaMan();
	}
	if (pFormulaMan != NULL)
	{
		StockDay* pStockDay = NULL;
		m_nStockDayDataLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);
		int nNum = pFormulaMan->GetLineNum(m_strName, Tech);
		if (nNum > 0 && m_nStockDayDataLen > 0)
		{
			while (m_arLine.GetSize() > 0)
			{
				CLineProperty* pLine = m_arLine.GetAt(0);
				m_arLine.RemoveAt(0);
				if (pLine != NULL)
				{
					delete pLine;
					pLine = NULL;
				}
			}
			for (int i = 0; i < nNum; i ++)
			{
				CLineProperty* pLine = new CLineProperty;
				pLine->Alloc(m_nStockDayDataLen, 0);
				m_arLine.Add(pLine);
			}
			pFormulaMan->CalcValue(m_strName, Tech, &m_arLine);
		}
	}

	return 0;
}

BOOL CDrawTechLine::IsMainLine()
{
	return FALSE;
}

void CDrawTechLine::SetHorizCellNum(int nHorizCellNum)
{
	m_nHoriCellNum = nHorizCellNum;
	CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();
	CClientDC dc(pWnd);

	CRect rcTemp;
	rcTemp.left = m_pTeckKLineMgr->GetStockUserInfo()->m_ciStockCode.m_cCodeType;
	CRect* pRect = m_pLinear->GetMargin(rcTemp, PKLINE_TYPE_FENSHI, &dc);

	CRect rcDraw = m_rect;
	rcDraw.left   += pRect->left;
	rcDraw.right  -= pRect->right;
	rcDraw.top    += pRect->top;
	rcDraw.bottom -= pRect->bottom;
	double fStep = (double)(rcDraw.Width() - 2) / (m_nHoriCellNum - 1);
	m_fHoriCellWidth = (float)fStep;

	SetRect(m_rect);
}

BOOL CDrawTechLine::GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex)
{
	IFormulaMan* pFormulaMan = NULL;
	if (m_pTeckKLineMgr != NULL)
	{
		pFormulaMan = m_pTeckKLineMgr->GetFormulaMan();
	}
	if (pFormulaMan != NULL)
	{
		int nImageIndex = -1;
		if (IsAddLine())
		{
			nImageIndex = 0;
		}
		ayTitle.Add( new CToptextType<double>(pFormulaMan->GetExpressVar(m_strName, Tech) + " ", 0, g_hsGraphicsData.clFixedTXT.m_lColor, this, 0, 0, nImageIndex));
	}
	if (m_arLine.GetSize() > 0 )
	{
		for (int nLineIndex = 0; nLineIndex < m_arLine.GetSize(); nLineIndex ++)
		{
			CLineProperty* pLine = m_arLine.GetAt(nLineIndex);
			if(pLine != NULL && pLine->IsStyle(HX_EXPRESSDRAW) && !pLine->IsStyle(CGraphics::NOTDRAWTEXT))
			{
				COLORREF clrText = pLine->m_lColor.m_lColor;
				if(pLine->m_strName[0] == '\0')
				{
					clrText = g_hsGraphicsData.clFixedTXT.m_lColor;
				}
				ayTitle.Add( new CToptextType<double>(CString(pLine->m_strName) + ":", 0, clrText, this, 0));
				CString strData;
				if (nDataIndex < pLine->m_nSize)
				{
					HSDouble hsData = pLine->m_pData[nDataIndex];
					if (g_hsGraphicsData.IsValue(&hsData.m_dData))
					{
						strData.Format("%.3f", hsData.m_dData);
					}

				}
				if (strData.IsEmpty())
				{
					strData = " - ";
				}
				ayTitle.Add( new CToptextType<double>(strData + " ", 0, clrText, this, 0));
			}
		}
	}
	return FALSE;
}

short CDrawTechLine::GetLineType()
{
	return etechLineType_tech;
}

BOOL CDrawTechLine::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if(!m_rcClose.IsRectEmpty() && m_rcClose.PtInRect(point))
	{
		if (m_pTeckKLineMgr != NULL)
		{
			m_pTeckKLineMgr->DelayDeleteTechLine(this);
		}
	}
	return FALSE;
}