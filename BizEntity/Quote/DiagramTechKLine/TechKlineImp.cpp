#include "stdafx.h"
#include "TechKlineImp.h"
#include "TechKLineMgr.h"
#include "..\QuoteComm\QuoteBaseDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
CTechKLineImp::CTechKLineImp(CManagerBase* pMgr, IRegionNode* pRegion, float fHorizCellWidth)
{
	m_bVisable = TRUE;
	memset(&m_stkInfo.m_ciStockCode, 0, sizeof(CodeInfo));
	memset(&m_stkInfo.m_cStockName, 0, sizeof(m_stkInfo.m_cStockName));
	m_pTeckKLineMgr = pMgr;
	m_pLinear = new CHSLinear<float>(this);
	m_pPointPos = NULL;
	m_sChartParam.m_dwType = IDR_hexinKLineChart;
	m_nGridNum = 3;
	m_bFenshiDrawType = FALSE;
	m_pRegion = pRegion;
	m_bAddLine = FALSE;
	m_dwLinearStyle = AXIS_RIGHTDISPPRICE | AXIS_LEFTMARGIN | AXIS_TOPMARGIN | AXIS_FIRSTTEXT | AXIS_LEFT | AXIS_FENSHIHORI ;
	if (m_pLinear != NULL)
	{
		m_pLinear->SetAxesStyle(m_dwLinearStyle);
	}
	m_fHoriCellWidth = fHorizCellWidth;
	m_fKLineWidth = m_fHoriCellWidth * 3 / 4;;

	int nTotalHoriWidth = ::GetSystemMetrics(SM_CXSCREEN);
	m_nHoriCellNum = 0;//(int)((float)nTotalHoriWidth / m_fHoriCellWidth);
	m_nMaxHoriCellNum = (int)((float)nTotalHoriWidth / m_fHoriCellWidth);
	m_bBottomLine = FALSE;
	m_rcVarTitle = CRect(0, 0, 0, 0);
}

CTechKLineImp::~CTechKLineImp()
{
	if(m_pLinear != NULL)
	{
		delete m_pLinear;
		m_pLinear = NULL;
	}
	if (m_pPointPos != NULL)
	{
		delete []m_pPointPos;
		m_pPointPos = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL CTechKLineImp::Create(IDrawBaseParam *pParam)
{
	return TRUE;
}

void CTechKLineImp::SetRegionNode(IRegionNode* pNode)
{
	m_pRegion = pNode;
}

void CTechKLineImp::SetHorizCellWidth(float fNewCellWidth)
{
//	m_nHoriCellNum = nNewCellNum;
	m_fHoriCellWidth = fNewCellWidth;
	m_fKLineWidth = m_fHoriCellWidth * 3 / 4;
	if (m_fKLineWidth < 1.0)
	{
		m_fKLineWidth = 0;
	}
	int nTotalHoriWidth = ::GetSystemMetrics(SM_CXSCREEN);
	m_nMaxHoriCellNum = (int)((float)nTotalHoriWidth / m_fHoriCellWidth);
	SetRect(m_rect);
}

void CTechKLineImp::SetHorizCellNum(int nHorizCellNum)
{
	m_nHoriCellNum = nHorizCellNum;
}

int	CTechKLineImp::GetHorizCellNum()
{
	return m_nHoriCellNum;
}

int CTechKLineImp::GetMaxHorizCellNum()
{
	return m_nMaxHoriCellNum;
}

float CTechKLineImp::GetHorizCellWidth()
{
	return m_fHoriCellWidth;
}

BOOL CTechKLineImp::SetRect(CRect rc)
{
	if (rc.IsRectEmpty() && m_fHoriCellWidth < 0.0001)
	{
		return FALSE;
	}
	m_rect = rc;
	m_rcGird = m_rect;
	if (m_pLinear == NULL || m_pTeckKLineMgr == NULL)
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

	CRect rcDraw = m_rcGird;
	rcDraw.left   += pRect->left;
	rcDraw.right  -= pRect->right;
	rcDraw.top    += pRect->top;
	rcDraw.bottom -= pRect->bottom;

	if (rcDraw.Width() < m_fHoriCellWidth / 2 )
	{
		return FALSE;
	}
	m_nHoriCellNum =  int((float)(rcDraw.Width() - m_fHoriCellWidth / 2) / m_fHoriCellWidth);
	if (m_nHoriCellNum > 32000)
	{
		m_nHoriCellNum = 32000; 
	}

	m_pPointPos = new DrawPointInfo[m_nHoriCellNum + 1];
	m_pPointPos[0] = (float)rcDraw.left + m_fHoriCellWidth;
	m_pPointPos[0] = 0;
	for (int i = 0; i < m_nHoriCellNum; i++)
	{
		m_pPointPos[i + 1] = float(m_pPointPos[i]) + m_fHoriCellWidth;
		m_pPointPos[i + 1] = i + 1;
	}
	m_pLinear->SetRange(0, 0, rcDraw.Width(), rcDraw.Height());
	m_pLinear->RecalcRects(m_rcGird);


	return TRUE;
}

void CTechKLineImp::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	m_lCurMaxValue.Empty();
	m_lCurMinValue.Empty();
	if (m_pTeckKLineMgr == NULL)
	{
		return;
	}
	HSDouble dblMax = hx_DefMaxValues; //pLine->m_lMaxValue;
	HSDouble dblMin = hx_DefMinValues; //pLine->m_lMinValue;
	dOpenPrice = 0;

	StockDay* pStockDay = NULL;
	int nStockDayLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);
	if (pStockDay == NULL || nStockDayLen <= 0 )
	{
		dMax = 0;
		dMin = 0;
		return ;
	}
	int nStart = 0, nEnd = 0;
	m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
	if(pStockDay != NULL)
	{
		StockDay* pStockDayCur = pStockDay + nStart;
		for (int i = nStart; i < nEnd; i ++)
		{
			StockDay* pData = pStockDayCur;
			if (pData != NULL && pData->m_lDate > 0 )
			{
				if((dblMin > pData->m_lMinPrice || dblMin == -1 ))
				{
					dblMin = pData->m_lMinPrice;
					m_lCurMinValue.m_lValue = dblMin;
					m_lCurMinValue.m_nPos = i;
				}
				if (dblMax < pData->m_lMaxPrice)
				{
					dblMax = pData->m_lMaxPrice;
					m_lCurMaxValue.m_lValue = dblMax;
					m_lCurMaxValue.m_nPos = i;
				}
				if( i == nStart)
				{
					dOpenPrice = pData->m_lClosePrice;
				}
			}
			pStockDayCur ++;
		}
	}
	dMax = dblMax;
	dMin = dblMin;
}

CRect CTechKLineImp::GetRect()
{
	return m_rect;
}

void CTechKLineImp::SetVisible(BOOL bVisible)
{
	m_bVisable = bVisible;
}

BOOL CTechKLineImp::GetVisible()
{
	return m_bVisable;
}

void CTechKLineImp::UpdateCodeInfo(StockUserInfo* pCodeInfoNew)
{
	m_stkInfo.Copy(pCodeInfoNew);
}

StockUserInfo* CTechKLineImp::GetCodeInfo()
{
	return &m_stkInfo;
}

short CTechKLineImp::GetDrawType()
{
	return 0;
}

BOOL CTechKLineImp::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineImp::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if (m_pLinear != NULL)
	{
//		m_pLinear->IsSelected()
	}
	return FALSE;
}

BOOL CTechKLineImp::MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineImp::MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineImp::RButtonDown(UINT nFlags, CPoint point, HWND hWndParent, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineImp::RButtonUp(UINT nFlags, CPoint point, HWND hWndParent, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineImp::OnCursor(UINT nFlags, CPoint point)
{
	return FALSE;
}

int	CTechKLineImp::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return 0;
}

long CTechKLineImp::OnMenuMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int	CTechKLineImp::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr == NULL || m_pLinear == NULL)
	{
		return -1;
	}

	if (pDC == NULL || m_pPointPos == NULL)
	{
		return -1;
	}

	StockDay* pStockDay = NULL;
	int nStockDayLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);

	if (!m_rcGird.IsRectEmpty() )
	{
		CHSBaseDC dc(pDC);
		m_pLinear->BeginDraw(&dc);

		m_pLinear->SetXNumOfTicks(0);
		m_pLinear->SetYNumOfTicks(8);
		HSDouble dMax, dMin, dFirstOpen;	
		m_pTeckKLineMgr->GetMaxMinData(dMax, dMin, dFirstOpen);
		float fVerCellHeight = m_rcGird.Height();
		float fUnitValue = (dMax.m_dData - dMin.m_dData) / fVerCellHeight;

		StockUserInfo* pStock = m_pTeckKLineMgr->GetStockUserInfo();
		int nDecimal = 2;
		if (pStock != NULL)
		{
			nDecimal = pStock->m_nDecimal;
		}
		m_pLinear->SetYRefParam(dMin.m_dData , dMax.m_dData , fUnitValue, dFirstOpen.m_dData, 1000, nDecimal, 1);
		m_pLinear->SetAxesType(m_pTeckKLineMgr->GetAxesType());
		m_pLinear->Grid();
		m_pLinear->Axes();

		if (pStockDay != NULL && nStockDayLen > 0 )
		{
			int nStart = 0, nEnd = 0;//nStockCompDayLen - m_nHoriCellNum;
			m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);

			m_pLinear->DrawKLine(&m_sChartParam,
				CTValues<DrawPointInfo>(m_pPointPos, m_nHoriCellNum),
				CTValues<StockDay>(pStockDay, nStart, (nEnd - nStart)),
				m_fKLineWidth, //　横向线与线的间距
				g_hsGraphicsData.clDayUpLine, g_hsGraphicsData.clDayDownLine, g_hsGraphicsData.clDayUpFillLine, g_hsGraphicsData.clDayDownFillLine);
		}

		m_pTeckKLineMgr->DrawTitle(m_pLinear);
		m_pLinear->EndDraw(&dc);
	}
	return 0;
}

BOOL CTechKLineImp::GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, BOOL bForce)
{
	return TRUE;
}

int	CTechKLineImp::OnUpdateData(DataHead* pData)
{
	return 0;
}

int	CTechKLineImp::Delete()
{
	delete this;
	return 0;
}

BOOL CTechKLineImp::IsUp(int nPosition)
{
	if (m_pTeckKLineMgr == NULL)
	{
		return FALSE;
	}
	StockDay* pStockDay = NULL;
	int nStockDayDataLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);

	if (pStockDay == NULL || nStockDayDataLen <= 0 || nStockDayDataLen < nPosition)
	{
		return FALSE;
	}

	return (pStockDay[nPosition].m_lOpenPrice < pStockDay[nPosition].m_lClosePrice
		|| (pStockDay[nPosition].m_lOpenPrice == pStockDay[nPosition].m_lClosePrice
		&& pStockDay[nPosition].m_lClosePrice >= pStockDay[(nPosition - 1)>0?(nPosition - 1) : 0].m_lClosePrice));

}

void CTechKLineImp::SetFenshiDrawType(BOOL bType)
{
	m_bFenshiDrawType = bType;
}

BOOL CTechKLineImp::IsFenshiDrawType()
{
	return m_bFenshiDrawType;
}

CManagerBase* CTechKLineImp::GetTechLineMgr()
{
	return m_pTeckKLineMgr;
}

BOOL CTechKLineImp::IsMainLine()
{
	return TRUE;
}

CRect CTechKLineImp::GetMarginRect()
{
	CRect rcRet = CRect(0, 0, 0, 0);
	if (m_pLinear != NULL)
	{
		CRect* pRect = m_pLinear->GetMargin();
		rcRet.CopyRect(pRect);
	}
	return rcRet;
}

void CTechKLineImp::SetAddLine(BOOL bAddLine)
{
	m_bAddLine = bAddLine;
}

BOOL CTechKLineImp::IsAddLine()
{
	return m_bAddLine;
}

BOOL CTechKLineImp::GetVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& ayTitle, int nDataIndex)
{
//	return FALSE;
	if(m_pTeckKLineMgr != NULL)
	{
		StockUserInfo* pStock = m_pTeckKLineMgr->GetStockUserInfo();
		if (pStock != NULL)
		{
			ayTitle.Add( new CToptextType<double>(pStock->GetName() + " ", 0, g_hsGraphicsData.clFixedNameTXT.m_lColor, this, 0));//名称的颜色用‘名称文本’
			return TRUE;
		}
	}
	return FALSE;
}

int CTechKLineImp::GetDataXPos(int nDataIndex)
{
	int	nRet = -1;
	if (nDataIndex < 0 && m_pLinear != NULL)
	{
		return m_pLinear->GetMargin()->left;
	}
	else if (m_pPointPos != NULL && m_pTeckKLineMgr != NULL)
	{
		StockDay* pStockDay = NULL;
		int nStockDayDataLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);
		int nMaxCell = min(m_nHoriCellNum, nStockDayDataLen);
		if (nDataIndex >= 0 && nDataIndex <= nMaxCell)
		{
			nRet = (int)(float(m_pPointPos[nDataIndex]));
		}
	}
	return nRet;
}

int	CTechKLineImp::GetDataYPos(int nDataIndex)
{
	if(m_pTeckKLineMgr != NULL)
	{
		StockDay* pStockDay = NULL;
		int nStockDayDataLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);
		int nStart = 0, nEnd = 0;
		m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
		int nCurIndex = nDataIndex + nStart;
		if (nCurIndex < nStockDayDataLen && pStockDay != NULL)
		{
			StockDay* pCurData = &pStockDay[nCurIndex];
			if (pCurData != NULL && !::IsBadReadPtr(pCurData, sizeof(StockDay)))
			{
				if (m_pLinear != NULL)
				{
					CPoint pt;
					pt.y = m_pLinear->ValueToVertPos(pCurData->m_lClosePrice);
					pt = m_pLinear->GetPoint(pt.x, pt.y);

					return pt.y;
				}
			}
		}
	}
	return 0;
}

int	CTechKLineImp::GetInMarginYPos(int nSourceYPos)
{
	int nRetPosY = nSourceYPos;
	if (m_pLinear != NULL)
	{
		CRect* pRect = m_pLinear->GetMargin();
		if (nSourceYPos < m_rect.top + pRect->top)
		{
			nRetPosY = m_rect.top + pRect->top;
		}
		if (nSourceYPos > m_rect.bottom - pRect->bottom)
		{
			nRetPosY = m_rect.bottom - pRect->bottom;
		}
	}
	return nRetPosY;
}

int CTechKLineImp::HitTest(CPoint pt)
{
	if (GetVisible() && !IsAddLine())
	{
		if(m_rect.PtInRect(pt))
		{
			int	nRet = 0;
			if (m_pPointPos != NULL)
			{
				StockDay* pStockDay = NULL;
				int nStockDayDataLen = m_pTeckKLineMgr->GetStockDayData(pStockDay);
				int nStart = 0, nEnd = 0;
				m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
				int nMaxCell = min(nEnd - nStart, nStockDayDataLen);
				if (pt.x > float(m_pPointPos[nMaxCell]))
				{
					nRet = nMaxCell - 1;
				}
				else
				{
					for (int i = 0; i <= m_nHoriCellNum; i ++)
					{
						if(float(m_pPointPos[i]) >= pt.x)
						{
							nRet = i;
							break;
						}
					}
				}
			}
			return nRet;
		}
	}
	return -1;
}

BOOL CTechKLineImp::GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point)
{
	if(m_pLinear != NULL)
		return m_pLinear->GetTipText(pMouseTipTextInfo, nPosition, point);
	return FALSE;
}

short CTechKLineImp::GetLineType()
{
	return etechLineType_main;
}

void CTechKLineImp::SetTeckKLineName(CString strName)
{

}

CString CTechKLineImp::GetTeckKLineName()
{
	return _T("");
}

void CTechKLineImp::SetIsBottom(BOOL bIsBottom)
{
	m_bBottomLine = bIsBottom;
}

BOOL CTechKLineImp::GetIsBottom()
{
	return m_bBottomLine;
}

DWORD CTechKLineImp::ModifyLinearStyle(DWORD dwAddStyle, DWORD dwRemoveStyle)
{
	m_dwLinearStyle |= dwAddStyle;
	m_dwLinearStyle &= ~dwRemoveStyle;
	if (m_pLinear != NULL)
	{
		m_pLinear->SetAxesStyle(m_dwLinearStyle);
	}
	return m_dwLinearStyle;
}

void CTechKLineImp::SetVarTitleRect(CRect rc, BOOL bAdd)
{
	if (bAdd)
	{
		m_rcVarTitle.right = rc.right;
	}
	else
	{
		m_rcVarTitle = rc;
	}
}

CRect CTechKLineImp::GetVarTitleRect()
{
	return m_rcVarTitle;
}

CDispMaxMinValue* CTechKLineImp::GetCurMaxValue()
{
	return &m_lCurMaxValue;
}

CDispMaxMinValue* CTechKLineImp::GetCurMinValue()
{
	return &m_lCurMinValue;
}
