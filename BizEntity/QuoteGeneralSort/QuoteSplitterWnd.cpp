#include "stdafx.h"
#include "QuoteSplitterWnd.h"

IMPLEMENT_DYNAMIC(CQuoteSplitterWnd,CSplitterWnd)


CQuoteSplitterWnd::CQuoteSplitterWnd()
{
		m_bkColor = RGB(0,0,0); 
		m_lineColor = RGB(255,0,0); 
 		m_cxSplitter = m_cySplitter = 10;			//size of splitter bar
 		m_cxBorderShare = m_cyBorderShare = 0;        // space on either side of splitte
 		m_cxSplitterGap = m_cySplitterGap =m_cySplitter;      // amount of space between panes
 		m_cxBorder = m_cyBorder = 0;               // borders in client area
	          
}

CQuoteSplitterWnd::~CQuoteSplitterWnd()
{

}

BEGIN_MESSAGE_MAP(CQuoteSplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN() 
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE() 
	ON_WM_SETCURSOR()  
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CQuoteSplitterWnd::HideSplitter()
{
 
}

void CQuoteSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)   
{   
	if(pDC == NULL)   
	{   
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);   
		return;   
	}   
	ASSERT_VALID(pDC);   
	CRect rect = rectArg;   
	switch (nType)   
	{   
	case splitBorder:   //»­±ß¿ò
    /*	pDC->Draw3dRect(rect, 0, 0);  */
       		rect.InflateRect(0,0,2,2);
        	pDC->Draw3dRect(rect, m_lineColor, m_lineColor);  
		return;

	case splitIntersection:   
		
		break;   

	case splitBox:   
		break;   
	case splitBar:   //»­·Ö¸ôÌõ
	/*	rect.InflateRect(5,5);*/
 	/*	pDC->Draw3dRect(rect, m_lineColor, m_bkColor);  */
		break;   

	default:   
		ASSERT(FALSE);  
	}   

	// fill the middle    
   	COLORREF clr = m_lineColor;   
	rect.InflateRect(0,0,5,5);
   	pDC->FillSolidRect(rect, clr);   

    	CPen hPen;
    	hPen.CreatePen(PS_SOLID ,2,RGB(255,0,0));
    	CPen* hOldPen = pDC->SelectObject(&hPen);
    
    	if (abs(rectArg.right-rectArg.left)>abs(rectArg.bottom-rectArg.top))
    	{
    
    		pDC->MoveTo(rectArg.left,rectArg.top+m_cxSplitter/2);
    		pDC->LineTo(rectArg.right,rectArg.top+m_cxSplitter/2);
    	}
    	else{
    
    		pDC->MoveTo(rectArg.left+m_cxSplitter/2,rectArg.top);
    		pDC->LineTo(rectArg.left+m_cxSplitter/2,rectArg.bottom);
    	}
    
    	pDC->SelectObject(hOldPen);
    	hPen.DeleteObject();
}   


void CQuoteSplitterWnd::OnPaint()
{
	ASSERT_VALID(this);
	CPaintDC dc(this);

	CRect rectClient;
	GetClientRect(&rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	dc.FillSolidRect(rectClient, m_bkColor);
	CRect rectInside;
	GetInsideRect(rectInside);

	// draw the splitter boxes
	if (m_bHasVScroll && m_nRows < m_nMaxRows)
	{
		OnDrawSplitter(&dc, splitBox,
			CRect(rectInside.right /*+ afxData.bNotWin4*/, rectClient.top,
				rectClient.right, rectClient.top + m_cySplitter));
	}

	if (m_bHasHScroll && m_nCols < m_nMaxCols)
	{
		OnDrawSplitter(&dc, splitBox,
			CRect(rectClient.left, rectInside.bottom /*+ afxData.bNotWin4*/,
				rectClient.left + m_cxSplitter, rectClient.bottom));
	}

	// extend split bars to window border (past margins)
	DrawAllSplitBars(&dc, rectInside.right, rectInside.bottom);

	if (1/*!afxData.bWin4*/)
	{
		// draw splitter intersections (inside only)
		GetInsideRect(rectInside);
		dc.IntersectClipRect(rectInside);
		CRect rect;
		rect.top = rectInside.top;
		for (int row = 0; row < m_nRows - 1; row++)
		{
			rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
			rect.bottom = rect.top + m_cySplitter;
			rect.left = rectInside.left;
			for (int col = 0; col < m_nCols - 1; col++)
			{
				rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
				rect.right = rect.left + m_cxSplitter;
				OnDrawSplitter(&dc, splitIntersection, rect);
				rect.left = rect.right + m_cxBorderShare;
			}
			rect.top = rect.bottom + m_cxBorderShare;
		}
	}

}

void CQuoteSplitterWnd::OnInvertTracker(CRect &rect) 
{                   
	ASSERT_VALID(this); 
	ASSERT(!rect.IsRectEmpty()); 
	ASSERT((GetStyle()&WS_CLIPCHILDREN)==0);
	CRect rc=rect;  
	rc.InflateRect(2,2);
	CDC* pDC=GetDC(); 
	CBrush* pBrush = CDC::GetHalftoneBrush();
/*	pBrush->CreateSolidBrush(RGB(255,0,0));*/
	HBRUSH hOldBrush=NULL; 
	if(pBrush!=NULL) hOldBrush=(HBRUSH)SelectObject(pDC->m_hDC,pBrush->m_hObject);
	pDC->PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),/*BLACKNESS*/PATCOPY);  
	if(hOldBrush!=NULL)
	SelectObject(pDC->m_hDC,hOldBrush);
	ReleaseDC(pDC); 
} 



void CQuoteSplitterWnd::OnMouseMove(UINT /*nFlags*/, CPoint pt)
{
	return;
}

void CQuoteSplitterWnd::OnLButtonDown(UINT nFlags, CPoint pt)   
{   
	CSplitterWnd::OnLButtonDown(nFlags, pt);
}   

void CQuoteSplitterWnd::OnLButtonUp(UINT /*nFlags*/, CPoint /*pt*/)
{
	return;
}
 BOOL CQuoteSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
 	return TRUE;
}

 BOOL CQuoteSplitterWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{ 
      if (CWnd::OnCommand(wParam, lParam)) 
              return TRUE; 

	
      return GetParent()->GetParent()->SendMessage(WM_COMMAND, wParam, lParam); 
          
}
BOOL CQuoteSplitterWnd::OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult )
{
      if (CWnd::OnNotify(wParam, lParam, pResult)) 
              return TRUE; 

      *pResult = GetParent()->GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);
      return TRUE;
} 
          
BOOL CQuoteSplitterWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{ 

      return CWnd::OnWndMsg(message, wParam, lParam, pResult); 
          
}