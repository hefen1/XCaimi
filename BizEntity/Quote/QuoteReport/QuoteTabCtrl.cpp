// QuoteTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteTabCtrl.h"
#include "quotestuctdefine.h"
#include "GeneralHelper.h"
#include "memdc.h"
#include "WinnerApplication.h"
#include "quotetabsetting.h"
#include "..\QuoteComm\pageid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IHsTabCfg* CQuoteTabCtrl::m_iTab = NULL;
IHsColor* CQuoteTabCtrl::m_iColor = NULL;
IHsFont*  CQuoteTabCtrl::m_iFont = NULL;
IBlockManager* CQuoteTabCtrl::m_iBlockManager = NULL;

IDataSourceEx* CQuoteTabCtrl::m_pDataSource = NULL;
long CQuoteTabCtrl::m_nDataSourceID = 0;

CMap<UINT,UINT,CString,CString> CQuoteTabCtrl::m_mapTempMenuIndex;
// CQuoteTabCtrl::HsTCItem
BOOL HsTCItem::Draw(CDC* pDC,CRect& rect,int nMask,
								  int nWidth /*= 0*/,
								  int nTabType /*= 1*/)
{
	if (!pDC)
	{
		return FALSE;
	}

	// 字体宽度
	CSize size; 
	CSize szText;
	szText = size = pDC->GetTextExtent(pszText);
	size.cx += 8;

	COLORREF clTabText = 0;
	COLORREF colorBK = CQuoteTabCtrl::m_iColor->GetBlackgroundColor();
	const ColorProperty* colorFrame = NULL;
	COLORREF colorFill = 0;
	CFont* pFont = NULL;

	if (nMask & CQuoteTab_Type_Guide)
	{
		clTabText = CQuoteTabCtrl::m_iColor->GetQuoteGuideTabTextColor();
		colorFrame = CQuoteTabCtrl::m_iColor->GetQuoteGuideTabFrameColorProp(); // 边框颜色
		colorFill = CQuoteTabCtrl::m_iColor->GetQuoteGuideTabBKColor();	// 选中时填充色
		pFont = (CFont*)CQuoteTabCtrl::m_iFont->GetQuoteGuideTabFont();
	} 
	else
	{
		clTabText = (nMask & TCIS_BUTTONPRESSED)?CQuoteTabCtrl::m_iColor->GetQuoteActiveTabTextColor():CQuoteTabCtrl::m_iColor->GetQuoteTabTextColor();	//字体颜色
		colorFrame = CQuoteTabCtrl::m_iColor->GetQuoteTabFrameColorProp();	// 边框颜色
		colorFill = (nMask & TCIS_BUTTONPRESSED) ? CQuoteTabCtrl::m_iColor->GetQuoteActiveTabBKColor() : CQuoteTabCtrl::m_iColor->GetQuoteTabBKColor();	// 选中时填充色
		pFont = (CFont*)CQuoteTabCtrl::m_iFont->GetQuoteTabFont();
	}
	LOGFONT logfont;
	pFont->GetLogFont(&logfont);
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
	CBrush brush(colorFill);
	


	// 计算当前大小 
	CRect curRC = rect;
	if ((nMask & CQuoteTab_Show_Auto_TILE) && nWidth > 0)
	{
		curRC.right = curRC.left + nWidth;
	}
	else
	{
		curRC.right = curRC.left + size.cx;
	}	
	// 判断是否已经显示到尽头
	if (curRC.right > rect.right)
	{
		return FALSE;
	} 
	rect.left = curRC.right;
	//设定当前标签的范围
	rc = curRC;

	int nSaveDC = pDC->SaveDC();
	if (nTabType == 0)		// 丑陋风格
	{
	} 
	else if (nTabType == 1)	// 红黑风格
	{
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);

		POINT point[6];
		if (nMask & CQuoteTab_ALIGN_TOP)
		{
		} 
		else if (nMask & CQuoteTab_ALIGN_LEFT)
		{
		}
		else if (nMask & CQuoteTab_ALIGN_RIGHT)
		{
		}
		else
		{
			pDC->MoveTo(rc.left,rc.top + 1);
			point[0].x = curRC.left ;
			point[0].y = curRC.top + 1 ;
			point[1].x = curRC.right - 5;
			point[1].y = curRC.top + 1 ;
			point[2].x = curRC.right;
			point[2].y = curRC.top + 6;
			point[3].x = curRC.right;
			point[3].y = curRC.bottom-1 ;
			point[4].x = curRC.left;
			point[4].y = curRC.bottom-1 ;
			point[5] = point[0];
			pDC->Polygon(point,6);				//选中的tab
		}
		pDC->SelectObject(pFont);
		pDC->SetTextColor(clTabText);
		curRC.left += 3;
		pDC->DrawText(pszText,curRC,DT_SINGLELINE | DT_VCENTER);
	}
	else if (nTabType == 2)	// 预留风格
	{
	}

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
	brush.DeleteObject();
	return TRUE;
}

BOOL HsTCItem::Operator(CWnd* pParentWnd,int nMouse)
{
	if( pParentWnd != NULL && CQuoteTabCtrl::m_pDataSource)
	{
		CString strCurItemName;
		HsTCItem* pCurItem = ((CQuoteTabCtrl*)pParentWnd)->GetItem(((CQuoteTabCtrl*)pParentWnd)->m_nCurItem);
		if (pCurItem)
		{
			strCurItemName = pCurItem->pszText;
		}
		TabItemData* pData = (TabItemData*)lParam;
		if (pData && !IsBadReadPtr(pData,1))
		{
			if (pData->m_lMenuID < TAB_ALLMARKET_MENUMASK)		// 四位菜单ID为市场，从数据引擎或配置模块取得内容动态生成菜单
			{
				if (pData->m_lMenuID == SYSBK_BOURSE)	// 系统板块
				{
					CStringArray ayBlockGroup;
					CQuoteTabCtrl::m_iBlockManager->GetAllSysBlockGroupName(ayBlockGroup);

					CQuoteTabCtrl::m_mapTempMenuIndex.RemoveAll();

					int nCount = 0;
					CMenu menu;
					menu.CreatePopupMenu();
					for (int i=0; i<ayBlockGroup.GetCount(); i++)
					{
						CMenu submenu;
						submenu.CreatePopupMenu();

						CStringArray ayBlock;
						CString strGroupName = ayBlockGroup.GetAt(i);
						CQuoteTabCtrl::m_iBlockManager->GetBlockName(strGroupName,ayBlock);
						for (int j=0; j<ayBlock.GetCount(); j++)
						{
							submenu.AppendMenu(0,ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
							CQuoteTabCtrl::m_mapTempMenuIndex.SetAt(ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
							if (!strCurItemName.IsEmpty() && strCurItemName==ayBlock.GetAt(j))
							{
								submenu.CheckMenuItem(ID_MENU_BLOCK_BEGIN+nCount,MF_BYCOMMAND|MF_CHECKED);
							}
							nCount++;
						}
						menu.AppendMenu(MF_POPUP,(UINT_PTR)submenu.GetSafeHmenu(),ayBlockGroup.GetAt(i));
					}
					CPoint point;
					GetCursorPos(&point);

					menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,pParentWnd);
					for (int i=0; i<ayBlockGroup.GetCount(); i++)
					{
						menu.GetSubMenu(i)->DestroyMenu();
					}
					menu.DestroyMenu();
				} 
				else if (pData->m_lMenuID == USERDEF_BOURSE)	// 自选板块
				{
					CStringArray ayBlockGroup;
					CQuoteTabCtrl::m_iBlockManager->GetAllUserBlockGroupName(ayBlockGroup);

					CQuoteTabCtrl::m_mapTempMenuIndex.RemoveAll();

					int nCount = 0;
					CMenu menu;
					menu.CreatePopupMenu();
					for (int i=0; i<ayBlockGroup.GetCount(); i++)
					{
						CMenu submenu;
						submenu.CreatePopupMenu();

						CStringArray ayBlock;
						CString strGroupName = ayBlockGroup.GetAt(i);
						CQuoteTabCtrl::m_iBlockManager->GetBlockName(strGroupName,ayBlock);
						for (int j=0; j<ayBlock.GetCount(); j++)
						{
							submenu.AppendMenu(0,ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
							CQuoteTabCtrl::m_mapTempMenuIndex.SetAt(ID_MENU_BLOCK_BEGIN+nCount,ayBlock.GetAt(j));
							if (!strCurItemName.IsEmpty() && strCurItemName==ayBlock.GetAt(j))
							{
								submenu.CheckMenuItem(ID_MENU_BLOCK_BEGIN+nCount,MF_BYCOMMAND|MF_CHECKED);
							}
							nCount++;
						}
						menu.AppendMenu(MF_POPUP,(UINT_PTR)submenu.GetSafeHmenu(),ayBlockGroup.GetAt(i));
					}
					CPoint point;
					GetCursorPos(&point);
					//pParentWnd->ScreenToClient(&point);

					menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,pParentWnd);
					for (int i=0; i<ayBlockGroup.GetCount(); i++)
					{
						menu.GetSubMenu(i)->DestroyMenu();
					}
					menu.DestroyMenu();

				}
				else
				{

				}
			}
			else if (pData->m_lMenuID == TAB_ALLMARKET_MENUMASK)		// 全部市场
			{
				CArray<StockType> ayAllMarket;
				CQuoteTabCtrl::m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(0,0,ayAllMarket);

				short nPreMarket = 0;
				CMenu menu;
				menu.CreatePopupMenu();
				for (int i=0; i<ayAllMarket.GetCount(); i++)
				{
					if ((nPreMarket&0xFF00) != (ayAllMarket.GetAt(i).m_nStockType&0xFF00))
					{
						menu.AppendMenu(MF_SEPARATOR|MF_MENUBARBREAK);
					}
					menu.AppendMenu(MF_BYCOMMAND,ID_MENU_MARKET_BEGIN+i,ayAllMarket.GetAt(i).m_stTypeName.m_szName);
					if (!strCurItemName.IsEmpty() && strCurItemName==ayAllMarket.GetAt(i).m_stTypeName.m_szName)
					{
						menu.CheckMenuItem(ID_MENU_MARKET_BEGIN+i,MF_BYCOMMAND|MF_CHECKED);
					}

					CString strTemp;
					strTemp.Format("%s-%d",ayAllMarket.GetAt(i).m_stTypeName.m_szName,ayAllMarket.GetAt(i).m_nStockType);
					CQuoteTabCtrl::m_mapTempMenuIndex.SetAt(ID_MENU_MARKET_BEGIN+i,strTemp);
					
					nPreMarket = (ayAllMarket.GetAt(i).m_nStockType&0xFF00);
				}
				CPoint point;
				GetCursorPos(&point);
				//pParentWnd->ScreenToClient(&point);
				menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,pParentWnd);
				menu.DestroyMenu();
			}
			else			// 用户自定义菜单
			{

			}
		}
	}

	return TRUE;
}

void HsTCItem::Serialize(CArchive& ar,CWnd* pParent)
{
	char cNext = 0;
	if ( ar.IsStoring() )
	{
	}
	else
	{
	}
}

BOOL HsTCButton::DrawButton(CDC* pDC, CRect& rect, int nMask, int nTabType /* = 1 */)
{
	if (!pDC)
	{
		return FALSE;
	}

	// 字体宽度
	CSize size; 
	COLORREF clTabText = 0;
	COLORREF colorBK = CQuoteTabCtrl::m_iColor->GetQuoteTabBKColor();
	const ColorProperty* colorFrame = NULL;
	COLORREF colorFill = 0;
	CFont* pFont = NULL;
	COLORREF colorArrow = 0;

	int nSaveDC = pDC->SaveDC();


	size = pDC->GetTextExtent(pszText);
	// 计算当前大小 
	CRect curRC = rect;
	curRC.top += 1;

	if (iImage > 0)
	{
	} 
	else
	{
		colorFrame = CQuoteTabCtrl::m_iColor->GetQuoteTabFrameColorProp();	// 边框颜色
		pFont = (CFont*)CQuoteTabCtrl::m_iFont->GetQuoteTabFont();
		switch(nMask)
		{
		case TCIS_BUTTONPRESSED:
			{
				clTabText = CQuoteTabCtrl::m_iColor->GetQuoteActiveTabTextColor();	//字体颜色
				colorFill = CQuoteTabCtrl::m_iColor->GetQuoteActiveTabBKColor();	// 选中时填充色
				colorArrow = colorFrame->m_lColor;
				break;
			}
		case TCIS_DISABLE:
			{
				clTabText = GetSysColor(COLOR_BTNSHADOW);
				colorFill = CQuoteTabCtrl::m_iColor->GetQuoteTabBKColor();
				colorArrow = GetSysColor(COLOR_BTNSHADOW);
				break;
			}
		default:
			{
				clTabText = CQuoteTabCtrl::m_iColor->GetQuoteTabTextColor();
				colorFill = CQuoteTabCtrl::m_iColor->GetQuoteTabBKColor();
				colorArrow = colorFrame->m_lColor;
				break;
			}
		}

		CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
		CBrush brush(colorFill);
		pDC->SelectObject(&pen);
		pDC->SelectObject(&brush);
		pDC->SetTextColor(clTabText);
		pDC->SelectObject(pFont);

		if (mask & TabPrev)
		{
			curRC.right = curRC.left + size.cx;

			//POINT point[7];
			//pDC->MoveTo(curRC.left,curRC.top + 5);
			//point[0].x = curRC.left ;
			//point[0].y = curRC.top + 5 ;
			//point[1].x = curRC.left + 5;
			//point[1].y = curRC.top;
			//point[2].x = curRC.right;
			//point[2].y = curRC.top;
			//point[3].x = curRC.right;
			//point[3].y = curRC.bottom;
			//point[4].x = curRC.left + 5;
			//point[4].y = curRC.bottom;
			//point[5].x = curRC.left;
			//point[5].y = curRC.bottom - 5;
			//point[6] = point[0];
			//pDC->Polygon(point,7);				//选中的tab
			pDC->Rectangle(curRC);

			POINT pt[4];
			CBrush brushTemp(colorArrow);
			pDC->SelectObject(&brushTemp);
			pDC->MoveTo(curRC.left + 3,curRC.CenterPoint().y);
			pt[0].x = curRC.left + 3;
			pt[0].y = curRC.CenterPoint().y;
			pt[1].x = curRC.right - 3;
			pt[1].y = curRC.top + 3;
			pt[2].x = curRC.right - 3;
			pt[2].y = curRC.bottom - 3;
			pt[3] = pt[0];
			pDC->Polygon(pt,4);
			pDC->SelectObject(&brush);
			brushTemp.DeleteObject();

			rect.left = curRC.right - 1;
		} 
		else if (mask & TabNext)
		{
			curRC.right = curRC.left + size.cx;

			//POINT point[7];
			//pDC->MoveTo(curRC.left,curRC.top);
			//point[0].x = curRC.left ;
			//point[0].y = curRC.top ;
			//point[1].x = curRC.right - 5;
			//point[1].y = curRC.top;
			//point[2].x = curRC.right;
			//point[2].y = curRC.top + 5;
			//point[3].x = curRC.right;
			//point[3].y = curRC.bottom - 5;
			//point[4].x = curRC.right - 5;
			//point[4].y = curRC.bottom;
			//point[5].x = curRC.left;
			//point[5].y = curRC.bottom;
			//point[6] = point[0];
			//pDC->Polygon(point,7);				//选中的tab
			pDC->Rectangle(curRC);

			POINT pt[4];
			CBrush brushTemp(colorArrow);
			pDC->SelectObject(&brushTemp);
			pDC->MoveTo(curRC.left + 3,curRC.CenterPoint().y);
			pt[0].x = curRC.left + 3;
			pt[0].y = curRC.top + 3;
			pt[1].x = curRC.right - 3;
			pt[1].y = curRC.CenterPoint().y;
			pt[2].x = curRC.left + 3;
			pt[2].y = curRC.bottom - 3;
			pt[3] = pt[0];
			pDC->Polygon(pt,4);
			pDC->SelectObject(&brush);
			brushTemp.DeleteObject();

			rect.left = curRC.right - 1;
		}
		else
		{
			CBrush brush(colorFill);
			pDC->SelectObject(&brush);
			curRC.left = curRC.right - size.cx - 8;
			rect.right = curRC.left - 15;
			pDC->Rectangle(curRC);
			pDC->DrawText(pszText,curRC,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			brush.DeleteObject();
		}

		rc = curRC;
		pen.DeleteObject();
		brush.DeleteObject();
	}

	pDC->RestoreDC(nSaveDC);
	return FALSE;
}

BOOL HsTCButton::Operator(CWnd* pParentWnd,int nMouse)
{
	return TRUE;
}

// CQuoteTabCtrl

IMPLEMENT_DYNAMIC(CQuoteTabCtrl, CWnd)

CQuoteTabCtrl::CQuoteTabCtrl(IDataSourceEx* pDataSource, long nDataSourceID)
{
	m_nLeftOffset = 0;
	m_nRightOffset = -1;
	m_nCurButton = -1;
	m_nCurItem = -1;
	m_nMouseActiveItem = -1;
	m_nStyle = 0;
	m_nTimerID = 0;

	m_iTab = (IHsTabCfg*)CWinnerApplication::GetObject(OBJ_TAB);
	m_iColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	m_iBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);

	m_pDataSource = pDataSource;
	m_nDataSourceID = nDataSourceID;
}

CQuoteTabCtrl::~CQuoteTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CQuoteTabCtrl, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_COMMAND(QUOTETAB_SETTING,&CQuoteTabCtrl::OnTabSetting)
	ON_COMMAND_RANGE(ID_MENU_MARKET_BEGIN,ID_MENU_MARKET_END,&CQuoteTabCtrl::OnGuideItemMarketMenuMsg)
	ON_COMMAND_RANGE(ID_MENU_BLOCK_BEGIN,ID_MENU_BLOCK_END,&CQuoteTabCtrl::OnGuideItemBlockMenuMsg)
	ON_COMMAND_RANGE(QUOTETAB_TABGROUPS_BEGIN,QUOTETAB_TABGROUPS_END,&CQuoteTabCtrl::OnTabGroupChangeMenuMsg)
END_MESSAGE_MAP()

void CQuoteTabCtrl::DrawTab(CDC *pDC)
{
	int nMask = m_nStyle;
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rc = rcClient;
	rc.top = rcClient.bottom - 1;

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rcClient,RGB(28,28,28));
	pDC->FillSolidRect(&rc, RGB(155,0,0));

	HsTCItem* pTab				= NULL;
	HsTCItem* pActiveTab		= NULL;
	HsTCItem* pMouseActiveTab	= NULL;

	CRect rcDisp = rcClient;
	// 画向导页
	if( m_ayGuideItem.GetCount() > 0 )
	{		
		for(int i = 0; i < m_ayGuideItem.GetSize(); i++)
		{
			pTab = m_ayGuideItem.GetAt(i);
			if( pTab == NULL )
				continue;

			if( !pTab->Draw( pDC, rcDisp, nMask|CQuoteTab_Type_Guide, 0, 1 ) )
				break;
		}
	}
	
	// 画按钮
	if (m_ayButtonItem.GetCount() > 0)
	{
		HsTCButton* pBn = NULL;
		for (int i=0; i<m_ayButtonItem.GetCount(); i++)
		{
			pBn = m_ayButtonItem.GetAt(i);
			if (!(pBn->mask & TabNext))
			{
				if ((pBn->mask & TabPrev) && m_nLeftOffset <= 0)
				{
					nMask |= TCIS_DISABLE;
					nMask &= ~TCIS_BUTTONPRESSED;
				}
				else
				{
					nMask &= ~TCIS_DISABLE;
					if (i == m_nCurButton)
					{
						nMask |= TCIS_BUTTONPRESSED;
					}
					else
					{
						nMask &= ~TCIS_BUTTONPRESSED;
					}
				}

				pBn->DrawButton(pDC,rcDisp,nMask,1);
			}
		}
	}

	// 画标签页
	if ( m_ayItem.GetCount() > 0)
	{
		m_nRightOffset = m_ayItem.GetCount() - 1;
		for(int i = m_nLeftOffset; i < m_ayItem.GetCount(); i++)
		{
			pTab = m_ayItem.GetAt(i);
			if( pTab == NULL )
				continue;

			// 当前页状态
			if (i == m_nCurItem || i == m_nMouseActiveItem)
			{
				nMask = nMask | TCIS_BUTTONPRESSED;
			}
			else
			{
				nMask = nMask & ~TCIS_BUTTONPRESSED;
			}

			if( !pTab->Draw( pDC, rcDisp, nMask|CQuoteTab_Type_Item, 0, 1) )
			{
				m_nRightOffset = i - 1;
				break;
			}
		}
	}
	if (m_ayButtonItem.GetCount() > 0)
	{
		HsTCButton* pBn = NULL;
		for (int i=0; i<m_ayButtonItem.GetCount(); i++)
		{
			pBn = m_ayButtonItem.GetAt(i);
			if (pBn->mask & TabNext)
			{
				nMask &= ~TCIS_BUTTONPRESSED;
				if ((pBn->mask & TabNext) && m_nRightOffset >= m_ayItem.GetCount() - 1 )
				{
					nMask |= TCIS_DISABLE;
				}
				else
				{
					nMask &= ~TCIS_DISABLE;
					if (i == m_nCurButton)
					{
						nMask |= TCIS_BUTTONPRESSED;
					}
				}

				pBn->DrawButton(pDC,rcDisp,nMask,1);
			}
		}
	}

	const ColorProperty* colorFrame = m_iColor->GetQuoteTabFrameColorProp();
	CPen pen(colorFrame->m_nPenStyle,colorFrame->m_nWidth,colorFrame->m_lColor);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(rcDisp.left,rcDisp.bottom);
	pDC->LineTo(rcDisp.left,rcDisp.top);
	pDC->LineTo(rcClient.right,rcDisp.top);

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

int CQuoteTabCtrl::HitTest(CArray<HsTCItem*,HsTCItem*>& ay,CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	HsTCItem* pHsTCItem;
	for( int i = (&ay == &m_ayItem?m_nLeftOffset:0); i < ay.GetCount(); i++)
	{
		pHsTCItem = ay.GetAt(i);
		if( pHsTCItem == NULL )
			continue;

		if( pHsTCItem->rc.left > rect.right ) // 超出
		{
			break;
		}

		if( pHsTCItem->rc.PtInRect( point ) )
		{
			if( &ay == &m_ayItem && 
				m_nRightOffset != -1 &&
				i > m_nRightOffset )
				return -1;

			return i;
		}
	}

	return -1;

}

int CQuoteTabCtrl::HitTest(CArray<HsTCButton*,HsTCButton*>& ay,CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	HsTCButton* pHsTCButton;
	for( int i = 0; i < ay.GetCount(); i++)
	{
		pHsTCButton = ay.GetAt(i);
		if( pHsTCButton == NULL )
			continue;

		if( pHsTCButton->rc.left > rect.right ) // 超出
		{
			break;
		}

		if( pHsTCButton->rc.PtInRect( point ) )
		{
			return i;
		}
	}

	return -1;
}

LONG CQuoteTabCtrl::AddTabItem(CString strName, UINT nID, DWORD dStyle /* = 0 */, PageViewInfo* pageInfo /* = NULL */, CString strGroupName /* = _T */)
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

	TabItemData* pNewItemData = new TabItemData;
	pNewItemData->m_nID = nID;
	pNewItemData->m_dStyle = dStyle;
	pNewItemData->m_strColGroupName = strGroupName;
	pNewItemData->m_PageInfo = pageInfo;
	if (dStyle & TabUsrDefine)
	{
		pNewItemData->m_lMenuID = m_iTab->GetUserDefMenuID(nID);
	}
	else
	{
		pNewItemData->m_lMenuID = dStyle;
	}

	int nCount = 0;
	if (dStyle != 0)
	{
		nCount = m_ayGuideItem.GetCount();
		nCount = InsertGuideItem(0,nCount,strName.GetString(),0,(LPARAM)pNewItemData,0);
	} 
	else
	{
		nCount = m_ayItem.GetCount();
		nCount = InsertItem(0,nCount,strName.GetString(),0,(LPARAM)pNewItemData,0);
	}

	return nCount;
}

LONG CQuoteTabCtrl::InsertGuideItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask /* = 0 */)
{
	HsTCItem* pItem = new HsTCItem;
	pItem->mask    = nMask;
	pItem->iImage  = nImage;
	pItem->lParam  = lParam;
	pItem->pszText.Format("%s",lpszItem);
	pItem->m_nOtherMask = nOtherMask;
	m_ayGuideItem.InsertAt(nItem,pItem);

	return m_ayGuideItem.GetCount();
}

LONG CQuoteTabCtrl::InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam, int nOtherMask /*= 0*/)
{
	HsTCItem* pItem = new HsTCItem;
	pItem->mask    = nMask;
	pItem->iImage  = nImage;
	pItem->lParam  = lParam;
	pItem->pszText.Format("%s",lpszItem);
	pItem->m_nOtherMask = nOtherMask;
	m_ayItem.InsertAt(nItem,pItem);

	return m_ayItem.GetCount();
}

LONG CQuoteTabCtrl::InsertButtonItem(UINT nMask, int nItem, LPCTSTR lpszItem, int nImage, LPARAM lParam,int nOtherMask /* = 0 */)
{
	HsTCButton* pItem = new HsTCButton;
	pItem->mask    = nMask;
	pItem->iImage  = nImage;
	pItem->lParam  = lParam;
	pItem->pszText.Format("%s",lpszItem);
	pItem->m_nOtherMask = nOtherMask;
	m_ayButtonItem.InsertAt(nItem,pItem);

	return m_ayButtonItem.GetCount();
}

BOOL CQuoteTabCtrl::DeleteItem(int nItem)
{
	if( nItem >= 0 && nItem < m_ayItem.GetSize() )
	{
		HsTCItem* pItem = m_ayItem.GetAt(nItem);
		if( pItem )
		{
			delete (TabItemData*)pItem->lParam;
			delete pItem;
			m_ayItem.RemoveAt(nItem);
			this->Invalidate(FALSE);
			return TRUE;
		}
	}
	else if (nItem < 0)
	{
		HsTCItem* pItem = NULL;
		for (int i=0; i<m_ayGuideItem.GetCount(); i++)
		{
			pItem = m_ayGuideItem.GetAt(i);
			if( pItem )
			{
				delete (TabItemData*)pItem->lParam;
				delete pItem;
			}
		}
		m_ayGuideItem.RemoveAll();
		for (int i = 0; i < m_ayItem.GetCount(); i++)
		{
			pItem = m_ayItem.GetAt(i);
			if( pItem )
			{
				delete (TabItemData*)pItem->lParam;
				delete pItem;
			}
		}
		m_ayItem.RemoveAll();
		this->Invalidate(FALSE);
		return TRUE;
	}
	return FALSE;
}

HsTCItem* CQuoteTabCtrl::GetItem(int nItem)
{
	if (nItem >= m_ayItem.GetCount())
	{
		return NULL;
	}
	return m_ayItem.GetAt(nItem);
}

LPARAM CQuoteTabCtrl::GetItemParam(int nItem)
{
	if (nItem >= m_ayItem.GetCount())
	{
		return NULL;
	}
	return m_ayItem.GetAt(nItem)->lParam;
}

int CQuoteTabCtrl::FindTabItem(CString strName)
{
	HsTCItem* pCur = NULL;
	for( int i = 0; i < m_ayItem.GetSize(); i++)
	{
		pCur = m_ayItem.GetAt(i);
		if( !pCur->pszText.CompareNoCase(strName) )
		{
			return i;
		}
	}

	// 若没有找到，则考虑更换标签组后再查找

	return -1;

}

int CQuoteTabCtrl::EnSureVisible(CString strName)
{
	int nIndex = FindTabItem(strName);
	return EnSureVisible(nIndex);
}

int CQuoteTabCtrl::EnSureVisible(int nIndex)
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

CString CQuoteTabCtrl::GetDefaultTabGroupName()
{
	CString strName = m_iTab->GetDefaultTabGroup();
	if (strName.IsEmpty())
	{
		strName = _T("股票市场");
	}
	return strName;
}

BOOL CQuoteTabCtrl::ChangeTabGroup(CString strGroup /* = _T */)
{
	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价标签-切换标签组：标签组名称 %s",strGroup);

	if (strGroup.IsEmpty())
	{
		strGroup = GetDefaultTabGroupName();
	}
	if (m_strCurGroupName == strGroup)
	{
		return FALSE;
	}
	m_strCurGroupName = strGroup;
	m_nCurItem = -1;

	PageViewInfo pageInfo;
	CTabItemArray ayTabItem;
	LONG nCount = m_iTab->GetTabList(strGroup,ayTabItem);
	if (nCount <= 0)
	{
		return FALSE;
	}

	DeleteItem(-1);
	TabItem* item;
	for (int i=0; i<ayTabItem.GetCount(); i++)
	{
		item = ayTabItem.GetAt(i);
		pageInfo.m_lPageType = item->m_lPageType;
		pageInfo.m_nInPageTag = item->m_lPageTag;
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			pageInfo.m_sMarket[i] = item->m_sMarket[i];
		}
		memcpy(pageInfo.m_szBlock,item->m_szBlock,BLOCK_NAME_LENGTH);
		AddTabItem(item->m_szName,item->m_ID,item->m_lMenuMask,&pageInfo,item->m_szColGroupName);
	}
	CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价标签-完成初始化，默认切换到第一项");
	ChangeCurItem(0);
	Invalidate(FALSE);

	return TRUE;
}


BOOL CQuoteTabCtrl::ChangeCurItem(CString strName, BOOL bNotify /* = TRUE */, short nMarkType /* = 0 */)
{
	int nIndex = FindTabItem(strName);
	if (nIndex < 0)		// 若在所有分组内都没有找到指定标签，则单独添加
	{
		//单独添加
		PageViewInfo pageinfo;
		pageinfo.m_lPageType = PriceAnalisys1;
		if (nMarkType > 0)
		{
			pageinfo.m_nInPageTag = PT_LIST|PT_TYPE;
			pageinfo.AddMarketType(nMarkType);
		} 
		else
		{
			pageinfo.m_nInPageTag = PT_LIST|PT_BLOCK;
			strncpy(pageinfo.m_szBlock,strName.GetString(),BLOCK_NAME_LENGTH);
		}
		AddTabItem(strName,0,0,&pageinfo);
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价标签-未发现%s标签，新建标签页，市场类别（0为板块）：%d",strName,nMarkType);
		
		return ChangeCurItem(strName);
	}
	return ChangeCurItem(nIndex, bNotify);
}

BOOL CQuoteTabCtrl::ChangeCurItem(int nIndex, BOOL bNotify /* = TRUE */)
{
	if (nIndex < 0)
	{
		return FALSE;
	}
	if (m_nCurItem == nIndex)
	{
		return TRUE;
	}
	m_nCurItem = nIndex;
	EnSureVisible(nIndex);
	Invalidate(FALSE);

	CWnd* pParent = GetParent();
	if (pParent && bNotify)
	{
		pParent->SendMessage(QUOTETAB_NOTIFY,(WPARAM)nIndex,(LPARAM)(GetItem(nIndex)->lParam));
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CQuoteTabCtrl 消息处理程序


int CQuoteTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	int nCount = m_ayButtonItem.GetCount();
	InsertButtonItem(TabPrev,nCount++,_T("←"),0,NULL,0);
	InsertButtonItem(TabNext,nCount++,_T("→"),0,NULL,0);
	InsertButtonItem(TabOpertor,nCount++,_T("设置"),0,NULL,0);
	return 0;
}

void CQuoteTabCtrl::OnPaint()
{
	CPaintDC cdc(this); // device context for painting
	// TODO: 在此添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()

	{
		CMemDC dc(&cdc);
		DrawTab(&dc);
	}
}

void CQuoteTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nIndex = HitTest(m_ayGuideItem,point);
	if( nIndex != -1 )
	{
		m_ayGuideItem.GetAt(nIndex)->Operator(this,WM_LBUTTONDOWN);
		return;
	}

	nIndex = HitTest(m_ayButtonItem,point);
	if( nIndex != -1 )
	{
		HsTCButton* pBn = m_ayButtonItem.GetAt(nIndex);
		pBn->Operator(this,WM_LBUTTONDOWN);
		OnButtonClickMsg(pBn->mask);

		if (pBn->mask&HsTCButton::TabPrev || pBn->mask&HsTCButton::TabNext )
		{
			SetCapture();
			m_nTimerID = SetTimer((long)pBn->mask,100,NULL);
		}

		return;
	}

	nIndex = HitTest(m_ayItem,point);
	if( nIndex != -1 )
	{
		HsTCItem* pItem = m_ayItem.GetAt(nIndex);
		if( m_nCurItem == nIndex )
		{
		}
		else
		{
			ChangeCurItem(nIndex);
		}
	}
}

void CQuoteTabCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQuoteTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nIndex = 0;
	m_nCurButton = -1;
	m_nMouseActiveItem = -1;
	if( (nIndex = HitTest(m_ayGuideItem,point)) != -1 )
	{
	}
	else if ((nIndex = HitTest(m_ayItem,point)) != -1)
	{
		if (nIndex != m_nMouseActiveItem)
		{
			m_nMouseActiveItem = nIndex;
		}
	}
	else if ((nIndex = HitTest(m_ayButtonItem,point)) != -1)
	{
		m_nCurButton = nIndex;
	}
	Invalidate(FALSE);

	TRACKMOUSEEVENT t = 
	{
		sizeof(TRACKMOUSEEVENT),
		TME_LEAVE,
		m_hWnd,
		0
	};
	::TrackMouseEvent(&t);
	//CWnd::OnMouseMove(nFlags, point);
}

void CQuoteTabCtrl::OnMouseLeave()
{
	m_nMouseActiveItem = -1;
	m_nCurButton = -1;
	this->Invalidate(FALSE);
}

void CQuoteTabCtrl::OnDestroy()
{
	CWnd::OnDestroy();

	for (int i = 0; i < m_ayGuideItem.GetCount(); i++)
	{
		delete (TabItemData*)(m_ayGuideItem.GetAt(i)->lParam);
		delete m_ayGuideItem.GetAt(i);
	}

	for (int i = 0; i < m_ayItem.GetCount(); i++)
	{
		delete (TabItemData*)(m_ayItem.GetAt(i)->lParam);
		delete m_ayItem.GetAt(i);
	}

	for (int i=0; i<m_ayButtonItem.GetCount(); i++)
	{
		delete m_ayButtonItem.GetAt(i);
	}

	m_ayGuideItem.RemoveAll();
	m_ayItem.RemoveAll();
	m_ayButtonItem.RemoveAll();

}

void CQuoteTabCtrl::OnButtonClickMsg(UINT nID)
{
	switch(nID)
	{
	case HsTCButton::TabPrev:
		{
			OnScrollTCItem(-1);
			break;
		}
	case HsTCButton::TabNext:
		{
			OnScrollTCItem(1);
			break;
		}
	case HsTCButton::TabOpertor:
		{
			CMenu menu;
			if (menu.CreatePopupMenu())
			{
				menu.InsertMenu(0,MF_BYCOMMAND|MF_STRING,QUOTETAB_SETTING,_T("设置"));
				menu.InsertMenu(1,MF_SEPARATOR);
				m_ayTempGroups.RemoveAll();
				m_iTab->GetAllTabGroups(m_ayTempGroups);
				for (int i=0; i<m_ayTempGroups.GetCount(); i++)
				{
					menu.AppendMenu(MF_BYCOMMAND|MF_STRING,QUOTETAB_TABGROUPS_BEGIN+i,m_ayTempGroups.GetAt(i));
					if (m_strCurGroupName == m_ayTempGroups.GetAt(i))
					{
						menu.CheckMenuItem(QUOTETAB_TABGROUPS_BEGIN+i,MF_BYCOMMAND|MF_CHECKED);
					}
				}
				POINT point;
				GetCursorPos(&point);
				menu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);

				menu.DestroyMenu();
			}

			break;
		}
	case HsTCButton::TabRestoreWin:
		{
			break;
		}
	case HsTCButton::TabClose:
		{
			break;
		}
	}
}

void CQuoteTabCtrl::OnGuideItemMarketMenuMsg(UINT nID)
{
	CString strMenuText;
	if (m_mapTempMenuIndex.Lookup(nID,strMenuText) && !strMenuText.IsEmpty())
	{
		int nIndex = strMenuText.Find("-");
		CString strName = strMenuText.Left(nIndex);
		HSMarketDataType nMarket = atoi(strMenuText.Right(strMenuText.GetLength()-nIndex-1));
		ChangeCurItem(strName,TRUE,nMarket);
	}
}

void CQuoteTabCtrl::OnGuideItemBlockMenuMsg(UINT nID)
{
	CString strMenuText;
	if (m_mapTempMenuIndex.Lookup(nID,strMenuText) && !strMenuText.IsEmpty())
	{
		ChangeCurItem(strMenuText);
	}
}
void CQuoteTabCtrl::OnTabGroupChangeMenuMsg(UINT nID)
{
	if (m_ayTempGroups.GetCount() <= 0)
	{
		return;
	}
	int nBegin = QUOTETAB_TABGROUPS_BEGIN;
	int nCount = nID - nBegin;
	CString strGroup = m_ayTempGroups.GetAt(nCount);
	ChangeTabGroup(strGroup);
}

void CQuoteTabCtrl::OnTabSetting()
{
	CQuoteTabSetting tabSettingDlg(this);
	if (tabSettingDlg.DoModal() == IDOK)
	{
		CString strGroup = tabSettingDlg.GetCurGroup();
		ChangeTabGroup(strGroup);
	}
}

void CQuoteTabCtrl::OnScrollTCItem(int nCount)
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
	Invalidate(FALSE);
}
void CQuoteTabCtrl::OnTimer(UINT_PTR nIDEvent)
{
	OnButtonClickMsg(nIDEvent);
}

void CQuoteTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	if (m_nTimerID)
	{
		KillTimer(m_nTimerID);
		m_nTimerID = 0;
	}
	CWnd::OnLButtonUp(nFlags, point);
}
