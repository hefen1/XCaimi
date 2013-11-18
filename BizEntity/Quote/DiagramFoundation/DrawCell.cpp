#include "StdAfx.h"
#include "DrawCell.h"
#include "DrawTableBase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridLineArray
void CDrawCellArray::DeleteAll()
{
	CDrawCell* pCell;
	for(int i=0; i<m_nSize; i++)
	{
		pCell = (CDrawCell*)m_pData[i];
		if(pCell != NULL)
		{
			delete pCell;
		}
	} 
	RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
CDrawCell::CDrawCell(CArray<ItemConstructionStruct>* ayItemStruct, DWORD dwCellMask )
{
	if (ayItemStruct == NULL)
	{
		return;
	}

	// 画笔结构初始化
	m_logPen.lopnStyle = PS_SOLID;
	m_logPen.lopnWidth.x = 1;
	m_logPen.lopnWidth.y = 1;  
	ReadCfg();

	m_dwStyle = dwCellMask;
	m_nItemID = -1;
	m_wSignal = NEED_NOTHING;

	m_ayItem.SetSize(ayItemStruct->GetCount());
	CDrawItem* pItem = NULL;
	double dwWidth = 0.0;
	for (int i = 0; i < ayItemStruct->GetCount(); i++)
	{
		ItemConstructionStruct* pStruct = &ayItemStruct->GetAt(i);
		pItem = new CDrawItem(pStruct);
//		pItem = CDrawItem::Create(pStruct);
		dwWidth += pStruct->dWidth;
		m_ayItem[i] = pItem;
	}
	
	// 重置下最后一个单元小格的宽度
	pItem = m_ayItem[m_ayItem.GetCount() - 1];
	pItem->m_dWidth = 1.0 - (dwWidth - pItem->m_dWidth);
}

CDrawCell::CDrawCell(CString str, WORD lClr, WORD lFont, DWORD nMask, DWORD dwCellMask, DWORD dwExtent)
{
	// 画笔结构初始化
	m_logPen.lopnStyle = PS_SOLID;
	m_logPen.lopnWidth.x = 1;
	m_logPen.lopnWidth.y = 1;    
	ReadCfg();
	
	m_dwStyle = dwCellMask;
	m_nItemID = -1;
	m_wSignal = NEED_NOTHING;	

	CDrawItem* pItem = NULL;
	char strText[64];
	strncpy(strText, str.GetBuffer(), min(str.GetLength()+1, 63));
	pItem = new CDrawItem(strText, lClr, lFont, nMask, 1.0, dwExtent);
//	pItem = CDrawItem::Create(strText, lClr, lFont, nMask, 1.0, dwExtent);
	m_ayItem.Add(pItem);
}

CDrawCell::~CDrawCell(void)
{
}

void CDrawCell::ReadCfg()
{
	m_logPen.lHorColor = theFoundationApp.m_iColor->GetRTableHor();
	m_logPen.lVerColor = theFoundationApp.m_iColor->GetRTableVer();
}

void CDrawCell::ReCreate(CArray<ItemConstructionStruct>* pAyItemStruct, DWORD dwCellMask)
{
	m_dwStyle = dwCellMask;
	
	int nNewCount = pAyItemStruct->GetCount();
	int nOldCount = m_ayItem.GetCount();
	double dwWidth = 0.0;
	if (nOldCount <= nNewCount)
	{
		// 覆盖原有的内存空间内容
		for (int i = 0; i < nOldCount; i++)
		{
			CDrawItem* pItem = m_ayItem[i];
			ItemConstructionStruct* pStruct = &pAyItemStruct->GetAt(i);
			pItem->ReCreate(pStruct);
			dwWidth += pItem->m_dWidth;
		}
		// 申请新的内容空间
		for (int i = nOldCount; i < nNewCount; i++)
		{
			ItemConstructionStruct* pStruct = &pAyItemStruct->GetAt(i);
			CDrawItem* pItem = new CDrawItem(pStruct);	
//			CDrawItem* pItem = CDrawItem::Create(pStruct);
			m_ayItem.Add(pItem);
			dwWidth += pItem->m_dWidth;
		}
	}
	else 
	{
		// 覆盖原有的内存空间内容
		for (int i = 0; i < nNewCount; i++)
		{
			CDrawItem* pItem = m_ayItem[i];
			ItemConstructionStruct* pStruct = &pAyItemStruct->GetAt(i);
			pItem->ReCreate(pStruct);
			dwWidth += pItem->m_dWidth;
		}
		// 删除旧的内存空间
		for (int i = nNewCount; i < nOldCount; i++)
		{
			CDrawItem* pItem = m_ayItem[i];
//			CDrawItem::Free(pItem);
			delete pItem;
		}
		m_ayItem.RemoveAt(nNewCount, nOldCount-nNewCount);
	}
	// 重置下最后一个单元小格的宽度
	CDrawItem* pItem = m_ayItem[m_ayItem.GetCount() - 1];
	pItem->m_dWidth = 1.0 - (dwWidth - pItem->m_dWidth);
}

void CDrawCell::ReCreate(CString str, WORD lClr, WORD lFont, DWORD nMask, DWORD dwCellMask, DWORD dwExtent)
{
	m_dwStyle = dwCellMask;

	// 取第一项 如果创建成功 可以确保第一项一定存在
	CDrawItem* pItem = m_ayItem[0];
	char strText[64];
	strncpy(strText, str.GetBuffer(), min(str.GetLength()+1, 63));
	pItem->ReCreate(strText, lClr, lFont, nMask, 1.0, dwExtent);

	// 删除旧的内存空间
	for (int i = 1; i < m_ayItem.GetCount(); i++)
	{
		CDrawItem* pItem = m_ayItem[i];
//		CDrawItem::Free(pItem);
		delete pItem;
	}
	m_ayItem.SetSize(1);
}

BOOL CDrawCell::DrawCell( CDC* pDC, CRect rect, CPoint pos, CDrawTableBase* pTableBase )
{
	// 先画背景色
	if (m_dwStyle & CELL_BK_HIGHLIGHT)
	{
		CRect rcFill = rect;
		pDC->FillSolidRect(&rcFill, theFoundationApp.m_iColor->GetQuoteHighLightBKColor());
	}

	// 再画边框
	BOOL bRet = DrawBoard(pDC, rect);

	CRect rcItem = rect;
	int nLeftDiff = 0;
	CDrawItem* pItem = NULL;
	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		pItem = m_ayItem[i];
		
		rcItem.left += nLeftDiff;
		rcItem.right = rcItem.left + (long)(rect.Width() * pItem->m_dWidth);

		if (pTableBase->DrawSelf(pDC, pos, i, rcItem))
		// 如果调用自定义绘制函数成功 则不会调用系统默认绘图函数		
			continue;
		
		bRet = pItem->DrawItem(pDC, rcItem);
		nLeftDiff = rcItem.Width();
	}

	return bRet;
}

BOOL CDrawCell::DrawBoard( CDC* pDC, CRect rect )
{
	if (rect.IsRectEmpty())
	{
		return FALSE;
	}

	// 画边框
	CPen *oldPen;
	CPen pen;
	// 左边框
	if (m_dwStyle & CELL_BORDER_LEFT)
	{
		pen.CreatePenIndirect(&m_logPen.GetPen(HsLOGPEN::ver));
		oldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(rect.left + 1, rect.top);
		pDC->LineTo(rect.left + 1, rect.bottom);

		pen.DeleteObject();
		pDC->SelectObject(oldPen);
	}

	// 右边框
	if (m_dwStyle & CELL_BORDER_RIGHT)
	{
		pen.CreatePenIndirect(&m_logPen.GetPen(HsLOGPEN::ver));
		oldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(rect.right - 1, rect.top);
		pDC->LineTo(rect.right - 1, rect.bottom);

		pen.DeleteObject();
		pDC->SelectObject(oldPen);
	}

	// 上边框
	if (m_dwStyle & CELL_BORDER_TOP)
	{
		pen.CreatePenIndirect(&m_logPen.GetPen(HsLOGPEN::hor));
		oldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(rect.left, rect.top + 1);
		pDC->LineTo(rect.right, rect.top + 1);

		pen.DeleteObject();
		pDC->SelectObject(oldPen);
	}

	// 下边框
	if (m_dwStyle & CELL_BORDER_BOTTOM)
	{
		pen.CreatePenIndirect(&m_logPen.GetPen(HsLOGPEN::hor));
		oldPen = pDC->SelectObject(&pen);

		pDC->MoveTo(rect.left, rect.bottom - 1);
		pDC->LineTo(rect.right, rect.bottom - 1);
		
		pen.DeleteObject();
		pDC->SelectObject(oldPen);
	}

	return TRUE;
}

void CDrawCell::SetItemText( int nItemID, const char* strText, WORD lClr, WORD lFont, DWORD lMark, DWORD lDelMark)
{
	CDrawItem* pItem = m_ayItem[nItemID];
	if (pItem)
	{
		pItem->SetText(strText, lClr, lFont, lMark, lDelMark);
	}
}

int CDrawCell::GetItemSignal(CPoint point, int nCellWidth, int nLeft)
{
	int nDiffX = 0;
	int nItemPosID = GetItemID(point, nCellWidth, nLeft, nDiffX);

	m_wSignal = NEED_NOTHING;
	if (nItemPosID != -1)
	{
		m_nItemID = nItemPosID;
		CDrawItem* pItem = m_ayItem[nItemPosID];
		if ( nDiffX < m_logPen.lopnWidth.x + 3)
		{// 在边界
			if (pItem->m_dwStyle & ITEM_RESIZABLE)
			{
				m_wSignal = NEED_RESIZE;
				return MOUSE_SIZEWE;
			}	
		}
		else
		{// 在内部
			if (pItem->m_dwStyle & ITEM_MOVABLE)
			{// 可移动
				m_wSignal = NEED_MOVE;
				return MOUSE_ARROW;
			}
			else if (pItem->m_dwStyle & ITEM_TEXT_SELECT)
			{// 可选中
				if (pItem->m_rectDraw.PtInRect(point))
				{
					m_wSignal = NEED_TEXTHIGHLIGHT;
					pItem->m_dwStyle |= ITEM_TEXT_HIGHLIGHT;
					return MOUSE_HAND;
				}
				else
				{
					pItem->m_dwStyle &= ~ITEM_TEXT_HIGHLIGHT;
				}
			}
		}
	}
	return MOUSE_ARROW;
}

int CDrawCell::GetItemID(CPoint point, int nCellWidth, int nLeft, int &nDiff)
{
	int nItemPosID = -1;
	int nWidth = nLeft;
	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		nWidth += (int)(nCellWidth * m_ayItem[i]->m_dWidth);
		if (nWidth > point.x)
		{
			nItemPosID = i;
			nDiff = point.x - (nWidth - (int)(nCellWidth * m_ayItem[i]->m_dWidth));
			break;
		}
	}

	return nItemPosID;
}

void CDrawCell::ReSize(int nItemLeft, int nItemRight, int nCellWidth, int nDiff )
{
	if (nItemLeft < 0 || nItemRight > m_ayItem.GetCount() || nCellWidth <= CELL_MIN_WIDTH)
	{
		return;
	}

	if (m_ayItem[nItemLeft]->m_dWidth * nCellWidth + nDiff < ITEM_MIN_WIDTH)
		nDiff = (int)(ITEM_MIN_WIDTH - m_ayItem[nItemLeft]->m_dWidth * nCellWidth);

	if (m_ayItem[nItemRight]->m_dWidth * nCellWidth - nDiff < ITEM_MIN_WIDTH)
		nDiff = (int)(m_ayItem[nItemRight]->m_dWidth * nCellWidth - ITEM_MIN_WIDTH);

	m_ayItem[nItemLeft]->m_dWidth = (double)(m_ayItem[nItemLeft]->m_dWidth * nCellWidth + nDiff) / (double)nCellWidth;
	m_ayItem[nItemRight]->m_dWidth = (double)(m_ayItem[nItemRight]->m_dWidth * nCellWidth - nDiff) / (double)nCellWidth;
}

void  CDrawCell::ResetItem()
{
	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		CDrawItem* pItem = m_ayItem[i];
		if (pItem->m_dwStyle & ITEM_TEXT_SELECT)
		{
			pItem->m_dwStyle &= ~ITEM_TEXT_HIGHLIGHT;
		}
	}
}

void  CDrawCell::Move(int nBegin, int nEnd)
{
	if (nBegin < 0 || nBegin > m_ayItem.GetCount() || nEnd < 0 || nEnd > m_ayItem.GetCount())
	{// 传入数据非法
		return;
	}

	if (nBegin == nEnd || nBegin + 1 == nEnd)
	{// 位置不变不需要移动
		return;
	}

	// 将开始位置备份
	CDrawItem* pItemTemp = m_ayItem[nBegin]; 

	if (nBegin < nEnd)
	{
		for (int i = nBegin + 1; i < nEnd; i++)
		{
			CDrawItem* pItem = m_ayItem[i];
			m_ayItem.ElementAt(i-1) = pItem;
		}

		m_ayItem.ElementAt(nEnd-1) = pItemTemp;
	}
	else
	{
		for (int i = nBegin - 1; i >= nEnd; i--)
		{
			CDrawItem* pItem = m_ayItem[i];
			m_ayItem.ElementAt(i+1) = pItem;
		}

		m_ayItem.ElementAt(nEnd) = pItemTemp;
	}
}

CRect CDrawCell::GetDrawRect( CPoint pos, int nCellWidth, int nCellHeight)
{
	CRect rect;
	if (pos.x == -1 || pos.y == -1)
	{
		rect.SetRectEmpty();
	}
	else
	{
		int nWidth = (int)(nCellWidth * (m_ayItem[m_nItemID]->m_dWidth));
		rect.left   = pos.x - nWidth / 2;
		rect.right  = pos.x + nWidth / 2;
		rect.top    = pos.y - nCellHeight / 2;
		rect.bottom = pos.y + nCellHeight / 2;
	}

	return rect;
}

CRect CDrawCell::GetItemRect(CRect rectCell, int nItemID)
{
	CRect rect;
	if (nItemID < 0 || nItemID >= m_ayItem.GetCount())
	{
		rect.SetRectEmpty();
	}
	else
	{
		rect = rectCell;
		for (int i = 0; i < nItemID; i++)
		{
			rect.left += (int)(rectCell.Width() * m_ayItem[i]->m_dWidth);
		}
		rect.right = rect.left + (int)(rectCell.Width() * m_ayItem[nItemID]->m_dWidth);
	}

	return rect;
}

CString CDrawCell::GetItemText(int nItemID /*= -1*/)
{
	CString str = "";
	if (nItemID == -1)
	{
		for (int i = 0; i < m_ayItem.GetCount(); i++)
			str.AppendFormat("%s ", m_ayItem[i]->m_str);
	}
	else if (nItemID < m_ayItem.GetCount())
	{
		str.Format("%s", m_ayItem[nItemID]->m_str);
	}

	return str;
}

int CDrawCell::GetItemID(CString str)
{
	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		CDrawItem* pItem = m_ayItem[i];
		if ( strncmp(pItem->m_str, str.GetString(), strlen(pItem->m_str)) == 0)
		{
			return i;
		}
	}
	return -1;
}

BOOL CDrawCell::GetItemStyle(int nItemID, ItemConstructionStruct& sConstructionStruct)
{
	if (nItemID < 0 || nItemID >= m_ayItem.GetCount())
	{
		return FALSE;
	}

	CDrawItem* pItem = m_ayItem[nItemID];
	strncpy(sConstructionStruct.str, pItem->m_str, strlen(pItem->m_str)+1);
	sConstructionStruct.dwItemStyle = pItem->m_dwStyle;
	sConstructionStruct.wClr = pItem->m_wColor;
	sConstructionStruct.wFont = pItem->m_wFont;
	sConstructionStruct.dWidth = pItem->m_dWidth;

	return TRUE;
}

BOOL CDrawCell::GetItemStyle(CArray<ItemConstructionStruct>& ayItemStruct)
{
	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		ItemConstructionStruct sItemStruct;
		GetItemStyle(i, sItemStruct);
		ayItemStruct.Add(sItemStruct);
	}

	return TRUE;
}

void CDrawCell::Copy(CDrawCell* pCell)
{
	if (pCell == NULL)
	{
		return;
	}

	m_dwStyle = pCell->m_dwStyle;
	if (pCell->m_ayItem.GetCount() == m_ayItem.GetCount())
	{
		for (int i = 0; i < m_ayItem.GetCount(); i++)
			m_ayItem[i]->Copy(pCell->m_ayItem[i]);
	}
}

void CDrawCell::SetEmpty()
{
	for (int i = 0; i < m_ayItem.GetCount(); i++)
		m_ayItem[i]->SetEmpty();

	// 去除边框
	m_dwStyle &= ~CELL_BORDER_ENY;
	m_dwStyle &= ~CELL_MOVABLE;
	m_dwStyle &= ~CELL_RESIZABLEWE;
	m_dwStyle &= ~CELL_BK_HIGHLIGHT;
}