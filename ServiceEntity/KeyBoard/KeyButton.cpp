// KeyButton.cpp : 实现文件
//

#include "stdafx.h"
#include "KeyBoard.h"
#include "KeyButton.h"


// CKeyButton
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CKeyButton, CButton)

CKeyButton::CKeyButton()
{
}

CKeyButton::~CKeyButton()
{
	if (m_dcBK.m_hDC)
		m_dcBK.DeleteDC();
	if (m_dcBut.m_hDC)
		m_dcBut.DeleteDC();
}




BEGIN_MESSAGE_MAP(CKeyButton, CButton)
		ON_WM_ERASEBKGND()
		ON_WM_DRAWITEM()
END_MESSAGE_MAP()


void CKeyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rect;
	rect = lpDrawItemStruct->rcItem;
	if (pDC)
	{
		if ( m_dcBK.m_hDC)
		{
			pDC->BitBlt(rect.left, rect.top,rect.Width(),rect.Height(),&m_dcBK,0,0,SRCCOPY);
		}
		if ( m_hBitmap )
		{
		    pDC->DrawState (rect.TopLeft (), m_sizeImage,  m_hBitmap,
				DST_BITMAP|DSS_NORMAL);
			
		}
	}
}
void CKeyButton::SetBkDC(CDC* dc)
{
	CRect rect,rectS;
	CBitmap bmp;
	GetClientRect(rect);
	GetWindowRect(rectS);
	CWnd* pWnd = GetParent();
	if (pWnd && m_dcBK.m_hDC == NULL)
	{
		pWnd->ScreenToClient(rectS);
		m_dcBK.DeleteDC();
		m_dcBK.CreateCompatibleDC(dc);
		bmp.CreateCompatibleBitmap(dc,rect.Width(), rect.Height());
		CBitmap *pOld = m_dcBK.SelectObject(&bmp);
		m_dcBK.BitBlt(0,0,rect.Width(), rect.Height(),dc,rectS.left,rectS.top, SRCCOPY);
		m_dcBK.SelectObject(pOld);
		bmp.DeleteObject();
	}
}
void CKeyButton::SetImage(HBITMAP hBmp)
{
	if (hBmp)
	{
		m_hBitmap = hBmp;
		BITMAP bmp;
		::GetObject (m_hBitmap, sizeof (BITMAP), (LPVOID) &bmp);
		m_sizeImage.cx = bmp.bmWidth;
		m_sizeImage.cy = bmp.bmHeight;

	}
}

BOOL CKeyButton::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;

}

