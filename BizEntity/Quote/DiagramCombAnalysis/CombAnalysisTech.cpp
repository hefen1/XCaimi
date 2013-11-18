#include "StdAfx.h"
#include "CombAnalysisTech.h"
#include "Watcher.h"
#include "..\..\..\HelperLib\GeneralHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCombAnalysisTech::CCombAnalysisTech(void)
{
	m_pIndicatrixObj = NULL;
	m_nValidCount = NULL;
	this->SetDrawType(DrawType_FenShi);
	m_pLinear = new CHSLinear<float>(this);
	m_dwLinearStyle = AXIS_FENSHIPRICE | AXIS_FIRSTTEXT |AXIS_LASTTEXT | AXIS_SOLIDANDDASH | AXIS_HIGHFREQUENCYHORI /*AXIS_FENSHIHORI*/;
	m_dwLinearStyle |= AXIS_BOTTOM| AXIS_LEFT | AXIS_RIGHT ;
	m_dwLinearStyle |= AXIS_LEFTMARGIN | AXIS_TOPMARGIN | AXIS_RIGHTMARGIN | AXIS_BOTTOMMARGIN ;
	if (m_pLinear != NULL)
	{
		m_pLinear->SetAxesStyle(m_dwLinearStyle);
	}
	m_pPointPos = new DrawPointInfo[IndicatrixValuesLen];
	m_fHoriCellWidth = 0;
	m_pDrawIndicatrixData = new DrawIndicatrixData[IndicatrixValuesLen];
	for( int i = 0; i < IndicatrixValuesLen; i++)
	{
		m_pDrawIndicatrixData[i].m_dValue = 0;
	}
}

CCombAnalysisTech::~CCombAnalysisTech(void)
{
	delete[] m_pDrawIndicatrixData;
	delete[] m_pPointPos;
}

BOOL CCombAnalysisTech::Create(IDrawBaseParam *pParam)
{

	return TRUE;
}

BOOL CCombAnalysisTech::SetRect(CRect rc)
{
	if (rc.IsRectEmpty() )
	{
		return FALSE;
	}
	m_rect = rc;
	m_rcGird = m_rect;
	return RecalcRect(m_rcGird);
}

BOOL CCombAnalysisTech::RecalcRect(CRect rcArea)
{
	m_pLinear->SetXRefParam(0, 0, 0, 0, (IndicatrixValuesLen - 1)/ 8);
	m_pLinear->SetXNumOfTicks(8);
	m_pLinear->SetYNumOfTicks(8);

	CRect rcTemp;
	CRect* pRect = m_pLinear->GetMargin(rcTemp,PKLINE_TYPE_FENSHI);
	CRect rcDraw = rcArea;
	rcDraw.left   += pRect->left;
	rcDraw.right  -= pRect->right;
	rcDraw.top    += pRect->top;
	rcDraw.bottom -= pRect->bottom;

	double fStep = (double)(rcDraw.Width()) / (IndicatrixValuesLen - 1);
	m_fHoriCellWidth = (float)fStep;
	m_pPointPos[0] = (float)rcDraw.left;
	m_pPointPos[0] = 0;
	for (int i = 0; i < IndicatrixValuesLen-1; i++)
	{
		m_pPointPos[i + 1] = float(m_pPointPos[i]) + m_fHoriCellWidth;
		m_pPointPos[i + 1] = i + 1;
	}
	m_pLinear->SetRange(0, 0, rcDraw.Width(), rcDraw.Height());
	m_pLinear->RecalcRects(m_rcGird);
	return TRUE;
}

int CCombAnalysisTech::Draw(CDC* pDC)
{
	if (m_rect.IsRectEmpty( ))
		return -1;
	float fVerCellHeight =m_rect.Height();
	HSDouble dMax, dMin, dOpen;
	dMax.m_dData = m_dMaxValue;
	dMin.m_dData = m_dMinValue;
	dOpen.m_dData = (dMax.m_dData + dMin.m_dData)/2;
	float fUnitValue =dMax.m_dData /  fVerCellHeight;

	m_pLinear->SetYRefParam(dMin.m_dData, dMax.m_dData, fUnitValue, dOpen.m_dData,1,2);

	//grid
	CHSBaseDC dc(pDC);
	m_pLinear->BeginDraw(&dc);
	m_pLinear->Grid();
	m_pLinear->Axes();

	//line
	 int nDataLen = m_nValidCount;
	 m_pLinear->EnableLegendShadow(TRUE);
	 m_pLinear->DrawCurve(CGraphics::NONE,
		CTValues<DrawPointInfo>(m_pPointPos, 0),
		CTValues<DrawIndicatrixData>((DrawIndicatrixData*)&m_pDrawIndicatrixData[0], nDataLen),
		dMax.m_dData , dMin.m_dData,
		g_hsGraphicsData.clJsPriceLine);

	 //title info
    CArray< CToptextType<double>*,CToptextType<double>* > ayAdd;
	double dLast = nDataLen > 0 ? m_pDrawIndicatrixData[nDataLen-1].m_dValue : m_pDrawIndicatrixData[0].m_dValue;
	CToptextType<double>* pTitleInfo = new CToptextType<double>(m_strLineName ,dLast, g_hsGraphicsData.clFixedNameTXT.m_lColor,2);
	ayAdd.Add(pTitleInfo);//名称的颜色用‘名称文本’
	m_pLinear->DrawVarTitle(ayAdd);
	delete pTitleInfo;

	m_pLinear->EndDraw(&dc);
	return 0;
}

void CCombAnalysisTech::OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam)
{
	if(nMsgId == DiagramMsg_Comb_Indicatrix_Sel)
	{
		IIndicatrixObj* pObj = (IIndicatrixObj*)pParam->GetIntegerParam(Param_Comb_IndicatrixObj);
		CString strLineName = pParam->GetStringParam(Param_Indicatrix_Name);
		m_pIndicatrixObj = pObj;
		SetLineName(strLineName);
		RefreshDrawData();
	}
	else if(nMsgId == DiagramMsg_Refresh_Indicatrix_Line)
	{
		RefreshDrawData();
	}
	else if(nMsgId == DiagramMsg_WatcherObj_Change)
	{
		CWatcher* pWatcher =  (CWatcher*)pParam->GetIntegerParam(Param_WatcherObj);
		CString      strChangeFlag = pParam->GetStringParam(Param_Change_Flag);
		if(pWatcher != NULL && strChangeFlag == Param_Change_Flag_Del)
		{
			m_pIndicatrixObj = NULL;
		}
	}
}

void CCombAnalysisTech::RefreshDrawData()
{
	if(m_pIndicatrixObj == NULL)
		return;

	m_pIndicatrixObj->GetOtherData(m_dMaxValue,m_dMinValue,m_nValidCount);
	m_pIndicatrixObj->GetDrawIndicatrixData(m_pDrawIndicatrixData,m_nValidCount);
	if(m_pRegion)
		m_pRegion->Invalidate(&m_rcGird);
}
