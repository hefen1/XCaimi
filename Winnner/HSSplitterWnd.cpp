// HSSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "HSSplitterWnd.h"

#define CX_BORDER   1   
#define CY_BORDER   1  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHSSplitterWnd

IMPLEMENT_DYNAMIC(CHSSplitterWnd, CSplitterWnd)

CHSSplitterWnd::CHSSplitterWnd():m_nHiddenCol(-1),m_nHiddenRow(-1)
{
	m_bSpliting = FALSE;
	m_bHasSpliter = TRUE;
	m_cxSplitter = m_cySplitter = 1;   
	m_cxBorderShare = m_cyBorderShare = 0;   
	m_cxSplitterGap = m_cySplitterGap = m_cxSplitter > 0 ? m_cxSplitter : 1;  
	m_cxBorder = m_cyBorder = 0;   
	m_bkColor = RGB(155,155,155);   
	m_lineColor = RGB(212,208,200);   
	m_bFixed = FALSE;   
}

CHSSplitterWnd::~CHSSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CHSSplitterWnd, CSplitterWnd)
	ON_WM_LBUTTONDOWN() 
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE() 
END_MESSAGE_MAP()



// CHSSplitterWnd 消息处理程序
void CHSSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)   
{
	// Let CSplitterWnd handle everything but the border-drawing   
	if(pDC == NULL)   
	{   
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);   
		return;   
	}   
	ASSERT_VALID(pDC);   
	CRect rect = rectArg;   
	switch (nType)   
	{   
	case splitBorder:   
		/*pDC->Draw3dRect(rect, m_lineColor, m_bkColor);   
		rect.InflateRect(-1, -1);   
		pDC->Draw3dRect(rect, m_bkColor, m_bkColor);  */ 
		return;   

	case splitIntersection:   
		break;   

	case splitBox:     
		/*pDC->Draw3dRect(rect, m_bkColor, m_bkColor);   
		rect.InflateRect(-1, -1);   
		pDC->Draw3dRect(rect, m_bkColor, m_bkColor);   
		rect.InflateRect(-1, -1); */  
		break;   
		// fall through...   
	case splitBar:   
		//rect.InflateRect(-1, -1);  
		//pDC->Draw3dRect(rect, m_bkColor,m_lineColor);    
		break;   

	default:   
		ASSERT(FALSE);  // unknown splitter type   
	}   

	// fill the middle    
	COLORREF clr = m_bkColor;   
	pDC->FillSolidRect(rect, clr);   
}   

void CHSSplitterWnd::OnMouseMove(UINT /*nFlags*/, CPoint pt)
{
	if(!HasSplieter())
		return;
	CSplitterWnd::OnMouseMove(NULL,pt);
}

void CHSSplitterWnd::OnLButtonDown(UINT /*nFlags*/, CPoint pt)   
{   
	if(!HasSplieter())
		return;

	if (m_bTracking)   
		return;   
	int row = HitTest(pt) - 101;   
	if(row == 8)   
		return;   
	StartTracking(HitTest(pt));   
}   

void CHSSplitterWnd::OnLButtonUp(UINT /*nFlags*/, CPoint /*pt*/)
{
	StopTracking(TRUE);
}

void CHSSplitterWnd::TrackRowSize(int y, int row)   
{   
	ASSERT_VALID(this);   
	ASSERT(m_nRows > 1);   

	if(m_bFixed)   
		return;     

	CPoint pt(0, y);   

	ClientToScreen(&pt);   

	int iOldHeigh, iOldHeigh1;   
	iOldHeigh = m_pRowInfo[row].nIdealSize;    

	GetPane(row, 0)->ScreenToClient(&pt);   
	m_pRowInfo[row].nIdealSize = pt.y;      // new size   

	int delta = pt.y - iOldHeigh;   
	iOldHeigh1 = m_pRowInfo[row+1].nIdealSize;   

	int iNewHeigh1 = iOldHeigh1 - delta;   
	CPoint pt1(0,iNewHeigh1);   
	m_pRowInfo[row+1].nIdealSize = pt1.y;     

	if (pt.y < m_pRowInfo[row].nMinSize)   
	{   
		// resized too small   
		m_pRowInfo[row].nIdealSize = 0; // make it go away   
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)   
			DeleteRow(row);   
	}   
	else if (m_pRowInfo[row].nCurSize + m_pRowInfo[row+1].nCurSize   
		< pt.y + m_pRowInfo[row+1].nMinSize)   
	{   
		// not enough room for other pane   
		if (GetStyle() & SPLS_DYNAMIC_SPLIT)   
			DeleteRow(row + 1);   
	}   
}

BOOL CHSSplitterWnd::SplitWindow(SplitType st,CWnd* pParent,
								 CRuntimeClass* pClass1,CRuntimeClass* pClass2)
{
	m_bSpliting = TRUE;
	m_splitType = st;
	TRY 
	{
		CCreateContext pContext;
		memset(&pContext,0,sizeof(CCreateContext));

		if(st == XSplit)
		{
			CreateStatic(pParent, 2,1,WS_CHILD | WS_VISIBLE );
			CreateView(0,0,pClass1,CSize(0,0),&pContext);
			CreateView(1,0,pClass2,CSize(0,0),&pContext);
		}
		else
		{
			CreateStatic(pParent, 1,2,WS_CHILD | WS_VISIBLE );
			CreateView(0,0,pClass1,CSize(0,0),&pContext);
			CreateView(0,1,pClass2,CSize(0,0),&pContext);
		}
	}
	CATCH (CMemoryException, e)
	{
	}
	END_CATCH

	m_bSpliting = FALSE;
	return TRUE;
}


void CHSSplitterWnd::RecalcSize(CRect rect,int nPane1Size,int nPane2Size)
{
	if(m_bSpliting == TRUE)
		return;

	int nRowCount = GetRowCount();
	int nColCount = GetColumnCount();

	if(nRowCount <=0 && nColCount <= 0 )
		return;

	if(nPane1Size > 0 || nPane2Size > 0)
	{
		if(m_splitType == XSplit)
		{
			int nWidth = rect.Width();
			int nPane1Height = nPane1Size !=0 ? nPane1Size : rect.Height() - nPane2Size;
			nPane1Height = nPane1Height > rect.Height() ? rect.Height() : nPane1Height;
			int nPane2Height = rect.Height() - nPane1Height;
			SetColumnInfo(0, nWidth, 0);
			SetRowInfo(0,nPane1Height,0);
			SetRowInfo(1,nPane2Height,0);
		}
		else
		{
			int nHeight = rect.Height();
			int nPane1Width = nPane1Size !=0 ? nPane1Size : rect.Width() - nPane2Size;
			nPane1Width = nPane1Width > rect.Width() ? rect.Width() : nPane1Width;
			int nPane2Width = rect.Width() - nPane1Width;
			SetRowInfo(0, nHeight, 0);
			SetColumnInfo(0,nPane1Width,0);
			SetColumnInfo(1,nPane2Width,0);			
		}
	}
	else
	{
		int nRowHeight = rect.Height() / nRowCount;
		int nColWidth  = rect.Width()  / nColCount;
		for(short nRow = 0;nRow < nRowCount;nRow++)
		{
			SetRowInfo(nRow,nRowHeight,0);
			for(short nCol = 0;nCol < nColCount;nCol++)
			{
				SetColumnInfo(nCol, nColWidth, 0);
			}
		}
	}

	MoveWindow(rect);
	CSplitterWnd::RecalcLayout();
}

BOOL CHSSplitterWnd::IsSplited()
{
	return this->GetColumnCount() > 0 || this->GetRowCount() > 0;
}

BOOL CHSSplitterWnd::IsVerticalSplited()
{
	return m_bSpliting == YSplit;
}

void CHSSplitterWnd::RememberSize()
{
	if (IsVerticalSplited()) 
	{
		GetRowInfo(0,m_nPaneSize[0],m_nPaneMinSize[0]);
		GetRowInfo(1,m_nPaneSize[1],m_nPaneMinSize[1]);
	}
	else 
	{
		GetColumnInfo(0,m_nPaneSize[0],m_nPaneMinSize[0]);
		GetColumnInfo(1,m_nPaneSize[1],m_nPaneMinSize[1]);
	}
}

BOOL CHSSplitterWnd::HasSplieter()
{
	return m_bHasSpliter;
}

void CHSSplitterWnd::ShowColumn()
{
	ASSERT_VALID(this);
	ASSERT(m_nCols < m_nMaxCols);
	ASSERT(m_nHiddenCol != -1);

	int colNew = m_nHiddenCol;
	m_nHiddenCol = -1;
	m_nCols++;  // add a column
	ASSERT(m_nCols == m_nMaxCols);

	// fill the hidden column
	int col;
	for (int row = 0; row < m_nRows; row++)
	{
		CWnd* pPaneShow = GetDlgItem(
			AFX_IDW_PANE_FIRST + row * 16 + m_nCols);
		ASSERT(pPaneShow != NULL);
		pPaneShow->ShowWindow(SW_SHOWNA);

		for (col = m_nCols - 2; col >= colNew; col--)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row, col + 1));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowCol(row, colNew));
	}
	// new panes have been created -- recalculate layout
	RecalcLayout();
}

void CHSSplitterWnd::HideColumn(int colHide)
{
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);
	ASSERT(colHide < m_nCols);
	ASSERT(m_nHiddenCol == -1);
	if (m_nHiddenCol != -1) return;

	RememberSize();

	m_nHiddenCol = colHide;

	// if the column has an active window -- change it
	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL &&
		colActive == colHide)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all column panes
	for (int row = 0; row < m_nRows; row++)
	{
		CWnd* pPaneHide = GetPane(row, colHide);
		ASSERT(pPaneHide != NULL);
		pPaneHide->ShowWindow(SW_HIDE);
		pPaneHide->SetDlgCtrlID(
			AFX_IDW_PANE_FIRST + row * 16 + m_nCols);

		for (int col = colHide + 1; col < m_nCols; col++)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row, col - 1));
		}
	}
	m_nCols--;
	m_pColInfo[m_nCols].nCurSize = m_pColInfo[colHide].nCurSize;
	RecalcLayout();
}

void CHSSplitterWnd::ShowRow()
{
	ASSERT_VALID(this);
	ASSERT(m_nRows < m_nMaxRows);
	ASSERT(m_nHiddenRow != -1);

	int rowNew = m_nHiddenRow;
	m_nHiddenRow = -1;
	m_nRows++;  // add a row
	ASSERT(m_nRows == m_nMaxRows);

	// fill the hidden row
	int row;
	for (int col = 0; col < m_nCols; col++)
	{
		CWnd* pPaneShow = GetDlgItem(
			AFX_IDW_PANE_FIRST + m_nRows * 16 + col);
		ASSERT(pPaneShow != NULL);
		pPaneShow->ShowWindow(SW_SHOWNA);

		for (row = m_nRows - 2; row >= rowNew; row--)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row + 1, col));
		}

		pPaneShow->SetDlgCtrlID(IdFromRowCol(rowNew, col));
	}

	// new panes have been created -- recalculate layout
	RecalcLayout();
}

void CHSSplitterWnd::HideRow(int rowHide)
{
	ASSERT_VALID(this);
	ASSERT(m_nRows > 1);
	ASSERT(rowHide < m_nRows);
	ASSERT(m_nHiddenRow == -1);
	if (m_nHiddenRow != -1) return;

	RememberSize();

	m_nHiddenRow = rowHide;

	// if the column has an active window -- change it
	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL &&
		rowActive == rowHide)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}

	// hide all row panes
	for (int col = 0; col < m_nCols; col++)
	{
		CWnd* pPaneHide = GetPane(rowHide, col);
		ASSERT(pPaneHide != NULL);
		pPaneHide->ShowWindow(SW_HIDE);
		pPaneHide->SetDlgCtrlID(
			AFX_IDW_PANE_FIRST + m_nRows * 16);

		for (int row = rowHide + 1; row < m_nRows; row++)
		{
			CWnd* pPane = GetPane(row, col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(row - 1, col));
		}
	}
	m_nRows--;
	m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[rowHide].nCurSize;

	RecalcLayout();
}

BOOL CHSSplitterWnd::ToggleSide(int nIndex)
{
	ASSERT(nIndex >=0 && nIndex < 2);

	RememberSize();
	if (IsVerticalSplited())
	{
		if (m_nHiddenRow == -1) {
			//HideRow(nIndex);
		}
		else if (m_nHiddenRow == nIndex) {
			//ShowRow();
		}
	}
	else
	{
		if (m_nHiddenCol == -1) {
			HideColumn(nIndex);
		}
		else if (m_nHiddenCol == nIndex) {
			ShowColumn();
		}
	}
	return TRUE;
}

CWnd* CHSSplitterWnd::GetActivePane(int* pRow, int* pCol)
// return active view, NULL when no active view
{
	ASSERT_VALID(this);

	// attempt to use active view of frame window
	CWnd* pView = NULL;
	CFrameWnd* pFrameWnd = GetParentFrame();
	if(pFrameWnd)
		pView = pFrameWnd->GetActiveView();

	// failing that, use the current focus
	if (pView == NULL)
		pView = GetFocus();

	// make sure the pane is a child pane of the splitter
	if (pView != NULL && !IsChildPane(pView, pRow, pCol))
		pView = NULL;

	return pView;
}