#include "StdAfx.h"
#include "DiagramTab.h"

int CDiagramTab::m_nDimensions = 18; 


void CDiagramTabItem::Draw(CDC* pDC,BOOL bActiveItem,CDiagramTab* pDiagarmTab)
{
	//pDC->Ellipse(m_rect);
	//return;

	DWORD dwPostion = pDiagarmTab->GetPostion();
#define HS_POINTS_NUM 6
#define HS_OFFSET     5
	POINT pts [HS_POINTS_NUM];
	if(dwPostion == TS_TOP )
	{
		pts [0].x = m_rect.left;
		pts [0].y = m_rect.top + HS_OFFSET;

		pts [1].x = m_rect.left + HS_OFFSET;
		pts [1].y = m_rect.top;

		pts [2].x = m_rect.right - HS_OFFSET;
		pts [2].y = m_rect.top;

		pts [3].x = m_rect.right;
		pts [3].y = m_rect.top + HS_OFFSET;

		pts [4].x = m_rect.right;
		pts [4].y = m_rect.bottom;

		pts [5].x = m_rect.left;
		pts [5].y = m_rect.bottom;
	}
	else if( dwPostion == TS_BOTTOM )
	{
		pts [0].x = m_rect.left;
		pts [0].y = m_rect.top;

		pts [1].x = m_rect.left;
		pts [1].y = m_rect.bottom - HS_OFFSET;

		pts [2].x = m_rect.left + HS_OFFSET;
		pts [2].y = m_rect.bottom;

		pts [3].x = m_rect.right - HS_OFFSET;
		pts [3].y = m_rect.bottom;

		pts [4].x = m_rect.right;
		pts [4].y = m_rect.bottom - HS_OFFSET;

		pts [5].x = m_rect.right;
		pts [5].y = m_rect.top;
	}
	else if( dwPostion == TS_LEFT )
	{
		pts [0].x = m_rect.left;
		pts [0].y = m_rect.top + HS_OFFSET;

		pts [1].x = m_rect.left + HS_OFFSET;
		pts [1].y = m_rect.top;

		pts [2].x = m_rect.right;
		pts [2].y = m_rect.top;

		pts [3].x = m_rect.right;
		pts [3].y = m_rect.bottom;

		pts [4].x = m_rect.left + HS_OFFSET;
		pts [4].y = m_rect.bottom;

		pts [5].x = m_rect.left;
		pts [5].y = m_rect.bottom - HS_OFFSET;
	}
	else //right
	{
		pts [0].x = m_rect.left;
		pts [0].y = m_rect.top;

		pts [1].x = m_rect.right - HS_OFFSET;
		pts [1].y = m_rect.top;

		pts [2].x = m_rect.right;
		pts [2].y = m_rect.top + HS_OFFSET;

		pts [3].x = m_rect.right;
		pts [3].y = m_rect.bottom - HS_OFFSET;

		pts [4].x = m_rect.right - HS_OFFSET;
		pts [4].y = m_rect.bottom;

		pts [5].x = m_rect.left;
		pts [5].y = m_rect.bottom;
	}

	CPen pen(PS_SOLID, 1, pDiagarmTab->m_clrBorder);
	CPen* pOLdPen = pDC->SelectObject(&pen);

	CBrush* pOldBrush = NULL;
	COLORREF clrBrush = bActiveItem ? pDiagarmTab->m_clrActiveFill : pDiagarmTab->m_clrFill;
	CBrush br(clrBrush);
	pOldBrush = pDC->SelectObject(&br);

	CFont* pOldItemFont = NULL;
	if(pDiagarmTab->m_pItemFont)
		pOldItemFont = pDC->SelectObject(pDiagarmTab->m_pItemFont);

	//»­±êÇ©Ò³ÇøÓò
	pDC->Polygon(pts, HS_POINTS_NUM);
	//ÊäÈë±êÇ©Ò³ÎÄ×Ö
	COLORREF clrOldTextColor = pDC->SetTextColor(pDiagarmTab->m_clrFore);
	UINT uFormat = 0;
	CRect rcText = m_rect;
	if(dwPostion == TS_TOP || dwPostion == TS_BOTTOM)
		uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	else
	{
		rcText.DeflateRect(0,HS_OFFSET,0,0);
		uFormat = DT_CENTER | DT_WORDBREAK;
	}

	pDC->DrawText(m_strCaption,rcText,uFormat) ;

	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);
	if( pOLdPen != NULL)
		pDC->SelectObject(pOLdPen);
	if(pOldItemFont != NULL)
		pDC->SelectObject(pOldItemFont);
	pDC->SetTextColor(clrOldTextColor);

}

CDiagramTab::CDiagramTab(void)
{
	m_pHostWnd = NULL;
	m_dwTabStyle = 0;
	m_rect.SetRectEmpty(); 
	m_nActiveIndex = 0;
	m_pDiagramTabNotify = NULL;

	m_clrBackground = RGB(0,0,0);
	m_clrActiveFill = RGB(48,48,48);
	m_clrFill = RGB(0,0,0);//RGB(48,48,48);
	m_clrFore = RGB(192,192,192);
	m_clrBorder = RGB(192,0,0);

	m_pItemFont = NULL;
}

CDiagramTab::~CDiagramTab(void)
{
	RemoveAllTabItem();
}

UINT CDiagramTab::AddTabItem(CString strCaption,int nIndex)
{
	CDiagramTabItem* pItemData = new CDiagramTabItem();
	pItemData->m_strCaption = strCaption;
	pItemData->m_nIndex = nIndex;
	m_tabItems.Add(pItemData);
	return m_tabItems.GetCount();
}

void CDiagramTab::RemoveTabItem(CString strCaption)
{
	for (int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pItemData = m_tabItems[i];
		if (pItemData->m_strCaption == strCaption)
		{
			delete pItemData;
			m_tabItems.RemoveAt(i);
		}
	}
}

void CDiagramTab::RemoveTabItem(int nIndex)
{
	for (int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pItemData = m_tabItems[i];
		if (pItemData->m_nIndex == nIndex)
		{
			delete pItemData;
			m_tabItems.RemoveAt(i);
		}
	}
}

void CDiagramTab::RemoveAllTabItem()
{
	for( int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pItemData = m_tabItems[i];
		if( pItemData )
			delete pItemData;
	}
}

int CDiagramTab::GetVisableItemCount()
{
	int nCount = 0;
	for( int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pItemData = m_tabItems[i];
		if( pItemData->m_bVisable )
			nCount++;
	}
	return nCount;
}

void CDiagramTab::ReCalTabItemSize()
{
	DWORD dwTabSizeStyle = GetTabSizeStyle();
	DWORD dwTabPostion = GetPostion();
	int nCount = GetVisableItemCount();
	if(nCount == 0)
		return;
	if( dwTabSizeStyle == TS_TABSIZEEQUAL )
	{
		if(dwTabPostion == TS_TOP || dwTabPostion == TS_BOTTOM )
		{
			int nWidth = m_rect.Width() / nCount + 1;
			int j = 0;
			for(int i = 0 ;i < m_tabItems.GetCount();i++)
			{
				CDiagramTabItem* pTabItem = m_tabItems[i];
				if(pTabItem->m_bVisable)
				{
					pTabItem->m_rect.left = m_rect.left + j * nWidth;
					pTabItem->m_rect.top = m_rect.top;
					pTabItem->m_rect.right = m_rect.left + (j + 1) * nWidth;
					pTabItem->m_rect.bottom = m_rect.bottom;
					j++;
				}
			}
		}
		else
		{
			int j = 0;
			int nHeight = m_rect.Height() / nCount + 1;
			for(int i = 0 ;i < m_tabItems.GetCount();i++)
			{
				CDiagramTabItem* pTabItem = m_tabItems[i];
				if(pTabItem->m_bVisable)
				{
					pTabItem->m_rect.left = m_rect.left;
					pTabItem->m_rect.top = m_rect.top + j * nHeight;
					pTabItem->m_rect.right = m_rect.right;
					pTabItem->m_rect.bottom = m_rect.top + (j + 1) * nHeight;
					j++;
				}
			}
		}	
	}
	else
	{	

	}
	
}

void CDiagramTab::Draw(CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(m_rect,m_clrBackground);
	for(int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pTabItem = m_tabItems[i];
		pTabItem->Draw(pDC,i == m_nActiveIndex,this);
	}
}

void CDiagramTab::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0; i < m_tabItems.GetCount(); i++)
	{
		CDiagramTabItem* pTabItem = m_tabItems[i];
		if( m_pDiagramTabNotify != NULL && pTabItem->m_rect.PtInRect(point) )
		{
			if(m_nActiveIndex != pTabItem->m_nIndex)
			{
				CDiagramTabItem* pOldTabItem = m_tabItems[m_nActiveIndex];
				m_nActiveIndex = pTabItem->m_nIndex;
				m_pDiagramTabNotify->OnActiveTabChange(pTabItem,pOldTabItem);
			}
			
		}
	}
}