#include "StdAfx.h"
#include "DrawItem.h"
#include "..\..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridLineArray
void CDrawItemArray::DeleteAll()
{
	CDrawItem* pItem;
	for(int i=0; i<m_nSize; i++)
	{
		pItem = (CDrawItem*)m_pData[i];
		if(pItem != NULL)
		{
//			CDrawItem::Free(pItem);
			delete pItem;
		}
	} 
	RemoveAll();
}
////////////////////////////////////////////////////////////////////////
// CDrawItemMemControl::CDrawItemMemControl()
// {
// 	unsigned int unSize = 49999;  // 素数
// 	m_mapAlloc.InitHashTable(unSize);
// 	m_mapAlloc.rehash(unSize);
// }
// 
// CDrawItemMemControl::~CDrawItemMemControl()
// {
// 	WORD wKey(0);
// 	CDrawItem* pItem = NULL;
// 
// 	while (m_lstIdle.size() > 0)
// 	{
// 		pItem = m_lstIdle.front();
// 		if (pItem && !::IsBadReadPtr(pItem, 1))
// 			delete pItem;
// 
// 		m_lstIdle.pop_front();
// 	}
// }
// 
// CDrawItem* CDrawItemMemControl::AllocMem(ItemConstructionStruct* pConstructionStruct)
// {
// 	if (m_lstIdle.size() > 0)
// 	{// 如果有内存可以分配
// 		
// 		// 将第一项中的元素出队列 
// 		CDrawItem* pItem = m_lstIdle.front();
// 		pItem->ReCreate(pConstructionStruct);
// 		m_lstIdle.pop_front();
// 		
// 		// 加入到已分配map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 
// 		return pItem;
// 	}
// 	else 
// 	{// 如果没有内存可用
// 
// 		// 向系统申请内存 
// 		CDrawItem* pItem = new CDrawItem(pConstructionStruct);
// 
// 		// 加入到已分配map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 		
// 		return pItem;
// 	}
// }
// 
// CDrawItem* CDrawItemMemControl::AllocMem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent)
// {
// 	if (m_lstIdle.size() > 0)
// 	{// 如果有内存可以分配
// 
// 		// 将第一项中的元素出队列 
// 		CDrawItem* pItem = m_lstIdle.front();
// 		pItem->ReCreate(str, wClr, wFont, dwStyle, dWidth, dwExtent);
// 		m_lstIdle.pop_front();
// 
// 		// 加入到已分配map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 		
// 		return pItem;
// 	}
// 	else 
// 	{// 如果没有内存可用
// 
// 		// 向系统申请内存 
// 		CDrawItem* pItem = new CDrawItem(str, wClr, wFont, dwStyle, dWidth, dwExtent);
// 
// 		// 加入到已分配map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 
// 		return pItem;
// 	}
// }
// 
// void CDrawItemMemControl::FreeMem(CDrawItem* &pItem)
// {
// 	if (pItem && !::IsBadReadPtr(pItem, 1))
// 	{
// 		// 从已分配map移除
//     	m_mapAlloc.RemoveKey((WORD)pItem);
// 		m_mapAlloc.erase((int)pItem);
// 
// 		// 加入到空闲队列队尾
// 		m_lstIdle.push_back(pItem);
// 	}
// }
///////////////////////////////////////////////////////////////////////
//CDrawItemMemControl CDrawItem::m_memControl;  // 内存管理
CDrawItem::CDrawItem(ItemConstructionStruct* pConstructionStruct)
{
	if (pConstructionStruct == NULL)
	{
		memset(m_str, 0, sizeof(m_str)); // 文本内容
		m_wColor = 0;
		m_wFont  = 0;
		m_dwStyle = 0; 
		m_dWidth = 1.0; 
		m_dwExtent = 0;
	}
	else 
	{
		strncpy(m_str, pConstructionStruct->str, min(strlen(pConstructionStruct->str)+1, sizeof(m_str)-1));
		m_wColor   = pConstructionStruct->wClr;
		m_wFont    = pConstructionStruct->wFont;
		m_dwStyle  = pConstructionStruct->dwItemStyle;
		m_dWidth   = pConstructionStruct->dWidth;
		m_dwExtent = pConstructionStruct->dwExtent;
	}
	m_rectDraw.SetRectEmpty();
}

CDrawItem::CDrawItem(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent)
{
	strncpy(m_str, str, min(strlen(str)+1, sizeof(m_str)-1));
	m_wColor = wClr;
	m_wFont  = wFont;
	m_dwStyle = dwStyle;
	m_dWidth = dWidth;
	m_dwExtent = dwExtent;
	m_rectDraw.SetRectEmpty();
}

CDrawItem::~CDrawItem(void)
{
}

// CDrawItem* CDrawItem::Create(ItemConstructionStruct* pConstructionStruct)
// {
// 	return m_memControl.AllocMem(pConstructionStruct);
// }
// CDrawItem* CDrawItem::Create(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent)
// {
// 	return m_memControl.AllocMem(str, wClr, wFont, dwStyle, dWidth, dwExtent);
// }
// void CDrawItem::Free(CDrawItem* pItem)
// {
// 	m_memControl.FreeMem(pItem);
// }

void CDrawItem::ReCreate(ItemConstructionStruct* pConstructionStruct)
{
	ASSERT(pConstructionStruct);
	ReCreate(pConstructionStruct->str, 
		pConstructionStruct->wClr, 
		pConstructionStruct->wFont, 
		pConstructionStruct->dwItemStyle, 
		pConstructionStruct->dWidth, 
		pConstructionStruct->dwExtent);
}
void CDrawItem::ReCreate(char str[64], WORD wClr, WORD wFont, DWORD dwStyle, double dWidth, DWORD dwExtent)
{
	strncpy(m_str, str, min(strlen(str)+1, sizeof(m_str)-1));
	m_wColor   = wClr;
	m_wFont    = wFont;
	m_dwStyle  = dwStyle;
	m_dWidth   = dWidth;
	m_dwExtent = dwExtent;
	m_rectDraw.SetRectEmpty();
}

void CDrawItem::SetText( CString str, WORD wClr, WORD wFont, DWORD dwMask, DWORD dwDelMask )
{
	SetText(str.GetString(), wClr, wFont, dwMask, dwDelMask);
}
void CDrawItem::SetText(const char* strText, WORD wClr, WORD wFont, DWORD dwMask, DWORD dwDelMask )
{
	if (strText != NULL)
		strncpy(m_str, strText, min(strlen(strText)+1, sizeof(m_str)-1) );
	
	if (wClr)
	{
		m_wColor = wClr;
	}
	if (wFont)
	{
		m_wFont = wFont;
	}

	m_dwStyle &= ~dwDelMask;
	m_dwStyle |= dwMask;
}

BOOL CDrawItem::DrawItem( CDC* pDC, CRect rect )
{
	if (rect.IsRectEmpty())
	{
		return FALSE;
	}

	CRect textRect;    // 输出文本大小
	CRect tringleRect; // 三角形大小

	// 设置单元格文本背景
	int nBkMode(0);
	COLORREF oldBkClr(0);
	if (m_dwStyle & ITEM_TEXT_HIGHLIGHT)
	{
		nBkMode = pDC->SetBkMode(BKMODE_LAST);
		oldBkClr = pDC->SetBkColor(theFoundationApp.m_iColor->GetQuoteHighLightBKColor()); // 高亮颜色
	}
	else
		nBkMode = pDC->SetBkMode(TRANSPARENT);
	
	// 设置字体
	CFont* pOldFont = pDC->SelectObject(DrawTool::GetFont(m_wFont));
	CString str = m_str;
	CSize sz = pDC->GetTextExtent(str); // 获取绘制字体所需要的大小

	//////////////////////////////////////////////////////////////////////////
	// 设置字体颜色
	COLORREF oldTxClr(0);
	oldTxClr = pDC->SetTextColor(DrawTool::GetColor(m_wColor));

	// 设置对齐方式
	int wAlign = DT_VCENTER | DT_SINGLELINE; 
	textRect = rect;
	if (m_dwStyle & ITEM_TEXT_LEFT)
	{// 左对齐
		wAlign |= DT_LEFT;
		textRect.DeflateRect(ITEM_LEFT_MARGIN, 0, 0, 0);
		// 绘制区域扩大或者缩小到可以容纳绘制字符 高度暂时不考虑
		if ( (m_dwStyle & ITEM_ARROW_NEED) && (m_dwStyle & ITEM_ARROW_LEFT) )
		{// 如果要画三角形 平且三角形的位置显示在左边 把三角形的位置预留出来
			textRect.left = textRect.left + ITEM_ARROW_WIDTH;
		}
		textRect.right = textRect.left + sz.cx;
	}
	else if(m_dwStyle & ITEM_TEXT_RIGHT)
	{// 右对齐
		wAlign |= DT_RIGHT;
		textRect.DeflateRect(0, 0, ITEM_RIGHT_MARGIN, 0);
		if ( (m_dwStyle & ITEM_ARROW_NEED) && !(m_dwStyle & ITEM_ARROW_LEFT) )
		{// 如果要画三角形 并且三角形的位置显示在右边 把三角形的位置预留出来
			textRect.right = textRect.right - ITEM_ARROW_WIDTH;
		}
		textRect.left = textRect.right - sz.cx;	
	}
	else if(m_dwStyle & ITEM_TEXT_CENTER)
	{// 居中
		wAlign |= DT_CENTER;
		if (m_dwStyle & ITEM_ARROW_NEED)
		{// 把三角形位置预留出来
			textRect.left = textRect.left + (rect.Width() - sz.cx - ITEM_ARROW_WIDTH) / 2;
			textRect.right = textRect.right - (rect.Width() - sz.cx) / 2;
		}
		else
		{
			textRect.left = textRect.left + (rect.Width() - sz.cx) / 2;
			textRect.right = textRect.right - (rect.Width() - sz.cx) / 2;
		}
	}
	textRect.top = textRect.top + (rect.Height() - sz.cy) / 2;
	textRect.bottom = textRect.bottom - (rect.Height() - sz.cy) / 2;

	pDC->DrawText(str, &textRect, wAlign); // 绘制字符
	m_rectDraw = textRect;
	
	//////////////////////////////////////////////////////////////////////////
	// 画三角形
	if (m_dwStyle & ITEM_ARROW_NEED)
	{
		IHsColor* pColor = theFoundationApp.m_iColor;
		// 确定三角形绘制区域
		tringleRect = rect;
		tringleRect.top += tringleRect.Height()/4;
		if (m_dwStyle & ITEM_ARROW_LEFT)
		{
			tringleRect.right = textRect.left - 2;
			tringleRect.left = tringleRect.right - ITEM_ARROW_WIDTH;
		}
		else
		{
			tringleRect.left = textRect.right + 2;
			tringleRect.right = tringleRect.left + ITEM_ARROW_WIDTH;
		}

		if (m_dwStyle & ITEM_ARROW_UP)
		{
			CGeneralHelper::DrawArrow(pDC, tringleRect, pColor->GetUpColor(), TRUE);
			textRect.right = tringleRect.right;
		}
		else if(m_dwStyle & ITEM_ARROW_DOWN)
		{
			CGeneralHelper::DrawArrow(pDC, tringleRect, pColor->GetDownColor(), FALSE);
			textRect.right = tringleRect.right;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 还原设备上下文
	if (m_dwStyle & ITEM_TEXT_HIGHLIGHT)
		pDC->SetBkColor(oldBkClr);
	
	pDC->SetBkMode(nBkMode);
	pDC->SetTextColor(oldTxClr);
	pDC->SelectObject(pOldFont);
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void CDrawItem::Copy(CDrawItem* pItem)
{
	if (pItem == NULL)
	{
		return;
	}

	memset(m_str, 0, sizeof(m_str));
	strncpy(m_str, pItem->m_str, strlen(pItem->m_str));
	m_wColor = pItem->m_wColor;
	m_wFont  = pItem->m_wFont;
	m_rectDraw = pItem->m_rectDraw;
	m_dwStyle  = pItem->m_dwStyle;
	m_dwExtent = pItem->m_dwExtent;
	// 去除高亮属性 高亮属性是需要鼠标移动之后才确定的
	m_dwStyle &= ~ITEM_TEXT_HIGHLIGHT;
}

void CDrawItem::SetEmpty()
{
	memset(m_str, 0, sizeof(m_str));

	// 去掉一些显示属性
	m_dwStyle &= ~ITEM_ARROW_UP;
	m_dwStyle &= ~ITEM_ARROW_DOWN;
	m_rectDraw.SetRectEmpty();
}
