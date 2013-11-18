#include "stdafx.h"
#include "TechKLineTab.h"
#include "GraphicsData.h"
#include "TechKLineMgr.h"
#include "..\..\..\Common\WinnerApplication.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//CTechKLineTab
CTechKLineTab::CTechKLineTab()
{
	m_pItemFont = g_hsGraphicsData.ftTab.GetFont();
	m_nTabKey = eTabItemIndex_RootTech;
	m_pMgr = NULL;
	IHsKLineTab* pTabCfg = (IHsKLineTab*)CWinnerApplication::GetObject(OBJ_KLINETAB);
	if (pTabCfg != NULL)
	{
		int nIndexCount = pTabCfg->GetTechIndexAyCount();
		if (nIndexCount > 0)
		{
			CString strIndexName = pTabCfg->GetTechIndexName(0);
			AddTabItem(strIndexName, eTabItemIndex_RootTech);
			pTabCfg->GetTechIndex(0, m_ayTechIndex);
			if (nIndexCount > 1)
			{
				pTabCfg->GetTechIndex(1, m_ayTechMore);
			}
		}
		int nTempleteCount = pTabCfg->GetTechTempleteAyCount();
		if (nTempleteCount > 0)
		{
			CString strTempleteName = pTabCfg->GetTechTempleteName(0);
			AddTabItem(strTempleteName, eTabItemIndex_Roottemplet);
			pTabCfg->GetTechTemplete(0, m_ayTemplete);
		}
		pTabCfg->GetDefaultGroupName();
	}
	ReSetItemHideShow(eTabItemIndex_RootTech);
}

CTechKLineTab::~CTechKLineTab()
{
}

void CTechKLineTab::SetTechKLineMgr(CTechKLineMgr* pMgr)
{
	m_pMgr = pMgr;
}

void CTechKLineTab::ReCalTabItemSize()
{
	DWORD dwTabSizeStyle = GetTabSizeStyle();
	DWORD dwTabPostion = GetPostion();
	int nCount = GetVisableItemCount();
	if(nCount == 0)
		return;
	if( dwTabSizeStyle == TS_TABSIZEEQUAL )
	{
	}
	else
	{	
		if(dwTabPostion == TS_TOP || dwTabPostion == TS_BOTTOM )
		{
			int nWidth = m_rect.Width() / nCount + 1;
			int nItemStartPos = m_rect.left;
			for(int i = 0 ;i < m_tabItems.GetCount();i++)
			{
				CDiagramTabItem* pTabItem = m_tabItems[i];
				if(pTabItem->GetVisible())
				{
					CString strCaption = pTabItem->GetCaption();
					int nCaptionLen = strCaption.GetLength();
					int nWidth = nCaptionLen * TAB_ITEM_UNITWIDTH;
					CRect rcItemCur(nItemStartPos, m_rect.top, nItemStartPos + nWidth, m_rect.bottom);
					pTabItem->SetRect(rcItemCur);
					nItemStartPos += nWidth;
				}
			}
		}
	}
}

void CTechKLineTab::Draw(CDC *pDC)
{
	CDiagramTab::Draw(pDC);
	if (!m_rect.IsRectEmpty())
	{
		CPen pen(g_hsGraphicsData.clChartTick.m_nPenStyle, g_hsGraphicsData.clChartTick.m_nWidth, g_hsGraphicsData.clChartTick.m_lColor);
		CPen *pOldPen = (CPen*)pDC->SelectObject(&pen);
		pDC->MoveTo(m_rect.left, m_rect.top);
		pDC->LineTo(m_rect.right, m_rect.top);
		pDC->SelectObject(pOldPen);
		if (pen.m_hObject)
		{
			pen.DeleteObject();
		}
	}
}

void CTechKLineTab::ReSetItemHideShow(int nRootIndex)
{
	if (m_tabItems.GetCount() > 2)
	{
		for(int i = m_tabItems.GetCount() - 1; i >= 2 ; i --)
		{
			CDiagramTabItem* pTabItem = m_tabItems[i];
			if (pTabItem != NULL)
			{
				RemoveTabItem(i);
			}
		}
	}

	if (nRootIndex == eTabItemIndex_RootTech && m_ayTechIndex.GetSize() > 0)
	{
		int nAddStart = 2;
		for (int i = 0; i < m_ayTechIndex.GetSize(); i ++)
		{
			TechIndex* pIndex = m_ayTechIndex.GetAt(i);
			if (pIndex != NULL)
			{
				AddTabItem(pIndex->m_szName, nAddStart ++);
			}
		}
	}
	else if (nRootIndex == eTabItemIndex_Roottemplet)
	{
		int nAddStart = 2;
		for (int i = 0; i < m_ayTemplete.GetSize(); i ++)
		{
			TechTemplete* pTemplete = m_ayTemplete.GetAt(i);
			if (pTemplete != NULL)
			{
				AddTabItem(pTemplete->m_szName, nAddStart ++);
			}
		}
	}
	m_nTabKey = nRootIndex;
	ReCalTabItemSize();
}

int CTechKLineTab::GetTabKey()
{
	return m_nTabKey;
}

TechIndex* CTechKLineTab::GetTabTechIndex(CString strCaption)
{
	TechIndex* pTechIndexRet = NULL;
	if (!strCaption.IsEmpty() && m_ayTechIndex.GetSize() > 0)
	{
		for(int i = 0; i < m_ayTechIndex.GetSize(); i ++)
		{
			TechIndex* pIndex = m_ayTechIndex.GetAt(i);
			if (pIndex != NULL && strCaption.CompareNoCase(pIndex->m_szName) == 0)
			{
				if (pIndex->m_szIndexName[0] != '\0' )
				{
					pTechIndexRet = pIndex;
					break;
				}
				else	//µ¯³ö²Ëµ¥
				{
					int nMenuTrackID = -1;
					POINT pt;
					::GetCursorPos(&pt);
					CMenu menu;
					if(menu.CreatePopupMenu() && m_ayTechMore.GetSize() > 0 && m_pMgr != NULL)
					{
						for (int j = 0; j < m_ayTechMore.GetSize(); j ++)
						{
							TechIndex* pIndexMenu = m_ayTechMore.GetAt(j);
							if (pIndexMenu != NULL && pIndexMenu->m_szName[0] != '\0')
							{
								CString strMenuItem;
								strMenuItem.Format("%s - %s", pIndexMenu->m_szName, pIndexMenu->m_szIndexName);
								menu.AppendMenu(MF_STRING, j + 1, strMenuItem);
							}
						}
						CWnd* pWnd = m_pMgr->GetParentWnd();
						nMenuTrackID = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, pWnd);
					}
					menu.DestroyMenu();
					if (nMenuTrackID > 0 && nMenuTrackID < m_ayTechMore.GetSize())
					{
						pTechIndexRet = m_ayTechMore.GetAt(nMenuTrackID - 1);
					}
					SetActiveItem(eTabItemIndex_RootTech);
				}
			}
		}
	}
	return pTechIndexRet;
}

TechTemplete* CTechKLineTab::GetTabTechTemplete(CString strCaption)
{
	TechTemplete* pTechTempleteRet = NULL;
	if (!strCaption.IsEmpty() && m_ayTechIndex.GetSize() > 0)
	{
		for(int i = 0; i < m_ayTemplete.GetSize(); i ++)
		{
			TechTemplete* pTemplete = m_ayTemplete.GetAt(i);
			if (pTemplete != NULL && strCaption.CompareNoCase(pTemplete->m_szName) == 0)
			{
				pTechTempleteRet = pTemplete;
				break;
			}
		}
	}
	return pTechTempleteRet;
}

void CTechKLineTab::OpenSpecifyTemplete(ExpPropery* pExp)
{
	TechTemplete* pTemplete = GetTabTechTemplete(_T("ÈýÍ¼"));
	if (pTemplete != NULL)
	{
		CString strTemplete = pTemplete->m_szDefaultIndex;
		int nRPos = strTemplete.Find(":");
		if (nRPos > 0 && nRPos < strTemplete.GetLength())
		{
			CString strRep;
			strRep.Format("%s,%d", pExp->m_strName, pExp->m_dExpType);
			strTemplete = strTemplete.Left(nRPos + 1) + strRep;

			strncpy(pTemplete->m_szDefaultIndex, strTemplete, min(sizeof(pTemplete->m_szDefaultIndex) - 1, strTemplete.GetLength()));
			pTemplete->m_szDefaultIndex[strTemplete.GetLength()] = 0;
		}
		if (m_pMgr != NULL)
		{
			m_pMgr->OnTabItemChanged(pTemplete);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//CTechKLineTabNotify
CTechKLineTabNotify::CTechKLineTabNotify()
{
}

void CTechKLineTabNotify::OnActiveTabChange(CDiagramTabItem* pActiveItem,CDiagramTabItem* pOldActiveItem)
{
	if (m_pMgr != NULL && m_pTab != NULL && pActiveItem != NULL)
	{
		int nCurIndex = pActiveItem->GetIndex();
		if (nCurIndex <= eTabItemIndex_Roottemplet)
		{
			m_pTab->ReSetItemHideShow(nCurIndex);
		}
		else 
		{
			CString strCaption = pActiveItem->GetCaption();
			TechIndex* pIndex = m_pTab->GetTabTechIndex(strCaption);
			if (pIndex != NULL)
			{
				m_pMgr->OnTabItemChanged(pIndex);
			}
			else
			{
				TechTemplete* pTemplete = m_pTab->GetTabTechTemplete(strCaption);
				if (pTemplete != NULL)
				{
					m_pMgr->OnTabItemChanged(pTemplete);
				}
			}
		}
	}
}

void CTechKLineTabNotify::SetNotifyMgr(CTechKLineMgr* pMgr, CTechKLineTab* pTab)
{
	m_pMgr = pMgr;
	m_pTab = pTab;
}

