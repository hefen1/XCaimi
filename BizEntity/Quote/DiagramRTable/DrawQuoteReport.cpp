#include "StdAfx.h"
#include "DrawQuoteReport.h"
#include "WinnerApplication.h"
#include "hsothercoldefine.h"
#include "ConfigStruct.h"
#include "..\QuoteComm\pageid.h"
#include "HSCalculate.h"
#include "..\..\..\HelperLib\GeneralHelper.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "QuoteSort.h"
#include "QuoteColumnSetting.h"
#include "MarkContentDlg.h"
#include "HSWinner.h"
#include "HSMessage.h"
#include "FormulaManPublic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDrawQuoteReport::CDrawQuoteReport(void)
{
	m_nCurActiveRow = -1;

	m_iColProp = (IHsColumnProperty*)CWinnerApplication::GetObject(OBJ_COLUMNPRO);
	m_iTab = (IHsTabCfg*)CWinnerApplication::GetObject(OBJ_TAB);
	m_iBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);
	m_pDataSource = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_pColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	m_pFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	m_iUserDefCfg = (IHsUserDataCfg*)CWinnerApplication::GetObject(OBJ_USERDATA);
	m_iQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	m_pImageList = NULL;

	SetStyle(TABLE_STYLE_SCREEN | TABLE_ROW_SELECT|TABLE_HSCROLL_NO_ENDING);
	m_nDelayRealTimeRequestTimerID = 0;			
    m_nDelaySortRequestTimerID = 0;	
	m_nRefreshTimerID = 0;
	m_bNeedRequestSort = FALSE;
	m_nSortColumnID = -1;

	CreateVerScroll(HSSB_VERT | HSSB_RIGHT | HSSB_BLOCK_ALWAYSTOP);
	m_pVerScrollBar->SetSize(QUOTEREPORT_SCROLL_VERT_SIZE);

	CreateHorScroll(HSSB_HORZ | HSSB_UP | HSSB_BLOCK_ALWAYLEFT);
	m_pHorScrollBar->SetSize(QUOTEREPORT_TAB_HEIGHT);
	m_pHorScrollBar->SetAutoSize(FALSE);

	m_pQuoteTabCtrl = new CDrawQuoteTabCtrl;
	m_pQuoteTabCtrl->SetNotify(this);
	
	m_strCurColGroup = "";
	CreateSelf();	
}

CDrawQuoteReport::~CDrawQuoteReport(void)
{
	KillAllTimer();
	EmptyStock();
	if (m_pQuoteTabCtrl)
	{
		delete m_pQuoteTabCtrl;
		m_pQuoteTabCtrl = NULL;
	}

	if (m_pImageList)
	{
		m_pImageList->DeleteImageList();
		delete m_pImageList;
	}
}

BOOL CDrawQuoteReport::CreateSelf()
{
	SetRowCount(1);
	SetFixRowCount(1);
	SetRowHeight(QUOTEREPORT_ROW_HEIGHT);

	if (m_pImageList == NULL)
	{
		// 加载图片资源
		IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
		HANDLE handle = pQuoteLang->LoadHImage(IDB_ALL_ICON, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if (handle)
		{
			CBitmap bit;
			bit.Attach((HBITMAP)handle);
			m_pImageList = new CImageList;
			m_pImageList->Create(16, 16, ILC_COLOR32 | ILC_MASK, MARK_COUNT + 1, 3); // 多取一张 最后一张为文字标识的图标
			BOOL bRet = m_pImageList->Add(&bit, RGB(0, 128, 128));
			bit.DeleteObject();
		}
	}
	return TRUE;
}

long CDrawQuoteReport::GetMarketNameByType(const PageInfo* pvInfo,CStringArray& strMarketNames)
{
	int nCount = 0;
	nCount = pvInfo->GetMarketTypeCount();
	WORD* markets = new WORD[nCount];
	for (int i=0,j=0; i<nCount; i++)
	{
		markets[i] = (WORD)pvInfo->GetFirstMarketType(j);
	}
	CArray<StockType> ayTypes;
	nCount = m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(markets,nCount,ayTypes);
	for (int i=0; i<nCount; i++)
	{
		strMarketNames.Add(CString(ayTypes.GetAt(i).m_stTypeName.m_szName));
	}
	delete []markets;
	ayTypes.RemoveAll();
	return nCount;
}

BOOL CDrawQuoteReport::SetPageInfo(PageInfo* pInfo, CString strColGroupName)
{
	if (pInfo == NULL || IsBadReadPtr(pInfo,1))
	{
		return FALSE;
	}
	if (pInfo->m_lPageType < PA_BEGIN || pInfo->m_lPageType > PA_END)
	{
		return FALSE;
	}

	// 不同的市场则切换标签和股票列表
	if (pInfo->m_lInPageTag & PT_BLOCK)		// 切换到板块
	{
		if (pInfo->GetBlockName().IsEmpty())
		{
			return FALSE;
		}
		m_pQuoteTabCtrl->ChangeCurItem(pInfo->GetBlockName(),FALSE);

		// 更改报价表的页面属性
		SetPageViewInfo(pInfo,strColGroupName);
		RequestStockUserInfo(pInfo);
	} 
	else if (pInfo->m_lInPageTag & PT_TYPE)	// 切换到市场
	{
		int nCount = pInfo->GetMarketTypeCount();
		if (nCount < 1)
		{
			return FALSE;
		} 
		else if (nCount == 1)		// 单个市场
		{
			
			CStringArray ayMarket;
			// 改变标签页
			if (GetMarketNameByType(pInfo, ayMarket) > 0)
			{
				m_pQuoteTabCtrl->ChangeCurItem(ayMarket.GetAt(0),FALSE);

				// 更改报价表的页面属性
				SetPageViewInfo(pInfo,strColGroupName);
				RequestStockUserInfo(pInfo);

// 				m_CurPageInfo.EmptyMarketType();
// 				m_CurPageInfo.AddMarketType(pInfo->GetFirstMarketType());
			}
			
		}
		else			// 市场组合
		{
			CStringArray ayMarket;
			// 改变标签页
			if (GetMarketNameByType(pInfo, ayMarket) > 0)
			{
				m_pQuoteTabCtrl->ChangeCurItem(ayMarket.GetAt(0),FALSE);

				// 更改报价表的页面属性
				SetPageViewInfo(pInfo);
				RequestStockUserInfo(pInfo);

				// 			m_CurPageInfo.EmptyMarketType();
				// 			int nIndex = 0;
				// 			for (int i=0; i<pInfo->GetMarketTypeCount(); i++,nIndex++)
				// 			{
				// 				m_CurPageInfo.AddMarketType(pInfo->GetFirstMarketType(nIndex));
				// 			}
			}
		}
	}
	return TRUE;

}

BOOL CDrawQuoteReport::SetPageViewInfo(PageInfo* pInfo, CString strColGroupName /*=  "" */,  BOOL bRequest /*=  FALSE */)
{
	if (memcmp(&m_pviInfo, pInfo, sizeof(PageInfo)))
	{
		memcpy(&m_pviInfo, pInfo, sizeof(PageInfo));
	}
	if ((strColGroupName.IsEmpty() && m_strCurColGroup == m_iColProp->GetDefDispColGroupName())
		|| (!strColGroupName.IsEmpty() && strColGroupName == m_strCurColGroup))
	{// 有变动时才重设当前排序状态和滚动条状
		m_pVerScrollBar->SetBlockToDefaultPosition();
		m_pHorScrollBar->SetBlockToDefaultPosition();
		SetSortColumn(-1);
		RemoveActiveRow(GetActiveRow());
	} 

	InitColGroup(strColGroupName, bRequest);
	return TRUE;
}

BOOL CDrawQuoteReport::InitColGroup( CString strGroupName, BOOL bRefreshData /* = FALSE */ )
{
	if ((strGroupName.IsEmpty() && m_strCurColGroup == m_iColProp->GetDefDispColGroupName())
		|| (!strGroupName.IsEmpty() && strGroupName == m_strCurColGroup))
	{
		return FALSE;
	} 
	else if( strGroupName.IsEmpty() )
	{
		strGroupName = m_iColProp->GetDefDispColGroupName();
	}

	// 删除所有列
	DWORD dwStart = ::GetTickCount();
	RemoveCol(0, GetColCount());
	m_mapColumn.RemoveAll();
	DWORD dwEnd = ::GetTickCount();
	TRACE("RemoveCol 共耗时 %d \n", dwEnd - dwStart);

	CUIntArray* pAyColTemp = NULL;
	m_iColProp->GetDispColGroup(strGroupName, pAyColTemp);
	if (pAyColTemp == NULL || pAyColTemp->GetCount() <= 0 || strGroupName == _T("缺省") )
	{
		AddColumn(COLUMN_HQ_BASE_NUMBER);
		AddColumn(COLUMN_HQ_BASE_CODE);
		AddColumn(COLUMN_HQ_BASE_NAME);
		AddColumn(COLUMN_HQ_BASE_NEW_PRICE);
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
	    dwStart = ::GetTickCount();
		WORD wKey = 0;
		WORD nWidth = 0;
		for(int i = 0; i < pAyColTemp->GetCount(); i++)
		{
			wKey = pAyColTemp->GetAt(i++);
			nWidth = pAyColTemp->GetAt(i);
			AddColumn(wKey, nWidth);		
		}
	    dwEnd = ::GetTickCount();
		TRACE(" AddColumn 列数为 %d 耗时 %d \n\n", pAyColTemp->GetCount(), dwEnd - dwStart);
	}
	m_strPreColGroup = m_strCurColGroup;
	m_strCurColGroup = strGroupName;

	// 重新请求当前页数据
	if (bRefreshData)
	{
		RequestStockUserInfo();
	}
	return TRUE;
}

BOOL CDrawQuoteReport::AddColumn( UINT nID, UINT nWidth)
{
	const TableColProperty* pCol = m_iColProp->GetColumn(nID);
	if (pCol)
	{
		int bFixedCol = IsFixedColumn(nID);
		int nOldPos = 0;
		nOldPos = GetColIndexByID(nID);
		if( nOldPos >= 0 )
		{// 该列已经存在
			return FALSE;
		}

		DWORD dwItemStyle(ITEM_TEXT_RIGHT);
		if( bFixedCol )
		{// 如果是固定列 有固定列的顺序		
			switch(nID)
			{
			case COLUMN_HQ_BASE_NAME:
				{
					dwItemStyle = ITEM_TEXT_LEFT;
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
					dwItemStyle = ITEM_TEXT_LEFT;
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
					dwItemStyle = ITEM_TEXT_CENTER;
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
					dwItemStyle = ITEM_TEXT_CENTER;
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
		    default: 
				nOldPos = 0; 
				dwItemStyle = ITEM_TEXT_CENTER;
				break;
	        }
		}
		else
		{// 非固定列 添加在末尾
			nOldPos = GetColCount();
		}
		m_mapColumn.SetAt(nID, nOldPos);  

		DWORD dwStart = ::GetTickCount();
		InsertCol(nOldPos, TRUE, pCol->strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, dwItemStyle,  pCol->m_nCellMask);
		DWORD dwEnd = ::GetTickCount();
		TRACE("InsertCol 共耗时 %d \n", dwEnd - dwStart);

		// 设置列属性
		ColInfoStruct sInfo;
		sInfo.nWidth = nWidth ? nWidth : pCol->m_nWidth;
		sInfo.nID = nID;
		SetColInfo(nOldPos, &sInfo);

		if (bFixedCol)
		{
			SetFixColCount(GetFixColCount()+1);
		}
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CDrawQuoteReport::IsFixedColumn(UINT nID)
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

int CDrawQuoteReport::GetColIndexByID(UINT uiID)
{	
	int nCol(-1);
	if (m_mapColumn.Lookup(uiID, nCol))
	{
		return nCol;
	}
	return nCol;
}

UINT CDrawQuoteReport::GetColIDByIndex(int nCol)
{
	const ColInfoStruct* pCol = GetColInfo(nCol);
	if (pCol)
		return pCol->nID;
	else
		return -1;
}

int CDrawQuoteReport::ChangeContent(PageInfo* pvInfo, CArray<StockUserInfo>& ayStock, CString strColGroup /*= ""*/, BOOL bForceRefresh /*= FALSE*/, BOOL bRequestData /*= TRUE*/)
{
	if (pvInfo == NULL || IsBadReadPtr(pvInfo,1))
	{
		return -1;
	}

	// 是否强制刷新内容
	if(!bForceRefresh)
	{
		BOOL bComp = 1;
		switch(pvInfo->m_lInPageTag & 0x0F00)
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
			if( !memcmp(pvInfo, &m_pviInfo,sizeof(PageInfo)) &&
				(GetRowCount() - GetFixRowCount()) != 0 )
			{
				return 1;
			}
		}
	}

	SetPageViewInfo(pvInfo,strColGroup);
	////////////////////////////////////////////////////////////////////////
	// 设置当前页的代码序号
	if (!ayStock.IsEmpty())
	{
		SetStock(ayStock);
	}
	else
	{
		EmptyStock();
	}

	// 默认按照代码排序
	BOOL bNeedRowInit(TRUE);
	if(pvInfo != NULL && pvInfo->m_lPageType == PriceAnalisys1)//SpecialReport)
	{
		if (m_nSortColumnID >= 0)
		{
			bNeedRowInit = Sort(m_nSortColumnID, m_bAscending);
		} 
		else
		{
			if( ((pvInfo->m_lInPageTag & 0x0F00) == PT_BLOCK) ||   // 板块
				((pvInfo->m_lInPageTag & 0x0F00) == PT_USERDEFINE) // 自选
				)
			{
			}
			else
			{
				bNeedRowInit = Sort(COLUMN_HQ_BASE_CODE,TRUE,TRUE);
			}
		}
		//		FindStockPos(m_pStock,pvInfo->m_ciCodeInfo,m_nCurActiveRow);
	}

	DWORD dwStart = ::GetTickCount();
	if (bNeedRowInit)
		InitRowData();
	DWORD dwEnd = ::GetTickCount();
	TRACE("InitRowData 耗时 %d \n", dwEnd - dwStart);

	KillAllTimer();
	if (bRequestData)
	{
		if (m_bNeedRequestSort)
		{
			m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
		} 
		else
		{
			m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
		}
	}

	CalCulateSize();
	InValidate(NULL, TRUE);
	return 0;
}
int CDrawQuoteReport::ChangeContent( PageInfo* pvInfo, StockUserInfo* pStock /*= NULL*/, int nTotal /*= 0*/,CString strColGroup /*= _T("")*/, BOOL bForceRefresh /*= FALSE*/, BOOL bRequestData /*= TRUE*/ )
{
	if (pvInfo == NULL || IsBadReadPtr(pvInfo,1))
	{
		return -1;
	}

	// 是否强制刷新内容
	if(!bForceRefresh)
	{
		BOOL bComp = 1;
		switch(pvInfo->m_lInPageTag & 0x0F00)
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
			if( !memcmp(pvInfo, &m_pviInfo,sizeof(PageInfo)) &&
				(GetRowCount() - GetFixRowCount()) != 0 )
			{
				return 1;
			}
		}
	}

	SetPageViewInfo(pvInfo,strColGroup);
    ////////////////////////////////////////////////////////////////////////
    // 设置当前页的代码序号
	if( (pStock != NULL) && (nTotal > 0) )
	{
		SetStock((StockUserInfo*)pStock,nTotal);
	}
	else if( nTotal > 0 )
	{
		// 去除错误代码
		for( int i = 0; i < m_ayStock.GetSize(); i++ )
		{
			pStock = GetStock(i);
			if( pStock && 
				(pStock->m_ciStockCode.m_cCodeType == 0 || pStock->m_ciStockCode.m_cCode[0] == '\0'))
			{
				delete pStock;
				m_ayStock.RemoveAt(i);
				i--;
			}
		}
	}
	else
	{
		EmptyStock();
	}

	// 默认按照代码排序
	BOOL bNeedRowInit(TRUE);
	if(pvInfo != NULL && pvInfo->m_lPageType == PriceAnalisys1)//SpecialReport)
	{
		if (m_nSortColumnID >= 0)
		{
			bNeedRowInit = Sort(m_nSortColumnID, m_bAscending);
		} 
		else
		{
			if( ((pvInfo->m_lInPageTag & 0x0F00) == PT_BLOCK) ||   // 板块
				((pvInfo->m_lInPageTag & 0x0F00) == PT_USERDEFINE) // 自选
				)
			{
			}
			else
			{
				bNeedRowInit = Sort(COLUMN_HQ_BASE_CODE,TRUE,TRUE);
//				m_nSortColumnID = COLUMN_HQ_BASE_CODE;
			}
		}
//		FindStockPos(m_pStock,pvInfo->m_ciCodeInfo,m_nCurActiveRow);
	}
	
	DWORD dwStart = ::GetTickCount();
	if (bNeedRowInit)
		InitRowData();
	DWORD dwEnd = ::GetTickCount();
	TRACE("InitRowData 耗时 %d \n", dwEnd - dwStart);

	KillAllTimer();
	if (bRequestData)
	{
		if (m_bNeedRequestSort)
		{
			m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
		} 
		else
		{
			m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
		}
	}

	CalCulateSize();
	InValidate(&GetRect(), TRUE);
	return 0;
}

void CDrawQuoteReport::EmptyStock()
{
	for (int i = 0; i < m_ayStock.GetCount(); i++)
	{
		delete m_ayStock.GetAt(i);
	}
	m_ayStock.RemoveAll();

	
// 	POSITION pos = m_mapCodeInfo.GetStartPosition();
// 	CString strCode("");
// 	StockUserInfo* pTemp= NULL;
// 	while (pos)
// 	{
// 		m_mapCodeInfo.GetNextAssoc(pos, strCode, (void*&)pTemp);
// 		delete pTemp;
// 	}
// 	m_mapCodeInfo.RemoveAll();
	
}

void CDrawQuoteReport::SetStock( CArray<StockUserInfo>& payStock )
{
	if( payStock.GetCount() <= 0 )
		return;

	EmptyStock();
	StockUserInfo* pTemp = NULL;
	StockUserInfo* pStock = NULL;
	for(int i = 0; i < payStock.GetCount(); i++)
	{
		pStock = &payStock.GetAt(i);
		if( m_nCurActiveRow == -1 &&
			!memcmp(&(pStock->m_ciStockCode), &m_pviInfo.m_ciCodeInfo, sizeof(CodeInfo)) )
			m_nCurActiveRow = i + 1;

		pTemp = new StockUserInfo;
		pTemp->Copy(pStock);
		m_ayStock.Add(pTemp);
	}
}

void CDrawQuoteReport::SetStock( StockUserInfo* pStock, int nTotal)
{
	if( !pStock || IsBadReadPtr(pStock,1) || nTotal <= 0 )
		return;

	EmptyStock();
	StockUserInfo* pTemp = NULL;
	
	m_ayStock.SetSize(nTotal);
	for(int i = 0; i < nTotal; i++)
	{
		if( m_nCurActiveRow == -1 &&
			!memcmp(&pStock[i].m_ciStockCode,&m_pviInfo.m_ciCodeInfo,sizeof(m_pviInfo.m_ciCodeInfo)) )
			m_nCurActiveRow = i + 1;

		pTemp = new StockUserInfo;
		pTemp->Copy(&pStock[i]);
		m_ayStock[i] = pTemp;
	
// 		//////////////////////////////////////////////////////////////////////////
// 		// 加入到map中
// 		pTemp = new StockUserInfo;
// 		pTemp->Copy(&pStock[i]);
// 		m_mapCodeInfo.SetAt(pTemp->GetCode(), (void*)pTemp);
	}
}

void CDrawQuoteReport::InitRowData()
{
	DWORD dwStart = ::GetTickCount();
	int nRowCount = m_ayStock.GetCount();
	CreateRowCell(nRowCount + GetFixRowCount());
	InitFixedDataItem(0, nRowCount);

	DWORD dwEnd = ::GetTickCount();
	TRACE("InitRowData 耗时 %d \n", dwEnd - dwStart);
}

void CDrawQuoteReport::InitFixedDataItem( int nFirst, int nCount)
{
//	SetRowCount(nCount + GetFixRowCount());
	DWORD dwStart = ::GetTickCount();

	StockUserInfo* pStock = NULL;
	CDrawCell* pCell = NULL;
	int nRow(0),nCol(0),nDataStart(0);
	for (nRow = GetFixRowCount(), nDataStart = nFirst; nRow <= GetRowCount() && nDataStart < nCount; nRow++,nDataStart++)
	{
		pStock = GetStock(nDataStart);
		if( pStock )
		{
			SetFixedData(nRow, pStock);
		}
	}

	DWORD dwEnd = ::GetTickCount();
	TRACE("初始化固定数据 共%d行 耗时 %d \n", nCount, dwEnd - dwStart);
}

void CDrawQuoteReport::SetFixedData(int nRow, StockUserInfo* pStock)
{
	if (nRow >= GetRowCount() || pStock == NULL)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(pStock->m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	
	//////////////////////////////////////////////////////////////////////////
	// 得到所有选股
	CMapStringToPtr mapCode;
	m_iBlockManager->GetStockList(m_iQuoteLang->LoadStr(IDS_SELF_BLOCK), mapCode);
	//////////////////////////////////////////////////////////////////////////
	// 遍历所有的行
	for (int nCol = 0; nCol < GetColCount(); nCol++)
	{
		int nCellID = nCol * GetRowCount() + nRow;
//		int  nCellID = PointToIndex(CPoint(nCol, nRow));
		CDrawCell* pCell = GetCell(nCellID);
		UINT nID = GetColIDByIndex(nCol);

		if (nID == 0 || pCell == NULL)
			continue;
		
		//财务数据 股本数据
		if( ( nID >= COLUMN_CAPITALIZATION_TOTAL && nID <= COLUMN_CAPITALIZATION_A2_GIVE ) 
			||	( nID >= COLUMN_FINANCE_TOTAL_ASSETS && nID <= COLUMN_FINANCE_ASSETS_YIELD ) )
		{
			CString str(" -");
			if (pFinanceData)
			{
				HSDouble hxData = pFinanceData->GetDataByID(nID);
// 				if( (long)hxData == hx_DefMaxValues )
// 					hxData = 0;
				str.Format("%.3f",hxData);
			}
			SetCellText(nCellID, 0, str.GetBuffer(), TEXT_COLOR_FIXEDNAME);
		}
		// 板块信息
		else if (nID >= COLUMN_BLOCK_BEGIN && nID <= COLUMN_BLOCK_END)		
		{

		}
		// 股票信息
		else
		{
			CString strText;
			switch(nID)
			{
			case COLUMN_HQ_BASE_INFO_MARK:
				if( pStock )
				{
					strText = pStock->GetCode();
				}
				else
				{
					strText = "";
				}
				SetCellText(nCellID, 0, strText.GetString(), TEXT_COLOR_STOCKCODE, TEXT_FONT_DYNAMICAL, 0, 0, FALSE);
				break;
			case COLUMN_HQ_BASE_CODE:       // 股票代码
				if( pStock )
				{				
					strText = pStock->GetCode();
					SetCellText(nCellID, 0, strText.GetString(), TEXT_COLOR_STOCKCODE, TEXT_FONT_QUOTE_CODE, 0, 0, FALSE);
				}
				break;
			case COLUMN_HQ_BASE_NAME: 		// 股票名称
				if( pStock )
				{
					strText = pStock->GetName();
					WORD wClr = TEXT_COLOR_STOCKNAME;
					void* pNull(NULL);
					if (mapCode.Lookup(pStock->GetCode(), pNull))
					// 如果是自选股 代码名称显示自选股的颜色
						wClr = TEXT_COLOR_STOCKNAME_SELF; 
					
					CDrawCell* pCell = SetCellText(nCellID, 0, strText.GetString(), wClr, TEXT_FONT_QUOTE_NAME, 0, 0, FALSE);
					if (pCell)
					{
						//////////////////////////////////////////////////////////////////////////
						// 加入标记类型
						CString strKey;
						strKey.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
						const HsCodeMark* pMark = m_iUserDefCfg->SearchMark(strKey);
						CDrawItem* pItem = GetItem(nCellID, 0);
						if (pMark)
							pItem->m_dwExtent = pMark->m_nType;
						else
							pItem->m_dwExtent = 0;
						//////////////////////////////////////////////////////////////////////////	
					}
				}
				break;
			case COLUMN_HQ_BASE_NUMBER:   	// 序号
				{
					strText.Format("%d", nRow);
					SetCellText(nCellID, 0, strText.GetString(), TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL);
				}
				break;
			case COLUMN_HQ_BASE_PRECLOSE:   // 昨收
				{
					if( pStock)
					{
						strText.Format("%.2f",(float)(pStock->m_lPrevClose)/pStock->m_nPriceUnit);
						SetCellText(nCellID, 0, strText.GetString(), TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL);
					}
				}
				break;
			case COLUMN_HQ_BASE_ARROW:
				{
					SetCellText(nCellID, 0, " ", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT);
				}
				break;
			default:
				strText = "-";
				SetCellText(nCellID, 0, strText.GetString(), TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL);
			}
		}
	}

	if (pFinanceData)
	{
		delete pFinanceData;
	}
}

StockUserInfo* CDrawQuoteReport::GetStock( int nPos )
{
	StockUserInfo* pStock;
	if( nPos >= 0 && nPos < m_ayStock.GetSize() )
	{
		pStock = m_ayStock.GetAt(nPos);
		return pStock;
	}
	return NULL;
}

StockUserInfo* CDrawQuoteReport::FindRowByCode(CodeInfo* pCode, int& nPos)
{
	if (pCode == NULL)
		return NULL;

	StockUserInfo* pStock = NULL;
	int nIndex(0);
	for (nIndex = 0; nIndex < m_ayStock.GetCount(); nIndex++)
	{
		pStock = m_ayStock.GetAt(nIndex);
		if (pStock->m_ciStockCode.CompareCode(pCode))
		{
			nPos = nIndex + GetFixRowCount();
			return pStock;
		}
	}
	return NULL;
}

void CDrawQuoteReport::OnColGroupChange(UINT nID)
{
	CString strColGroup;
	if (m_mapTempColGroup.Lookup(nID,strColGroup) && !strColGroup.IsEmpty())
	{
		SetPageViewInfo(&m_pviInfo, strColGroup, TRUE);
	}
}

void CDrawQuoteReport::OnColumnSetting()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HWND hWndParent = m_pRegion->GetHostHandle();
	CWnd* pWnd = CWnd::FromHandle(hWndParent);

	CQuoteColumnSetting dlg(m_strCurColGroup, pWnd);
	if (dlg.DoModal() == IDOK)
	{
		m_strCurColGroup = ""; // 强行请求一遍
		SetPageViewInfo(&m_pviInfo, dlg.GetCurReportGroup(), TRUE);
	}
	DWORD dw = GetLastError();
}

void CDrawQuoteReport::UpdateRefreshMap()
{
	POSITION pos = m_mapRefreshCells.GetStartPosition();
	while (pos)
	{
		int nCellID(0), nValue(0);
		m_mapRefreshCells.GetNextAssoc(pos, nCellID, nValue);
		
		nValue --;
		if (nValue < 0)
			m_mapRefreshCells.RemoveKey(nCellID);
		else
			m_mapRefreshCells.SetAt(nCellID, nValue);
	}
}

BOOL CDrawQuoteReport::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (m_strCurColGroup.IsEmpty())
	{// 第一次进入行情报价界面
		m_pQuoteTabCtrl->ChangeTabGroup("", TRUE);
	}
	else
	{// 之后进入
		KillAllTimer();
		if (m_bNeedRequestSort)
		{
			m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
		} 
		else
		{
			m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
		}
	}
	return TRUE;
}
int CDrawQuoteReport::OnUpdateData( DataHead* pData )
{
	DataHead* pHead = (DataHead*)pData;

	// 股票列表信息应答
	if (pHead->m_nType == RT_INITIALINFO)			// 行情初始化
	{								
		RequestStockUserInfo();
	} 
	else if (pHead->m_nType == RT_STOCKINFOLIST)	// 股票代码信息
	{
		AnsStockInfoList* pStockList = (AnsStockInfoList*)pData;
		if (pStockList == NULL || IsBadReadPtr(pStockList,1))
		{
			return FALSE;
		}

		PageInfo pageinfo;
		memcpy(&pageinfo,&m_pviInfo,sizeof(PageInfo));
		pageinfo.EmptyMarketType();
		HSMarketDataType m_cPreCodeType=0;
		for (int i=0; i<pStockList->m_nSize; i++)		//多市场支持
		{
			int nType = pStockList->m_pnowData[i].m_ciStockCode.m_cCodeType;
			if (m_cPreCodeType != nType)
			{
				pageinfo.AddMarketType(nType);
			}
			m_cPreCodeType = nType;
		}
		
		DWORD dwEnd = ::GetTickCount();
		TRACE("RT_STOCKINFOLIST 请求到收到 共耗时 %d \n", dwEnd - m_nTickCount);
		m_nTickCount = dwEnd;
		ChangeContent(&pageinfo, pStockList->m_pnowData, pStockList->m_nSize, m_strCurColGroup, TRUE);
		dwEnd = ::GetTickCount();
		TRACE("ChangeConten 共耗时 %d \n", dwEnd - m_nTickCount);
	}
	// 实时数据应答
	else if (pHead->m_nType == RT_REALTIME_EXT)
	{
		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
		if(pNowData->m_nSize <= 0)
		{
			return FALSE;
		}
		HandleNowData(pHead, (CommRealTimeData_Ext*)pNowData->m_pnowData, pNowData->m_nSize);
	}
	else if (pHead->m_nType == RT_REALTIME)
	{
		AnsRealTime* pNowData = (AnsRealTime*)pData;
		if (pNowData->m_nSize <= 0)
		{
			return FALSE;
		}
		HandleNowData(pHead, (CommRealTimeData*)pNowData->m_pnowData, pNowData->m_nSize);
	}
	// 主推数据
	else if (pHead->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsHSAutoPushData_Ext* pEveryData = (AnsHSAutoPushData_Ext*)pData;
		if(pEveryData->m_nSize <= 0)
		{
			return FALSE;
		}

		if (!m_bNeedRequestSort)
		{// 如果请求了 排序数据 要过滤到主推包
			HandleAutoPushData(pHead, (CommRealTimeData_Ext *)pEveryData->m_pstData, pEveryData->m_nSize);
		}
	}
	else if (pHead->m_nType == RT_AUTOPUSH)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return FALSE;
		}

		if (!m_bNeedRequestSort)
		{// 如果请求了 排序数据 要过滤到主推包
			HandleAutoPushData(pHead, (CommRealTimeData *)pEveryData->m_pstData, pEveryData->m_nSize);
		}
	}
	// 报价排序信息
	else if (pHead->m_nType == RT_REPORTSORT_EXT)			
	{
		AnsReportData* pSortData = (AnsReportData*)pData;
		if(pSortData->m_nSize <= 0)
		{
			return FALSE;
		}
		HandleSortData(pHead,pSortData);
	}
	return TRUE;
}

void CDrawQuoteReport::HandleNowData(DataHead* pHead,const CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	StockUserInfo* pStock = NULL;
	int nPos = 0;
	int nNowSize = 0;
	m_bNeedRefresh = FALSE;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}

		pStock = FindRowByCode( &pData->m_ciStockCode, nPos);
		if( pStock == NULL || 
			pStock->m_ciStockCode.m_cCodeType == 0 ||
			(pStock->m_ciStockCode).m_cCode[0] == '\0' )
		{
			pData = pNextData;
			continue;
		}
		UpdateDataEntry(nPos, pStock, (HSStockRealTime_Ext*)pData->m_cNowData, &pData->m_othData);
		pData = pNextData;		
	}

	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
	InValidate(&GetRect(), FALSE);
}

void CDrawQuoteReport::HandleNowData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData* pData = (CommRealTimeData*)pnowData;
	CommRealTimeData* pNextData = NULL;

	StockUserInfo* pStock = NULL;
	int nPos = 0;
	int nNowSize = 0;
	m_bNeedRefresh = FALSE;

	HSStockRealTime_Ext realTimeExt;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode, nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode, nNowSize);
		}

		pStock = FindRowByCode( &pData->m_ciStockCode, nPos);
		if( pStock == NULL || 
			pStock->m_ciStockCode.m_cCodeType == 0 ||
			(pStock->m_ciStockCode).m_cCode[0] == '\0' )
		{
			pData = pNextData;
			continue;
		}

		memset(&realTimeExt, 0, sizeof(HSStockRealTime_Ext));
		memcpy(&realTimeExt.m_stockRealTime, (HSStockRealTime*)pData->m_cNowData, sizeof(HSStockRealTime));
		UpdateDataEntry(nPos, pStock, &realTimeExt, &pData->m_othData);
		pData = pNextData;		
	}

	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
	InValidate(&GetRect(), FALSE);
}

void CDrawQuoteReport::HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pHead == NULL || nSize <= 0 )
		return;

	StockUserInfo* pStock = NULL;
	int nPos = 0;

	CommRealTimeData_Ext* pRealTimeData = (CommRealTimeData_Ext *)pnowData;
	CommRealTimeData_Ext* pNext = NULL;
	int nNowSize = 0;
	
	m_bNeedRefresh = TRUE;
	BOOL bDeal = FALSE;
	for( int i = 0; i < nSize; i++ )
	{
		pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);

		pStock = FindRowByCode(&pRealTimeData->m_ciStockCode,nPos);
		if(pStock == NULL)
		{
			pRealTimeData = pNext;
			continue;
		}

		if(!bDeal) 
			bDeal = TRUE;

		UpdateDataEntry(nPos, pStock, &pRealTimeData->GetShareData()->m_nowDataExt, &pRealTimeData->m_othData);
		pRealTimeData = pNext;
	}

	if( !bDeal )
		return;

	InValidate(&GetRect(), FALSE);
	if (m_nRefreshTimerID == 0)
	{
		m_nRefreshTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this, REFRESH_DISAPPEAR_TIMESPAN, RefreshProc);
	}
}

void CDrawQuoteReport::HandleAutoPushData(DataHead* pHead, const CommRealTimeData* pnowData, int nSize)
{
	if( pHead == NULL || nSize <= 0 )
		return;

	StockUserInfo* pStock = NULL;
	int nPos = 0;

	CommRealTimeData* pRealTimeData = (CommRealTimeData *)pnowData;
	CommRealTimeData* pNext = NULL;
	int nNowSize = 0;

	m_bNeedRefresh = TRUE;
	BOOL bDeal = FALSE;

	HSStockRealTime_Ext realTimeExt;
	for( int i = 0; i < nSize; i++ )
	{
		pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);

		pStock = FindRowByCode(&pRealTimeData->m_ciStockCode,nPos);
		if(pStock == NULL)
		{
			pRealTimeData = pNext;
			continue;
		}

		if(!bDeal) 
			bDeal = TRUE;

		memset(&realTimeExt, 0, sizeof(HSStockRealTime_Ext));
		memcpy(&realTimeExt.m_stockRealTime, &pRealTimeData->GetShareData()->m_nowData, sizeof(HSStockRealTime));
		UpdateDataEntry(nPos, pStock, &realTimeExt, &pRealTimeData->m_othData);
		pRealTimeData = pNext;
	}

	if( !bDeal )
		return;

	InValidate(&GetRect(), FALSE);
	if (m_nRefreshTimerID == 0)
	{
		m_nRefreshTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this, REFRESH_DISAPPEAR_TIMESPAN, RefreshProc);
	}
}

void CDrawQuoteReport::HandleSortData(DataHead* pHead, const AnsReportData* pReportData)
{
	if(pReportData == NULL || IsBadReadPtr(pReportData,1) ||pReportData->m_nSize <= 0)
	{
		return;
	}

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pReportData->m_prptData;
	int nSize = pReportData->m_nSize;	
	int nFirstRow  = GetFixRowCount() + GetMove().y;  // 起始位置

	StockUserInfo* pStockInfo = NULL;
	CommRealTimeData_Ext* pRealTimeData = pData;
	CommRealTimeData_Ext* pNext = NULL;
	int nNowSize = 0;

	m_bNeedRefresh = FALSE;
	for(int i = nFirstRow; i < nFirstRow + nSize; i++)
	{
		if( i >= GetRowCount() )
			break;

		pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);
		
		// 将当前股票信息存储到对应的队列中
		pStockInfo = GetStock(i - GetFixRowCount());
		if (!m_pDataSource->HSDataSourceEx_GetStockUserInfo(&pRealTimeData->m_ciStockCode, *pStockInfo))
		{
			pRealTimeData = pNext;
			continue;
		}

		if(pStockInfo == NULL)
		{
			pRealTimeData = pNext;
			continue;
		}

		// 填充该行的固定列值
		SetFixedData(i, pStockInfo);
	    UpdateDataEntry(i, pStockInfo, &pRealTimeData->GetShareData()->m_nowDataExt, &pRealTimeData->m_othData);

		pRealTimeData = pNext;	
		CString strText;
		strText.Format("获得排序数据 第%d行 股票名称为%s \n", i, pStockInfo->GetName());
		//TRACE(strText);
	}

	InValidate(NULL, FALSE);
	KillAllTimer();
	m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, 3000 + DELAY_REQUEST_TIMESPAN, DelaySortRequestTimerProc);
}

void CDrawQuoteReport::UpdateDataEntry(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData)
{
	if( MakeMarket(pStock->m_ciStockCode.m_cCodeType) == QH_Bourse ||
		WP_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) )		// 期货
	{
		// 			UpdateFuturesData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSQHRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( MakeIndexMarket(pStock->m_ciStockCode.m_cCodeType) )			// 指数
	{
		UpdateIndexData(nRow, pStock, (HSIndexRealTime*)&pData->m_stockRealTime, pOtherData);
	}
	else if( (HSMarketDataType)FOREIGN_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) ) // 外汇
	{
		// 		UpdateForeignData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( HK_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) )	// 港股
	{
		// 		UpdateHKData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else																	// 股票
	{
		UpdateStockData(nRow, pStock, pData, pOtherData);
	}
}

void CDrawQuoteReport::UpdateStockData(int nRow, StockUserInfo* pStock, HSStockRealTime_Ext* pData, StockOtherData* pOtherData)
{
	if (pStock == NULL || pData == NULL || pOtherData == NULL || nRow < GetFixRowCount())
		return;
	
	long  lClose = pStock->m_lPrevClose;
	short nUnit  = pStock->m_nPriceUnit;
	short nDecimal = pStock->m_nDecimal;
	int   nHand = pStock->m_nHandNum; 

	double dValue = 0.0;
	char   strText[64];

	//////////////////////////////////////////////////////////////////////////
	// 得到财务数据
	CArray<CodeInfo> ayCode;
	CArray<CurrentFinanceData> ayFinanceData;
	ayCode.Add(m_stkInfo.m_ciStockCode);
	m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
	CHSFinanceData* pFinanceData = NULL;
	if (ayFinanceData.GetCount() == 1)
		pFinanceData = new CHSFinanceData(&ayFinanceData.GetAt(0));
	//
	
	BOOL bIsStop = TRUE;//是否停牌
	if (pData->m_stockOther.m_lStopFlag == 0)
	{
		bIsStop = FALSE;
	}

	int nCol(0);
	int nCellID(0);
	// 价格的上下箭头
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		if (pData->m_stockRealTime.m_lNewPrice != 0)
		{
			if( pData->m_stockRealTime.m_lNewPrice > lClose )
			{
				UpdateCellText(nCellID, 0, "↑", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
			else if( pData->m_stockRealTime.m_lNewPrice < lClose )
			{
				UpdateCellText(nCellID, 0, "↓", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
			else
			{
				UpdateCellText(nCellID, 0, " ", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
		}
	}

	// 反汇率
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_INVERSE_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetFanHuiLv(pStock, pData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 开盘
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OPEN)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lOpen;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 成交
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_NEW_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lNewPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 涨跌
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_VALUE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZhangdie(bIsStop, pStock, &pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 总手
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_TOTAL_HAND)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZongshou(pStock, &pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 现手
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_HAND)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetXianshou(pStock, pOtherData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 最高
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MAX_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lMaxPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 最低
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MIN_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lMinPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 买入
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_BUY_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT, 0, FALSE);	
	}

	// 卖出
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_SELL_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT, 0, FALSE);
	}

	// 涨幅
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_RATIO)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetFudu(bIsStop, pStock, &pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	//涨速
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_SPEEDUP)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZhangSu(pStock, &pData->m_stockOther, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 昨收
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_PRECLOSE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = lClose;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 量比 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_VOLUME_RATIO)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetLiangbi(pStock, &pData->m_stockRealTime, pOtherData, m_pDataSource, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委买量
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_VOLUME)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount1, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委买价
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委卖量
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_VOLUME)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount1, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委卖价
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 内盘 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_IN_HANDS)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pOtherData->m_lInside, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 外盘 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OUT_HANDS)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pOtherData->m_lOutside, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 总额 
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_MONEY)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_fAvgPrice;
		CGeneralHelper::LongToString(dValue, 0, 10000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_ZONGE, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 均价 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_AVERAGE_PRICE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetJunjia(pStock, &pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 振幅
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_RANGE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZhenFu(pStock, &pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委比		
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_RATIO)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetWeiBi(&pData->m_stockRealTime, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委差
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_DIFF)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetWeicha(&pData->m_stockRealTime, nHand, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE1)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME1)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount1, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE2)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice2;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME2)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount2, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE3)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice3;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME3)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount3, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE4)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice4;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME4)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount4, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE5)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lBuyPrice5;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME5)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lBuyCount5, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 卖出价格一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE1)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice1;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME1)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount1, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE2)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice2;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME2)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount2, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE3)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice3;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME3)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount3, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 卖出价格四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE4)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice4;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME4)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount4, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE5)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lSellPrice5;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME5)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_stockRealTime.m_lSellCount5, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 基金净值
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_FUND_NETVALUE)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetJingZhi(pStock, &pData->m_stockRealTime, pFinanceData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_JINGZHI, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 国债利息
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BOND_ACCRUAL)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_stockRealTime.m_lNationalDebtRatio;
		CGeneralHelper::LongToString(dValue, 0, 1000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 换手率:某一段时期内的成交量/发行总股数＊100％ 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_EXHAND_RATIO)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetHuanshou(pStock, &pData->m_stockRealTime, pFinanceData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	//5日平均量
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_5DAY_AVGVOLUME)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pStock->m_l5DayVol;
		CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_FIXEDLEN_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_JUNLIANG, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	
	//市盈率 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_PE_RATIO)) != -1 )
	{
		//nCellID = PointToIndex(CPoint(nCol, nRow));
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetShiying(pStock, &pData->m_stockRealTime, pFinanceData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 
	if (pFinanceData)
	{
		delete pFinanceData;
		pFinanceData = NULL;
	}
}

void CDrawQuoteReport::UpdateIndexData(int nRow, StockUserInfo* pStock, HSIndexRealTime* pData, StockOtherData* pOtherData)
{
	BOOL bIsStop = TRUE;//是否停牌
	if (pData->m_lOpen > 0)
	{
		bIsStop = FALSE;
	}

	long  lClose = pStock->m_lPrevClose;
	short nUnit  = pStock->m_nPriceUnit;
	short nDecimal = pStock->m_nDecimal;
	int   nHand = pStock->m_nHandNum; // 股票手数100

	double dValue = 0.0;
	char   strText[64];

	int nCol(0);
	int nCellID(0);
	// 价格的上下箭头
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		if (pData->m_lNewPrice != 0)
		{
			if( pData->m_lNewPrice > lClose )
			{
				UpdateCellText(nCellID, 0, "↑", TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
			else if( pData->m_lNewPrice < lClose )
			{
				UpdateCellText(nCellID, 0, "↓", TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
			else
			{
				UpdateCellText(nCellID, 0, " ", TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
			}
		}
	}

	// 反汇率
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_INVERSE_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetFanHuiLv(pStock, pData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 开盘
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OPEN)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_lOpen;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 成交
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_NEW_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_lNewPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 涨跌
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_VALUE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZhangdie(bIsStop, pStock, pData, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 总手
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_TOTAL_HAND)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZongshou(pData, pStock->m_nHandNum, LTOA_MASK_HORLINE_IFZERO, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_TOTALHAND, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 现手
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_HAND)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetXianshou(pStock, pOtherData, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_XIANHAND, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 最高
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MAX_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_lMaxPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 最低
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MIN_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_lMinPrice;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);	
	}

	// 买入
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_BUY_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT, 0, FALSE);	
	}

	// 卖出
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_SELL_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT, 0, FALSE);
	}

	// 涨幅
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_RATIO)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetFudu(bIsStop, pStock, pData, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 昨收
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_PRECLOSE)) != -1 )
	{	
		nCellID = nCol * GetRowCount() + nRow;
		dValue = lClose;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_FLAT, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 量比 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_VOLUME_RATIO)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_LIANGBI, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委买量
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_VOLUME)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_lBuyCount, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委买价
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委卖量
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_VOLUME)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pData->m_lSellCount, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委卖价
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_PRICE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 内盘 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_IN_HANDS)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pOtherData->m_lInside, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_DOWN, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 外盘 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OUT_HANDS)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetBuySellCount(pStock, pOtherData->m_lOutside, strText);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 总额 
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_MONEY)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pData->m_fAvgPrice * 100;
		CGeneralHelper::LongToString(dValue, 0, 10000, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_ZONGE, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 振幅
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_RANGE)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = CHSCalculate::GetZhenFu(pStock, pData, strText);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委比		
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_RATIO)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 委差
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_DIFF)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, 0), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE1)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME1)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE2)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME2)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE3)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME3)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE4)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME4)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入价格五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE5)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 买入数量五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME5)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEIBUY, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 卖出价格一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE1)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量一
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME1)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE2)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量二
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME2)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE3)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量三
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME3)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 卖出价格四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE4)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量四
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME4)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出价格五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE5)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		CGeneralHelper::LongToString(dValue, lClose, nUnit, nDecimal, strText, LTOA_MASK_HORLINE_IFZERO);
		UpdateCellText(nCellID, 0, strText, GetColor(dValue, lClose), TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}
	// 卖出数量五
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME5)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_WEISELL, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	// 换手率:某一段时期内的成交量/发行总股数＊100％ 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_EXHAND_RATIO)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_HUANSHOU, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	//5日平均量
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_5DAY_AVGVOLUME)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = pStock->m_l5DayVol;
		CGeneralHelper::LongToString(dValue, 0, 1, 0, strText, LTOA_MASK_FIXEDLEN_IFZERO);
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_JUNLIANG, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

	//市盈率 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_PE_RATIO)) != -1 )
	{
		nCellID = nCol * GetRowCount() + nRow;
		dValue = 0.0;
		UpdateCellText(nCellID, 0, strText, TEXT_COLOR_UP, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, 0, FALSE);
	}

}

WORD CDrawQuoteReport::GetColor(double dNewValue, double dPreValue)
{
	if (dNewValue > dPreValue)
	{
		return TEXT_COLOR_UP;
	}
	else if (dNewValue == dPreValue)
	{
		return TEXT_COLOR_FLAT;
	}
	else
	{
		return TEXT_COLOR_DOWN;
	}
}

void CDrawQuoteReport::UpdateCellText(int nCellID, int nItemID, const char* strText, WORD lClr, WORD lFont, DWORD lMark, DWORD lDelMark, BOOL bNeedRedraw)
{
	if (m_bNeedRefresh)
	{
		CString str;
		GetCellText(nCellID, nItemID, str);
		str.Trim();

		CString strNewText = strText;
		strNewText.Trim();
		
		if (strNewText != str)
		{// 数据更新
			m_mapRefreshCells.SetAt(nCellID, QUOTEREPORT_REFRESH_TIMES);
// 			TRACE("刷新单元格 key:%d value:%d \n", nCellID, QUOTEREPORT_REFRESH_TIMES);
// 			TRACE("刷新单元格 map数量 %d \n", m_mapRefreshCells.GetCount());
		}
	}
	SetCellText(nCellID, nItemID, strText, lClr, lFont, lMark, lDelMark, bNeedRedraw);
}

void CDrawQuoteReport::SetSortColumn(int nCol)
{
	RemoveActiveCol(GetActiveCol());
	if (nCol >= 0)
	{
		SetActiveCol(nCol);
		int nColID = GetColIDByIndex(nCol);
		m_nSortColumnID = nColID;
		if (!IsSupportSortByCol(nCol)
			|| nColID == COLUMN_HQ_BASE_NAME 
			|| nColID == COLUMN_HQ_BASE_CODE)
		{
			m_bNeedRequestSort = FALSE;
		} 
		else
		{
			m_bNeedRequestSort = TRUE;
		}
	}
	else
	{
		m_nSortColumnID = -1;
		m_bNeedRequestSort = FALSE;
	}
}

int CDrawQuoteReport::CancelSort()
{
	if (m_nSortColumnID >= 0)
	{
		int nSortColumn = GetColIndexByID(m_nSortColumnID);
		RemoveActiveCol(m_nSortColumnID);

		KillAllTimer();
		m_nSortColumnID = -1;
		m_bAscending = FALSE;
		RequestStockUserInfo();
		return TRUE;
	}

	return FALSE;
}

BOOL CDrawQuoteReport::GetDefaultSortTypeByCol(int nCol)
{
	return GetDefaultSortTypeByColID(GetColIDByIndex(nCol));
}

BOOL CDrawQuoteReport::GetDefaultSortTypeByColID(UINT nColID)
{
	return FALSE;
}

BOOL CDrawQuoteReport::IsSupportSortByCol(int nCol)
{
	return IsSupportSortByColID(GetColIDByIndex(nCol));
}

BOOL CDrawQuoteReport::IsSupportSortByColID(UINT nColID)
{
	return (nColID != COLUMN_HQ_BASE_INFO_MARK
		&& !(nColID >= COLUMN_BLOCK_BEGIN && nColID <= COLUMN_BLOCK_END)
		&& nColID != COLUMN_HQ_BASE_NUMBER
		&& nColID != COLUMN_HQ_BASE_ARROW
		);
}

BOOL CDrawQuoteReport::IsSupportLocalSortByCol(int nCol)
{
	return IsSupportLocalSortByColID(GetColIDByIndex(nCol));
}

BOOL CDrawQuoteReport::IsSupportLocalSortByColID(UINT nColID)
{
	return (nColID == COLUMN_HQ_BASE_NUMBER || nColID == COLUMN_HQ_BASE_CODE || nColID == COLUMN_HQ_BASE_NAME
		|| (nColID >= COLUMN_CAPITALIZATION_TOTAL && nColID <= COLUMN_CAPITALIZATION_A2_GIVE)
		|| (nColID >= COLUMN_FINANCE_TOTAL_ASSETS && nColID <= COLUMN_FINANCE_ASSETS_YIELD)
	     );
}

BOOL CDrawQuoteReport::GetSortInfoByPageInfo(IN const PageInfo* pvInfo, IN short nID, OUT int& nCount,OUT HSMarketDataType& cCodeType)
{
	if( pvInfo == NULL )
		return FALSE;

	nCount    = 0;
	cCodeType = -1;

	switch(pvInfo->m_lInPageTag & 0x0F00)
	{
	case PT_INDEX:
	case PT_TYPE:
		{
			nCount = 0;

			// 组合分类
			if( (pvInfo->GetFirstMarketType() & SC_Union) == SC_Union ) 
			{
				cCodeType = SH_Bourse | SZ_Bourse | MakeSubMarket(pvInfo->GetFirstMarketType());
			}
			else
			{
				cCodeType = pvInfo->GetFirstMarketType();
			}
		}
		break;
	case PT_BLOCK:			// 数据无返回，暂时使用自定义板块相同处理
		//{
		//	nCount    = 0;
		//	cCodeType = SYSBK_BOURSE;
		//}
		//break;
	case PT_USERDEFINE:
	case PT_BLOCK_INDEX:		// 板块指数
		{
			nCount    = m_ayStock.GetCount();
			cCodeType = USERDEF_BOURSE;
		}
		break;
	default:
		return FALSE;
	}

	return (cCodeType != -1);
}

BOOL CDrawQuoteReport::Sort(UINT nColID, BOOL bAscending, BOOL bForce /*= FALSE*/ )
{
	int nPreSortColID = m_nSortColumnID;
	if (nPreSortColID == nColID && m_bAscending == bAscending)
	{// 不需要再排序
		return FALSE;
	}
	if (!bForce)
	{
		if (nPreSortColID < 0)
		{
			if (m_bAscending && nColID == COLUMN_HQ_BASE_CODE)
			{
				bAscending = FALSE;
			}
		}
		else
		{
			if (!IsSupportLocalSortByColID(nPreSortColID) && IsSupportLocalSortByColID(nColID))
			{
				// 重新初始化代码列表
				//RequestStockUserInfo();
				CArray<StockUserInfo> ayStock;
				for (int i = 0; i < MARKETTYPE_COUNT; i++)
				{
					if (m_pviInfo.m_sMarket[i] != 0)
					{
						HSMarketDataType nType = m_pviInfo.m_sMarket[i];
						CArray<StockUserInfo> ayOneMarket;
						m_pDataSource->HSDataSourceEx_GetMarketStocksInfo(nType, ayOneMarket);
						ayStock.Append(ayOneMarket);
					}
				}
				SetStock(ayStock);
			}
		}
	}

	m_bAscending = bAscending;
	m_nSortColumnID = nColID;

	int nPreSortColumn = GetColIndexByID(nPreSortColID);
	RemoveActiveCol(nPreSortColumn);
	int nSortColumn = GetColIndexByID(nColID);
	SetActiveCol(nSortColumn);

	StockUserInfo* pStock = NULL;
	int nDataCount = m_ayStock.GetCount();
	if (nColID == COLUMN_HQ_BASE_NAME
		|| nColID == COLUMN_HQ_BASE_CODE
	   )
	{
		HSSortDataStr* pStockKey = new HSSortDataStr[nDataCount];
		if( pStockKey )
		{
			for(int i = 0; i < nDataCount; i++)
			{
				pStock = m_ayStock.GetAt(i);	
				if( pStock == NULL )
					continue;

				pStockKey[i].m_pKey = pStock;
				if( nColID == COLUMN_HQ_BASE_NAME )
				{// 股票名称
					pStockKey[i].Copy(pStock->GetName());
				}
				else if(nColID == COLUMN_HQ_BASE_CODE)
				{// 股票代码
					pStockKey[i].Copy(pStock->m_ciStockCode.GetCode());
				}				
			}
			m_ayStock.RemoveAll();
			if (nColID == COLUMN_HQ_BASE_CODE && bAscending)
				SetSortColumn(-1);

			KQuickSort qsData;
			qsData.m_pData      = pStockKey;
			qsData.m_nDataLen   = sizeof(HSSortDataStr);
			qsData.m_nKeyOffset = sizeof(void*);
			qsData.m_nKeyLen    = 64;
			qsData.m_nLow       = 0;
			qsData.m_nHigh      = (short)(nDataCount - 1);

			if( bAscending )
			{
				QuickSortAscend(&qsData);
			}
			else
			{
				QuickSortDescend(&qsData);
			}

			HSSortDataStr* pData = pStockKey;
			m_ayStock.SetSize(nDataCount);
			for(int i = 0; i < nDataCount; i++,pData++)
			{
				m_ayStock.GetAt(i) = pData->m_pKey;
				//m_pStock.Add(pData->m_pKey);
			}
			delete[] pStockKey;
		}
		KillAllTimer();
		m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
		return TRUE;
	} 
	else if (
		(nColID >= COLUMN_CAPITALIZATION_TOTAL && nColID <= COLUMN_CAPITALIZATION_A2_GIVE)
		|| (nColID >= COLUMN_FINANCE_TOTAL_ASSETS && nColID <= COLUMN_FINANCE_ASSETS_YIELD)
		)
	{
		HSSortData* pStockKey = new HSSortData[nDataCount];
		if( pStockKey )
		{
			for(int i = 0; i < nDataCount; i++)
			{
				pStock = m_ayStock.GetAt(i);
				if( pStock == NULL )
					continue;

				pStockKey[i].m_pKey = pStock;
				
				//////////////////////////////////////////////////////////////////////////
				// 获取单元格文本
				int nCellID = GetColIndexByID(nColID) * GetRowCount() + (i + GetFixRowCount());
				CString str("");
				GetCellText(nCellID, 0, str);
				double dNum = strtod(str, NULL); 
				pStockKey[i].m_dValue = dNum;

				//////////////////////////////////////////////////////////////////////////
				// 得到财务数据
// 				CArray<CodeInfo> ayCode;
// 				CArray<CurrentFinanceData> ayFinanceData;
// 				ayCode.Add(pStock->m_ciStockCode);
// 				m_pDataSource->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFinanceData);
// 				if (ayFinanceData.GetCount() == 1)
// 				{
// 					CHSFinanceData financeData(&ayFinanceData.GetAt(0));
// 					HSDouble hxData = financeData.GetDataByID(nColID);
// 					pStockKey[i].m_dValue = hxData;
// 				}
			}
			m_ayStock.RemoveAll();

			KQuickSort qsData;
			qsData.m_pData      = pStockKey;
			qsData.m_nDataLen   = sizeof(HSSortData);
			qsData.m_nKeyOffset = sizeof(StockUserInfo*);
			qsData.m_nKeyLen    = QS_DOUBLE;
			qsData.m_nLow       = 0;
			qsData.m_nHigh      = (short)(nDataCount - 1);

			if( bAscending )
			{
				QuickSortAscend(&qsData);
			}
			else
			{
				QuickSortDescend(&qsData);
			}

			HSSortData* pData = pStockKey;
			m_ayStock.SetSize(nDataCount);
			for(int i = 0; i < nDataCount; i++,pData++)
			{
				m_ayStock.GetAt(i) = pData->m_pKey;
				//m_pStock.Add(pData->m_pKey);
			}
			delete[] pStockKey;
		}
		KillAllTimer();
		m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
		return TRUE;	
	}
	else
	{
		KillAllTimer();
		m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
		return FALSE;
	}
}

void CDrawQuoteReport::KillAllTimer()
{
	if (m_nDelayRealTimeRequestTimerID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nDelayRealTimeRequestTimerID);
		m_nDelayRealTimeRequestTimerID = 0;
	}
	if (m_nDelaySortRequestTimerID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nDelaySortRequestTimerID);
		m_nDelaySortRequestTimerID = 0;
	}
	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_pRegion->GetHostHandle(), m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
}

void CDrawQuoteReport::RequestStockUserInfo(PageInfo* pPageInfo)
{
	if (pPageInfo == NULL)			// 无指定市场或页面类型则重新请求当前页
	{
		pPageInfo = &m_pviInfo;
	}
	if (pPageInfo->m_lInPageTag & PT_TYPE)		// 从数据引擎请求指定 市场 的股票代码列表
	{
		CArray<StockUserInfo> ayStock;
		for (int i = 0; i < MARKETTYPE_COUNT; i++)
		{
			if (pPageInfo->m_sMarket[i] != 0)
			{
				HSMarketDataType nType = pPageInfo->m_sMarket[i];
				CArray<StockUserInfo> ayOneMarket;
				m_pDataSource->HSDataSourceEx_GetMarketStocksInfo(nType, ayOneMarket);
				ayStock.Append(ayOneMarket);
			}
		}
		ChangeContent(pPageInfo, ayStock, m_strCurColGroup, TRUE);		
	}
	else if (pPageInfo->m_lInPageTag & PT_BLOCK)
	{
		CArray<CodeInfo> ayCodeList;
		CArray<StockUserInfo> ayStockUserInfo;
		m_iBlockManager->GetStockList(pPageInfo->GetBlockName(),ayCodeList);
		if (ayCodeList.GetCount() > 0)
		{
			m_pDataSource->HSDataSourceEx_GetStocksUserInfo(ayCodeList,ayStockUserInfo,TRUE);
		}
		ChangeContent(pPageInfo, ayStockUserInfo, _T(""), TRUE);
	}
}

void CDrawQuoteReport::RequestRealTimeData(int nStart, int nTotal)
{
	if (nStart < 0 ||  nTotal > m_ayStock.GetCount())
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// 找出指数和非指数股票
	CArray<StockUserInfo*> ayIndex;
	CArray<StockUserInfo*> ayStock;
	StockUserInfo* pStock = NULL;
	for (int i = nStart; i <= nStart + nTotal; i++)
	{
		pStock = GetStock(i);
		if( pStock != NULL && !IsBadReadPtr(pStock,1))
		{
			// 国内指数
			if (MakeIndexMarket(pStock->m_ciStockCode.m_cCodeType) == TRUE)
				ayIndex.Add(pStock);
			else 
				ayStock.Add(pStock);
		}
	}
	
	if (!ayStock.IsEmpty())
	{// 股票
		int nCount = ayStock.GetCount();
		AskData*  paskData;
		int nRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCount-1);
		paskData = (AskData*) new char[nRetLen];
		memset(paskData, 0, nRetLen);

		paskData->m_nType  = RT_REALTIME_EXT;
		paskData->m_nSize  = nCount;
		paskData->m_lKey   = (long)this;
		CodeInfo* pCodeInfo = paskData->m_pCode;
		StockUserInfo* pStock = NULL;
		for(int i = 0; i < nCount; i++)
		{
			pStock = ayStock.GetAt(i);
			if( pStock != NULL && !IsBadReadPtr(pStock,1))
			{
				memcpy(&pCodeInfo[i], &pStock->m_ciStockCode,sizeof(CodeInfo));
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 发送请求
		IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
		if (pReqCollector)
		{
			pReqCollector->AddRequest((char*)paskData, nRetLen);
			// 同时订阅主推
			paskData->m_nType = RT_AUTOPUSH_EXT;
			pReqCollector->AddRequest((char*)paskData, nRetLen);

			m_pRegion->SendRequest(pReqCollector);
			pReqCollector->Release();
		}
		ayStock.RemoveAll();
		delete[] paskData;
	}

	if (!ayIndex.IsEmpty())
	{// 指数
		int nCount = ayIndex.GetCount();
		AskData*  paskData;
		int nRetLen = sizeof(AskData) + sizeof(CodeInfo)*(nCount-1);
		paskData = (AskData*) new char[nRetLen];
		memset(paskData, 0, nRetLen);

		paskData->m_nType  = RT_REALTIME;
		paskData->m_nSize  = nCount;
		paskData->m_lKey   = (long)this;
		CodeInfo* pCodeInfo = paskData->m_pCode;
		StockUserInfo* pStock = NULL;
		for(int i = 0; i < nCount; i++)
		{
			pStock = ayIndex.GetAt(i);
			if( pStock != NULL && !IsBadReadPtr(pStock,1))
			{
				memcpy(&pCodeInfo[i], &pStock->m_ciStockCode,sizeof(CodeInfo));
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 发送请求
		IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
		if (pReqCollector)
		{
			pReqCollector->AddRequest((char*)paskData, nRetLen);
			// 同时订阅主推
			paskData->m_nType = RT_AUTOPUSH;
			pReqCollector->AddRequest((char*)paskData, nRetLen);

			m_pRegion->SendRequest(pReqCollector);
			pReqCollector->Release();
		}
		ayIndex.RemoveAll();
		delete[] paskData;
	}
}

void CDrawQuoteReport::RequestSortData(PageInfo* pPageInfo, BOOL bNeedAutoPush)
{
	if (!pPageInfo)
	{
		pPageInfo = &m_pviInfo;
	}
	
	if (m_nSortColumnID < 0)
		return;
	
	int nStockSize = 0;			//   =请求结构长度/sizeof(CodeInfo) 
	int nCount;
	HSMarketDataType cCodeType;
	if( !GetSortInfoByPageInfo(pPageInfo, m_nSortColumnID, nCount, cCodeType) )
	{
		m_nSortColumnID = -1;
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
	pAnyReportDataReq->m_nBegin		= GetMove().y;
	pAnyReportDataReq->m_nCount		= GetShowRowCount() - GetFixRowCount() + 1;		//+1防止显示半行的行没有排序数据
	pAnyReportDataReq->m_cAlignment = 1;

	pAnyReportDataReq->m_nColID     = m_nSortColumnID;
	pAnyReportDataReq->m_bAscending = !m_bAscending;
	pAnyReportDataReq->m_nSize		= nCount;

	CString strText;
	strText.Format("\n发请求 当前排列ID=%d %s \n", m_nSortColumnID, m_bAscending?"升序":"降序");
	TRACE(strText);
	strText.Format("请求包 cCodeType = %d nBegin = %d nCount = %d nSize = %d \n\n", pAnyReportDataReq->m_cCodeType,
		pAnyReportDataReq->m_nBegin, 
		pAnyReportDataReq->m_nCount,
		pAnyReportDataReq->m_nSize);
	TRACE(strText);

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
			memcpy(&pAnyReportData->m_ciStockCode, &ayStock.GetAt(i).m_ciStockCode, sizeof(CodeInfo));
		}
	}
	else if( cCodeType == SYSBK_BOURSE ) // 系统板块
	{
		ReqOneBlock* pBlockData = (ReqOneBlock*)pAnyReportDataReq->m_sAnyReportData;
		pBlockData->m_lCRC = 0;
		strncpy(pBlockData->m_szBlockName,(*pPageInfo).m_szBlock,sizeof(pBlockData->m_szBlockName));
	}

	//////////////////////////////////////////////////////////////////////////
	// 发送请求
	IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
	if (pReqCollector)
	{
		pReqCollector->AddRequest((char*)paskData, lRetLen);
		m_pRegion->SendRequest(pReqCollector);
		pReqCollector->Release();
	}

	delete[] paskData;

}

void CDrawQuoteReport::MoveCol( int nBegin, int nEnd )
{
	if (nBegin < GetFixColCount() || nBegin >= GetColCount() || nEnd < GetFixColCount() || nEnd > GetColCount())
	{// 传入数据非法
		return;
	}

	if (nBegin == nEnd || nBegin + 1 == nEnd)
	{// 位置不变不需要移动
		return;
	}
	
	// 基类功能要先实现
	__super::MoveCol(nBegin, nEnd);

	// map重新加载一遍
	m_mapColumn.RemoveAll();
	int nColCount = GetColCount();
	for (int i = 0; i < GetColCount(); i++)
	{
		const ColInfoStruct* pCol = GetColInfo(i);
		m_mapColumn.SetAt(pCol->nID, i);
	}
}

BOOL CDrawQuoteReport::Scroll( int nDiff, BOOL bIsVert /*= TRUE*/, BOOL bReDraw /*= TRUE*/ )
{
	BOOL bRet = __super::Scroll(nDiff, bIsVert, TRUE);
	if (bRet)
	{
		if (bIsVert)
		{// 垂直滚动
			KillAllTimer();
			if (m_bNeedRequestSort)
			{
				m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
			}
			else
			{
				m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1,DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
			}
		}
		else
		{// 水平滚动
			
		}
	}
	return bRet;
}

BOOL CDrawQuoteReport::SetPosition( int nPosition, BOOL bIsVert, BOOL bIsRedraw /*= TRUE*/ )
{
	BOOL bRet = __super::SetPosition(nPosition, bIsVert, TRUE);
	if (bRet)
	{
		if (bIsVert)
		{// 垂直滚动
			KillAllTimer();
			if (m_bNeedRequestSort)
			{
				m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
			}
			else
			{
				m_nDelayRealTimeRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+1,DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
			}
		}
	}
	return bRet;
}

void CDrawQuoteReport::OnFixedRowClick()
{
	int nCellID = GetActiveCellID();    // 获取当前单击到的单元格下标
	CPoint pos = IndexToPoint(nCellID); // 转换为二维坐标
	int nCol = GetColumn(pos);          // 得到当前选中列
	
	UINT nColID = GetColIDByIndex(nCol); // 得到当前选中列ID
	if (IsSupportSortByColID(nColID))
	{// 如果当前列支持排序
	
		if ( !IsSupportLocalSortByColID(nColID) )
			m_bNeedRequestSort = TRUE;
		else
			m_bNeedRequestSort = FALSE;
		DWORD dwStart = ::GetTickCount();
		BOOL bNeedRowInit = Sort(nColID, (m_nSortColumnID == nColID) ? !m_bAscending : GetDefaultSortTypeByColID(nColID));
		DWORD dwEnd = ::GetTickCount();
		TRACE("行情报价 Sort函数 耗时 %d \n", dwEnd - dwStart);
		if (bNeedRowInit)
			InitRowData();
	}
	else
	{
		CancelSort();
	}
	InValidate(&GetRect(), FALSE);
}

BOOL CDrawQuoteReport::Draw( CDC* pDC )
{
	m_pQuoteTabCtrl->DrawTab(pDC);
	CDrawTableBase::Draw(pDC);
	return TRUE;
}

BOOL CDrawQuoteReport::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem)
{
	int nCellID = PointToIndex(pos);
	int nColumn = GetColumn(pos);
	UINT nColID = GetColIDByIndex(nColumn);
	if (IsCellRowFixed(pos) && nColID == m_nSortColumnID)
	{// 固定行 排序列头加上排序箭头
		CString strText;
		GetCellText(nCellID, -1, strText);
		strText.Trim();
		if (m_bAscending)
			strText.AppendFormat("%s", "↑");
		else
			strText.AppendFormat("%s", "↓");

		int nBkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF oldClrText = pDC->SetTextColor(m_pColor->GetRTableFixedName());
		CFont*   pOldFont = pDC->SelectObject(m_pFont->GetRTableFixedFont());

		pDC->DrawText(strText, strText.GetLength(), &rcItem, DT_VCENTER | DT_SINGLELINE | DT_RIGHT);

		pDC->SetTextColor(oldClrText);
		pDC->SelectObject(pOldFont);
		pDC->SetBkMode(nBkMode);

		return TRUE;
	}
	else
	{// 非固定行

		//////////////////////////////////////////////////////////////////////////
		// 寻找高亮单元格
		int nValue(0);
		if (m_mapRefreshCells.Lookup(nCellID, nValue))
		{
			CDrawCell* pCell = GetCell(nCellID);
			COLORREF clrDefaultBK;
			if (pCell->m_dwStyle & CELL_BK_HIGHLIGHT)
				clrDefaultBK = m_pColor->GetQuoteHighLightBKColor();
			else
			    clrDefaultBK = m_pColor->GetBlackgroundColor();
			
			COLORREF clrHighLightBK = m_iColProp->GetQuoteRefreshProp()->m_dwBkColor;
			COLORREF clrBK = CGeneralHelper::DelutionColor(clrHighLightBK, clrDefaultBK, (QUOTEREPORT_REFRESH_TIMES - nValue) * 100 / QUOTEREPORT_REFRESH_TIMES);

			rcItem.DeflateRect(0,1,0,1);
			pDC->FillSolidRect(&rcItem, clrBK);
		}
		//////////////////////////////////////////////////////////////////////////
		// 如果是 列名
		if (nColID == COLUMN_HQ_BASE_NAME)
		{
			CDrawItem* pItem = GetItem(nCellID, nItemID);
			if (pItem && pItem->m_dwExtent != 0)
			{
				int nImage(-1);
				if (pItem->m_dwExtent == -1)
				{// 文字标识
					nImage = 10; // 取第十个图片
				}
				else
				{// 普通图片标识
					nImage = pItem->m_dwExtent - 1;
				}
				CRect rc = rcItem;   // 图片矩形大小
				rc.left = rc.right - 16;
				m_pImageList->Draw(pDC, nImage, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
				
				rcItem.right = rc.left; // 输出文字的区域变小
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}
	return FALSE;	
}

CString CDrawQuoteReport::GetTipText(int nCellID, int nItemID)
{
	CString strText("");
	GetCellText(nCellID, nItemID, strText);
	
	int nColumn = GetColumn(nCellID);
	int nColID = GetColIDByIndex(nColumn);
	if (nColID == COLUMN_HQ_BASE_NAME)
	{
		CDrawItem* pItem = GetItem(nCellID, nItemID);
		if (pItem && pItem->m_dwExtent)
		{
			int nRow = GetRow(nCellID);
			StockUserInfo* pStock = GetStock(nRow - 1);
			if (pStock)
			{
				CString strKey;
				strKey.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
				const HsCodeMark* pMark = m_iUserDefCfg->SearchMark(strKey);
				if (pMark)
					strText = pMark->m_strContent;
			}
		}
	}
	return strText;
}

BOOL CDrawQuoteReport::SetRect(CRect rc)
{
	if (rc.IsRectEmpty())
	{
		return FALSE;
	}

	if (rc.Height() < QUOTEREPORT_TAB_HEIGHT)
	{
		rc.left = rc.right - QUOTEREPORT_SCROLL_HORT_WIDTH;
		m_pHorScrollBar->SetRect(rc);

		rc.right = rc.left - 1;
		m_pQuoteTabCtrl->SetRect(rc);

		CDrawTableBase::SetRect(CRect(0,0,0,0));
	}
	else
	{
		CRect rcScroll = rc;
		rcScroll.bottom = rcScroll.top + QUOTEREPORT_TAB_HEIGHT;
		rcScroll.left = rcScroll.right - QUOTEREPORT_SCROLL_HORT_WIDTH;	
		m_pHorScrollBar->SetRect(rcScroll);

		CRect rcTab = rcScroll;
		rcTab.left = rc.left;
		rcTab.right = rcScroll.left - 1;
		m_pQuoteTabCtrl->SetRect(rcTab);

		rc.top = rcTab.bottom + 1;
		CRect rcPreRegion = GetRect();
		BOOL bRequset(FALSE);
		if (rc.Width() > rcPreRegion.Width() || rc.Height() > rcPreRegion.Height())
		// 区域变大 则需要重新请求数据		
			bRequset = TRUE;
			
		CDrawTableBase::SetRect(rc);
		if (bRequset)
		{
			KillAllTimer();
			if (m_bNeedRequestSort)
			{
				m_nDelaySortRequestTimerID = SetTimer(m_pRegion->GetHostHandle(), (long)this+2, DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelaySortRequestTimerProc);
			}
			else
			{
				m_nDelayRealTimeRequestTimerID = ::SetTimer(m_pRegion->GetHostHandle(), (long)this+1,DELAY_REQUEST_TIMESPAN, &CDrawQuoteReport::DelayRealTimeRequestTimerProc);
			}
		}
	}
	return TRUE;
}

int	CDrawQuoteReport::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_F1)
	{// 按F1 进入F1界面
		int nRow = GetActiveRow();
		GotoDealDetail(nRow - 1);
		return TRUE;
	}
	else if (nChar == VK_F2)
	{// 按F2 进入F2界面
		int nRow = GetActiveRow();
		GotoPriceBook(nRow - 1);
		return TRUE;
	}
	else if (nChar == VK_F5)
	{// 按F5 切换到选中股票的分时
		int nRow = GetActiveRow();
		GotoTechAnalysis(nRow - 1);
		return TRUE;
	}
	else if (nChar == VK_F6)
	{// 按F6 进入自选股
		CString strSelfBlock = m_iQuoteLang->LoadStr(IDS_SELF_BLOCK);
		m_pQuoteTabCtrl->ChangeCurItem(strSelfBlock);
		return TRUE;
	}
	else if (nChar == VK_F10)
	{// 按F10 进入财务数据
		int nRow = GetActiveRow();
		GotoFinanceInfo(nRow - 1);
		return TRUE;
	}
	else if (nChar == 'Z')
	{// alt + Z 加入自选股
		if ( ::GetAsyncKeyState(VK_MENU) & 0x8000 )
		{
			int nRow = GetActiveRow();
			OnAddToSelfBlock(nRow);
			return TRUE;
		}
	}
	else if (nChar == 'R')
	{// Ctrl + R 所属板块
		if ( ::GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		{
			int nRow = GetActiveRow();
			OnBlockAdscription(nRow);
			return TRUE;
		}
	}
	else if (nChar >= '1' && nChar <= '9')
	{// Ctrl + 1...9 添加标记
		if ( ::GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		{
			UINT uID = nChar - '0' + ID_MARK_TAG_BEGIN;
			int nRow = GetActiveRow();
			OnMarkCodeByIcon(uID, nRow);
			return TRUE;
		}
	}
	else if (nChar == '0')
	{// Ctrl + 0 添加标记
		if ( ::GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		{
			int nRow = GetActiveRow();
			OnMarkCodeByIcon(ID_MARK_TAG_BEGIN + 10, nRow);
			return TRUE;
		}
	}
	else if (nChar == 'S')
	{// Ctrl + S 添加文字
		if ( ::GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		{
			int nRow = GetActiveRow();
			OnMarkCodeByString(nRow);
			return TRUE;
		}
	}
	else if (nChar == VK_DELETE)
	{
		// Ctrl + Delete 删除标记
		if (::GetAsyncKeyState(VK_CONTROL) & 0x8000 )
		{
			int nRow = GetActiveRow();
			OnCancelMarkCode(nRow);
			return TRUE;
		}
		else 
		{
			// 删除自选板块
			if (m_pviInfo.GetBlockName() == m_iQuoteLang->LoadStr(IDS_SELF_BLOCK))
			{// 当前是自选板块
				int nRow = GetActiveRow();
				OnBlockDelete(nRow);
				return TRUE;
			}
		}
	}
	else if (nChar == VK_TAB)
	{// 切换财务
		CString strFinanceGroupName = m_iColProp->GetFinanceGroupName();
		if (!strFinanceGroupName.IsEmpty())
		{
			SetPageViewInfo(&m_pviInfo, strFinanceGroupName==m_strCurColGroup?m_strPreColGroup:strFinanceGroupName, TRUE);	
			return TRUE;
		}
	}
	else if (nChar == VK_PRIOR)
	{// page up
		int nPageRow = GetShowRowCount() - GetFixRowCount();
		int nPosition = GetMove().y + nPageRow * (-1);
		BOOL bRet = SetPosition(nPosition, TRUE);
		if (bRet == FALSE)
		{// 已经到达顶部 
			nPosition = GetScrollRange(TRUE);
			SetPosition(nPosition, TRUE);
			m_pVerScrollBar->SetBottom();
		}
		else
		{
			if (GetMove().y == 0)
				m_pVerScrollBar->SetTop();
			else
				m_pVerScrollBar->OnScroll(nPageRow * (-1));
		}
		return TRUE;
	}
	else if (nChar == VK_NEXT)
	{// page down
		int nPageRow = GetShowRowCount() - GetFixRowCount();
		int nPosition = GetMove().y + nPageRow;
		BOOL bRet = SetPosition(nPosition, TRUE);
		if (bRet == FALSE)
		{// 已经到达底部
			nPosition = 0;
			SetPosition(nPosition, TRUE);
			m_pVerScrollBar->SetTop();
		}
		else
		{
			if (GetMove().y == GetScrollRange(TRUE))
				m_pVerScrollBar->SetBottom();
			else 
				m_pVerScrollBar->OnScroll(nPageRow);
		}
		return TRUE;
	}
	else if (nChar == VK_HOME)
	{// 返回到顶部
		TRACE("VK_HOME \n");
		SetPosition(0, TRUE);
		m_pVerScrollBar->SetTop();
		return TRUE;
	}
	else if (nChar == VK_END)
	{// 返回底部
		TRACE("VK_END \n");
		int nPosition = GetScrollRange(TRUE);
		SetPosition(nPosition, TRUE);
		m_pVerScrollBar->SetBottom();
		return TRUE;
	}
	else if (nChar == VK_RETURN)
	{// 切换到选中股票的分时
		int nRow = GetActiveRow();
		GotoTechAnalysis(nRow - 1);
		return nRow;
	}
	else if (nChar == VK_LEFT)
	{// 按键盘左键
		Scroll(-1, FALSE, TRUE);
		return TRUE;
	}
	else if (nChar == VK_RIGHT)
	{// 按键盘右键
		Scroll(1, FALSE, TRUE);
		return TRUE;
	}
	else if (nChar == VK_ESCAPE)
	{// 按ESC键 取消排序
		RemoveActiveRow(GetActiveRow());
		if ( CancelSort() == FALSE)
		{
			InValidate(NULL, FALSE);
		}
		return TRUE;
	}

	return CDrawTableBase::OnKey(nChar, nRepCnt, nFlags);
}

void CDrawQuoteReport::OnLayoutNotify(UINT nMsgId,IDrawBaseParam* pParam)
{
	if (nMsgId == DiagramMsg_BlockChange)
	{// 板块改变
		CString strBlockName = pParam->GetStringParam(Param_BlockChange_BlockName);
		m_pQuoteTabCtrl->ChangeCurItem(strBlockName);
	}
}

BOOL CDrawQuoteReport::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == HSCurKeyInfo::KeyCommand)
	{// 键盘精灵消息

		BOOL bSort(FALSE);
		int nMarketType = GetMarketType(lParam, bSort); // 解析参数
		if (nMarketType != -1)
		{
			CArray<StockType> ayType;
			unsigned short unMarket = nMarketType;
			m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(&unMarket, 1, ayType);
			if (ayType.GetCount() == 1)
			{
				m_pQuoteTabCtrl->ChangeCurItem(ayType.GetAt(0).m_stTypeName.m_szName, TRUE, unMarket);
				if (bSort)
				{
					EnSureVisible(COLUMN_HQ_BASE_RISE_RATIO);
					RemoveActiveRow(GetActiveRow());
					m_bNeedRequestSort = TRUE;
					Sort(COLUMN_HQ_BASE_RISE_RATIO, 0);
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDrawQuoteReport::MouseMove( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	if (m_pQuoteTabCtrl->MouseMove(nFlags, point))
	{
		return TRUE;
	}
	return CDrawTableBase::MouseMove(nFlags, point, pData);
}

BOOL CDrawQuoteReport::MouseDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	if (m_pQuoteTabCtrl->MouseDown(nFlags, point))
	{
		return TRUE;
	}
	return CDrawTableBase::MouseDown(nFlags, point, pData);
}

BOOL CDrawQuoteReport::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData)
{
	CDrawTableBase::MouseDBDown(nFlags, point, pData);
	
	int nCellID = GetActiveCellID();
	int nRow = GetRow(nCellID);
	
	StockUserInfo* pInfo = GetStock(nRow - 1);
	if (pInfo)
	{
		int nCol = GetColumn(nCellID);
		int nColID = GetColIDByIndex(nCol);

		CString strPrice("");
		GetCellText(nCellID, 0, strPrice);
		double dPrice = strtod(strPrice, NULL);
		if (dPrice > 0.0001)
		{
			if (nColID == COLUMN_HQ_BASE_BUY_PRICE)
			{// 闪电卖出
				//////////////////////////////////////////////////////////////////////////
				// 添加参数
				CDrawBaseParam param;
				param.AddParam(PARAM_QUICKTRADE_PRICE, strPrice);
				param.AddParam(PARAM_QUICKTRADE_CODE, pInfo->GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
				return TRUE;
			}
			else if (nColID == COLUMN_HQ_BASE_SELL_PRICE)
			{// 闪电买入
				//////////////////////////////////////////////////////////////////////////
				// 添加参数
				CDrawBaseParam param;
				param.AddParam(PARAM_QUICKTRADE_PRICE, strPrice);
				param.AddParam(PARAM_QUICKTRADE_CODE, pInfo->GetCode());
				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_BUY);
				return TRUE;
			}
		}

		GotoTechAnalysis(nRow - 1);
	}
	return FALSE;
}

BOOL CDrawQuoteReport::RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData)
{
	CDrawTableBase::RButtonDown(nFlags, point, hWndParent, pData);
    // 弹出菜单之后 鼠标移动无法捕捉 会导致第一次左键下去的位置错误 因此重新调用一遍mouseMove
	BOOL bNeedMove(FALSE); 
	int nCellID  = GetActiveCellID();
	if (nCellID == -1)
		return FALSE;

	if (IsCellRowFixed(nCellID))
	{// 在标题行右键

		CMenu popMenu;
		popMenu.CreatePopupMenu();
		if (FALSE)
		{// 设置过滤条件 先阶段 还没有实现
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
					popMenu.AppendMenu(0, ID_QUOTE_DISP_COLGROUP_BEGIN+i, strDispCol);
					if (m_strCurColGroup == strDispCol)
					{
						popMenu.CheckMenuItem(ID_QUOTE_DISP_COLGROUP_BEGIN+i, MF_BYCOMMAND|MF_CHECKED);
					}
					m_mapTempColGroup.SetAt(ID_QUOTE_DISP_COLGROUP_BEGIN+i, strDispCol);
				}
			}
			popMenu.AppendMenu(MF_SEPARATOR);
		}
		popMenu.AppendMenu(0, ID_COLUMN_SETTING, m_iQuoteLang->LoadStr(ID_COLUMN_SETTING));
		int nMenuID(-1);
		if (popMenu.GetMenuItemCount() > 0)
		{
			bNeedMove = TRUE;
			::ClientToScreen(hWndParent, &point);
			CWnd* pWnd = CWnd::FromHandle(hWndParent);
			nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, pWnd);
		}
		
		if (nMenuID > 0)
		{
			if (nMenuID == ID_COLUMN_SETTING)
			// 选中列设定 
				OnColumnSetting();
			else
			// 选中列菜单 更改显示列
				OnColGroupChange(nMenuID);
		}
		popMenu.DestroyMenu();
	}
	else
	{// 在活动列右键

		CMenu popMenu;
		CMenu subMenu;
		subMenu.CreatePopupMenu();
		popMenu.CreatePopupMenu();

		popMenu.AppendMenu(0, IDS_GOTO_DEAlDETAIL, m_iQuoteLang->LoadStr(IDS_GOTO_DEAlDETAIL)); // 分时走势
		popMenu.AppendMenu(0, IDS_GOTO_PRICEBOOK, m_iQuoteLang->LoadStr(IDS_GOTO_PRICEBOOK));   // 技术分析
		popMenu.AppendMenu(MF_SEPARATOR);
		popMenu.AppendMenu(0, IDS_GOTO_FINANCEINFO, m_iQuoteLang->LoadStr(IDS_GOTO_FINANCEINFO));// F10财务数据
		popMenu.AppendMenu(MF_SEPARATOR);                                                         

		popMenu.AppendMenu(MF_POPUP, (UINT_PTR)subMenu.m_hMenu, m_iQuoteLang->LoadStr(IDS_MARK_CURCODE));
		subMenu.AppendMenu(0, ID_MARK_STRING, m_iQuoteLang->LoadStr(ID_MARK_STRING)+_T("\tCTRL+S"));
		CBitmap bitmap[MARK_COUNT];	// for test
		for (int i = 1; i <= MARK_COUNT; i++)
		{
			CString strText;
			strText.Format("%s-%d\tCTRL+%d", m_iQuoteLang->LoadStr(ID_MARK_TAG_BEGIN), i, i%10);
			subMenu.AppendMenu(0, ID_MARK_TAG_BEGIN + i, strText);
			CGeneralHelper::GetListImage(*m_pImageList, bitmap[i-1], i - 1);
			BOOL bRes = subMenu.SetMenuItemBitmaps(i, MF_BYPOSITION, &bitmap[i-1], &bitmap[i-1]);
		}
		popMenu.AppendMenu(0, ID_CANCEL_MARK_CODE, m_iQuoteLang->LoadStr(ID_CANCEL_MARK_CODE) + _T("\tCTRL+DEL"));
		CString strIndex;
		int nCurRow = GetRow(nCellID);
		StockUserInfo* pStock = GetStock(nCurRow - 1);
		if (pStock)
		{
			strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
			if (!m_iUserDefCfg->SearchMark(strIndex))
			{
				popMenu.EnableMenuItem(ID_CANCEL_MARK_CODE, MF_BYCOMMAND|MF_GRAYED);
			}
		}
		popMenu.AppendMenu(MF_SEPARATOR);
		popMenu.AppendMenu(0,ID_BLOCK_ADSCRIPTION,m_iQuoteLang->LoadStr(ID_BLOCK_ADSCRIPTION));
		//popMenu.AppendMenu(0,ID_ADD_TO_BLOCK,m_iQuoteLang->LoadStr(ID_ADD_TO_BLOCK));
		if (m_pviInfo.GetBlockName() == m_iQuoteLang->LoadStr(IDS_SELF_BLOCK))
		{// 当前是自选板块
			popMenu.AppendMenu(0, ID_BLOCK_DELETE, m_iQuoteLang->LoadStr(ID_BLOCK_DELETE));
		}
		else
		{// 当前非自选板块
			popMenu.AppendMenu(0,ID_ADD_TO_SELFBLOCK, m_iQuoteLang->LoadStr(ID_ADD_TO_SELFBLOCK));
		}
		
		int nMenuID(-1);
		if (popMenu.GetMenuItemCount() > 0)
		{
			bNeedMove = TRUE;
			::ClientToScreen(hWndParent, &point);
			CWnd* pWnd = CWnd::FromHandle(hWndParent);
			nMenuID = popMenu.TrackPopupMenu(TPM_VERPOSANIMATION | TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,point.x,point.y, pWnd);
		}

		if (nMenuID > 0)
		{
			if (nMenuID == ID_CANCEL_MARK_CODE)
			{// 取消标记
				OnCancelMarkCode(nCurRow);
			}
			else if (nMenuID == ID_MARK_STRING)
			{// 添加文本内容
				OnMarkCodeByString(nCurRow);
			}
			else if (nMenuID >= ID_MARK_TAG_BEGIN && nMenuID <= ID_MARK_TAG_END)
			{// 添加图标
				OnMarkCodeByIcon(nMenuID, nCurRow);
			}
			else if (nMenuID == ID_BLOCK_ADSCRIPTION)
			{// 所属板块
				OnBlockAdscription(nCurRow);
			}
			else if (nMenuID == ID_ADD_TO_SELFBLOCK)
			{// 加入自选板块
				OnAddToSelfBlock(nCurRow);
			}
			else if (nMenuID == ID_BLOCK_DELETE)
			{// 从板块数据删除
				OnBlockDelete(nCurRow);
			}
			else if (nMenuID == IDS_GOTO_DEAlDETAIL)
			{// 切换到分时
				GotoTechAnalysis(nCurRow - 1);
			}
			else if (nMenuID == IDS_GOTO_PRICEBOOK)
			{// 切换到K线
				GotoTechAnalysis(nCurRow - 1, 2);
			}
			else if (nMenuID == IDS_GOTO_FINANCEINFO)
			{// 切换到F10
				GotoFinanceInfo(nCurRow - 1);
			}
		}

		for (int i = 0; i < MARK_COUNT; i++)
		{
			bitmap[i].DeleteObject();
		}
		subMenu.DestroyMenu();
		popMenu.DestroyMenu();
	}

	if (bNeedMove)
	{// 解决菜单弹出之后 第一次按下 无法正确判断位置的问题
		::GetCursorPos(&point);
		::ScreenToClient(hWndParent, &point);
		MouseMove(nFlags, point, pData);
	}
	return TRUE;
}

void CDrawQuoteReport::OnMarkCodeByIcon(UINT nID, int nRow)
{	
	HsCodeMark mark;
	CString strIndex;
	mark.m_nType = nID - ID_MARK_TAG_BEGIN;
	StockUserInfo* pStock = GetStock(nRow - 1);
	if (pStock)
	{
		strIndex.Format("%04x%s", pStock->m_ciStockCode.m_cCodeType, pStock->GetCode());
		strncpy(mark.m_szIndex, strIndex, BLOCK_NAME_LENGTH);
		if (m_iUserDefCfg->SetMark(mark))
		{ 
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			int nCellID = nCol * GetRowCount() + nRow;
			CDrawItem* pItem = GetItem(nCellID, 0);
			pItem->m_dwExtent = mark.m_nType;

			CRect rc;
			GetCellRect(nCellID, rc);
			InValidate(&rc, FALSE);    
		}
	}
}

void CDrawQuoteReport::OnMarkCodeByString(int nRow)
{
	CString strIndex;
	HsCodeMark mark;
	mark.m_nType = -1;
	StockUserInfo* pStock = GetStock(nRow - 1);
	if (pStock)
	{
		strIndex.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
		strncpy(mark.m_szIndex,strIndex,BLOCK_NAME_LENGTH);
		POINT point;
		::GetCursorPos(&point);
		mark.m_strContent = GetMarkContent(CPoint(point));

		if (!mark.m_strContent.IsEmpty() && m_iUserDefCfg->SetMark(mark))
		{
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			int nCellID = nCol * GetRowCount() + nRow;
			CDrawItem* pItem = GetItem(nCellID, 0);
			pItem->m_dwExtent = mark.m_nType;

			CRect rc;
			GetCellRect(nCellID, rc);
			InValidate(&rc, FALSE);  
		}
	}
}

CString CDrawQuoteReport::GetMarkContent(CPoint& point)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMarkContentDlg markDlg(point);
	if (markDlg.DoModal() == IDOK)
	{
		return markDlg.GetContent();
	}
	return _T("");
}

void CDrawQuoteReport::OnCancelMarkCode(int nRow)
{
	CString strIndex;
	StockUserInfo* pStock = GetStock(nRow - 1);
	if (pStock)
	{
		strIndex.Format("%04x%s", pStock->m_ciStockCode.m_cCodeType, pStock->GetCode());
		if (m_iUserDefCfg->DelMark(strIndex))
		{
			int nCol = GetColIndexByID(COLUMN_HQ_BASE_NAME);
			int nCellID = nCol * GetRowCount() + nRow;
			CDrawItem* pItem = GetItem(nCellID, 0);
			pItem->m_dwExtent = 0;

			CRect rc;
			GetCellRect(nCellID, rc);
			InValidate(&rc, FALSE);  
		}
	}
}

void CDrawQuoteReport::OnBlockAdscription(int nRow)
{
	StockUserInfo* pInfo = GetStock(nRow - 1);
	if (pInfo)
	{
		CString strCodeType(""), strCode("");
		strCodeType.Format("%d", pInfo->m_ciStockCode.m_cCodeType);
		strCode = pInfo->m_ciStockCode.GetCode();

		CDrawBaseParam param;
		param.AddParam(PARAM_BLOCK_ADCRIPTION_CODETYPE, strCodeType);
		param.AddParam(PARAM_BLOCK_ADCRIPTION_CODE, strCode);
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_BLOCK_ADSCRIPTION, &param, PN_BLOCKADSCRIPTION);
	}
}

void CDrawQuoteReport::OnAddToSelfBlock(int nRow)
{
	StockUserInfo* pStock = GetStock(nRow-1);
	if (pStock)
	{
		m_iBlockManager->AddStockToBlock(m_iQuoteLang->LoadStr(IDS_SELF_BLOCK), &pStock->m_ciStockCode);
	}
}

void CDrawQuoteReport::OnBlockDelete(int nRow)
{
	StockUserInfo* pStock = GetStock(nRow-1);
	if (pStock)
	{
		m_iBlockManager->DelStockFromBlock(m_iQuoteLang->LoadStr(IDS_SELF_BLOCK), &pStock->m_ciStockCode);
		RemoveRow(nRow);
		InValidate(NULL, FALSE);
	}
}

void CDrawQuoteReport::GotoDealDetail(int nRow)
{
	StockUserInfo* pStockInfo = GetStock(nRow);
	if (pStockInfo)
	{
		CString strType("");
		strType.Format("%d", pStockInfo->m_ciStockCode.m_cCodeType);
		CString strID("");
		strID.Format("%d", DiagramMsg_OpenLayout);

		CDrawBaseParam param;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_DealDetail);
		param.AddParam(PARAM_DIAGRAMMSGID, strID);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, pStockInfo->GetCode());
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
	}
}
void CDrawQuoteReport::GotoPriceBook(int nRow)
{
	StockUserInfo* pStockInfo = GetStock(nRow);
	if (pStockInfo)
	{
		CString strType("");
		strType.Format("%d", pStockInfo->m_ciStockCode.m_cCodeType);
		CString strID("");
		strID.Format("%d", DiagramMsg_OpenLayout);

		CDrawBaseParam param;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_PriceBook);
		param.AddParam(PARAM_DIAGRAMMSGID, strID);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, pStockInfo->GetCode());
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
	}
}
void CDrawQuoteReport::GotoFinanceInfo(int nRow)
{
	StockUserInfo* pStockInfo = GetStock(nRow);
	if (pStockInfo)
	{
		CString strType("");
		strType.Format("%d", pStockInfo->m_ciStockCode.m_cCodeType);
		CString strID("");
		strID.Format("%d", DiagramMsg_OpenLayout);

		CDrawBaseParam param;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_F10Info);
		param.AddParam(PARAM_DIAGRAMMSGID, strID);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, pStockInfo->GetCode());
		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
	}
}

void CDrawQuoteReport::GotoTechAnalysis(int nRow, int nMark)
{
	StockUserInfo* pStockInfo = GetStock(nRow);
	if (pStockInfo)
	{
		CString strType("");
		strType.Format("%d", pStockInfo->m_ciStockCode.m_cCodeType);
		CString strID("");
		strID.Format("%d", DiagramMsg_OpenLayout);
		CString strMark("");
		strMark.Format("%d", nMark);

		CDrawBaseParam param;
		param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_TechAnalysis);
		param.AddParam(PARAM_DIAGRAMMSGID, strID);
		param.AddParam(Param_Open_MarketType, strType);
		param.AddParam(Param_Open_StockCode, pStockInfo->GetCode());
		param.AddParam(Param_Open_Mark, strMark);

		m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
	}
}

int CDrawQuoteReport::GetMarketType(LPARAM lparam, BOOL& bSort)
{
	switch (lparam)
	{
	case KeyShangAQuote:       // 上A
		bSort = FALSE;
		return STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
	case KeyShangARange:
		bSort = TRUE;
		return STOCK_MARKET | SH_BOURSE | KIND_STOCKA;
	case KeyShangBQuote:       // 上B
		bSort = FALSE;
		return STOCK_MARKET | SH_BOURSE | KIND_STOCKB;
	case KeyShangBRange:
		bSort = TRUE;
		return STOCK_MARKET | SH_BOURSE | KIND_STOCKB;
	case KeyShengAQuote:       // 深A
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKA;
	case KeyShengARange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKA;
	case KeyShengBQuote:       // 深B
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKB;
	case KeyShengBRange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKB;
	case KeyShangBondQuote:    // 上债
		bSort = FALSE;
		return STOCK_MARKET | SH_BOURSE | KIND_BOND;
	case KeyShangBondRange:
		bSort = TRUE;
		return STOCK_MARKET | SH_BOURSE | KIND_BOND;
	case KeyShengBondQuote:    // 深债
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_BOND;
	case KeyShengBondRange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_BOND; 
	case KeyShangFundQuote:    // 上金
		bSort = FALSE;
		return STOCK_MARKET | SH_BOURSE | KIND_FUND;
	case KeyShangFundRange:
		bSort = TRUE;
		return STOCK_MARKET | SH_BOURSE | KIND_FUND;
	case KeyShengFundQuote:    // 深金
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_FUND;
	case KeyShengFundRange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_FUND;
	case KeySmallStocksQuote:  // 中小盘
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_SMALLSTOCK;   
	case KeySmallStocksRange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_SMALLSTOCK;  
	case KeyGEM:               // 创业板
		bSort = FALSE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKGE; 
	case KeyGEMRange:
		bSort = TRUE;
		return STOCK_MARKET | SZ_BOURSE | KIND_STOCKGE; 
	default:
		bSort = FALSE;
		return -1;
	}
}

void CDrawQuoteReport::EnSureVisible(int nColID)
{
	int nColPos = GetColIndexByID(nColID);
	if (nColPos == -1)
	{
		//////////////////////////////////////////////////////////////////////////
		// 不存在 要添加
		TableColProperty* pColProp = m_iColProp->GetColumn(nColID);
		if (pColProp)
		{
			nColPos = GetFixColCount();
			InsertCol(nColPos, TRUE, pColProp->strName, TEXT_COLOR_FIXEDNAME, TEXT_FONT_FIXED, ITEM_TEXT_RIGHT, pColProp->m_nCellMask);
			// 设置列属性
			ColInfoStruct sInfo;
			sInfo.nWidth = pColProp->m_nWidth;
			sInfo.nID = pColProp->m_ColumnID;
			SetColInfo(nColPos, &sInfo);
			// map重置
			m_mapColumn.RemoveAll();
			int nColCount = GetColCount();
			for (int i = 0; i < nColCount; i++)
			{
				const ColInfoStruct* pCol = GetColInfo(i);
				m_mapColumn.SetAt(pCol->nID, i);
			}
		}
	}

	int nShowLeft = GetFixColCount() + GetMove().x;    // 当前能显示的起始活动列
	int nShowRight = nShowLeft + (GetShowColCount() - GetFixColCount() - 1);// 当前能显示的最终活动列
	if (nColPos < nShowLeft)
	{// 需要滚动条往左移动
		Scroll(nColPos - nShowLeft, FALSE, TRUE);
	}
	else if (nColPos > nShowRight)
	{// 需要滚动条右移动
		Scroll(nColPos - nShowRight, FALSE, TRUE);
	}
}
void CDrawQuoteReport::OnActiveTabChange( CDrawQuoteTab* pActiveItem)
{
	if (pActiveItem)
	{
		SetPageInfo(pActiveItem->GetPageInfo(), pActiveItem->GetGroupName());
	}
}

void CDrawQuoteReport::TabInvalidate( CRect* pRect, BOOL bErase /*= TRUE*/ )
{
	InValidate(pRect, bErase);
}

CWnd* CDrawQuoteReport::TabGetCWnd()
{
	HWND hwnd = m_pRegion->GetHostHandle();
	CWnd* pWnd = NULL;
	if (hwnd)
	{
		pWnd = CWnd::FromHandle(hwnd);
	}
	return pWnd;
}

VOID CALLBACK CDrawQuoteReport::RefreshProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CDrawQuoteReport* pThis = (CDrawQuoteReport*)(idEvent);
	if (pThis->m_mapRefreshCells.GetCount() <= 0 || pThis->GetVisible() == FALSE)
	{
		::KillTimer(hwnd,idEvent);
		pThis->m_nRefreshTimerID = 0;
		pThis->m_mapRefreshCells.RemoveAll();
	}
	else
	{		
		pThis->InValidate(&pThis->GetTableRect(), FALSE);
		pThis->UpdateRefreshMap();
	}
}

VOID CALLBACK CDrawQuoteReport::DelayRealTimeRequestTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	CDrawQuoteReport* pThis = (CDrawQuoteReport*)(idEvent-1);
	KillTimer(hwnd, idEvent);
	pThis->m_nDelayRealTimeRequestTimerID = 0;

	// 请求即时数据（同时订阅主推）
	int nStart = pThis->GetMove().y;
	int nTotal = pThis->GetShowRowCount() - pThis->GetFixRowCount();
	pThis->RequestRealTimeData(nStart, nTotal);
}

VOID CALLBACK CDrawQuoteReport::DelaySortRequestTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CDrawQuoteReport* pThis = (CDrawQuoteReport*)(idEvent-2);
	::KillTimer(hwnd,idEvent);
	pThis->m_nDelaySortRequestTimerID = 0;

	pThis->RequestSortData();
}
