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
// 	unsigned int unSize = 49999;  // ����
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
// 	{// ������ڴ���Է���
// 		
// 		// ����һ���е�Ԫ�س����� 
// 		CDrawItem* pItem = m_lstIdle.front();
// 		pItem->ReCreate(pConstructionStruct);
// 		m_lstIdle.pop_front();
// 		
// 		// ���뵽�ѷ���map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 
// 		return pItem;
// 	}
// 	else 
// 	{// ���û���ڴ����
// 
// 		// ��ϵͳ�����ڴ� 
// 		CDrawItem* pItem = new CDrawItem(pConstructionStruct);
// 
// 		// ���뵽�ѷ���map
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
// 	{// ������ڴ���Է���
// 
// 		// ����һ���е�Ԫ�س����� 
// 		CDrawItem* pItem = m_lstIdle.front();
// 		pItem->ReCreate(str, wClr, wFont, dwStyle, dWidth, dwExtent);
// 		m_lstIdle.pop_front();
// 
// 		// ���뵽�ѷ���map
// 		m_mapAlloc.SetAt((WORD)pItem, (void*)pItem);
// 		m_mapAlloc[(int)pItem] = pItem;
// 		
// 		return pItem;
// 	}
// 	else 
// 	{// ���û���ڴ����
// 
// 		// ��ϵͳ�����ڴ� 
// 		CDrawItem* pItem = new CDrawItem(str, wClr, wFont, dwStyle, dWidth, dwExtent);
// 
// 		// ���뵽�ѷ���map
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
// 		// ���ѷ���map�Ƴ�
//     	m_mapAlloc.RemoveKey((WORD)pItem);
// 		m_mapAlloc.erase((int)pItem);
// 
// 		// ���뵽���ж��ж�β
// 		m_lstIdle.push_back(pItem);
// 	}
// }
///////////////////////////////////////////////////////////////////////
//CDrawItemMemControl CDrawItem::m_memControl;  // �ڴ����
CDrawItem::CDrawItem(ItemConstructionStruct* pConstructionStruct)
{
	if (pConstructionStruct == NULL)
	{
		memset(m_str, 0, sizeof(m_str)); // �ı�����
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

	CRect textRect;    // ����ı���С
	CRect tringleRect; // �����δ�С

	// ���õ�Ԫ���ı�����
	int nBkMode(0);
	COLORREF oldBkClr(0);
	if (m_dwStyle & ITEM_TEXT_HIGHLIGHT)
	{
		nBkMode = pDC->SetBkMode(BKMODE_LAST);
		oldBkClr = pDC->SetBkColor(theFoundationApp.m_iColor->GetQuoteHighLightBKColor()); // ������ɫ
	}
	else
		nBkMode = pDC->SetBkMode(TRANSPARENT);
	
	// ��������
	CFont* pOldFont = pDC->SelectObject(DrawTool::GetFont(m_wFont));
	CString str = m_str;
	CSize sz = pDC->GetTextExtent(str); // ��ȡ������������Ҫ�Ĵ�С

	//////////////////////////////////////////////////////////////////////////
	// ����������ɫ
	COLORREF oldTxClr(0);
	oldTxClr = pDC->SetTextColor(DrawTool::GetColor(m_wColor));

	// ���ö��뷽ʽ
	int wAlign = DT_VCENTER | DT_SINGLELINE; 
	textRect = rect;
	if (m_dwStyle & ITEM_TEXT_LEFT)
	{// �����
		wAlign |= DT_LEFT;
		textRect.DeflateRect(ITEM_LEFT_MARGIN, 0, 0, 0);
		// �����������������С���������ɻ����ַ� �߶���ʱ������
		if ( (m_dwStyle & ITEM_ARROW_NEED) && (m_dwStyle & ITEM_ARROW_LEFT) )
		{// ���Ҫ�������� ƽ�������ε�λ����ʾ����� �������ε�λ��Ԥ������
			textRect.left = textRect.left + ITEM_ARROW_WIDTH;
		}
		textRect.right = textRect.left + sz.cx;
	}
	else if(m_dwStyle & ITEM_TEXT_RIGHT)
	{// �Ҷ���
		wAlign |= DT_RIGHT;
		textRect.DeflateRect(0, 0, ITEM_RIGHT_MARGIN, 0);
		if ( (m_dwStyle & ITEM_ARROW_NEED) && !(m_dwStyle & ITEM_ARROW_LEFT) )
		{// ���Ҫ�������� ���������ε�λ����ʾ���ұ� �������ε�λ��Ԥ������
			textRect.right = textRect.right - ITEM_ARROW_WIDTH;
		}
		textRect.left = textRect.right - sz.cx;	
	}
	else if(m_dwStyle & ITEM_TEXT_CENTER)
	{// ����
		wAlign |= DT_CENTER;
		if (m_dwStyle & ITEM_ARROW_NEED)
		{// ��������λ��Ԥ������
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

	pDC->DrawText(str, &textRect, wAlign); // �����ַ�
	m_rectDraw = textRect;
	
	//////////////////////////////////////////////////////////////////////////
	// ��������
	if (m_dwStyle & ITEM_ARROW_NEED)
	{
		IHsColor* pColor = theFoundationApp.m_iColor;
		// ȷ�������λ�������
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
	// ��ԭ�豸������
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
	// ȥ���������� ������������Ҫ����ƶ�֮���ȷ����
	m_dwStyle &= ~ITEM_TEXT_HIGHLIGHT;
}

void CDrawItem::SetEmpty()
{
	memset(m_str, 0, sizeof(m_str));

	// ȥ��һЩ��ʾ����
	m_dwStyle &= ~ITEM_ARROW_UP;
	m_dwStyle &= ~ITEM_ARROW_DOWN;
	m_rectDraw.SetRectEmpty();
}
