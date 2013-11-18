#include "StdAfx.h"
#include "DrawTableStruct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMap<WORD,WORD&, DWORD, DWORD&>  DrawTool::m_mapColor;
CMapWordToPtr                    DrawTool::m_mapFont;
COLORREF DrawTool::GetColor(WORD wClr)
{
	COLORREF clr = g_lDefautColor;
	if (m_mapColor.Lookup(wClr, clr))
		return clr;	
	
	IHsColor* pColor = theFoundationApp.m_iColor;
	switch (wClr)
	{
	case TEXT_COLOR_UP:
		clr = pColor->GetUpColor();
		break;
	case TEXT_COLOR_DOWN:
		clr = pColor->GetDownColor();
		break;
	case TEXT_COLOR_FLAT:
		clr = pColor->GetEqualColor();
		break;
	case TEXT_COLOR_TIME:
		clr = pColor->GetRTableTime();
		break;
	case TEXT_COLOR_FIXEDNAME:
		clr = pColor->GetRTableFixedName();
		break;
	case TEXT_COLOR_ACTIVEFIXEDNAME:
		clr = pColor->GetZiXunFG();
		break;
	case TEXT_COLOR_TOTALHAND:
		clr = pColor->GetTotalColor();
		break;
	case TEXT_COLOR_XIANHAND:
		clr = pColor->GetNowColor();
		break;
	case TEXT_COLOR_CHENGJIAO:
		clr = pColor->GetDoneColor();
		break;
	case TEXT_COLOR_HUANSHOU:
		clr = pColor->GetChangeColor();
		break;
	case TEXT_COLOR_LIANGBI:
		clr = pColor->GetQuantitativeRateColor();
		break;
	case TEXT_COLOR_STOCKCODE:
		clr = pColor->GetRTitleCode();
		break;
	case TEXT_COLOR_STOCKNAME:
		clr = pColor->GetRTitleCodeName();
		break;
	case TEXT_COLOR_STOCKNAME_SELF:
		clr = pColor->GetFixedNameSelfColor();
		break;
	case TEXT_COLOR_WEIBUY:
		clr = pColor->GetOrderBuyVolColor();
		break;
	case TEXT_COLOR_WEISELL:
		clr = pColor->GetOrderSellVolColor();
		break;
	case TEXT_COLOR_TIPFG:
		clr = pColor->GetTipTextFg();
		break;
	case TEXT_COLOR_TIPBK:
		clr = pColor->GetTipTextBk();
		break;
	case TEXT_COLOR_TRADEFIXEDNAME:
		clr = pColor->GetTradeTableFixedName();
		break;
	case TEXT_COLOR_TRADEUP:
		clr = RGB(255,0,0);
		break;
	case TEXT_COLOR_TRADEDOWN:
		clr = RGB(0,200,64);
		break;
	case TEXT_COLOR_TRADEXIANSHOU:
		clr = RGB(0,0,255);
		break;
	default:
		break;
	}

	m_mapColor.SetAt(wClr, clr);
	return clr;
}

CFont* DrawTool::GetFont(WORD wFont)
{
	CFont* pFont = NULL;
	if (m_mapFont.Lookup(wFont, (void*&)pFont))
	{
		return pFont;
	}

	IHsFont* iFont = theFoundationApp.m_iFont;
	switch (wFont)
	{
	case TEXT_FONT_FIXED:
		pFont = iFont->GetRTableFixedFont();
		break;
	case TEXT_FONT_DYNAMICAL:
		pFont = iFont->GetRTableDynamicalFont();
		break;
	case TEXT_FONT_FINANCE:
		pFont = iFont->GetRTableFinanceFont();
		break;
	case TEXT_FONT_TIME:
		pFont = iFont->GetRTableTimeFont();
		break;
	case TEXT_FONT_TITLE:
		pFont = iFont->GetDiagramTableTitleFont();
		break;
	case TEXT_FONT_TIP:
		pFont = iFont->GetTipTxtFont();
		break;
	case TEXT_FONT_TRADE:
		pFont = iFont->GetDiagramTradeFont();
		break;
	case TEXT_FONT_QUOTE_CODE:
		pFont = iFont->GetQuoteCodeFont();
		break;
	case TEXT_FONT_QUOTE_NAME:
		pFont = iFont->GetQuoteStringFont();
		break;
	default:
		break;
	}

	m_mapFont.SetAt(wFont, (void*)pFont);
	return pFont;
}