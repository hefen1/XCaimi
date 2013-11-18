#include "StdAfx.h"
#include "HSScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHSScrollBar::CHSScrollBar(DWORD Style)
{
	m_rect.SetRectEmpty();
	m_rcUpArrow.SetRectEmpty();
	m_rcDownArrow.SetRectEmpty();
	m_rcBlock.SetRectEmpty();

	m_iColor  = theFoundationApp.m_iColor;
	m_nStyle = Style;

	m_nScrollRange = 1;
	m_nRange = 1;
	m_nPosition = 0;
	m_nPrePosition = -1;
	m_dUserDiff = 0.0;
	m_iNotify = NULL;
	m_nSize = HSSB_SIZE;
	m_bAutoSize = TRUE;
	Reset();
	ReadFile();
}

CHSScrollBar::~CHSScrollBar(void)
{
}

BOOL CHSScrollBar::ReadFile()
{
	m_logpen.lopnStyle = PS_SOLID;
	m_logpen.lopnWidth.x = 1;
	m_logpen.lopnWidth.y = 1;
	if (m_iColor)
	{
		m_logpen.lHorColor = m_iColor->GetRTableHor();
		m_logpen.lVerColor = m_iColor->GetRTableVer();
		m_clrBK = m_iColor->GetBlackgroundColor();
	}
	else
	{
		m_logpen.lHorColor = RGB(255,0,0);
		m_logpen.lVerColor = RGB(255,0,0);
		m_clrBK = RGB(0,0,0);
	}
	
	m_clrBlock = RGB(45,45,45);
	m_clrArrow = RGB(255,0,0);
	return TRUE;
}

CRect CHSScrollBar::SetRect(CRect rc)
{
	if (m_nStyle & HSSB_VERT)
	{// 滚动条垂直

		m_rect = rc;
		if (m_nStyle & HSSB_LEFT)
		{// 左边
			m_rect.right = m_rect.left + m_nSize;
			rc.left = m_rect.right;
		}
		else if(m_nStyle & HSSB_RIGHT)
		{// 右边
			m_rect.left = m_rect.right - m_nSize;
			rc.right = m_rect.left;
		}

		m_rcUpArrow   = m_rect;
		m_rcDownArrow = m_rect;
		if (m_rect.Height() < 2 * m_nSize)
		{
			m_rcUpArrow.bottom = m_rect.top + m_rect.Height()/2; 
			m_rcDownArrow.top = m_rcUpArrow.bottom;
		}
		else
		{
			m_rcUpArrow.bottom = m_rect.top + m_nSize;
			m_rcDownArrow.top  = m_rect.bottom - m_nSize;
		}
		// 去除边框的大小
		int nWidth = m_logpen.lopnWidth.x;
		m_rcDownArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
		m_rcUpArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
	}
	else if(m_nStyle & HSSB_HORZ)
	{// 滚动条水平

		m_rect = rc;
		if (m_nStyle & HSSB_UP)
		{// 上面
			m_rect.bottom = m_rect.top + m_nSize;
			rc.top = m_rect.bottom;
		}
		else if(m_nStyle & HSSB_RIGHT)
		{// 下面
			m_rect.top = m_rect.bottom - m_nSize;
			rc.bottom = m_rect.top;
		}

		m_rcUpArrow   = m_rect;
		m_rcDownArrow = m_rect;
		if (m_rect.Width() < 2 * m_nSize)
		{
			m_rcUpArrow.right  = m_rect.left + m_rect.Width()/2; 
			m_rcDownArrow.left = m_rcUpArrow.right;
		}
		else
		{
			m_rcUpArrow.right  = m_rect.left + m_nSize;
			m_rcDownArrow.left = m_rect.right - m_nSize;
		}
		// 去除边框的大小
		int nWidth = m_logpen.lopnWidth.x;
		m_rcDownArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
		m_rcUpArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
	}

	return rc;
}

CRect CHSScrollBar::SetRect(CRect rc, int nRange)
{
	m_nRange = nRange;
	if (m_nRange <= 0)
	{// 滚动条不需要显示
		m_rect.SetRectEmpty();
		m_rcUpArrow.SetRectEmpty();
		m_rcDownArrow.SetRectEmpty();
		m_rcBlock.SetRectEmpty();
	}
	else
	{// 滚动条需要显示

		m_rect = rc;
		//////////////////////////////////////////////////////////////////////////
		// 计算滚动条大小 上下箭头大小
		if (m_nStyle & HSSB_VERT)
		{// 滚动条垂直
			if (m_nStyle & HSSB_LEFT)
			{// 左边
				m_rect.right = m_rect.left + m_nSize;
				rc.left = m_rect.right;
			}
			else if(m_nStyle & HSSB_RIGHT)
			{// 右边
				m_rect.left = m_rect.right - m_nSize;
				rc.right = m_rect.left;
			}

			m_rcUpArrow   = m_rect;
			m_rcDownArrow = m_rect;
			if (m_rect.Height() < 2 * m_nSize)
			{
				m_rcUpArrow.bottom = m_rect.top + m_rect.Height()/2; 
				m_rcDownArrow.top = m_rcUpArrow.bottom;
			}
			else
			{
				m_rcUpArrow.bottom = m_rect.top + m_nSize;
				m_rcDownArrow.top  = m_rect.bottom - m_nSize;
			}
			// 去除边框的大小
			int nWidth = m_logpen.lopnWidth.x;
			m_rcDownArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
			m_rcUpArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
		}
		else if(m_nStyle & HSSB_HORZ)
		{// 滚动条水平
			if (m_nStyle & HSSB_UP)
			{// 上面
				m_rect.bottom = m_rect.top + m_nSize;
				rc.top = m_rect.bottom;
			}
			else if(m_nStyle & HSSB_RIGHT)
			{// 下面
				m_rect.top = m_rect.bottom - m_nSize;
				rc.bottom = m_rect.top;
			}

			m_rcUpArrow   = m_rect;
			m_rcDownArrow = m_rect;
			if (m_rect.Width() < 2 * m_nSize)
			{
				m_rcUpArrow.right  = m_rect.left + m_rect.Width()/2; 
				m_rcDownArrow.left = m_rcUpArrow.right;
			}
			else
			{
				m_rcUpArrow.right  = m_rect.left + m_nSize;
				m_rcDownArrow.left = m_rect.right - m_nSize;
			}
			// 去除边框的大小
			int nWidth = m_logpen.lopnWidth.x;
			m_rcDownArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
			m_rcUpArrow.DeflateRect(nWidth, nWidth, nWidth, nWidth);
		}
		//////////////////////////////////////////////////////////////////////////
		// 计算滑块大小 以及滑动的像素
		Reset();	
		int nPreScrollRange = m_nScrollRange;
		m_rcBlock = m_rect;
		if (m_nStyle & HSSB_VERT)
		{
			int nBlock = CalcBlock(m_rcDownArrow.top - m_rcUpArrow.bottom); // 滑块可以滑动的区域
			m_rcBlock.top = m_rcUpArrow.bottom;
			m_rcBlock.bottom = m_rcBlock.top + nBlock;
		}
		else if (m_nStyle & HSSB_HORZ)
		{
			int nBlock = CalcBlock(m_rcDownArrow.left - m_rcUpArrow.right);
			m_rcBlock.left = m_rcBlock.right;
			m_rcBlock.right = m_rcBlock.left + nBlock;
		}
		int nWidth = m_logpen.lopnWidth.x;
		m_rcBlock.DeflateRect(nWidth, nWidth, nWidth, nWidth);

		if (nPreScrollRange != 0 && nPreScrollRange != m_nScrollRange)
		{// 设置滑块位置

			m_nPosition = m_nPosition * m_nScrollRange / nPreScrollRange;
			int nTablePosition = m_nPosition*m_nScrollDiff;
			if (nTablePosition < 0)
			{
				nTablePosition = 0;
			}
			else if (nTablePosition > m_nRange)
			{
				nTablePosition = m_nRange;
			}
			m_iNotify->SetPosition(nTablePosition, (m_nStyle & HSSB_VERT), FALSE);
		}
	}
	return rc;
}

CRect CHSScrollBar::GetRect()
{
	return m_rect;
}

void  CHSScrollBar::SetSize(int nSize)
{
	if (nSize > 0)
	{
		m_nSize = nSize;
	}
}

BOOL CHSScrollBar::SetRange( int nRange )
{
	BOOL bIsNeedReSize = FALSE;
	if ( (nRange <= 0 && m_nRange > 0) 
	   || (nRange > 0 && m_nRange <= 0)
		)
	{// 当滚动条需要隐藏 或者 出现时 外部需要重新调整大小
		bIsNeedReSize = TRUE;
	}

	m_nRange = nRange;
//	if (m_nRange > 0)
	{			
		Reset();	
		int nPreScrollRange = m_nScrollRange;
		m_rcBlock = m_rect;
		if (m_nStyle & HSSB_VERT)
		{
			int nBlock = CalcBlock(m_rcDownArrow.top - m_rcUpArrow.bottom); // 滑块可以滑动的区域
			m_rcBlock.top = m_rcUpArrow.bottom;
			m_rcBlock.bottom = m_rcBlock.top + nBlock;
		}
		else if (m_nStyle & HSSB_HORZ)
		{
			int nBlock = CalcBlock(m_rcDownArrow.left - m_rcUpArrow.right);
			m_rcBlock.left = m_rcBlock.right;
			m_rcBlock.right = m_rcBlock.left + nBlock;
		}
		int nWidth = m_logpen.lopnWidth.x;
		m_rcBlock.DeflateRect(nWidth, nWidth, nWidth, nWidth);

		if (nPreScrollRange != 0)
		{// 设置滑块位置

			m_nPosition = m_nPosition * m_nScrollRange / nPreScrollRange;
			int nTablePosition = m_nPosition*m_nScrollDiff;
			if (nTablePosition < 0)
			{
				nTablePosition = 0;
			}
			else if (nTablePosition > m_nRange)
			{
				nTablePosition = m_nRange;
			}
			m_iNotify->SetPosition(nTablePosition, (m_nStyle & HSSB_VERT), FALSE);
		}
	}
// 	else
// 	{
// 		m_nRange = 0;
// 		m_rect.SetRectEmpty();
// 		m_rcUpArrow.SetRectEmpty();
// 		m_rcDownArrow.SetRectEmpty();
// 		m_rcBlock.SetRectEmpty();
// 	}

	return bIsNeedReSize;	
} 

int CHSScrollBar::CalcBlock( int nMaxSize )
{
	if (nMaxSize <= HSSB_BLOCK_MINSIZE)
	{
		return 0;
	}

	m_nScrollRange = m_nRange;
	int nNeed = m_nMoveDistance * m_nRange;
	while (nNeed >= (nMaxSize - HSSB_BLOCK_MINSIZE) && m_nMoveDistance > 0)
	{// 滑动范围要预留滑块的最小尺寸， 每次移动距离不少为0
		m_nMoveDistance--;
		nNeed = m_nMoveDistance * m_nRange; 
	}

	int nBlock = nMaxSize - nNeed;
	if (m_nMoveDistance == 0)
	{
		m_nMoveDistance = 1;
		nNeed = m_nRange;
		while (nNeed >= (nMaxSize - nBlock) && (nBlock >= HSSB_BLOCK_MINSIZE))
		{// 减少滑块尺寸 
			nBlock--;
		}

		if (nBlock < HSSB_BLOCK_MINSIZE)
		{// 滑块尺寸小于最小尺寸 只有增加移动一次滚动的次数
			nBlock = HSSB_BLOCK_MINSIZE;
			m_nScrollDiff++;
			nNeed = m_nRange / m_nScrollDiff + ((m_nRange%m_nScrollDiff)==0?0:1);
			while (nNeed >= (nMaxSize - nBlock) && (nNeed > 1))
			{
				m_nScrollDiff++;
				nNeed = m_nRange / m_nScrollDiff + ((m_nRange%m_nScrollDiff)==0?0:1);
			}
		}
		m_nScrollRange = nNeed;
		nBlock = nMaxSize - m_nScrollRange;
	}

	return nBlock;
}
void CHSScrollBar::SetBlockToDefaultPosition()
{
	if (m_nStyle & HSSB_BLOCK_ALWAYSTOP)
	{// 滑块在顶部
		m_nPosition = 0;
	}
	else if (m_nStyle & HSSB_BLOCK_ALWAYBOTTOM)
	{// 滑块在底部
		m_nPosition = m_nScrollRange;
	}

	int nTablePosition = m_nPosition*m_nScrollDiff;
	if (nTablePosition < 0)
	{
		nTablePosition = 0;
	}
	else if (nTablePosition > m_nRange)
	{
		nTablePosition = m_nRange;
	}
	m_iNotify->SetPosition(nTablePosition, (m_nStyle & HSSB_VERT), FALSE);

}
void CHSScrollBar::Draw( CDC* pDC )
{
	if (m_rect.IsRectEmpty())
	{
		return;
	}

	pDC->FillSolidRect(&m_rect, m_clrBK);

	DrawBorder(pDC);
	DrawArrow(pDC);
	DrawBlock(pDC);
}

void CHSScrollBar::DrawBorder( CDC* pDC )
{
	CPen Pen;
	Pen.CreatePenIndirect(&m_logpen.GetPen(HsLOGPEN::hor));
	CPen* pOldPen;
	pOldPen = pDC->SelectObject(&Pen);

	// 画上边框
	pDC->MoveTo(m_rect.left,  m_rect.top);
	pDC->LineTo(m_rect.right, m_rect.top);

	// 画下边框
	pDC->MoveTo(m_rect.left,  m_rect.bottom);
	pDC->LineTo(m_rect.right, m_rect.bottom);

	Pen.DeleteObject();
	Pen.CreatePenIndirect(&m_logpen.GetPen(HsLOGPEN::ver));
	pDC->SelectObject(&Pen);

	// 画左边框
	pDC->MoveTo(m_rect.left, m_rect.top);
	pDC->LineTo(m_rect.left, m_rect.bottom);

	// 画右边框
	pDC->MoveTo(m_rect.right, m_rect.top);
	pDC->LineTo(m_rect.right, m_rect.bottom);

	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
}

void CHSScrollBar::DrawArrow( CDC* pDC )
{
	// 填充背景
	pDC->FillSolidRect(&m_rcUpArrow, m_clrBlock);
	pDC->FillSolidRect(&m_rcDownArrow, m_clrBlock);

	CPen  pen;
	CPen* pOldPen;
	pen.CreatePen(PS_SOLID, 1, m_clrArrow);
	pOldPen = pDC->SelectObject(&pen);

	POINT arrow[3];
	if (m_nStyle & HSSB_VERT)
	{// 画上，下箭头
		
		// 上箭头
		arrow[0].y = m_rcUpArrow.top   + (m_rcUpArrow.Height() >> 2);
		arrow[1].x = m_rcUpArrow.left  + m_rcUpArrow.Width() / 4;
		arrow[1].y = m_rcUpArrow.top   + m_rcUpArrow.Height() * 3 / 4;
		arrow[2].x = m_rcUpArrow.right - m_rcUpArrow.Width() / 4;
		arrow[2].y = arrow[1].y;
		arrow[0].x = (arrow[2].x + arrow[1].x) >> 1;
		pDC->Polygon(arrow, 3);
		
		// 画下边框
		pDC->MoveTo(m_rcUpArrow.left, m_rcUpArrow.bottom);
		pDC->LineTo(m_rcUpArrow.right,m_rcUpArrow.bottom);

		// 下箭头
		arrow[0].y = m_rcDownArrow.top   + m_rcDownArrow.Height() * 3 / 4;
		arrow[1].x = m_rcDownArrow.left  + m_rcDownArrow.Width() / 4;
		arrow[1].y = m_rcDownArrow.top   + (m_rcDownArrow.Height() >> 2);
		arrow[2].x = m_rcDownArrow.right - m_rcDownArrow.Width() / 4;
		arrow[2].y = arrow[1].y;
		arrow[0].x = (arrow[2].x + arrow[1].x) >> 1;
		pDC->Polygon(arrow, 3);

		// 画上边框
		pDC->MoveTo(m_rcDownArrow.left, m_rcDownArrow.top);
		pDC->LineTo(m_rcDownArrow.right,m_rcDownArrow.top);
	}
	else if (m_nStyle & HSSB_HORZ)
	{
		// 左箭头
		arrow[0].x = m_rcUpArrow.left   + (m_rcUpArrow.Width() >> 2);
		arrow[1].y = m_rcUpArrow.top    + m_rcUpArrow.Height() / 4;
		arrow[1].x = m_rcUpArrow.left   + m_rcUpArrow.Width() * 3 / 4;
		arrow[2].y = m_rcUpArrow.bottom - m_rcUpArrow.Height() / 4;
		arrow[2].x = arrow[1].x;
		arrow[0].y = (arrow[2].y + arrow[1].y) >> 1;
		pDC->Polygon(arrow, 3);

		// 画左边框
		pDC->MoveTo(m_rcUpArrow.right, m_rcUpArrow.top);
		pDC->LineTo(m_rcUpArrow.right, m_rcUpArrow.bottom);

		// 右箭头
		arrow[0].x = m_rcDownArrow.left   + m_rcDownArrow.Width() * 3 / 4;
		arrow[1].y = m_rcDownArrow.top    + m_rcDownArrow.Height() / 4;
		arrow[1].x = m_rcDownArrow.left   + (m_rcDownArrow.Width() >> 2);
		arrow[2].y = m_rcDownArrow.bottom - m_rcDownArrow.Height() / 4;
		arrow[2].x = arrow[1].x;
		arrow[0].y = (arrow[2].y + arrow[1].y) >> 1;
		pDC->Polygon(arrow, 3);

		// 画右边框
		pDC->MoveTo(m_rcDownArrow.left, m_rcDownArrow.top);
		pDC->LineTo(m_rcDownArrow.left, m_rcDownArrow.bottom);
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CHSScrollBar::DrawBlock(CDC* pDC)
{
	SetBlockRect(); // 获得滑块当前位置

	pDC->FillSolidRect(m_rcBlock, m_clrBlock);
}

BOOL CHSScrollBar::MouseDown( UINT nFlags, CPoint point )
{
	HitPlace eHit = GetHitPlace(point);
	if (eHit == NONE)
		return FALSE;
	else
	{
		MouseEvent(eHit, point);
		return TRUE;
	}
}

void CHSScrollBar::MouseEvent(HitPlace eHit, CPoint point)
{
	if (m_iNotify == NULL || ::IsBadReadPtr(m_iNotify, 1))
	{
		return;
	}

	SetCapture(m_iNotify->GetHandle());	

	CPoint  pt(0,0);
	CPoint  prePt = point;
	int     xDiff, yDiff;
	xDiff = yDiff = 0;
	
	DWORD nStart = ::GetTickCount();
	DWORD nEnd   = -1;
	for (;;) 
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));


		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			{
				if(!(msg.wParam & MK_LBUTTON) || eHit != Block)
					goto ExitLoop;
				pt.x = (int)(short)LOWORD(msg.lParam);
				pt.y = (int)(short)HIWORD(msg.lParam);

				xDiff = pt.x - prePt.x;
				yDiff = pt.y - prePt.y;

				if (m_nStyle & HSSB_VERT)
				{
					if (abs(yDiff) >= m_nMoveDistance)
					{
						prePt = pt;
						int nDiff = (int)(((double)(abs(yDiff)/m_nMoveDistance)*10 + 5) / 10); // 四舍五入 尽量使鼠标移动与滑块移动同步
						nDiff = (yDiff<0)?nDiff*(-1):nDiff;
						Scrolling(nDiff);
					}
				}
				else if (m_nStyle & HSSB_HORZ)
				{
					if ( abs(xDiff) >= m_nMoveDistance)
					{
						prePt = pt;
						int nDiff = (int)(((double)(abs(xDiff)/m_nMoveDistance)*10 + 5) / 10); // 四舍五入 尽量使鼠标移动与滑块移动同步
						nDiff = (xDiff<0)?nDiff*(-1):nDiff;
						Scrolling(nDiff);
					}
				}
				//eHit = GetHitPlace(pt);
			}
			break;
		case WM_KEYDOWN:
			if (msg.wParam == VK_ESCAPE)
			{
			}
			break;
		case WM_LBUTTONDBLCLK:
			{
			}
			break;
		case WM_LBUTTONDOWN:
			{
			}
		case WM_LBUTTONUP:
			goto ExitLoop;
		case WM_RBUTTONUP:
			goto ExitLoop;
		default:
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			break;
		}

		DWORD nEnd = ::GetTickCount();
		if (eHit != Block)
		{
			if (nStart - nEnd > 800)
			{
				int nDiff = 0;
				if (eHit == Up)
				{
					nDiff = -1;
				}
				else if (eHit == Down)
				{
					nDiff = 1;
				}
				else if (eHit == UpBlank)
				{
					nDiff = -3;
				}
				else if (eHit == DownBlank)
				{
					nDiff = 3;
				}
				Scrolling(nDiff);
				nStart = nEnd;
				eHit = GetHitPlace(point);
				TRACE1("eHit = %d \n", eHit);
			}
		}
		
	}
ExitLoop:
	ReleaseCapture();
}

BOOL CHSScrollBar::SetBlockRect()
{
	if (m_nPosition < 0 || m_nPosition > m_nScrollRange)
	{
		return FALSE;
	}

	int nSize = 0;
	if (m_nStyle & HSSB_HORZ)
	{
		nSize = m_rcBlock.Width();

		m_rcBlock.left = m_rcUpArrow.right + m_nPosition*m_nMoveDistance + 1;
		m_rcBlock.right = m_rcBlock.left + nSize ;
	}
	else if (m_nStyle & HSSB_VERT)
	{
		nSize = m_rcBlock.Height();

		m_rcBlock.top = m_rcUpArrow.bottom + m_nPosition*m_nMoveDistance + 1;
		m_rcBlock.bottom = m_rcBlock.top + nSize;
	}
	
	return TRUE;
}

void  CHSScrollBar::OnScroll(int nMoveDiff)
{
	if (nMoveDiff == 0 || m_rect.IsRectEmpty())
	{
		return;
	}

	// 外部传入的移动距离需要
	int nDiff = nMoveDiff / m_nScrollDiff;
	if (nDiff == 0)
	{
		// 累积所有小于0的位移
		m_dUserDiff += (double)nMoveDiff / m_nScrollDiff;
		if (abs(m_dUserDiff) > 1.0)
		{
			nDiff = (int)m_dUserDiff;
			m_dUserDiff -= nDiff;
		}
	}
	else
	{
		m_dUserDiff = 0.0;
	}
	
	// 将位置移动到合理的位置
	int nPosition = m_nPosition + nDiff;
	if (nPosition < 0)
	{ 
		nPosition = 0;
	}
	else if (nPosition > m_nScrollRange)
	{
		nPosition = m_nScrollRange;
	}

	m_nPosition = nPosition;
	if (SetBlockRect() && m_iNotify)
	{
		m_iNotify->ScrollInvalidate(&m_rect, FALSE);
	}
}

void CHSScrollBar::SetTop()
{
	m_nPosition = 0;
	if (SetBlockRect() && m_iNotify)
	{
		m_iNotify->ScrollInvalidate(&m_rect, FALSE);
	}
}

void CHSScrollBar::SetBottom()
{
	m_nPosition = m_nScrollRange;
	if (SetBlockRect() && m_iNotify)
	{
		m_iNotify->ScrollInvalidate(&m_rect, FALSE);
	}
}

void CHSScrollBar::Scrolling(int nDiff)
{
	if (m_rect.IsRectEmpty())
	{
		return ;
	}

	// 将位置移动到合理的位置
	int nPosition = m_nPosition + nDiff;
	if (nPosition < 0)
	{ 
		nPosition = 0;
	}
	else if (nPosition > m_nScrollRange)
	{
		nPosition = m_nScrollRange;
	}
//	TRACE("nDiff = %d \n", nDiff);
//	TRACE2("nPosition = %d, m_nScrollRange = %d \n", nPosition, m_nScrollRange);
	if (nPosition == m_nPosition)
	{// 位置没有改变
		return ;
	}

	m_nPosition = nPosition;
	if (SetBlockRect() && m_iNotify)
	{
		m_iNotify->ScrollInvalidate(&m_rect, TRUE);

		if (m_nStyle & HSSB_VERT)
		{
			m_iNotify->SetPosition(nPosition*m_nScrollDiff, TRUE);
			//m_iNotify->OnScroll(nDiff*m_nScrollDiff, TRUE);
		}
		else if (m_nStyle & HSSB_HORZ)
		{
			m_iNotify->SetPosition(nPosition*m_nScrollDiff, FALSE);
			//m_iNotify->OnScroll(nDiff*m_nScrollDiff, FALSE);
		}
	}	
}

CHSScrollBar::HitPlace CHSScrollBar::GetHitPlace( CPoint pt )
{
	HitPlace eHit = NONE;
	if (m_rect.PtInRect(pt))
	{
		if (m_rcUpArrow.PtInRect(pt))
		{
			eHit = Up;
		}
		else if (m_rcDownArrow.PtInRect(pt))
		{
			eHit = Down;
		}
		else if (m_rcBlock.PtInRect(pt))
		{
			eHit = Block;
		}
		else
		{
			if (pt.y < m_rcBlock.top && (m_nStyle & HSSB_VERT)
				|| 
				pt.x < m_rcBlock.left && (m_nStyle & HSSB_HORZ)
				)
				eHit = UpBlank;
			else
				eHit = DownBlank;
		}
	}

	return eHit;
}

void CHSScrollBar::Reset()
{
	m_nScrollDiff = 1;
	m_nMoveDistance = 3;
}
