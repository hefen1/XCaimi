#include "stdafx.h"
#include "DrawHistrendRTable.h"
#include "GraphicsData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
CDrawHisTrendRTable::CDrawHisTrendRTable()
{
	m_nRowCount = 13;
	m_nColCount = 2;

	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE );
	CreateTable();
}

CDrawHisTrendRTable::~CDrawHisTrendRTable()
{

}

BOOL CDrawHisTrendRTable::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem)
{
	if (pos.y == 0 )
	{
		rcItem.top = 0;
		CString strText;
		GetCellText(pos, nItemID, strText);
		
		CDrawItem* pItem = GetItem(pos, nItemID);
		if (pItem != NULL)
		{
			COLORREF clr = DrawTool::GetColor(pItem->m_wColor);
			CFont* pFont = DrawTool::GetFont(pItem->m_wFont);
			if (pFont != NULL)
			{
				LOGFONT lf ;
				pFont->GetLogFont(&lf);
				lf.lfHeight -= 4;
				
				CFont fontTitle;
				if(fontTitle.CreateFontIndirect(&lf))
				{
					COLORREF clrOld = pDC->SetTextColor(clr);
					CFont* pFontOld = (CFont*)pDC->SelectObject(&fontTitle);
					if(pItem->m_str != NULL && pItem->m_str[0] != '\0')
						pDC->DrawText(pItem->m_str, strlen(pItem->m_str), rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

					pDC->SelectObject(pFontOld);
					pDC->SetTextColor(clrOld);
				}
				if(fontTitle.m_hObject)
				{
					fontTitle.DeleteObject();
				}
				return TRUE;
			}
		}
	}
	return CDrawTableBase::DrawSelf(pDC, pos, nItemID, rcItem);
}

BOOL CDrawHisTrendRTable::CreateTable()
{
	SetRowCount(m_nRowCount);
	SetRowHeight(18);
	SetColCount(m_nColCount);
	DWORD lItemStyleLeft = ITEM_TEXT_LEFT;
	DWORD lItemStyleRight = ITEM_TEXT_RIGHT;
	DWORD lCellStyle = CELL_BORDER_BOTTOM; 

	for (int i = 0; i < m_nRowCount; i ++)		//先第一列
	{
		if (i == 0 || i == 8 || i == m_nRowCount -1)
		{
			CreateCell(_T(""), TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyleLeft, lCellStyle);
		}
		else
			CreateCell(_T(""), TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, lItemStyleLeft);

	}
	for (int i = 0; i < m_nRowCount; i ++)	//第二列
	{
		if (i == 0 || i == 8 || i == m_nRowCount -1)
		{
			CreateCell(_T(""), 0, TEXT_FONT_DYNAMICAL, lItemStyleRight, lCellStyle);
		}
		else
			CreateCell(_T(""), 0, TEXT_FONT_DYNAMICAL, lItemStyleRight);
	}

	return FALSE;
}

BOOL CDrawHisTrendRTable::UpdateData(StockUserInfo* pStock, StockDay* pStockDayData, long lPreClose, long lFinaceTotalHand)
{
	CPoint pt(0, 0);
	SetCellText(pt, 0, pStock->m_ciStockCode.GetCode(), TEXT_COLOR_STOCKCODE, 0, ITEM_TEXT_CENTER, ITEM_TEXT_LEFT);
	pt.y ++;
	SetCellText(pt, 0, _T("日期"));
	pt.y ++;
	SetCellText(pt, 0, _T("开盘价"));
	pt.y ++;
	SetCellText(pt, 0, _T("最高价"));
	pt.y ++;
	SetCellText(pt, 0, _T("收盘价"));
	pt.y ++;
	SetCellText(pt, 0, _T("最低价"));
	pt.y ++;
	SetCellText(pt, 0, _T("成交量"));
	pt.y ++;
	SetCellText(pt, 0, _T("成交额"));
	pt.y ++;
	SetCellText(pt, 0, _T("均价"));
	pt.y ++;
	SetCellText(pt, 0, _T("涨跌值"));
	pt.y ++;
	SetCellText(pt, 0, _T("涨跌幅"));
	pt.y ++;
	SetCellText(pt, 0, _T("振幅"));
	pt.y ++;
	SetCellText(pt, 0, _T("换手率"));

	int nUnit = pStock->m_nPriceUnit;
	int nDecimal = pStock->m_nDecimal;
	pt.x = 1; pt.y = 0;
	SetCellText(pt, 0, pStock->GetName(), TEXT_COLOR_STOCKNAME, 0, ITEM_TEXT_CENTER, ITEM_TEXT_RIGHT);

	pt.y ++;
	char szText[64] = {0};
	CGeneralHelper::LongToString(pStockDayData->m_lDate % 1000000, 0, 1, -6, szText); 
	CString strWeek = "/" + CGeneralHelper::GetWeekDayString(pStockDayData->m_lDate);
	strcpy(szText,(char*)(LPCTSTR)(CString(szText) + strWeek));
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	CGeneralHelper::LongToString(pStockDayData->m_lOpenPrice, ((lPreClose == 0) ? pStockDayData->m_lOpenPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	CGeneralHelper::LongToString(pStockDayData->m_lMaxPrice, ((lPreClose == 0) ? pStockDayData->m_lMaxPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);
	pt.y ++;
	CGeneralHelper::LongToString(pStockDayData->m_lClosePrice, ((lPreClose == 0) ? pStockDayData->m_lClosePrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);
	pt.y ++;
	CGeneralHelper::LongToString(pStockDayData->m_lMinPrice, ((lPreClose == 0) ? pStockDayData->m_lMinPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);
	
	pt.y ++;
	CString strTmp;
	//成交量
	if(pStockDayData->m_lTotal > 999999999)
	{
		CGeneralHelper::LongToString(pStockDayData->m_lTotal, 0, 100000000, 0,  szText);
		strTmp = szText;
		strTmp += _T("亿");
	}
	else if(pStockDayData->m_lTotal > 999999)
	{
		CGeneralHelper::LongToString(pStockDayData->m_lTotal, 0, 10000, 0,  szText);
		strTmp = szText;
		strTmp += _T("万");
	}
	else
	{
		CGeneralHelper::LongToString(pStockDayData->m_lTotal, 0, 1, 0, szText);
		strTmp = szText;
	}	
	SetCellText(pt, 0, strTmp, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	if(pStockDayData->m_lMoney > 99999999) 
	{
		CGeneralHelper::LongToString(pStockDayData->m_lMoney, 0, 100000000, nDecimal, szText);
		strTmp = szText;
		strTmp += _T("千亿");
	}
	else if(pStockDayData->m_lMoney > 99999)
	{
		CGeneralHelper::LongToString(pStockDayData->m_lMoney, 0, 100000, nDecimal, szText);
		strTmp = szText;
		strTmp += _T("亿");
	}
	else if( pStockDayData->m_lMoney > 9)
	{ 
		CGeneralHelper::LongToString(pStockDayData->m_lMoney, 0, 10, 0, szText);
		strTmp = szText;
		strTmp += _T("万");
	}
	else
	{
		CGeneralHelper::LongToString(pStockDayData->m_lMoney, 0, 1, 0, szText);
		strTmp = szText;
	}
	SetCellText(pt, 0, strTmp, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	long lAvgPrice = (long)((double long)pStockDayData->m_lMoney * 10000/ (double long)pStockDayData->m_lTotal);
	CGeneralHelper::LongToString(lAvgPrice, ((lPreClose == 0) ? pStockDayData->m_lOpenPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	CGeneralHelper::LongToString(pStockDayData->m_lClosePrice - lPreClose, ((lPreClose == 0) ? pStockDayData->m_lOpenPrice: lPreClose), nUnit, nDecimal, szText, LTOA_MASK_HORLINE_IFZERO);
	SetCellText(pt, 0, szText, TEXT_COLOR_STOCKNAME);

	pt.y ++;
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
	strTmp += _T("%");
	SetCellText(pt, 0, strTmp, TEXT_COLOR_STOCKNAME);

	pt.y ++;
	CGeneralHelper::LongToString((pStockDayData->m_lMaxPrice - pStockDayData->m_lMinPrice) * 100,
		0, lPreClose, 2, szText,0x04);
	strTmp = szText;
	strTmp += _T("%");
	SetCellText(pt, 0, strTmp, TEXT_COLOR_STOCKNAME);
	
	pt.y ++;
	//换手
	if (lFinaceTotalHand > 0)
	{
		strTmp.Format("%.2f%%",(float)pStockDayData->m_lTotal / (float)lFinaceTotalHand * (float)100);	
	}
	else 
		strTmp = _T(" - ");
	SetCellText(pt, 0, strTmp, TEXT_COLOR_STOCKNAME);
	return FALSE;
}