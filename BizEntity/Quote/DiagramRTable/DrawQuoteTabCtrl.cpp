#include "StdAfx.h"
#include "DrawQuoteTabCtrl.h"
#include "WinnerApplication.h"
#include "ConfigStruct.h"
#include "..\QuoteComm\pageid.h"
#include "QuoteTabSetting.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
IHsColor* CDrawQuoteTab::m_pColor = NULL;         // 颜色配置文件指针
IHsFont*  CDrawQuoteTab::m_pFont = NULL;          // 字体配置文件指针
CDrawQuoteTab::CDrawQuoteTab()
{
	m_strName = ""; 
	m_nID = 0;		
	m_lMenuMask = 0;
	m_strColGroupName = "";	
	m_lMenuID = 0;
    m_bVisable = TRUE;      
	m_rcTab.IsRectEmpty();  

	m_pColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_pFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
}
CDrawQuoteTab::~CDrawQuoteTab()
{

}

BOOL CDrawQuoteTab::DrawGuide( CDC* pDC, CRect& rect )
{
	m_rcTab.SetRectEmpty();
	if (rect.IsRectEmpty())
		return FALSE;
	
	const ColorProperty* colorFrame = m_pColor->GetQuoteGuideTabFrameColorProp(); // 边框颜色
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);

	CBrush brush(m_pColor->GetQuoteGuideTabBKColor());
	CFont* pFont = m_pFont->GetQuoteGuideTabFont();

	CFont* pOldFont = pDC->SelectObject(pFont);
	COLORREF clrOldTabText = pDC->SetTextColor(m_pColor->GetQuoteGuideTabTextColor());
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// 字体宽度
	CSize size; 
	CSize szText;
	szText = size = pDC->GetTextExtent(m_strName);
	size.cx += 8;

	// 计算当前大小 
	CRect curRC = rect;
	curRC.right = curRC.left + size.cx;
	// 判断是否已经显示到尽头
	if (curRC.right <= rect.right)
	{
		rect.left = curRC.right; // 减去当前已经使用过的区域
		//设定当前标签的范围
		m_rcTab = curRC;

		POINT point[6];
		pDC->MoveTo(m_rcTab.left, m_rcTab.top + 1);
		point[0].x = curRC.left ;
		point[0].y = curRC.top + 1 ;
		point[1].x = curRC.right - 5;
		point[1].y = curRC.top + 1 ;
		point[2].x = curRC.right;
		point[2].y = curRC.top + 6;
		point[3].x = curRC.right;
		point[3].y = curRC.bottom;
		point[4].x = curRC.left;
		point[4].y = curRC.bottom;
		point[5] = point[0];
		pDC->Polygon(point,6);				//选中的tab
		curRC.left += 3;
		pDC->DrawText(m_strName, curRC, DT_SINGLELINE | DT_VCENTER);
	}
	
	//CleanGUI
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(clrOldTabText);
	pen.DeleteObject();
	brush.DeleteObject();

	return TRUE;
}

BOOL CDrawQuoteTab::DrawItem( CDC* pDC, CRect& rect, BOOL bPressed)
{
	m_rcTab.SetRectEmpty();
	if (rect.IsRectEmpty())
		return FALSE;
	
	const ColorProperty* colorFrame = m_pColor->GetQuoteGuideTabFrameColorProp(); // 边框颜色
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
	
	CBrush brush(bPressed?m_pColor->GetQuoteActiveTabBKColor():m_pColor->GetQuoteTabBKColor()); // 选中填充色
	CFont* pFont = m_pFont->GetQuoteGuideTabFont();

	CFont* pOldFont = pDC->SelectObject(pFont);
	COLORREF clrOldTabText = pDC->SetTextColor(bPressed?m_pColor->GetQuoteActiveTabTextColor():m_pColor->GetQuoteTabTextColor());
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// 字体宽度
	CSize size; 
	CSize szText;
	szText = size = pDC->GetTextExtent(m_strName);
	size.cx += 8;

	// 计算当前大小 
	CRect curRC = rect;
	curRC.right = curRC.left + size.cx;
	// 判断是否已经显示到尽头
	BOOL bRet(FALSE);
	if (curRC.right <= rect.right)
	{
		rect.left = curRC.right; // 减去当前已经使用过的区域
		//设定当前标签的范围
		m_rcTab = curRC;

		POINT point[6];
		pDC->MoveTo(m_rcTab.left, m_rcTab.top + 1);
		point[0].x = curRC.left ;
		point[0].y = curRC.top + 1 ;
		point[1].x = curRC.right - 5;
		point[1].y = curRC.top + 1 ;
		point[2].x = curRC.right;
		point[2].y = curRC.top + 6;
		point[3].x = curRC.right;
		point[3].y = curRC.bottom;
		point[4].x = curRC.left;
		point[4].y = curRC.bottom;
		point[5] = point[0];
		pDC->Polygon(point,6);				//选中的tab
		curRC.left += 3;
		pDC->DrawText(m_strName, curRC, DT_SINGLELINE | DT_VCENTER);
		bRet = TRUE;
	}

	//CleanGUI
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(clrOldTabText);
	pen.DeleteObject();
	brush.DeleteObject();

	return bRet;
}

BOOL CDrawQuoteTab::DrawButton( CDC* pDC, CRect& rect, short nMask)
{
	m_rcTab.SetRectEmpty();
	if (rect.IsRectEmpty())
		return FALSE;

	const ColorProperty* colorFrame = m_pColor->GetQuoteTabFrameColorProp();	// 边框颜色;
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
	
	CFont* pFont = m_pFont->GetQuoteTabFont();
	CFont* pOldFont = pDC->SelectObject(pFont);
	// 字体宽度
	CSize size = pDC->GetTextExtent(m_strName); 

	COLORREF clrTabText = 0;
	COLORREF clrFill = 0;
	COLORREF clrArrow = 0;
	switch(nMask)
	{
	case Tab_Pressed:
		{
			clrTabText = m_pColor->GetQuoteActiveTabTextColor();	//字体颜色
			clrFill  = m_pColor->GetQuoteActiveTabBKColor();   	    // 选中时填充色
			clrArrow = colorFrame->m_lColor;
			break;
		}
	case Tab_Disabled:
		{
			clrTabText = GetSysColor(COLOR_BTNSHADOW);
			clrFill = m_pColor->GetQuoteTabBKColor();
			clrArrow = GetSysColor(COLOR_BTNSHADOW);
			break;
		}
	default:
		{
			clrTabText = m_pColor->GetQuoteTabTextColor();
			clrFill = m_pColor->GetQuoteTabBKColor();
			clrArrow = colorFrame->m_lColor;
			break;
		}
	}
	CBrush brush(clrFill);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	COLORREF clrOldTabText = pDC->SetTextColor(clrTabText);
	
	// 计算当前大小 
	m_rcTab = rect;
	m_rcTab.top += 1;
	if (m_lMenuMask & CDrawQuoteTabCtrl::TabPrev)
	{
		m_rcTab.right = m_rcTab.left + size.cx;
		pDC->Rectangle(CRect(m_rcTab.left, m_rcTab.top, m_rcTab.right, m_rcTab.bottom+1));

		POINT pt[4];
		CBrush brushTemp(clrArrow);
		pDC->SelectObject(&brushTemp);
		pDC->MoveTo(m_rcTab.left + 3,m_rcTab.CenterPoint().y);
		pt[0].x = m_rcTab.left + 3;
		pt[0].y = m_rcTab.CenterPoint().y;
		pt[1].x = m_rcTab.right - 3;
		pt[1].y = m_rcTab.top + 3;
		pt[2].x = m_rcTab.right - 3;
		pt[2].y = m_rcTab.bottom - 3;
		pt[3] = pt[0];
		pDC->Polygon(pt,4);
		pDC->SelectObject(&brush);
		brushTemp.DeleteObject();

		rect.left = m_rcTab.right - 1;
	} 
	else if (m_lMenuMask & CDrawQuoteTabCtrl::TabNext)
	{
		m_rcTab.right = m_rcTab.left + size.cx;
		pDC->Rectangle(CRect(m_rcTab.left, m_rcTab.top, m_rcTab.right, m_rcTab.bottom+1));

		POINT pt[4];
		CBrush brushTemp(clrArrow);
		pDC->SelectObject(&brushTemp);
		pDC->MoveTo(m_rcTab.left + 3,m_rcTab.CenterPoint().y);
		pt[0].x = m_rcTab.left + 3;
		pt[0].y = m_rcTab.top + 3;
		pt[1].x = m_rcTab.right - 3;
		pt[1].y = m_rcTab.CenterPoint().y;
		pt[2].x = m_rcTab.left + 3;
		pt[2].y = m_rcTab.bottom - 3;
		pt[3] = pt[0];
		pDC->Polygon(pt,4);
		pDC->SelectObject(&brush);
		brushTemp.DeleteObject();

		rect.left = m_rcTab.right - 1;
	}
	else
	{
		m_rcTab.left = m_rcTab.right - size.cx - 8;
		rect.right = m_rcTab.left - 15;
		pDC->Rectangle(CRect(m_rcTab.left, m_rcTab.top, m_rcTab.right, m_rcTab.bottom+1));
		pDC->DrawText(m_strName, m_rcTab, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		brush.DeleteObject();
	}

	// CleanGUI
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(clrOldTabText);
	pen.DeleteObject();
	brush.DeleteObject();

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
IHsColor*  CDrawQuoteTabCtrl::m_pColor = NULL;                  // 颜色模块接口
IHsTabCfg* CDrawQuoteTabCtrl::m_iTab = NULL;			        // 配置模块接口
IBlockManager* CDrawQuoteTabCtrl::m_iBlockManager = NULL;	    // 板块管理接口
IQuoteLang*	CDrawQuoteTabCtrl::m_iQuoteLang = NULL;	            // 行情资源接口
CMap<UINT,UINT,CString,CString> CDrawQuoteTabCtrl::m_mapTempMenuIndex; // 
CDrawQuoteTabCtrl::CDrawQuoteTabCtrl()
{ 
	m_pColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iTab = (IHsTabCfg*)CWinnerApplication::GetObject(OBJ_TAB);
	m_iBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_iQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	m_nLeftOffset = 0;
	m_nRightOffset = -1;
	m_nCurItem = -1;
	m_nCurButton = -1;
	m_nMouseActiveItem = -1;
	m_pDrawTabNotify = NULL;
	m_strCurGroupName = "";
	Create();
}

CDrawQuoteTabCtrl::~CDrawQuoteTabCtrl(void)
{
	RemoveTabItem(-1);
	for (int i = 0; i < m_ayButtonItem.GetCount(); i++)
	{
		CDrawQuoteTab* pItem = m_ayButtonItem.GetAt(i);
		delete pItem;
	}
	m_ayButtonItem.RemoveAll();
}

void CDrawQuoteTabCtrl::Create()
{
	CDrawQuoteTab* pItem = new CDrawQuoteTab;
	pItem->m_lMenuMask = TabPrev;
	pItem->m_strName = _T("←");
	m_ayButtonItem.Add(pItem);

	pItem = new CDrawQuoteTab;
	pItem->m_lMenuMask = TabNext;
	pItem->m_strName = _T("→");
	m_ayButtonItem.Add(pItem);

	pItem = new CDrawQuoteTab;
	pItem->m_lMenuMask = TabOpertor;
	pItem->m_strName = "设置";
	m_ayButtonItem.Add(pItem);

}
void CDrawQuoteTabCtrl::SetRect( CRect rc )
{
	m_rcClient = rc;
}

BOOL CDrawQuoteTabCtrl::ChangeTabGroup( CString strGroup /*= _T("")*/, BOOL bForce /*= FALSE*/ )
{
	if (strGroup.IsEmpty())
	{
		strGroup = GetDefaultTabGroupName();
	}
	if (m_strCurGroupName == strGroup && !bForce)
	{
		return FALSE;
	}	
	m_strCurGroupName = strGroup;
	m_nCurItem = -1;

	PageInfo pageInfo;
	CTabItemArray ayTabItem;
	LONG nCount = m_iTab->GetTabList(strGroup, ayTabItem);
	if (nCount <= 0)
	{
		return FALSE;
	}

	RemoveTabItem(-1);
	int nItemID = m_iTab->GetDefaultTabItemID();
	int nDefault(0);

	CArray<StockType> ayBlockMarket;
	TabItem* pItem = NULL;
	for (int i = 0, j = 0; i < nCount; i++)
	{
		pItem = ayTabItem.GetAt(i);

		ayBlockMarket.RemoveAll();
		m_pDataSource->HSDataSourceEx_GetSystemMarketInfo((unsigned short*)pItem->m_sMarket, MARKETTYPE_COUNT, ayBlockMarket);
		if (ayBlockMarket.GetCount() != 0)
		{
			// 如果服务器当面配置了名称 以服务器那边的名称为主
			strncpy(pItem->m_szName, ayBlockMarket.GetAt(0).m_stTypeName.m_szName, min(strlen(pItem->m_szName), strlen(ayBlockMarket.GetAt(0).m_stTypeName.m_szName)));
			j++;
		}

		if (pItem->m_ID == nItemID)
			nDefault = j - 1;
		
		pageInfo.m_lPageType = pItem->m_lPageType;
		pageInfo.m_lInPageTag = pItem->m_lPageTag;
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			pageInfo.m_sMarket[i] = pItem->m_sMarket[i];
		}
		memcpy(pageInfo.m_szBlock,pItem->m_szBlock,BLOCK_NAME_LENGTH);
		AddTabItem(pItem->m_szName, pItem->m_ID, pItem->m_lMenuMask, &pageInfo, pItem->m_szColGroupName);
	}
	if (strGroup == GetDefaultTabGroupName())
	{
		ChangeCurItem(nDefault);
	}
	else
		ChangeCurItem(0);
//	m_nLeftOffset = 0;
//	Invalidate(FALSE);
	return TRUE;
}

CString CDrawQuoteTabCtrl::GetDefaultTabGroupName()
{
	CString strName = m_iTab->GetDefaultTabGroup();
	if (strName.IsEmpty())
	{
		strName = _T("股票市场");
	}
	return strName;
}

LONG CDrawQuoteTabCtrl::AddTabItem( CString strName, UINT nID, long lMenuMask /*= 0*/, PageInfo* pageInfo /*= NULL*/, CString strGroupName /*= _T("")*/ )
{
	if (strName.IsEmpty() || !pageInfo)
	{
		return -1;
	}

	// 若当前已有此项，则直接返回当前项
	int nIndex = FindTabItem(strName);
	if (nIndex >= 0)
	{
		return nIndex;
	}

	CDrawQuoteTab* pNewItemData = new CDrawQuoteTab;
	pNewItemData->m_strName = strName;
	pNewItemData->m_nID = nID;
	pNewItemData->m_lMenuMask = lMenuMask;
	pNewItemData->m_strColGroupName = strGroupName;
	pNewItemData->m_PageInfo = pageInfo;
	if (lMenuMask & TabUsrDefine)
	{
		pNewItemData->m_lMenuID = m_iTab->GetUserDefMenuID(nID);
	}
	else
	{
		pNewItemData->m_lMenuID = lMenuMask;
	}

	int nCount = 0;
	if (lMenuMask != 0)
	{// 加入导航页
		m_ayGuideItem.Add(pNewItemData);
		nCount = m_ayGuideItem.GetCount();
	} 
	else
	{// 加入标签页
		m_ayItem.Add(pNewItemData);
		nCount = m_ayItem.GetCount();
	}
	return nCount;
}

int CDrawQuoteTabCtrl::FindTabItem(CString strName)
{
	CDrawQuoteTab* pCurTab = NULL;
	for( int i = 0; i < m_ayItem.GetCount(); i++)
	{
		pCurTab = m_ayItem.GetAt(i);
		if( !pCurTab->m_strName.CompareNoCase(strName) )
		{
			return i;
		}
	}
	// 若没有找到，则考虑更换标签组后再查找
	return -1;
}

BOOL CDrawQuoteTabCtrl::ChangeCurItem( CString strName, BOOL bNotify /*= TRUE*/, short nMarkType /*= 0*/ )
{
	int nIndex = FindTabItem(strName);
	if (nIndex < 0)		// 若在所有分组内都没有找到指定标签，则单独添加
	{
		//单独添加
		PageInfo pageinfo;
		pageinfo.m_lPageType = PriceAnalisys1;
		if (nMarkType > 0)
		{
			pageinfo.m_lInPageTag = PT_LIST|PT_TYPE;
			pageinfo.AddMarketType(nMarkType);
			if (nMarkType == 0x1201)
			{// 如果是深圳A股 还要加上中小盘股、创业板
				pageinfo.AddMarketType(0x1206);  // 中小盘股
				pageinfo.AddMarketType(0x120d);  // 创业板
			}
		} 
		else
		{
			pageinfo.m_lInPageTag = PT_LIST|PT_BLOCK;
			strncpy(pageinfo.m_szBlock,strName.GetString(),BLOCK_NAME_LENGTH);
		}
		AddTabItem(strName,0,0,&pageinfo);
		//CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价标签-未发现%s标签，新建标签页，市场类别（0为板块）：%d",strName,nMarkType);

		return ChangeCurItem(strName);
	}
	return ChangeCurItem(nIndex, bNotify);
}

BOOL CDrawQuoteTabCtrl::ChangeCurItem( int nIndex, BOOL bNotify /*= TRUE*/ )
{
	if (nIndex < 0)
	{
		return FALSE;
	}
	if (m_nCurItem == nIndex)
	{
		return TRUE;
	}

	EnSureVisible(nIndex);
//	Invalidate(FALSE);
	m_nCurItem = nIndex;
	CDrawQuoteTab* pTabItem = m_ayItem[m_nCurItem];
	pTabItem->m_bVisable = TRUE;

	if (bNotify)
		m_pDrawTabNotify->OnActiveTabChange(pTabItem);
	return TRUE;
}

BOOL CDrawQuoteTabCtrl::RemoveTabItem(int nItem)
{
	if( nItem >= 0 && nItem < m_ayItem.GetCount() )
	{
		CDrawQuoteTab* pItem = m_ayItem.GetAt(nItem);
		delete pItem;
		m_ayItem.RemoveAt(nItem);
		return TRUE;
	}
	else if (nItem < 0)
	{
		for (int i = 0; i< m_ayGuideItem.GetCount(); i++)
		{
			CDrawQuoteTab* pItem = m_ayGuideItem.GetAt(i);
			delete pItem;
		}
		m_ayGuideItem.RemoveAll();
		for (int i = 0; i < m_ayItem.GetCount(); i++)
		{
			CDrawQuoteTab* pItem = m_ayItem.GetAt(i);
			delete pItem;
		}
		m_ayItem.RemoveAll();
		return TRUE;
	}
	return FALSE;
}

int CDrawQuoteTabCtrl::EnSureVisible( CString strName )
{
	int nIndex = FindTabItem(strName);
	return EnSureVisible(nIndex);
}

int CDrawQuoteTabCtrl::EnSureVisible( int nIndex )
{
	int nRes = 0;
	if (nIndex >= 0)
	{
		if (nIndex < m_nLeftOffset)
		{
			nRes = nIndex-m_nLeftOffset;
			OnScrollTCItem(nRes);
		} 
		else if (m_nRightOffset>=0 && nIndex > m_nRightOffset)
		{
			nRes = nIndex-m_nRightOffset;
			OnScrollTCItem(nIndex-m_nRightOffset);
		}
	}
	return nRes;
}

void CDrawQuoteTabCtrl::OnScrollTCItem( int nCount )
{
	if (nCount>0 && (nCount>m_ayItem.GetCount()-m_nRightOffset-1))
	{
		nCount = m_ayItem.GetCount()-m_nRightOffset-1;
	}
	else if (nCount<0 && (nCount*-1)>m_nLeftOffset)
	{
		nCount = m_nLeftOffset*-1;
	}
	m_nLeftOffset += nCount;
	m_pDrawTabNotify->TabInvalidate(&m_rcClient, FALSE);
}

void CDrawQuoteTabCtrl::DrawTab( CDC *pDC )
{
	pDC->FillSolidRect(&m_rcClient, RGB(28,28,28));
	
	CRect rc = m_rcClient;
	rc.top = m_rcClient.bottom;
	rc.bottom = rc.top + 1;
	pDC->FillSolidRect(&rc, RGB(155,0,0));

	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	CDrawQuoteTab* pTab				= NULL;
	CDrawQuoteTab* pActiveTab		= NULL;
	CDrawQuoteTab* pMouseActiveTab	= NULL;

	CRect rcDisp = m_rcClient;
	// 画向导页
	for(int i = 0; i < m_ayGuideItem.GetCount(); i++)
	{
		pTab = m_ayGuideItem.GetAt(i);
		if( pTab == NULL )
			continue;
		pTab->DrawGuide(pDC, rcDisp);
	}
	
	int nMask(0);
	// 画按钮
	for (int i=0; i<m_ayButtonItem.GetCount(); i++)
	{
		pTab = m_ayButtonItem.GetAt(i);
		if (!(pTab->m_lMenuMask & TabNext))
		{
			nMask = 0;
			if ((pTab->m_lMenuMask & TabPrev) && m_nLeftOffset <= 0)
			{
				nMask = CDrawQuoteTab::Tab_Disabled;
			}
			else
			{
				if (i == m_nCurButton)
				{
					nMask = CDrawQuoteTab::Tab_Pressed;
				}
			}
			pTab->DrawButton(pDC, rcDisp, nMask);
		}
	}
	
	// 画标签页
	BOOL bPressed(FALSE);
	m_nRightOffset = m_ayItem.GetCount() - 1;
	for(int i = m_nLeftOffset; i < m_ayItem.GetCount(); i++)
	{
		pTab = m_ayItem.GetAt(i);
		if( pTab == NULL )
			continue;

		// 当前页状态
		if (i == m_nCurItem || i == m_nMouseActiveItem)
			bPressed = TRUE;
		else
			bPressed = FALSE;

		if( !pTab->DrawItem(pDC, rcDisp, bPressed) )
		{
			m_nRightOffset = i - 1;
			break;
		}
	}
	
	// 画按钮
	nMask = 0;
	for (int i=0; i<m_ayButtonItem.GetCount(); i++)
	{
		pTab = m_ayButtonItem.GetAt(i);
		if (pTab->m_lMenuMask & TabNext)
		{
			if ((pTab->m_lMenuMask & TabNext) && m_nRightOffset >= m_ayItem.GetCount() - 1 )
			{
				nMask = CDrawQuoteTab::Tab_Disabled;
			}
			else
			{
				if (i == m_nCurButton)
				{
					nMask = CDrawQuoteTab::Tab_Pressed;
				}
			}
			pTab->DrawButton(pDC, rcDisp, nMask);
		}
	}
	
	const ColorProperty* colorFrame = m_pColor->GetQuoteTabFrameColorProp();
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rcDisp.left,rcDisp.bottom);
	pDC->LineTo(rcDisp.left,rcDisp.top);

	pDC->SetBkMode(nOldBkMode);
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

BOOL CDrawQuoteTabCtrl::MouseMove( UINT nFlags, CPoint point)
{
	if (m_rcClient.PtInRect(point))
	{
		int nIndex = 0;
		int nType = HitTest(point, nIndex);
		if (nType == CDrawQuoteTab_Type_Item)
		{
			if (m_nMouseActiveItem != nIndex)
			{
				m_nMouseActiveItem = nIndex;
				m_nCurButton = -1;
				m_pDrawTabNotify->TabInvalidate(&m_rcClient, FALSE);
			}
			return TRUE;	
		}
		else if (nType == CDrawQuoteTab_Type_Button)
		{
			m_nCurButton = nIndex;
			m_nMouseActiveItem = -1;
			m_pDrawTabNotify->TabInvalidate(&m_rcClient, FALSE);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if (m_nMouseActiveItem != -1 || m_nCurButton != -1)
		{
			m_nMouseActiveItem = -1;	
			m_nCurButton = -1;
			m_pDrawTabNotify->TabInvalidate(&m_rcClient, FALSE);
		}
		return FALSE;
	}
}
	
BOOL CDrawQuoteTabCtrl::MouseDown( UINT nFlags, CPoint point)
{
	if (!m_rcClient.PtInRect(point))
		return FALSE;
	int nIndex(0);
	int nType = HitTest(point, nIndex);
	if (nType)
	{
		CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
		if (pWnd)
		{// 将坐标装化为 屏幕坐标
			ClientToScreen(pWnd->GetSafeHwnd(), &point);
		}
		if (nType == CDrawQuoteTab_Type_Item)
		{// 单击标签页 
			m_nMouseActiveItem = nIndex;
			if (m_nMouseActiveItem != m_nCurItem)
			{
				m_nCurItem = m_nMouseActiveItem;
				CDrawQuoteTab* pNewTab = m_ayItem[m_nCurItem];
				m_pDrawTabNotify->OnActiveTabChange(pNewTab);
			}
			return TRUE;
		}
		else if (nType == CDrawQuoteTab_Type_Button)
		{// 单击按钮项
			int nMenu(0);
			CDrawQuoteTab* pTab = m_ayButtonItem[nIndex];
			if (pTab->m_lMenuMask & TabPrev)
				OnScrollTCItem(-1);
			else if (pTab->m_lMenuMask & TabNext)
				OnScrollTCItem(1);
			else if (pTab->m_lMenuMask & TabOpertor)
				nMenu = OpenSettingMenu(point);
			
			if (nMenu)
			{
				if (nMenu == CDRAWTAB_QUOTETAB_SETTING)
				{
					AFX_MANAGE_STATE(AfxGetStaticModuleState());
					CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
					CQuoteTabSetting tabSettingDlg(pWnd, m_strCurGroupName);
					tabSettingDlg.DoModal();
					ChangeTabGroup(tabSettingDlg.GetCurGroup(), TRUE);
				}
				else 
				{
					int nBegin = CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN;
					int nCount = nMenu - nBegin;
					CString strGroup = m_ayTempGroups.GetAt(nCount);
					ChangeTabGroup(strGroup);
				}
			}

			return TRUE;
		}
		else
		{// 单击导航页
			CDrawQuoteTab* pTab = m_ayGuideItem[nIndex];
			CString strCurItemName = pTab->m_strName;
			if (pTab->m_lMenuID < TAB_ALLMARKET_MENUMASK)		// 四位菜单ID为市场，从数据引擎或配置模块取得内容动态生成菜单
			{
				int nMenuID(-1);
				if (pTab->m_lMenuID == SYSBK_BOURSE)
				{ 
					nMenuID = OpenBlockMenu(point, TRUE);
				}
				else if (pTab->m_lMenuID == USERDEF_BOURSE)
				{
					nMenuID = OpenBlockMenu(point, FALSE);
				}

				CString strMenuText;
				if (m_mapTempMenuIndex.Lookup(nMenuID, strMenuText) && !strMenuText.IsEmpty())
				{
					ChangeCurItem(strMenuText, TRUE);
				}
				return TRUE;
			}
			else if (pTab->m_lMenuID == TAB_ALLMARKET_MENUMASK) // 所有市场
			{
				int nMenuID = OpenAllMarketMenu(point);
				CString strMenuText;
				if (m_mapTempMenuIndex.Lookup(nMenuID, strMenuText) && !strMenuText.IsEmpty())
				{
					int nIndex = strMenuText.Find("-");
					CString strName = strMenuText.Left(nIndex);
					HSMarketDataType nMarket = atoi(strMenuText.Right(strMenuText.GetLength()-nIndex-1));
					ChangeCurItem(strName, TRUE, nMarket);
				}
				return TRUE;
			}
			else if (pTab->m_lMenuID == TAB_BLOCK_MENUMASK)  // 板块市场
			{
				int nMenuID = OpenSecondBlockMenu(pTab->m_strName, point);
				CString strMenuText;
				if (m_mapTempMenuIndex.Lookup(nMenuID, strMenuText) && !strMenuText.IsEmpty())
				{
					ChangeCurItem(strMenuText, TRUE);
				}
				return TRUE;
			}
			return FALSE;
		}
	}
	else
		return FALSE;
}

int CDrawQuoteTabCtrl::HitTest( CPoint point, int& nIndex )
{
	CDrawQuoteTab* pTab = NULL;
	for (int i = 0; i < m_ayGuideItem.GetCount(); i++)
	{
		pTab = m_ayGuideItem[i];
		if (pTab->m_rcTab.PtInRect(point))
		{
			nIndex = i;
			return CDrawQuoteTab_Type_Guide;
		}
	}

	for (int i = m_nLeftOffset; i < m_ayItem.GetCount(); i++)
	{
		pTab = m_ayItem[i];
		if (pTab->m_rcTab.PtInRect(point))
		{
			nIndex = i;
			return CDrawQuoteTab_Type_Item;
		}
	}

	for (int i = 0; i < m_ayButtonItem.GetCount(); i++)
	{
		pTab = m_ayButtonItem[i];
		if (pTab->m_rcTab.PtInRect(point))
		{
			nIndex = i;
			return CDrawQuoteTab_Type_Button;
		}
	}
	return 0;
}

int CDrawQuoteTabCtrl::OpenBlockMenu(CPoint point, BOOL bSysBlock)
{	
	CDrawQuoteTab* pTab = m_ayItem[m_nCurItem];
	CString strCurItemName = pTab->m_strName;
	
	if (bSysBlock)	// 系统板块
	{
		CStringArray ayBlockGroup;
		m_iBlockManager->GetAllSysBlockGroupName(ayBlockGroup);

		m_mapTempMenuIndex.RemoveAll();
		int nCount = 0;
		CMenu menu;
		menu.CreatePopupMenu();
		for (int i=0; i<ayBlockGroup.GetCount(); i++)
		{
			CMenu submenu;
			submenu.CreatePopupMenu();

			CStringArray ayBlock;
			CString strGroupName = ayBlockGroup.GetAt(i);
			m_iBlockManager->GetBlockName(strGroupName,ayBlock);
			for (int j=0; j<ayBlock.GetCount(); j++)
			{
				submenu.AppendMenu(0,CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
				m_mapTempMenuIndex.SetAt(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
				if (!strCurItemName.IsEmpty() && strCurItemName==ayBlock.GetAt(j))
				{
					submenu.CheckMenuItem(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,MF_BYCOMMAND|MF_CHECKED);
				}
				nCount++;
			}
			menu.AppendMenu(MF_POPUP,(UINT_PTR)submenu.GetSafeHmenu(),ayBlockGroup.GetAt(i));
		}

		CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
		int nMenuID = menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y,pWnd);
		for (int i=0; i<ayBlockGroup.GetCount(); i++)
		{
			menu.GetSubMenu(i)->DestroyMenu();
		}
		menu.DestroyMenu();	
		return nMenuID;
	}
	else // 自选板块
	{
		CStringArray ayBlockGroup;
		m_iBlockManager->GetAllUserBlockGroupName(ayBlockGroup);
		m_mapTempMenuIndex.RemoveAll();

 		int nCount = 0;
 		CMenu menu;
 		menu.CreatePopupMenu();
// 		for (int i=0; i<ayBlockGroup.GetCount(); i++)
// 		{
// 			CMenu submenu;
// 			submenu.CreatePopupMenu();
// 
// 			CStringArray ayBlock;
// 			CString strGroupName = ayBlockGroup.GetAt(i);
// 			m_iBlockManager->GetBlockName(strGroupName,ayBlock);
// 			for (int j=0; j<ayBlock.GetCount(); j++)
// 			{
// 				submenu.AppendMenu(0,CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
// 				m_mapTempMenuIndex.SetAt(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
// 				if (!strCurItemName.IsEmpty() && strCurItemName==ayBlock.GetAt(j))
// 				{
// 					submenu.CheckMenuItem(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,MF_BYCOMMAND|MF_CHECKED);
// 				}
// 				nCount++;
// 			}
// 			menu.AppendMenu(MF_POPUP,(UINT_PTR)submenu.GetSafeHmenu(),ayBlockGroup.GetAt(i));
// 		}
		//////////////////////////////////////////////////////////////////////////
		// 添加自选板块
		CString strSelf = m_iQuoteLang->LoadStr(IDS_SELF_BLOCK);
		m_mapTempMenuIndex.SetAt(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount, strSelf);
		menu.AppendMenu(MF_POPUP, CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount, strSelf);
		if (!strCurItemName.IsEmpty() && strCurItemName == strSelf)
		{
			menu.CheckMenuItem(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount, MF_BYCOMMAND|MF_CHECKED);
		}
		//////////////////////////////////////////////////////////////////////////
		CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
		int nMenuID = menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y,pWnd);
		for (int i=0; i<ayBlockGroup.GetCount(); i++)
		{
			menu.GetSubMenu(i)->DestroyMenu();
		}
		menu.DestroyMenu();
		return nMenuID;
	}

	return -1;
}

int CDrawQuoteTabCtrl::OpenSecondBlockMenu(CString strMarketName, CPoint point)
{
	CStringArray ayBlock;
	if (m_iBlockManager->GetBlockName(strMarketName, ayBlock) > 0)
	{
		CDrawQuoteTab* pTab = m_ayItem[m_nCurItem];
		CString strCurItemName = pTab->m_strName;

		CMenu menu;
		menu.CreatePopupMenu();

		int nCount(0);
		for (int i = 0; i < ayBlock.GetCount(); i++)
		{
			menu.AppendMenu(0, CDRAWTAB_ID_MENU_BLOCK_BEGIN + nCount, ayBlock.GetAt(i));
			m_mapTempMenuIndex.SetAt(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount, ayBlock.GetAt(i));
			if (!strCurItemName.IsEmpty() && strCurItemName == ayBlock.GetAt(i))
			{
				menu.CheckMenuItem(CDRAWTAB_ID_MENU_BLOCK_BEGIN+nCount,MF_BYCOMMAND|MF_CHECKED);
			}
			nCount++;
		}
		
		//////////////////////////////////////////////////////////////////////////
		CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
		int nMenuID = menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y,pWnd);
		menu.DestroyMenu();
		return nMenuID;
	}
	return -1;
}

int CDrawQuoteTabCtrl::OpenAllMarketMenu(CPoint point )
{
	CDrawQuoteTab* pTab = m_ayItem[m_nCurItem];
	CString strCurItemName = pTab->m_strName;
	CArray<StockType> ayAllMarket;
	m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(0, 0, ayAllMarket);

	short nPreMarket = 0;
	CMenu menu;
	menu.CreatePopupMenu();
	for (int i=0; i<ayAllMarket.GetCount(); i++)
	{
		if ((nPreMarket&0xFF00) != (ayAllMarket.GetAt(i).m_nStockType&0xFF00))
		{
			menu.AppendMenu(MF_SEPARATOR|MF_MENUBARBREAK);
		}
		menu.AppendMenu(MF_BYCOMMAND,CDRAWTAB_ID_MENU_MARKET_BEGIN+i,ayAllMarket.GetAt(i).m_stTypeName.m_szName);
		if (!strCurItemName.IsEmpty() && strCurItemName==ayAllMarket.GetAt(i).m_stTypeName.m_szName)
		{
			menu.CheckMenuItem(CDRAWTAB_ID_MENU_MARKET_BEGIN+i,MF_BYCOMMAND|MF_CHECKED);
		}

		CString strTemp;
		strTemp.Format("%s-%d",ayAllMarket.GetAt(i).m_stTypeName.m_szName,ayAllMarket.GetAt(i).m_nStockType);
		m_mapTempMenuIndex.SetAt(CDRAWTAB_ID_MENU_MARKET_BEGIN+i,strTemp);

		nPreMarket = (ayAllMarket.GetAt(i).m_nStockType&0xFF00);
	}

	CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
	int nMenuID = menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y,pWnd);
	menu.DestroyMenu();
	return nMenuID;
}

int CDrawQuoteTabCtrl::OpenSettingMenu( CPoint point )
{
	CMenu menu;
	menu.CreatePopupMenu();

	menu.InsertMenu(0,MF_BYCOMMAND|MF_STRING, CDRAWTAB_QUOTETAB_SETTING,_T("设置"));
	menu.InsertMenu(1,MF_SEPARATOR);
	m_ayTempGroups.RemoveAll();
	m_iTab->GetAllTabGroups(m_ayTempGroups);
	for (int i=0; i<m_ayTempGroups.GetCount(); i++)
	{
		menu.AppendMenu(MF_BYCOMMAND|MF_STRING, CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN+i,m_ayTempGroups.GetAt(i));
		if (m_strCurGroupName == m_ayTempGroups.GetAt(i))
		{
			menu.CheckMenuItem(CDRAWTAB_QUOTETAB_TABGROUPS_BEGIN+i,MF_BYCOMMAND|MF_CHECKED);
		}
	}

	CWnd* pWnd = m_pDrawTabNotify->TabGetCWnd();
	int nMenuID = menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y,pWnd);
	menu.DestroyMenu();
	return nMenuID;
}