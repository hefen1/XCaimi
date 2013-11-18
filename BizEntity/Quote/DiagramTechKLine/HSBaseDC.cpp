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
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	m_bSupportMulticolor = TRUE;
}

CHSBaseDC::CHSBaseDC(CDC* pDC) : CDC()
{
	m_bSelf = 0;
	ASSERT(pDC != NULL);

	m_bPrinting = pDC->m_bPrinting;
	m_hDC       = pDC->m_hDC;
	m_hAttribDC = pDC->m_hAttribDC;
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
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
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SetBkColor(m_clPrintBKColor);
	}

	return CDC::SetBkColor(crColor);
}

COLORREF CHSBaseDC::SetTextColor(COLORREF crColor)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() || (m_bPrinting && crColor == RGB(255,255,255)))
	{
		return CDC::SetTextColor(m_clPrintTextColor);
	}

	return CDC::SetTextColor(crColor);
}

CGdiObject* CHSBaseDC::SelectStockObject(int nIndex)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SelectStockObject(nIndex);
	}

	return CDC::SelectStockObject(nIndex);
}

CPen* CHSBaseDC::SelectObject(CPen* pPen)
{
	/*************************************************************
	* �����¼���
	* ʱ�䣺  2003��9��12��
	* ������  Ϊ��֧�ֲ�ӡ, ��ӡʱ�����pPenɫ��Ϊ��ɫ����ȡ��
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
	/**********�����¼������*************/

	return CDC::SelectObject(pPen);
}

CBrush* CHSBaseDC::SelectObject(CBrush* pBrush)
{
	/*************************************************************
	* �����¼���
	* ʱ�䣺  2003��9��12��
	* ������  Ϊ��֧�ֲ�ӡ,��ӡʱ,���pBrushΪ��ɫ����ȡ��
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
	/**********�����¼������*************/

	return CDC::SelectObject(pBrush);
}

CFont* CHSBaseDC::SelectObject(CFont* pFont)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pFont);
	}

	return CDC::SelectObject(pFont);
}

CBitmap* CHSBaseDC::SelectObject(CBitmap* pBitmap)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pBitmap);
	}

	return CDC::SelectObject(pBitmap);
}

int CHSBaseDC::SelectObject(CRgn* pRgn)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pRgn);
	}

	return CDC::SelectObject(pRgn);
}

CGdiObject* CHSBaseDC::SelectObject(CGdiObject* pObject)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ
	if( IsMonochromePrint() )
	{
		return CDC::SelectObject(pObject);
	}

	return CDC::SelectObject(pObject);
}

void CHSBaseDC::FillRect(LPCRECT lpRect, CBrush* pBrush)
{
	/*************************************************************
	* �����¼���
	* ʱ�䣺  2003��9��12��
	* ������  Ϊ��֧�ֲ�ӡ,��ӡʱ,���pBrushΪ��ɫ����ȡ��
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
	/**********�����¼������*************/

	CDC::FillRect(lpRect, pBrush);
}

void CHSBaseDC::FillSolidRect(LPCRECT lpRect, COLORREF clr)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ,���clrΪ��ɫ����ȡ��
	if( IsMonochromePrint() || (m_bPrinting && clr== RGB(0,0,0)) )
	{
		CDC::FillSolidRect(lpRect, m_clPrintBKColor);
		return;
	}

	CDC::FillSolidRect(lpRect, clr);
}

void CHSBaseDC::FillSolidRect(int x, int y, int cx, int cy, COLORREF clr)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ, ���clrΪ��ɫ����ȡ��
	if( IsMonochromePrint() || (m_bPrinting && clr== RGB(0,0,0)) )
	{
		CDC::FillSolidRect(x, y, cx, cy, m_clPrintBKColor);
		return;
	}

	CDC::FillSolidRect(x, y, cx, cy, clr);
}

BOOL CHSBaseDC::FloodFill(int x, int y, COLORREF crColor)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ,
	if( IsMonochromePrint() )
	{
		return CDC::FloodFill(x, y, m_clPrintBKColor);
	}

	return CDC::FloodFill(x, y, crColor);
}

BOOL CHSBaseDC::FillRgn(CRgn* pRgn, CBrush* pBrush)
{
	// �����¼��룬 Ϊ��֧�ֲ�ӡ,
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
