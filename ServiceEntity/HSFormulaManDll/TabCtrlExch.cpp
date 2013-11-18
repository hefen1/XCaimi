// TabCtrlExch.cpp : implementation file
//

#include "stdafx.h"
#include "TabCtrlExch.h"

#define HS_ORDER_WIDTH 2
// CTabCtrlExch


void CTabCtrlExch::tabItem::DrawItem(CDC* pDc, COLORREF &crActive, COLORREF &crTxt, COLORREF& crMove, CImageList *pImage)
{
	CRect rc(rcItem);
	
	if( bActive == 1 )
	{
		pDc->SetTextColor(crActive);
	}
	else if( bActive == 2 )
	{
		pDc->SetTextColor(crMove);
	}
	else
		pDc->SetTextColor(crTxt);
	pDc->DrawText(strData, rc, DT_CENTER|DT_VCENTER);
}

BOOL CTabCtrlExch::tabItem::HitTest(CPoint& pt)
{
	CRect rc(rcItem);

	return rc.PtInRect(pt);
}

IMPLEMENT_DYNAMIC(CTabCtrlExch, CWnd)
CTabCtrlExch::CTabCtrlExch()
{
	m_pCall = NULL;
	m_dwMsg = 0;
	m_crBlock = ::GetSysColor(CTLCOLOR_DLG);
	m_crText = 0;
	m_crActive = RGB(255,0,0);
	m_crMove = RGB(0,0,255);
}

CTabCtrlExch::~CTabCtrlExch()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CTabCtrlExch, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_NCMOUSEMOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CTabCtrlExch message handlers


void CTabCtrlExch::OnPaint()
{
	CPaintDC dc(this);

	ReDraw();
}

void CTabCtrlExch::AddItem(tabItem* pItem)
{
	if( pItem == NULL )
		return;
	m_ayItem.Add(pItem);
}

void CTabCtrlExch::RemoveAt(int nIdx)
{
	if( nIdx >= 0 && nIdx < m_ayItem.GetSize() )
	{
		delete m_ayItem.GetAt(nIdx);
		m_ayItem.RemoveAt(nIdx);
	}
}

void CTabCtrlExch::RemoveAll()
{
	for( int i = 0; i < m_ayItem.GetSize(); i++ )
	{
		delete m_ayItem.GetAt(i);
	}
	m_ayItem.RemoveAll();
}

BOOL CTabCtrlExch::SetItem(int nIdx, tabItem* pItem)
{
	if( nIdx >= 0 && nIdx < m_ayItem.GetSize() )
	{
		delete m_ayItem.GetAt(nIdx);
		m_ayItem.SetAt(nIdx, pItem);

		return TRUE;
	}

	return FALSE;
}

BOOL CTabCtrlExch::SetItem(int nIdx, CString strData, UINT uP, int iM)
{
	if( nIdx >= 0 && nIdx < m_ayItem.GetSize() )
	{
		tabItem* pItem = m_ayItem.GetAt(nIdx);
		if( pItem != NULL )
		{
			if( !strData.IsEmpty() )
				pItem->strData = strData;
			pItem->iImage = iM;
			pItem->uParam = uP;
			return TRUE;
		}
	}

	SetSize(CRect(0,0,0,0));
	return FALSE;
}

// client pt
int CTabCtrlExch::HisTest(CPoint& pt)
{
	int iActive = -1;
	int iOld = 0;

	for( int i = 0; i < GetItemCount(); i++ )
	{
		tabItem *pItem = GetItem(i);
		if( pItem != NULL )
		{
			if( pItem->bActive == 1 )
			{
				iOld = i;
			}

			if( pItem->HitTest(pt) )
			{
				iActive = i;
			}
			//else
			//{
			//	pItem->bActive = 0;
			//}
		}
	}

	if( iActive == -1 )
	{
		tabItem *pItem = GetItem(iOld);
		if( pItem != NULL )
		{
			iActive = iOld;
		}
	}
	return iActive;
}

void CTabCtrlExch::SetCallBreak(HS_Call_Notify pCall, DWORD dwMsg)
{ 
	m_dwMsg = dwMsg;
	m_pCall = pCall; 
}

void CTabCtrlExch::SetActive(int nIdx)
{
	for( int i = 0; i < GetItemCount(); i++ )
	{
		tabItem *pItem = GetItem(i);
		if( pItem != NULL )
		{
			if( i == nIdx )
			{
				pItem->bActive = 1;
			}
			else
			{
				pItem->bActive = 0;
			}
		}
	}
	ReDraw();
}

void CTabCtrlExch::SetActive(UINT uParam)
{
	for( int i = 0; i < GetItemCount(); i++ )
	{
		tabItem *pItem = GetItem(i);
		if( pItem != NULL )
		{
			if( pItem->uParam == uParam )
			{
				pItem->bActive = 1;
			}
			else
			{
				pItem->bActive = 0;
			}
		}
	}
	//Invalidate();
	ReDraw();
}

int CTabCtrlExch::GetCurSel()
{
	tabItem* pItem;
	for( int i = 0; i < GetItemCount(); i++ )
	{
		pItem = GetItem(i);
		if( pItem != NULL && pItem->bActive == 1 )
		{
			return i;
		}
	}

	return -1;
}

// client pt
BOOL CTabCtrlExch::MousePress(CPoint& pt)
{
	int iActive = HisTest(pt);
	if( iActive == -1 )
		return FALSE;

	tabItem *pItem = GetItem(iActive);
	if( pItem == NULL || m_pCall == NULL )
	{
		return FALSE;
	}

	if( (*m_pCall)(m_dwMsg, (WPARAM)iActive, (LPARAM)pItem->uParam) == 0 )
	{
		SetActive((int)iActive);
	}
	else
	{
		MouseMove(CPoint(-1, -1));
	}

	return TRUE;
}

BOOL CTabCtrlExch::Create(CWnd* pParent, UINT uID, int nItem, CRect rcClient)
{
	if( CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rcClient, pParent, uID) == FALSE )
		return FALSE;

	RemoveAll();

	tabItem* pItem;
	for( int i = 0; i < nItem; i++ )
	{
		pItem = new tabItem;
		if( pItem == NULL )
			return FALSE;
		AddItem(pItem);
	}

	return TRUE;
}

CTabCtrlExch::tabItem* CTabCtrlExch::GetItem(int nIdx)
{
	if( nIdx >= 0 && nIdx < m_ayItem.GetSize() )
	{
		return m_ayItem.GetAt(nIdx);
	}
	return NULL;
}

BOOL CTabCtrlExch::SetSize(CRect rectClient)
{
	if( rectClient.IsRectEmpty() )
	{
		GetClientRect(rectClient);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, rectClient.Width(), rectClient.Height(), SWP_NOMOVE|SWP_SHOWWINDOW);
	}

	CDC *pDc = GetDC();
	CRect rcClient;

	GetClientRect(rcClient);
	m_rectVialid = rcClient;
	
	if( pDc != NULL )
	{
		tabItem* pItem = NULL;
		for( int i = 0; i < GetItemCount(); i++ )
		{
			pItem = GetItem(i);
			if( pItem != NULL )
			{
				pItem->rcItem = rcClient;
				pItem->rcItem.left += HS_ORDER_WIDTH;
				CSize szText = pDc->GetTextExtent(pItem->strData);
				pItem->rcItem.right = pItem->rcItem.left + HS_ORDER_WIDTH + szText.cx;
				rcClient.left = pItem->rcItem.right;
			}
		}
		if( pItem != NULL )
		{
			m_rectVialid.right = pItem->rcItem.right;
		}
		this->ReleaseDC(pDc);
		return TRUE;
	}

	this->ReleaseDC(pDc);
	return FALSE;
}

BOOL CTabCtrlExch::HitTestEx(CPoint& pt)
{
	int iActive = -1;

	for( int i = 0; i < GetItemCount(); i++ )
	{
		tabItem *pItem = GetItem(i);
		if( pItem != NULL )
		{
			if( pItem->bActive == 2 )
			{
				iActive = i;
			}

			if( pItem->HitTest(pt) )
			{
				if( pItem->bActive != 1 )
					pItem->bActive = 2;
				iActive = i;
			}
			else if( pItem->bActive == 2 )
			{
				pItem->bActive = 0;
			}
		}
	}

	return iActive;
}

void CTabCtrlExch::MouseMove(CPoint& pt)
{
	if( HitTestEx(pt) != -1 )
	{
		ReDraw();
		SetCapture();
	}
	else
	{
		::SetCapture(NULL);
	}
}

void CTabCtrlExch::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CTabCtrlExch::SetColors(COLORREF crSelCol, COLORREF crUnSelCol, COLORREF crBk, COLORREF crMove)
{
	m_crBlock = crBk;
	m_crText = crUnSelCol;
	m_crActive = crSelCol;
	m_crMove = crMove;
}

BOOL CTabCtrlExch::ReDraw()
{
	CDC* pDc = GetDC();
	if( pDc == NULL )
		return FALSE;

	HGDIOBJ hOldObj= pDc->SelectObject(::GetStockObject(SYSTEM_FIXED_FONT));

	CRect rcClient;
	GetClientRect(rcClient);
	pDc->FillSolidRect(rcClient, m_crBlock);
	pDc->SetBkMode(TRANSPARENT);

	COLORREF crOldColor = pDc->GetTextColor();
	for( int i = 0; i < GetItemCount(); i++ )
	{
		tabItem* pItem = GetItem(i);
		if( pItem != NULL )
		{
			pItem->DrawItem(pDc, m_crActive, m_crText, m_crMove);
		}
	}
	pDc->SetTextColor(crOldColor);

	pDc->SelectObject(hOldObj);
	ReleaseDC(pDc);
	return TRUE;
}

void CTabCtrlExch::OnLButtonUp(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonUp(nFlags, point);

	MousePress(point);
}

void CTabCtrlExch::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
	MouseMove(point);
}

BOOL CTabCtrlExch::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
	MessageBox("sfds");
}

BOOL CTabCtrlExch::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	//::SetCursor(::LoadCursor(0, IDC_CROSS));
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CTabCtrlExch::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	CWnd::OnNcMouseMove(nHitTest, point);
}

void CTabCtrlExch::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	SetSize(CRect(0,0,0,0));
}
