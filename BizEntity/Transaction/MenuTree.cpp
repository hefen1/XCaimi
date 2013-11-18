
#include "stdafx.h"
#include "tinyxml.h"
#include ".\MenuTree.h"

#include "..\..\HelperLib\GeneralHelper.h"

#include <stack>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTree

CMenuTree::CMenuTree()
{
	m_bBuilding = FALSE;
}

CMenuTree::~CMenuTree()
{
	
}

void CMenuTree::Release()
{
	//POSITION pos;
	//CString key;
	//HTREEITEM h = NULL;
	//for( pos = m_MenuItemMap.GetStartPosition(); pos != NULL; )
	//{
	//	m_MenuItemMap.GetNextAssoc( pos, key, (void*&)h );
	//	if(h)
	//	{
	//		CMenuTreeItem* pItem;
	//		pItem = (CMenuTreeItem*)GetItemData(h);
	//		if (pItem)
	//		{
	//			delete pItem;
	//		}			
	//	}
	//}
	for (int i = 0; i < m_paMenuItem.GetCount(); i++)
	{
		CMenuTreeItem* pItem = (CMenuTreeItem*)m_paMenuItem.GetAt(i);
		delete pItem;
	}
	m_MenuItemMap.RemoveAll();
	delete this;
}

BEGIN_MESSAGE_MAP(CMenuTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMenuTree::OnNMCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTree 消息处理程序

BOOL CMenuTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

void CMenuTree::BuildMenuTree(CString strMenuFile)
{
	m_bBuilding = TRUE;

	TiXmlDocument m_doc;
	if( !m_doc.LoadFile(strMenuFile))
		return;

	TiXmlElement* pRoot = m_doc.FirstChildElement("Trading");
	TiXmlElement* pTradingMenu = pRoot->FirstChildElement("TradingMenu");
	TiXmlElement* node = pTradingMenu->FirstChildElement("Item")->FirstChildElement("Item");
	BuildMenuTree(0,node);

	m_bBuilding = FALSE;

	return;
}

void CMenuTree::BuildMenuTree(HTREEITEM h,TiXmlElement* pMenuElment)
{
	m_bBuilding = TRUE;
	TiXmlElement* pElem = pMenuElment;
	while(pElem != NULL)
	{	
		CString strCaption = pElem->Attribute(_T("caption"));
		CString strLayout =  pElem->Attribute(_T("layout"));
		CString sTmp = pElem->Attribute(_T("imageindex"));
		if (sTmp.IsEmpty())
		{
			sTmp = "1";
		}
		int nImageIndex = atoi(sTmp);
		if (nImageIndex)
		{
			nImageIndex--;
		}
		CMenuTreeItem *pItemData = new CMenuTreeItem(strCaption,strLayout);
		/*CMenuTreeItem *pItemDataExist = NULL;

		if(m_MenuItemMap.Lookup(strLayout,(void*&)(pItemDataExist)) != 0)
		{
			delete pItemDataExist;
		}*/
		//m_MenuItemMap.SetAt(strLayout,(void*)(pItemData));
		HTREEITEM hNew = NULL;		
		hNew = InsertItem(strCaption,nImageIndex, nImageIndex, h,TVI_LAST);
		SetItemData(hNew, DWORD_PTR(pItemData));
		m_MenuItemMap.SetAt(strLayout,(void*)hNew);
		m_paMenuItem.Add(pItemData);
		if(!pElem->NoChildren())
		{
			TiXmlElement* pChildElem = pElem->FirstChild(_T("Item"))->ToElement();
			BuildMenuTree(hNew,pChildElem);
			CString strExpand = pElem->Attribute(_T("expand"));
			if ("1" == strExpand)
			{
				Expand(hNew, TVE_EXPAND);
			}			
		}
		pElem=pElem->NextSiblingElement();
	}
	m_bBuilding = FALSE;
}

HTREEITEM CMenuTree::GetMenuTreeItem(CString strLayout)
{
	HTREEITEM h = NULL;
	if(m_MenuItemMap.Lookup(strLayout,(void*&)h) != 0)
	{
		return h;
	}
	else
		return NULL;
}


void CMenuTree::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here

	LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	switch (pCustomDraw->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
		// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
		//pCustomDraw->clrTextBk = RGB(0, 0, 255);
		//pCustomDraw->clrText = RGB(255, 255, 255);
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;

	case CDDS_ITEMPREPAINT:
		if(this->IsWindowEnabled() == 1)
		{
			if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS)) == 0
				&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED)) == CDIS_SELECTED) // selected
			{ 
				//pCustomDraw->clrTextBk = RGB(255, 255, 255);
				//pCustomDraw->clrText = RGB(0, 0, 0);
				pCustomDraw->clrTextBk = RGB(0, 0, 255);
				pCustomDraw->clrText = RGB(255, 255, 255);
			}
			*pResult = CDRF_NOTIFYPOSTPAINT;
			return;
		}
		else{
			*pResult = CDRF_DODEFAULT ;
			return;
		}
	case CDDS_ITEMPOSTPAINT:
		if(this->IsWindowEnabled() == 1)
		{
			/*if ((pCustomDraw->nmcd.uItemState & (CDIS_FOCUS)) == 0
				&&(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED)) == CDIS_SELECTED) // selected
			{
				CRect   rcText;  
				HTREEITEM hItem=(HTREEITEM) pCustomDraw->nmcd.dwItemSpec;
				GetItemRect(hItem,   &rcText,   true); 
				CPen penBlue(PS_SOLID ,1,RGB(0, 0, 255));
				CDC* pDC=CDC::FromHandle(pCustomDraw->nmcd.hdc); 
				CBrush* pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush* pOldBrush = pDC->SelectObject(pBrush);
				CPen* pOldPen = pDC->SelectObject(&penBlue);
				pDC->Rectangle(&rcText);
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pOldPen);
			}*/
			*pResult = CDRF_SKIPDEFAULT;
			return;
		}
		else{
			*pResult = CDRF_DODEFAULT ;
			return;
		}

	}
}   