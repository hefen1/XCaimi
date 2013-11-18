// HSBaseDC.cpp: implementation of the CHSBaseDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSBaseDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COLORREF CHSBaseDC::m_clPrintBKColor   = RGB(255,255,255);
COLORREF CHSBaseDC::m_clPrintTextColor = 0;
CPen	 CHSBaseDC::m_pPrintPen;
CBrush	 CHSBaseDC::m_pPrintBrush;

BOOL CHSBaseDC::Init()
{
	m_pPrintBrush.CreateSolidBrush(m_clPrintBKColor);
	m_pPrintPen.CreatePen(PS_SOLID,1,m_clPrintTextColor);
	return TRUE;
}

CHSBaseDC::CHSBaseDC() :CDC() 
{
	m_bSelf = 1;
	// 吕连新加入， 为了支持彩印
	m_bSupportMulticolor = TRUE;
}

CHSBaseDC::CHSBaseDC(CDC* pDC) : CDC()
{
	m_bSelf = 0;
	ASSERT(pDC != NULL);

	m_bPrinting = pDC->m_bPrinting;
	m_hDC       = pDC->m_hDC;
	m_hAttribDC = pDC->m_hAttribDC;
	// 吕连新加入， 为了支持彩印
	m_bSupportMulticolor = TRUE;
}

CHSBaseDC::~CHSBaseDC()
{
	if( !m_bSelf )
	{
		m_hDC = m_hAttribDC = NULL;
	}
}

COLORREF CHSBaseDC::SetBkColor(COLORREF crColor)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SetBkColor(m_clPrintBKColor);
	}

	return CDC::SetBkColor(crColor);
}

COLORREF CHSBaseDC::SetTextColor(COLORREF crColor)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() || (m_bPrinting && crColor == RGB(255,255,255)))
	{
		return CDC::SetTextColor(m_clPrintTextColor);
	}

	return CDC::SetTextColor(crColor);
}

CGdiObject* CHSBaseDC::SelectStockObject(int nIndex)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SelectStockObject(nIndex);
	}

	return CDC::SelectStockObject(nIndex);
}

CPen* CHSBaseDC::SelectObject(CPen* pPen)
{
	/*************************************************************
	* 吕连新加入
	* 时间：  2003年9月12日
	* 描述：  为了支持彩印, 打印时，如果pPen色采为白色，则取代
	*************************************************************/
	LOGPEN logPen;
	if( pPen != NULL )
	{
		pPen->GetLogPen(&logPen);
	}
	if( (IsMonochromePrint() || (m_bPrinting && pPen && logPen.lopnColor== RGB(255,255,255)) )
		&& m_pPrintPen.m_hObject)
	{
		return CDC::SelectObject(&m_pPrintPen);
	}
	/**********吕连新加入结束*************/

	return CDC::SelectObject(pPen);
}

CBrush* CHSBaseDC::SelectObject(CBrush* pBrush)
{
	/*************************************************************
	* 吕连新加入
	* 时间：  2003年9月12日
	* 描述：  为了支持彩印,打印时,如果pBrush为黑色，则取代
	*************************************************************/
	LOGBRUSH logBrush;
	if( pBrush )
	{
		pBrush->GetLogBrush( &logBrush );
	}
	if( IsMonochromePrint() 
		|| (pBrush && m_bPrinting && logBrush.lbColor == RGB(0,0,0)) )
	{
		return CDC::SelectObject(&m_pPrintBrush);
	}
	/**********吕连新加入结束*************/

	return CDC::SelectObject(pBrush);
}

CFont* CHSBaseDC::SelectObject(CFont* pFont)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pFont);
	}

	return CDC::SelectObject(pFont);
}

CBitmap* CHSBaseDC::SelectObject(CBitmap* pBitmap)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pBitmap);
	}

	return CDC::SelectObject(pBitmap);
}

int CHSBaseDC::SelectObject(CRgn* pRgn)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pRgn);
	}

	return CDC::SelectObject(pRgn);
}

CGdiObject* CHSBaseDC::SelectObject(CGdiObject* pObject)
{
	// 吕连新加入， 为了支持彩印
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pObject);
	}

	return CDC::SelectObject(pObject);
}

void CHSBaseDC::FillRect(LPCRECT lpRect, CBrush* pBrush)
{
	/*************************************************************
	* 吕连新加入
	* 时间：  2003年9月12日
	* 描述：  为了支持彩印,打印时,如果pBrush为黑色，则取代
	*************************************************************/
	LOGBRUSH sLogBrush;

	if( pBrush )
	{
		pBrush->GetLogBrush(&sLogBrush);
	}
	if( IsMonochromePrint() || (m_bPrinting && pBrush && sLogBrush.lbColor == RGB(0,0,0)) )
	{
		CDC::FillRect(lpRect, &m_pPrintBrush);
		return;
	}
	/**********吕连新加入结束*************/

	CDC::FillRect(lpRect, pBrush);
}

void CHSBaseDC::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	// 吕连新加入， 为了支持彩印,如果clr为黑色，则取代
	if( IsMonochromePrint() || (m_bPrinting && clr== RGB(0,0,0)) )
	{
		CDC::FillSolidRect(lpRect, m_clPrintBKColor);
		return;
	}

	CDC::FillSolidRect(lpRect, clr);
}

void CHSBaseDC::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	// 吕连新加入， 为了支持彩印, 如果clr为黑色，则取代
	if( IsMonochromePrint() || (m_bPrinting && clr== RGB(0,0,0)) )
	{
		CDC::FillSolidRect(x, y, cx, cy, m_clPrintBKColor);
		return;
	}

	CDC::FillSolidRect(x, y, cx, cy, clr);
}

BOOL CHSBaseDC::FloodFill(int x, int y, COLORREF crColor)
{
	// 吕连新加入， 为了支持彩印,
	if( IsMonochromePrint() )
	{
		return CDC::FloodFill(x, y, m_clPrintBKColor);
	}

	return CDC::FloodFill(x, y, crColor);
}

BOOL CHSBaseDC::FillRgn(CRgn* pRgn, CBrush* pBrush)
{
	// 吕连新加入， 为了支持彩印,
	if( IsMonochromePrint() )
	{
		return CDC::FillRgn(pRgn, &m_pPrintBrush);
	}

	return CDC::FillRgn(pRgn, pBrush);
}

HGDIOBJ CHSBaseDC::SelectObject(HGDIOBJ hGdiobj)
{
	ASSERT(FALSE);
	return NULL;

	/*
	if(m_bPrinting)
	{
		CFont font;
		if(font.Attach(hGdiobj) && font.m_hObject != NULL)
		{
			return CHSBaseDC::SelectObject(&font)->m_hObject;
		}
		return CDC::SelectObject(hGdiobj);		
	}

	return CDC::SelectObject(hGdiobj);
	*/
}
