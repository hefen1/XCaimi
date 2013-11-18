// Graphics.cpp: implementation of the CGraphics class.
//
//////////////////////////////////////////////////////////////////////
//20080602 chenhf 多股票K线图中当前选中的股票名称高亮显示，单股票时不高亮显示

#include "stdafx.h"
#include "Graphics.h"
#include "..\QuoteComm\QuoteBaseDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <stdio.h>
#include <math.h>

///////////////////////////////////////////////////////////////////////////////
// class CGroupLines

#define Offset_Width  6
#define ImageFocus_Width  11
#define ImageFocus_Height 13

// 基本面分析财务柱形图 顶部纵向偏移值 注意,此定义有条件编译 熊钟彬
//#define FINANCIAL_3DBAR_OFFSET 30
extern int GetLastTimer(CTime& tm,HSMarketDataType cCodeType,int nCurTimePos);
extern int nBottom, nTop;
CGraphics::MoveCostParamArray g_ayParam;
void SetMoveCostParam()
{
/*	ColorProperty* pColor;
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST) )
	{
		pColor = &g_hsGraphicsData.clHuoYanShan;
	}
	else if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST) )
	{
		pColor = &g_hsGraphicsData.clHuoYaoDu;
	}
	else
	{
		pColor = &g_hsGraphicsData.clDefCost;
	}

	CGraphics::MoveCostParam* pMoveColor;
	if( g_ayParam.ayParam.GetSize() <= 0 )
	{
		for( int i = 0; i < MOVECOSTCOUNT + 1; i ++)
		{
			pMoveColor = new CGraphics::MoveCostParam;
			pMoveColor->cPos = i;
			g_ayParam.ayParam.Add(pMoveColor);
		}
	}

	{
		g_ayParam.lColor.m_lColor    = pColor->m_lColor;
		g_ayParam.lColor.m_nPenStyle = pColor->m_nPenStyle;
		g_ayParam.lColor.m_nWidth    = pColor->m_nWidth;

		BYTE r,g,b;
		r = GetRValue(g_ayParam.lColor.m_lColor);
		g = GetGValue(g_ayParam.lColor.m_lColor);
		b = GetBValue(g_ayParam.lColor.m_lColor);

		for( int i = 0; i < MOVECOSTCOUNT + 1; i ++)
		{
			pMoveColor = g_ayParam.ayParam.GetAt(i);
			pMoveColor->SetColor(r,g,b);
		}
	}*/
}

static CGraphics::HxhMoveCostData g_HxhMoveData;

int CGraphics::HxhMoveCostData::Save()
{
/*	CWinApp* pApp = AfxGetApp();

	if( pApp->WriteProfileBinary("UserData", "MoveCostData",
		(LPBYTE)&g_HxhMoveData, sizeof(g_HxhMoveData) ) )
		return 1;
*/
	return 0;
}

int CGraphics::HxhMoveCostData::Load()
{
	CWinApp* pApp = AfxGetApp();

	char* pData = NULL;
	UINT  pBytes = sizeof(g_HxhMoveData);
	BYTE** pTemp = (BYTE**)&pData;
	if( pApp->GetProfileBinary("UserData", "MoveCostData",pTemp,&pBytes) )
	{
		if( *pTemp )
		{
			memcpy(&g_HxhMoveData,*pTemp,sizeof(g_HxhMoveData));
			delete[] *pTemp;
			return 1;
		}
	}
	if( pData && *pTemp)
	{
		delete[] *pTemp;
	}
	return 0;
}

CGraphics::HxhMoveCostData::HxhMoveCostData()
{
	//	if( !CGraphics::HxhMoveCostData::Load() )
	{
		m_nCalcType = 1;
		m_nHYSCount = 0;
		m_nHYDCount = 0;

		for( int i = 0; i <  MOVECOSTCOUNT; i++ )		//data for test,will be delete later
		{
			m_ayHouYanShan[i] = (i + 1) * 10;
			m_ayHuoYueDu[i]	  = (i + 1) * 10;
			m_ayMoveCost[i]   = (i + 1) * 5;
		}

		for( int i = 0; i < MOVECOSTCOUNT; i++ )		//calculate the valid num;
		{
			if( m_ayHouYanShan[i] != 0 )
			{
				m_nHYSCount++;
			}
			if( m_ayHuoYueDu[i] != 0 )
			{
				m_nHYDCount++;
			}
		}
	}
}


CGraphics::HxhMoveCostData*		 GetHxhMoveData()
{
	return &g_HxhMoveData;
}

double CGraphics::MoveCostConfig::dCostNowData[MOVECOSTCOUNT + 1][1000];
CGraphics::MoveCostDetail	g_CostDetail;

void CGraphics::MoveCostConfig::EmptyCost()
{
	memset(dCostNowData,0,sizeof(dCostNowData));
}

void CGraphics::MoveCostParam::SetColor(BYTE r,BYTE g,BYTE b)
{
/*	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST) )
	{
		if( cPos == 0 )
		{
			m_lColor = RGB(r,255,0);
		}
		else
		{
			m_lColor = RGB(r,(cPos - 1) * 28,b);
		}
	}
	else if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		if( cPos == 10 )
		{
			m_lColor = RGB(r,255,0);
		}
		else
		{
			m_lColor = RGB(r,cPos*28,255 - cPos * 28);
		}
	}
	else if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
	{
		if( cPos == 10 ) 
		{
			m_lColor = g_hsGraphicsData.clPageBK.m_lColor;
		}
		else
		{
			m_lColor = RGB( r - cPos*25, g - cPos*25, b - cPos*25 );
		}
	}*/
}

CImageList  CGraphics::m_MenuBitmap;
SIZE		CGraphics::m_MenuSize;

static  char m_nFontDefHeight = -15;
LOGFONT	CGraphics::m_LogFont;

//static int nMarkIndex = CExpression::Yls_Exp_Info_Mark;  //标记信息地雷标记所用图案的在长图条中的编号
static bool bEverRead = true;  //记录是否已经读过了配置文件

CGraphics::CurrentActiveStatus CGraphics::m_curAtiveStatus;

CGraphics::~CGraphics() 
{
	/*
	if (m_Bitmap.m_hObject) 
	{
	m_Bitmap.DeleteObject();
	}
	*/
	if(m_pBackColor != NULL)
	{
		delete[] (char*)m_pBackColor;
		m_pBackColor = NULL;
	}	
	if(m_pVerLineColor != NULL)
	{
		delete[] (char*)m_pVerLineColor;
		m_pVerLineColor = NULL;
	}
	if(m_pHorLineColor != NULL)
	{
		delete[] (char*)m_pHorLineColor;
		m_pHorLineColor = NULL;
	}	
	if(m_pGridLineColor != NULL)
	{
		delete[] (char*)m_pGridLineColor;
		m_pGridLineColor = NULL;
	}	
	if(m_pChartTickColor != NULL)
	{
		delete[] (char*)m_pChartTickColor;
		m_pChartTickColor = NULL;
	}	
	if(m_pChartTickTXTColor != NULL)
	{
		delete[] (char*)m_pChartTickTXTColor;
		m_pChartTickTXTColor = NULL;
	}	
	if(m_pChartTitleColor != NULL)
	{
		delete[] (char*)m_pChartTitleColor;
		m_pChartTitleColor = NULL;
	}
}	

CGraphics::CGraphics()
{	
	Defaut();
}

void CGraphics::Defaut()
{
	m_ayDrawList = NULL;
	m_ayDrag	 = NULL;
	m_pThis		 = NULL;

	SetRatio();

	m_pBackColor	= (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pBackColor   = &g_hsGraphicsData.clChartBK;
	m_pVerLineColor = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pVerLineColor  = &g_hsGraphicsData.clDotVerLine;
	m_pHorLineColor   = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pHorLineColor  = &g_hsGraphicsData.clDotHorLine;
	m_pGridLineColor  = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pGridLineColor  = &g_hsGraphicsData.clGridLine;
	m_pChartTickColor  = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pChartTickColor = &g_hsGraphicsData.clChartTick;
	m_pChartTickTXTColor  = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pChartTickTXTColor = &g_hsGraphicsData.clChartTickTXT;
	m_pChartTitleColor  = (ColorProperty**)new char[sizeof(ColorProperty*)];
	*m_pChartTitleColor = &g_hsGraphicsData.clChartTitle;

	m_nXDecimal		= 0;
	m_nYDecimal		= 0;
	m_XGridTicks	= 10;
	m_YGridTicks	= 10;
	m_XTicks		= 50;
	m_YTicks		= 50;
	m_bEnableLegend = false;
	m_bPrinting		= false;
	m_bLegendShadow = false;
	m_bMemoryDraw	= false;
	m_nPrintScale	= 8;
	m_nAxesType		= XY;
	m_nXStep		= 0;
	m_nYStep		= 0;

	m_nPreLeftMaxMargin  = 0;
	m_nPreRightMaxMargin = 0;

	memset(&m_PlotRect,0,sizeof(RECT));
	memset(&m_Rect,0,sizeof(RECT));

	m_GL = m_PlotRect.left;
	m_GR = m_PlotRect.right;
	m_GT = m_PlotRect.top;
	m_GB = m_PlotRect.bottom;
	m_PX = m_GR - m_GL;
	m_PY = m_GB - m_GT;

	m_XTicksUnit = m_YTicksUnit = 5;
	SetXRefParam();
	SetYRefParam();
	SetTickLineLen();

	//20090721 YJT 将这句话移到这里
	m_dwAxesStyle = AXIS_LEFT | /*AXIS_DRAWBOUNDARY | */AXIS_LEFTMARGIN | AXIS_RIGHTMARGIN;

	SetMargin(CMarginParam(66,45,70),CMarginParam::left);
	SetMargin(CMarginParam(20,20,30),CMarginParam::top);
	SetMargin(CMarginParam(50,35,60),CMarginParam::right);
	SetMargin(CMarginParam(23,20,30),CMarginParam::bottom);

	SetMargin( CRect(m_rcDefMargin.left.m_nDef,m_rcDefMargin.top.m_nDef,
		m_rcDefMargin.right.m_nDef,m_rcDefMargin.bottom.m_nDef) );

	m_rcExplain.SetRectEmpty();

	static char cOneTime = 0;
	if( !cOneTime )
	{
		cOneTime = 1;

		m_MenuSize.cx = 0;
		m_MenuSize.cy = 0;

		// 字体
		memset(&m_LogFont, 0, sizeof(m_LogFont));
		m_LogFont.lfWidth			= 0;
		m_LogFont.lfItalic			= false;
		m_LogFont.lfUnderline		= false;
		m_LogFont.lfStrikeOut		= false;
		m_LogFont.lfWeight			= FW_NORMAL;
		m_LogFont.lfCharSet			= DEFAULT_CHARSET;//GB2312_CHARSET;//ANSI_CHARSET;
		//		m_LogFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
		//		m_LogFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
		//		m_LogFont.lfQuality			= PROOF_QUALITY;
		//		m_LogFont.lfPitchAndFamily	= DEFAULT_PITCH;
		strcpy(m_LogFont.lfFaceName,"宋体");
	}

	//20090721 YJT 将这句话移到前面 m_dwAxesStyle = AXIS_LEFT | /*AXIS_DRAWBOUNDARY | */AXIS_LEFTMARGIN | AXIS_RIGHTMARGIN;
	m_cSelectedStyle = -1;

	//	m_Bitmap.m_hObject = NULL;
}

void CGraphics::SetPrintScale(HDC& hDC, RECT& rect)
{
	int Width     = ::GetDeviceCaps(hDC, HORZRES);
	int Height    = ::GetDeviceCaps(hDC, VERTRES);
	int wd		  = abs(rect.right - rect.left);
	int ht		  = abs(rect.bottom - rect.top);
	if( (Width + Height) != 0)
	{
		m_nPrintScale = (wd + ht) / (Width + Height);
	}
	else
		m_nPrintScale = 1;
	m_bPrinting	  = true;
	RecalcRects(rect);
}

CRect* CGraphics::GetMargin(CRect& rect,DWORD dwStyle /*= 0*/,CDC* pDC /*= NULL*/)
{
	if( pDC != NULL )
	{
		SetSizeByFont(pDC);
	}

	if( IsAxesStyle(AXIS_LEFTMARGIN) )
	{
		//if( (rect.left & 0x0F) != KIND_INDEX ) // 股票是指数
		if( !MakeIndexMarket(rect.left) )
		{
			m_rcMargin.left = m_rcDefMargin.left.m_nDef;// +
			//m_rcDefMargin.left.m_nMax;
		}
		else
		{
			m_rcMargin.left = m_rcDefMargin.left.m_nDef / 8.00 * 8;// + 
			//m_rcDefMargin.left.m_nMax;
		}
	}
	else
	{
		m_rcMargin.left = 0;
	}

	if( dwStyle & PKLINE_TYPE_FENSHI )
	{
		rect.top = m_rcDefMargin.left.m_nMin + 1; // cell height
	}
	else
	{
		rect.top = m_rcDefMargin.left.m_nMin + 3; // cell height
	}

	if( IsAxesStyle(AXIS_TOPMARGIN) )
	{
		m_rcMargin.top = m_rcDefMargin.top.m_nDef;
	}
	else if( IsAxesStyle(AXIS_FIRSTTEXT) )
	{
		if( dwStyle & PKLINE_TYPE_FENSHI )
		{
			m_rcMargin.top = m_rcDefMargin.left.m_nMin / 2 + 5;//m_rcDefMargin.left.m_nMin / 2 + 1;
		}
		else
		{
			m_rcMargin.top = m_rcDefMargin.top.m_nDef;
		}
	}
	else
	{
		m_rcMargin.top = 0;
	}

	if( IsAxesStyle(AXIS_RIGHTMARGIN) )
	{
		if( dwStyle & PKLINE_TYPE_FENSHI )
		{
			m_rcMargin.right = m_rcDefMargin.right.m_nDef;
		}
		else
		{
			//m_rcMargin.right = 50;
			if( !MakeIndexMarket(rect.left) )//yulx add 左右一样
			{
				m_rcMargin.right = m_rcDefMargin.left.m_nDef;
			}
			else
			{
				m_rcMargin.right = m_rcDefMargin.left.m_nDef / 8.00 * 8;
			}
		}
	}
	else
	{
		m_rcMargin.right = 0; // 1
	}

	if( IsAxesStyle(AXIS_BOTTOMMARGIN) )
	{
		if( dwStyle & PKLINE_TYPE_FENSHI )
		{
			m_rcMargin.bottom = m_rcDefMargin.bottom.m_nDef + 3;
		}
		else
		{
			m_rcMargin.bottom = m_rcDefMargin.bottom.m_nDef + 1;
		}
	}
	else
	{
		m_rcMargin.bottom = 0;
	}

	return &m_rcMargin;
}

void CGraphics::RecalcRects(RECT& rt)
{
	RECT rcTmp = rt;
	//	rcTmp.left   += rtMargin.left;
	//	rcTmp.right  -= rtMargin.right;
	//	rcTmp.top    += rtMargin.top;
	//	rcTmp.bottom -= rtMargin.bottom;

	//m_Rect = rt;
	m_ClientRect = rcTmp;
	SetPixelRect(rcTmp);	


	m_PlotRect.left		= m_Rect.left + m_rcMargin.left;
	if (m_bEnableLegend)
		m_PlotRect.right= m_Rect.right - m_rcMargin.right;
	else
		m_PlotRect.right= m_Rect.right - m_rcMargin.right;
	m_PlotRect.top		= m_Rect.top + m_rcMargin.top;
	m_PlotRect.bottom	= m_Rect.bottom - m_rcMargin.bottom;

	m_GL = m_PlotRect.left;
	m_GR = m_PlotRect.right;

	m_GT = m_PlotRect.top;
	m_GB = m_PlotRect.bottom ;
	m_PX = m_GR - m_GL;
	m_PY = m_GB - m_GT;

	//
	if(m_PX != 0)
	{
		m_Scale.dx   = (m_Scale.xmax- m_Scale.xmin) / m_PX;
	}
	else
	{
		m_Scale.dx = 0;
	}
	if(m_PY != 0)
	{
		m_Scale.dy   = (m_Scale.ymax- m_Scale.ymin) / m_PY;
	}
	else
	{
		m_Scale.dy = 0;
	}
	m_Size.cx = (m_PY < m_PX) ? m_PY : m_PX;

	m_rcExplain.top    = m_Rect.top + 1;
	m_rcExplain.bottom = m_Rect.top + m_rcDefMargin.top.m_nDef - 1;
}
void CGraphics::BeginDraw(CHSBaseDC* pDC)						
{ 
	if (m_bPrinting || !m_bMemoryDraw)
	{
		m_pDC = pDC;
		m_nPreMode = m_pDC->SetBkMode(TRANSPARENT);		
	}
	else
	{
		/*
		pDC->GetClipBox(&m_ClipBox);
		m_pDC->CreateCompatibleDC(pDC);
		m_Bitmap.CreateCompatibleBitmap( pDC, m_ClipBox.right - m_ClipBox.left, 
		m_ClipBox.bottom - m_ClipBox.top);
		m_OldBitmap = m_pDC->SelectObject(&m_Bitmap);
		m_pDC->SetWindowOrg(m_ClipBox.left, m_ClipBox.top);
		*/
	}
	DrawBkGround();
	DrawBoundary(GetBorderColor(), 0);
}

void CGraphics::EndDraw(CHSBaseDC* pDC)
{
	if (m_bPrinting || !m_bMemoryDraw)
	{
		m_pDC->SetBkMode(m_nPreMode);
		m_bPrinting = false;
	}
	m_pDC = NULL;
	/*
	pDC->BitBlt( m_ClipBox.left, m_ClipBox.top, m_ClipBox.right - m_ClipBox.left,
	m_ClipBox.bottom - m_ClipBox.top, m_pDC, m_ClipBox.left, m_ClipBox.top, SRCCOPY);
	m_pDC->SelectObject(m_OldBitmap);
	m_Bitmap.DeleteObject();
	m_pDC->DeleteDC();
	m_pDC = NULL;
	*/
}

void CGraphics::DrawBkGround()
{
	if( !IsAxesStyle(AXIS_DRAWBACK) )
		return;

	CBrush hBrush;
	hBrush.CreateSolidBrush(**m_pBackColor);
	CBrush* pOldBrush = m_pDC->SelectObject(&hBrush);
	m_pDC->Rectangle( &m_ClientRect );
	m_pDC->SelectObject(pOldBrush);	
	hBrush.DeleteObject();
}

void CGraphics::AddUsrDefAxis( double dwAxis,BOOL bDelAll /*= FALSE*/ )
{
	if( bDelAll )
	{
		m_ayUsrDefAxis.RemoveAll();
	}
	else
	{
		m_ayUsrDefAxis.Add(dwAxis);
	}
}

void CGraphics::SetYRefParam(double min /*= 0*/, double max /*= 0*/, 
							 double unit /*= 1*/, double ref /*= 0*/,
							 int nDiv /*= 1*/, int nDecimal /*= 0*/,
							 int lineWidth /*= 0*/,
							 CHSBaseDC* pDC /*= NULL*/)
{
	//	if(min != -1)
	//	{
	m_YRefParam.min  = min;
	//	}
	//	if(max != -1)
	//	{
	m_YRefParam.max  = max;
	//	}
	if(unit != -1)
	{
		m_YRefParam.unit = unit;
	} 
	if(ref != -1 && (double)hx_DefMinValues != ref ) // hx_DefMinValues 于2003-07-29由吕连新加入
	{
		m_YRefParam.ref  = ref;
	}
	if(nDiv != -1)
	{
		m_YRefParam.nDiv = nDiv;
	}
	if(nDecimal != -1)
	{
		m_YRefParam.nDecimal  = nDecimal;
	}
	if(lineWidth != -1)
	{
		m_YRefParam.lineWidth  = lineWidth;
	}

	// CalcLeftRightWidth(pDC);

}

void CGraphics::SetXRefParam(double min /*= 0*/, double max /*= 0*/, 
							 double unit /*= 1*/, double ref /*= 0*/,
							 int nDiv /*= 1*/, int nDecimal /*= 0*/,
							 int lineWidth /*= 0*/,
							 CHSBaseDC* pDC /*= NULL*/)
{
	if(min != -1)
		m_XRefParam.min  = min;
	if(max != -1)
		m_XRefParam.max  = max;
	if(unit != -1)
		m_XRefParam.unit = unit;
	if(ref != -1)
		m_XRefParam.ref  = ref;
	if(nDiv != -1)
		m_XRefParam.nDiv = nDiv;	
	if(nDecimal != -1)
		m_XRefParam.nDecimal  = nDecimal;
	if(lineWidth != -1)
		m_XRefParam.lineWidth  = lineWidth;
}

void CGraphics::SetMargin(CMarginParam margin,char cType)
{
	if(cType == CMarginParam::left)
	{
		m_rcDefMargin.left = margin;
	}
	else if(cType == CMarginParam::top)
	{
		m_rcDefMargin.top    = margin;
	}
	else if(cType == CMarginParam::right)
	{
		m_rcDefMargin.right  = margin;
	}
	else if(cType == CMarginParam::bottom)
	{
		m_rcDefMargin.bottom = margin;
	}
}

CGraphics::CMarginParam* CGraphics::GetMargin(char cType)
{
	if(cType == CMarginParam::left)
	{
		return &m_rcDefMargin.left;
	}
	else if(cType == CMarginParam::top)
	{
		return &m_rcDefMargin.top;
	}
	else if(cType == CMarginParam::right)
	{
		return &m_rcDefMargin.right;
	}
	else if(cType == CMarginParam::bottom)
	{
		return &m_rcDefMargin.bottom;
	}
	return NULL;
}

void CGraphics::SetMargin(CRect rect)
{
	m_rcMargin.left   = IsAxesStyle(AXIS_LEFTMARGIN)? rect.left:0;
	m_rcMargin.top    = IsAxesStyle(AXIS_TOPMARGIN)?  rect.top:0;
	m_rcMargin.right  = IsAxesStyle(AXIS_RIGHTMARGIN)?rect.right:0;
	m_rcMargin.bottom = IsAxesStyle(AXIS_BOTTOMMARGIN)?rect.bottom:0;
}

void CGraphics::SetTickLineLen(unsigned char lm /*= 3*/,unsigned char tm /*= 3*/,
							   unsigned char rm /*= 3*/,unsigned char bm /*= 4*/)
{
	m_TickLineLen.lm = lm;
	m_TickLineLen.tm = tm;
	m_TickLineLen.rm = rm;
	m_TickLineLen.bm = bm;
}

void CGraphics::SetRatio(double xmin, double ymin, double xmax, double ymax)
{
	m_Ratio.xmin = xmin;
	m_Ratio.ymin = ymin;
	m_Ratio.xmax = xmax;
	m_Ratio.ymax = ymax;
}

void CGraphics::GetPixelRect(RECT& rt)
{
	rt.left	  = m_Rect.left;
	rt.top	  = m_Rect.top;
	rt.right  = m_Rect.right;
	rt.bottom = m_Rect.bottom;
}

void CGraphics::SetPixelRect(RECT rt)
{
	LONG Width    = rt.right  - rt.left;
	LONG Height   = rt.bottom - rt.top; 
	m_Rect.left   = (LONG)(rt.left + m_Ratio.xmin * Width);
	m_Rect.top	  = (LONG)(rt.top + m_Ratio.ymin * Height);
	m_Rect.right  = (LONG)(rt.right - (1 - m_Ratio.xmax) * Width);
	m_Rect.bottom = (LONG)(rt.bottom - (1 - m_Ratio.ymax) * Height);
}

void CGraphics::Title(const char* Title, int Pos)
{
	GetFontSize(ftTitle);
	CFont font;
	if ( font.CreateFontIndirect(&m_LogFont) )
	{
		CFont* hOldFont = m_pDC->SelectObject(&font);
		m_pDC->SetTextColor(RGB(0, 0, 0)); 
		SetStringAlign(DT_CENTER, DT_VCENTER);
		if (Pos == DT_TOP)
		{
			PrintString((m_GL + m_GR) / 2, (m_Rect.top + m_GT ) / 2 + 1, 0, Title);
			m_pDC->SetTextColor(GetTitleColor().m_lColor); 
			PrintString((m_GL + m_GR) / 2, (m_Rect.top + m_GT ) / 2, 0, Title);
		}
		else
		{
			int n = m_Rect.bottom - (m_Rect.bottom - m_GB) / 3;
			PrintString((m_GL + m_GR) / 2, n + 1, 0, Title);
			m_pDC->SetTextColor(GetTitleColor().m_lColor); 
			PrintString((m_GL + m_GR) / 2, n, 0, Title);
		}
		m_pDC->SelectObject(hOldFont);
		font.DeleteObject();		
	}
}

void CGraphics::XAxisTitle(const char* Title, int Pos)
{  
	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftXAxis));
	SetStringAlign(Pos, DT_VCENTER);
	if (Pos == DT_RIGHT)
	{
		//----2010.1.12 litao 修改星空图的横坐标显示位置-----------------------------
		PrintString(m_GR - 8, m_GB - (m_rcMargin.bottom / 2) + 1 , 0, Title);
	}
	else
	{
		//----2010.1.12 litao 修改星空图的横坐标显示位置-----------------------------
		PrintString(m_GL + 8, m_GB - (m_rcMargin.bottom / 2) + 1, 0, Title);
	}
	m_pDC->SelectObject(hOldFont);
}

void CGraphics::YAxisTitle(const char* Title, int Pos)
{ 
	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftYAxis));
	CRect rc;
	if (Pos == DT_TOP)
	{
		//----2010.1.12 litao 修改星空图的纵坐标显示位置-----------------------------
		rc.SetRect(m_GL - m_rcMargin.left + 8, m_GT+8, m_GL - 4,
			m_GT + m_PY / m_YGridTicks);			
		m_pDC->DrawText(Title, strlen(Title), &rc, DT_TOP|DT_LEFT|DT_WORDBREAK);
	}
	else
	{
		rc.SetRect(m_GL - m_rcMargin.left + 8, 
			m_GB - m_PY / m_YGridTicks, m_GL - 4,
			m_GB - 1);			
		m_pDC->DrawText(Title, strlen(Title), &rc, DT_BOTTOM|DT_LEFT|DT_WORDBREAK);
	}
	m_pDC->SelectObject(hOldFont);
}

void CGraphics::DrawBoundary(ColorProperty& cr, int nMargin)
{
	if( !IsAxesStyle(AXIS_DRAWBOUNDARY) )
		return;

	CPen hPen;
	hPen.CreatePen(cr.m_nPenStyle, cr.m_nWidth, cr.m_lColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	if (m_bPrinting)	nMargin *= m_nPrintScale;
	DrawRectangle(m_Rect.left + nMargin, m_Rect.top + nMargin, 
		m_Rect.right - nMargin, m_Rect.bottom - nMargin);

	m_pDC->SelectObject(hOldPen);	
	hPen.DeleteObject();
}

void CGraphics::Legend(COLORREF cr, int Index, const char* Name)
{
	GetFontSize(ftLegend);
	CFont font;
	if ( font.CreateFontIndirect(&m_LogFont) )
	{
		int n  = (m_Rect.right - m_GR) / 20 + 1;
		int xb = m_GR + 2 * n;
		int xe = xb + 4 * n;
		int y  = m_GT - 3 * Index * m_LogFont.lfHeight / 2;
		DrawLine(xb, y, xe, y);
		CFont* hOldFont = m_pDC->SelectObject(&font);
		m_pDC->SetTextColor(cr); 
		SetStringAlign(DT_LEFT, DT_VCENTER);
		PrintString(xe + n, y, 0, Name);
		m_pDC->SelectObject(hOldFont);
		font.DeleteObject();		
	}
}

void CGraphics::DrawCircle(int x, int y, int radius)
{
	int x1 = x - radius;
	int y1 = y - radius;
	int x2 = x + radius;
	int y2 = y + radius;
	m_pDC->Arc(x1, y1, x2, y2, x, y2, x, y2);
}

//add by yuhj 2008.5.14
void CGraphics::DrawCross(POINT &pt, int radius)
{
	m_pDC->MoveTo(pt.x - radius, pt.y - radius);
	m_pDC->LineTo(pt.x + radius, pt.y + radius);

	m_pDC->MoveTo(pt.x + radius, pt.y - radius);
	m_pDC->LineTo(pt.x - radius, pt.y + radius);
}
//end

void CGraphics::DrawFilledCircle(int x, int y, int radius)
{
	int x1 = x - radius;
	int y1 = y - radius;
	int x2 = x + radius;
	int y2 = y + radius;
	m_pDC->Ellipse( x1, y1, x2, y2);
}

void CGraphics::DrawRectangle(int x1, int y1, int x2, int y2)
{
	m_pDC->MoveTo(x1, y1);
	m_pDC->LineTo(x1, y2);
	m_pDC->LineTo(x2, y2);
	m_pDC->LineTo(x2, y1);
	m_pDC->LineTo(x1, y1);
}

void CGraphics::PrintString(int x, int y, int theta, const char* fmt,
							UINT nFormat /*= DT_SINGLELINE|DT_END_ELLIPSIS|DT_WORDBREAK*/,
							short sType /*= PrintString_Float*/,
							CFont* pCurFont /*= NULL*/)
{
	WORD    Height, Width;
	//	UINT    PreSet;
	double  thta;

	//PreSet = m_pDC->SetTextAlign(TA_LEFT|TA_TOP);

	CSize size = m_pDC->GetTextExtent(fmt, lstrlen(fmt));
	Height = (WORD)size.cy;
	Width  = (WORD)size.cx;

	thta   = PiV*theta/ConstV;
	if(m_StrAlign.HAlign == DT_LEFT && m_StrAlign.VAlign == DT_TOP)
	{
	} 
	else if(m_StrAlign.HAlign == DT_LEFT && m_StrAlign.VAlign == DT_VCENTER)
	{     
		x = (int)(x - Height/2.*sin(thta));
		//y = (int)(y + Width * sin(thta) - Height/2.*cos(thta));
		y = (int)(y - Height/2.);//*cos(thta));
	}
	else if(m_StrAlign.HAlign == DT_CENTER && m_StrAlign.VAlign == DT_TOP)
	{
		x = (int)(x - Width/2. * cos(thta));
		y = (int)(y /*+ Height/2.*/);//. * sin(thta));
	}
	else if(m_StrAlign.HAlign == DT_CENTER && m_StrAlign.VAlign == DT_VCENTER)
	{
		x = (int)(x - Width/2. * cos(thta) - Height/2.*sin(thta));
		y = (int)(y + Width/2. * sin(thta) - Height/2.*cos(thta));
	}
	else if(m_StrAlign.HAlign == DT_CENTER && m_StrAlign.VAlign == DT_BOTTOM)
	{
		x = (int)(x - Width/2. * cos(thta) - Height*sin(thta));
		y = (int)(y + Width/2. * sin(thta) - Height*cos(thta));
	}
	else if(m_StrAlign.HAlign == DT_RIGHT && m_StrAlign.VAlign == DT_TOP)
	{
		x = (int)(x - Width * cos(thta));
		y = (int)(y + Width * sin(thta));
	}
	else if(m_StrAlign.HAlign == DT_RIGHT && m_StrAlign.VAlign == DT_VCENTER)
	{
		x = (int)(x - Width * cos(thta) - Height/2.*sin(thta));
		y = (int)(y + Width * sin(thta) - Height/2.*cos(thta));
	}
	else if(m_StrAlign.HAlign == DT_RIGHT && m_StrAlign.VAlign == DT_BOTTOM)
	{
		x = (int)(x - Width * cos(thta) - Height*sin(thta));
		y = (int)(y + Width * sin(thta) - Height*cos(thta));
	}
	else //if(m_StrAlign.HAlign == DT_LEFT && m_StrAlign.VAlign == DT_BOTTOM)
	{             
		x = (int)(x - Height*sin(thta));
		y = (int)(y - Height*cos(thta));
	} 

	if( sType & PrintString_Rect )
	{
		CPen hPen;
		hPen.CreatePen(GetBorderColor().m_nPenStyle,GetBorderColor().m_nWidth,GetBorderColor().m_lColor);
		CPen* hOldPen = m_pDC->SelectObject(&hPen);

		Height -= 3;
		DrawLine(x, y,x + Width,y);
		DrawLine(x + Width, y,x + Width,y + Height);
		DrawLine(x + Width, y + Height,x,y + Height);
		DrawLine(x, y + Height,x,y);

		m_pDC->SelectObject(hOldPen);
	}

	if( (sType & PrintString_Float) && 0 )
	{
		if( pCurFont == NULL )
		{
			pCurFont = m_pDC->GetCurrentFont();
		}

		if( pCurFont != NULL )
		{
/*			CFont* pFloat = g_hsGraphicsData.CreateFloatFont((HFONT)pCurFont->m_hObject);
			if( pFloat != NULL )
			{
				CRect textRect;
				textRect.left = x;
				textRect.top  = y;
				textRect.right  = textRect.left + Width;
				textRect.bottom = textRect.top + Height;				
				g_hsGraphicsData.DrawFloatNumber(m_pDC->m_hDC,fmt, -1, &textRect,
					nFormat | m_StrAlign.HAlign | m_StrAlign.VAlign, 
					pFloat->m_hObject,this);				
			}*/
		}

		//if( font.m_hObject != NULL )
		//{
		//	font.DeleteObject();
		//}

	}
	else
	{	
		m_pDC->TextOut(x, y, fmt, lstrlen(fmt));
	}

	//m_pDC->SetTextAlign(PreSet);
}

void CGraphics::Format(int decimal, CString& str, double value)
{
	CString tstr;
	str.Format( "%%.%df", decimal );
	tstr.Format( str, value );

	str = tstr;

	//if ( fabs(value) > 0.99999 )
	{		
		if( str.ReverseFind('.') == str.GetLength() - 1 )
		{
			str.Delete(str.GetLength() - 1,1);//str.Left(str.GetLength() - 1);
		}
	}

#if 0

	char tstr[32];
	sprintf(str, "%%.%df", decimal);
	sprintf(tstr, str, value);
	if (fabs(value) > 0.99999)
	{
		//		_gcvt(atof(tstr), 12, str);
		strncpy(str,tstr,32);

		/*
		int nLen = lstrlen(str);
		for(int nPos = 0; nPos < nLen; nPos++)
		{
		if(str[nPos] == '.')
		{
		int nNext = nLen - nPos - 1;
		nPos = 0;
		for(; nNext < decimal;nNext++,nPos++)
		{
		str[nPos + nLen] = '0';
		}
		break;
		}
		}
		*/
		if (str[lstrlen(str) - 1] == '.')
		{
			str[lstrlen(str) - 1] = '\0';
		}
	}
	else
	{
		if (fabs(value) < 1.0e-10)
		{
			lstrcpy(str, "0");
		}
		else
		{
			strncpy(str,tstr,32);
			//lstrcpy(str, tstr);
			//			int n = lstrlen(str) - 1;
			//			while(str[n] == '0' && n >= 0)
			//				str[n--] = '\0';
		}
	}
#endif
}

void CGraphics::DrawExplainRect()
{
	return; // 不处理

	if( m_rcExplain.IsRectEmpty() )
		return;

//	m_pDC->FillSolidRect(m_rcExplain, g_hsGraphicsData.crHightBkColor.m_lColor/*::GetSysColor(COLOR_HIGHLIGHT)*/);	

	if( m_MenuBitmap.m_hImageList != NULL )
	{
		POINT imagePt;
		imagePt.x = m_rcExplain.left;
		//imagePt.x -= m_MenuSize.cx;
		imagePt.y = m_rcExplain.top+(m_rcExplain.Height() - m_MenuSize.cy)/2;
		m_MenuBitmap.Draw(m_pDC,0,imagePt,0);					
	}

}

void CGraphics::DrawActive(BOOL bActive)
{
/*  if(bActive)
  {
		CRect rc;
		if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_RIGHT_AXES) )
		{
			rc.SetRect(m_GR+1,m_Rect.top,m_Rect.right + 2,m_GB);
		}
		else
		{
			rc.SetRect(m_GL-1,m_Rect.top,m_Rect.left+1,m_GB);
		}

		m_pDC->Draw3dRect(rc,g_hsGraphicsData.clGridLine.m_lColor,g_hsGraphicsData.clGridLine.m_lColor);
  }*/
}

void CGraphics::Grid_LogY()
{
/*	int i, j;                                    

	if( g_hsGraphicsData.IsStyle(GL_FenShiHorAxes) )
	{
		CPen hPen;
		hPen.CreatePen(GetGridVerColor().m_nPenStyle,
			GetGridVerColor().m_nWidth, GetGridVerColor().m_lColor);
		CPen* hOldPen = m_pDC->SelectObject(&hPen);			
		for(i = 1; i < m_XGridTicks; i ++)
		{
			j = m_GL + (int)(1.0 * i * (m_GR-m_GL) / m_XGridTicks);
			if( (i % 8) == 0 && i != m_XGridTicks)
			{
				DrawMiddleLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceMiddleLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceMiddleLine.m_nPenStyle);
			}
			else if( !(i%2) && IsAxesStyle(AXIS_SOLIDANDDASH) )
			{
				if((i%4)==0)
					DrawMiddleVerLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceMiddleVerLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceMiddleVerLine.m_nPenStyle);

				if((i%4)!=0)
					DrawHourLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceHourLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceHourLine.m_nPenStyle);
//				DrawLine(j,m_GT + 1, j, m_GB - 1, 1,*m_pVerLineColor);			
			}
			else
			{
					DrawHalfHourLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceHalfHourLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceHalfHourLine.m_nPenStyle);
//				DrawLine(j,m_GT + 1, j, m_GB - 1, 3,*m_pVerLineColor);
			}
		}

		m_pDC->SelectObject(hOldPen);
		hPen.DeleteObject();
	}

	//	if(hDotPen.m_hObject != NULL)
	//	{
	//		hDotPen.DeleteObject();
	//	}

	//	CPen hPen1;
	//	hPen1.CreatePen(GetGridHorColor().m_nPenStyle,GetGridHorColor().m_nWidth,GetGridHorColor().m_lColor);
	//	CPen* hOldPen1 = m_pDC->SelectObject(&hPen1);

	// y grid
	if( m_ayUsrDefAxis.GetSize() <= 0 ) // 不是自定义坐标
	{
		CDrawTechKLine* pPKline = GetPKLine();
		for(i = 1; i < m_YGridTicks; i ++) 
		{ 
			j = m_GT + (int)(1.0 * i * (m_GB-m_GT) / m_YGridTicks);
			if( IsAxesStyle(AXIS_FENSHIPRICE) && i == m_YGridTicks/2) // 分时价格线中间横线
			{
				DrawMiddleLine(m_GL, j, m_GR - 1, j,
					g_hsGraphicsData.clJsPriceMiddleLine.m_nWidth, g_hsGraphicsData.clJsPriceMiddleLine.m_nPenStyle);
			}
			else
			{
				if( pPKline != NULL && !pPKline->IsFenshiDrawType() )
				{
					if( g_hsGraphicsData.IsStyle(GL_DayHorAxes) ) // GL_DayVerAxes
					{
						DrawLine(m_GL + 1, j, m_GR - 1, j,3,*m_pHorLineColor);
					}
				}
				else
				{
					if( g_hsGraphicsData.IsStyle(GL_FenShiVerAxes) )
					{
						DrawLine(m_GL + 1, j, m_GR - 1, j,0,*m_pHorLineColor);
					}
				}
			}
		}
	}

	//	m_pDC->SelectObject(hOldPen1);
	//	hPen1.DeleteObject();

	// 大盘多空横线
	if( IsAxesStyle(AXIS_DUOKONGMIDDLE) && !m_YRefParam.IsValid() )
	{
		if( (m_YRefParam.max - m_YRefParam.min) != 0 )
		{
			int y = m_GB - (int)((float)m_PY * 
				(0 - m_YRefParam.min) / (m_YRefParam.max - m_YRefParam.min) + 0.5);
			DrawMiddleLine(m_GL + 1, y, m_GR - 1, y,1);		
		}
	}
*/
}

void CGraphics::Grid()
{
	int i, j;                                    

	if( g_hsGraphicsData.IsStyle(GL_FenShiHorAxes) )
	{
		CPen hPen;
		hPen.CreatePen(GetGridVerColor().m_nPenStyle,
			GetGridVerColor().m_nWidth, GetGridVerColor().m_lColor);
		CPen* hOldPen = m_pDC->SelectObject(&hPen);			
		for(i = 1; i < m_XGridTicks; i ++)
		{
			j = m_GL + (int)(1.0 * i * (m_GR-m_GL) / m_XGridTicks);
			if( (i % 8) == 0 && i != m_XGridTicks)
			{
				DrawMiddleLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceMiddleLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceMiddleLine.m_nPenStyle);
			}
			else if( !(i%2) && IsAxesStyle(AXIS_SOLIDANDDASH) )
			{

			//	DrawLine(j, m_GT + 1, j, m_GB - 1,1,*m_pVerLineColor);	
				if((i%4)==0)
					DrawMiddleVerLine(j+1, m_GT + 1, j+1, m_GB - 1,  //加1其实没啥影响的，只是让11:30那条线看着更像在上下午分割线上
					g_hsGraphicsData.clJsPriceMiddleVerLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceMiddleVerLine.m_nPenStyle);  //此句2008-11-4添加用于画分时图中的上下午分割线

				if((i%4)!=0)
					DrawHourLine(j, m_GT + 1, j, m_GB - 1,        
					g_hsGraphicsData.clJsPriceHourLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceHourLine.m_nPenStyle);  //此句2008-11-4添加，用于画分时图中的小时线
			}
			else
			{
				//DrawLine(j, m_GT + 1, j, m_GB - 1,3,*m_pVerLineColor);
				DrawHalfHourLine(j, m_GT + 1, j, m_GB - 1,
					g_hsGraphicsData.clJsPriceHalfHourLine.m_nWidth, 
					g_hsGraphicsData.clJsPriceHalfHourLine.m_nPenStyle); //此句2008-11-4添加，用于画分时图中的半小时线
			}
		}

		m_pDC->SelectObject(hOldPen);
		hPen.DeleteObject();
	}

	if( m_ayUsrDefAxis.GetSize() <= 0 && m_YGridTicks) // 不是自定义坐标
	{
		int   center;
		float valuePer;
		if(m_nAxesType == YLN)
		{
			valuePer = (m_YRefParam.max - m_YRefParam.min) / m_YGridTicks;
			for(i = 1; i < m_YGridTicks; i++)
				m_ayUsrDefAxis.Add(valuePer * i);
			if(m_nAxesType == YLN)
				valuePer = (float)exp((log(m_YRefParam.max)-log(m_YRefParam.min))/(m_YGridTicks));        
			else
				valuePer = (float)pow((double)10,(log10(m_YRefParam.max)-log10(m_YRefParam.min))/(m_YGridTicks));        
			center = m_YGridTicks/2;
		}
		else if(m_nAxesType == YEQU)
		{
			valuePer = (m_YRefParam.max - m_YRefParam.min) / 4;
			for(i = 1; i < 4; i++)
				m_ayUsrDefAxis.Add(valuePer * i);
			center = 1;
		}
		else if(m_nAxesType == YGLD)
		{
			valuePer = (m_YRefParam.max - m_YRefParam.min) * 0.191;
			m_ayUsrDefAxis.Add(valuePer);
			valuePer = (m_YRefParam.max - m_YRefParam.min) * 0.382;
			m_ayUsrDefAxis.Add(valuePer);
			valuePer = (m_YRefParam.max - m_YRefParam.min) * 0.5;
			m_ayUsrDefAxis.Add(valuePer);
			valuePer = (m_YRefParam.max - m_YRefParam.min) * 0.618;
			m_ayUsrDefAxis.Add(valuePer);
			valuePer = (m_YRefParam.max - m_YRefParam.min) * 0.809;
			m_ayUsrDefAxis.Add(valuePer);
			center = 2;
		}

		CDrawBase* pPKline = GetPKLine();

		if(m_nAxesType == YGLD || m_nAxesType == YEQU || m_nAxesType == YLN)
		{

			for(i = 0; i < m_ayUsrDefAxis.GetSize(); i++)
			{
				j = m_GT + (int)((m_GB - m_GT) * m_ayUsrDefAxis[i] / (m_YRefParam.max - m_YRefParam.min));
				if( IsAxesStyle(AXIS_FENSHIPRICE) && i == center) // 分时价格线中间横线
				{
					DrawMiddleLine(m_GL, j, m_GR - 1, j,
						g_hsGraphicsData.clJsPriceMiddleLine.m_nWidth, g_hsGraphicsData.clJsPriceMiddleLine.m_nPenStyle);
				}
				else
				{
					if( pPKline != NULL && !pPKline->IsDrawType(DrawType_FenShi) )
					{
						if(m_nAxesType	== YLOG || m_nAxesType == YLN)
						{
							float value = m_YRefParam.max / pow(valuePer,(m_YGridTicks - i - 1));
							POINT pt = GetPoint(0,ValueToVertPos(value));
							j = pt.y;
						}

						if(g_hsGraphicsData.IsStyle(GL_DayHorAxes) ) // GL_DayVerAxes
						{
							DrawLine(m_GL + 1, j, m_GR - 1, j,3,*m_pHorLineColor);
						}
					}
					else
					{
						if(g_hsGraphicsData.IsStyle(GL_FenShiVerAxes) )
						{
							DrawLine(m_GL + 1, j, m_GR - 1, j,0,*m_pHorLineColor);
						}
					}
				}
			}

		}
		else
		{

			if( m_nAxesType	== YLOG )
			{
				if( m_YGridTicks != 0 )
					valuePer = (float)pow((double)10,(log10(m_YRefParam.max)-log10(m_YRefParam.min))/(m_YGridTicks));
				else
					valuePer = 1;
			}

			for(i = 1; i < m_YGridTicks; i ++) 
			{ 
				j = m_GT + (int)(1.0 * i * (m_GB-m_GT) / m_YGridTicks);
				if( IsAxesStyle(AXIS_FENSHIPRICE) && i == m_YGridTicks/2) // 分时价格线中间横线
				{
					DrawMiddleLine(m_GL, j, m_GR - 1, j,
						g_hsGraphicsData.clJsPriceMiddleLine.m_nWidth, g_hsGraphicsData.clJsPriceMiddleLine.m_nPenStyle);
				}
				else
				{
					if( pPKline != NULL && !pPKline->IsDrawType(DrawType_FenShi) )
					{
						if( m_nAxesType	== YLOG )
						{
							float value = m_YRefParam.max / pow(valuePer,(m_YGridTicks - i));
							POINT pt = GetPoint(0,ValueToVertPos(value));
							j = pt.y;
						}

						if(g_hsGraphicsData.IsStyle(GL_DayHorAxes) ) // GL_DayVerAxes
						{
//							DrawLine(m_GL + 1, j, m_GR - 1, j,NULL,*m_pHorLineColor);//覃宏念2008-11-5将3改为NULL使日线图，网格横线可改变大小
							DrawLine(m_GL + 1, j, m_GR - 1, j,NULL, &g_hsGraphicsData.clDayHorLine);
						}
					}
					else
					{
						if( g_hsGraphicsData.IsStyle(GL_FenShiVerAxes) )
						{
							DrawLine(m_GL + 1, j, m_GR - 1, j,0,*m_pHorLineColor);  
						}
					}
				}
			}

		}
		m_ayUsrDefAxis.RemoveAll();
	}

	// 大盘多空横线
	if( IsAxesStyle(AXIS_DUOKONGMIDDLE) && !m_YRefParam.IsValid() )
	{
		if( (m_YRefParam.max - m_YRefParam.min) != 0 )
		{
			int y = m_GB - (int)((float)m_PY * 
				(0 - m_YRefParam.min) / (m_YRefParam.max - m_YRefParam.min) + 0.5);
			DrawMiddleLine(m_GL + 1, y, m_GR - 1, y,1);		
		}
	}
}
void CGraphics::DrawMiddleLine(int x,int y,int x1,int y1,int LineWidth /*= 2*/, int LineStyle /*= PS_SOLID*/)
{
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth,g_hsGraphicsData.clJsPriceMiddleLine.GetColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	DrawLine(x, y, x1, y1);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

void CGraphics::DrawMiddleVerLine(int x,int y,int x1,int y1,int LineWidth /*= 2*/, int LineStyle /*= PS_SOLID*/)
{
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth,g_hsGraphicsData.clJsPriceMiddleVerLine.GetColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	DrawLine(x, y, x1, y1);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

void CGraphics::DrawHourLine(int x,int y,int x1,int y1,int LineWidth /*= 1*/, int LineStyle /*= PS_SOLID*/)
{
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth,g_hsGraphicsData.clJsPriceHourLine.GetColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	DrawLine(x, y, x1, y1);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

void CGraphics::DrawHalfHourLine(int x,int y,int x1,int y1,int LineWidth /*= 1*/, int LineStyle /*= PS_DOT*/)
{
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth,g_hsGraphicsData.clJsPriceHalfHourLine.GetColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	DrawLine(x, y, x1, y1);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}
 void CGraphics::DrawKLineTopLine(int x,int y,int x1,int y1,int LineWidth /*= 1*/, int LineStyle /*= PS_DOT*/)
{
	CPen hPen;
	hPen.CreatePen(LineStyle, LineWidth,g_hsGraphicsData.clDayTopLine.GetColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	DrawLine(x, y, x1, y1);

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}
void CGraphics::XAxis()
{
	int	  xb, yb, xe, ye;   
	int   i;                                  
	CString  str;
	//	memset(str,0,sizeof(str));

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftXAxis));
	yb = m_GB;
	CSize size;
	int xx = -1;
	float value;

	for(i = 0; i <= m_XTicks && m_XTicks != 0; i ++) 
	{                                     
		if( i == 0 &&  
			IsAxesStyle(AXIS_NOTDRAWFIRST) )
			continue;
		if( i == m_XTicks && 
			IsAxesStyle(AXIS_NOTDRAWLAST) )
			continue;

		xb = xe = (int)(m_GL + 1.0 * m_PX * i / m_XTicks );
		if((i % m_XTicksUnit) == 0)
		{
			ye = m_GB + m_TickLineLen.bm;
			//value = (float)(m_Scale.xmin + i * (m_Scale.xmax - m_Scale.xmin) / m_XTicks);
			if( IsAxesStyle(AXIS_FENSHIHORI) && m_XTicksUnit != 0 )
			{				
				GetFenShiTimeStr(m_XRefParam.ref + m_XRefParam.nDiv * (i / m_XTicksUnit),
					str,(i == m_XTicks));
			}
			if( IsAxesStyle(AXIS_HIGHFREQUENCYHORI) && m_XTicksUnit != 0 )
			{				
				GetHighFrequencyTimeStr(m_XRefParam.ref + m_XRefParam.nDiv * (i / m_XTicksUnit),
					str,(i == m_XTicks));
			}
			else // xkt
			{
				value = 
					(float)(m_XRefParam.min + i * (m_XRefParam.max - m_XRefParam.min) / m_XTicks);
				if(m_XRefParam.nDiv != 0)
					value /= m_XRefParam.nDiv;
				Format(m_XRefParam.nDecimal, str, value);
				//goto _XAxis_;				
			}

			size = m_pDC->GetTextExtent(str, lstrlen(str));

			if( /*i == m_XTicks 
				||*/ (xx != -1 && (xb - xx) < (size.cx + 1))
				|| size.cx == 0)
				continue;

			PrintString(xb, m_GB + m_TickLineLen.bm, 0, str);
			str.Empty();

			xx = xb;
		}
		else 
		{
			ye = m_GB + 2;//m_TickLineLen.bm;
		}
		if (i > 0 && i < m_XTicks)
		{
			DrawLine(xb, yb, xe, ye);
		}
	}

	//_XAxis_:		
	m_pDC->SelectObject(hOldFont);
	//m_pDC->SetTextColor(colorOld);
}

void CGraphics::GetFenShiTimeStr(int nPos,CString& strBuf,BOOL bLast /*= FASLE*/)
{
	strBuf = "00:00";
}

void CGraphics::GetHighFrequencyTimeStr(int nPos,CString& strBuf,BOOL bLast/* = FALSE*/)
{	
	CTime tm = CTime::GetCurrentTime();
	if( nPos <= 2 * 60* 60 ) //9:30 --- 11:30
	{
		CTime tmStart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,30,0);
		int nH = nPos / 3600; //小时
		int nM= ( nPos - (nH * 3600) ) / 60; //分	
		int nS = ( nPos - (nH * 3600) ) %60;;
		CTimeSpan ts(0,nH,nM,nS);
		CTime tmPos1 = tmStart + ts;
		strBuf.Format("%02d:%02d",tmPos1.GetHour(),tmPos1.GetMinute());
	}
	else 
	{
		CTime tmStart(tm.GetYear(),tm.GetMonth(),tm.GetDay(),13,00,0);
		nPos = nPos -   2 * 60* 60;
		int nH = nPos / 3600; //小时
		int nM= ( nPos - (nH * 3600) ) / 60; //分	
		int nS = ( nPos - (nH * 3600) ) %60;;
		CTimeSpan ts(0,nH,nM,nS);
		CTime tmPos1 = tmStart + ts;
		strBuf.Format("%02d:%02d",tmPos1.GetHour(),tmPos1.GetMinute());
	}
}

void CGraphics::YAxis_LogY()
{
/*
	if( !m_YRefParam.IsValid() )
		return;

	int	  xb, yb, xe, ye;
	int   i;
	CString  str;
	float value;

	double dMin = m_YRefParam.min;
	double dMax = m_YRefParam.max;
	double dMulti = 0;
	CDrawTechKLine* pPKline = GetPKLine();
	if( pPKline != NULL && !pPKline->IsFenshiDrawType() ) // 计算坐标文本是否超过显示边界
	{
		int    nLeftWidth = GetYLeftMaxWidth(m_pDC,TRUE);
		float nCharWidths = LOWORD(nLeftWidth);
		short  nCharCount = HIWORD(nLeftWidth);
		if(nCharCount > 0)
		{
			nLeftWidth  = (nCharWidths - m_rcDefMargin.left.m_nDef);
			nCharWidths = nCharWidths / nCharCount;
			if( nCharWidths > 0 && nLeftWidth > 0 )
			{
				dMulti = (nLeftWidth / nCharWidths);
				if(dMulti > (int)dMulti)
				{
					dMulti = (int)dMulti + 1;
				}
				else
				{
					dMulti = (int)dMulti;
				}
				dMulti = pow((int)10,(int)dMulti);
				if(dMulti != 0)
				{
					dMax /= dMulti;
					dMin /= dMulti;
				}
			}
		}
	}

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftYAxis));

	xe = m_GL;
	for(i = 0; i <= m_YTicks && m_YTicks != 0; i ++)     
	{
		yb = ye = (int)(m_GT + 1.0 * m_PY * i / m_YTicks );
		if((i % m_YTicksUnit) == 0 && m_YTicksUnit != 0) 
		{
			xb = m_GL - m_TickLineLen.lm;

			if( i == 0 && !IsAxesStyle(AXIS_FIRSTTEXT) )	// 顶部标签
				continue;
			if( i/m_YTicksUnit == m_YTicks/m_YTicksUnit &&  // 底部标签
				!IsAxesStyle(AXIS_LASTTEXT) )
			{
				if(dMulti) // 倍数
				{
					CFont* hOldFont = m_pDC->SelectObject(GetFont(ftMulti));
					COLORREF oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clPageBK.GetInvertColor());
					PrintString(m_GL - m_TickLineLen.lm, yb - (m_TickLineLen).bm*2, 0, 
						ConvertIntToStr((int)dMulti),
						DT_SINGLELINE|DT_END_ELLIPSIS|DT_WORDBREAK,PrintString_Rect);
					m_pDC->SetTextColor(oldColor);
					m_pDC->SelectObject(hOldFont);
				}
				continue;
			}

			if( m_PY == 0 )
				continue;

			value = ((m_GB - yb) * (dMax - dMin)) / m_PY + dMin;

			BOOL bMainChart = IsMainChart();
			if( IsAxesStyle(AXIS_RIGHTDISPPRICE) || !bMainChart )
			{
				if(m_YRefParam.nDiv != 0)
					value /= m_YRefParam.nDiv;
				Format(m_YRefParam.nDecimal, str, value);
				PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);
				//PrintString(m_GR - m_TickLineLen.lm, yb, 0, str);
			}
			else
			{
				double fRate;
				fRate = fabs(value - m_YRefParam.ref) ;	
				char strFormat[10];
				if( value < 0)
					sprintf(strFormat, "-%%.%df", 2);
				else 
					sprintf(strFormat, "%%.%df", 2);
				str.Format(strFormat, fRate);

				COLORREF oldColor; 
				//高炳权添加
				if( value < 0 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
				}
				else if( value > 0 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
				}
				else
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);
				}

				PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);
				m_pDC->SetTextColor(oldColor);
			}
		}
		else
			xb = m_GL - 1;//m_TickLineLen.lm;

		if (i > 0 && i < m_YTicks)
			DrawLine(xb, yb, xe, ye);
	}

	m_pDC->SelectObject(hOldFont);
*/
}

void CGraphics::YAxis()
{
	if( !m_YRefParam.IsValid() )
		return;

	int	  xb, yb, xe, ye;
	int   i;
	CString  str;
	float value;


	double dMin = m_YRefParam.min;
	double dMax = m_YRefParam.max;
	double dMulti = 0;
	CDrawBase* pPKline = GetPKLine();
	if( pPKline != NULL && !pPKline->IsDrawType(DrawType_FenShi) ) // 计算坐标文本是否超过显示边界
	{
		int    nLeftWidth = GetYLeftMaxWidth(m_pDC,TRUE);
		float nCharWidths = LOWORD(nLeftWidth);
		short  nCharCount = HIWORD(nLeftWidth);

		if(nCharCount > 0) 
		{
			nLeftWidth  = (nCharWidths - m_rcDefMargin.left.m_nDef);
			nCharWidths = nCharWidths / nCharCount;
			if( nCharWidths > 0 && nLeftWidth > 0 )
			{
				dMulti = (nLeftWidth / nCharWidths);
				if(dMulti > (int)dMulti)
				{
					dMulti = (int)dMulti + 1;
				}
				else
				{
					dMulti = (int)dMulti;
				}
				dMulti = pow((double)10,(double)dMulti);
				if(dMulti != 0)
				{
					dMax /= dMulti;
					dMin /= dMulti;
				}
			}
		}
	}

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftYAxis));

	//change by yuhj 2008.3.28
	float valuePer;
	if( m_YGridTicks != 0 )
	{
		if( m_nAxesType	== YLOG)
			valuePer = (float)pow((double)10,(log10(dMax)-log10(dMin))/(m_YGridTicks));
		else if(m_nAxesType == YLN)
			valuePer = (float)exp((log(dMax)-log(dMin))/(m_YGridTicks));
	}
	else
		valuePer = 1;
	//end

	xe = m_GL ;
	//ADDBY YUHJ 2008.3.28

	if(m_nAxesType == YGLD || m_nAxesType == YEQU)
	{
		if(m_nAxesType == YEQU)
		{
			m_ayUsrDefAxis.Add(.25);
			m_ayUsrDefAxis.Add(.5);
			m_ayUsrDefAxis.Add(.75);
		}
		else if(m_nAxesType == YGLD)
		{
			m_ayUsrDefAxis.Add(0.191);
			m_ayUsrDefAxis.Add(0.382);
			m_ayUsrDefAxis.Add(0.5);
			m_ayUsrDefAxis.Add(0.618);
			m_ayUsrDefAxis.Add(0.809);
		}

		for(i = 0; i < m_ayUsrDefAxis.GetSize(); i++)
		{
			xb = m_GL - m_TickLineLen.lm;
			yb = ye = m_GT + (int)((m_GB - m_GT) * m_ayUsrDefAxis[i]);

			value = (float)(dMax - m_ayUsrDefAxis[i] * (dMax - dMin));
			BOOL bMainChart = IsMainChart();
			if( IsAxesStyle(AXIS_RIGHTDISPPRICE) || !bMainChart )
			{
				if(m_YRefParam.nDiv != 0)
					value /= m_YRefParam.nDiv;
				Format(m_YRefParam.nDecimal, str, value);
				PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);   
				//PrintString(m_GR - m_TickLineLen.lm, yb, 0, str);
			}
			else
			{
				double fRate;
				//高炳权添加
				fRate = fabs(value - m_YRefParam.ref) ;	
				char strFormat[10];
				//高炳权添加
				if( value < 0)
					sprintf(strFormat, "-%%.%df", 2);
				else 
					sprintf(strFormat, "%%.%df", 2);

				//sprintf(str, strFormat, fRate);
				str.Format(strFormat, fRate);

				COLORREF oldColor; 
				//高炳权添加
				if( value < 0 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
				}
				else if( value > 0 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
				}
				else
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);
				}
				PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);  
				m_pDC->SetTextColor(oldColor);
			}

			DrawLine(xb, yb, xe, ye);
		}

		m_ayUsrDefAxis.RemoveAll();
	}
	else
	{
		//END

		for(i = 0; i <= m_YTicks && m_YTicks != 0; i ++)     
		{
			yb = ye = (int)(m_GT + 1.0 * m_PY * i / m_YTicks );
			if((i % m_YTicksUnit) == 0 && m_YTicksUnit != 0) 
			{
				xb = m_GL - m_TickLineLen.lm;

				if( i == 0 && !IsAxesStyle(AXIS_FIRSTTEXT) )	// 顶部标签
					continue;
				if( i/m_YTicksUnit == m_YTicks/m_YTicksUnit &&  // 底部标签
					!IsAxesStyle(AXIS_LASTTEXT) )
				{
					if(dMulti>=10) // 倍数
					{
						CFont* hOldFont = m_pDC->SelectObject(GetFont(ftMulti));
						COLORREF oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clPageBK.GetInvertColor());
						PrintString(m_GL - m_TickLineLen.lm, yb - (m_TickLineLen).bm*2, 0, 
							ConvertIntToStr((int)dMulti),
							DT_SINGLELINE|DT_END_ELLIPSIS|DT_WORDBREAK,PrintString_Rect);
						m_pDC->SetTextColor(oldColor);
						m_pDC->SelectObject(hOldFont);
					}
					continue;
				}

				//value = (float)(m_Scale.ymax - i * (m_Scale.ymax - m_Scale.ymin) / m_YTicks);
				//value = (float)(dMax - i * (dMax - dMin) / m_YTicks);
				if( m_nAxesType	== YLOG || m_nAxesType == YLN)
				{
					value = dMax / pow(valuePer,(m_YGridTicks - i/m_YTicksUnit - 1));
					POINT pt = GetPoint(0,ValueToVertPos(value));
					yb = ye = pt.y;
				}
				else
				{
					value = (float)(dMax - i * (dMax - dMin) / m_YTicks);
				}

				BOOL bMainChart = IsMainChart();
				//if( IsAxesStyle(AXIS_RIGHTDISPPRICE)  || !bMainChart )
				if( m_nAxesType == XY )
				{
					if(m_YRefParam.nDiv != 0)
						value /= m_YRefParam.nDiv;
					Format(m_YRefParam.nDecimal, str, value);
					PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);     
					//PrintString(m_GR - m_TickLineLen.lm, yb, 0, str);
				}
				else if(m_nAxesType == YPER)
				{
					double fRate;
					//高炳权添加
					fRate = fabs(value - m_YRefParam.ref) ;	

					char strFormat[10];

					if( value < m_YRefParam.ref )
						sprintf(strFormat, "-%%.%df%%%%", 2);
					else 
						sprintf(strFormat, "%%.%df%%%%", 2);

					//sprintf(str, strFormat, fRate);
					str.Format(strFormat, fRate / m_YRefParam.ref * 100);

					COLORREF oldColor; 
					//高炳权添加
					if( value < m_YRefParam.ref )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
					}
					else if( value > m_YRefParam.ref )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
					}
					else
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);
					}

					PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);
					m_pDC->SetTextColor(oldColor);
				}
				else
				{
					double fRate;
					fRate = fabs(value - m_YRefParam.ref) ;	

					char strFormat[10];
					if( value < 0 )
						sprintf(strFormat, "-%%.%df", 2);
					else 
						sprintf(strFormat, "%%.%df", 2);

					//sprintf(str, strFormat, fRate);
					str.Format(strFormat, fRate);

					COLORREF oldColor; 
					if( value < 0 )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
					}
					else if( value > 0 )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
					}
					else
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);
					}
					PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);
					m_pDC->SetTextColor(oldColor);
				}

			}
			else
				xb = m_GL - 1;//m_TickLineLen.lm;

			if (i > 0 && i < m_YTicks)
				DrawLine(xb, yb, xe, ye);
		}

	}
	m_pDC->SelectObject(hOldFont);
}

void CGraphics::RiXiaRightYAxis()
{
	if( !m_YRefParam.IsValid() )
		return;

/*	int	  xb, yb, xe, ye;
	int   i;
	CString str;
	float value;

	double dMin = m_YRefParam.min;
	double dMax = m_YRefParam.max;
	double dMulti = 0;

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftYAxis));

	m_pDC->SetTextColor(GetTickTXTColor().m_lColor);

	SetStringAlign(DT_RIGHT, DT_VCENTER);

	// 自定义坐标
	if( m_ayUsrDefAxis.GetSize() > 0 )
	{
		xe = m_GR;
		xb = m_GR + m_TickLineLen.rm;

		float fMin = m_YRefParam.min;
		short nCount = m_ayUsrDefAxis.GetSize();
		for( i = 0; i < nCount; i++ )     
		{
			value = m_ayUsrDefAxis.GetAt(i);
			if( value == fMin )
				continue;

			yb = ye = m_GB - ValueToVertPos(value);

			// 坐标数值
			DrawLine(xb, yb, xe, ye);
			Format(m_YRefParam.nDecimal, str, value);
			PrintString(m_Rect.right, yb, 0, str);

			// 坐标横线
			DrawLine(m_GL + 1, yb, m_GR - 1, yb,3,*m_pHorLineColor);
		}
	}
	// 自动坐标
	else
	{
		CDrawTechKLine* pPKline = GetPKLine();
		if( pPKline != NULL && !pPKline->IsFenshiDrawType() ) // 计算坐标文本是否超过显示边界
		{
			int    nLeftWidth = GetYLeftMaxWidth(m_pDC,TRUE);
			float nCharWidths = LOWORD(nLeftWidth);
			short  nCharCount = HIWORD(nLeftWidth);
			if(nCharCount > 0)
			{
				nLeftWidth  = (nCharWidths - m_rcMargin.right);
				nCharWidths = nCharWidths / nCharCount;
				if(CDrawTechKLine::bIsVol)//VOL栏坐标最多显示五位数
				{	
					if(nCharCount>5)
						dMulti = nCharCount - 5;
					dMulti = pow((int)10,(int)dMulti);
					if(dMulti != 0)
					{
						dMax /= dMulti;
						dMin /= dMulti;
					}
				}
				else if( nCharWidths > 0 && nLeftWidth > 0 )
				{
					dMulti = (nLeftWidth / nCharWidths);
					if(dMulti > (int)dMulti)
					{
						dMulti = (int)dMulti + 1;
					}
					else
					{
						dMulti = (int)dMulti;
					}
					dMulti = pow((int)10,(int)dMulti);
					if(dMulti != 0)
					{
						dMax /= dMulti;
						dMin /= dMulti;
					}
				}
			}
		}

		xe = m_GR;
		for(i = 0; i <= m_YTicks && m_YTicks != 0; i ++)     
		{
			yb = ye = (int)(m_GT + 1.0 * m_PY * i / m_YTicks );

			if((i % m_YTicksUnit) == 0 && m_YTicksUnit != 0) 
			{
				xb = m_GR + m_TickLineLen.rm;

				if( i == 0 && !IsAxesStyle(AXIS_FIRSTTEXT) )	// 顶部标签
					continue;
				if( i/m_YTicksUnit == m_YTicks/m_YTicksUnit &&  // 底部标签
					!IsAxesStyle(AXIS_LASTTEXT) )
				{
					if(dMulti >= 10) // 倍数
					{
						CFont* hOldFont = m_pDC->SelectObject(GetFont(ftMulti));
						COLORREF oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clPageBK.GetInvertColor());
						PrintString(m_Rect.right, 
							yb - (m_TickLineLen).bm - 1, 0, ConvertIntToStr((int)dMulti),
							DT_SINGLELINE | DT_END_ELLIPSIS | DT_WORDBREAK,PrintString_Rect);
						m_pDC->SetTextColor(oldColor);
						m_pDC->SelectObject(hOldFont);
					}
					continue;
				}

				//value = (float)(m_Scale.ymax - i * (m_Scale.ymax - m_Scale.ymin) / m_YTicks);
				value = (float)(dMax - i * (dMax - dMin) / m_YTicks);

	
				BOOL bMainChart = IsMainChart();
				if( IsAxesStyle(AXIS_RIGHTDISPPRICE) || !bMainChart )
				{
					if(m_YRefParam.nDiv != 0)
						value /= m_YRefParam.nDiv;
					Format(m_YRefParam.nDecimal, str, value);
					CSize size = m_pDC->GetTextExtent(str, lstrlen(str));
					PrintString(m_Rect.right - m_rcMargin.right + size.cx + 6, yb, 0, str);//yulx modify 右边显示价时，靠左显示
					//PrintString(m_GL + m_rcMargin.right, yb, 0, str);
				}
				else
				{
					double fRate;
					if(m_YRefParam.ref != 0)
					{
						fRate = fabs((value - m_YRefParam.ref) / m_YRefParam.ref) * 100;
					}
					else
					{
						fRate = 0;
					}					

					char strFormat[10];
					if ( i/m_YTicksUnit > m_YTicks/m_YTicksUnit/2 )
					{
						sprintf(strFormat, "%%.%df%%%%", 2);
					}
					else
					{
						sprintf(strFormat, "%%.%df%%%%", 2);
					}
					//sprintf(str, strFormat, fRate);
					str.Format(strFormat, fRate);

					COLORREF oldColor; 
					if( value < m_YRefParam.ref )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
					}
					else if( value > m_YRefParam.ref )
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
					}
					else
					{
						oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);
					}

					PrintString(m_Rect.right - 2, yb, 0, str);
					m_pDC->SetTextColor(oldColor);
				}
	
			}
			else
				xb = m_GR + 1;//m_TickLineLen.lm;
			if (i > 0 && i < m_YTicks)
				DrawLine(xb, yb, xe, ye);
		}
	}

	m_pDC->SelectObject(hOldFont);*/
}

// 是否为主图，并且其中是否包含K线（后面再加）
BOOL CGraphics::IsMainChart()
{
/*	CDrawTechKLine* pPKline = GetPKLine();
	CDragObjManager* pDrag = NULL;
	BOOL bMainChart = FALSE;
	if( m_ayDrag && m_ayDrag->GetSize() > 0 )
		pDrag = m_ayDrag->GetAt(0);

	if( pDrag != NULL && pPKline!= NULL && pDrag->Compare(pPKline->GetID()))
	{
		bMainChart = TRUE;
	}

	return bMainChart;*/
	return FALSE;
}

void CGraphics::FenShiYAxis()
{
	if( !m_YRefParam.IsValid() )
		return;

	int	  xb, yb, xe, ye;
	int   i;
	CString  str;
	float value;

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftFenShiYAxis));

	xe = m_GL;
	COLORREF oldColor = -1;
	for(i = 0; i <= m_YTicks && m_YTicks != 0; i ++)     
	{
		yb = ye = (int)(m_GT + 1.0 * m_PY * i / m_YTicks );
		if((i % m_YTicksUnit) == 0 && m_YTicksUnit != 0) 
		{			
			xb = m_GL - m_TickLineLen.lm;

			if( i == 0 && 
				!IsAxesStyle(AXIS_FIRSTTEXT) )
				continue;
			if( i/m_YTicksUnit == m_YTicks/m_YTicksUnit && 
				!IsAxesStyle(AXIS_LASTTEXT) )
				continue;

			value = (float)(m_YRefParam.max - i * (m_YRefParam.max - m_YRefParam.min) / m_YTicks);
			if(m_YRefParam.nDiv != 0)
				value /= m_YRefParam.nDiv;
			Format(m_YRefParam.nDecimal, str, value);

			oldColor = -1;
			if( i/m_YTicksUnit > m_YTicks/m_YTicksUnit/2 )
			{
				oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);				
			}
			else if( i/m_YTicksUnit < m_YTicks/m_YTicksUnit/2 )
			{
				oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);				
			}
			else
			{
				oldColor = m_pDC->SetTextColor( g_hsGraphicsData.clEqual.m_lColor);

			}

			PrintString(m_GL - m_TickLineLen.lm, yb, 0, str);

			if(oldColor != -1)
			{
				m_pDC->SetTextColor(oldColor);
			}
		}
		else
		{
			xb = m_GL - 1;//m_TickLineLen.lm;
		}
		if (i > 0 && i < m_YTicks)
		{
			DrawLine(xb, yb, xe, ye);
		}
	}

	m_pDC->SelectObject(hOldFont);
}

void CGraphics::Ticks()
{  
	if( IsAxesStyle(AXIS_BOTTOM) )
	{
		//----2010.1.12 litao增加对星空图的判断------------------
		if (IsAxesStyle(AXIS_STARHORI))
			SetStringAlign(DT_RIGHT, DT_TOP);
		else
			SetStringAlign(DT_CENTER, DT_TOP);
		//-------------------------------------------------------
		XAxis();		
	}
	if( IsAxesStyle(AXIS_LEFT) )
	{
		SetStringAlign(DT_RIGHT, DT_VCENTER);
		if( IsAxesStyle(AXIS_FENSHIPRICE) )
		{
			FenShiYAxis();
		}
		else
		{
			YAxis();
		}
	}
}

void CGraphics::RightYTick()
{
	if (m_bEnableLegend)
		return;

	if ( !IsAxesStyle(AXIS_RIGHT) )
		return;

	if( IsAxesStyle(AXIS_FENSHIPRICE) )
	{
		FenShiRightYTick();
		return;
	}

	CDrawBase* pPKline = GetPKLine();
	if( pPKline != NULL && !pPKline->IsDrawType(DrawType_FenShi ))
	{
		RiXiaRightYAxis();
		return;
	}

	return;
}

void CGraphics::FenShiRightYTick()
{
	if( !m_YRefParam.IsValid() )
		return;

	int	   xb, yb, xe, ye;
	int    i;
	CString  str;
	float  value;
	double fRate;

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftFenShiRightYTick));

	xb = m_GR;
	SetStringAlign(DT_LEFT, DT_VCENTER);
	COLORREF oldColor = -1;
	if (m_nAxesType == XY || m_nAxesType == XLOG)
	{
		for(i = 0; i <= m_YTicks && m_YTicks != 0; i ++)
		{
			yb = ye = (int)(m_GT + 1.0 * m_PY * i / m_YTicks );			
			if((i % m_YTicksUnit) == 0 && m_YTicksUnit != 0) 
			{
				xe = m_GR + m_TickLineLen.rm;

				if(i == 0 && !IsAxesStyle(AXIS_FIRSTTEXT) )
					continue;
				if(i/m_YTicksUnit == m_YTicks/m_YTicksUnit && !IsAxesStyle(AXIS_LASTTEXT) )
					continue;

				oldColor = -1;
				if( i/m_YTicksUnit > m_YTicks/m_YTicksUnit/2 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);				
				}
				else if( i/m_YTicksUnit < m_YTicks/m_YTicksUnit/2 )
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);				
				}
				else
				{
					oldColor = m_pDC->SetTextColor(g_hsGraphicsData.clEqual.m_lColor);					
				}

				value = (float)(m_YRefParam.max - i * (m_YRefParam.max - m_YRefParam.min) / m_YTicks);

				if( IsAxesStyle(AXIS_RIGHTDISPPRICE) )
				{
					if(m_YRefParam.nDiv != 0)
						value /= m_YRefParam.nDiv;
					Format(m_YRefParam.nDecimal, str, value);
				}
				else
				{
					if(m_YRefParam.ref != 0)
					{
						fRate = fabs((value - m_YRefParam.ref) / m_YRefParam.ref) * 100;
					}
					else
					{
						fRate = 0;
					}					
					if(m_YRefParam.nDiv != 0)
						value /= m_YRefParam.nDiv;

					char strFormat[10];

					//qinhn 20090811 modify  分时右边显示小数位数根据显示百分数据的最大值改变，大于0.5时显示两位小数，小于等于0.5时显示一位小数
//					CString strFileName = CHSFilePath::GetPath(CHSFilePath::Setting) + "HappyStar.ini"; 
					bool read = false; 
					static int nDigit = 0;
					int DefaultDecimalDigits = 2;
//					int RealDecimalDigits;
		//			read = CWriteReadStandardConfig::ReadConfigInt("纵坐标显示小数数位", "分时图右边小数位数目", RealDecimalDigits, strFileName);
		//			if(read)
		//				DefaultDecimalDigits = RealDecimalDigits;

					if (0==i)
					{
						if (fRate>0.5)
							nDigit = 1;
						else
							if (fRate <= 0.00000001)
								nDigit  = 1;
							else
								nDigit = 2;
					}
					DefaultDecimalDigits = nDigit;
					//qinhn 20090811 modify End
					if ( i/m_YTicksUnit > m_YTicks/m_YTicksUnit/2 )
					{
						sprintf(strFormat, "%%.%df%%%%", DefaultDecimalDigits);   
					}
					else
					{
						sprintf(strFormat, "%%.%df%%%%",DefaultDecimalDigits);
					}
					str.Format( strFormat, fRate );
					//sprintf(str, strFormat, fRate);
				}

				PrintString(m_GR + m_TickLineLen.rm, yb, 0, str);

				if(oldColor != -1)
				{
					m_pDC->SetTextColor(oldColor);
				}
			}
			else
			{
				xe = m_GR + 2;//m_TickLineLen.rm;
			}
			if (i > 0 && i < m_YTicks)
			{
				DrawLine(xb, yb, xe, ye);
			}
		}
	}
	else
	{

	}
	m_pDC->SelectObject(hOldFont);
}

void CGraphics::DrawBorder()
{
	CPen hPen;
	hPen.CreatePen(GetBorderColor().m_nPenStyle,GetBorderColor().m_nWidth,GetBorderColor().m_lColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	CDrawBase* pPKline = GetPKLine();
	if( pPKline != NULL )
	{
		int nRight = m_Rect.right + 2;

		if( pPKline->IsDrawType(DrawType_FenShi ))
		{
			if(IsAxesStyle(AXIS_FENSHIPRICE))
			{
				DrawRectangle(m_GL, m_GT, m_GR, m_GB);
			}
			else
			{
				DrawLine(m_GL, m_Rect.top,m_GL,m_GB);
				DrawLine(m_GL, m_GT,m_GR,m_GT);
				DrawLine(m_GR, m_Rect.top,m_GR,m_GB);
				DrawLine(m_Rect.left, m_GB,nRight,m_GB);   
			}
		}
		else
		{
			if ( IsAxesStyle(AXIS_RIGHTMARGIN) )
			{
				DrawLine(m_GL, m_GT, m_GR, m_GT,3,&g_hsGraphicsData.clDayGridVerLine);
				DrawLine(m_GR, m_Rect.top,m_GR,m_GB);
				DrawLine(m_GL, m_GB,nRight,m_GB);
			}
			else
			{				                                     
				if(g_hsGraphicsData.IsStyle(GL_KLine_TopLine))       //2008-11-7覃宏念把日线图中左边垂直方向三个方框的顶部横线，也
					DrawKLineTopLine(m_GL, m_GT, m_GR, m_GT,//即MA，VOL，MACD字列下面的横线独立出来单独画线，并通过GL_KLine_TopLine判断是否画线
					g_hsGraphicsData.clDayTopLine.m_nWidth, 
					g_hsGraphicsData.clDayTopLine.m_nPenStyle);                

				DrawLine(m_GL, m_Rect.top,m_GL ,m_GB);  
				DrawLine(m_Rect.left, m_GB,nRight,m_GB);    
			}
		}
	}
	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();
}

void CGraphics::DrawMarginRect(int nMask)
{
	CDrawBase* pPKline = GetPKLine();
	if( pPKline != NULL )
	{
		if(pPKline->IsDrawType(DrawType_FenShi ))
		{
			if( IsAxesStyle(AXIS_FENSHIPRICE) )
			{

			}
			else
			{

			}
		}
		else
		{
			CRect rect;
			if( nMask == -1 )
			{
				if ( IsAxesStyle(AXIS_RIGHTMARGIN) )
				{
					rect = GetMarginRect(AXIS_RIGHTMARGIN);
				}
				else
				{
					rect = GetMarginRect(AXIS_LEFTMARGIN);
				}
			}
			else
			{
				rect = GetMarginRect(nMask);
			}
			m_pDC->FillSolidRect(rect, g_hsGraphicsData.crHightBkColor.m_lColor);
		}
	}
}

void CGraphics::Axes()
{
/*	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_RIGHT_CHUQUAN) )
	{
	}
*/
	DrawBorder();

	CPen hPen1;
	hPen1.CreatePen(GetTickLineColor().m_nPenStyle, GetTickLineColor().m_nWidth, GetTickLineColor().m_lColor);
	CPen* hOldPen1 = m_pDC->SelectObject(&hPen1);
	m_pDC->SetTextColor(GetTickTXTColor().m_lColor); 	

	Ticks();
	RightYTick();

	m_pDC->SelectObject(hOldPen1);
	hPen1.DeleteObject();
}

void CGraphics::DrawMarker( CPoint ptStart, CPoint ptEnd, ColorProperty& crColor )
{
//	g_hsGraphicsData.Draw3DStyle(m_pDC,CRect(ptStart,ptEnd),1,crColor.GetColor());
}

void CGraphics::DrawMarker(int x, int y, int mode, int n /*= 6*/,CString strText /*= ""*/)
{
/*	if (m_bPrinting)	
		n *= m_nPrintScale;

	switch(mode)
	{
	case CROSS:
		DrawLine(x - n, y, x + n, y );
		DrawLine(x, y - n, x, y + n );
		break;

	case STAR:
		DrawLine(x - n, y, x + n, y );
		DrawLine(x - n / 2, (int)(y + n * sqrt(3.) / 2), x + n / 2, 
			(int)(y - n * sqrt(3.) / 2));
		DrawLine(x - n / 2, (int)(y - n * sqrt(3.) / 2), x + n / 2, 
			(int)(y + n * sqrt(3.) / 2));
		break;

	case FCIRCLE:
		DrawFilledCircle(x, y, n);
		break;

	case DOWNFTRIANGLE:
		{
			POINT  p[3];
			p[0].x = x;
			p[0].y = (int)(y + n * sqrt(3.) / 3);
			p[1].x = x - n;
			p[1].y = (int)(y - 2 * n * sqrt(3.) / 3);
			p[2].x = x + n;
			p[2].y = (int)(y - 2 * n * sqrt(3.) / 3);			
			m_pDC->Polygon( p, 3);
		}
		break;
	case FTRIANGLE:
		{
			POINT  p[3];
			p[0].x = x - n;
			p[0].y = (int)(y + n * sqrt(3.) / 3);
			p[1].x = x + n;
			p[1].y = (int)(y + n * sqrt(3.) / 3);
			p[2].x = x;
			p[2].y = (int)(y - 2 * n * sqrt(3.) / 3);
			m_pDC->Polygon( p, 3);			
		}
		break;
	case XCROSS:
		DrawLine((int)(x + n * sqrt(2.) / 2),
			(int)(y - n * sqrt(2.) / 2), 
			(int)(x - n * sqrt(2.) / 2),
			(int)(y + n * sqrt(2.) / 2));
		DrawLine((int)(x - n * sqrt(2.) / 2),
			(int)(y - n * sqrt(2.) / 2), 
			(int)(x + n * sqrt(2.) / 2),
			(int)(y + n * sqrt(2.) / 2));
		break;

	case CIRCLE:
		DrawCircle(x, y, n);
		break;

	case TRIANGLE:
		DrawLine(x - n, (int)(y + n * sqrt(3.) / 3),
			x + n, (int)(y + n * sqrt(3.) / 3));
		DrawLine(x + n, (int)(y + n * sqrt(3.) / 3),
			x, (int)(y - 2 * n * sqrt(3.) / 3));
		DrawLine(x, (int)(y - 2 * n * sqrt(3.) / 3),
			x - n, (int)(y + n * sqrt(3.) / 3));  
		break;

	case FSQUARE:
		m_pDC->Rectangle(x - n, y - n, x + n, y + n);
		break;

	case SQUARE:      
		DrawRectangle(x - n, y - n, x + n, y + n );
		break;

	case FDIAMOND:
		{
			POINT  p[4];
			p[0].x = x - n;
			p[0].y = y;
			p[1].x = x;
			p[1].y = y + n;
			p[2].x = x + n;
			p[2].y = y;
			p[3].x = x;
			p[3].y = y - n;
			m_pDC->Polygon( p, 4);
			break;
		}

	case DIAMOND:
		m_pDC->MoveTo( x - n, y);
		m_pDC->LineTo(x, y + n);
		m_pDC->LineTo(x + n, y);
		m_pDC->LineTo(x, y - n);
		m_pDC->LineTo( x - n, y);
		break;
	case SELECTED: // 选中
		m_pDC->PatBlt(x - n/2,y - n/2, n, n, DSTINVERT);//PATINVERT
		break;
	case INFODATA:
		{
			CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
			if( pImage != NULL && pImage->m_hImageList != NULL )
			{
			//这里画分时图 信息地雷 的图标，目前信息地雷为一个类似菱形的图案,其是调用一长条图片里面的一个小图.
			//参看 CPublicData::InitExpress 函数里面m_expressImages的创建过程  现信息地雷的图案可配置， 
			//不固定用CExpression::Yls_Exp_Info_Mark所指定位置的图案。 qinhn  20090604 Modify
				if(bEverRead)
				{
					CString sFileName = CHSFilePath::GetPath(CHSFilePath::Setting) + "HappyStar.ini";

					
				//	bool readR = CWriteReadStandardConfig::ReadConfigInt("信息地雷图案", "图案位置编号", nMarkIndex, sFileName);
					if(!CfgValues::GetDetailCfgInt(m_CfgTopMapOb,"信息地雷图案", "图案位置编号" , nMarkIndex))
						nMarkIndex =  CExpression::Yls_Exp_Info_Mark;
		

					if(nMarkIndex <=0 || nMarkIndex >100)
						nMarkIndex = CExpression::Yls_Exp_Info_Mark;
					bEverRead = false;
				}
				pImage->Draw(m_pDC,nMarkIndex,
					CPoint(x - 12,m_GT),ILD_TRANSPARENT);
			}
			//m_pDC->PatBlt(x - n/2,y - n/2, n, n, DSTINVERT);
		}
		break;
	case DRAWSTRING:
		{
			//PrintString(x - n/2,y - n/2, 0, strText);
			//m_pDC->TextOut(strText
			m_pDC->TextOut(x - n/2,y - n/2,strText);
		}
		break;
	}*/
}

void CGraphics::DrawLine(int x1, int y1, int x2, int y2,
						 char bUser /*= FALSE*/,ColorProperty* pColor /*= NULL*/)
{
	if(bUser)
	{
		if( pColor )
		{
			if(y1 == y2) // 横向
			{
				for(; x1 <= x2; x1 += bUser)
				{
					m_pDC->SetPixel(x1, y1,pColor->m_lColor);
				}
			}
			else
			{
				for(; y1 <= y2; y1 += bUser)
				{
					m_pDC->SetPixel(x1, y1,pColor->m_lColor);
				}
			}
		}
	}
	else
	{
		if( pColor != NULL )
		{
			CPen hPen;
			hPen.CreatePen(pColor->m_nPenStyle,pColor->m_nWidth,pColor->m_lColor);
			CPen* hOldPen = m_pDC->SelectObject(&hPen);

			m_pDC->MoveTo(x1, y1); 
			m_pDC->LineTo(x2, y2);

			m_pDC->SelectObject(hOldPen);
			hPen.DeleteObject();
		}
		else
		{
 			m_pDC->MoveTo(x1, y1); 
  		m_pDC->LineTo(x2, y2); 
		}
	}

}

void CGraphics::DrawShadow(int n)
{
	CPen hCurPen; 
	hCurPen.CreatePen(PS_SOLID, 0, RGB(127,127,127));
	CPen* hOldPen = m_pDC->SelectObject(&hCurPen);
	CBrush hBrush;
	hBrush.CreateSolidBrush(RGB(127,127,127));
	CBrush* hBrold = m_pDC->SelectObject(&hBrush);
	int w  = (m_Rect.right - m_GR) / 20 + 1;
	int xb = m_GR + 2 * n;
	m_pDC->Rectangle(m_GR + (int)(2.2 * w), m_GT - m_LogFont.lfHeight, 
		m_Rect.right - (int)(0.8 * w), m_GT - 2 * (n + 1) * m_LogFont.lfHeight);
	m_pDC->SelectObject(hBrold);
	hBrush.DeleteObject();
	m_pDC->SelectObject(hOldPen);
	hCurPen.DeleteObject();

	CBrush hBrush1;
	hBrush1.CreateSolidBrush(RGB(255, 255, 255));
	CBrush* hBrold1 = m_pDC->SelectObject(&hBrush1);
	m_pDC->Rectangle(m_GR + w, m_GT, m_Rect.right - 2 * w, 
		m_GT - 2 * (n + 1) * m_LogFont.lfHeight + m_LogFont.lfHeight);
	m_pDC->SelectObject(hBrold1);
	hBrush1.DeleteObject();
}

void CGraphics::SetXNumOfTicks(int x /*= 50*/)
{ 
	m_XTicks = x; 
	m_XTicks *= m_XTicksUnit;
	SetXNumOfGridTicks(x);
}

void CGraphics::SetYNumOfTicks(int y /*= 50*/)				
{ 
	m_YTicks = y; 
	m_YTicks *= m_YTicksUnit;
	SetYNumOfGridTicks(y);
}

int CGraphics::GetYNumOfTicks()
{
  return m_YTicks / m_YTicksUnit;
}

void CGraphics::SetXNumOfGridTicks(int x /*= 10*/)			
{ 
	m_XGridTicks = x; 
}

void CGraphics::SetYNumOfGridTicks(int y /*= 10*/)			
{ 
	m_YGridTicks = y; 
}

int CGraphics::DrawVarTitle(CArray< CToptextType<double>*,CToptextType<double>* >& Value, int nStartPositon/* = 0*/)
{
	try
	{
		CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawVarTitle));

		CToptextType<double>* pValue;

		int nCount = Value.GetSize();

		CSize  size;			
		CString  strValue;

		CRect rect = GetExplainTextRect();

		POINT pt;
		pt.x = m_GL + 1;
		pt.y = (m_Rect.top + m_GT ) / 2 + 1;

		SetStringAlign(DT_LEFT, DT_VCENTER);
		for(int i = 0; i < nCount; i++ )
		{				
			pValue = Value.GetAt(i);

			m_pDC->SetTextColor(pValue->m_cl);
			size = m_pDC->GetTextExtent(pValue->m_strName, lstrlen(pValue->m_strName));
			size.cx += 4;
			if( pt.x + size.cx + 16 > rect.right )
			{
				break;
			}

			if( pValue->IsAddHig() )
			{
				CRect rectText(m_Rect.left+pt.x, m_Rect.top, m_Rect.left+size.cx+pt.x, m_Rect.top+size.cy); 
				m_pDC->FillSolidRect(rectText, g_hsGraphicsData.crHightBkColor.m_lColor/*::GetSysColor(COLOR_HIGHLIGHT)*/);
			}

			PrintString(pt.x,pt.y, 0, pValue->m_strName);
			pt.x += size.cx;

			if(pValue->m_Value != 0)
			{
				Format(pValue->m_nDecimal, strValue, pValue->m_Value);				
				if(pValue->IsArrow())
				{
					CBrush hBrush; 
					hBrush.CreateSolidBrush(pValue->m_cl);
					CBrush* hBrold = m_pDC->SelectObject(&hBrush);

					CPen hPen;
					hPen.CreatePen(PS_SOLID, 1,pValue->m_cl);
					CPen* hOldPen = m_pDC->SelectObject(&hPen);

					pt.x += 4;
					DrawMarker(pt.x,pt.y, (strValue[0] == '-')?DOWNFTRIANGLE:FTRIANGLE); pt.x += 8;

					m_pDC->SelectObject(hOldPen);
					hPen.DeleteObject();
					m_pDC->SelectObject(hBrold);
					hBrush.DeleteObject();
				}

				size = m_pDC->GetTextExtent(strValue, lstrlen(strValue));
				size.cx += 4;
				if( pt.x + size.cx + 16 > rect.right )
				{
					break;
				}

				PrintString(pt.x,pt.y, 0, strValue);
				pt.x += size.cx;				
			}
		}

		m_pDC->SelectObject(hOldFont);

		return pt.x;
	}
	catch(...){}

	return 0;
}

int CGraphics::DrawVarTitle(CToptextType<double>* Value,...)
{
	try
	{
		CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawVarTitle));

		va_list argList;
		va_start(argList, Value);
		CToptextType<double>* pValue = Value;

		CSize  size;			
		CString  strValue;

		CRect rect = GetExplainTextRect();

		POINT pt;
		pt.x = m_GL + 1;
		pt.y = (m_Rect.top + m_GT ) / 2 + 1;

		SetStringAlign(DT_LEFT, DT_VCENTER);
		while( pValue != NULL )
		{				
			m_pDC->SetTextColor(pValue->m_cl);
			size = m_pDC->GetTextExtent(pValue->m_strName, lstrlen(pValue->m_strName));
			size.cx += 4;
			if( pt.x + size.cx + 16 > rect.right )
			{
				break;
			}

			if( pValue->IsAddHig() )
			{
				CRect rectText(m_Rect.left+pt.x, rect.top, m_Rect.left+size.cx+pt.x, rect.top+rect.Height()); 
				m_pDC->FillSolidRect(rectText,g_hsGraphicsData.crHightBkColor.m_lColor);
			}

			PrintString(pt.x,pt.y, 0, pValue->m_strName);
			pt.x += size.cx;

			if(pValue->m_Value != 0)
			{
				Format(pValue->m_nDecimal, strValue, pValue->m_Value);				
				if(pValue->IsArrow())
				{
					CBrush hBrush; 
					hBrush.CreateSolidBrush(pValue->m_cl);
					CBrush* hBrold = m_pDC->SelectObject(&hBrush);

					CPen hPen;
					hPen.CreatePen(PS_SOLID, 1,pValue->m_cl);
					CPen* hOldPen = m_pDC->SelectObject(&hPen);

					pt.x += 4;
					DrawMarker(pt.x,pt.y, (pValue->m_cl == g_hsGraphicsData.clDown.m_lColor )?DOWNFTRIANGLE:FTRIANGLE); pt.x += 8;

					m_pDC->SelectObject(hOldPen);
					hPen.DeleteObject();
					m_pDC->SelectObject(hBrold);
					hBrush.DeleteObject();
				}

				size = m_pDC->GetTextExtent(strValue, lstrlen(strValue));
				size.cx += 4;
				//if( pt.x + size.cx + 10 > rect.right )
				//{
				//	int lLen = lstrlen(strValue);
				//	break;
				//}

				PrintString(pt.x,pt.y, 0, strValue);
				pt.x += size.cx;				
			}
			pValue = (CToptextType<double>*)va_arg( argList, CToptextType<double>* );
		}
		va_end(argList);

		m_pDC->SelectObject(hOldFont);
	}
	catch(...){}

	return 0;
}

int CGraphics::DrawVarTitle(class CDrawBase* pCurObj,CNameProperty& strExpName,
							CArray<CLineProperty*,CLineProperty*>* ayData,
							int nCurDataPos,int nDiv,short nDecimal,BYTE bArrow,
							DWORD dStyle /*= 0*/,DWORD dObjectStyle /*= 0*/)
{
/*	try
	{				
		if(ayData == NULL && strExpName.IsEmpty())
			return 0;

		POINT pt;   pt.x = 0;
		CSize size; size.cx = 0;
		CRect rect;
		BOOL bHighLight = FALSE;//!m_rcExplain.IsRectEmpty();
		CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawVarTitle));

		CString strValue;
		CString strText;
		BOOL	bAdd = FALSE;

		pt.x = m_GL + 1 + m_MenuSize.cx;
		pt.y = (m_Rect.top + m_GT ) / 2;// + 1;
		pt.y = (m_Rect.top + m_GT ) / 2;

		if(bHighLight)
		{
			m_pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}

		CDrawBase* pObj = GetPrevObjHaveShow(pCurObj,FALSE);
		if( pObj != NULL )//&& 
			//pObj->IsKindOf(RUNTIME_CLASS(CDrawTechKLine)) )
		{
			bAdd = TRUE;
			pt.x -= m_GL;
			//pt.x -= m_MenuSize.cx;
			pt.x += ((CDrawTechKLine*)pObj)->GetExplainTextWidth();
		}

		rect = GetExplainTextRect();
		rect.left = pt.x-1;
		pt.x++;

		SetStringAlign(DT_LEFT, DT_VCENTER);

		// 表达式名称及参数  
		if(!bHighLight)
		{
			m_pDC->SetTextColor(strExpName.m_lColor.m_lColor);
		}
		
		//名称颜色用‘名称文本’
		if(strExpName.m_strName=="KLINE" && !((CDrawTechKLine*)pCurObj)->GetTechAnaObj()->GetDefColor())
			m_pDC->SetTextColor(g_hsGraphicsData.clFixedNameTXT);
		else if (((CDrawTechKLine*)pCurObj)->GetTechAnaObj()->GetDefColor())//叠加K线
		{
			m_pDC->SetTextColor(((CDrawTechKLine*)pCurObj)->GetTechAnaObj()->GetDefColor()->GetColor());
		}
		
		if (strExpName.IsStyle(hx_NameProperty_DrawDescribe))
		{
			m_pDC->SetTextColor(g_hsGraphicsData.clJsKLineTitle.m_lColor);
		}

		CTechAnalysisObj* pTech = ((CDrawTechKLine*)pCurObj)->GetTechAnaObj();

		if( !strExpName.IsStyle(hx_NameProperty_NotDrawName) )
		{
			if( bAdd && g_hsGraphicsData.m_expressImages )
			{
				g_hsGraphicsData.m_expressImages->Draw(m_pDC,((pCurObj && pCurObj->IsStyle(CUT_ADDPKLINE))?(CExpression::Yls_Exp_Tech-1):
			(dObjectStyle & CExpression::Condition)?CExpression::Yls_Exp_SelectStock:
			(dObjectStyle & CExpression::Exchange)?(CExpression::Yls_Exp_SelectStock + 2):
			(dObjectStyle & CExpression::MoreKLine)?(CExpression::Yls_Exp_SelectStock - 1):
			CExpression::Yls_Exp_CheckTech),
				CPoint(pt.x,m_Rect.top),
				( (pTech && pTech->IsStyle(KLINE_SIGN_MOREKLINE | KLINE_SIGN_CONDITION | KLINE_SIGN_EXCHANGE))?ILD_FOCUS:0 ) | ILD_TRANSPARENT); // ILD_TRANSPARENT

			pt.x += 16;
			bAdd = FALSE;
			}

			strText.Format("%s",
				strExpName.IsStyle(hx_NameProperty_DrawDescribe)?strExpName.m_strDescribe:
			strExpName.m_strName);
			size = m_pDC->GetTextExtent(strText, lstrlen(strText));				
			if( pt.x + size.cx + 10> rect.right )
			{
				long lTemp = 0;
			}
			else
			{
				PrintString(pt.x,pt.y, 0, strText);
				pt.x += size.cx;
			}
		}

		if( !strExpName.IsStyle(hx_NameProperty_NotDrawExp) )
		{
			if( bAdd && g_hsGraphicsData.m_expressImages )
			{
				g_hsGraphicsData.m_expressImages->Draw(m_pDC,((pCurObj && pCurObj->IsStyle(CUT_ADDPKLINE))?(CExpression::Yls_Exp_Tech-1):
			(dObjectStyle & CExpression::Condition)?CExpression::Yls_Exp_SelectStock:
			(dObjectStyle & CExpression::Exchange)?(CExpression::Yls_Exp_SelectStock + 2):
			(dObjectStyle & CExpression::MoreKLine)?(CExpression::Yls_Exp_SelectStock - 1):
			CExpression::Yls_Exp_CheckTech),
				CPoint(pt.x,m_Rect.top),ILD_TRANSPARENT);
			pt.x += 16;
			}

			strText.Format("%s",strExpName.m_strExp);
			size = m_pDC->GetTextExtent(strText, lstrlen(strText));				
			if( pt.x + size.cx + 10 > rect.right )
			{		
				long lTemp = 0;
			}
			else
			{
				//20080602 chenhf 多股票K线图中当前选中的股票名称高亮显示，单股票时不高亮显示
				if ((pCurObj->GetView()->IsMultiView()) && (pCurObj->GetView()->IsActive()) && (strExpName.IsStyle(hx_NameProperty_NotDrawName)))
				{
					CRect rectText(m_Rect.left+pt.x, rect.top, m_Rect.left+size.cx+pt.x, rect.top+rect.Height()); 
					m_pDC->FillSolidRect(rectText,g_hsGraphicsData.crHightBkColor.m_lColor);
				}
				//2008060 chenhf end
				PrintString(pt.x,pt.y, 0, strText);
				size.cx += 4;

				pt.x += size.cx;
			}
		}


		int nCount = 0;
		if(ayData != NULL)
			nCount = ayData->GetSize();
		
		CExpression* pExpression = g_hsGraphicsData.GetExpress(strExpName.m_strName,strExpName.m_dStyle & 0x00FF);
		CMapVariabile* pParam = NULL;
		if( pExpression )
			pParam = pExpression->GetParamVar();

		// 参数名称及数值
		CLineProperty* pValue;			
		for(int nNext = 0; nNext < nCount; nNext++)
		{			
			pValue = ayData->GetAt(nNext);
			if( pValue == NULL					 || 
				!pValue->IsStyle(HX_EXPRESSDRAW) || 
				pValue->IsStyle(NOTDRAWTEXT) )
				continue;

			if( !bHighLight )
			{
				m_pDC->SetTextColor(pValue->m_lColor.m_lColor);					  // 名称
			}

			//
			if( !pValue->m_strName.IsEmpty() )
			{
				CString str = pValue->m_strName.Left(pValue->m_strName.GetLength() - 3);//yulx add 去掉最后的000
				strText.Format("%s:",str);
				if( pParam )
				{
					CString strTmp;
					strTmp.Format("%s",str);

					POSITION pos = pParam->GetStartPosition();
					CString sName;
					CValue *pVal;
					while (pos != NULL)
					{			
						pParam->GetNextAssoc (pos, sName, pVal);
						if(pVal != NULL && pVal->IsStyle(HX_EXPRESSNUMBER) )
						{
							int nPos = strTmp.Find(sName);
							if( nPos != -1 && (nPos + sName.GetLength() == strTmp.GetLength()) )
							{
								//sName  = strTmp.Mid(nPos+sName.GetLength());
								strTmp = strTmp.Left(nPos);
								CString str;
								str.Format("%f",((CNumericValue *)pVal)->GetValue());
								strTmp += hxSubZero(str);
								//strTmp += sName;
								strTmp += ":";
								strText = strTmp;
								break;
							}
						}
					}
				}

				size = m_pDC->GetTextExtent(strText, lstrlen(strText));				
				if( pt.x + size.cx + 10 > rect.right )
				{
					break;
				}
				PrintString(pt.x,pt.y, 0, strText);
				pt.x += size.cx;
			}
			else
			{
				long lTemp = 0;
			}

			if( pValue->m_pData != NULL && nCurDataPos >= 0 && 
				//pValue->m_pData[nCurDataPos] != 0 && 
				pValue->m_pData[nCurDataPos].m_dData != (long)hx_DefMinValues) // 数值
			{
				Format(nDecimal, strValue, (double)pValue->m_pData[nCurDataPos]);

				// 2003-02-17 llx 
				if( bArrow&01 )
				{
					CBrush hBrush; 
					hBrush.CreateSolidBrush(pValue->m_lColor.m_lColor);
					CBrush* hBrold = m_pDC->SelectObject(&hBrush);

					CPen hPen;
					hPen.CreatePen(pValue->m_lColor.m_nPenStyle, 
						pValue->m_lColor.m_nWidth,pValue->m_lColor.m_lColor);
					CPen* hOldPen = m_pDC->SelectObject(&hPen);

					pt.x += 4;
					if( pt.x > rect.right )
					{
						pt.x -= 4;
						break;
					}
					DrawMarker(pt.x,pt.y, (pValue->m_lColor.m_lColor == g_hsGraphicsData.clDown.m_lColor)?DOWNFTRIANGLE:FTRIANGLE); pt.x += 8;

					m_pDC->SelectObject(hOldPen);
					hPen.DeleteObject();
					m_pDC->SelectObject(hBrold);
					hBrush.DeleteObject();
				}

				strText = strValue;			
				size = m_pDC->GetTextExtent(strText, lstrlen(strText));	
				if( pt.x + size.cx + 16 > rect.right )
				{
					break;
				}
				PrintString(pt.x,pt.y, 0, strText);
				pt.x += size.cx;
			}
			else
			{
				long lTemp = 0;
			}

			if( nNext != nCount - 1 )
			{
				pt.x += 8;
			}
			else
			{
				//pt.x -= size.cx;
			}
		}
		m_pDC->SelectObject(hOldFont);

		if( dStyle & PKLINE_TYPE_ACTIVE_CHART &&  // 激活状态 
			g_hsGraphicsData.m_expressImages )
		{
			CRect rc;
			if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_RIGHT_AXES) )
			{
				rc.SetRect(m_GR+1,m_Rect.top,m_Rect.right + 2,m_GB);
			}
			else
			{
				rc.SetRect(m_GL-1,m_Rect.top,m_Rect.left+1,m_GB);
			}

			m_pDC->Draw3dRect(rc,g_hsGraphicsData.clGridLine.m_lColor,g_hsGraphicsData.clGridLine.m_lColor);

		}

		if( ayData != NULL && 
			pCurObj->GetID() != AnalysisPKLine )
		{
			rect.right = pt.x;
			rect.left  -= m_MenuSize.cx;
			SetExplainRect(rect,pCurObj,TRUE);
		}

		return pt.x + m_MenuSize.cx;
	}
	catch(...){}
*/
	return 0;
}

void CGraphics::SetRelating(CArray<class CHSDrawObj*, class CHSDrawObj*>* payDrawList,
							CArray<class CDragObjManager*,class CDragObjManager*>* ayDrag)
{
	m_ayDrawList = payDrawList;
	m_ayDrag	 = ayDrag;
}

void CGraphics::RemoveSelectedStyle()
{ 
}

CDrawBase* CGraphics::GetPrevObjHaveShow(CDrawBase* pCurObj,BOOL bNext)
{
/*	if( m_ayDrag == NULL || 
		m_ayDrag->GetSize() <= 0 )
		return NULL;

	CDragObjManager* pDrag;
	short			 nRetPos;
	for(int i = 0; i < m_ayDrag->GetSize(); i++)
	{
		pDrag = m_ayDrag->GetAt(i);
		if( pDrag != NULL && pDrag->Compare(pCurObj->GetID(),nRetPos) )
		{
			CDrawBase* pRet = NULL;
			if( bNext )
			{
				for( nRetPos++; nRetPos < m_ayDrag->GetSize(); nRetPos++ )
				{
					pRet = pDrag->GetObjByPos(nRetPos);
					if( pRet->IsShow() )
					{
						return pRet;
					}
				}
			}
			else
			{
				for( nRetPos--; nRetPos >= 0; nRetPos-- )
				{
					pRet = pDrag->GetObjByPos(nRetPos);
					if( pRet->IsShow() )
					{
						return pRet;
					}
				}
			}
		}
	}
*/
	return NULL;
}

CDrawBase* CGraphics::GetPrevObj(CDrawBase* pCurObj,BOOL bNext)
{
/*	if( m_ayDrag == NULL || 
		m_ayDrag->GetSize() <= 0 )
		return NULL;

	CDragObjManager* pDrag;
	short			 nRetPos;
	for(int i = 0; i < m_ayDrag->GetSize(); i++)
	{
		pDrag = m_ayDrag->GetAt(i);
		if( pDrag != NULL && pDrag->Compare(pCurObj->GetID(),nRetPos) )
		{
			(bNext)?nRetPos++:nRetPos--;
			return pDrag->GetObjByPos(nRetPos);			
		}
	}
*/
	return NULL;
}

CRect CGraphics::GetExplainTextRect()
{
	return CRect(m_GL + 1,m_Rect.top + 1,
		m_GL + 1 + m_PX,
		m_Rect.top + m_rcMargin.top - 1);//m_nMax
}

CRect CGraphics::GetOperatorRect(char cType /*= 0*/)
{
	//add by hxh 2002-6-18
	CRect rect(0,0,0,0);

	if ( IsAxesStyle(AXIS_RIGHTMARGIN) ) // 右边坐标
	{
		rect.SetRect(m_GR,m_Rect.top,m_Rect.right,
			m_Rect.top + m_rcMargin.top);
		rect.right = rect.left;
	}
	else if ( IsAxesStyle(AXIS_LEFTMARGIN) ) // 左边坐标
	{
		rect.SetRect(m_Rect.left,m_Rect.top,m_GL,
			m_Rect.top + m_rcMargin.top);

		if( cType == 1 ) // 绘图
		{
			rect.left = rect.right - m_GL;
		}
		else if( cType == 2 ) // 操作
		{
			rect.left  = m_Rect.left + 1;
			rect.right = rect.left;
		}
	}

	return rect;
}

void CGraphics::DrawOperatorRect(DWORD dwStyle)
{
/*	CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
	if( pImage == NULL || pImage->m_hImageList == NULL )
	{
		return;
	}

	CRect rect = GetOperatorRect(1);
	if( rect.IsRectNull() )
		return;

	if( dwStyle & PKLINE_TYPE_ACTIVE_CHART )
	{
		m_pDC->Draw3dRect(rect,g_hsGraphicsData.clPageBK.GetInvertColor(),
			g_hsGraphicsData.clPageBK.GetInvertColor());
	}

	if( dwStyle & PKLINE_TYPE_TECH_LIST )  // 技术指标列表
	{
		pImage->Draw(m_pDC,CExpression::Yls_Exp_CheckTech,
			CPoint(rect.left,rect.top - 1),ILD_TRANSPARENT);
		rect.left += 16;
	}

	if( dwStyle & PKLINE_TYPE_CHARTUNION )	 // 组合指标图
	{
		pImage->Draw(m_pDC,CExpression::Yls_Exp_TechUnion,
			CPoint(rect.left,rect.top - 1),ILD_TRANSPARENT);
		rect.left += 16;
	}
	else if( dwStyle & PKLINE_TYPE_DRAG_CHART )	// 拖放图表
	{
		pImage->Draw(m_pDC,
			CExpression::Yls_Exp_Drag_Chart,
			CPoint(rect.left,rect.top - 1),ILD_TRANSPARENT);
		rect.left += 16;
	}

	if( dwStyle & PKLINE_TYPE_MAIN_CHART )	 // 放大
	{
		pImage->Draw(m_pDC,
			g_hsGraphicsData.IsStyle(GL_STOCK_INFO_SHOW)?
			CExpression::Yls_Exp_Hor_Right:CExpression::Yls_Exp_Hor_Left,
			CPoint(rect.left,rect.top - 1),ILD_TRANSPARENT);
	}
	else if( dwStyle & PKLINE_TYPE_CLOSE )	 // 关闭指标
	{
		pImage->Draw(m_pDC,CExpression::Yls_Exp_Close,
			CPoint(rect.left,rect.top - 1),ILD_TRANSPARENT);
		rect.left += 16;
	}
*/
}

int CGraphics::IsOperatorRect(CPoint point,DWORD dwStyle /*= 0*/)
{
/*	CRect rect = GetOperatorRect(2);
	if( rect.IsRectNull() )
		return -1;

	rect.right += 16;
	if( dwStyle & PKLINE_TYPE_TECH_LIST )  // 技术指标列表
	{
		if( rect.PtInRect(point) )
			return PKLINE_TYPE_TECH_LIST;
		rect.OffsetRect(16,0);
	}

	if( dwStyle & PKLINE_TYPE_CHARTUNION )	 // multi tech
	{
		if( rect.PtInRect(point) )
			return PKLINE_TYPE_CHARTUNION;
		rect.OffsetRect(16,0);
	}
	else if( dwStyle & PKLINE_TYPE_DRAG_CHART )	 // drag
	{
		if( rect.PtInRect(point) )
			return PKLINE_TYPE_DRAG_CHART;
		rect.OffsetRect(16,0);
	}

	if( dwStyle & PKLINE_TYPE_MAIN_CHART )		 // 放大
	{
		if( rect.PtInRect(point) )
			return PKLINE_TYPE_MAIN_CHART;
	}
	else if( dwStyle & PKLINE_TYPE_CLOSE )		 // close
	{
		if( rect.PtInRect(point) )
			return PKLINE_TYPE_CLOSE;
	}*/
	return -1;
}

CRect CGraphics::GetChartRect()
{
	return CRect(m_GL,m_GT,m_GR,m_GB);
}

CRect CGraphics::GetMarginRect(int nMask)
{
	CRect rect = m_Rect;
	if(nMask & AXIS_LEFTMARGIN)  // 左边界
	{
		rect.right = m_GL;
	}
	else if(nMask & AXIS_TOPMARGIN)  // 上边界
	{
		rect.bottom = m_GT;
	}
	else if(nMask & AXIS_RIGHTMARGIN)  // 右边界
	{
		rect.left = m_GR;
	}
	else if(nMask & AXIS_BOTTOMMARGIN)  // 下边界
	{
		rect.top = m_GB;
	}
	else
	{
		rect.SetRectEmpty();
	}
	return rect;
}

void CGraphics::DrawMaxMin(int nXPos,int nTop,int nBottom,CRect rect,
						   CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax)
{
// 	CTechKLineImp* pPkLine = GetPKLine();
// 	if( pPkLine == NULL || pPkLine->IsAddLine())
// 		return;

	CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawMaxMin));

	CString  strValue;
	SetStringAlign(DT_LEFT, DT_VCENTER);
	CSize size;

	if(g_hsGraphicsData.IsStyle(GL_HighLightText))
		m_pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		m_pDC->SetTextColor(g_hsGraphicsData.clPageBK.GetInvertColor());

	m_pDC->SetTextColor(g_hsGraphicsData.clInChartText.m_lColor);   //qinhn 20100105 Add 图元内部文本颜色用可配置的颜色 
	CPen hPen;
	hPen.CreatePen(g_hsGraphicsData.clPageBK.m_nPenStyle,g_hsGraphicsData.clPageBK.m_nWidth,g_hsGraphicsData.clPageBK.GetInvertColor());
	CPen* hOldPen = m_pDC->SelectObject(&hPen);	

	int nPos,x = nXPos;	
	CRect charRect = GetChartRect();	
	if(pCurMin != NULL)
	{
		nPos = nBottom;
		Format(m_YRefParam.nDecimal, strValue, 
			((m_YRefParam.nDiv != 0)?((double)pCurMin->m_lValue/m_YRefParam.nDiv):pCurMin->m_lValue));
		size = m_pDC->GetTextExtent(strValue, lstrlen(strValue));
		if(nPos < (charRect.top + size.cy))
		{
			nPos = charRect.top + size.cy/2;
		}
		if( nPos > (charRect.bottom - size.cy) )
		{
			nPos = charRect.bottom - size.cy/2;
		}
		if(x > (charRect.right - size.cx))
		{
			x = charRect.right - size.cx - 5;
		}
		//if(x < (charRect.left + size.cx))
		//{
		//	x = charRect.left + size.cx + 5;
		//}

		m_pDC->MoveTo(nXPos,nBottom);
		m_pDC->LineTo(x+5,nPos);

		DrawMarker(nXPos,nBottom, CROSS, 2);

		if( x == charRect.right - size.cx - 5 )
			x -= size.cx;

		if(g_hsGraphicsData.IsStyle(GL_HighLightText))
		{
			charRect.left   = x+5;
			charRect.right  = charRect.left + size.cx;
			charRect.top    = nPos-size.cy/2;
			charRect.bottom = charRect.top + size.cy;
			m_pDC->FillSolidRect(charRect,g_hsGraphicsData.clMaxMinValueBk.m_lColor);// g_hsGraphicsData.crHightBkColor.m_lColor);
		}

		PrintString(x+5,nPos, 0, strValue);
	}

	if(pCurMax != NULL)
	{
		nPos = nTop;// - 6;
		Format(m_YRefParam.nDecimal, strValue,
			((m_YRefParam.nDiv != 0)?((double)pCurMax->m_lValue/m_YRefParam.nDiv):pCurMax->m_lValue) );
		size = m_pDC->GetTextExtent(strValue, lstrlen(strValue));
		if(nPos < (charRect.top + size.cy))
		{
			nPos = charRect.top + size.cy/2;
		}
		if( nPos > (charRect.bottom - size.cy) )
		{
			nPos = charRect.bottom - size.cy/2;
		}
		if(x > (charRect.right - size.cx))
		{
			x = charRect.right - size.cx - 5;
		}
		//if(x < (charRect.left + size.cx))
		//{
		//	x = charRect.left + size.cx + 5;
		//}

		m_pDC->MoveTo(nXPos,nTop);
		m_pDC->LineTo(x+5,nPos);

		DrawMarker(nXPos,nTop, CROSS, 2);

		if( x == charRect.right - size.cx - 5 )
			x -= size.cx;

		if(g_hsGraphicsData.IsStyle(GL_HighLightText))
		{
			charRect.left   = x+5;
			charRect.right  = charRect.left + size.cx;
			charRect.top    = nPos-size.cy/2;
			charRect.bottom = charRect.top + size.cy;
			m_pDC->FillSolidRect(charRect,g_hsGraphicsData.clMaxMinValueBk.m_lColor);// g_hsGraphicsData.crHightBkColor.m_lColor);
		}

		PrintString(x+5,nPos, 0, strValue);
	}

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();

	m_pDC->SelectObject(hOldFont);
}

void CGraphics::DrawPKline( BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						   CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush )
{
	CPen* pOldPen;
	if(bUp)
	{
		pOldPen = m_pDC->SelectObject(pUpPen);

		m_pDC->MoveTo(x, top);
		m_pDC->LineTo(x, bottom);

		rect.left++;rect.top++;
		m_pDC->FillRect(rect,pUpBrush);

		rect.left--;rect.top--;
		m_pDC->MoveTo(rect.left,rect.top);
		m_pDC->LineTo(rect.right, rect.top); 
		m_pDC->LineTo(rect.right,rect.bottom); 
		m_pDC->LineTo(rect.left, rect.bottom); 
		m_pDC->LineTo(rect.left,rect.top);

		m_pDC->SelectObject(pOldPen);
	}
	else
	{
		pOldPen = m_pDC->SelectObject(pDownPen);

		m_pDC->MoveTo(x, top);
		m_pDC->LineTo(x, bottom);

		rect.left++;rect.top++;
		m_pDC->FillRect(rect,pDownBrush);

		rect.left--;rect.top--;
		m_pDC->MoveTo(rect.left,rect.top);
		m_pDC->LineTo(rect.right, rect.top); 
		m_pDC->LineTo(rect.right,rect.bottom); 
		m_pDC->LineTo(rect.left, rect.bottom); 
		m_pDC->LineTo(rect.left,rect.top);

		m_pDC->SelectObject(pOldPen);
	}
}

//ADD BY YUHJ 2008.5.13
void CGraphics::DrawOXline(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						   CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
	POINT pt;
	int w = rect.Width();
	int radius = w >> 1;
	int count  = w ? (abs(bottom - top) / w + 1) : 0;

	CPen* pOldPen;
	if(!bUp)
	{
		pOldPen = m_pDC->SelectObject(pDownPen);

		pt.x = x;
		pt.y = top + radius;

		if(radius > 0)
		{
			for(int i = 0; i < count; i++)
			{
				DrawCircle(pt, radius);
				pt.y += w;
			}
		}
		else
		{
			m_pDC->MoveTo(x, top);
			m_pDC->LineTo(x, bottom);
		}

		m_pDC->SelectObject(pOldPen);
	}
	else
	{
		pOldPen = m_pDC->SelectObject(pUpPen);

		pt.x = x;
		pt.y = top + radius;

		if(radius > 0)
		{
			for(int i = 0; i < count; i++)
			{
				DrawCross(pt, radius);
				pt.y += w;
			}
		}
		else
		{
			m_pDC->MoveTo(x, top);
			m_pDC->LineTo(x, bottom);
		}

		m_pDC->SelectObject(pOldPen);
	}
}
//END

void CGraphics::DrawNPline( CKLineChartParam* sChartParam,StockDay& pValues,BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						   CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush )
{
	CPen* pOldPen = NULL;
	if ( sChartParam->m_nPreUpDown == -1 )
	{
		if ( bUp )
		{
			sChartParam->m_nPreUpDown  = 1;
		}
		else
			sChartParam->m_nPreUpDown  = 0;

		sChartParam->m_nPreHigh = rect.top;
		sChartParam->m_nPreLow  = rect.bottom;
		sChartParam->m_pointFistpoint.x = rect.left;
		return;
	}

	if ( sChartParam->m_nPreUpDown == 1 )		//前一列涨
	{
		if ( bUp && rect.top < sChartParam->m_nPreHigh )	//今天涨,涨破前一列
		{
			pOldPen = m_pDC->SelectObject(pUpPen);

			int width = rect.right - rect.left;

			m_pDC->MoveTo(sChartParam->m_pointFistpoint.x, rect.top);
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width, rect.top); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width,sChartParam->m_nPreHigh); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x, sChartParam->m_nPreHigh); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x,rect.top);

			sChartParam->m_nPreLow    = sChartParam->m_nPreHigh;
			sChartParam->m_nPreHigh   = rect.top;
			sChartParam->m_nPreUpDown = 1;
			sChartParam->m_pointFistpoint.x += width;

			goto DrawNPline_end;

		}
		else if ( bUp && rect.top > sChartParam->m_nPreHigh )   //今天涨,没涨破前一列
		{
			return;		
		}

		float fdown = float(rect.bottom - sChartParam->m_nPreLow) / sChartParam->m_nPreLow;

		if ( !bUp && fdown < sChartParam->m_fChange )   //今天跌，没跌破转折点
		{
			return;
		}
		else if ( !bUp && fdown > sChartParam->m_fChange )   //今天跌，跌破转折点
		{
			pOldPen = m_pDC->SelectObject(pDownPen);

			int width = rect.right - rect.left;

			m_pDC->MoveTo(sChartParam->m_pointFistpoint.x, sChartParam->m_nPreLow);
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width, sChartParam->m_nPreLow); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width, rect.bottom); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x, rect.bottom); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x, sChartParam->m_nPreLow);

			sChartParam->m_nPreHigh   = sChartParam->m_nPreLow;
			sChartParam->m_nPreLow    = rect.bottom;
			sChartParam->m_nPreUpDown = 0;
			sChartParam->m_pointFistpoint.x += width;

			goto DrawNPline_end;
		}
	}
	else if ( sChartParam->m_nPreUpDown == 0 )	//前一列跌
	{
		if ( !bUp && rect.bottom > sChartParam->m_nPreLow )	 //今天跌，跌破前一列
		{
			pOldPen = m_pDC->SelectObject(pDownPen);

			int width = rect.right - rect.left;

			m_pDC->MoveTo(sChartParam->m_pointFistpoint.x,sChartParam->m_nPreLow);
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width, sChartParam->m_nPreLow); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width,rect.bottom); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x, rect.bottom); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x,sChartParam->m_nPreLow);

			sChartParam->m_nPreHigh   = sChartParam->m_nPreLow;
			sChartParam->m_nPreLow    = rect.bottom;
			sChartParam->m_nPreUpDown = 0;
			sChartParam->m_pointFistpoint.x += width;

			goto DrawNPline_end;
		}
		else if ( !bUp && rect.bottom < sChartParam->m_nPreLow )	//今天跌，没跌破昨天最低点
		{
			return;
		}
		float fup = float( sChartParam->m_nPreHigh - rect.top) / sChartParam->m_nPreHigh;

		if ( bUp && fup > sChartParam->m_fChange )	//今天涨，涨破转折点
		{
			pOldPen = m_pDC->SelectObject(pUpPen);

			int width = rect.right - rect.left;

			m_pDC->MoveTo(sChartParam->m_pointFistpoint.x,rect.top);
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width, rect.top); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x + width,sChartParam->m_nPreHigh); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x, sChartParam->m_nPreHigh); 
			m_pDC->LineTo(sChartParam->m_pointFistpoint.x,rect.top);

			sChartParam->m_nPreLow    = sChartParam->m_nPreHigh;
			sChartParam->m_nPreHigh   = rect.top;
			sChartParam->m_nPreUpDown = 1;
			sChartParam->m_pointFistpoint.x += width;

			goto DrawNPline_end;
		}

		else if ( bUp && fup < sChartParam->m_fChange )	//今天涨，没涨破转折点
		{
			return;
		}
	}

DrawNPline_end:
	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}
}

void CGraphics::DrawTowerline( CKLineChartParam* sChartParam,StockDay& pValues,BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
							  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush )
{
	CPen* pOldPen = NULL;
	if ( sChartParam->m_nPreUpDown == -1 )
	{
		if ( bUp )
		{
			sChartParam->m_nPreUpDown  = 1;
		}
		else
			sChartParam->m_nPreUpDown  = 0;

		sChartParam->m_nPreHigh = rect.top;
		sChartParam->m_nPreLow  = rect.bottom;
		return;
	}

	if ( sChartParam->m_nPreUpDown == 1 )		//昨天涨
	{
		if ( bUp )								//今天涨
		{
			pOldPen = m_pDC->SelectObject(pUpPen);

			m_pDC->MoveTo(rect.left,rect.top);
			m_pDC->LineTo(rect.right, rect.top); 
			m_pDC->LineTo(rect.right,rect.bottom); 
			m_pDC->LineTo(rect.left, rect.bottom); 
			m_pDC->LineTo(rect.left,rect.top);

			sChartParam->m_nPreHigh = rect.top;
			sChartParam->m_nPreLow  = rect.bottom;

			goto DrawTowerline_end;
		}
		else if ( rect.bottom < sChartParam->m_nPreLow )   //今天跌，且没跌破昨天低点
		{
			pOldPen = m_pDC->SelectObject(pUpPen);

			m_pDC->MoveTo(rect.left, sChartParam->m_nPreHigh);
			m_pDC->LineTo(rect.right, sChartParam->m_nPreHigh); 
			m_pDC->LineTo(rect.right,rect.bottom); 
			m_pDC->LineTo(rect.left, rect.bottom); 
			m_pDC->LineTo(rect.left,sChartParam->m_nPreHigh);	

			if ( sChartParam->m_nPreHigh < rect.bottom )
			{
				sChartParam->m_nPreHigh = sChartParam->m_nPreHigh;
				sChartParam->m_nPreLow  = rect.bottom;
			}
			else
			{
				sChartParam->m_nPreHigh = rect.bottom;
				sChartParam->m_nPreLow  = sChartParam->m_nPreHigh;
			}
			goto DrawTowerline_end;
		}
		else if ( rect.bottom > sChartParam->m_nPreLow )   //今天跌，且跌破昨天低点
		{
			pOldPen = m_pDC->SelectObject(pUpPen);

			m_pDC->MoveTo(rect.left, sChartParam->m_nPreHigh);
			m_pDC->LineTo(rect.right, sChartParam->m_nPreHigh); 
			m_pDC->LineTo(rect.right, sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left, sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left, sChartParam->m_nPreHigh);

			m_pDC->SelectObject(pDownPen);

			m_pDC->MoveTo(rect.left,sChartParam->m_nPreLow + 1);
			m_pDC->LineTo(rect.right, sChartParam->m_nPreLow + 1); 
			m_pDC->LineTo(rect.right,rect.bottom); 
			m_pDC->LineTo(rect.left, rect.bottom); 
			m_pDC->LineTo(rect.left,sChartParam->m_nPreLow + 1);

			sChartParam->m_nPreUpDown = 0;
			if ( sChartParam->m_nPreHigh < rect.bottom )
			{
				sChartParam->m_nPreHigh   = sChartParam->m_nPreHigh;
				sChartParam->m_nPreLow    = rect.bottom;
			}
			else
			{
				sChartParam->m_nPreHigh   = rect.bottom;
				sChartParam->m_nPreLow    = sChartParam->m_nPreHigh;
			}
			goto DrawTowerline_end;
		}
	}
	else if ( sChartParam->m_nPreUpDown == 0 )	//昨天跌
	{
		if ( !bUp )								//今天跌
		{
			pOldPen = m_pDC->SelectObject(pDownPen);

			m_pDC->MoveTo(rect.left,rect.top);
			m_pDC->LineTo(rect.right, rect.top); 
			m_pDC->LineTo(rect.right,rect.bottom); 
			m_pDC->LineTo(rect.left, rect.bottom); 
			m_pDC->LineTo(rect.left,rect.top);


			sChartParam->m_nPreHigh   = rect.top;
			sChartParam->m_nPreLow    = rect.bottom;

			goto DrawTowerline_end;
		}
		else if ( bUp && rect.top > sChartParam->m_nPreHigh )	//今天涨，没涨破昨天最高点
		{
			pOldPen = m_pDC->SelectObject(pDownPen);

			m_pDC->MoveTo(rect.left, rect.top);
			m_pDC->LineTo(rect.right, rect.top); 
			m_pDC->LineTo(rect.right,sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left, sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left,rect.top);

			if ( sChartParam->m_nPreLow > rect.top )
			{
				sChartParam->m_nPreHigh   = rect.top;
				sChartParam->m_nPreLow    = sChartParam->m_nPreLow;
			}
			else
			{
				sChartParam->m_nPreHigh   = sChartParam->m_nPreLow;
				sChartParam->m_nPreLow    = rect.top;
			}

			goto DrawTowerline_end;
		}
		else if ( bUp && rect.top < sChartParam->m_nPreHigh )	//今天涨，涨破昨天最高点
		{
			pOldPen = m_pDC->SelectObject(pDownPen);

			m_pDC->MoveTo(rect.left,sChartParam->m_nPreHigh);
			m_pDC->LineTo(rect.right, sChartParam->m_nPreHigh); 
			m_pDC->LineTo(rect.right,sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left, sChartParam->m_nPreLow); 
			m_pDC->LineTo(rect.left,sChartParam->m_nPreHigh);

			m_pDC->SelectObject(pUpPen);

			m_pDC->MoveTo(rect.left,sChartParam->m_nPreHigh - 1);
			m_pDC->LineTo(rect.right, sChartParam->m_nPreHigh - 1); 
			m_pDC->LineTo(rect.right, rect.top); 
			m_pDC->LineTo(rect.left, rect.top); 
			m_pDC->LineTo(rect.left,sChartParam->m_nPreHigh - 1);

			sChartParam->m_nPreUpDown = 1;
			if ( sChartParam->m_nPreLow > rect.top )
			{
				sChartParam->m_nPreHigh   = rect.top;
				sChartParam->m_nPreLow    = sChartParam->m_nPreLow;
			}
			else
			{
				sChartParam->m_nPreHigh   = sChartParam->m_nPreLow;
				sChartParam->m_nPreLow    = rect.top;
			}
			goto DrawTowerline_end;
		}
	}
DrawTowerline_end:
	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}

}

void CGraphics::DrawAmerica( BOOL& bUp,int& nX,int& nTop,int& nBottom,CRect& rect,
							CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
	CPen* pOldPen = NULL;
	if(bUp)
	{
		pOldPen = m_pDC->SelectObject(pUpPen);

		m_pDC->MoveTo(nX, nTop);
		m_pDC->LineTo(nX, nBottom);

		m_pDC->MoveTo(nX,rect.top);
		m_pDC->LineTo(rect.right, rect.top); 
		m_pDC->MoveTo(rect.left,rect.bottom);
		m_pDC->LineTo(nX,rect.bottom);
	}
	else
	{
		pOldPen = m_pDC->SelectObject(pDownPen);

		m_pDC->MoveTo(nX, nTop);
		m_pDC->LineTo(nX, nBottom);

		m_pDC->MoveTo(rect.left,rect.top);
		m_pDC->LineTo(nX, rect.top); 
		m_pDC->MoveTo(nX,rect.bottom);
		m_pDC->LineTo(rect.right,rect.bottom);
	}

	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}
}

void CGraphics::DrawCNDEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						   CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
	rect.left = x - pValues.m_lTotal / sChartParam->m_nValues * GetChartRect().Width() / 2;
	rect.right = x + pValues.m_lTotal / sChartParam->m_nValues * GetChartRect().Width() / 2;

	CPen* pOldPen = NULL;
	if(bUp)
	{
		pOldPen = m_pDC->SelectObject(pUpPen);

		m_pDC->MoveTo(x, top);
		m_pDC->LineTo(x, rect.top);
		m_pDC->MoveTo(x, bottom);
		m_pDC->LineTo(x, rect.bottom);


		m_pDC->MoveTo(rect.left,rect.top);
		m_pDC->LineTo(rect.right, rect.top); 
		m_pDC->LineTo(rect.right, rect.bottom);
		m_pDC->LineTo(rect.left,rect.bottom);
		m_pDC->LineTo(rect.left,rect.top);
	}
	else
	{
		pOldPen = m_pDC->SelectObject(pDownPen);

		m_pDC->MoveTo(x, top);
		m_pDC->LineTo(x, rect.top);
		m_pDC->MoveTo(x, bottom);
		m_pDC->LineTo(x, rect.bottom);

		m_pDC->MoveTo(rect.left,rect.top);
		m_pDC->LineTo(rect.right, rect.top); 
		m_pDC->LineTo(rect.right, rect.bottom);
		m_pDC->LineTo(rect.left,rect.bottom);
		m_pDC->LineTo(rect.left,rect.top);
	}

	if( pOldPen ) 
	{
		m_pDC->SelectObject(pOldPen);
	}
}

void CGraphics::DrawEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
	CPen* pOldPen = NULL;
	if ( sChartParam->m_pointFistpoint == CPoint( -1, -1 ))
	{
		sChartParam->m_pointFistpoint.x = rect.left;
	}
	rect.left = sChartParam->m_pointFistpoint.x;
	rect.right = rect.left + pValues.m_lTotal / sChartParam->m_nValues * GetChartRect().Width();

	pOldPen = m_pDC->SelectObject(pUpPen);	

	if(bUp)
	{	
		m_pDC->MoveTo(rect.left, top);
		m_pDC->LineTo(rect.right -1, top); 
		m_pDC->LineTo(rect.right -1, bottom);
		m_pDC->LineTo(rect.left, bottom);
		m_pDC->LineTo(rect.left, top);
	}
	else
	{
		m_pDC->MoveTo(rect.left, top);
		m_pDC->LineTo(rect.right -1, top); 
		m_pDC->LineTo(rect.right -1, bottom);
		m_pDC->LineTo(rect.left, bottom);
		m_pDC->LineTo(rect.left, top);
	}

	sChartParam->m_pointFistpoint.x = rect.right;

	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}
}

void CGraphics::DrawKEQV(CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						 CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
	if ( sChartParam->m_pointFistpoint == CPoint( -1, -1 ))
	{
		sChartParam->m_pointFistpoint.x = rect.left;
	}
	rect.left = sChartParam->m_pointFistpoint.x;
	rect.right = rect.left + pValues.m_lTotal / sChartParam->m_nValues * GetChartRect().Width();

	CPen* pOldPen = NULL;
	if(bUp)
	{
		pOldPen = m_pDC->SelectObject(pUpPen);

		m_pDC->MoveTo(rect.left, top);
		m_pDC->LineTo(rect.left, bottom);

		m_pDC->MoveTo(rect.left, rect.top);
		m_pDC->LineTo(rect.right -1, rect.top); 
		m_pDC->LineTo(rect.right -1, rect.bottom);
		m_pDC->LineTo(rect.left, rect.bottom);
		m_pDC->LineTo(rect.left, rect.top);
	}
	else
	{
		pOldPen = m_pDC->SelectObject(pDownPen);	

		m_pDC->MoveTo(rect.left, top);
		m_pDC->LineTo(rect.left, bottom);

		m_pDC->MoveTo(rect.left, rect.top);
		m_pDC->LineTo(rect.right -1, rect.top); 
		m_pDC->LineTo(rect.right -1, rect.bottom);
		m_pDC->LineTo(rect.left, rect.bottom);
		m_pDC->LineTo(rect.left, rect.top);
	}

	sChartParam->m_pointFistpoint.x = rect.right;

	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}

}
void CGraphics::DrawClose( CKLineChartParam* sChartParam,StockDay& pValues, BOOL& bUp,int& x,int& top,int& bottom,CRect& rect,
						  CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{	

	CPen* pOldPen = NULL;
	if ( sChartParam->m_pointFistpoint == CPoint( -1, -1 ))
	{
		sChartParam->m_pointFistpoint.x = x;

		if ( bUp )
			sChartParam->m_pointFistpoint.y = rect.top;
		else
			sChartParam->m_pointFistpoint.y = rect.bottom;
		m_pDC->MoveTo( sChartParam->m_pointFistpoint );
		return;
	}

	else if( sChartParam->m_pointFistpoint.x == x )
	{
		m_pDC->MoveTo( sChartParam->m_pointFistpoint );
		return;
	}

	if(bUp)
	{		
		//		m_pDC->MoveTo(sChartParam->m_pointFistpoint);
		pOldPen = m_pDC->SelectObject(pUpPen);
		m_pDC->LineTo(x, rect.top);
	}
	else
	{	
		//		m_pDC->MoveTo(sChartParam->m_pointFistpoint);
		pOldPen = m_pDC->SelectObject(pDownPen);

		m_pDC->LineTo(x, rect.bottom);
	}
	if( pOldPen )
	{
		m_pDC->SelectObject(pOldPen);
	}
}

BOOL CGraphics::DrawPKByMark(CKLineChartParam* sChartParam,StockDay& pValues,
							 CDragObjManager* pDragObjManager,
							 CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
							 BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
							 CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush,
							 BOOL bSpecial)
{
	char cDefalut = 1;
	char cDraw	  = 0;

/*	if( pDragObjManager != NULL && pDragObjManager->GetCount() > 1 ) 
	{
		CTechAnalysisObj* pTechObj;
		CDrawBase* pObj;
		CLineProperty*    pData;
		int i;

		CRect rcFocus;
		int nRet;

		for(int nPos = 0; nPos < pDragObjManager->GetCount(); nPos++)
		{
			pObj = pDragObjManager->GetObjByPos(nPos);
			if( pObj == NULL )//|| 
				//!pObj->IsKindOf(RUNTIME_CLASS(CDrawTechKLine)) )
				continue;
			pTechObj = ((CDrawTechKLine*)pObj)->GetTechAnaObj();
			if( pTechObj == NULL || 
				pTechObj->IsStyle(CExpression::Tech) ||
				pTechObj->IsPKLine() )
				continue;
#if 1
			if( pTechObj->IsStyle(CExpression::MoreKLine) )
			{
#ifdef Support_Hlt
				CNameProperty* pCNameProperty = pTechObj->GetNameProperty();
				if( pCNameProperty != NULL && !pCNameProperty->m_strName.CompareNoCase("弘历太极") )
				{
				}
				else
#endif
				{
					cDefalut = 0;
				}
			}
#endif
			for( i = 0; i < pTechObj->GetSize(); i++ ) // 多个
			{ 
				pData = pTechObj->GetDataAt(i);
				if( pData == NULL	 ||
					pData->IsEmpty() || 
					!pData->IsStyle(HX_EXPRESSDRAW) )
					continue;

				nRet = pData->IsTrue(nDataPos); // 2007.08.23 add
				if( !nRet )
					continue;

				if( pTechObj->IsStyle(CExpression::Condition) )
				{
					//区分选中图标与非选中图标

					rcFocus.left   = x - 2;
					rcFocus.top    = bottom;
					rcFocus.right  = rcFocus.left + ImageFocus_Width;
					rcFocus.bottom = rcFocus.top  + ImageFocus_Height;

					CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
					if( pImage != NULL && pImage->m_hImageList != NULL )
					{
						pImage->Draw(m_pDC, CExpression::Yls_Exp_SelectStock,
							CPoint(x - 1,bottom + 1),
							(pTechObj->IsStyle(KLINE_SIGN_CONDITION)?ILD_FOCUS:0) | ILD_TRANSPARENT,
							rcFocus);
					}
				}
				else if( pTechObj->IsStyle(CExpression::Exchange) 
					|| pTechObj->IsStyle(CExpression::Strategy))
				{
					//int nMask1,nMask2;
					CRect rc1;//,rc2;

					CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawMaxMin));
					m_pDC->SetTextColor(pData->m_lColor.m_lColor);

					int nImage;
					CString str = pData->m_strName.Left(pData->m_strName.GetLength() - 3);//yulx add 去掉最后的000
					if( !str.CompareNoCase("EXITLONG") ) // 多头卖出
					{
						//nMask1 = MAKELONG(0,1);
						//nMask2 = MAKELONG(0,2);
						//rc2.left = rc1.left = x - 6;

						rc1.left = x - 9;
						rc1.top  = top - 17; 

						rcFocus.left   = x - 6;

						//rc2.top  = rect.top;
						//区分选中图标与非选中图标
						//modify by lxqi 20090506
						if (bSpecial)
						{
							nImage = CExpression::Yls_Exp_Union_Sub;			
						} 
						else
						{
							nImage = CExpression::Yls_Exp_OpenInfo + 9;	
						}


						m_pDC->TextOut(rc1.left + 2,rc1.top - 17,HS_LANGUAGE("卖"));
					}
					else
					{
						//nMask1 = MAKELONG(1,1);
						//nMask2 = MAKELONG(1,2);
						//rc2.left = rc1.left = x - 6;

						rc1.left = x - 6;
						rc1.top  = bottom + 2;

						rcFocus.left   = rc1.left;
						//modify by lxqi 20090506
						if (bSpecial)
						{
							nImage = CExpression::Yls_Exp_Union_Add;			
						} 
						else
						{
							nImage = CExpression::Yls_Exp_SelectStock + 2;
						}

						m_pDC->TextOut(rc1.left,rc1.top + 15,HS_LANGUAGE("买"));
					}


					rcFocus.top    = rc1.top;
					rcFocus.right  = rcFocus.left + ImageFocus_Width;
					rcFocus.bottom = rcFocus.top  + ImageFocus_Height;

					CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
					if( pImage != NULL && pImage->m_hImageList != NULL )
					{
						pImage->Draw(m_pDC,nImage,
							CPoint(rc1.left,rc1.top),
							(pTechObj->IsStyle(KLINE_SIGN_EXCHANGE)?ILD_FOCUS:0) | ILD_TRANSPARENT,
							rcFocus);
					}

					//g_hsGraphicsData.DrawArrow(m_pDC,rc1,pData->m_lColor.m_lColor,nMask1);
					//g_hsGraphicsData.DrawArrow(m_pDC,rc2,pData->m_lColor.m_lColor,nMask2);	

					m_pDC->SelectObject(hOldFont);
				}
				else if( pTechObj->IsStyle(CExpression::MoreKLine) )
				{
					cDefalut = 0;
					COLORREF cl;
					if( nRet == 2 )
					{
						cl = RGB(0,0,255);
					}
					else if( nRet == 4 )
					{
						cl = RGB(255,255,0);
					}
					else
					{
						cl = pData->m_lColor.m_lColor;
					}

					CPen pen(pData->m_lColor.m_nPenStyle,
						pData->m_lColor.m_nWidth,cl);
					CBrush brush(cl);
					if( sChartParam->IsType(IDR_hexinKLineChart) )
					{
						DrawPKline( bUp,x,top,bottom,rect,
							&pen,pUpBrush,&pen,&brush );
					}
					else if( sChartParam->IsType(IDR_hexinAmericaChart) )
					{
						DrawAmerica( bUp,x,top,bottom,rect,
							&pen,pUpBrush,&pen,&brush );
					}
					else if( sChartParam->IsType(IDR_hexinCloseChart) )
					{
						DrawClose(sChartParam,pValues, bUp,x,top,bottom,rect,
							&pen,pUpBrush,&pen,&brush );
					}
					else if( sChartParam->IsType(IDR_hexinNewPriceChart) )
					{
						DrawCNDEQV(sChartParam,pValues, bUp,x,top,bottom,rect,
							&pen,pUpBrush,&pen,&brush );
					}
					else if( sChartParam->IsType(IDR_hexinTowerChart) )
					{
						DrawTowerline(sChartParam,pValues, bUp,x,top,bottom,rect,
							&pen,pUpBrush,&pen,&brush );
					}
					cDraw = 1;
				}
			}			
		}
	}*/

	if( cDefalut )
	{
		if( sChartParam->IsType(IDR_hexinKLineChart) )
		{
			DrawPKline( bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
		}
		else if( sChartParam->IsType(IDR_hexinAmericaChart) )
		{
			DrawAmerica( bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
		}
		else if( sChartParam->IsType(IDR_hexinAxisCndeqv) )
		{
			DrawCNDEQV(sChartParam, pValues, bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
		}
		else if( sChartParam->IsType(IDR_hexinCloseChart) )
		{
			DrawClose(sChartParam, pValues, bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
		}
		else if( sChartParam->IsType(IDR_hexinOXChart) )
		{
			//CHANGE BY YUHJ 2008.5.14
			DrawOXline(sChartParam,pValues, bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
			//END
		}
		else if( sChartParam->IsType(IDR_hexinTowerChart) )
		{
			DrawTowerline(sChartParam,pValues, bUp,x,top,bottom,rect,
				pUpPen,pUpBrush,pDownPen,pDownBrush );
		}
	}
	else
	{
		if( !cDraw )
		{
			if( sChartParam->IsType(IDR_hexinKLineChart) )
			{
				DrawPKline( bUp,x,top,bottom,rect,
					pDownPen,pUpBrush,pDownPen,pDownBrush );
			}
/*			else if( sChartParam->IsType(IDR_hexinAmericaChart) )
			{
				DrawAmerica( bUp,x,top,bottom,rect,
					pDownPen,pUpBrush,pDownPen,pDownBrush );
			}
			else if( sChartParam->IsType(IDR_hexinCloseChart) )
			{
				DrawClose(sChartParam,pValues, bUp,x,top,bottom,rect,
					pDownPen,pUpBrush,pDownPen,pDownBrush );
			}
			else if( sChartParam->IsType(IDR_hexinAxisCndeqv) )
			{
				DrawCNDEQV(sChartParam, pValues, bUp,x,top,bottom,rect,
					pDownPen,pUpBrush,pDownPen,pDownBrush );
			}	
			else if( sChartParam->IsType(IDR_hexinOXChart) )
			{
				//CHANGE BY YUHJ 2008.5.14
				DrawOXline(sChartParam,pValues, bUp,x,top,bottom,rect,
					pUpPen,pUpBrush,pDownPen,pDownBrush );
				//END
			}
			else if( sChartParam->IsType(IDR_hexinTowerChart) )
			{
				DrawTowerline(sChartParam,pValues, bUp,x,top,bottom,rect,
					pDownPen,pUpBrush,pDownPen,pDownBrush );
			}*/
		}
	}

	return FALSE;
}

BOOL CGraphics::DrawCloseByMark(StockDay& pValues,
								CDragObjManager* pDragObjManager,
								CDispMaxMinValue* pCurMin,CDispMaxMinValue* pCurMax,
								BOOL bUp,int nDataPos,int x,int top,int bottom,CRect rect,
								CPen* pUpPen,CBrush* pUpBrush,CPen* pDownPen,CBrush* pDownBrush)
{
/*	char cDefalut = 1;
	char cDraw	  = 0;

	if( pDragObjManager != NULL && pDragObjManager->GetCount() > 1 )
	{
		CTechAnalysisObj* pTechObj;
		CDrawBase* pObj;
		CLineProperty*    pData;
		int i;
		for(int nPos = 0; nPos < pDragObjManager->GetCount(); nPos++)
		{
			pObj = pDragObjManager->GetObjByPos(nPos);
			if( pObj == NULL )//|| 
				//!pObj->IsKindOf(RUNTIME_CLASS(CDrawTechKLine)) )
				continue;
			pTechObj = ((CDrawTechKLine*)pObj)->GetTechAnaObj();
			if( pTechObj == NULL || 
				pTechObj->IsStyle(CExpression::Tech) ||
				pTechObj->IsPKLine() )
				continue;

			for( i = 0; i < pTechObj->GetSize(); i++ ) // 多个
			{     
				pData = pTechObj->GetDataAt(i);
				if( pData != NULL &&
					!pData->IsEmpty() && 
					pData->IsStyle(HX_EXPRESSDRAW) )
				{
					if( pData->IsTrue(nDataPos) )
					{
						CPen pen(pData->m_lColor.m_nPenStyle,
							pData->m_lColor.m_nWidth,pData->m_lColor.m_lColor);
						CBrush brush(pData->m_lColor.m_lColor);

						CPen* pOldPen = NULL;
						CBrush* pOldBrush = NULL;

						if(bUp)
						{
							pOldPen = m_pDC->SelectObject(&pen);
							//							m_pDC->SelectObject(pUpBrush);

							m_pDC->MoveTo(x, top);
							m_pDC->LineTo(x, bottom);

							m_pDC->MoveTo(x,rect.top);
							m_pDC->LineTo(rect.right, rect.top); 
							m_pDC->MoveTo(rect.left,rect.bottom);
							m_pDC->LineTo(x,rect.bottom);
						}
						else
						{
							pOldPen = m_pDC->SelectObject(&pen);
							//							pOldBrush = m_pDC->SelectObject(&brush);

							m_pDC->MoveTo(x, top);
							m_pDC->LineTo(x, bottom);

							m_pDC->MoveTo(rect.left,rect.top);
							m_pDC->LineTo(x, rect.top); 
							m_pDC->MoveTo(x,rect.bottom);
							m_pDC->LineTo(rect.right,rect.bottom);
						}

						//m_pDC->MoveTo(x, top);
						//m_pDC->LineTo(x, bottom);
						//m_pDC->Rectangle(rect);

						if(pOldPen != NULL)
						{
							m_pDC->SelectObject(pOldPen);
						}
						if(pOldBrush != NULL)
						{
							m_pDC->SelectObject(pOldBrush);
						}

						cDraw = 1;
					}					
				}
			}
			cDefalut = 0;
		}
	}

	if( cDefalut )
	{
		if( bUp )
		{
			m_pDC->SelectObject(pUpPen);
			//			m_pDC->SelectObject(pUpBrush);

			m_pDC->MoveTo(x, top);
			m_pDC->LineTo(x, bottom);

			m_pDC->MoveTo(x,rect.top);
			m_pDC->LineTo(rect.right, rect.top); 
			m_pDC->MoveTo(rect.left,rect.bottom);
			m_pDC->LineTo(x,rect.bottom);

		}
		else
		{
			m_pDC->SelectObject(pDownPen);
			//			m_pDC->SelectObject(pDownBrush);

			m_pDC->MoveTo(x, top);
			m_pDC->LineTo(x, bottom);

			m_pDC->MoveTo(rect.left,rect.top);
			m_pDC->LineTo(x, rect.top); 
			m_pDC->MoveTo(x,rect.bottom);
			m_pDC->LineTo(rect.right,rect.bottom);
		}
	}
	else
	{
		if( !cDraw )
		{
			if(bUp)
			{
				m_pDC->SelectObject(pDownPen);
				//				m_pDC->SelectObject(pUpBrush);

				m_pDC->MoveTo(x, top);
				m_pDC->LineTo(x, bottom);

				m_pDC->MoveTo(x,rect.top);
				m_pDC->LineTo(rect.right, rect.top); 
				m_pDC->MoveTo(rect.left,rect.bottom);
				m_pDC->LineTo(x,rect.bottom);


			}
			else
			{
				m_pDC->SelectObject(pDownPen);
				//				m_pDC->SelectObject(pDownBrush);

				m_pDC->MoveTo(x, top);
				m_pDC->LineTo(x, bottom);

				m_pDC->MoveTo(rect.left,rect.top);
				m_pDC->LineTo(x, rect.top); 
				m_pDC->MoveTo(x,rect.bottom);
				m_pDC->LineTo(rect.right,rect.bottom);
			}
		}			

		//			m_pDC->MoveTo(x, top);
		//			m_pDC->LineTo(x, bottom);			
		//			m_pDC->Rectangle(rect);
	}

	DrawPKMessageMask( x,m_GB,rect,pValues );
*/
	return FALSE;
}

void CGraphics::DrawPKMessageMask(int x,int y,CRect& rect,StockDay& pValues)
{
/*	if( pValues.m_nMessageMask & SD_NEWS )			//重要新闻
	{
		CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
		if( pImage != NULL && pImage->m_hImageList != NULL )
		{
			//这里画K线图 信息地雷 的图标，目前信息地雷为一个类似菱形的图案,其是调用一长条图片里面的一个小图.
			//参看 CPublicData::InitExpress 函数里面m_expressImages的创建过程  现信息地雷的图案可配置， 
			//不固定用CExpression::Yls_Exp_Info_Mark所指定位置的图案。 qinhn  20090603 Modify
			if(bEverRead)
			{
				CString sFileName = CHSFilePath::GetPath(CHSFilePath::Setting) + "HappyStar.ini";
			//	bool readR = CWriteReadStandardConfig::ReadConfigInt("信息地雷图案", "图案位置编号", nMarkIndex, sFileName);
				if(!CfgValues::GetDetailCfgInt(m_CfgTopMapOb,"信息地雷图案", "图案位置编号" , nMarkIndex))
					nMarkIndex =  CExpression::Yls_Exp_Info_Mark;
					
				if(nMarkIndex <=0 || nMarkIndex >100)
					nMarkIndex = CExpression::Yls_Exp_Info_Mark;
				bEverRead = false;
			}

			pImage->Draw(m_pDC,nMarkIndex,    
				CPoint(x - 8,m_GT + 1),ILD_TRANSPARENT);
		}
	}
	if( pValues.m_nMessageMask & SD_BULLETIN )		//公告信息
	{
	}
	if( pValues.m_nMessageMask & SD_SPECIFICATION )	//上市通知
	{
	}
	if( pValues.m_nMessageMask & SD_MIDREPORT )	    //中报摘要
	{
	}
	if( pValues.m_nMessageMask & SD_ANNALS )		//年报摘要
	{
	}
	if( pValues.m_nMessageMask & SD_SPSCHEME )		//送配方案
	{
	}
	if( pValues.m_nMessageMask & SD_REVIEW )		//股市评说
	{
	}
	if (pValues.m_nMessageMask & SD_SelectStockMark)
	{
		CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
		if( pImage != NULL && pImage->m_hImageList != NULL )
		{
			pImage->Draw(m_pDC,CExpression::Yls_Exp_SelectStock,
				CPoint(x - 4,y - 16 - 1),ILD_TRANSPARENT);
		}
	}
	if (pValues.m_nMessageMask & SD_MEMOINFO)//绘制备忘录图标
	{
		CImageListEx* pImage = g_hsGraphicsData.m_expressImages; 
		if( pImage != NULL && pImage->m_hImageList != NULL )
		{
			pImage->Draw(m_pDC,CExpression::Yls_Exp_MemoInfo,
				CPoint(x - 8, y - 32 - 1),ILD_TRANSPARENT);
		}
	}
	if( pValues.m_nMessageMask & SD_CHUQUAN )		//除权数据
	{
		CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
		if( pImage != NULL && pImage->m_hImageList != NULL )
		{
			if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_RIGHT_CHUQUAN) )
			{
				pImage->Draw(m_pDC,CExpression::Exright_Right,
					CPoint(x + 3,y - 16),ILD_TRANSPARENT);
			}
			else 	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_AUTO_CHUQUAN) )
			{
				pImage->Draw(m_pDC,CExpression::Exright_Lift,
					CPoint(x - 18,y - 16),ILD_TRANSPARENT);
			}
			pImage->Draw(m_pDC,CExpression::Yls_Exp_ChuQuan,
				CPoint(x - 8,y - 16 - 1),ILD_TRANSPARENT);
		}
	}*/
}

void CGraphics::SetExplainRect( CRect rect,CDrawBase* pThis,BOOL bAuto /*= FALSE*/)
{
/*	if(bAuto == 3)
	{
		m_rcExplain = rect;
		return;
	}
	if(bAuto == 2)
		return;
	else if( bAuto )
	{
		if(	!m_rcExplain.IsRectEmpty() )
		{
			m_rcExplain = rect;
			return;
		}
		return;
	}
	else
	{
		m_rcExplain = rect;
		if(	!m_rcExplain.IsRectEmpty() )
		{
			AddAxesStyle(AXIS_SELEXPLAINRECT);
		}
	}

	if( m_ayDrawList == NULL || pThis == NULL )
		return;

	CHSMinuteFigure* pDrawObj;  // 绘制对象
	for( int i = 0; i < m_ayDrawList->GetSize();i++ )
	{
		pDrawObj = (CHSMinuteFigure*)m_ayDrawList->GetAt(i);
		if( pDrawObj != NULL )//&& 
			//pDrawObj->IsKindOf(RUNTIME_CLASS(CHSMinuteFigure)) )
		{
			if( bAuto )
			{
				if( pDrawObj == pThis &&	
					!pDrawObj->GetLinear()->GetExplainRect().IsRectEmpty() )
				{
					pDrawObj->GetLinear()->SetExplainRect(rect,pDrawObj,bAuto);
					return;
				}
			}
			else
			{
				if(pDrawObj != pThis && pDrawObj->GetLinear()->IsAxesStyle(AXIS_SELEXPLAINRECT) )
				{
					pDrawObj->GetLinear()->SetExplainEmpty();
				}
			}
		}
	}*/
}

void CGraphics::SetExplainEmpty() 
{ 
	if( !IsAxesStyle(AXIS_SELEXPLAINRECT) )
		return;
	RemoveAxesStyle(AXIS_SELEXPLAINRECT);
	m_rcExplain.SetRectEmpty();
}

int CGraphics::GetAxisRect(CPoint& pt)
{
	return -1;
}

CRect CGraphics::GetExplainMenuRect()
{
	CRect rect;
	if( IsAxesStyle(AXIS_SELEXPLAINRECT) )
	{
		rect.SetRect(m_rcExplain.left,m_rcExplain.top,
			m_rcExplain.left+m_MenuSize.cx,
			m_rcExplain.bottom);
	}
	else
		rect.SetRectEmpty();
	return rect;
}

CDrawBase* CGraphics::GetPKLine()
{
	return (CDrawBase*)m_pThis; // 一定是它
}

CDispMaxMinValue* CGraphics::GetMaxMin(BOOL bMax)
{
// 	CTechKLineImp* pPKLine = GetPKLine();
// 	if(pPKLine == NULL)
// 		return NULL;
// 	if(bMax)
// 	{
// 		return pPKLine->GetCurMaxValue();
// 	}
// 	else
// 	{
// 		return pPKLine->GetCurMinValue();
// 	}

	return NULL;
}

BOOL CGraphics::IsUp(int nPosition)
{
// 	CTechKLineImp* pPKline = GetPKLine();
// 	if(pPKline != NULL)
// 	{
// 		return pPKline->IsUp(nPosition);
// 	}
	return FALSE;
}

int CGraphics::GetFontSize(char cType)
{
	switch(cType)
	{
	case ftTitle:	   // 标题
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -15.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			m_LogFont.lfWeight	   = 700;
		}
		break;
	case ftXAxisTitle: // 横向标题
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -20.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			m_LogFont.lfWeight	   = 700;
		}
		break;
	case ftYAxisTitle: // 纵向标题
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -20.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			m_LogFont.lfWeight	   = 700;
		}
		break;
	case ftLegend:     // 图元标注
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -25.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			m_LogFont.lfWeight	   = 500;
		}
		break;
	case ftXAxis:      // 横向坐标
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -25.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight)
				m_LogFont.lfHeight = m_nFontDefHeight;
			if (abs(m_LogFont.lfHeight) > m_rcMargin.bottom - m_TickLineLen.bm - 1)
				m_LogFont.lfHeight = -(m_rcMargin.bottom - m_TickLineLen.bm - 1);
			m_LogFont.lfWeight     = 500;
		}
		break;
	case ftYAxis:	  // 纵向坐标
		{
			m_LogFont.lfHeight = -15;
			m_LogFont.lfWeight = 500;
		}
		//break;
	case ftFenShiYAxis:// 纵向左边坐标
	case ftRightYTick: // 纵向右边坐标
	case ftFenShiRightYTick: // 纵向右边坐标
		break;
	case ftMulti:          // 放大倍数
		{
			m_LogFont.lfHeight = -12;		
		}
		break;
	case ftDrawVarTitle:   // 顶部说明
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -10.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			if (abs(m_LogFont.lfHeight) > m_rcMargin.top - m_TickLineLen.tm - 2) 
				m_LogFont.lfHeight = -(m_rcMargin.top - m_TickLineLen.tm - 2);
			m_LogFont.lfWeight	   = 0;
		}
		break;
	case ftDrawMaxMin:     // 最大最小值
		{
			m_LogFont.lfHeight = (int)(m_Size.cx / -10.0);
			if (m_LogFont.lfHeight > m_nFontDefHeight) 
				m_LogFont.lfHeight = m_nFontDefHeight;
			if (abs(m_LogFont.lfHeight) > m_rcMargin.top - m_TickLineLen.tm - 2) 
				m_LogFont.lfHeight = -(m_rcMargin.top - m_TickLineLen.tm - 2);
			m_LogFont.lfWeight	   = 700;
		}
		break;
	}
	return m_LogFont.lfHeight;
}

CFont* CGraphics::GetFont(char cType)
{
	switch(cType)
	{
	case ftXAxis:			 // 横向坐标
		return g_hsGraphicsData.ftXAxis.m_pFont;
	case ftYAxis:			 // 纵向坐标
	case ftFenShiYAxis:		 // 纵向左边坐标
	case ftRightYTick:		 // 纵向右边坐标
	case ftFenShiRightYTick: // 纵向右边坐标
		return g_hsGraphicsData.ftYAxis.m_pFont;
	case ftMulti:            // 放大倍数
		return g_hsGraphicsData.ftMulti.m_pFont;
	case ftDrawVarTitle:     // 顶部说明
		return g_hsGraphicsData.ftDrawVarTitle.m_pFont;
	case ftDrawMaxMin:       // 最大最小值
		return g_hsGraphicsData.ftDrawInText.m_pFont;
	}

	return g_hsGraphicsData.ftDrawInText.m_pFont;
	return NULL;
}

int CGraphics::GetYLeftMaxWidth(CHSBaseDC* pDC,BOOL bOneCharWidth /*= FALSE*/)
{
	if( pDC == NULL || 
		m_YRefParam.max == 0 )
		return -1;

	CDrawBase* pPKline = GetPKLine();
	if(pPKline != NULL)
	{
		CString  strText;
		float value;
		CSize size;

		CFont* hOldFont = pDC->SelectObject(GetFont(ftFenShiYAxis));

		value = (float)m_YRefParam.max;
		if(m_YRefParam.nDiv != 0)
			value /= m_YRefParam.nDiv;
		Format(m_YRefParam.nDecimal, strText, value);
		size = pDC->GetTextExtent(strText, lstrlen(strText));

		pDC->SelectObject(hOldFont);

		if(bOneCharWidth)
		{
			if(lstrlen(strText) > 0)
			{
				return MAKELONG(size.cx,lstrlen(strText));
			}
			else
			{
				return 8;
			}
		}
		else
		{
			return (size.cx + m_TickLineLen.lm);
		}
	}

	return -1;
}

int CGraphics::GetYRightMaxWidth(CHSBaseDC* pDC)
{
/*	if( pDC == NULL || 
		m_YRefParam.max == 0 )
	{
		return -1;
	}

	CDrawTechKLine* pPKline = GetPKLine();
	if(pPKline != NULL)
	{		
		if( pPKline->IsFenshiDrawType() )
		{
			char  strText[32];
			float value;
			CSize size;

			CFont* hOldFont = pDC->SelectObject(GetFont(ftFenShiYAxis));				

			value = (float)m_YRefParam.max;
			double fRate = 0;
			if(m_YRefParam.ref != 0)
			{
				fRate = fabs((value - m_YRefParam.ref) / m_YRefParam.ref) * 100;
			}

			if(m_YRefParam.nDiv != 0)
				value /= m_YRefParam.nDiv;

			char strFormat[10];
			sprintf(strFormat, "%%.%df%%%%", 2);
			sprintf(strText, strFormat, fRate);				
			size = pDC->GetTextExtent(strText, lstrlen(strText));

			pDC->SelectObject(hOldFont);

			return (size.cx + m_TickLineLen.rm);
		}
		else
		{
			return m_rcMargin.right;//m_rcDefMargin.right.m_nDef;
		}	
	}*/
	return -1;
}

void CGraphics::CalcLeftRightWidth(CHSBaseDC* pDC)
{
/*	if( m_ayDrawList == NULL || 
		pDC == NULL )
		return;

	CDrawTechKLine* pPKline = GetPKLine();
	if(pPKline == NULL)
		return;

	int nDefLeftWidth  = -1;
	int nDefRightWidth = -1;
	int nLeftWidth;
	int nRightWidth;
	CHSMinuteFigure* pDrawObj = NULL;  // 绘制对象
	int i;
	for( i = 0; i < m_ayDrawList->GetSize();i++ )
	{
		pDrawObj = (CHSMinuteFigure*)m_ayDrawList->GetAt(i);
		if( pDrawObj != NULL && 
			//pDrawObj->IsKindOf(RUNTIME_CLASS(CHSMinuteFigure)) &&
			pDrawObj->IsShow() )
		{
			nLeftWidth  = pDrawObj->GetLinear()->GetYLeftMaxWidth(pDC);
			if( pDrawObj->GetLinear()->IsAxesStyle(AXIS_FENSHIPRICE) )
			{
				nRightWidth = pDrawObj->GetLinear()->GetYRightMaxWidth(pDC);
				if( nRightWidth > nDefRightWidth )
				{
					nDefRightWidth = nRightWidth;
				}
			}
			if( nLeftWidth > nDefLeftWidth )
			{
				nDefLeftWidth = nLeftWidth;
			}
		}
	}

	if( ( m_nPreLeftMaxMargin == nDefLeftWidth && m_nPreRightMaxMargin == nDefRightWidth) ||
		(nDefLeftWidth < 0 || (pPKline->IsFenshiDrawType() && nDefRightWidth < 0) ) )
	{
		if( !IsAxesStyle(AXIS_FORCEDMARGIN) )
			return;
	}

	CMarginParam* pMargin;
	CRect  rect;rect.SetRectEmpty();
	for( i = 0; i < m_ayDrawList->GetSize();i++ )
	{
		pDrawObj = (CHSMinuteFigure*)m_ayDrawList->GetAt(i);
		if( pDrawObj != NULL && 
			//pDrawObj->IsKindOf(RUNTIME_CLASS(CHSMinuteFigure)) &&
			pDrawObj->IsShow() )
		{			
			pMargin = pDrawObj->GetLinear()->GetMargin(CMarginParam::left);
			pMargin->SetSame(nDefLeftWidth);  // left

			pMargin = pDrawObj->GetLinear()->GetMargin(CMarginParam::right);
			pMargin->SetSame(nDefRightWidth); // right

			pDrawObj->ReSize(rect,NULL,2);
		}
	}
	m_nPreLeftMaxMargin  = nDefLeftWidth;
	m_nPreRightMaxMargin = nDefRightWidth;
	if( IsAxesStyle(AXIS_FORCEDMARGIN))
	{
		RemoveAxesStyle(AXIS_FORCEDMARGIN);
	}*/
}

CString CGraphics::ConvertIntToStr(int nValue)
{
	switch(nValue)
	{
	case 10:
		return "x10";//"x拾";//╳
	case 100:
		return HS_LANGUAGE("x100");//"x佰";
	case 1000:
		return HS_LANGUAGE("x1000");//"x仟";
	case 10000:
		return HS_LANGUAGE("x1万");
	case 100000:
		return HS_LANGUAGE("x10万");
	case 1000000:
		return HS_LANGUAGE("x百万");
	case 10000000:
		return HS_LANGUAGE("x仟万");
	case 100000000:
		return HS_LANGUAGE("x亿");
	case 1000000000:
		return HS_LANGUAGE("x10亿"); // Ⅹ
	}

	return HS_LANGUAGE("越界");
}
/********************************************************************************
* 函数功能 : 判断点是否在区域中
* 函数参数 : DWORD linetype - 线风格例如PKLINESTICK
*			  CPoint& point- 点
*			  CRect& rect-区域
*			  short nCurve,void* pMask - 未使用
*  返回值  : 判断成功返回FINDED, 否则CONTINUE
* 调用依赖 : 调用了IsOnLine
* 作    者 :俞礼石
* 完成日期 :2003-4-23
* 修改记录 : 
* 作	者 : 熊钟彬
* 日	期 : 2003-07-29
* 修改内容 : 加入系统指示鼠标点击选中的判断,并做选中标识
*******************************************************************************/
long CGraphics::IsSelected(DWORD linetype,CPoint& point,CRect& rect,short nCurve,void* pMask,
						   CLineProperty* pData /*= NULL*/)
{
/*	if( (linetype & CROSSDOT) || (linetype & CIRCLEDOT) || (linetype & POINTDOT) )
	{				
		return BREAK;
	}

	rect.NormalizeRect(); 

	// 用户定义图标或交易系统、选股指示、五彩k线
	if( linetype & SELECTMARK )
	{
		CDrawTechKLine* pKLine = GetPKLine();
		if( pKLine == NULL )
			return CONTINUE;

		CTechAnalysisObj* pTechObj = pKLine->GetTechAnaObj();	//技术分析对象
		if( pTechObj == NULL )
			return CONTINUE;

		//pTechObj->RemoveStyle(KLINE_SIGN_CONDITION | KLINE_SIGN_EXCHANGE | KLINE_SIGN_MOREKLINE);

		//
		int nMask = 0;
		if( pTechObj->IsStyle(CExpression::Condition) )
		{
			rect.top    = rect.bottom;
			rect.bottom = rect.bottom + 16;

			nMask = KLINE_SIGN_CONDITION;
		}
		else if( pTechObj->IsStyle(CExpression::Exchange) )
		{
			int nCount = pTechObj->GetSize();

			if( nCurve == (nCount - 1) )      // 多头卖出 EXITLONG
			{
				rect.bottom = rect.top;
				rect.top    = rect.top - 16;
			}
			else if( nCurve == (nCount - 2) ) // 多头买入 
			{
				rect.top    = rect.bottom;
				rect.bottom = rect.bottom + 16;
			}

			nMask = KLINE_SIGN_EXCHANGE;
		}
		else if( pTechObj->IsStyle(CExpression::MoreKLine) )
		{
			rect.top    = rect.bottom;
			rect.bottom = rect.bottom + 16;

			nMask = KLINE_SIGN_MOREKLINE;
		}
		else
		{
			return CONTINUE;
		}

		if( //(point.x >= (rect.left - 8) && point.x <= (rect.left + 8)) && 
			(point.x >= rect.left && point.x <= rect.right) &&
			(point.y >= rect.top  && point.y <= rect.bottom) )
		{			
			if( !(linetype & CGraphics::NOTSELMARK) )
			{
				pTechObj->AddStyle(nMask);
			}
			return FINDED;
		}

		return CONTINUE;
	}

	//rect.NormalizeRect();
	if( (linetype & VOLSTICK)  || 
		//(linetype & COLORSTICK)|| 
		//(linetype & LINESTICK) || 
		(linetype & DRAW3D) || 
		(linetype & PKLINESTICK))
	{
		if( rect.PtInRect(point) )
		{
			return FINDED;
		}
	}
	else if( IsOnLine(rect.TopLeft(), rect.BottomRight(),point) )
	{
		return FINDED;
	}

	return CONTINUE;*/
	return 0;
}

BOOL CGraphics::IsOnDot(CPoint& StartPt, CPoint& EndPt, CPoint& point)
{
	if( point.x < StartPt.x + 2 && 
		point.x > StartPt.x - 2 && 
		point.y < StartPt.y + 2 && 
		point.y > StartPt.y - 2 )
	{
		return TRUE;
	}

	if( point.x < EndPt.x + 2 && 
		point.x > EndPt.x - 2 && 
		point.y < EndPt.y + 2 && 
		point.y > EndPt.y - 2 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGraphics::IsOnLine(CPoint& StartPt, CPoint& EndPt, CPoint& point)
{
	if( IsOnDot(StartPt, EndPt, point) )
	{
		return TRUE;
	}

	int x1,y1,x2,y2,x,y;

	if(	StartPt.x <= EndPt.x )
	{
		x1 = StartPt.x;
		y1 = StartPt.y;
		x2 = EndPt.x;
		y2 = EndPt.y;
		x  = point.x;
		y  = point.y;
	}
	else
	{
		x2 = StartPt.x;
		y2 = StartPt.y;
		x1 = EndPt.x;
		y1 = EndPt.y;
		x  = point.x;
		y  = point.y;
	}

	if (x < x1) return FALSE;
	if (x > x2) return FALSE;

	if ( x1 == x2 )
	{
		if ( y1 <= y && y <= y2 )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	float nSlope = (float)(y2 - y1) / (float)(x2 - x1);
	float yLine  = (float)y1 + nSlope*(float)(x - x1);

	double q = (yLine - y > 0) ? (yLine - y):(y - yLine);
	if ( q < 8 ) // -3~3
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CGraphics::GetTipText(MouseTipTextInfo* pMouseTipTextInfo,int nPosition,CPoint& point)
{
	if( m_PY == 0 || m_YRefParam.nDiv == 0 || !pMouseTipTextInfo ||
		point.y > m_GB)
	{
		pMouseTipTextInfo->m_strY.Empty();
		return 0;
	}

	CString str;

	double dMin = m_YRefParam.min;
	double dMax = m_YRefParam.max;
	double dMulti = 0;
	CDrawBase* pPKline = GetPKLine();
	if( pPKline != NULL && !pPKline->IsDrawType(DrawType_FenShi) ) // 计算坐标文本是否超过显示边界
	{
		int    nLeftWidth = GetYLeftMaxWidth(m_pDC,TRUE);
		float nCharWidths = LOWORD(nLeftWidth);
		short  nCharCount = HIWORD(nLeftWidth);
		if(nCharCount > 0)
		{
			nLeftWidth  = (nCharWidths - (IsAxesStyle(AXIS_LEFT) ? m_rcMargin.left : m_rcMargin.right));
			nCharWidths = nCharWidths / nCharCount;
			if( nCharWidths > 0 && nLeftWidth > 0 )
			{
				dMulti = (nLeftWidth / nCharWidths);
				if(dMulti > (int)dMulti)
				{
					dMulti = (int)dMulti + 1;
				}
				else
				{
					dMulti = (int)dMulti;
				}
				dMulti = pow((double)10,(double)dMulti);
				if(dMulti != 0)
				{
					dMax /= dMulti;
					dMin /= dMulti;
				}
			}
		}
	}

	float value;
	if( m_nAxesType	== YLOG )
	{
		float valuePer;
		if( m_PY != 0 )
			valuePer = (float)pow((double)10,(log10(dMax)-log10(dMin))/m_PY);
		else
			valuePer = 1;

		value = dMax / pow(valuePer,(point.y - m_GT));
	}
	else
	{
		if(m_PY != 0)
			value = (float)(dMax - ( point.y - m_GT ) * (dMax - dMin) / m_PY);
		else
			value = 0;
	}

	if(value >= dMin && value <= dMax)
	{
		if(m_YRefParam.nDiv != 0)
			value /= m_YRefParam.nDiv;
		Format(m_YRefParam.nDecimal, str, value);	
		pMouseTipTextInfo->m_strY = str;

		return TRUE;
	}
	else
	{
		pMouseTipTextInfo->m_strY.Empty();
	}

	return FALSE;
}

int CGraphics::IsValue(long* lValue,...)
{
	va_list argList;
	va_start(argList, lValue);
	long* pCurValue = lValue;

	while (pCurValue)
	{
		if( (*pCurValue == hx_DefMinValues) || 
			(*pCurValue == hx_DefMaxValues) )
			return FALSE;

		pCurValue = (long*)va_arg( argList, long* );
	}

	va_end( argList );

	return TRUE;
}

int CGraphics::IsValue(double* lValue,...)
{
	va_list argList;
	va_start(argList, lValue);
	double* pCurValue = lValue;

	while (pCurValue)
	{
		if( (*pCurValue == (double)hx_DefMinValues) || 
			(*pCurValue == (double)hx_DefMaxValues) )
			return FALSE;

		pCurValue = (double*)va_arg( argList, double* );
	}

	va_end( argList );

	return TRUE;
}

int CGraphics::IsValue(float* lValue,...)
{
	va_list argList;
	va_start(argList, lValue);
	float* pCurValue = lValue;

	while (pCurValue)
	{
		if( ((long)*pCurValue == hx_DefMinValues) || 
			((long)*pCurValue == hx_DefMaxValues) )
			return FALSE;

		pCurValue = (float*)va_arg( argList, float* );
	}

	va_end( argList );

	return TRUE;
}

int	CGraphics::IsAxesType(int nAxesType)
{
	return (m_nAxesType == nAxesType);
}

void CGraphics::SetAxesType(int nAxesType)
{
	m_nAxesType = nAxesType;
}

int CGraphics::ValueToVertPos(double lValue)
{
	int nPos = 0;
	if( m_nAxesType	== YLOG )
	{		
		if( (m_YRefParam.max - m_YRefParam.min) != 0 )
		{
			nPos = (int)((float)m_PY * 
				( log10(lValue) - log10(m_YRefParam.min)) / (log10(m_YRefParam.max) - log10(m_YRefParam.min)) );
		}
		else if(m_YRefParam.min != hx_DefMinValues)
		{
			nPos = (int)( (float)m_PY * ( log10(lValue) - log10(m_YRefParam.min) ) + 0.5 );
		}
		else
		{
			nPos = (int)( (float)m_PY * log10(lValue) + 0.5 );
		}
	}
	else if(m_nAxesType == YLN)
	{
		if( (m_YRefParam.max - m_YRefParam.min) != 0 )
		{
			nPos = (int)((float)m_PY * 
				( log(lValue) - log(m_YRefParam.min)) / (log(m_YRefParam.max) - log(m_YRefParam.min)) );
		}
		else if(m_YRefParam.min != hx_DefMinValues)
		{
			nPos = (int)( (float)m_PY * ( log(lValue) - log(m_YRefParam.min) ) + 0.5 );
		}
		else
		{
			nPos = (int)( (float)m_PY * log(lValue) + 0.5 );
		}
	}
	else
	{
		if( (m_YRefParam.max - m_YRefParam.min) != 0 )
		{
			nPos = (int)((float)m_PY * 
				( (lValue) - (m_YRefParam.min)) / ((m_YRefParam.max) - (m_YRefParam.min)) );
		}
		else if(m_YRefParam.min != hx_DefMinValues)
		{
			nPos = (int)( (float)m_PY * ( lValue - m_YRefParam.min ) + 0.5 );
		}
		else
		{
			nPos = (int)( (float)m_PY * lValue + 0.5 );
		}
	}
	return nPos;
}

int CGraphics::DrawAdvance(DWORD& linetype,int& nCurve,ColorProperty& crSingleLineColor,
						   POINT& pt1,POINT& pt2,
						   HSDouble& pCurData,HSDrawOtherData* pOtherData,int& nPos,
						   int nDistance) //　横向线与线的间距
{
/*	CBrush SingleBrush(crSingleLineColor.m_lColor);

	CBrush* pOldBrush = m_pDC->SelectObject(&SingleBrush);
	switch(pOtherData->m_bType1)
	{
	case YLS_DRAWICON: //   1
		{
			CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
			if( pImage == NULL || pImage->m_hImageList == NULL )
				return -3;
			if( nPos > 0 )
			{
				if( pt1.y - 16 < m_GT )
					return -30;
				if( pt1.y + 16 > m_GB )
					return -31;
				if( pt1.x - 16 < m_GL )
					return -32;
				if( pt1.x + 16 > m_GR )
					return -33;

				pImage->Draw(m_pDC,(*pOtherData).m_bType2 == 0?CExpression::Yls_Exp_DrawIcon0:
				(*pOtherData).m_bType2 == 1?CExpression::Yls_Exp_DrawIcon0:
				(*pOtherData).m_bType2 == 2?CExpression::Yls_Exp_DrawIcon2:
				(*pOtherData).m_bType2 == 3?CExpression::Yls_Exp_DrawIcon3:
				(*pOtherData).m_bType2 == 4?CExpression::Yls_Exp_DrawIcon4:
				(*pOtherData).m_bType2 == 5?CExpression::Yls_Exp_DrawIcon5:
				(*pOtherData).m_bType2 == 6?CExpression::Yls_Exp_DrawIcon6:
				(*pOtherData).m_bType2 == 7?CExpression::Yls_Exp_DrawIcon7:
				(*pOtherData).m_bType2 == 8?CExpression::Yls_Exp_DrawIcon8:
				(*pOtherData).m_bType2 == 9?CExpression::Yls_Exp_DrawIcon9:
				(*pOtherData).m_bType2 == 10?CExpression::Yls_Exp_SelectStock:
				(*pOtherData).m_bType2 == 11?CExpression::Yls_Exp_DrawIcon11:
				(*pOtherData).m_bType2 == 12?CExpression::Yls_Exp_Active:
				(*pOtherData).m_bType2 == 13?CExpression::Yls_Exp_DrawIcon13:
				(*pOtherData).m_bType2 == 14?CExpression::Yls_Exp_DrawIcon14:
				CExpression::Yls_Exp_DrawIcon2,
					pt1,ILD_TRANSPARENT);
			}
		}
		break;
	case YLS_DRAWLINE: //   2
		{
			m_pDC->MoveTo(pt1.x,pt1.y);
			m_pDC->LineTo(pt2.x,pt2.y);
			DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
			DrawMarker(pt2.x, pt2.y, FSQUARE, 2);

			CRect rect = GetChartRect();

			double dYSlope = double(pt2.y - pt1.y) / double(pt2.x - pt1.x);
			double dYJieju = double(pt1.y + pt2.y)/2 - dYSlope * double(pt1.x + pt2.x)/2;

			switch(pOtherData->m_bType4)
			{
			case 1:
				{
					pt2 = pt1;
					// left
					pt1.x = rect.left;
					pt1.y = int(dYSlope * pt1.x + dYJieju);
					if( pt1.y > m_GB )
					{
						pt1.x = (m_GB - dYJieju)/ dYSlope;
						pt1.y = int(dYSlope * pt1.x + dYJieju);
					}
					if( pt1.y < m_GT )
					{
						pt1.x = (m_GT - dYJieju)/ dYSlope;
						pt1.y = int(dYSlope * pt1.x + dYJieju);
					}

				}
				break;
			case 2:
				{
					pt1 = pt2;
					// right
					pt2.x = rect.right;
					pt2.y = int(dYSlope * pt2.x + dYJieju);
					if( pt2.y > m_GB )
					{
						pt2.x = (m_GB - dYJieju)/ dYSlope;
						pt2.y = int(dYSlope * pt2.x + dYJieju);
					}
					if( pt2.y < m_GT )
					{
						pt2.x = (m_GT - dYJieju)/ dYSlope;
						pt2.y = int(dYSlope * pt2.x + dYJieju);
					}
				}
				break;
			case 3:
				{
					// left
					pt1.x = rect.left;
					pt1.y = int(dYSlope * pt1.x + dYJieju);
					if( pt1.y > m_GB )
					{
						pt1.x = (m_GB - dYJieju)/ dYSlope;
						pt1.y = int(dYSlope * pt1.x + dYJieju);
					}
					if( pt1.y < m_GT )
					{
						pt1.x = (m_GT - dYJieju)/ dYSlope;
						pt1.y = int(dYSlope * pt1.x + dYJieju);
					}
					// right
					pt2.x = rect.right;
					pt2.y = int(dYSlope * pt2.x + dYJieju);
					if( pt2.y > m_GB )
					{
						pt2.x = (m_GB - dYJieju)/ dYSlope;
						pt2.y = int(dYSlope * pt2.x + dYJieju);
					}
					if( pt2.y < m_GT )
					{
						pt2.x = (m_GT - dYJieju)/ dYSlope;
						pt2.y = int(dYSlope * pt2.x + dYJieju);
					}
				}
				break;
			}
			m_pDC->MoveTo(pt1.x,pt1.y);
			m_pDC->LineTo(pt2.x,pt2.y);
		}
		break;
	case YLS_DRAWTEXT: //   3
		{
			if( pOtherData->m_strText )
			{
				CFont* hOldFont = m_pDC->SelectObject(GetFont(ftDrawMaxMin));

				SetStringAlign(DT_LEFT, DT_VCENTER);
				CSize size;

				if(g_hsGraphicsData.IsStyle(GL_HighLightText))
					m_pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
				else
					m_pDC->SetTextColor(crSingleLineColor.m_lColor);

				CPen hPen;
				hPen.CreatePen(g_hsGraphicsData.clPageBK.m_nPenStyle,
					g_hsGraphicsData.clPageBK.m_nWidth,g_hsGraphicsData.clPageBK.GetInvertColor());
				CPen* hOldPen = m_pDC->SelectObject(&hPen);	

				int nPos = pt2.y;
				int x = pt2.x;	
				CRect charRect = GetChartRect();	
				size = m_pDC->GetTextExtent(*pOtherData->m_strText, lstrlen(*pOtherData->m_strText));
				if(nPos < (charRect.top + size.cy))
				{
					nPos = charRect.top + size.cy;
				}
				if( nPos > (charRect.bottom - size.cy) )
				{
					nPos = charRect.bottom - size.cy;
				}
				if(x > (charRect.right - size.cx))
				{
					x = charRect.right - size.cx - 5;
				}

				if(g_hsGraphicsData.IsStyle(GL_HighLightText))
				{
					charRect.left   = x+5;
					charRect.right  = charRect.left + size.cx;
					charRect.top    = nPos-size.cy/2;
					charRect.bottom = charRect.top + size.cy; 
					m_pDC->FillSolidRect(charRect, g_hsGraphicsData.crHightBkColor.m_lColor);
				}

				DrawMarker(pt2.x, pt2.y, FSQUARE, 2);

				PrintString(x+5,nPos, 0, *pOtherData->m_strText);

				m_pDC->SelectObject(hOldPen);
				hPen.DeleteObject();

				m_pDC->SelectObject(hOldFont);

			}
		}
		break;
	case YLS_POLYLINE: //   4
		{
			if( pOtherData->m_bType2 )
			{
				m_pDC->MoveTo(pt1.x,pt1.y);
			}
			else
			{
				m_pDC->LineTo(pt1.x,pt1.y);
			}
		}
		break;
	case YLS_STICKLINE://
		{
			CRect rect(pt1.x, pt1.y,pt1.x, pt2.y);
			if ( pOtherData->m_bType2 == (BYTE)-1 )
			{
				rect.left  -= nDistance/2;
				rect.right += nDistance/2;
			}
			else
			{
				rect.left  -= pOtherData->m_bType2/2;
				rect.right += pOtherData->m_bType2/2;
			}

			if( pOtherData->m_bType3 )
			{
				m_pDC->MoveTo(rect.left, rect.top);
				m_pDC->LineTo(rect.right, rect.top); 
				m_pDC->LineTo(rect.right, rect.bottom); 
				m_pDC->LineTo(rect.left, rect.bottom); 
				m_pDC->LineTo(rect.left, rect.top);
			}
			else
			{
				m_pDC->Rectangle(rect);
			}

			//m_pDC->MoveTo(pt1.x,pt1.y);
			//m_pDC->LineTo(pt2.x,pt2.y);

			//DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
			//DrawMarker(pt2.x, pt2.y, FSQUARE, 2);
		}
		break;
	case YLS_HLTHBQ:
		{
			if( pOtherData->m_bType2 == 1 )
			{
				CPen hPen;
				hPen.CreatePen(g_hsGraphicsData.clUp.m_nPenStyle,
					g_hsGraphicsData.clUp.m_nWidth,g_hsGraphicsData.clUp.m_lColor);
				CPen* hOldPen = m_pDC->SelectObject(&hPen);

				m_pDC->MoveTo(pt1.x,pt1.y);
				m_pDC->LineTo(pt2.x,pt2.y);

				DrawMarker(pt1.x,pt1.y, CIRCLE,nDistance);

				m_pDC->SelectObject(hOldPen);
			}
			else
			{
				CPen hPen;
				hPen.CreatePen(g_hsGraphicsData.clEqual.m_nPenStyle,
					g_hsGraphicsData.clEqual.m_nWidth,g_hsGraphicsData.clEqual.m_lColor);
				CPen* hOldPen = m_pDC->SelectObject(&hPen);

				m_pDC->MoveTo(pt1.x,pt1.y);
				m_pDC->LineTo(pt2.x,pt2.y);

				DrawMarker(pt1.x,pt1.y, CIRCLE,nDistance);

				m_pDC->SelectObject(hOldPen);
			}
		}
		break;
	case YLS_BUYSELL:
		{
			if( pOtherData->m_bType2 == 1 )
			{
				CPen hPen;
				hPen.CreatePen(g_hsGraphicsData.clUp.m_nPenStyle,
					2,RGB(219,219,0));
				CPen* hOldPen = m_pDC->SelectObject(&hPen);

				CPen hPen1;
				hPen1.CreatePen(PS_DASHDOT,
					0,RGB(219,219,0));

				//m_pDC->MoveTo(pt1.x,pt1.y);

				//DrawMarker(pt1.x,pt1.y, CIRCLE,nDistance);

				if( pOtherData->m_bType4 == 1 )
				{
					m_pDC->SetTextColor(g_hsGraphicsData.clUp.m_lColor);
					DrawMarker(pt1.x - (float)nDistance/2*3,pt1.y,DRAWSTRING,nDistance,"B");					
					m_pDC->MoveTo(pt1.x - (float)nDistance/2*3,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);

					//CRect charRect = GetChartRect();
					m_pDC->SelectObject(&hPen1);
					DrawMarker(pt1.x - (float)nDistance/2*3,pt1.y - 5,FTRIANGLE,4);//,nDistance);
					//m_pDC->MoveTo(pt1.x,charRect.top);
					//m_pDC->LineTo(pt1.x,charRect.bottom);
				}
				else if( pOtherData->m_bType4 == 2 )
				{
					m_pDC->SetTextColor(g_hsGraphicsData.clDown.m_lColor);
					DrawMarker(pt1.x,pt1.y,DRAWSTRING,nDistance,"S");
					//m_pDC->LineTo(pt2.x - nDistance,pt2.y);

					//CRect charRect = GetChartRect();
					m_pDC->SelectObject(&hPen1);
					DrawMarker(pt1.x,pt1.y - 5,FTRIANGLE,4);//,nDistance);
					//m_pDC->MoveTo(pt1.x,charRect.top);
					//m_pDC->LineTo(pt1.x,charRect.bottom);
				}
				else
				{
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);
				}

				m_pDC->SelectObject(hOldPen);
			}
			else
			{
				CPen hPen;
				hPen.CreatePen(g_hsGraphicsData.clEqual.m_nPenStyle,
					g_hsGraphicsData.clEqual.m_nWidth,g_hsGraphicsData.clEqual.m_lColor);
				CPen* hOldPen = m_pDC->SelectObject(&hPen);

				//m_pDC->MoveTo(pt1.x,pt1.y);
				//m_pDC->LineTo(pt2.x,pt2.y);

				//DrawMarker(pt1.x,pt1.y, CIRCLE,nDistance);

				if( pOtherData->m_bType3 == 1 )
				{
					//DrawMarker(pt1.x,pt1.y, CROSS,nDistance);
				}
				else if( pOtherData->m_bType3 == 2 )
				{
					//DrawMarker(pt1.x,pt1.y, STAR,nDistance);
				}

				m_pDC->SelectObject(hOldPen);
			}
		}
		break;
	case YLS_DRAWSL:
		{
			double dYSlope = (*pOtherData).m_dValue;

			switch(pOtherData->m_bType4)
			{
			case 1:
				{
					// left
					if( dYSlope != 0 )
					{
						pt2.x = pt1.x - (pt2.x - pt1.x);
						pt2.y = (int)(((pt2.x - pt1.x) * dYSlope + (double)pt1.y));					
						if( pt2.y > m_GB )
							pt2.y = m_GB;
						if( pt2.y < m_GT )
							pt2.y = m_GT;
					}					

					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);

					DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
					DrawMarker(pt2.x, pt2.y, FSQUARE, 2);
				}
				break;			
			case 2:
				{
					POINT pt1_ = pt1;
					POINT pt2_ = pt2;

					// left
					if( dYSlope != 0 )
					{
						pt2.x = pt1.x - (pt2.x - pt1.x);
						pt2.y = (int)(((pt2.x - pt1.x) * dYSlope + (double)pt1.y));		
						if( pt2.y > m_GB )
							pt2.y = m_GB;
						if( pt2.y < m_GT )
							pt2.y = m_GT;
					}
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);

					DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
					DrawMarker(pt2.x, pt2.y, FSQUARE, 2);

					pt1 = pt1_;
					pt2 = pt2_;

					// right
					if( dYSlope != 0 ) 
					{
						pt2.y = (int)(((pt2.x - pt1.x) * dYSlope + (double)pt1.y));
						if( pt2.y > m_GB )
							pt2.y = m_GB;
						if( pt2.y < m_GT )
							pt2.y = m_GT;
					}
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);

					DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
					DrawMarker(pt2.x, pt2.y, FSQUARE, 2);

				}
				break;
			default:
				{				
					// right
					if( dYSlope != 0 ) 
					{
						pt2.y = (int)(((pt2.x - pt1.x) * dYSlope + (double)pt1.y));
						if( pt2.y > m_GB )
							pt2.y = m_GB;
						if( pt2.y < m_GT )
							pt2.y = m_GT;
					}
					m_pDC->MoveTo(pt1.x,pt1.y);
					m_pDC->LineTo(pt2.x,pt2.y);

					DrawMarker(pt1.x, pt1.y, FSQUARE, 2);
					DrawMarker(pt2.x, pt2.y, FSQUARE, 2);					
				}
				break;
			}			
		}
		break;
	}

	m_pDC->SelectObject(pOldBrush);
*/
	return -1;
}

int CGraphics::PaintMoveCost(CGraphics::MoveCostConfig& moveCost,
							 CGraphics::MoveCostParamArray& ayParam, 
							 CRect& clipBox,
							 AnalisysObjData* pAnalisysData,int nCurPos)
{
/*	if ( !moveCost.IsValid() )
		return 0;

	short nWidth = m_PX,nStart;
	if ( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_INDRAWK_COST) )
	{
		nWidth /= 3;
		nStart = m_GR;
	}
	else
	{
		nWidth = 0 - m_PX;
		nStart = m_GL;
	}

	CPoint pt;
	char cPos;
	int  i;

	int  nSize = ayParam.ayParam.GetSize();

	CPen hPen1,hPen2,hPen3,hPen4,hPen5,hPen6,hPen7,hPen8,hPen9,hPen10,hPen11,hPenWhite;
	hPen1.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(0)->m_lColor);
	hPen2.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(1)->m_lColor);
	hPen3.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(2)->m_lColor);
	hPen4.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(3)->m_lColor);
	hPen5.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(4)->m_lColor);
	hPen6.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(5)->m_lColor);
	hPen7.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(6)->m_lColor);
	hPen8.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(7)->m_lColor);
	hPen9.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(8)->m_lColor);
	hPen10.CreatePen(ayParam.lColor.m_nPenStyle,ayParam.lColor.m_nWidth,ayParam.ayParam.GetAt(9)->m_lColor);
	hPen11.CreatePen(ayParam.lColor.m_nPenStyle,1,ayParam.ayParam.GetAt(10)->m_lColor);
	hPenWhite.CreatePen( PS_SOLID,1,g_hsGraphicsData.clPageBK.GetInvertColor() );

	CPen* hOldPen = m_pDC->GetCurrentPen();

	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST) | g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
	{
		for( i = 0 ; i < m_PY; i++ )
		{
			for(cPos = 0; cPos <nSize; cPos++ )
			{
				if( cPos == 0 )
				{
					if( i < g_CostDetail.fInterestPan )
					{
						m_pDC->SelectObject(&hPen11);
						if( i%3 == 0 )
						{
							pt.x = nStart - moveCost.dCostNowData[0][i] * nWidth / moveCost.nMaxData;
							pt.y = m_GT + i;

							m_pDC->MoveTo(pt.x,pt.y);
							m_pDC->LineTo(nStart,pt.y);
						}
					}
					else
					{
						m_pDC->SelectObject(&hPenWhite);
						if( i%3 == 0 )
						{
							pt.x = nStart - moveCost.dCostNowData[0][i] * nWidth / moveCost.nMaxData;
							pt.y = m_GT + i;

							m_pDC->MoveTo(pt.x,pt.y);
							m_pDC->LineTo(nStart,pt.y);
						}
					}

					continue;
				}
				else if( cPos == 1 )
				{
					m_pDC->SelectObject(&hPen2);
				}
				else if( cPos == 2 )
				{
					m_pDC->SelectObject(&hPen3);
				}
				else if( cPos == 3 )
				{
					m_pDC->SelectObject(&hPen4);
				}
				else if( cPos == 4 )
				{
					m_pDC->SelectObject(&hPen5);
				}
				else if( cPos == 5 )
				{
					m_pDC->SelectObject(&hPen6);
				}
				else if( cPos == 6 )
				{
					m_pDC->SelectObject(&hPen7);
				}
				else if( cPos == 7 )
				{
					m_pDC->SelectObject(&hPen8);
				}
				else if( cPos == 8 )
				{
					m_pDC->SelectObject(&hPen9);
				}
				else if( cPos == 9 )
				{
					m_pDC->SelectObject(&hPen10);
				}
				else if( cPos == 10 )
				{
					m_pDC->SelectObject(&hPen1);			
				}
				pt.x = nStart - moveCost.dCostNowData[cPos][i] * nWidth / moveCost.nMaxData;
				pt.y = m_GT + i;

				m_pDC->MoveTo(pt.x,pt.y);
				m_pDC->LineTo(nStart,pt.y);
			}
		}
	}
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		for( i = 0 ; i < m_PY; i++ )
		{
			for(cPos = nSize - 1; cPos >= 0; cPos-- )
			{
				if( cPos == 0 )
				{
					m_pDC->SelectObject(&hPen1);
				}
				else if( cPos == 1 )
				{
					m_pDC->SelectObject(&hPen2);
				}
				else if( cPos == 2 )
				{
					m_pDC->SelectObject(&hPen3);
				}
				else if( cPos == 3 )
				{
					m_pDC->SelectObject(&hPen4);
				}
				else if( cPos == 4 )
				{
					m_pDC->SelectObject(&hPen5);
				}
				else if( cPos == 5 )
				{
					m_pDC->SelectObject(&hPen6);
				}
				else if( cPos == 6 )
				{
					m_pDC->SelectObject(&hPen7);
				}
				else if( cPos == 7 )
				{
					m_pDC->SelectObject(&hPen8);
				}
				else if( cPos == 8 )
				{
					m_pDC->SelectObject(&hPen9);
				}
				else if( cPos == 9 )
				{
					m_pDC->SelectObject(&hPen10);
				}
				else if( cPos == 10 )
				{
					m_pDC->SelectObject(&hPen11);
					if( i%3 == 0 )
					{
						pt.x = nStart - moveCost.dCostNowData[cPos][i] * nWidth / moveCost.nMaxData;
						pt.y = m_GT + i;

						m_pDC->MoveTo(pt.x,pt.y);
						m_pDC->LineTo(nStart,pt.y);
					}
					continue;
				}
				pt.x = nStart - moveCost.dCostNowData[cPos][i] * nWidth / moveCost.nMaxData;
				pt.y = m_GT + i;

				m_pDC->MoveTo(pt.x,pt.y);
				m_pDC->LineTo(nStart,pt.y);
			}
		}
	}
	CString str;
	//用于画白色的分割线
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
	{
		m_pDC->SelectObject(hOldPen);
		return 1;
	}

	CPoint Pts[1000];
	LPPOINT lpPoints = Pts;
	int nPointCount;
	m_pDC->SelectObject(&hPenWhite);
	for(cPos = nSize - 2; cPos >= 0; cPos-- )
	{
		nPointCount = 0;	
		for( i = 0 ; i < m_PY; i++ )
		{
			if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
			{
				pt.x = nStart - moveCost.dCostNowData[cPos][i] * nWidth / moveCost.nMaxData;
			}
			if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
			{
				pt.x = nStart - moveCost.dCostNowData[cPos + 1][i] * nWidth / moveCost.nMaxData;
			}

			pt.y = m_GT + i;
			if( pt.x != 0 )
			{
				Pts[nPointCount].x = pt.x;
				Pts[nPointCount].y = pt.y;
				nPointCount++;
			}
			else
			{	
				m_pDC->Polyline( lpPoints,nPointCount );
				nPointCount = 0;
			}
		}
		if( nPointCount == 0 )
			continue;

		m_pDC->Polyline( lpPoints,nPointCount );

	}

	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
	{
		m_pDC->SelectObject(hOldPen);
		return 1;
	}

	if( g_CostDetail.nData == 0 )
	{
		m_pDC->SelectObject(hOldPen);
		return 1;
	}

	CRect lRect = clipBox;

	//lRect.bottom = lRect.bottom - 20;
	CRect rect = lRect;
	CRect rect1,rect2;

	float fTotalHand = 0;
	float fCurrentHand = 0;

	CFont* hOldFont;
	CFont *pFont1 = g_hsGraphicsData.ftMoveCost.m_pFont;
	hOldFont = (CFont *)m_pDC->SelectObject(pFont1);
	m_pDC->SetTextColor( 0 );
	COLORREF clLine = g_hsGraphicsData.clPageBK.GetInvertColor();

	CSize size = m_pDC->GetTextExtent("A");
	size.cy += 2;

	rect.right -= 5;
	rect.left  = rect.right - size.cx * 16;//28;
	if( rect.left <= 0 )
		rect.left = 0;

	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
	{
		for(  i = g_HxhMoveData.m_nHYSCount - 1; i >= 0; i-- )
		{
			if( g_CostDetail.fHYS[g_HxhMoveData.m_nHYSCount - 1 - i] == 0 )
			{
				goto PaintMoveCost_end_;
			}

			rect.bottom = lRect.bottom - ( g_HxhMoveData.m_nHYSCount - i - 1) * size.cy  - size.cy * 4 - 3;
			rect.top    = rect.bottom - size.cy;

			rect1       = rect;
			//rect1.left  += 30;
			//rect2       = rect;
			//rect2.left  = rect.right - 20;

			COLORREF cl = ayParam.ayParam.GetAt(g_HxhMoveData.m_nHYSCount  - i)->m_lColor;
			//m_pDC->FillSolidRect( rect1, g_hsGraphicsData.clPageBK.m_lColor );
			//m_pDC->FillSolidRect( rect2, cl );
			m_pDC->FillSolidRect( rect1, cl );

			//rect1.top++;
			//m_pDC->Draw3dRect( rect1, cl, cl );
			m_pDC->Draw3dRect( rect1, clLine, clLine );
			str.Format( HS_LANGUAGE("%d天前%.2f%%"),g_HxhMoveData.m_ayHouYanShan[g_HxhMoveData.m_nHYSCount - 1 - i],g_CostDetail.fHYS[g_HxhMoveData.m_nHYSCount - 1 - i]);
			rect1.left += 2;
			m_pDC->DrawText( str, -1, &rect1,DT_LEFT );
			rect1.left -= 2;

			fCurrentHand = 0;
		}
	}
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		for(  i = 0; i < g_HxhMoveData.m_nHYDCount; i++ )
		{
			if( g_CostDetail.fHYD[i] == 0 )
			{
				goto PaintMoveCost_end_;
			}

			rect.bottom = lRect.bottom -  i * size.cy - size.cy * 4 - 3;
			rect.top    = rect.bottom  - size.cy;

			rect1       = rect;
			//rect1.left  += 30;
			//rect2       = rect;
			//rect2.left  = rect.right - 20;

			COLORREF cl = ayParam.ayParam.GetAt(i)->m_lColor;
			//m_pDC->FillSolidRect( rect1, g_hsGraphicsData.clPageBK.m_lColor );
			//m_pDC->FillSolidRect( rect2, cl );
			m_pDC->FillSolidRect( rect1, cl );

			//rect1.top++;
			//m_pDC->Draw3dRect( rect1, cl, cl );
			m_pDC->Draw3dRect( rect1, clLine, clLine );
			str.Format( HS_LANGUAGE("%d天内%.2f%%"),g_HxhMoveData.m_ayHuoYueDu[i], g_CostDetail.fHYD[i] );
			rect1.left += 2;
			m_pDC->DrawText( str, -1, &rect1,DT_LEFT );
			rect1.left -= 2;

			fCurrentHand = 0;
		}
	}
	//m_pDC->SelectObject(hOldFont);

PaintMoveCost_end_:
	//
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
	{
		//rect.bottom = lRect.bottom - ( g_HxhMoveData.m_nHYSCount - i - 1) * size.cy;
		rect.bottom = lRect.bottom;
		rect.top    = rect.bottom  - size.cy * 4;
	}
	else if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		//rect.bottom = lRect.bottom - ( i ) * size.cy;
		rect.bottom = lRect.bottom;
		rect.top    = rect.bottom  - size.cy * 4;
	}

	rect.left = lRect.left;
	str.Format(HS_LANGUAGE("成本分布日期：%d"),g_CostDetail.nData);
	m_pDC->SetTextColor( g_hsGraphicsData.clPageBK.GetInvertColor() );
	m_pDC->DrawText( str, -1, &rect,DT_LEFT );

	rect.OffsetRect(0,size.cy);

	str.Format(HS_LANGUAGE("获利盘：%.2f%%"),g_CostDetail.fInterestRatio * 100);
	m_pDC->SetTextColor( g_hsGraphicsData.clPageBK.GetInvertColor() );
	m_pDC->DrawText( str, -1, &rect,DT_LEFT );

	rect.OffsetRect(0,size.cy);

	str.Format(HS_LANGUAGE("套牢盘：%.2f%%"),(1 - g_CostDetail.fInterestRatio) * 100);
	m_pDC->SetTextColor( g_hsGraphicsData.clPageBK.GetInvertColor() );
	m_pDC->DrawText( str, -1, &rect,DT_LEFT );

	rect.OffsetRect(0,size.cy);

	double dCostValue = (double)g_hsGraphicsData.m_pHxDataSource.CAPITAL();
	if( dCostValue != hx_DefMaxValues )
	{
		if( dCostValue > 10000 )
		{
			str.Format(HS_LANGUAGE("流通盘：%.2f亿"),dCostValue/1000000);
		}
		else
		{
			str.Format(HS_LANGUAGE("流通盘：%.2f万"),dCostValue/100);
		}
	}
	else
	{
		str = HS_LANGUAGE("流通盘：-");
	}
	m_pDC->SetTextColor( g_hsGraphicsData.clPageBK.GetInvertColor() );
	m_pDC->DrawText( str, -1, &rect,DT_LEFT );

	m_pDC->SetTextColor( g_hsGraphicsData.clPageBK.GetColor() );

	//
	//rect.left   = clipBox.left;
	//rect.right  = clipBox.right;
	//rect.bottom = clipBox.bottom;
	//rect.top    = rect.bottom - 15;

	//if( g_lCurrentTime != 0 )
	//str.Format("成本分布日期：%d",g_lCurrentTime );

	m_pDC->SelectObject(hOldPen);
	m_pDC->SelectObject(hOldFont);
*/
	return 1;
}

//火焰山算法
int CGraphics::CalcMoveCost( AnalisysObjData* pAnalisysData,
							CGraphics::MoveCostConfig& moveCost,
							int nStart, int nEnd, int nCurPos )
{
/*	moveCost.cValid = 0;

	CDrawTechKLine* pPKline = GetPKLine();

	if ( pAnalisysData == NULL		     ||
		pAnalisysData->m_pData == NULL  ||
		pAnalisysData->m_nValueNum <= 0 ||
		pPKline == NULL )
		return 0;

	int i, j, k = -1;
	int v, day;
	int a, b;
	int ready, over;

	int     AddData;
	double  dCostValue;

	StockDay* pData = pAnalisysData->m_pData;

	dCostValue = (double)g_hsGraphicsData.m_pHxDataSource.GetFinaceDataByID(COLUMN_CAPITALIZATION_TOTAL) * 10000;  //(double)10000 / __max(10, moveCost.nHand) * moveCost.nMoveCostMul * moveCost.dLuiTongPan;
	v = 0;
	a = nCurPos;
	if ( dCostValue >= 0 )
	{
		for( i = a; i > 0; i-- )
		{
			if ( i < 0 || i >= pAnalisysData->m_nValueNum )
				break;

			v = v + pData[i].m_lTotal;
			if (v >= dCostValue )
				break;
		}
		day = a - i;
	}
	else
	{
		if( moveCost.nCostDay > 0 )
		{
			day = moveCost.nCostDay;
		}
		else
		{
			return 0;
		}
	}

	b = a - day;
	moveCost.nMaxData = 1;

	if( b < 0 ) b = 0;
	moveCost.EmptyCost();

	long lMaxValue,lMinValue;
	if ( a >= 0 && a < pAnalisysData->m_nValueNum )
	{
		StockDay* pDayData = pData;
		int nMaxPos,nMinPos;
		pPKline->GetMaxMinValue(hx_HIGH_DATA,pDayData, 0, a,
			nMaxPos,nMinPos,lMaxValue,lMinValue);
		pPKline->GetMaxMinValue(hx_LOW_DATA,pDayData, 0, a,
			nMaxPos,nMinPos,lMaxValue,lMinValue);

		if( lMaxValue == hx_DefMaxValues )
		{
			lMaxValue = pData->m_lMaxPrice;
		}
		if( lMinValue == hx_DefMinValues )
		{
			lMinValue = pData->m_lMinPrice;
		}
	}

	for( i = a; i >= b; i-- )
	{
		if ( i < 0 || i >= pAnalisysData->m_nValueNum )
			break;

		ready = m_PY * (m_YRefParam.max - pData[i].m_lMaxPrice) / __max(m_YRefParam.max - m_YRefParam.min, 1);
		over  = m_PY * (m_YRefParam.max - pData[i].m_lMinPrice) / __max(m_YRefParam.max - m_YRefParam.min, 1);
		//在此添加判断三角，还是平行算法从而解决两个算法的区别
		if( g_HxhMoveData.m_nCalcType == 1 )
		{
			AddData = pData[i].m_lTotal;
		}
		if( g_HxhMoveData.m_nCalcType == 2 )
		{
			AddData = pData[i].m_lTotal / __max((over - ready + 1), 1);
		}
		if( (ready > m_PY) || (over < 0) )
			continue;

		if( ready < 0 )
			ready = 0;
		if( over > m_PY )
			over = m_PY;

		//计算存储到二维数组的位置,天数小的放在数组的前面
		if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
		{
			if( g_HxhMoveData.m_nHYSCount == 0 )	//空数组，返回
			{
				return 0;
			}
			for( int s = 0; s < g_HxhMoveData.m_nHYSCount - 1; s++ )
			{
				if( i >= a - g_HxhMoveData.m_ayHouYanShan[s + 1] && i <= a - g_HxhMoveData.m_ayHouYanShan[s] )
				{
					k = s + 1;
					break;
				}
				if( i <= a && i > a - g_HxhMoveData.m_ayHouYanShan[0] )
				{
					k = 0;
					break;
				}
				if( i >= b && i < a - g_HxhMoveData.m_ayHouYanShan[g_HxhMoveData.m_nHYSCount - 1] )
				{
					k = g_HxhMoveData.m_nHYSCount;
					break;
				}
			}
		}
		if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
		{
			k = 4 - (a - i) / (day / 5 + 1);
		}
		if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
		{
			if( g_HxhMoveData.m_nHYDCount == 0 )	//空数组，返回
			{
				return 0;
			}
			for( int s = 0; s < g_HxhMoveData.m_nHYDCount - 1; s++ )
			{
				if( i >= a - g_HxhMoveData.m_ayHuoYueDu[s + 1] && i <= a - g_HxhMoveData.m_ayHuoYueDu[s] )
				{
					k = s + 1;
					break;
				}
				if( i <= a && i > a - g_HxhMoveData.m_ayHuoYueDu[0] )
				{
					k = 0;
					break;
				}
				if( i >= b && i < a - g_HxhMoveData.m_ayHuoYueDu[g_HxhMoveData.m_nHYDCount - 1] )
				{
					k = g_HxhMoveData.m_nHYDCount;
					break;
				}
			}
		}
		//	k = 4 - (a - i) / (day / 5 + 1);//11 是最后的一组
		if( k < 0 || k > MOVECOSTCOUNT )	//if k > 10,invalid
		{
			break;
		}
		if( g_HxhMoveData.m_nCalcType == 1 )
		{
			for( j = ready; j < (over + ready) / 2; j++ )
			{
				moveCost.dCostNowData[k][j] += 4 * AddData * (j - ready) / (over - ready) / (over - ready);
			}
			for( j = (over + ready) / 2; j < over; j++ )
			{
				moveCost.dCostNowData[k][j] += 4 * AddData * (over - j) / (over - ready) / (over - ready);
			}
		}
		if( g_HxhMoveData.m_nCalcType == 2 )
		{
			for( j = ready; j < over; j++ )
			{
				moveCost.dCostNowData[k][j] += AddData;
			}
		}
	}

	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
	{
		for( j = 0; j < m_PY; j++ )
		{
			for( int c = g_HxhMoveData.m_nHYSCount; c > 0 ; c-- )
			{
				moveCost.dCostNowData[c - 1][j] += moveCost.dCostNowData[c][j];
			}
			if( moveCost.dCostNowData[0][j] > moveCost.nMaxData )
				moveCost.nMaxData = moveCost.dCostNowData[0][j];
		}
	}
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_DEFCOST_COST))
	{
		for( j = 0; j < m_PY; j++ )
		{
			moveCost.dCostNowData[3][j] += moveCost.dCostNowData[4][j];
			moveCost.dCostNowData[2][j] += moveCost.dCostNowData[3][j];
			moveCost.dCostNowData[1][j] += moveCost.dCostNowData[2][j];
			moveCost.dCostNowData[0][j] += moveCost.dCostNowData[1][j];
			if( moveCost.dCostNowData[0][j] > moveCost.nMaxData )
				moveCost.nMaxData = moveCost.dCostNowData[0][j];
		}
	}
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		for( j = 0; j < m_PY; j++ )
		{
			for( int c = 0; c < g_HxhMoveData.m_nHYDCount; c++ )
			{
				moveCost.dCostNowData[c + 1][j] += moveCost.dCostNowData[c][j];
			}
			if( moveCost.dCostNowData[g_HxhMoveData.m_nHYDCount][j] > moveCost.nMaxData )
				moveCost.nMaxData = moveCost.dCostNowData[g_HxhMoveData.m_nHYDCount][j];
		}
	}

	g_CostDetail.DeleteAll();
	g_CostDetail.nData = pData[a].m_lDate;
	g_CostDetail.fInterestRatio = 0;

	float rich = 1;
	float poor = 1;
	long lClose = pData[a].m_lClosePrice;
	ready = m_PY * (m_YRefParam.max - pData[a].m_lClosePrice) / __max(m_YRefParam.max - m_YRefParam.min, 1);

	if( ready < 0 )  // by yls add 2002.11.20
		ready = 0;

	double tlpan = 0;
	double hlpan = 0;
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
	{
		for( j = 0; j < ready; j++ )
		{
			tlpan += moveCost.dCostNowData[0][j];
		}
		for( j = ready; j < m_PY; j++ )
		{
			hlpan += moveCost.dCostNowData[0][j];
		}
	}
	else if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		for( j = 0; j < ready; j++ )
		{
			tlpan += moveCost.dCostNowData[g_HxhMoveData.m_nHYDCount][j];
		}
		for( j = ready; j < m_PY; j++ )
		{
			hlpan += moveCost.dCostNowData[g_HxhMoveData.m_nHYDCount][j];
		}
	}
	g_CostDetail.fInterestRatio = hlpan / ( tlpan + hlpan );
	g_CostDetail.fInterestPan   = (float)ready;
	float fTotalHand = 0;
	float fCurrentHand = 0;

	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYANSHAN_COST))
	{
		for( i = 0; i < 1000; i ++ )	//get the total hand
		{
			fTotalHand += moveCost.dCostNowData[0][i];
		}
		for(  i = 1; i <= g_HxhMoveData.m_nHYSCount; i++ ) //get the different span's total hand
		{
			for( int j = 0; j < 1000; j++ )
			{
				fCurrentHand += moveCost.dCostNowData[i][j];
			}
			if( fCurrentHand == 0 )
			{
				moveCost.cValid = 1;
				return 1;
			}
			g_CostDetail.fHYS[i - 1] = fCurrentHand / fTotalHand * 100;
			fCurrentHand = 0;
		}
	}
	if( g_hsGraphicsData.IsStyle(GL_PERIOD_TYPE_DAY_HUOYAODU_COST))
	{
		for( i = 0; i < 1000; i ++ )
		{
			fTotalHand += moveCost.dCostNowData[g_HxhMoveData.m_nHYDCount][i];
		}
		for(  i = 0; i < g_HxhMoveData.m_nHYDCount; i++ )
		{
			for( int j = 0; j < 1000; j++ )
			{
				fCurrentHand += moveCost.dCostNowData[i][j];
			}
			if( fCurrentHand == fTotalHand )
			{
				moveCost.cValid = 1;
				return 1;
			}
			g_CostDetail.fHYD[i] = fCurrentHand / fTotalHand * 100;
			fCurrentHand = 0;
		}
	}

	moveCost.cValid = 1;*/
	return 1;
}


void CGraphics::Refresh(CDC* pDC,WPARAM wp /*= 0*/,LPARAM lp /*= 0*/)
{
	SetSizeByFont(pDC);
	//
	GetMargin(CRect(0,0,100,100));
}

void CGraphics::SetSizeByFont(CDC* pDC)
{
	CFont* pOldFont;

	// left
	pOldFont = pDC->SelectObject(GetFont(ftYAxis));//
	CSize size = pDC->GetTextExtent("999999.99");
	pDC->SelectObject(pOldFont);

	pOldFont = pDC->SelectObject(g_hsGraphicsData.ftPopupText.m_pFont);
	CSize size1 = pDC->GetTextExtent("+19990909 ");
	pDC->SelectObject(pOldFont);
	if( size1.cx > size.cx )
		size.cx = size1.cx + 4;
	else
		size.cx = size.cx + 4;

	m_rcDefMargin.left.m_nDef = size.cx ; 
	m_rcDefMargin.left.m_nMin = size.cy + size.cy/3;///3;// * 2;

#ifdef Support_FLHQ
	m_rcDefMargin.left.m_nMin += size.cy/3*2;
#endif

	//#ifdef _New_Face
	//	m_rcDefMargin.left.m_nMin = size.cy * 2;
	//#else
	//	m_rcDefMargin.left.m_nMin = size.cy;//+ size.cy;///3*2; // 图形标题高度
	//#endif
	//m_rcDefMargin.left.m_nMax = m_rcDefMargin.left.m_nDef;


	// right
	pOldFont = pDC->SelectObject(GetFont(ftYAxis));
	m_rcDefMargin.right.m_nDef = pDC->GetTextExtent("999.99%").cx;
	//m_rcDefMargin.right.m_nMin = m_rcDefMargin.right.m_nDef;
	//m_rcDefMargin.right.m_nMax = m_rcDefMargin.right.m_nDef;
	pDC->SelectObject(pOldFont);

	// top
	pOldFont = pDC->SelectObject(GetFont(ftDrawVarTitle));
	m_rcDefMargin.top.m_nDef = pDC->GetTextExtent("测").cy + 4;
	//m_rcDefMargin.top.m_nMin = m_rcDefMargin.top.m_nDef;
	//m_rcDefMargin.top.m_nMax = m_rcDefMargin.top.m_nDef;
	pDC->SelectObject(pOldFont);

	// bottom
	pOldFont = pDC->SelectObject(GetFont(ftXAxis));
	m_rcDefMargin.bottom.m_nDef = pDC->GetTextExtent("测").cy + 4;
	//m_rcDefMargin.bottom.m_nMin = m_rcDefMargin.bottom.m_nDef;
	//m_rcDefMargin.bottom.m_nMax = m_rcDefMargin.bottom.m_nDef;
	pDC->SelectObject(pOldFont);

	//qinhn 2008-12-12号添加，读取配置文件看分时图纵轴要显示的小数位以改变图形显示区域的左右宽度
	//注意:此函数是设置分时,K线图显示区域的左右上下边框位置的源头
/*	CString sFileName = CHSFilePath::GetPath(CHSFilePath::Setting) + "HappyStar.ini";

	bool readL = false;
	bool bNormalAvgLine = false, bNormalKLine = false,bIndexKLine = false;

	int LeftOffset =2, RightOffset =2;
	int nNormalAvgLine , nNormalKLine, nIndexKLine;

	readL = CfgValues::GetDetailCfgInt(m_CfgTopMapOb,"纵坐标显示小数数位", "指数分时图均线图小数位(最小值小于1000无效)" , LeftOffset);
	//CWriteReadStandardConfig::ReadConfigInt("纵坐标显示小数数位", "指数分时图均线图小数位(最小值小于1000无效)", LeftOffset, sFileName);

	bIndexKLine =  CfgValues::GetDetailCfgInt(m_CfgTopMapOb,"纵坐标显示小数数位", "指数K线图纵坐标显示小数位(最小值小于1000无效)" , nIndexKLine);
	//CWriteReadStandardConfig::ReadConfigInt("纵坐标显示小数数位", "指数K线图纵坐标显示小数位(最小值小于1000无效)", nIndexKLine, sFileName);
	if(readL)
	{
		switch(LeftOffset)
		{
			case 0:	m_rcDefMargin.left.m_nDef = m_rcDefMargin.left.m_nDef - Offset_Width;  
				break;
			case 2: m_rcDefMargin.left.m_nDef = m_rcDefMargin.left.m_nDef + Offset_Width;  
				break;
			case 3:	m_rcDefMargin.left.m_nDef = m_rcDefMargin.left.m_nDef + Offset_Width*2;   
				break;
			case 4:	m_rcDefMargin.left.m_nDef = m_rcDefMargin.left.m_nDef + Offset_Width*3;  
				break;
			case 5:	m_rcDefMargin.left.m_nDef = m_rcDefMargin.left.m_nDef + Offset_Width*4;  
				break;
			default: ;
				break;
		}
	}*/
	SetMargin( CRect(m_rcDefMargin.left.m_nDef,m_rcDefMargin.top.m_nDef,
		m_rcDefMargin.right.m_nDef,m_rcDefMargin.bottom.m_nDef) );
		
}

void CGraphics::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}
/********************************************************************************
* 函数功能 : 绘制星星群
* 函数参数 : DWORD linetype - 线的类型,
* 			  CTValues<DrawPointInfo> pRefX - X向相对坐标,
* 			  CTValues<StarFieldItem> pRefValues - 数据,
* 			  long lMaxValue , long lMinValue, - 最大最小值
*			  int Index = 0,const char* Name - 绘制扩展相关参数
*  返回值  : void
* 调用依赖 : 星空图用
* 作    者 : 熊钟彬
* 完成日期 : 2003-08-14
* 修改记录 : 
* 作	者 : 
* 日	期 : 
* 修改内容 : 
*******************************************************************************/

void CGraphics::DrawStars(DWORD linetype,
						  CTValues<DrawPointInfo> pRefX,
						  CTValues<StarFieldItem> pRefValues,
						  ColorProperty& clLine,
						  COLORREF cr /*= 0*/, int Index/* = 0*/, const char* Name/* = NULL*/)
{
/*	if(pRefValues.m_pValue == NULL 
		|| pRefValues.m_lSize <= 0
		|| pRefValues.m_lStart < 0 
		|| pRefValues.m_pValue == NULL 
		|| pRefValues.m_lSize <= 0
		//|| pRefValues.m_lStart < 0	
		) // 检验参数
	{
		//ASSERT(FALSE);
		TRACE("CHSLinear<T>::DrawStars(), params is invalid! break!");
		return;
	}
	//DrawPointInfo* pXPoint = pRefX.m_pValue;		// X轴点
	StarFieldItem* pValues = pRefValues.m_pValue;	// 数值
	// 初始化画笔
	CPen hPen;
	hPen.CreatePen(clLine.m_nPenStyle, clLine.m_nWidth, clLine.m_lColor);
	CPen* hOldPen = m_pDC->SelectObject(&hPen);

	int nYPos = 0;
	int nXPos = 0;
	// 计算区间范围
	double dbXRange = m_XRefParam.max - m_XRefParam.min;
	if(dbXRange > -0.000005 && dbXRange < 0.00005)
	{
		dbXRange = 1;
	}
	double dbYRange = m_YRefParam.max - m_YRefParam.min;
	if(dbYRange > -0.000005 && dbYRange < 0.00005)
	{
		dbYRange = 1;
	}
	//	杨朝惠添加
	CRect	*pMargin	= GetMargin();
	int	nDownLimit		= m_GB;
	int	nUpperLimit		= m_GT;

	for (int i = 0; i < pRefValues.m_lSize; i++)
	{
		if( !IsValue( & pValues[i].m_dbY.m_dData,
			& pValues[i].m_dbX.m_dData, NULL ) )
		{
			continue;
		}
		// X Y 屏幕坐标
		nYPos = m_GB - (int)((float)(m_PY - 16) * (pValues[i].m_dbY.m_dData 
			- m_YRefParam.min) / dbYRange ) - 8;

		//	not permit display outside of the valid draw rectangle
		//	Appended by smallfool. 20031120
		if ( nYPos <= nUpperLimit )
		{
			nYPos = nUpperLimit;
		}
		if ( nYPos > nDownLimit )
		{
			nYPos	= nDownLimit;
		}
		//	End of append by smallfool 20031120


		nXPos = m_GL + (int)((float)(m_PX - 16)* (pValues[i].m_dbX.m_dData 
			- m_XRefParam.min) / dbXRange ) + 8;

		//	not permit display outside of the valid draw rectangle
		//	Appended by smallfool. 20031120		
		if ( nXPos < m_GL)
		{
			nXPos	= m_GL;
		}
		if ( nXPos > m_GR )
		{
			nXPos	= m_GR;
		}

		CImageListEx* pImage = g_hsGraphicsData.m_expressImages;
		if( pImage != NULL && pImage->m_hImageList != NULL )
		{
			pImage->Draw(m_pDC, pValues[i].GetImage(),
				CPoint(nXPos - 8, nYPos - 8), ILD_TRANSPARENT);
		}		
	}

	m_pDC->SelectObject(hOldPen);
	hPen.DeleteObject();*/
}

POINT CGraphics::GetPoint_LogY(double x, double y)
{
	return GetPoint(x,y);

	if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((x - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
	if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (log10(y) - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}

	if(m_Pt.y <= m_GT)
		m_Pt.y = m_GT + 1;
	if(m_Pt.y >= m_GB)
		m_Pt.y = m_GB - 1;

	return m_Pt;
}

POINT CGraphics::GetPoint(double x, double y)
{
	if(m_Scale.dx != 0)
	{
		m_Pt.x = (LONG)((x - m_Scale.xmin) / m_Scale.dx) + m_GL;
	}
	else
	{
		m_Pt.x = 0;
	}
	if(m_Scale.dy != 0)
	{
		m_Pt.y = (LONG)(m_GB - (y - m_Scale.ymin) / m_Scale.dy);
	}
	else
	{
		m_Pt.y = 0;
	}

	if(m_Pt.y <= m_GT)
		m_Pt.y = m_GT + 1;
	if(m_Pt.y >= m_GB)
		m_Pt.y = m_GB - 1;

	return m_Pt;
}

bool CGraphics::SetRange_LogY(double xmin, double ymin, double xmax, double ymax)
{
	m_nAxesType	 = YLOG;

	m_Scale.xmin = xmin;
	if( ymin < 0.000001 )
		m_Scale.ymin = 0;
	else
		m_Scale.ymin = log10(ymin);


	m_Scale.ymin = ymin;

	m_Scale.xmax = xmax;
	if( ymax < 0.000001 )
		m_Scale.ymax = 0;
	else
		m_Scale.ymax = log10(ymax);

	m_Scale.ymax = ymax;

	m_nYStep = (int)(m_Scale.ymax - m_Scale.ymin);

	return true;
}
