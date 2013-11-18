// QuoteReoprtTable.cpp : 实现文件
//

#include "stdafx.h"
#include "QuoteReport.h"
#include "QuoteReportTable.h"
#include "hscoldefine.h"
#include "hsothercoldefine.h"
#include <sys/stat.h>
#include "quotesort.h"
#include "GeneralHelper.h"
#include "WinnerApplication.h"
#include "../QuoteComm/pageid.h"
#include "quotecolumnsetting.h"
#include "blockadscriptiondlg.h"
#include "HSBizDialog.h"
#include "WinnerApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CQuoteReportTable

IMPLEMENT_DYNAMIC(CQuoteReportTable, CHSGridTable)

CQuoteReportTable::CQuoteReportTable(IDataSourceEx* pDataSouce, long nDataSourceID, int nRows /* = 0 */, int nCols /* = 0 */, int nFixedRows /* = 0 */, int nFixedCols /* = 0 */)
:CHSGridTable(nRows,nCols,nFixedRows,nFixedCols)
{
	m_nCurActiveRow = -1;
	m_bAllowDraw = TRUE;
	m_bResizeColWidth = FALSE;
	
	m_pHorScrollBar = NULL;
	m_pVerScrollBar = NULL;

	m_pDataSource = pDataSouce;
	m_nDataSourceID = nDataSourceID;
	m_iColProp = (IHsColumnProperty*)CWinnerApplication::GetObject(OBJ_COLUMNPRO);
	m_iColorProt = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_iFontProt = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	m_iUserDefCfg = (IHsUserDataCfg*)CWinnerApplication::GetObject(OBJ_USERDATA);
	m_iBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	m_iQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);

	m_nRefreshTimerID = 0;
	m_nDelayRealTimeRequestTimerID = 0;
	m_nDelaySortRequestTimerID = 0;

	m_bNeedRequestSort = FALSE;

	SetSortColumn(-1);
	SetEditable(FALSE);
	SetListMode();
	EnableSelection();
	SetFixedRowSelection(TRUE);
	SetHeaderSort();
	SetGridLines(GVL_NONE);
	SetColumnResize();
	SetRowResize(FALSE);
	SetVirtualMode(FALSE);
	SetDoubleBuffering();
	SetAlwaysShowSelection();
	SetFrameFocusCell(FALSE);
	SetTotalMode(FALSE);
	SetFilterMode(FALSE);

	AllowSelectRowInFixedCol(true);
	EnableDragRowMode();

	SetDefCellHeight(25);
	SetDefCellMargin(2);

	// 设置单元格字体
	LOGFONT logfont;
	ZeroMemory(&logfont,sizeof(LOGFONT));
	CFont* pFont = (CFont*)m_iFontProt->GetQuoteColHeadFont();
	if (pFont)
	{
		pFont->GetLogFont(&logfont);
		SetFixedRowFont(&logfont);
		SetFixedRowColFont(&logfont);
	}

	ZeroMemory(&logfont,sizeof(LOGFONT));
	pFont = (CFont*)m_iFontProt->GetQuoteDynamicFont();
	if (pFont)
	{
		pFont->GetLogFont(&logfont);
		SetNonFixedFont(&logfont);
	}
	// 设置单元格背景
	SetBkColor(m_iColorProt->GetBlackgroundColor());
	GetDefaultCell(FALSE,FALSE)->SetBackClr(m_iColorProt->GetBlackgroundColor());
	SetFixedRowColBkColor(m_iColorProt->GetQuoteFixedRowColBKColor());
	SetFixedRowBkColor(m_iColorProt->GetQuoteFixedRowBKColor());
	SetFixedColBkColor(m_iColorProt->GetQuoteFixedColBKColor());

	CImageList* pImageList = new CImageList;
	pImageList->Create(IDB_ALL_ICON,16,105,RGB(0,128,128));
	SetImageList(pImageList);
}

CQuoteReportTable::~CQuoteReportTable()
{
	EmptyStock();
	if (GetImageList())
	{
		delete GetImageList();
	}
}


BEGIN_MESSAGE_MAP(CQuoteReportTable, CHSGridTable)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_COLUMN_SETTING,OnColumnSetting)
	ON_COMMAND(ID_MARK_STRING,OnMarkCodeByString)
	ON_COMMAND_RANGE(ID_MARK_TAG_BEGIN, ID_MARK_TAG_END, OnMarkCodeByIcon)
	ON_COMMAND(ID_CANCEL_MARK_CODE, OnCancelMarkCode)
	ON_COMMAND(ID_ADD_TO_BLOCK, OnAddToBlock)
	ON_COMMAND(ID_ADD_TO_SELFBLOCK, OnAddToSelfBlock)
	ON_COMMAND(ID_BLOCK_ADSCRIPTION, OnBlockAdscription)
	ON_COMMAND_RANGE(ID_QUOTE_DISP_COLGROUP_BEGIN, ID_QUOTE_DISP_COLGROUP_END, OnColGroupChange)
END_MESSAGE_MAP()

BOOL CQuoteReportTable::IsFixedColumn(int nID)
{
	if( nID == COLUMN_HQ_BASE_NAME		|| 
		nID == COLUMN_HQ_BASE_CODE		||
		nID == COLUMN_HQ_BASE_NUMBER	||
		nID == COLUMN_HQ_BASE_INFO_MARK ||
		nID == COLUMN_HQ_BASE_ARROW
		)
	{
		return TRUE;
	}
	return FALSE;
}

int CQuoteReportTable::GetColIndexByID(UINT uiID)
{
	int nPos = -1;
	if (!m_mapColumn.Lookup(uiID,nPos))
	{
		return -1;
	}
	return nPos;
}

UINT CQuoteReportTable::GetColHeadID(int nPos /* = 0 */)
{
	if (nPos<0 || nPos>=m_ayColInfo.GetCount())
	{
		return 0;
	}
	return m_ayColInfo.GetAt(nPos);
}

int CQuoteReportTable::SetColumnWidth(int nColID,int nWidth /* = -1 */)
{
	int nPos = 0;
	nPos = GetColIndexByID(nColID);

	CClientDC dc(this);
	short tmLen = (short)dc.GetTextExtent("0").cx;

	return CHSGridTable::SetColumnWidth(nPos,nWidth*tmLen);
}

int CQuoteReportTable::DelColHead(int nBegin /* = -1 */,int nEnd /* = -1 */)
{
	if(nBegin < 0)
		nBegin = 0;
	if(nEnd < 0)
		nEnd = m_ayColInfo.GetCount();

	if (nBegin == 0 && nEnd == m_ayColInfo.GetCount())
	{
		DeleteAllItems();
	} 
	else
	{
		for (int i=nBegin; i<nEnd-nBegin; i++)
		{
			DeleteColumn(i);
		}
	}
	m_ayColInfo.RemoveAt(nBegin,nEnd-nBegin);

	m_nFixedCols = 0;

	ResetCorrespondMap();

	return m_ayColInfo.GetCount();
}

void CQuoteReportTable::ResetCorrespondMap()
{
	m_mapColumn.RemoveAll();
	int nSize = m_ayColInfo.GetSize();
	if( nSize <= 0 )
		return;

	m_mapColumn.InitHashTable((UINT)(nSize + nSize*0.2));
	UINT nColID = 0;
	for(int i = 0; i < nSize; i++)
	{
		nColID = m_ayColInfo.GetAt(i);
		m_mapColumn.SetAt(nColID,i);
	}
}

StockUserInfo* CQuoteReportTable::FindRowByCode(CodeInfo* pCode, int& nPos)
{
	if( pCode == NULL )
		return NULL;

	// 获取可见范围的股票行
	CCellRange VisCellRange = GetVisibleNonFixedCellRange();
	int minVisibleRow = VisCellRange.GetMinRow() - GetFixedRowCount();
	int maxVisibleRow = VisCellRange.GetMaxRow() - GetFixedRowCount();

	StockUserInfo* pStock;
	for(int i=minVisibleRow ;i <= maxVisibleRow; i++ )
	{
		pStock = GetStock(i);
		if(pStock == NULL)
			continue;

		if( pStock->m_ciStockCode.CompareCode(pCode))
		{
			nPos = i + GetFixedRowCount();
			return pStock;
		}
	}

	for (int i=0; i<minVisibleRow; i++)
	{
		pStock = GetStock(i);
		if(pStock == NULL)
			continue;

		if( pStock->m_ciStockCode.CompareCode(pCode))
		{
			nPos = i + GetFixedRowCount();
			return pStock;
		}
	}

	for (int i=maxVisibleRow; i<GetRowCount()-GetFixedRowCount(); i++)
	{
		pStock = GetStock(i);
		if(pStock == NULL)
			continue;

		if( pStock->m_ciStockCode.CompareCode(pCode))
		{
			nPos = i + GetFixedRowCount();
			return pStock;
		}
	}

	return NULL;

}

BOOL CQuoteReportTable::InitColGroup(CString strGroupName, BOOL bRefreshData /* = FALSE */)
{
	if ((strGroupName.IsEmpty() && m_strCurColGroup==m_iColProp->GetDefDispColGroupName())
		|| (!strGroupName.IsEmpty() && strGroupName==m_strCurColGroup))
	{
		return FALSE;
	} 
	else if( strGroupName.IsEmpty() )
	{
		strGroupName = m_iColProp->GetDefDispColGroupName();
	}
	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-初始化列：列组名称：%s，重新请求数据：%d",strGroupName,bRefreshData);

	DelColHead();


// 	CArray<CStringArray*,CStringArray*>* ayColTemplList;
// 	CStringArray* pArray;
// 
// 
// 	ayColTemplList = m_iColProp->GetColHeadTemplate(strGroupName);
	CUIntArray* pAyColTemp = NULL;
	m_iColProp->GetDispColGroup(strGroupName, pAyColTemp);
	if (pAyColTemp ==NULL || pAyColTemp->GetCount() <= 0 || strGroupName == _T("缺省") )
	{
		AddColumn(COLUMN_HQ_BASE_NUMBER  );
		AddColumn(COLUMN_HQ_BASE_CODE		);
		AddColumn(COLUMN_HQ_BASE_NAME		);
		AddColumn(COLUMN_HQ_BASE_NEW_PRICE		);
		AddColumn(COLUMN_HQ_BASE_RISE_RATIO);
		AddColumn(COLUMN_HQ_BASE_TOTAL_HAND);
		AddColumn(COLUMN_HQ_BASE_HAND);
		AddColumn(COLUMN_HQ_BASE_PRECLOSE);
		AddColumn(COLUMN_HQ_BASE_OPEN);
		AddColumn(COLUMN_HQ_BASE_MAX_PRICE);
		AddColumn(COLUMN_HQ_BASE_MIN_PRICE);
		AddColumn(COLUMN_HQ_BASE_BUY_PRICE);
		AddColumn(COLUMN_HQ_BASE_SELL_PRICE);
		AddColumn(COLUMN_HQ_BASE_RISE_VALUE);	
		AddColumn(COLUMN_HQ_BASE_MONEY);
		AddColumn(COLUMN_HQ_BASE_RANGE);
		AddColumn(COLUMN_HQ_EX_EXHAND_RATIO);
		AddColumn(COLUMN_HQ_BASE_ORDER_RATIO);
	}
	else
	{
		WORD wKey = 0;
		int nOldPos = 0;
		int nWidth = 0;
		for(int i = 0; i < pAyColTemp->GetCount(); i++)
		{
			wKey = pAyColTemp->GetAt(i++);
			AddColumn( wKey );

			nWidth = pAyColTemp->GetAt(i);
			SetColumnWidth(wKey,nWidth);			
		}
	}

// 	if(!ayColTemplList || ayColTemplList->GetCount() <= 0 || strGroupName == _T("缺省"))
// 	{
// 		AddColumn(COLUMN_HQ_BASE_NUMBER  );
// 		AddColumn(COLUMN_HQ_BASE_CODE		);
// 		AddColumn(COLUMN_HQ_BASE_NAME		);
// 		AddColumn(COLUMN_HQ_BASE_NEW_PRICE		);
// 		AddColumn(COLUMN_HQ_BASE_RISE_RATIO);
// 		AddColumn(COLUMN_HQ_BASE_TOTAL_HAND);
// 		AddColumn(COLUMN_HQ_BASE_HAND);
// 		AddColumn(COLUMN_HQ_BASE_PRECLOSE);
// 		AddColumn(COLUMN_HQ_BASE_OPEN);
// 		AddColumn(COLUMN_HQ_BASE_MAX_PRICE);
// 		AddColumn(COLUMN_HQ_BASE_MIN_PRICE);
// 		AddColumn(COLUMN_HQ_BASE_BUY_PRICE);
// 		AddColumn(COLUMN_HQ_BASE_SELL_PRICE);
// 		AddColumn(COLUMN_HQ_BASE_RISE_VALUE);	
// 		AddColumn(COLUMN_HQ_BASE_MONEY);
// 		AddColumn(COLUMN_HQ_BASE_RANGE);
// 		AddColumn(COLUMN_HQ_EX_EXHAND_RATIO);
// 		AddColumn(COLUMN_HQ_BASE_ORDER_RATIO);
// 
// 	}
// 	else
// 	{
// 		WORD wKey = 0;
// 		int nOldPos = 0;
// 		int nWidth = 0;
// 		for(int i = 0; i < ayColTemplList->GetCount(); i++)
// 		{
// 			pArray = ayColTemplList->GetAt(i);
// 			if(pArray->GetSize() > 0)
// 			{
// 				wKey = atoi(pArray->GetAt(0));
// 				AddColumn( wKey );
// 				if(pArray->GetSize() > 1)
// 				{
// 					nWidth = atoi(pArray->GetAt(1));
// 					SetColumnWidth(wKey,nWidth);
// 				}
// 			}
// 		}
// 	}
	m_strPreColGroup = m_strCurColGroup;
	m_strCurColGroup = strGroupName;

	// 重新请求当前页数据
	if (bRefreshData)
	{
		RequestStockUserInfo();
	}
	CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-完成初始化列");
	return TRUE;

}

void CQuoteReportTable::AddColumn(UINT nID)
{
	const TableColProperty* pCol = m_iColProp->GetColumn(nID);
	if( pCol )
	{
		int bFixedCol = IsFixedColumn(nID);

		int nOldPos = 0;
		nOldPos = GetColIndexByID(nID);
		if( nOldPos >= 0 )
		{
			return;
		}

		if( bFixedCol )
		{		
			switch(nID)
			{
			case COLUMN_HQ_BASE_NAME:
				{
					if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE)) >= 0 )
					{
						nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK)) >= 0 )
					{
						//				nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) >= 0 )
					{
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER)) >= 0 )
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
					if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME)) >= 0 )
					{
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK)) >= 0 )
					{
						break;
					}				
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) >= 0 )
					{
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER)) >= 0 )
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
					if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME)) >= 0 )
					{
						nOldPos++;
						break;
					}				
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) >= 0 )
					{
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE)) >= 0 )
					{
						nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER)) >= 0 )
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
					if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_INFO_MARK)) >= 0 ) 
					{
						nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NAME)) >= 0 )
					{
						nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_CODE)) >= 0 )
					{
						nOldPos++;
						break;
					}
					else if( (nOldPos=GetColIndexByID(COLUMN_HQ_BASE_NUMBER)) >= 0 )
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
			default: nOldPos = 0; break;
			}

			m_ayColInfo.InsertAt(nOldPos,nID);			
			ResetCorrespondMap();
		}
		else
		{
			int nIndex = m_ayColInfo.Add(nID);
			AddColToMap(nID,nIndex);
		}

		CString strHead;
		int nWidth = pCol->m_nWidth;
		for (int i = 0; i < nWidth; i++)
		{
			strHead.AppendChar('A');
		}

		int nCol = InsertColumn(strHead,/*m_iColProp->GetColumnFormat(nID)|*/DT_SINGLELINE,nOldPos);
		SetItemText(0,nCol,pCol->strName);
		AddColumnState(nCol,pCol->m_nCellMask);
		SetColumnFgColor(nCol,RGB(0,0,0)/*m_iColProp->GetColumnColor(nID)*/);

		if (bFixedCol)
		{
			SetFixedColumnCount(GetFixedColumnCount() + 1);
		}
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"行情报价-添加列，列ID：%d，列名：%s，列宽（字符数）：%d，是否固定列：%d",
			nID,pCol->strName,nWidth,bFixedCol);
	}
}

void CQuoteReportTable::AddColToMap(int nID,int nColPos)
{
	int nPos;
	if( m_mapColumn.Lookup(nID,nPos) )
	{
		return;
	}
	m_mapColumn.SetAt(nID,nColPos);
}

void CQuoteReportTable::SaveCurDispColHead()
{
	if (!m_bResizeColWidth)
	{
		return;
	}
	CDC* pDC = GetDC();
	CSize size = pDC->GetTextExtent("A");
	// 同步更改到配置模块
	//CArray<CStringArray*,CStringArray*> ayGroup;
	CUIntArray ayGroup;
	for (int i=0; i<m_ayColInfo.GetCount(); i++)
	{
		ayGroup.Add(m_ayColInfo.GetAt(i));
		ayGroup.Add(GetColumnWidth(i)/size.cx);
		
// 		CString strTemp;
// 		CStringArray* payCol = new CStringArray;
// 		strTemp.Format("%d",m_ayColInfo.GetAt(i));
// 		payCol->Add(strTemp);
// 
// 		strTemp.Format("%d",GetColumnWidth(i)/size.cx);
// 		payCol->Add(strTemp);
// 		ayGroup.Add(payCol);
	}
	m_iColProp->SetDispColGroup(m_strCurColGroup, &ayGroup);
	pDC->DeleteDC();

// 	for (int i=0; i<ayGroup.GetCount(); i++)
// 	{
// 		delete ayGroup.GetAt(i);
// 	}
	ayGroup.RemoveAll();
	m_bResizeColWidth = FALSE;
	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价-保存当前列组信息，列组名称：%s",m_strCurColGroup);
}

void CQuoteReportTable::GetClientRect(LPRECT lpRect)
{
	CHSGridTable::GetClientRect(lpRect);
}

void CQuoteReportTable::SetPageViewInfo(PageViewInfo* pvInfo, CString strColGroupName /* = _T */, BOOL bRequest /*= FALSE*/)
{
	BOOL bRes = FALSE;
	if (memcmp(&m_pviInfo,pvInfo,sizeof(PageViewInfo)))
	{
		bRes = TRUE;
		memcpy(&m_pviInfo, pvInfo, sizeof(PageViewInfo));
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价列表-设置页面信息，PageType=%x，PageTag=%x，BlockName=%s，Market[0]=%x",
			pvInfo->m_lPageType,pvInfo->m_nInPageTag,pvInfo->m_szBlock,pvInfo->GetFirstMarketType());
	}
	bRes |= InitColGroup(strColGroupName,bRequest);
	if (bRes)				// 有变动时才重设当前排序状态和滚动条状态
	{
		SetSortColumn(-1);
		SetScrollPos32(SB_HORZ,0);
		SetScrollPos32(SB_VERT,0);
	}
}

StockUserInfo* CQuoteReportTable::GetStock( int nPos )
{
	StockUserInfo* pStock;
	if( nPos >= 0 && nPos < m_pStock.GetSize() )
	{
		pStock = m_pStock.GetAt(nPos);
		//		if( !AfxIsValidAddress(pStock,sizeof(*pStock)) )
		//			return NULL;

		return pStock;
	}
	return NULL;
}

void CQuoteReportTable::SetStock(StockUserInfo* pStock,int nTotal)
{
	if( !pStock || IsBadReadPtr(pStock,1) || nTotal <= 0 )
		return;

	EmptyStock();
	StockUserInfo* pTemp = NULL;

	for(int i = 0; i < nTotal; i++)
	{
		if( m_nCurActiveRow == -1 &&
			!memcmp(&pStock[i].m_ciStockCode,&m_pviInfo.m_ciCodeInfo,sizeof(m_pviInfo.m_ciCodeInfo)) )
			m_nCurActiveRow = i + 1;

		pTemp = new StockUserInfo;
		pTemp->Copy(&pStock[i]);
		m_pStock.Add(pTemp);
	}
}

void CQuoteReportTable::SetStock(CArray<StockUserInfo*,StockUserInfo*>& payStock)
{
	if( payStock.GetCount() <= 0 )
		return;

	EmptyStock();
	StockUserInfo* pTemp = NULL;

	for(int i = 0; i < payStock.GetCount(); i++)
	{
		if( m_nCurActiveRow == -1 &&
			!memcmp(&(payStock.GetAt(i)->m_ciStockCode),&m_pviInfo.m_ciCodeInfo,sizeof(CodeInfo)) )
			m_nCurActiveRow = i + 1;

		pTemp = new StockUserInfo;
		pTemp->Copy(payStock.GetAt(i));
		m_pStock.Add(pTemp);
	}
}

void CQuoteReportTable::EmptyStock(int nType /* = 0 */)
{
	for (int i = 0; i < m_pStock.GetCount(); i++)
	{
		delete m_pStock.GetAt(i);
	}
	m_pStock.RemoveAll();
}

CFont* CQuoteReportTable::GetColHeadFontPtr(int nType)
{
	return NULL/*pFont*/;

}

int CQuoteReportTable::ChangeContent(PageViewInfo* pvInfo, StockUserInfo* pStock /*= NULL*/, int nTotal /*= 0*/,CString strColGroup /*= _T("")*/, BOOL bForceRefresh /*= FALSE*/, BOOL bRequestData /*= TRUE*/)
{
	if (pvInfo == NULL || IsBadReadPtr(pvInfo,1))
	{
		CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL,QuoteReportLoggerName,"行情报价-改变当前显示内容错误，页面信息为空");
		return -1;
	}

	// 是否强制刷新内容
	if(!bForceRefresh)
	{
		BOOL bComp = 1;
		switch(pvInfo->m_nInPageTag & 0x0F00)
		{
		case PT_INDEX:
		case PT_TYPE:
			break;
		case PT_BLOCK:                                                     
		case PT_USERDEFINE:
			bComp = 0;
			break;
		}

		// 判断是否跟当前页面信息相同，若为显示板块则不判断（板块的成员股票可能变更）
		if ( bComp )
		{
			if( !memcmp(pvInfo, &m_pviInfo,sizeof(PageViewInfo)) &&
				(GetRowCount() - GetFixedRowCount()) != 0 )
			{
				CWinnerApplication::SimpleLog(WARN_LOG_LEVEL,QuoteReportLoggerName,"行情报价-改变当前显示内容失败，页面信息相同");
				return 1;
			}
		}
	}

	SetPageViewInfo(pvInfo,strColGroup);

	 
	if( (pStock != NULL) && (nTotal > 0) )
	{
		SetStock((StockUserInfo*)pStock,nTotal);
	}
	else if( nTotal > 0 )
	{
		// 去除错误代码
		for( int i = 0; i < m_pStock.GetSize(); i++ )
		{
			pStock = GetStock(i);
			if( pStock && 
				(pStock->m_ciStockCode.m_cCodeType == 0 || pStock->m_ciStockCode.m_cCode[0] == '\0'))
			{
				delete pStock;
				m_pStock.RemoveAt(i);
				i--;
			}
		}
	}
	else
	{
		EmptyStock();
	}

	InitColGroup(strColGroup);

	InitRowData();
	
	// 默认按照代码排序
	if(pvInfo != NULL && pvInfo->m_lPageType == PriceAnalisys1)//SpecialReport)
	{
		if (GetSortColumn() >= 0)
		{
			int nSortID = GetColHeadID(GetSortColumn());
			Sort(nSortID,GetSortAscending());
		} 
		else
		{
			if( ((pvInfo->m_nInPageTag & 0x0F00) == PT_BLOCK) ||   // 板块
				((pvInfo->m_nInPageTag & 0x0F00) == PT_USERDEFINE) // 自选
				)
			{
			}
			else
			{
				Sort(COLUMN_HQ_BASE_CODE,TRUE,TRUE);
				SetSortColumn(-1);
			}
		}
		FindStockPos(m_pStock,pvInfo->m_ciCodeInfo,m_nCurActiveRow);

	}

	KillAllTimer();
	if (bRequestData)
	{
		if (m_bNeedRequestSort)
		{
			m_nDelaySortRequestTimerID = SetTimer((long)this+2,DELAY_REQUEST_TIMESPAN,DelaySortRequestTimerProc);
		} 
		else
		{
			m_nDelayRealTimeRequestTimerID = SetTimer((long)this+1,DELAY_REQUEST_TIMESPAN,DelayRealTimeRequestTimerProc);
		}
	}

	return 0;
}

void CQuoteReportTable::InitRowData()
{
	CArray<StockUserInfo*,StockUserInfo*>* pArray = &m_pStock;
	if(pArray == NULL)
		return;

	InitFixedDataItem(0,m_pStock.GetCount());
}

void CQuoteReportTable::InitFixedDataItem(int nFirst,int nCount)
{
	try
	{
		SetRowCount(nCount  + GetFixedRowCount());
	}
	catch (CMemoryException* e)
	{
		CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL,QuoteReportLoggerName,
			"行情报价-设置表格行数异常");
		e->ReportError();
		e->Delete();
		return;
	}

	// 行
	StockUserInfo* pStock = NULL;
	CGridCellBase* pCell;
	CCellRange cellRange = GetVisibleNonFixedCellRange();

	int row,col,nDataStart;

	if( m_idCurrentCell.row == -1 &&
		!(m_idCurrentCell.row >= nFirst && m_idCurrentCell.row <= nFirst + nCount) )
	{
		m_idCurrentCell.row = m_nFixedRows;
		m_idCurrentCell.col = m_nFixedCols;
	}

	for ( row = GetFixedRowCount(),nDataStart = nFirst + 1; 
		row <= GetRowCount()/*cellRange.GetMaxRow()*/; row++,nDataStart++)
	{
		pStock = GetStock( nDataStart - 1 );
		if( pStock )
		{
			for ( col = 0; col < GetColumnCount()/* && col < maxVisibleCol*/; col++)
			{
				pCell = GetCell(row, col);
				if( pCell == NULL )
					continue;

				SetFixedData( row,col,pCell,pStock, nDataStart,FALSE,NULL );

			}
		}
	}

	this->Invalidate(FALSE);
}

BOOL CQuoteReportTable::SetHorScrollBarRef(CSkinScrollBar* pNewHorScrollBar /* = NULL */)
{
	if (pNewHorScrollBar && !IsBadReadPtr(pNewHorScrollBar,1))
	{
		EnableScrollBars(SB_HORZ,FALSE);
		m_pHorScrollBar = pNewHorScrollBar;
		ResetScrollBars();
	} 
	else
	{
		EnableScrollBars(SB_HORZ);
		m_pHorScrollBar = NULL;
	}
	return TRUE;
}

BOOL CQuoteReportTable::SetVerScrollBarRef(CSkinScrollBar* pNewVerScrollBar /* = NULL */)
{
	if (pNewVerScrollBar && !IsBadReadPtr(pNewVerScrollBar,1))
	{
		EnableScrollBars(SB_VERT,FALSE);
		m_pVerScrollBar = pNewVerScrollBar;
		ResetScrollBars();
	} 
	else
	{
		EnableScrollBars(SB_VERT);
		m_pVerScrollBar = NULL;
	}
	return TRUE;
}

void CQuoteReportTable::ResetScrollBars()
{
	// Force a refresh. 
	m_idTopLeftCell.row = -1;

	if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect;

	// This would have caused OnSize event - Brian 
	//EnableScrollBars(SB_BOTH, FALSE); 

	GetClientRect(rect);

	if (rect.left == rect.right || rect.top == rect.bottom)
		return;

	if (IsVisibleVScroll())
		rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);

	if (IsVisibleHScroll())
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);

	rect.left += GetFixedColumnWidth();
	rect.top += GetFixedRowHeight();


	if (rect.left >= rect.right || rect.top >= rect.bottom)
	{
		if (m_pVerScrollBar && IsBadReadPtr(m_pVerScrollBar,1))
		{
 			//m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, FALSE);
		} 
		else if (m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, FALSE);
		}
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		return;
	}

	CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), 
		rect.right, rect.bottom);
	CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(),
		GetVirtualWidth(), GetVirtualHeight());

	// Removed to fix single row scrollbar problem (Pontus Goffe)
	// CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
	// if (!IsValid(visibleCells)) return;

	//TRACE(_T("Visible: %d x %d, Virtual %d x %d.  H %d, V %d\n"), 
	//      VisibleRect.Width(), VisibleRect.Height(),
	//      VirtualRect.Width(), VirtualRect.Height(),
	//      IsVisibleHScroll(), IsVisibleVScroll());

	// If vertical scroll bar, horizontal space is reduced
	if (VisibleRect.Height() < VirtualRect.Height())
		VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
	// If horz scroll bar, vert space is reduced
	if (VisibleRect.Width() < VirtualRect.Width())
		VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);

	// Recheck vertical scroll bar
	//if (VisibleRect.Height() < VirtualRect.Height())
	// VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);

	if (VisibleRect.Height() < VirtualRect.Height())
	{
		if (m_pVerScrollBar)
		{
// 			m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, TRUE);
		} 
		else
		{
			EnableScrollBars(SB_VERT, TRUE); 
		}
		m_nVScrollMax = VirtualRect.Height() - 1;
	}
	else
	{
		if (m_pVerScrollBar)
		{
// 			m_pVerScrollBar->EnableScrollBarCtrl(SB_VERT, FALSE);
		} 
		else
		{
			EnableScrollBars(SB_VERT, FALSE); 
		}
		m_nVScrollMax = 0;
	}

	if (VisibleRect.Width() < VirtualRect.Width())
	{
		if (m_pHorScrollBar)
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, TRUE);
		} 
		else
		{
			EnableScrollBars(SB_HORZ, TRUE); 
		}
		m_nHScrollMax = VirtualRect.Width() - 1;
	}
	else
	{
		if (m_pHorScrollBar)
		{
// 			m_pHorScrollBar->EnableScrollBarCtrl(SB_HORZ, FALSE);
		} 
		else
		{
			EnableScrollBars(SB_HORZ, FALSE); 
		}
		m_nHScrollMax = 0;
	}

	ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine

	// New code - Paul Runstedler 
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage = (m_nHScrollMax>0)? VisibleRect.Width() : 0;
	si.nMin = 0;
	si.nMax = m_nHScrollMax;
	if (m_pHorScrollBar)
	{
		m_pHorScrollBar->SetScrollInfo(&si, TRUE);
	} 
	else
	{
		SetScrollInfo(SB_HORZ, &si, TRUE);
	}

	si.fMask |= SIF_DISABLENOSCROLL;
	si.nPage = (m_nVScrollMax>0)? VisibleRect.Height() : 0;
	si.nMin = 0;
	si.nMax = m_nVScrollMax;
	if (m_pVerScrollBar)
	{
		m_pVerScrollBar->SetScrollInfo(&si, TRUE);
	} 
	else
	{
		SetScrollInfo(SB_VERT, &si, TRUE);
	}

}

int CQuoteReportTable::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	if (nBar == SB_HORZ)
	{
		if (m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);

			if (bGetTrackPos)
			{
				if (m_pHorScrollBar->GetScrollInfo(&si, SIF_TRACKPOS))
					return si.nTrackPos;
			}
			else
			{
				if (m_pHorScrollBar->GetScrollInfo(&si, SIF_POS))
					return si.nPos;
			}

			return 0;
		} 
		else
		{
			return CHSGridTable::GetScrollPos32(nBar,bGetTrackPos);
		}
	} 
	else
	{
		if (m_pVerScrollBar && !IsBadReadPtr(m_pVerScrollBar,1))
		{
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);

			if (bGetTrackPos)
			{
				if (m_pVerScrollBar->GetScrollInfo(&si, SIF_TRACKPOS))
					return si.nTrackPos;
			}
			else
			{
				if (m_pVerScrollBar->GetScrollInfo(&si, SIF_POS))
					return si.nPos;
			}

			return 0;
		} 
		else
		{
			return CHSGridTable::GetScrollPos32(nBar,bGetTrackPos);
		}
	}
}

BOOL CQuoteReportTable::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	m_idTopLeftCell.row = -1;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = nPos;

	if (nBar == SB_HORZ
		&& m_pHorScrollBar && !IsBadReadPtr(m_pHorScrollBar,1))
	{
		return m_pHorScrollBar->SetScrollInfo(&si, bRedraw);
	} 
	else if (nBar == SB_VERT
		&& m_pVerScrollBar && !IsBadReadPtr(m_pVerScrollBar,1))
	{
		return m_pVerScrollBar->SetScrollInfo(&si, bRedraw);
	}
	return SetScrollInfo(nBar, &si, bRedraw);
}

CString CQuoteReportTable::GetMenuName(int nType /* = 0 */, int nSubType /* = 0 */)
{
	return CHSGridTable::GetMenuName(nType,nSubType);
}

CString CQuoteReportTable::GetMenuName(UINT nMenuID)
{
	return m_iQuoteLang->LoadStr(nMenuID);
}

BOOL CQuoteReportTable::ShowRow(int row, BOOL bRefresh /* = FALSE */)
{
	BOOL bRes = SetRowHeight(row,m_cellFixedRowDef.GetHeight());
	if (bRefresh)
	{
		Refresh();
	}
	return bRes;
}
BOOL CQuoteReportTable::ShowCol(int col, BOOL bRefresh /* = FALSE */)
{
	int nWidth = 1;
	if (col >= 0)
	{
		nWidth = m_iColProp->GetColumnWidth((int)GetColHeadID(col));
	}
	else
		return FALSE;
	BOOL bRes = SetColumnWidth(col,nWidth);
	if (nWidth == 1)
	{
		AutoSizeColumn(col);
	}
	if (bRefresh)
	{
		Refresh();
	}
	return bRes;
}

BOOL CQuoteReportTable::MoveRowTo(int nSrc,int nDes)
{
	if (!CHSGridTable::MoveRowTo(nSrc, nDes))
	{
		return FALSE;
	}

	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}

	// 序号不变
	InitNumber(min(nSrc,nDes),max(nSrc,nDes));

	nSrc--;
	StockUserInfo* pStock = m_pStock.GetAt(nSrc);
	if (pStock)
	{
		if (nDes == -1)
		{
			m_pStock.Add(pStock);
		} 
		else
		{
			nDes--;
			m_pStock.InsertAt(nDes,pStock);
			if (nDes <= nSrc)
			{
				m_nCurActiveRow++;
				nSrc++;
			}
		}
		m_pStock.RemoveAt(nSrc);
		return TRUE;
	}
	return FALSE;

}

BOOL CQuoteReportTable::MoveColTo(int nSrc,int nDes)
{
	UINT nID = m_ayColInfo.GetAt(nSrc);
	if (nID > 0)
	{
		if (CHSGridTable::MoveColTo(nSrc,nDes))
		{
			if (m_nRefreshTimerID != 0)
			{
				KillTimer(m_nRefreshTimerID);
				m_nRefreshTimerID = 0;
				m_mapRefreshCells.RemoveAll();
			}

			int nPos = 0;
			if (nDes < 0)
			{
				nPos = m_ayColInfo.Add(nID);
			}
			else
			{
				m_ayColInfo.InsertAt(nDes,nID);
				if (nDes <= nSrc)
				{
					nSrc++;
				}
			}
			m_ayColInfo.RemoveAt(nSrc);
			ResetCorrespondMap();

			m_bResizeColWidth = TRUE;
			SaveCurDispColHead();
			return TRUE;
		}
	}
	return FALSE;

}

RefreshDispTextProp* CQuoteReportTable::GetRefreshDispTextProp()
{
	return m_iColProp->GetQuoteRefreshProp();
}

int CQuoteReportTable::DelutionHilightCells()
{
	if (m_mapRefreshCells.GetCount() <= 0)
	{
		return 0;
	}

	POSITION pos = m_mapRefreshCells.GetStartPosition();
	int nDelCount = 0;
	int nKey = 0;
	int nValue = 0;
	int nRow = 0;
	int nCol = 0;

	COLORREF colBK = 0;
	COLORREF colFG = 0;
	CGridCellBase* pCell = NULL;
	CGridDefaultCell *pDefaultCell = NULL;
	while(pos)
	{
		m_mapRefreshCells.GetNextAssoc(pos,nKey,nValue);
		nRow = LOWORD(nKey);
		nCol = HIWORD(nKey);
		if (nValue <= 0)
		{
			RedrawCell(nRow,nCol);
			m_mapRefreshCells.RemoveKey(nKey);
			nDelCount++;
			continue;
		}
		nValue--;

		pCell = GetCell(nRow,nCol);
		pDefaultCell = (CGridDefaultCell*)pCell->GetDefaultCell();

		if (pCell->GetBackClr() == CLR_DEFAULT)
			colBK = pDefaultCell->GetBackClr();
		else
			colBK = pCell->GetBackClr();

		if (pCell->GetTextClr() == CLR_DEFAULT)
			colFG = pDefaultCell->GetTextClr();
		else
			colFG = pCell->GetTextClr();

		// 调整背景颜色
		if (GetRefreshDispTextProp()->m_dwBkColor != CLR_DEFAULT)
		{
			colBK =CGeneralHelper::DelutionColor(GetRefreshDispTextProp()->m_dwBkColor,colBK,(REFRESH_DISAPPEAR_TIMES-nValue)*100/REFRESH_DISAPPEAR_TIMES);
		}

		// 调整字体颜色
		if (GetRefreshDispTextProp()->m_dwFgColor != CLR_DEFAULT)
		{
			colFG =CGeneralHelper::DelutionColor(GetRefreshDispTextProp()->m_dwFgColor,colFG,(REFRESH_DISAPPEAR_TIMES-nValue)*100/REFRESH_DISAPPEAR_TIMES);
		}

		RedrawUpdateCell(nRow,nCol,colBK,colFG);

		m_mapRefreshCells.SetAt(nKey,nValue);
	}
	return nDelCount;
}

//////////////////////////////////////////////////////////////////////////数据请求操作
void CQuoteReportTable::RequestData( PageViewInfo* pPageInfo /* = NULL */ )
{	
	CCellRange cellRange = GetVisibleNonFixedCellRange();
	// 请求即时数据（同时订阅主推）
	RequestRealTimeData(cellRange.GetMinRow() - GetFixedRowCount(),cellRange.GetRowSpan());

}

void CQuoteReportTable::RequestStockUserInfo(PageViewInfo* pPageInfo /* = NULL */)
{
	if (pPageInfo == NULL)			// 无指定市场或页面类型则重新请求当前页
	{
		pPageInfo = &m_pviInfo;
	}
	if (pPageInfo->m_nInPageTag & PT_TYPE)		// 从数据引擎请求指定 市场 的股票代码列表
	{
		AskData*  paskData;
		int nMarketCount = pPageInfo->GetMarketTypeCount();		// 市场个数 多市场暂不支持
		int nRetLen = sizeof(AskData) - sizeof(CodeInfo) 
			+ sizeof(ReqStockInfoList) - sizeof(HSMarketDataType) 
			+ nMarketCount*sizeof(HSMarketDataType);
		paskData = (AskData*)new char[nRetLen];
		if( paskData == NULL )
		{
			return;
		}
		memset(paskData,0,nRetLen);

		paskData->m_nType  = RT_STOCKINFOLIST;
		paskData->m_nSize  = 1;

		ReqStockInfoList* pMarketList = (ReqStockInfoList*)paskData->m_pCode;
		pMarketList->m_nSize = nMarketCount;
		CString strMarket;
		for (int i=0,j=0; i<MARKETTYPE_COUNT; i++)
		{
			if (pPageInfo->m_sMarket[i] != 0)
			{
				pMarketList->m_nMarketType[j] = pPageInfo->m_sMarket[i];
				strMarket.AppendFormat("%X,",pPageInfo->m_sMarket[i]);
				j++;
			}
		}

		m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, nRetLen, 0/*, e_DataSouceSend_Clear*/);

		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"行情报价列表-请求股票代码，请求功能号：%X，市场个数：%d，市场代码：%s",
			paskData->m_nType,paskData->m_nSize,strMarket);

		delete paskData;
	}
	else if (pPageInfo->m_nInPageTag & PT_BLOCK)
	{
		CArray<CodeInfo> ayCodeList;
		CArray<StockUserInfo> ayStockUserInfo;
		m_iBlockManager->GetStockList(pPageInfo->GetBlockName(),ayCodeList);
		if (ayCodeList.GetCount() > 0)
		{
			m_pDataSource->HSDataSourceEx_GetStocksUserInfo(ayCodeList,ayStockUserInfo,TRUE);
		}
		ChangeContent(pPageInfo,ayStockUserInfo.GetData(),ayStockUserInfo.GetCount(),_T(""),TRUE);
		
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"行情报价列表-切换板块股票代码，板块名称：%s，股票个数：%d",pPageInfo->GetBlockName(),ayCodeList.GetCount());
	}
}

void CQuoteReportTable::RequestRealTimeData(int nFirst /* = -1 */, int nCount /* = -1 */, BOOL bNeedAutoPush /* = TRUE */)
{
	CCellRange cellRange = GetVisibleNonFixedCellRange();
	if (nFirst < 0)
	{
		nFirst = cellRange.GetMinRow() - GetFixedRowCount();
	}
	if (nCount <= 0)
	{
		nCount = cellRange.GetRowSpan();
		if (GetRowCount()-GetFixedRowCount()-nFirst < nCount)
		{
			nCount = GetRowCount()-GetFixedRowCount()-nFirst;
		}
	}

	AskData*  paskData;
	int lRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCount-1);
	paskData = (AskData*) new char[lRetLen];
	memset(paskData, 0, lRetLen);

	paskData->m_nType  = RT_REALTIME_EXT;
	paskData->m_nSize  = nCount;
	CodeInfo* pCodeInfo = paskData->m_pCode;
	StockUserInfo* pStock = NULL;
	for(int i = nFirst,j = 0; i < nFirst + nCount; i++,j++)
	{
		pStock = GetStock(i);
		if( pStock != NULL && !IsBadReadPtr(pStock,1))
		{
			memcpy(&pCodeInfo[j], &pStock->m_ciStockCode,sizeof(CodeInfo));
		}
	}

	m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, lRetLen, 0);

	if (bNeedAutoPush)
	{
		paskData->m_nType = RT_AUTOPUSH_EXT;
		m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, lRetLen, 0, e_DataSouceSend_Clear);
	}

	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-请求实时数据，请求功能号：%X，代码个数：%d，起始代码：%s，是否同时订阅主推：%d",
		paskData->m_nType,paskData->m_nSize,paskData->m_pCode->GetCode(),bNeedAutoPush);
	
	delete paskData;
}

void CQuoteReportTable::RequestSortData(PageViewInfo* pPageInfo /* = NULL */, BOOL bNeedAutoPush /* = TRUE */)
{
	if (!pPageInfo)
	{
		pPageInfo = &m_pviInfo;
	}
	CCellRange cellRange = GetVisibleNonFixedCellRange();
	int nColID = GetColHeadID(GetSortColumn());
	if (nColID < 0)
	{
		return;
	}
	int nStockSize = 0;			//   =请求结构长度/sizeof(CodeInfo) 
	int nCount;
	HSMarketDataType cCodeType;
	if( !GetSortInfoByPageInfo(pPageInfo,nColID,nCount,cCodeType) )
	{
		SetSortColumn(-1);
		return;
	}


	// 计算请求结构长度相对于CodeInfo长度的个数（请求结构长度/sizeof(CodeInfo) ）
	if (cCodeType == USERDEF_BOURSE) // 自定义板块（报价上的所有股票）
	{
		nStockSize = (sizeof(ReqAnyReport) + sizeof(AnyReportData) * nCount) / sizeof(CodeInfo) +
			(((sizeof(ReqAnyReport) + sizeof(AnyReportData) * nCount) % sizeof(CodeInfo))?1:0);
	}
	else if( cCodeType == SYSBK_BOURSE ) // 系统板块
	{
		nStockSize = (sizeof(ReqAnyReport) + sizeof(ReqOneBlock) * nCount) / sizeof(CodeInfo) +
			(((sizeof(ReqAnyReport) + sizeof(ReqOneBlock) * nCount) % sizeof(CodeInfo))?1:0);
	}
	else // 市场类型（系统分类）
	{
		nStockSize = sizeof(ReqAnyReport) / sizeof(CodeInfo) +
			((sizeof(ReqAnyReport) % sizeof(CodeInfo))?1:0);
	}

	AskData* paskData = NULL;
	int lRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nStockSize - 1);
	paskData = (AskData*)new char[lRetLen];
	if( paskData == NULL )
	{
		return;
	}
	memset(paskData,0,lRetLen);

	paskData->m_nType  = RT_REPORTSORT_EXT;
	paskData->m_nSize  = nStockSize;

	ReqAnyReport* pAnyReportDataReq = (ReqAnyReport*)paskData->m_pCode;
	pAnyReportDataReq->m_cCodeType  = cCodeType;
	pAnyReportDataReq->m_nBegin		= cellRange.GetMinRow() - GetFixedRowCount();
	pAnyReportDataReq->m_nCount		= cellRange.GetRowSpan() + 1;		//+1防止显示半行的行没有排序数据
	pAnyReportDataReq->m_cAlignment = 1;

	pAnyReportDataReq->m_nColID     = nColID;
	pAnyReportDataReq->m_bAscending = !GetSortAscending();
	pAnyReportDataReq->m_nSize		= nCount;

	if( cCodeType == USERDEF_BOURSE )
	{
		CArray<StockUserInfo> ayStock;
		CArray<CodeInfo> ayCodeList;
		m_iBlockManager->GetStockList(pPageInfo->GetBlockName(),ayCodeList);
		m_pDataSource->HSDataSourceEx_GetStocksUserInfo(ayCodeList,ayStock,TRUE);

		AnyReportData* pAnyReportData;
		pAnyReportData = pAnyReportDataReq->m_sAnyReportData;

		for(int i=0; i<nCount && i<ayStock.GetCount(); i++,pAnyReportData++)
		{
			//pStock = m_pStock.GetAt(i);
#ifdef _DEBUG
			CString strTest;
			strTest.Format("行情报价-请求排序数据：Market=%x,Code=%s,Name=%s\n",
				ayStock.GetAt(i).m_ciStockCode.m_cCodeType,ayStock.GetAt(i).m_ciStockCode.m_cCode,ayStock.GetAt(i).GetName());
			OutputDebugString(strTest);
#endif
			memcpy(&pAnyReportData->m_ciStockCode, &ayStock.GetAt(i).m_ciStockCode, sizeof(CodeInfo));
		}
	}
	else if( cCodeType == SYSBK_BOURSE ) // 系统板块
	{
		ReqOneBlock* pBlockData = (ReqOneBlock*)pAnyReportDataReq->m_sAnyReportData;
		pBlockData->m_lCRC = 0;
		strncpy(pBlockData->m_szBlockName,(*pPageInfo).m_szBlock,sizeof(pBlockData->m_szBlockName));
	}

	m_pDataSource->HSDataSource_RequestAsyncData(m_nDataSourceID, (char*)paskData, lRetLen, 0);

	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-请求排序数据，请求功能号：%X，代码个数：%d，是否同时订阅主推：%d",
		paskData->m_nType,paskData->m_nSize,bNeedAutoPush);

	delete paskData;

}

void CQuoteReportTable::KillAllTimer()
{
	if (m_nDelayRealTimeRequestTimerID != 0)
	{
		KillTimer(m_nDelayRealTimeRequestTimerID);
		m_nDelayRealTimeRequestTimerID = 0;
	}
	if (m_nDelaySortRequestTimerID != 0)
	{
		KillTimer(m_nDelaySortRequestTimerID);
		m_nDelaySortRequestTimerID = 0;
	}
	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////消息处理
void CQuoteReportTable::OnDraw(CDC* pDC)
{
	CHSGridTable::OnDraw(pDC);
}
void CQuoteReportTable::OnLButtonDown(UINT nFlags, CPoint point)
{
	CHSGridTable::OnLButtonDown(nFlags, point);
	if (GetMouseMode() == MOUSE_SIZING_COL)
	{
		m_bResizeColWidth = TRUE;
	}
}

void CQuoteReportTable::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CHSBizDialog *pBizDlg = (CHSBizDialog*)GetParent();
	IMainWnd * pMain = pBizDlg->GetMain();
	pMain->ShowPanel(PN_QUOTEDIAGRAM,SP_SHOW);

	StockUserInfo* pStkInfo = GetStock(m_CurRow-1);
	if(pStkInfo)
	{
		CLinkageParam param;
		CString strMarkeType;
		strMarkeType.Format("%d",pStkInfo->m_ciStockCode.m_cCodeType);
		param.AddParam(PARAM_CODE_CHANGE_MARK_TYPE,strMarkeType);
		param.AddParam(PARAM_CODE_CHANGE_STOCK_CODE,pStkInfo->m_ciStockCode.GetCode());
		//SendLinkageMsg(QUOTE_MESSAGE_CODE_CHANGE,&param);
	}

	CHSGridTable::OnLButtonDblClk(nFlags,point);
}

void CQuoteReportTable::OnRButtonDown(UINT nFlags, CPoint point)
{
	CCellID cell = GetCellFromPt(point);
	if (cell.row >= 0 && cell.row < GetFixedRowCount())
	{
		CMenu popMenu;
		popMenu.CreatePopupMenu();
		if (GetFilterMode())
		{
			popMenu.AppendMenu(0,ID_FILTER_CONDITION,m_iQuoteLang->LoadStr(ID_FILTER_CONDITION));
			popMenu.AppendMenu(0,ID_CANCEL_FILTER,m_iQuoteLang->LoadStr(ID_CANCEL_FILTER));
			popMenu.AppendMenu(MF_SEPARATOR);
		}
		// 动态显示所有可用列头组
		CStringArray ayDispColGroup;
		m_iColProp->GetAllDispColTemplName(ayDispColGroup);
		if (ayDispColGroup.GetCount() > 0)
		{
			m_mapTempColGroup.RemoveAll();
			for (int i=0; i<ayDispColGroup.GetCount(); i++)
			{
				CString strDispCol = ayDispColGroup.GetAt(i);
				if (!strDispCol.IsEmpty())
				{
					popMenu.AppendMenu(0,ID_QUOTE_DISP_COLGROUP_BEGIN+i,strDispCol);
					if (m_strCurColGroup == strDispCol)
					{
						popMenu.CheckMenuItem(ID_QUOTE_DISP_COLGROUP_BEGIN+i,MF_BYCOMMAND|MF_CHECKED);
					}
					m_mapTempColGroup.SetAt(ID_QUOTE_DISP_COLGROUP_BEGIN+i,strDispCol);
				}
			}
			popMenu.AppendMenu(MF_SEPARATOR);
		}


		popMenu.AppendMenu(0,ID_COLUMN_SETTING,m_iQuoteLang->LoadStr(ID_COLUMN_SETTING));
		if (popMenu.GetMenuItemCount() > 0)
		{
			ClientToScreen(&point);
			popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);
			m_nLastColHeadRDown = cell.col;
		}
		popMenu.DestroyMenu();
	} 
	else
	{
		CHSGridTable::OnRButtonDown(nFlags,point);

		CMenu popMenu;
		CMenu subMenu;
		subMenu.CreatePopupMenu();
		popMenu.CreatePopupMenu();

		popMenu.AppendMenu(MF_POPUP,(UINT_PTR)subMenu.m_hMenu,m_iQuoteLang->LoadStr(IDS_MARK_CURCODE));
		subMenu.AppendMenu(0,ID_MARK_STRING,m_iQuoteLang->LoadStr(ID_MARK_STRING)+_T("\tCTRL+`"));
		CBitmap bitmap[MARK_COUNT];	// for test
		for (int i=1; i<= MARK_COUNT; i++)
		{
			CString strText;
			strText.Format("%s-%d\tCTRL+%d",m_iQuoteLang->LoadStr(ID_MARK_TAG_BEGIN),i,i%10);
			subMenu.AppendMenu(0,ID_MARK_TAG_BEGIN+i-1,strText);
			CGeneralHelper::GetListImage(*GetImageList(), bitmap[i-1], i-1);
			BOOL bRes = subMenu.SetMenuItemBitmaps(i,MF_BYPOSITION,&bitmap[i-1],&bitmap[i-1]);
		}
		popMenu.AppendMenu(0,ID_CANCEL_MARK_CODE,m_iQuoteLang->LoadStr(ID_CANCEL_MARK_CODE)+_T("\tCTRL+DEL"));
		CString strIndex;
		StockUserInfo* pStock = GetStock(m_CurRow-1);
		if (pStock)
		{
			strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
			if (!m_iUserDefCfg->SearchMark(strIndex))
			{
				popMenu.EnableMenuItem(ID_CANCEL_MARK_CODE,MF_BYCOMMAND|MF_GRAYED);
			}
		}
		popMenu.AppendMenu(MF_SEPARATOR);
		popMenu.AppendMenu(0,ID_BLOCK_ADSCRIPTION,m_iQuoteLang->LoadStr(ID_BLOCK_ADSCRIPTION));
		popMenu.AppendMenu(0,ID_ADD_TO_BLOCK,m_iQuoteLang->LoadStr(ID_ADD_TO_BLOCK));
		popMenu.AppendMenu(0,ID_ADD_TO_SELFBLOCK, m_iQuoteLang->LoadStr(ID_ADD_TO_SELFBLOCK));
		ClientToScreen(&point);
		popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x,point.y,this);

		for (int i=0; i<10; i++)
		{
			bitmap[i].DeleteObject();
		}
		subMenu.DestroyMenu();
		popMenu.DestroyMenu();

	}

}

void CQuoteReportTable::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CHSGridTable::OnVScroll(nSBCode, nPos, pScrollBar);
	//CCellRange cellRange = GetVisibleNonFixedCellRange();
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		{

			break;
		}
	case SB_PAGEDOWN:
		{
			break;
		}
	case SB_LINEUP:
		{
			break;
		}
	case SB_PAGEUP:
		{
			break;
		}
	case SB_THUMBPOSITION:
		{
			break;
		}
	case SB_TOP:
		{
			break;
		}
	case SB_BOTTOM:
		{
			break;
		}
	default:
		return;
	}
	KillAllTimer();
	if (m_bNeedRequestSort)
	{
		m_nDelaySortRequestTimerID = SetTimer((long)this+2,DELAY_REQUEST_TIMESPAN,DelaySortRequestTimerProc);
	}
	else
	{
		m_nDelayRealTimeRequestTimerID = SetTimer((long)this+1,DELAY_REQUEST_TIMESPAN,DelayRealTimeRequestTimerProc);
	}

}

void CQuoteReportTable::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CHSGridTable::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CQuoteReportTable::OnSize(UINT nType, int cx, int cy)
{
	CHSGridTable::OnSize(nType,cx,cy);

	KillAllTimer();
	if (m_bNeedRequestSort)
	{
		m_nDelaySortRequestTimerID = SetTimer((long)this+2,DELAY_REQUEST_TIMESPAN,DelaySortRequestTimerProc);
	}
	else
	{
		m_nDelayRealTimeRequestTimerID = SetTimer((long)this+1,DELAY_REQUEST_TIMESPAN,DelayRealTimeRequestTimerProc);
	}
}

void CQuoteReportTable::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CHSGridTable::OnChar(nChar, nRepCnt, nFlags);
}

void CQuoteReportTable::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT)
	{
		SendMessage(WM_HSCROLL, SB_LINEUP, 0);
		return;
	}
	else if (nChar == VK_RIGHT)
	{
		SendMessage(WM_HSCROLL, SB_LINEDOWN, 0);
		return;
	}
	else if (nChar == VK_HOME)
	{
		BYTE bt[256];
		BOOL bRes = GetKeyboardState(bt);
		bt[17] = 129;
		bRes = SetKeyboardState(bt);
	}
	else if (nChar == VK_END)
	{
		BYTE bt[256];
		BOOL bRes = GetKeyboardState(bt);
		bt[17] = 129;
		bRes = SetKeyboardState(bt);
	}
	else if (nChar == VK_PRIOR)
	{
		CCellID idTopLeft = GetTopleftNonFixedCell();
		if (idTopLeft.row <= GetFixedRowCount())
		{
			nChar = VK_END;
			BYTE bt[256];
			BOOL bRes = GetKeyboardState(bt);
			bt[17] = 129;
			bRes = SetKeyboardState(bt);
			DWORD dw = GetLastError();
			bRes = IsCTRLpressed();
			OutputDebugString("");
		}
	} 
	else if (nChar == VK_NEXT)
	{
		CCellRange cellrange = GetVisibleNonFixedCellRange();
		if (cellrange.GetMaxRow() >= GetRowCount()-1)
		{
			nChar = VK_HOME;
			BYTE bt[256];
			BOOL bRes = GetKeyboardState(bt);
			bt[17] = 129;
			bRes = SetKeyboardState(bt);
			DWORD dw = GetLastError();
			bRes = IsCTRLpressed();
			OutputDebugString("");
		}
	}
	CHSGridTable::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CQuoteReportTable::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		CancelSort();
	}
	else if (nChar == VK_TAB)
	{
		CString strFinanceGroupName = m_iColProp->GetFinanceGroupName();
		if (!strFinanceGroupName.IsEmpty())
		{
			InitColGroup(strFinanceGroupName==m_strCurColGroup?m_strPreColGroup:strFinanceGroupName,TRUE);
		}
	}
	CHSGridTable::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CQuoteReportTable::OnColGroupChange(UINT nID)
{
	CString strColGroup;
	if (m_mapTempColGroup.Lookup(nID,strColGroup) && !strColGroup.IsEmpty())
	{
		SetPageViewInfo(&m_pviInfo,strColGroup,TRUE);
	}
}

void CQuoteReportTable::OnColumnSetting()
{
	CQuoteColumnSetting dlg(m_strCurColGroup,this);
	if (dlg.DoModal() == IDOK)
	{
	}
	DWORD dw = GetLastError();
}

void CQuoteReportTable::OnMarkCodeByString()
{
	CString strIndex;
	HsCodeMark mark;
	mark.m_nType = -1;
	StockUserInfo* pStock = GetStock(m_CurRow-1);
	if (pStock)
	{
		strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
		strncpy(mark.m_szIndex,strIndex,BLOCK_NAME_LENGTH);
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		mark.m_strContent = GetMarkContent(CPoint(point));

		if (!mark.m_strContent.IsEmpty() && m_iUserDefCfg->SetMark(mark))
		{
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			CGridCellBase* pCell = GetCell(m_CurRow,nCol);
			pCell->SetImageEx(20);
			RedrawCell(m_CurRow,nCol);
		}
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-为代码%s添加文字标记，标记内容：%s",pStock->GetCode(),mark.m_strContent);
	}
}

void CQuoteReportTable::OnMarkCodeByIcon(UINT nID)
{
	HsCodeMark mark;
	CString strIndex;
	mark.m_nType = nID-ID_MARK_TAG_BEGIN;
	StockUserInfo* pStock = GetStock(m_CurRow-1);
	if (pStock)
	{
		strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
		strncpy(mark.m_szIndex,strIndex,BLOCK_NAME_LENGTH);
		if (m_iUserDefCfg->SetMark(mark))
		{ 
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			CGridCellBase* pCell = GetCell(m_CurRow,nCol);
			pCell->SetImageEx(mark.m_nType);
			RedrawCell(m_CurRow,nCol);
		}
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-为代码%s添加图标标记，标记类型：第%d项",pStock->GetCode(),mark.m_nType);
	}
}

void CQuoteReportTable::OnCancelMarkCode()
{
	CString strIndex;
	StockUserInfo* pStock = GetStock(m_CurRow-1);
	if (pStock)
	{
		strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
		if (m_iUserDefCfg->DelMark(strIndex))
		{
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			CGridCellBase* pCell = GetCell(m_CurRow,nCol);
			pCell->SetImageEx(-1);
			RedrawCell(m_CurRow,nCol);
		}
		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-取消当前代码标记");
	}
}

void CQuoteReportTable::OnAddToBlock()
{
	StockUserInfo* pStock = GetStock(m_CurRow-1);
	if (pStock)
	{

	}

}

void CQuoteReportTable::OnAddToSelfBlock()
{
	StockUserInfo* pStock = GetStock(m_CurRow-1);
	if (pStock)
	{
		m_iBlockManager->AddStockToBlock(m_iQuoteLang->LoadStr(IDS_SELF_BLOCK),&pStock->m_ciStockCode);
	}

}

void CQuoteReportTable::OnBlockAdscription()
{
	// 所属板块
// 	StockUserInfo* pStock = GetStock(m_CurRow-1);
// 	CBlockAdscriptionDlg dlg(this);
// 	dlg.SetStockInfo(&pStock->m_ciStockCode);
// 	if (dlg.DoModal() == IDOK)
// 	{
// 	}
}
void CQuoteReportTable::OnResizeColWidth(int nCol)
{
	SaveCurDispColHead();

	CHSGridTable::OnResizeColWidth(nCol);
}

void CQuoteReportTable::TimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	CQuoteReportTable* pThis = (CQuoteReportTable*)idEvent; 

	pThis->DelutionHilightCells();

	if (pThis->m_mapRefreshCells.GetCount() <= 0)
	{
		::KillTimer(hwnd,idEvent);
		pThis->m_nRefreshTimerID = 0;
	}
	OutputDebugString("");
}

void CQuoteReportTable::DelayRealTimeRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	CQuoteReportTable* pThis = (CQuoteReportTable*)(idEvent-1);
	::KillTimer(hwnd,idEvent);
	pThis->m_nDelayRealTimeRequestTimerID = 0;

	CCellRange cellRange = pThis->GetVisibleNonFixedCellRange();
	// 请求即时数据（同时订阅主推）
	pThis->RequestRealTimeData(cellRange.GetMinRow() - pThis->GetFixedRowCount(),cellRange.GetRowSpan());


}

void CQuoteReportTable::DelaySortRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CQuoteReportTable* pThis = (CQuoteReportTable*)(idEvent-2);
	::KillTimer(hwnd,idEvent);
	pThis->m_nDelaySortRequestTimerID = 0;

	pThis->RequestSortData();

}