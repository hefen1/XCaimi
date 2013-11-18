#include "StdAfx.h"
#include "DrawTrendDetail.h"
#include "HSCalculate.h"
#include "GeneralHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDrawTrendDetail::CDrawTrendDetail(IDataSourceEx* pDataSource)
:CDrawTableBase()
{
	SetStyle(TABLE_STYLE_SNAKE);
	m_pDataSource = pDataSource;   

	m_lTotalVol  = 0; 
	memset(m_strPreTime, 0, sizeof(m_strPreTime));
	m_bIsShow = FALSE;
	m_bDispFour = FALSE;

	long lTime;
	m_pDataSource->HSDataSourceEx_GetServerDataTime(m_lDate, lTime);

	CreateVerScroll(HSSB_VERT | HSSB_RIGHT | HSSB_BLOCK_ALWAYBOTTOM); // 创建滚动条
	CreateTitle(); // 创建标题
}

CDrawTrendDetail::~CDrawTrendDetail(void)
{
}

BOOL CDrawTrendDetail::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}

	m_bIsShow = FALSE;
	m_pTitle->SetText(_T("分时成交列表(共0笔)"));
	//////////////////////////////////////////////////////////////////////////
	//  设置一些表格属性
	if (pStockInfo->m_ciStockCode.m_cCodeType & SH_BOURSE)
	{// 如果是上海
		m_bDispFour = FALSE;
		SetRowHeight(22);
		SetColCount(1);
		ColInfoStruct colInfo;
		colInfo.nWidth = TickCellWidth;
		SetColInfo(0, &colInfo);
	}
	else if(pStockInfo->m_ciStockCode.m_cCodeType & SZ_BOURSE)
	{// 如果是深圳
		m_bDispFour = TRUE;
		SetRowHeight(22);
		SetColCount(1);
		ColInfoStruct colInfo;
		colInfo.nWidth = TickCellWidth + 30;
		SetColInfo(0, &colInfo);
	}
	m_bIsShow = FALSE;
	m_lStockTickCount = 0;
	//////////////////////////////////////////////////////////////////////////
	// 发StockTick包
	int lAskLen = sizeof(AskData);
	AskData*  askData = (AskData*)(new char[lAskLen]);
	memset(askData, 0, lAskLen);
	askData->Emtpy();
	askData->m_nType  = RT_STOCKTICK;
	askData->m_nSize  = 1;

	askData->m_lKey		   = (long)this;
	askData->m_nPrivateKey = m_stkInfo.m_ciStockCode;
	memcpy(&askData->m_pCode[0], &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)askData,lAskLen);
	delete[] askData;

	//////////////////////////////////////////////////////////////////////////
	// 请求主推
	lAskLen = sizeof(AskData);
	askData = (AskData*)(new char[lAskLen]);
	memset(askData, 0, lAskLen);
	askData->m_nType  = RT_AUTOPUSH_EXT;
	askData->m_nSize  = 1;
	askData->m_nPrivateKey = pStockInfo->m_ciStockCode;
	memcpy(askData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)askData,lAskLen);
	delete[] askData;

	HSMarketDataType* pMarket = &m_stkInfo.m_ciStockCode.m_cCodeType;
	CArray<StockType> ayStockType;
	m_pDataSource->HSDataSourceEx_GetSystemMarketInfo((unsigned short *)pMarket, 1, ayStockType);
	m_StockType = ayStockType.GetAt(0);
	
	m_lTickCount = ::GetTickCount();
	TRACE("Tick 发包 \n");
	return TRUE;
}

int CDrawTrendDetail::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_STOCKTICK)
	{	
		TRACE("OnUpdateData - RT_STOCKTICK \n");
		HSMarketDataType nMarket = m_stkInfo.m_ciStockCode.m_cCodeType;
		AnsStockTick *pTraceData = (AnsStockTick *)pData;

		// 过滤包长度为0 以及不是当前股票信息的包
		if( pTraceData->m_nSize <= 0 || !m_stkInfo.m_ciStockCode.CompareCode(&pTraceData->m_dhHead.m_nPrivateKey.m_pCode))
			return -1;
		if (m_lStockTickCount == pTraceData->m_nSize)
		{// 相同的Stocktick包
			return -1;
		}
		m_lStockTickCount = pTraceData->m_nSize;

		//////////////////////////////////////////////////////////////////////////
		// 恢复一些标志变量的初始值
// 		m_lPreTime = -1;
// 		memset(m_strPreTime, 0, sizeof(m_strPreTime));
// 		m_lPreNewPrice = m_stkInfo.m_lPrevClose;
// 		m_lPreSellPrice = m_lPreBuyPrice = -1;

		StockTick* pStockTick = NULL;
		StockTick* pPreStockTick = NULL;
		if( MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == (HSMarketDataType)FOREIGN_MARKET 
		 || MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == (HSMarketDataType)WP_MARKET  
		  )
		{
// 			for( int i = 0; i < pTraceData->m_nSize; i++ )
// 			{
// 				if( i > 0 && pTraceData->m_traData[i].m_nTime < pTraceData->m_traData[i-1].m_nTime)
// 					continue;
// 
// 				pStockTick = new StockTick;
// 				memcpy(pStockTick, &pTraceData->m_traData[i], sizeof(StockTick));
// 			}
// 			m_lTotalVol = 0;
		}
		else
		{
			int nIndex(0);
			if (pTraceData->m_nSize > 0)
				m_nTotalHand = pTraceData->m_traData[0].m_nChiCangLiang;
			else 
				m_nTotalHand = 0;
			
			DWORD dwStart = ::GetTickCount();
			//SetCellCount(pTraceData->m_nSize);
			for( int i = 0; i < pTraceData->m_nSize; i++ )
			{	
				// 过滤不足1手的情况
				if( pPreStockTick && 
					pTraceData->m_traData[i].m_lCurrent != pPreStockTick->m_lCurrent &&
					(long)(pTraceData->m_traData[i].m_lCurrent - pPreStockTick->m_lCurrent) < m_stkInfo.m_nHandNum)
				{
					pPreStockTick = &pTraceData->m_traData[i];
					continue;
				}

				if( (pPreStockTick && pPreStockTick->m_lCurrent == pTraceData->m_traData[i].m_lCurrent)
					|| pTraceData->m_traData[i].m_lCurrent == 0 )
				{
					// 15.01分笔 
					if(pPreStockTick && pPreStockTick->m_lNewPrice == pTraceData->m_traData[i].m_lNewPrice)
					{
						pPreStockTick = &pTraceData->m_traData[i];
						continue;
					}
					else // 15.01分笔 价格必定有变化
					{
						// 15.01分笔 时间必定在240 
						if(pTraceData->m_traData[i].m_nTime < m_StockType.m_nTotalTime)
						{
							pPreStockTick =  &pTraceData->m_traData[i];
							continue;	
						}
					}
				}

				pStockTick = new StockTick;
				memcpy(pStockTick, &pTraceData->m_traData[i], sizeof(StockTick));
				// 现手			
				if( pPreStockTick )
				{
					pStockTick->m_lCurrent = pStockTick->m_lCurrent - pPreStockTick->m_lCurrent;
				}
				AddRowData(pStockTick, nIndex++);

				delete pStockTick;
				pPreStockTick = &pTraceData->m_traData[i];
			}
			SetCellCount(nIndex, TRUE); // 设置单元格数量
			DWORD dwEnd = ::GetTickCount();
			TRACE("单元格创建完成 耗时 %d \n\n", dwEnd - dwStart);
			TRACE("从发包到收包再创建 耗时 %d \n\n", dwEnd - m_lTickCount);
			if( pTraceData->m_nSize > 0 )
			{
				m_lTotalVol  = pTraceData->m_traData[pTraceData->m_nSize-1].m_lCurrent;
			}
			else
			{
				m_lTotalVol = 0;
			}	
		}
		CalCulateSize();
	
		InValidate(NULL, FALSE);
		TRACE("开始重绘 \n");
	}
	else if(pData->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if (pEveryData->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CommRealTimeData_Ext* pRealData = (CommRealTimeData_Ext *)pEveryData->m_pstData;
			CommRealTimeData_Ext* pNextData = NULL;

			int nNowSize = 0;
			for(int i = 0; i < pEveryData->m_nSize; i++)
			{
				pNextData = pRealData->GetNext(&pRealData->m_ciStockCode,nNowSize);
				if( pNextData == pRealData )
				{
					pNextData = pRealData->GetNext(&pData->m_nPrivateKey.m_pCode, nNowSize);
				}

				if (pRealData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode) && 
					pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal != m_lTotalVol
					)
				{
					StockTick* pStockTick = new StockTick;
					memset(pStockTick,0,sizeof(StockTick));
					pStockTick->m_nTime  = pRealData->m_othData.m_nTime;	
					pStockTick->m_sDetailTime.m_nSecond = (char)(pRealData->m_othData.m_sDetailTime.m_nSecond);

					pStockTick->m_lNewPrice  = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lNewPrice;
					pStockTick->m_lCurrent   = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal - m_lTotalVol;
					m_lTotalVol              = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal;
					pStockTick->m_lBuyPrice  = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lBuyPrice1;
					pStockTick->m_lSellPrice = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lSellPrice1;
					pStockTick->m_nChiCangLiang = pRealData->m_othData.m_lKaiCang;
					
					AddRowData(pStockTick, m_ayCell.GetCount());	

					delete pStockTick;
					break;
				}
				pRealData = pNextData;		
			}

			if (GetMove().y == GetScrollRange(TRUE))
			{// 如果之前的滚动条是在最底部 则主推包过来之后 滚动条一直在最底部
				DWORD dwScrollStyle = m_pVerScrollBar->GetStyle();
				dwScrollStyle |= HSSB_BLOCK_ALWAYBOTTOM;
				m_pVerScrollBar->SetStyle(dwScrollStyle);
			}
			else 
			{
				DWORD dwScrollStyle = m_pVerScrollBar->GetStyle();
				dwScrollStyle &= ~HSSB_BLOCK_ALWAYBOTTOM;
				m_pVerScrollBar->SetStyle(dwScrollStyle);
			}
			CalCulateSize();	
			InValidate(NULL, FALSE);
		}
	}
	return 1;
}

int CDrawTrendDetail::Draw( CDC* pDC )
{	
	TRACE("Draw 成交明细 \n");
	BOOL bRet = TRUE;
	bRet = CDrawTableBase::Draw(pDC);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	
	// 绘制分隔线
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *oldPen = pDC->SelectObject(&pen);
	CRect rcTable = GetTableRect();
	const ColInfoStruct* pColInfo = GetColInfo(0);
	for (int i = 1; i < GetShowColCount(); i++)
	{
		pDC->MoveTo(rcTable.left + pColInfo->nWidth*i, rcTable.top);
		pDC->LineTo(rcTable.left + pColInfo->nWidth*i, rcTable.bottom);
	}
	pDC->SelectObject(oldPen);
	pen.DeleteObject();

	DWORD dwEnd = ::GetTickCount();
	TRACE("从发包到绘制成功一共耗时 %d \n\n", dwEnd - m_lTickCount);
	return bRet;
}
void CDrawTrendDetail::AddRowData(StockTick* pStockTick, int nIndex)
{
	if (m_bIsShow == FALSE)
	{
		//////////////////////////////////////////////////////////////////////////
		// 恢复一些标志变量的初始值
		m_lPreTime = -1;
		memset(m_strPreTime, 0, sizeof(m_strPreTime));
		m_lPreNewPrice = m_stkInfo.m_lPrevClose;
		m_lPreSellPrice = m_lPreBuyPrice = -1;
		m_bIsShow = TRUE;
	}
	
	if (pStockTick == NULL || IsBadReadPtr(pStockTick, 1))
	{
		return;
	}

	CArray<ItemConstructionStruct> ayConstructionStuct;
	if (m_bDispFour)
		ayConstructionStuct.SetSize(4);
	else
		ayConstructionStuct.SetSize(3);
	ItemConstructionStruct* pItemStruct = NULL;
	
	char strText[64];
	double dValue = 0.0;

	//////////////////////////////////////////////////////////////////////////
	// 时间
	pItemStruct = &ayConstructionStuct.GetAt(0);
	pItemStruct->dwItemStyle = ITEM_TEXT_CENTER;// 单元小格风格
	pItemStruct->wClr = TEXT_COLOR_TIME;        // 文本颜色
	pItemStruct->wFont = TEXT_FONT_TIME;        // 文本字体
	pItemStruct->dWidth = 0.45;                 // 单元小格宽度
	CTime tm = CHSCalculate::GetTimer(&m_stkInfo, pStockTick, &m_StockType, TRUE, m_lDate, m_lPreTime, strText);
	m_lPreTime = pStockTick->m_nTime;

	// 比较前后两次时间 确定单元格的边框
 	DWORD dwCellSyle = 0;
	if (m_strPreTime[0] != '\0')
	{
		if ( strncmp(strText, m_strPreTime, 8) < 0)
		{// 收盘要做特殊处理 
			memset(strText, 0, sizeof(strText));
			strncpy(strText, tm.Format("%H:%M:%S").GetBuffer(), 8);
		}
		for (int i = 0; i < 5; i++)
		{
			if (m_strPreTime[i] != strText[i])
			{
				dwCellSyle |= CELL_BORDER_TOP;
				break;
			}
		}
	}
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // 字符串
	strncpy(m_strPreTime, strText, strlen(strText)+1);
	//

	//////////////////////////////////////////////////////////////////////////
	// 成交价
	pItemStruct = &ayConstructionStuct.GetAt(1);
	pItemStruct->dwItemStyle = ITEM_TEXT_LEFT | ITEM_ARROW_NEED;  // 单元小格风格
	pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // 文本字体
	pItemStruct->dWidth = 0.2;  // 单元小格宽度
	CGeneralHelper::LongToString(pStockTick->m_lNewPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // 字符串

	pItemStruct->wClr = GetTextColorID(pStockTick->m_lNewPrice, m_stkInfo.m_lPrevClose); // 颜色
	// 价格后面的箭头
	if (pStockTick->m_lNewPrice - m_lPreNewPrice > 0)
	{
		pItemStruct->dwItemStyle &= ~ITEM_ARROW_DOWN;
		pItemStruct->dwItemStyle |= ITEM_ARROW_UP;
	}
	else if (pStockTick->m_lNewPrice - m_lPreNewPrice == 0)
	{
		pItemStruct->dwItemStyle &= ~ITEM_ARROW_UP;
		pItemStruct->dwItemStyle &= ~ITEM_ARROW_DOWN;
	}	
	else
	{
		pItemStruct->dwItemStyle &= ~ITEM_ARROW_UP;
		pItemStruct->dwItemStyle |= ITEM_ARROW_DOWN;
	}
	m_lPreNewPrice = pStockTick->m_lNewPrice;


	//////////////////////////////////////////////////////////////////////////
	// 现手
	pItemStruct = &ayConstructionStuct.GetAt(2);
	pItemStruct->dwItemStyle = ITEM_TEXT_RIGHT; // 单元小格风格
	pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // 单元小格字体
	pItemStruct->dWidth = 0.2; // 单元小格宽度
	CGeneralHelper::LongToString(pStockTick->m_lCurrent, 0, m_stkInfo.m_nHandNum, 0, strText);
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // 单元小格文本内容
	pItemStruct->wClr = TEXT_COLOR_UP;  // 单元小格文本颜色
	if (m_lPreBuyPrice != -1)
	{
		if (pStockTick->m_lNewPrice <= m_lPreBuyPrice)
		{
			pItemStruct->wClr = TEXT_COLOR_DOWN;
		}
		else if (pStockTick->m_lNewPrice >= m_lPreSellPrice)
		{
			pItemStruct->wClr = TEXT_COLOR_UP;
		}
		else
		{
			if (pStockTick->m_lNewPrice <= pStockTick->m_lBuyPrice)
			{
				pItemStruct->wClr = TEXT_COLOR_DOWN;
			}
			else if (pStockTick->m_lNewPrice >= m_lPreSellPrice)
			{
				pItemStruct->wClr = TEXT_COLOR_UP;
			}
		}
	}
	else
	{
		if (pStockTick->m_lNewPrice <= pStockTick->m_lBuyPrice)
		{
			pItemStruct->wClr = TEXT_COLOR_DOWN;
		}
		else if (pStockTick->m_lNewPrice >= pStockTick->m_lSellPrice)
		{
			pItemStruct->wClr = TEXT_COLOR_UP;
		}
	}
	m_lPreSellPrice = pStockTick->m_lSellPrice;
	m_lPreBuyPrice = pStockTick->m_lBuyPrice;

	//////////////////////////////////////////////////////////////////////////
	// 深交所股票的单笔成交数
	if (m_bDispFour)
	{	
		pItemStruct = &ayConstructionStuct.GetAt(3);
		pItemStruct->dwItemStyle = ITEM_TEXT_RIGHT; // 单元小格风格
		pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // 单元小格字体
		pItemStruct->dWidth = 0.1; // 单元小格宽度
		if(pStockTick->m_nChiCangLiang <= 0) //增加对m_nChiCangLiang为空的保护
			pStockTick->m_nChiCangLiang = m_nTotalHand; 
		CGeneralHelper::LongToString((pStockTick->m_nChiCangLiang < m_nTotalHand) ? 0 : (pStockTick->m_nChiCangLiang - m_nTotalHand), 0, 1, 0, strText);
		strncpy(pItemStruct->str, strText, strlen(strText)+1); // 单元小格文本内容
		pItemStruct->wClr = TEXT_COLOR_FLAT;  // 单元小格文本颜色
		m_nTotalHand = pStockTick->m_nChiCangLiang;
	}

	CDrawCell* pCell = CreateCell(&ayConstructionStuct, dwCellSyle, nIndex);
	pCell->m_logPen.lopnStyle = PS_DOT;

	SetRowCount(nIndex+1);
	CString str;
	str.Format("分时成交列表(共%d笔)", nIndex+1);
	m_pTitle->SetText(str);
}

WORD CDrawTrendDetail::GetTextColorID( double newValue, double preValue )
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_UP;   // 涨的颜色
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_DOWN; // 跌的颜色
	}
	else
	{
		return TEXT_COLOR_FLAT; // 平盘颜色
	}
}
