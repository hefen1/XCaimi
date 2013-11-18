//QuoteTableCtrlGeneralSort.cpp实现文件


#include "stdafx.h"
#include "QuoteGeneralSortTabCtrl.h"
#include "QuoteGeneralSortDlg.h"


#include "GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// #ifdef Support_XHX_Dll 
// #define Negative(x,y)  ( y ? x&0x0FFFFFFF : x )
// #define NegativeFlag(x,y) ( y ? ( ( x&0xF0000000 ) ? 1 : 0 ) : 0  ) 
// #else
// #define Negative(x,y)   x
// #define NegativeFlag(x,y) 0
// #endif

int g_LastSelectWnd = -1;
extern UINT g_SplitterLBClickMsg;
extern CArray<StockUserInfo*,StockUserInfo*>	g_pStock;
extern StockUserInfo* GetStockUserInfo(const char* code, HSMarketDataType type);
extern void RefreashOtherWnd(int nIndex);

IMPLEMENT_DYNCREATE(QuoteTableCtrlGeneralSort, CHSGridTable)
BOOL  g_bOldMode = 1;



QuoteTableCtrlGeneralSort::QuoteTableCtrlGeneralSort()
{
	m_sDisplayRow = 9;
	SetEditable(FALSE);
	SetListMode();
	EnableSelection(TRUE);
	SetFixedRowSelection(TRUE);
	SetHeaderSort();
	SetGridLines(GVL_NONE);
	SetColumnResize();
	SetRowResize(FALSE);
	SetVirtualMode(FALSE);
	SetDoubleBuffering();
	SetAlwaysShowSelection(FALSE);
	SetFrameFocusCell(FALSE);

	AllowSelectRowInFixedCol(FALSE);
	EnableDragRowMode(FALSE);
	EnableDragAndDrop(FALSE);
	EnableTitleTips(FALSE);

	SetDefCellHeight(24);
	SetDefCellMargin(0);
	
	m_iColProp = (IHsColumnProperty*)CWinnerApplication::GetObject(OBJ_COLUMNPRO);
	m_iColorFontProt = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iFont  = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	if (m_iColorFontProt )
	{
		SetGridBkColor(m_iColorFontProt->GetBlackgroundColor());
		SetFixedBkColor(m_iColorFontProt->GetBlackgroundColor());
	}
	else
	{
		SetGridBkColor(0);
		SetFixedBkColor(0);
	}

}

QuoteTableCtrlGeneralSort::~QuoteTableCtrlGeneralSort()
{
	DeleteStock();
	m_ayColInfo.RemoveAll();
};

BEGIN_MESSAGE_MAP(QuoteTableCtrlGeneralSort, CHSGridCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
END_MESSAGE_MAP()

int QuoteTableCtrlGeneralSort::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CHSGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	

	// TODO: Add your specialized creation code here

	return 0;
}
char* QuoteTableCtrlGeneralSort::GetRequestData(int nPacketIndex, int& lRetLen, int nMask)
{
	
	int nCurLen = sizeof(ReqGeneralSortEx) / sizeof(CodeInfo) +
		((sizeof(ReqGeneralSortEx) / sizeof(CodeInfo))?1:0);

	lRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCurLen - 1);


	AskData* pskData = (AskData*) (new char[lRetLen]);
	memset(pskData,0,lRetLen);

	pskData->m_nType  = RT_GENERALSORT_EX;
	pskData->m_nIndex = nPacketIndex;
	pskData->m_nSize  = nCurLen;
	pskData->m_lKey   = (long)this;
	//if( (nMask & SC_Union) == SC_Union ) // 组合分类
	//{
	//	nMask = STOCK_MARKET| SH_BOURSE| SZ_BOURSE| KIND_STOCKA;
	//}
	pskData->m_nPrivateKey.m_pCode.m_cCodeType = nMask;

	ReqGeneralSortEx* pReqGeneralSort = (ReqGeneralSortEx*)pskData->m_pCode;

	CRect rectClient;
	GetClientRect(&rectClient);
	pReqGeneralSort->m_nRetCount = (short)(((rectClient.Height()/*-SPLITTERGAP*/) * 1.0 /*/ 3 */)/ GetDefCellHeight()) - 1;

	if( pReqGeneralSort->m_nRetCount <= 0 )
	{
		pReqGeneralSort->m_nRetCount = 6;
	}

	m_sDisplayRow = pReqGeneralSort->m_nRetCount;
	
	pReqGeneralSort->m_cCodeType = nMask;
	#ifdef Support_CompAsk
		pReqGeneralSort->m_nSortType = RT_RISE | RT_FALL | RT_5_RISE
										| RT_5_FALL | RT_AHEAD_COMM | RT_AFTER_COMM
										| RT_AHEAD_PRICE | RT_AHEAD_VOLBI | RT_AHEAD_MONEY;
	#else
		/*pReqGeneralGeneralSort->m_nSortType = m_pTableFlag->nType;*/
	#endif

	#ifdef SUPPORT_NEW_GENERALGeneralSort
		/*DWORD dwTime = m_pviInfo.m_nInPageTag&0xFF000000;
		pReqGeneralGeneralSort->m_nMinuteCount = dwTime >> 24;*/
	#endif
	
	return (char*)pskData;	
	
}
//void  QuoteTableCtrlGeneralSort::InitTableData()
//{
//	SetRowCount(m_sDisplayRow);
//}


BOOL QuoteTableCtrlGeneralSort::HandleRecvData(AnsGeneralSortEx* pData, int nIndex)
{
	GeneralSortData* pReportTotalData = (GeneralSortData*)pData->m_prptData;

	DeleteStock();
	int nDataNumber = 0;
	StockUserInfo* pStockInfo;
	long lMinsize = min(m_sDisplayRow, pData->m_nSize);
	for (int i = 0; i < lMinsize; i++)
	{
 		pStockInfo = GetStockUserInfo(pReportTotalData[i].m_ciStockCode.m_cCode,
 			pReportTotalData[i].m_ciStockCode.m_cCodeType);
		if ( pStockInfo != NULL)
		{
				AddStock(pStockInfo);
				nDataNumber++;
		}
	}
	if (nDataNumber < 1)
		return FALSE;
	
	InitRowData();
	//DrawTitle();
	long nUnit;
	short nDecimal;
	long  lClose;
	char  strText[64];
	int col;
	CGridCell* pCell;
	int nFixedRows = 1;
	for(int i = 0; i < nDataNumber; i++)
	{
	 
		pStockInfo = m_pStock.GetAt(i);

        lClose	 = pStockInfo->m_lPrevClose;
		nUnit    = 1000;
	    nDecimal = 2;
		
		//特殊处理价格
		BOOL bNegative = FALSE;
		#ifdef Support_XHX_Dll
			bNegative = MakeNegativeIndex(pStockInfo->m_ciStockCode.m_cCodeType);
		#endif

		col = 0;	
		pCell = (CGridCell*)GetCell(nFixedRows+i, col);
		if( pCell == NULL )
			continue;
		SetFixedData(nFixedRows+i,col,pCell,(StockUserInfo*)pStockInfo, -1, TRUE);
		
		// 价格
		if ( Negative(pReportTotalData[i].m_lNewPrice,bNegative) == 0 )
			CGeneralHelper::LongToString(lClose, lClose, nUnit, nDecimal, strText);
		else
			CGeneralHelper::LongToString(Negative(pReportTotalData[i].m_lNewPrice,bNegative), lClose, nUnit, nDecimal, strText);
		UpdateCellText(nFixedRows+i,1,strText,GRID_DATA_UPDATE_DEFAULT);
		
		// 排名值

 		switch(nIndex)
		{		     
		case 1:   // 5分钟涨幅排名
		case 4:   // 5分钟跌幅排名
			{
				long lNewClose = 0;
				if((pReportTotalData[i].m_lValue*((g_bOldMode)?1:10000) + 10000) > 0)
				{
					lNewClose = (long)(Negative(pReportTotalData[i].m_lNewPrice,bNegative) * 10000 /
						(pReportTotalData[i].m_lValue*((g_bOldMode)?1:10000) + 10000));
				}
				/*
				if(lNewClose > 0)
				{
					pStockInfo->m_lPrevClose = lNewClose;
				}
				*/
			}
			CGeneralHelper::LongToString((long)(pReportTotalData[i].m_lValue*((g_bOldMode)?1:10000)),0,100,nDecimal,strText, 0x0400);  
			break;

		case 2:   // 今日委比前六名
		case 5:   // 今日委比后六名
		case 6:   // 今日震幅排名
			CGeneralHelper::LongToString((long)(pReportTotalData[i].m_lValue*((g_bOldMode)?1:10000)),0,100,2,strText, 0x0400);  
			break;

		case 0:   // 今日涨幅排名
		case 3:   // 今日跌幅排名
			{
				CGeneralHelper::LongToString((long)(pReportTotalData[i].m_lValue*((g_bOldMode)?1:10000)),0,100,nDecimal,strText, 0x0400);  
				break;			
			}
			
		case 7:   // 今日量比排名
			CGeneralHelper::LongToString((long)(pReportTotalData[i].m_lValue),0,100,2,strText);
			break;
		case 8:   // 今日总金额排名				 
			CGeneralHelper::LongToString((long)(pReportTotalData[i].m_lValue),0,100,2,strText); // 服务器除以100传过来的机会
			break;
		default:
		    break;
		}
		UpdateCellText(nFixedRows+i,2,strText,GRID_DATA_UPDATE_DEFAULT );
	}
	Invalidate();
	return TRUE;
}
void QuoteTableCtrlGeneralSort::OnDraw(CDC* pDC)
{
	CHSGridCtrl::OnDraw(pDC);

	CRect rect, itemrect, rccvlient;
	GetClientRect(rccvlient);
	ClientToScreen(rccvlient);
	GetClientRect(rect);
	itemrect = rect;
	if (GetRowCount() <=0)
		return;

	int rowheight = GetRowHeight(0);
	itemrect.bottom = rowheight-1;
	
	COLORREF bkcolor = GetBkColor();
	COLORREF textcolor = RGB(155,155,155);
	if (m_iColorFontProt)
		textcolor = m_iColorFontProt->GetQuoteTabTextColor();
	CFont* pFont;
	CFont* pOldFont;
	BOOL bCreat = FALSE;
	if (m_iFont)
		pFont = (CFont*)m_iFont->GetQuoteColHeadFont();
	else
	{
		pFont = new CFont();
		VERIFY(pFont->CreateFont(16,0, 0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_SWISS, _T("宋体")));
		bCreat = TRUE;
	}

	pOldFont = pDC->SelectObject(pFont);

	
	pDC->SetBkColor( GetSysColor(COLOR_HIGHLIGHT) );
	pDC->SetBkColor(bkcolor);
	pDC->SetTextColor(textcolor);

	pDC->SetBkColor(bkcolor);
	pDC->DrawText(m_pTableFlag->strTitle, &itemrect, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

	pDC->SelectObject(pOldFont);
	if (bCreat)
		pFont->DeleteObject();

	
	
	CPen hPen;
	hPen.CreatePen(PS_SOLID ,1,RGB(200,0,0));
	CPen* hOldPen = pDC->SelectObject(&hPen);

	pDC->MoveTo(CPoint(itemrect.left,itemrect.bottom));
	pDC->LineTo(CPoint(itemrect.right, itemrect.bottom));

	pDC->SelectObject(hOldPen);
	hPen.DeleteObject();

	
}
void QuoteTableCtrlGeneralSort::DrawTitle()
{
	CGridCellBase* pCell;
	for (int col =0; col<GetColumnCount(); col++)
	{

		pCell = GetCell(0, col);
		pCell->SetText("");		
	}
}
void QuoteTableCtrlGeneralSort::AddStock(StockUserInfo* pStock)
{
	m_pStock.Add(pStock);
}
void QuoteTableCtrlGeneralSort::DeleteStock()
{
// 	for (int i=0;i<m_pStock.GetCount(); i++)
// 	{
// 		StockUserInfo* pStock = m_pStock.GetAt(i);
// 		if (pStock)
// 			delete pStock;
// 	}
	m_pStock.RemoveAll();	
}
void  QuoteTableCtrlGeneralSort::InitColData()
{
	ModifyStyleEx(WS_EX_CLIENTEDGE,0,0);
	AddColumn(COLUMN_HQ_BASE_NAME);	
	AddColumn(COLUMN_HQ_BASE_NEW_PRICE);
	AddColumn(COLUMN_HQ_BASE_MONEY);
	
	CRect rect;
	GetClientRect(rect);
	int width = 0;
	for (int i=0; i<3; i++)
	{
		int columnwidth = rect.Width()/3 ;
		
		if (i == 2)
			SetColumnWidth(i, rect.Width()-width/*+3*/);
		else
			SetColumnWidth(i, columnwidth );
		width += columnwidth;
	}

}
void QuoteTableCtrlGeneralSort::InitRowData()
{
	int count = m_pStock.GetCount();
	SetRowCount(count+1);
	InitFixedDataItem(count+1,1);
}
void QuoteTableCtrlGeneralSort::InitFixedDataItem(int nCount,int nFirst)
{

	// 行
	COLORREF bkcolor = RGB(0,0,0);
	if (m_iColorFontProt)
	   bkcolor = m_iColorFontProt->GetBlackgroundColor();
	StockUserInfo* pStock = NULL;
	CGridCell* pCell;

	int row,col;

	int height = GetDefCellHeight();
	
	CRect rect;
	GetClientRect(rect);
	for ( row = 1; row < nCount; row++)
	{
		pStock = m_pStock.GetAt(row-1);
// 		int rowheight = rect.Height()-height;
// 		if (row == nCount-1)
// 			SetRowHeight(nCount-1, rowheight);
// 		height += GetDefCellHeight();
		if( pStock )
		{
			for ( col = 0; col < GetColumnCount()/* && col < maxVisibleCol*/; col++)
			{
		
				//
				pCell = (CGridCell*)(GetCell(row, col));
				if( pCell == NULL )
					continue;
				if (col == 0)
				{				
					CFont* pFont;
					if (m_iFont)
						pFont = (CFont*)m_iFont->GetQuoteStringFont();
					if (pFont)
					{
						LOGFONT font;
						ZeroMemory(&font,sizeof(LOGFONT));
						pFont->GetLogFont(&font);
						pCell->SetFont(&font);
					}
				}
				else
				{
					CFont* pFont;
					if (m_iFont)
						pFont = (CFont*)m_iFont->GetQuoteDynamicFont();
					if (pFont)
					{
						LOGFONT font;
						ZeroMemory(&font,sizeof(LOGFONT));
						pFont->GetLogFont(&font);
						pCell->SetFont(&font);
					}
				}
				pCell->SetBackClr(bkcolor);
// 				pCell->SetTextClr(m_iColProp->GetColumnColor(GetHeadID(col)));
			
				pCell->SetState(pCell->GetState()|m_iColProp->GetColumnMask(GetHeadID(col)));
			
				pCell->SetFormat(pCell->GetFormat()|DT_SINGLELINE|DT_LEFT|DT_CENTER|DT_VCENTER);
				SetFixedData( row,col,pCell,pStock, -1,FALSE,NULL );
			}
		}
	}
	this->Invalidate();
}


void QuoteTableCtrlGeneralSort::SetFlag(QuoteTableCtrlGeneralSort::TableFlag* pTableFlag)
{
	m_pTableFlag = pTableFlag;
	m_pTableFlag->pTable = this;
}
int QuoteTableCtrlGeneralSort::GetColIndexByID(UINT uiID)
{
	int index = -1;
	switch(uiID)
	{
	case COLUMN_HQ_BASE_NAME:
		index = 0;
		break;
	case COLUMN_HQ_BASE_NEW_PRICE:
		index = 1;
		break;
	case COLUMN_HQ_BASE_MONEY:
		index = 2;
		break;
	default:
		break;
	}
	return index;
}
// CTableColume* QuoteTableCtrlGeneralSort::GetColumn(UINT nID,int& nPos)
// {
// 	nPos = GetColIndexByID(nID);
// 	if (nPos >= 0 && nPos < m_ayColInfo.GetCount())
// 	{
// 		CTableColume* pTabCol = m_ayColInfo.GetAt(nPos);
// 		if (pTabCol && !IsBadReadPtr(pTabCol,1))
// 		{
// 			return pTabCol;
// 		}
// 	}
// 	return NULL;
// }
void QuoteTableCtrlGeneralSort::AddColumn(UINT nID)
{
	if (m_iColProp == NULL)
		return;
	const TableColProperty* pCol = m_iColProp->GetColumn(nID);
	if( pCol )
	{
		int bFixedCol = FALSE;
		if( nID == COLUMN_HQ_BASE_NAME		|| 
			nID == COLUMN_HQ_BASE_CODE		||
			nID == COLUMN_HQ_BASE_NUMBER	||
			nID == COLUMN_HQ_BASE_INFO_MARK ||
			nID == COLUMN_BSORDER_ORDER		||
			nID == COLUMN_HQ_DETAIL_TREND_Time	 || // 分时明细表
			nID == COLUMN_HQ_DETAIL_TECH_TIME    || // 盘后明细
			nID == COLUMN_HQ_DETAIL_INDEX_TIME   || // 大盘明细
			nID == COLUMN_HQ_BASE_NEW_PRICE ||	// 成交价
			nID == COLUMN_HQ_BASE_ARROW
			)
		{
			bFixedCol = TRUE;
		}

		int nOldPos = 0;
		nOldPos = GetColIndexByID(nID);
		if( nOldPos < 0 )
		{
			return;
		}
// 		CTableColume* pOldCol = GetColumn(nID,nOldPos);
// 		if( pOldCol != NULL && nOldPos != -1 )
// 		{
// 			return;
// 		}
// 
// 		CTableColume* pCol = new CTableColume(nID);
// 		if (!pCol || IsBadReadPtr(pCol,1))
// 		{
// 			return;
// 		}

		//ADD BY YUHJ 2008.5.30
//  		if(nID == COLUMN_HQ_BASE_OPEN || nID == COLUMN_HQ_BASE_NEW_PRICE)
//  			pCol->mask |= GVIF_VALUEABS;
// 		//END
// 		//20081111 add by fangly begin: 最高，最低排序时对数字取
// 		if(nID == COLUMN_HQ_BASE_MAX_PRICE || nID == COLUMN_HQ_BASE_MIN_PRICE)
// 			pCol->mask |= GVIF_VALUEABS;
// 		if(nID == COLUMN_HQ_BASE_RISE_RATIO)
// 			pCol->mask &= ~GVIF_VALUEABS;
// 		//20081111 add by fangly end
// 
// 		if(nID == COLUMN_HQ_DETAIL_TREND_RISE_VALUE) //20090326 add by fangly end 分时成交涨跌去掉前面的箭头
// 		{
// 			pCol->mask = GVIF_SYMBOL|GVIF_VALUEABS;
// 		}

		if( bFixedCol )
		{		
			switch(nID)
			{
			case COLUMN_HQ_BASE_NAME:
				{
					if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE) >= 0 )
					{
						nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK) >=0 )
					{
						//				nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW) >=0 )
					{
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER) >=0 )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;
			case COLUMN_HQ_BASE_CODE:
				{
					if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME) >=0 )
					{
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK) >=0 )
					{
						break;
					}				
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW) >=0 )
					{
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER) >=0 )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;
			case COLUMN_HQ_BASE_INFO_MARK:
				{
					if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME) >=0 )
					{
						nOldPos++;
						break;
					}				
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW) >=0 )
					{
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE) >=0 )
					{
						nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER) >=0 )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;

			case COLUMN_HQ_BASE_ARROW:
				{
					if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK) >=0 ) 
					{
						nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME) >=0 )
					{
						nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE) >=0 )
					{
						nOldPos++;
						break;
					}
					else if( nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER) >=0 )
					{
						nOldPos++;
						break;
					}
					else
					{
						nOldPos = 0;
					}
				}
				break;

			case COLUMN_HQ_BASE_NUMBER:
			default:  break;
			}

			m_ayColInfo.InsertAt(nOldPos,nID);			
		}
		else
		{
			int nIndex = m_ayColInfo.Add(nID);
		}

		CString strHead = "";
		int nWidth = m_iColProp->GetColumnWidth(nID);
		for (int i = 0; i < nWidth; i++)
		{
			strHead.AppendChar('A');
		}

		int nCol = InsertColumn(strHead,/*m_iColProp->GetColumnFormat(nID)|*/DT_SINGLELINE|DT_VCENTER,nOldPos);
		SetItemText(0,nCol,"");

//  		if (bFixedCol)
//  		{
//  			SetFixedColumnCount(GetFixedColumnCount() + 1);
//  		}
	}
}


UINT QuoteTableCtrlGeneralSort::GetHeadID(int nPos)
{
	if (nPos < 0)
	{
		return 0;
	}
	return m_ayColInfo.GetAt(nPos);
}
void QuoteTableCtrlGeneralSort::SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock /* = NULL */,
									 int nNumber /* = -1 */, BOOL bRefresh /* = TRUE */, CDC* pDC /* = NULL */)
{
	UINT nID = this->GetHeadID(nCol);
	if( nID <= 0 )
		return;

	CString strText; 
	//财务数据 股本数据
	if( ( nID >= COLUMN_CAPITALIZATION_TOTAL && nID <= COLUMN_CAPITALIZATION_A2_GIVE ) 
		||	( nID >= COLUMN_FINANCE_TOTAL_ASSETS && nID <= COLUMN_FINANCE_ASSETS_YIELD ) )
	{
	}
	else if (nID >= COLUMN_BLOCK_BEGIN && nID <= COLUMN_BLOCK_END)		//add by lxqi 20090706	板块信息
	{
	}
	else
	{


		switch(nID)
		{
		case COLUMN_HQ_BASE_INFO_MARK:
			if( pStock )
			{
				pCell->SetText(pStock->m_ciStockCode.GetCode());
			}
			else
			{
				pCell->SetText("");
			}
			break;
		case COLUMN_HQ_BASE_CODE: // 股票代码
			if( pStock )
			{				
				pCell->SetText(pStock->GetCode());
				pCell->SetData((LPARAM)pStock);
			}
			break;
		case COLUMN_HQ_BASE_NAME: 		// 股票名称
			if( pStock )
			{
				COLORREF coltest = m_iColorFontProt->GetFixedNameColor();
				pCell->SetTextClr(coltest);
				pCell->SetText(pStock->GetName());
				pCell->SetData((LPARAM)pStock);
			
			}
			break;
		case COLUMN_HQ_BASE_NEW_PRICE:  //成交价
			{				
				COLORREF downCol = RGB(0,255,0);
				COLORREF upcol   = RGB(255,0,0);
				if (m_iColorFontProt)
				{
					downCol = m_iColorFontProt->GetDownColor();
					upcol   = m_iColorFontProt->GetUpColor();
				}
				int nindex = m_pTableFlag->nIndex;
				if (nindex == 4 || nindex == 3 || nindex==5)
					pCell->SetTextClr(downCol);
				else
					pCell->SetTextClr(upcol);
				break;
			}
		case COLUMN_HQ_BASE_MONEY: //成交金额
			{
				COLORREF downCol = RGB(0,255,0);
				COLORREF upcol   = RGB(255,0,0);
				if (m_iColorFontProt)
				{
					downCol = m_iColorFontProt->GetDownColor();
					upcol   = m_iColorFontProt->GetUpColor();
				}
				int nindex = m_pTableFlag->nIndex;
				if (nindex == 4 || nindex == 3 || nindex==5)
					pCell->SetTextClr(downCol);
				else
					pCell->SetTextClr(upcol);
				break;
			}
		case COLUMN_HQ_BASE_NUMBER:   	// 序号
			{
				if( nNumber == -1 )
				{
					CString strTemp;
					strTemp.Format("%i",nRow - m_nFixedRows + 1);
					pCell->SetText(strTemp);
				}
				else
				{
					CString strTemp;
					strTemp.Format("%i",nNumber);
					pCell->SetText(strTemp);
				}				
			}
			break;
		case COLUMN_HQ_BASE_PRECLOSE:   // 昨收
			{
				if( pStock == NULL )
					break;

				// for test
				CString strTemp;
				strTemp.Format("%.2f",(float)(pStock->m_lPrevClose)/1000);
				pCell->SetText(strTemp);
			}
			break;
			// gbq test 20060620
		case COLUMN_HQ_BASE_ARROW:
			{
				pCell->SetText("!");
			}
			break;
			// gbq end
		default:
			{
				bRefresh = FALSE;
			}
			break;
		}
	}
/*	pCell->SetFont(m_iColorFontProt->GetQuoteTabFontProt()->GetFont());*/
	pCell->SetFormat(pCell->GetFormat()/*|m_iColProp->GetColumnFormat(nID)*/|DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	if( bRefresh )
	{
// 		if (GetDoubleBuffering())
// 		{
// 			CMemDC memDC(pDC);
// 			RedrawCell( nRow, nCol, &memDC );
// 		} 
// 		else
// 		{
			RedrawCell( nRow, nCol, pDC );
		/*}*/
	}
}
//清除上次选中行
void QuoteTableCtrlGeneralSort::RefreshWnd()
{
	COLORREF color = GetGridBkColor();
	CRect rectCell, rectRow;
	CCellID cell =  GetCellFromPt(m_LastDownPoint);

	CGridCellBase* pCell = GetCell(cell.row, cell.col);
	BOOL bInTextArea = FALSE;
	if (pCell)
	{

		if (GetCellRect(cell.row, cell.col, rectCell) &&
			pCell->GetTextRect(rectCell))
		{
			bInTextArea = rectCell.PtInRect(m_LastDownPoint);
		}
	}

	if ( bInTextArea )
	{
		for (int i=0; i<GetColumnCount(); i++)
		{
			pCell = GetCell(cell.row, i);
		/*	pCell->GetBackClr();*/
			if (pCell)
			{
				pCell->SetState(/*pCell->GetState() | */m_iColProp->GetColumnMask(GetHeadID(i)));
				pCell->SetBackClr(color);
			}
		}
	}
	Invalidate();
}
void QuoteTableCtrlGeneralSort::OnLButtonDown(UINT nFlags, CPoint point)
{ 	
	/*CHSGridCtrl::OnLButtonDown(nFlags, point);*/
	SetFocus();
	RefreshWnd();
	RefreashOtherWnd(g_LastSelectWnd);
	g_LastSelectWnd = m_pTableFlag->nIndex;
	CRect rectCell, rectRow;
	CCellID cell =  GetCellFromPt(point);
	if (cell.row == 0) return;
	CGridCellBase* pCell = GetCell(cell.row, cell.col);
	
 	BOOL bInTextArea = FALSE;
 	if (pCell)
 	{
 
 		if (GetCellRect(cell.row, cell.col, rectCell) &&
 			pCell->GetTextRect(rectCell))
 		{
 			bInTextArea = rectCell.PtInRect(point);
 		}
 	}
 	COLORREF selectbk = RGB(100,100,100);
 	if ( bInTextArea )
 	{
 		for (int i=0; i<GetColumnCount(); i++)
 		{
 			pCell = GetCell(cell.row, i);
 			if (pCell)
			{
				pCell->SetState(pCell->GetState() | m_iColProp->GetColumnMask(GetHeadID(i))| GVIS_SELECTED);
				pCell->SetSelectColor(selectbk);		
			}
 		}
 	}
	
  	Invalidate();
	m_LastDownPoint = point;
}