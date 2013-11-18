#include "StdAfx.h"
#include "HSViewInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHSViewInfo::CHSViewInfo(void)
{
	m_pScrollBar = new CHSScrollBar(HSSB_VERT | HSSB_RIGHT | HSSB_BLOCK_ALWAYSTOP);
	m_pScrollBar->SetScrollNotify(this);
	m_pScrollBar->SetSize(VIEW_SCROLL_SIZE);
	m_rcInfo.SetRectEmpty();
	m_strText.Empty();
	m_nMove = 0;
}

CHSViewInfo::~CHSViewInfo(void)
{
	if (m_pScrollBar)
	{
		delete m_pScrollBar;
		m_pScrollBar = NULL;
	}
}

BOOL CHSViewInfo::Draw( CDC* pDC )
{
	// 画背景
	pDC->FillSolidRect(&m_rcInfo, theFoundationApp.m_iColor->GetZiXunBK());

	// 绘制滚动条
	m_pScrollBar->Draw(pDC);

	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	// 画文本
	CFont* pFont = theFoundationApp.m_iFont->GetZiXunFont();
	CFont* pOldFont = pDC->SelectObject(pFont);
	COLORREF oldClr = pDC->SetTextColor(theFoundationApp.m_iColor->GetZiXunFG());

	CRect rc = m_rcInfo;
	rc.DeflateRect(10,10,0,0);
	CString strText;
	GetTextOutString(strText);
	pDC->DrawText(strText, strText.GetLength(), &rc, DT_WORDBREAK | DT_EXTERNALLEADING);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(oldClr);
	pDC->SetBkMode(nOldBkMode);

	return TRUE;
}
void CHSViewInfo::SetRect( CRect rc )
{
	if (rc.IsRectEmpty())
	{
		return;
	}

	if (m_strText.IsEmpty())
	{
		m_rcInfo = rc;
	}
	else
	{
		m_rcInfo = m_pScrollBar->SetRect(rc, m_nRowCount - CalculateShowRow());
	}
}

void CHSViewInfo::SetText( CString str )
{
	m_strText = str;
	m_nRowCount = CalculateRow();
}

void CHSViewInfo::SetViewNotify( IViewInfoNotify* pNotify )
{
	m_iNotify = pNotify;
}

void CHSViewInfo::GetTextOutString( CString& strText )
{	
	CString strEnter = "\r\n";
	int nStart = 0;
	int nLocate = 0;

	int nTime = 0;
	while (nTime < m_nMove)
	{
		nLocate = m_strText.Find(strEnter, nStart);
		nStart = nLocate + 1;
		
		nTime ++;
	}
	nLocate += strEnter.GetLength(); // 去除前面的换行符
	strText = m_strText.Right(m_strText.GetLength() - nLocate);
}

int CHSViewInfo::CalculateRow()
{
	CString strEnter = "\r\n";
	
	int nStart = 0; 
	int nLocate= 0; 

	int nCount = 0;
	nLocate = m_strText.Find(strEnter, nStart); 
	while(nLocate != -1) 
	{ 
		nCount++; 
		nStart = nLocate + 1; 
		nLocate = m_strText.Find(strEnter, nStart); 
	} 
	return nCount;
}

int CHSViewInfo::CalculateShowRow()
{
	CFont* pFont = theFoundationApp.m_iFont->GetZiXunFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	int nFontHeight = (int)(abs(lf.lfHeight)*1.5);
	return m_rcInfo.Height() / nFontHeight;
}

BOOL CHSViewInfo::MouseDown( UINT nFlags, CPoint point)
{
	if (!m_rcInfo.PtInRect(point))
	{
		m_pScrollBar->MouseDown(nFlags, point);
	}

	return TRUE;
}

BOOL CHSViewInfo::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	if (m_pScrollBar->GetRange() != 0)
	{
		int nDiff = (-1) * zDelta / WHEEL_DELTA;
		int nPosition = m_nMove + nDiff;
		SetPosition(nPosition, TRUE);
		m_pScrollBar->OnScroll(nDiff);
		return TRUE;
	}
	return FALSE;
}

int CHSViewInfo::OnKey( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	int nShowRow = CalculateShowRow();
	if (nChar == VK_PRIOR)
	{
		int nPosition = m_nMove + nShowRow * (-1);
		SetPosition(nPosition, TRUE);
		m_pScrollBar->OnScroll(nShowRow * (-1));
		return TRUE;
	}
	else if (nChar == VK_NEXT)
	{
		int nPosition = m_nMove + nShowRow;
		SetPosition(nPosition, TRUE);
		m_pScrollBar->OnScroll(nShowRow);
		return TRUE;
	}
	return FALSE;
}

BOOL CHSViewInfo::SetPosition( int nPosition, BOOL bIsVert, BOOL bIsRedraw /*= TRUE*/ )
{
	int nRange = m_nRowCount - CalculateShowRow();
	if (nPosition <= 0)
		nPosition = 0;
	else if (nPosition >= nRange)
		nPosition = nRange;
	
	if (nPosition == m_nMove)
		return FALSE;
	
	m_nMove = nPosition;
	if (bIsRedraw)
		m_iNotify->ViewInfoInvalidate(&m_rcInfo, FALSE);
	
	return TRUE;
}

void CHSViewInfo::ScrollInvalidate( CRect* pRect, BOOL bErase /*= TRUE*/ )
{
	m_iNotify->ViewInfoInvalidate(pRect, bErase);
}

HWND CHSViewInfo::GetHandle()
{
	return m_iNotify->ViewInfoGetHandle();
}
