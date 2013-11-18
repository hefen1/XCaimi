#include "stdafx.h"
#include "DrawBox.h"
#include "TechKLineMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
//
CDrawBox::CDrawBox(CTechKLineMgr* pMgr, IRegionNode* pRegion)
{
	m_penBorder.lopnStyle = g_hsGraphicsData.clBoxLineWidth.m_nPenStyle;
	m_penBorder.lopnWidth.x = g_hsGraphicsData.clBoxLineWidth.m_nWidth;;
	m_penBorder.lopnWidth.y = g_hsGraphicsData.clBoxLineWidth.m_nWidth;;    
	m_penBorder.lHorColor = g_hsGraphicsData.clBoxLineWidth.m_lColor;
	m_penBorder.lVerColor = g_hsGraphicsData.clBoxLineWidth.m_lColor;

	m_pTeckKLineMgr = pMgr;
	m_pRegion = pRegion;

	m_nType = PKLINE_TYPE_FENSHI;
	m_nRowCount = 14;
}

CDrawBox::~CDrawBox()
{

}

BOOL CDrawBox::SetRect(CRect rc)
{
	ColInfoStruct colInfo;
	colInfo.nWidth = rc.Width();
	SetColInfo(0, &colInfo);

	CRect rcSet = rc;
	if (m_nType == PKLINE_TYPE_FENSHI)
	{
		m_nRowCount = 14;
		rcSet.bottom = rcSet.top + 252;
	}
	else if (m_nType == PKLINE_TYPE_K)
	{
		m_nRowCount = 22;
		rcSet.bottom = rcSet.top + 396;
	}
	SetRowCount(m_nRowCount);

	return CDrawTableBase::SetRect(rcSet);
}

BOOL CDrawBox::ReCreate(short nType)
{
	m_nType = nType;
	RemoveAllCell();
	SetColCount(1);

	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE | TABLE_HEIGHT_CHANGABLE);
	DWORD lItemStyleLeft = ITEM_TEXT_LEFT;
	DWORD lItemStyleRight = ITEM_TEXT_RIGHT;
	if (m_nType == PKLINE_TYPE_FENSHI)
	{
		m_nRowCount = 14;
	}
	else if (m_nType == PKLINE_TYPE_K)
	{
		m_nRowCount = 22;
	}	
	for (int i = 0; i < m_nRowCount; i ++)
	{
		if ( i % 2 == 0)
		{
			CreateCell(_T(""), TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyleLeft);
		}
		else 
		{
			CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyleRight);
		}
	}
	SetRect(m_rect);
	return TRUE;
}

void CDrawBox::SetDateTime(CString strDate, CString strTime)
{
	m_strDate = strDate;
	m_strTime = strTime;
}

void CDrawBox::SetFenshiFixedData()
{
	ReCreate(PKLINE_TYPE_FENSHI);
	CPoint pt(0, 0);
	SetCellText(pt, 0, _T("日期"));
	pt.y += 2;
	SetCellText(pt, 0, _T("时间"));
	pt.y += 2;
	SetCellText(pt, 0, _T("最新"));
	pt.y += 2;
	SetCellText(pt, 0, _T("涨跌"));
	pt.y += 2;
	SetCellText(pt, 0, _T("幅度"));
	pt.y += 2;
	SetCellText(pt, 0, _T("均价"));
	pt.y += 2;
	SetCellText(pt, 0, _T("手数"));
}

void CDrawBox::SetKLineFixedData()
{
	ReCreate(PKLINE_TYPE_K);
	CPoint pt(0, 0);
	SetCellText(pt, 0, _T("日期"));
	pt.y += 2;
	SetCellText(pt, 0, _T("开盘"));
	pt.y += 2;
	SetCellText(pt, 0, _T("最高"));
	pt.y += 2;
	SetCellText(pt, 0, _T("最低"));
	pt.y += 2;
	SetCellText(pt, 0, _T("收盘"));
	pt.y += 2;
	SetCellText(pt, 0, _T("成交"));
	pt.y += 2;
	SetCellText(pt, 0, _T("金额"));
	pt.y += 2;
	SetCellText(pt, 0, _T("涨跌幅"));
	pt.y += 2;
	SetCellText(pt, 0, _T("换手"));
	pt.y += 2;
	SetCellText(pt, 0, _T("振幅"));
	pt.y += 2;
	SetCellText(pt, 0, _T("值"));
}

void CDrawBox::UpdateData(StockUserInfo* pStock, DrawFenShiData* pFenshiData, long* plAvePrice, long* plHand)
{
	if (pStock != NULL && pFenshiData != NULL && !::IsBadReadPtr(pFenshiData, sizeof(DrawFenShiData)))
	{
		SetFenshiFixedData();
		CPoint pt(0, 1);
		SetCellText(pt, 0, m_strDate);
		pt.y += 2;
		SetCellText(pt, 0, m_strTime);
		pt.y += 2;

		byte bClrFlag = TEXT_COLOR_FLAT;
		if (pStock->m_lPrevClose > pFenshiData->m_lValue)
		{
			bClrFlag = TEXT_COLOR_DOWN;
		}
		else if (pStock->m_lPrevClose < pFenshiData->m_lValue)
		{
			bClrFlag = TEXT_COLOR_UP;
		}

		char szText[64];
		CGeneralHelper::LongToString(pFenshiData->m_lValue, 0, pStock->m_nPriceUnit, pStock->m_nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		SetCellText(pt, 0, szText, bClrFlag);
		pt.y += 2;

		long lUpDown = pFenshiData->m_lValue - pStock->m_lPrevClose;
		CGeneralHelper::LongToString(lUpDown, 0, pStock->m_nPriceUnit, pStock->m_nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		WORD wArrow = (bClrFlag == TEXT_COLOR_UP ? ITEM_ARROW_UP : ITEM_ARROW_DOWN);
		WORD wArrowDel = (bClrFlag == TEXT_COLOR_UP ?  ITEM_ARROW_DOWN : ITEM_ARROW_UP);
		SetCellText(pt, 0, szText, bClrFlag, 0,wArrow, wArrowDel);
		pt.y += 2;

		CString strRatio;
		if (pStock->m_lPrevClose > 0)
		{
			strRatio.Format("%.2f%%", (float)lUpDown / pStock->m_lPrevClose * 100);
		}
		else 
			strRatio = " - ";
		SetCellText(pt, 0, strRatio, bClrFlag);

		pt.y += 2;
		CGeneralHelper::LongToString(*plAvePrice, 0, pStock->m_nPriceUnit, pStock->m_nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		SetCellText(pt, 0, szText, bClrFlag);

		pt.y += 2;
		CGeneralHelper::LongToString(*plHand, 0, 100, 0, szText, LTOA_MASK_HORLINE_IFZERO | LTOA_MASK_DIVISION_TRUNCATION);
		SetCellText(pt, 0, szText, TEXT_COLOR_TOTALHAND);

	}
}

void CDrawBox::UpdateData(StockDay* pStockDayData, StockUserInfo* pStock, long lPreClose, long lFinaceTotalHand, CString strValue)
{
	if (pStockDayData != NULL && !::IsBadReadPtr(pStockDayData, sizeof(StockDay)) && m_pTeckKLineMgr != NULL && pStock != NULL)
	{
		SetKLineFixedData();
		if (lPreClose == 0)
		{
			lPreClose = pStockDayData->m_lOpenPrice;
		}

		int nUnit = pStock->m_nPriceUnit;
		int nDecimal = pStock->m_nDecimal;

		char szText[64] = {0};
		CGeneralHelper::LongToString(pStockDayData->m_lDate % 1000000, 0, 1, -6, szText); 
		CString strWeek = "/" + CGeneralHelper::GetWeekDayString(pStockDayData->m_lDate);
		strcpy(szText,(char*)(LPCTSTR)(CString(szText) + strWeek));


		CPoint pt(0, 1);
		SetCellText(pt, 0, szText);

		WORD wClr;
		GetCellItemAttribute(pStockDayData->m_lOpenPrice, lPreClose, wClr);
		CGeneralHelper::LongToString(pStockDayData->m_lOpenPrice, ((lPreClose == 0) ? pStockDayData->m_lOpenPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		pt.y += 2;
		SetCellText(pt, 0, szText, wClr);

		GetCellItemAttribute(pStockDayData->m_lMaxPrice, lPreClose, wClr);
		CGeneralHelper::LongToString(pStockDayData->m_lMaxPrice, ((lPreClose == 0) ? pStockDayData->m_lMaxPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		pt.y += 2;
		SetCellText(pt, 0, szText, wClr);

		GetCellItemAttribute(pStockDayData->m_lMinPrice, lPreClose, wClr);
		CGeneralHelper::LongToString(pStockDayData->m_lMinPrice, ((lPreClose == 0) ? pStockDayData->m_lMinPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		pt.y += 2;
		SetCellText(pt, 0, szText, wClr);

		GetCellItemAttribute(pStockDayData->m_lClosePrice, lPreClose, wClr);
		CGeneralHelper::LongToString(pStockDayData->m_lClosePrice, ((lPreClose == 0) ? pStockDayData->m_lClosePrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
		pt.y += 2;
		SetCellText(pt, 0, szText, wClr);

		pt.y += 2;
		CString strTmp;
		//成交量
		long lAmount = (long)pStockDayData->m_lTotal;
		if(lAmount > 999999999)
		{
			CGeneralHelper::LongToString(lAmount, 0, 100000000, 0,  szText);
			strTmp = szText;
			strTmp += _T("亿");
		}
		else if(lAmount > 999999)
		{
			CGeneralHelper::LongToString(lAmount, 0, 10000, 0,  szText);
			strTmp = szText;
			strTmp += _T("万");
		}
		else
		{
			CGeneralHelper::LongToString(lAmount, 0, 1, 0, szText);
			strTmp = szText;
		}	
		SetCellText(pt, 0, strTmp, TEXT_COLOR_TOTALHAND);


		long lMoney = (long)pStockDayData->m_lMoney;
		if(lMoney > 99999999) 
		{
			CGeneralHelper::LongToString(lMoney, 0, 100000000, nDecimal, szText);
			strTmp = szText;
			strTmp += _T("千亿");
		}
		else if(lMoney > 99999)
		{
			CGeneralHelper::LongToString(lMoney, 0, 100000, nDecimal, szText);
			strTmp = szText;
			strTmp += _T("亿");
		}
		else if( lMoney > 9)
		{ 
			CGeneralHelper::LongToString(lMoney, 0, 10, 0, szText);
			strTmp = szText;
			strTmp += _T("万");
		}
		else
		{
			CGeneralHelper::LongToString(lMoney, 0, 1, 0, szText);
			strTmp = szText;
		}
		pt.y += 2;
		SetCellText(pt, 0, strTmp, wClr);

		// 涨跌幅
		if ( pStockDayData->m_lClosePrice != 0 )
		{
			CGeneralHelper::LongToString((pStockDayData->m_lClosePrice - lPreClose) * 100, 
				0, lPreClose, 2, szText, 0x04);
		}
		else
		{
			CGeneralHelper::LongToString((long)0, 0, lPreClose, 2, szText, 0x04);
		}
		strTmp = szText;
		if (strTmp != " -")
		{
			strTmp += _T("%");
		}
		pt.y += 2;
		SetCellText(pt, 0, strTmp, wClr);

		//换手
		if (lFinaceTotalHand > 0)
		{
			strTmp.Format("%.2f%%",pStockDayData->m_lTotal / (float)lFinaceTotalHand * (float)100);	
		}
		else 
			strTmp = _T(" - ");
		pt.y += 2;
		SetCellText(pt, 0, strTmp, wClr);

		CGeneralHelper::LongToString((pStockDayData->m_lMaxPrice - pStockDayData->m_lMinPrice) * 100,
			0, lPreClose, 2, szText,0x04);
		strTmp = szText;
		if (strTmp != " -")
		{
			strTmp += _T("%");
		}
		pt.y += 2;
		SetCellText(pt, 0, strTmp, wClr);

		pt.y += 2;
		SetCellText(pt, 0, strValue);
	}
}

BOOL CDrawBox::GetCellItemAttribute(long lDataTarget, long lDataSource, WORD& wClr)
{
	wClr = TEXT_COLOR_FLAT;
	if (lDataTarget > lDataSource && lDataSource != 0)
	{
		wClr = TEXT_COLOR_UP;
	}
	else if (lDataTarget < lDataSource)
	{
		wClr = TEXT_COLOR_DOWN;
	}
	return TRUE;
}

int CDrawBox::Draw(CDC* pDC)
{
	CRect rc = GetRect();
	CDrawCell* pCell = GetCell(2);

	int nRet = CDrawTableBase::Draw(pDC);
	if (!m_rect.IsRectEmpty())
	{
		CPen pen;
		if (pen.CreatePenIndirect(&m_penBorder.GetPen(HsLOGPEN::ver)))
		{
			CPen* pOldPen = (CPen*)pDC->SelectObject(&pen);
			pDC->MoveTo(m_rect.left - 1, m_rect.top);
			pDC->LineTo(m_rect.right, m_rect.top);
			pDC->LineTo(m_rect.right, m_rect.bottom);
			pDC->LineTo(m_rect.left - 1, m_rect.bottom);
			pDC->LineTo(m_rect.left - 1, m_rect.top);
			pDC->SelectObject(pOldPen);
		}
		if(pen.m_hObject != NULL)
		{
			pen.DeleteObject();
		}
	}
	return 0;
}

BOOL  CDrawBox::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if(m_rect.PtInRect(point) && m_pTeckKLineMgr != NULL)
	{
		MSG msg;
		CPoint pt;
		CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();

		CPoint ptLast = point;
		SetCapture(pWnd->m_hWnd);
		for(;;)
		{
			VERIFY(::GetMessage(&msg, NULL, 0, 0));

			switch(msg.message)
			{
			case WM_MOUSEMOVE:
				{
					pt.x = (int)(short)LOWORD(msg.lParam);
					pt.y = (int)(short)HIWORD(msg.lParam);
					if(pt == ptLast)
						break;

					CRect rcMove = m_rect;
					CPoint ptMove = CPoint( pt.x - ptLast.x, pt.y - ptLast.y);
					rcMove.OffsetRect(ptMove);
					CRect rcComb = m_rect;
					CDrawTableBase::SetRect(rcMove);
					rcComb.UnionRect(rcComb, rcMove);
					rcComb.InflateRect(1, 1, 1, 1);
					pWnd->InvalidateRect(rcComb, FALSE);
					ptLast = pt;
					break;
				}
			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:			
				goto __ExitLoop__;
			default:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					break;
				}
			}
		}
__ExitLoop__:

		ReleaseCapture();
		return TRUE;
	}
	return FALSE;
}