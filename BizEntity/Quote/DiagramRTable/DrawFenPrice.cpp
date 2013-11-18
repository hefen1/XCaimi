#include "StdAfx.h"
#include "DrawFenPrice.h"
#include "resource.h"
#include <atlcore.h>
#include "GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CDrawFenPrice::CDrawFenPrice(IDataSourceEx* pDataSource, IHsColor* pColor, IHsFont* pFont)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE);
	m_pDataSource = pDataSource;	
	m_pColor = pColor;
	m_pFont  = pFont;

	m_clrTitleBK  = m_pColor->GetTableTitleBK();
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT); // 创建滚动条
	CreatSelf();
}

CDrawFenPrice::~CDrawFenPrice(void)
{
	RemoveAllData();
}

void CDrawFenPrice::CreatSelf()
{
	// 构造第一行 一个单元格中分为四个单元小格
	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	CString str;
	//

	//////////////////////////////////////////////////////////////////////////
	// 分价表(序号)
	::LoadString(instance, IDS_STRING_FENJIABIAO, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// 成交价
	::LoadString(instance, IDS_STRING_CHENGJIAOJIA, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// 成交量
	::LoadString(instance, IDS_STRING_CHENGJIAOLIANG, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// 比例
	::LoadString(instance, IDS_STRING_BILI, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	InitSize(1);
}

void CDrawFenPrice::InitSize(int nRowCount)
{
	// 行属性
	SetRowCount(nRowCount);
	SetFixRowCount(1);
	SetRowHeight(FENPRICE_ROW_HEIGHT);
	// 列属性
	SetColCount(FENPRICE_COL_COUNT);
	SetFixColCount(3);
	ColInfoStruct colInfo;
	colInfo.nWidth = FENPRICE_COL_WIDTH_NUM1;
	SetColInfo(0, &colInfo);

	colInfo.nWidth = FENPRICE_COL_WIDTH_NUM2;
	SetColInfo(1, &colInfo);

	colInfo.nWidth = FENPRICE_COL_WIDTH_NUM3;
	SetColInfo(2, &colInfo);

	colInfo.nWidth = FENPRICE_COL_WIDTH_NUM4;
	SetColInfo(3, &colInfo);
}

int CDrawFenPrice::Draw( CDC* pDC )
{
	TRACE("成交明细收到 Draw \n");
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
	int nWidth(0);
	const ColInfoStruct* pColInfo = NULL;
	for (int i = 0; i < GetShowColCount()-1; i++)
	{
		pColInfo = GetColInfo(i);
		nWidth += pColInfo->nWidth;
		pDC->MoveTo(rcTable.left + nWidth, rcTable.top);
		pDC->LineTo(rcTable.left + nWidth, rcTable.bottom);
	}
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	return bRet;
}

BOOL CDrawFenPrice::DrawSelf(CDC* pDC, CPoint pos, int nItemID, CRect& rcItem)
{
	// 第一行的 二、三、四列单元格 需要背景色
	if (pos.y == 0)
	{
		if (pos.x > 0 && pos.x < 4)
		{
			// 画背景
			pDC->FillSolidRect(&rcItem, m_clrTitleBK);
		}
		return FALSE; // 该单元格 还需要在进行绘制
	}
	// 后面几行的 第四列 需要背景需要绘制比例图
	else
	{
		if (m_bIsShow == FALSE)
		{// 如果没有数据 不需要绘制 直接返回
			return TRUE;
		}

		long lMaxVolume = FindMaxVolume();
		CPoint posMove = GetMove();
		if (pos.x == 3 && (pos.y + posMove.y - 1) < m_ayFenPriceData.GetCount())
		{
			// 画比例图
			CFenPriceData* pPriceData = m_ayFenPriceData.GetAt(pos.y + posMove.y - 1);
			double dVolume = pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume;
			// 计算当前比例图的宽度
			double dWidth = rcItem.Width() * dVolume / lMaxVolume;
			
			// 先画内盘(卖盘)
			CRect rc = rcItem;
			rc.DeflateRect(1,3,0,3);

			int nNeiWidth = (int)(dWidth * pPriceData->m_lSellVolume / dVolume);
			if (nNeiWidth > 0)
			{
				rc.right = rc.left + nNeiWidth;
				pDC->FillSolidRect(&rc, RGB(255,255,255));
				rc.DeflateRect(1,1,1,1);
				pDC->FillSolidRect(&rc, m_pColor->GetDownColor());
			}

			// 再画外盘(买盘)
			rc = rcItem;
			rc.DeflateRect(1,3,0,3);

			int nWaiWidth = (int)(dWidth * pPriceData->m_lBuyVolume / dVolume);
			if (nWaiWidth > 0)
			{
				rc.left = rc.left + nNeiWidth; // 先除去内盘的大小
				rc.right = rc.left + nWaiWidth;
				
				pDC->FillSolidRect(&rc, RGB(255,255,255));
				rc.DeflateRect(1,1,1,1);
				pDC->FillSolidRect(&rc, m_pColor->GetUpColor());
				// 绘制分隔线
				if (nNeiWidth > 0)
				{
					CPen pen(PS_SOLID, 1, RGB(0,0,0));
					CPen *oldPen = pDC->SelectObject(&pen);
					pDC->MoveTo(rc.left, rc.top);
					pDC->LineTo(rc.left, rc.bottom);
					pDC->SelectObject(oldPen);
					pen.DeleteObject();
				}
			}

			// 获取比例文字
			char strText[64];
			double dValue = (double)(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume) * 100 / (double)(m_lTotalVol);
			CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_SHOW_PERSENT);
			CString str = strText;
			// 设置绘图模式
			int nBkMode = pDC->SetBkMode(TRANSPARENT);
			// 设置字体
			CFont* pOldFont = pDC->SelectObject(m_pFont->GetRTableDynamicalFont());
			// 设置颜色 先绘制一次固定颜色字体
			COLORREF clrOld = pDC->SetTextColor(m_pColor->GetRTableFixedName());
			pDC->DrawText(str, str.GetLength(), &rcItem, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			// 再在比例图上 绘制蓝色
			rc = rcItem;
			rc.right = rc.left + nNeiWidth + nWaiWidth;
			pDC->SetTextColor(RGB(0,0,255));
			pDC->DrawText(str, str.GetLength(), &rc, DT_VCENTER | DT_SINGLELINE | DT_LEFT);

			pDC->SetBkMode(nBkMode);
			pDC->SelectObject(pOldFont);
			pDC->SetTextColor(clrOld);
		
			return TRUE; // 绘制结束
		}
		return FALSE;
	}
}

BOOL CDrawFenPrice::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if(pStockInfo == NULL)
		return FALSE;

	m_bIsShow = FALSE;
	//////////////////////////////////////////////////////////////////////////
	// 恢复一些标志变量的初始值
	m_lPreNewPrice = m_stkInfo.m_lPrevClose;
	m_lPreSellPrice = m_lPreBuyPrice = -1;
	m_lStockTickCount = 0;
	RemoveAllData();

	//////////////////////////////////////////////////////////////////////////
	// 发StockTick包
	int lAskLen = sizeof(AskData);
	AskData*  askData = (AskData*)(new char[lAskLen]);
	memset(askData, 0, lAskLen);
	askData->Emtpy();
	askData->m_nType  = RT_STOCKTICK;
	askData->m_nSize  = 1;

	askData->m_lKey		   = (long)this;
	askData->m_nPrivateKey.m_pCode = m_stkInfo.m_ciStockCode;
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
	memcpy(askData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)askData,lAskLen);
	delete[] askData;

	HSMarketDataType* pMarket = &m_stkInfo.m_ciStockCode.m_cCodeType;
	CArray<StockType> ayStockType;
	m_pDataSource->HSDataSourceEx_GetSystemMarketInfo((unsigned short *)pMarket, 1, ayStockType);
	m_StockType = ayStockType.GetAt(0);

	return TRUE;
}

int CDrawFenPrice::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_STOCKTICK)
	{
		TRACE("成交明细收到 RT_STOCKTICK \n");
		HSMarketDataType nMarket = m_stkInfo.m_ciStockCode.m_cCodeType;
		AnsStockTick *pTraceData = (AnsStockTick *)pData;

		// 过滤包长度为0 以及不是当前股票信息的包
		if( pTraceData->m_nSize <= 0 || !m_stkInfo.m_ciStockCode.CompareCode(&pTraceData->m_dhHead.m_nPrivateKey.m_pCode))
			return -1;
		// 过滤数据缓存与服务器相同的包
		if (pTraceData->m_nSize == m_lStockTickCount)
			return -1;
		m_lStockTickCount = pTraceData->m_nSize;

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

				InsertArray(pStockTick); // 将StockTick数据添加到分笔结构数组中

				delete pStockTick;
				pPreStockTick = &pTraceData->m_traData[i];
			}
			if( pTraceData->m_nSize > 0 )
			{
				m_lTotalVol = pTraceData->m_traData[pTraceData->m_nSize-1].m_lCurrent;
			}
			else
			{
				m_lTotalVol = 0;
			}
		}
		// 通过得到的数据 更新单元格
		UpdateCell();
		m_bIsShow = TRUE;
		CalCulateSize();
		
		InValidate(NULL, FALSE);
	}
	else if(pData->m_nType == RT_AUTOPUSH_EXT)
	{
		TRACE("成交明细收到 RT_AUTOPUSH_EXT \n");
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

					pStockTick->m_lNewPrice  = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lNewPrice;
					pStockTick->m_lCurrent   = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal - m_lTotalVol;
					m_lTotalVol              = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal;
					pStockTick->m_lBuyPrice  = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lBuyPrice1;
					pStockTick->m_lSellPrice = pRealData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lSellPrice1;
				
					InsertArray(pStockTick); // 将StockTick数据添加到分笔结构数组中
					
					delete pStockTick;		
					break;
				}
				pRealData = pNextData;		
			}
			// 通过得到的数据 更新单元格
			UpdateCell();
			CalCulateSize();
			InValidate(NULL, FALSE);
		}
	}
	return TRUE;
}

void CDrawFenPrice::InsertArray(StockTick* pStockTick)
{
	if (pStockTick == NULL)
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// 从StockTick中计算出 买量和卖量
	long lBuyCount(0), lSellCount(0); // 买量和卖量
	if (m_lPreBuyPrice != -1)
	{
		if (pStockTick->m_lNewPrice <= m_lPreBuyPrice)
		{
			lSellCount = pStockTick->m_lCurrent;
		}
		else if (pStockTick->m_lNewPrice >= m_lPreSellPrice)
		{
			lBuyCount = pStockTick->m_lCurrent;
		}
		else
		{
			if (pStockTick->m_lNewPrice <= pStockTick->m_lBuyPrice)
			{
				lSellCount = pStockTick->m_lCurrent;
			}
			else if (pStockTick->m_lNewPrice >= m_lPreSellPrice)
			{
				lBuyCount = pStockTick->m_lCurrent;
			}
			else
				lBuyCount = pStockTick->m_lCurrent; // 如果一直比不出来 就认为是买量
		}
	}
	else
	{
		if (pStockTick->m_lNewPrice <= pStockTick->m_lBuyPrice)
		{
			lSellCount = pStockTick->m_lCurrent;
		}
		else if (pStockTick->m_lNewPrice >= pStockTick->m_lSellPrice)
		{
			lBuyCount = pStockTick->m_lCurrent;
		}
		else 
			lBuyCount = pStockTick->m_lCurrent;   // 如果一直比不出来 就认为是买量
	}
	m_lPreSellPrice = pStockTick->m_lSellPrice;
	m_lPreBuyPrice = pStockTick->m_lBuyPrice;
	//////////////////////////////////////////////////////////////////////////
	
	// 从数组中需要 是否该价格已经存在
	CFenPriceData* pExitPriceData = NULL;
	for (int i = 0; i < m_ayFenPriceData.GetCount(); i++)
	{
		pExitPriceData = m_ayFenPriceData.GetAt(i);
		if (pExitPriceData->m_lPrice == pStockTick->m_lNewPrice)
		{
			break;
		}
		pExitPriceData = NULL;
	}

	if (pExitPriceData)
	{// 如果已经存在
		pExitPriceData->m_lBuyVolume  += lBuyCount;
		pExitPriceData->m_lSellVolume += lSellCount;
		pExitPriceData->m_lTurnover ++; // 该价位上笔数加一
	}
	else
	{// 是新的价格
		CFenPriceData* pPriceData = new CFenPriceData;
		pPriceData->m_lPrice = pStockTick->m_lNewPrice;
		pPriceData->m_lBuyVolume = lBuyCount;
		pPriceData->m_lSellVolume = lSellCount;
		pPriceData->m_lTurnover = 1;

		int i(0);
		for (i = 0; i < m_ayFenPriceData.GetCount(); i++)
		{// 一直到找到比当前价格小的项为止
			pExitPriceData = m_ayFenPriceData.GetAt(i);
			if (pPriceData->m_lPrice > pExitPriceData->m_lPrice)
				break;
		}
		m_ayFenPriceData.InsertAt(i, pPriceData);
	}
}

void CDrawFenPrice::RemoveAllData()
{
	CFenPriceData* pPriceData = NULL;
	for (int i = 0; i < m_ayFenPriceData.GetCount(); i++)
	{
		pPriceData = m_ayFenPriceData.GetAt(i);
		delete pPriceData;
		pPriceData = NULL;
	}
	m_ayFenPriceData.RemoveAll();
}

void CDrawFenPrice::UpdateCell()
{
	if (m_ayFenPriceData.GetCount() < 1)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// 读取资源字符串
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	CString str;
	char    strText[64];
	//

	CFenPriceData* pPriceData = NULL;

	//RemoveAllCell();
	int nRowCount = m_ayFenPriceData.GetSize() + 1; // 当前所有数据 + 固定行
	int nCellIndex(0);
	SetCellCount(nRowCount * FENPRICE_COL_COUNT, TRUE);

	//////////////////////////////////////////////////////////////////////////
	// 第一列 分价表(序号)
	::LoadString(instance, IDS_STRING_FENJIABIAO, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM, 0, nCellIndex++);
	for (int i = 0; i < nRowCount - 1; i++)
	{
		str.Format("%d", i + 1);
		CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, CELL_TIP, 0, nCellIndex++);
	}

	//////////////////////////////////////////////////////////////////////////
	// 第二列 成交价
	::LoadString(instance, IDS_STRING_CHENGJIAOJIA, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM, 0, nCellIndex++);
	for (int i = 0; i < nRowCount - 1; i++)
	{
		pPriceData = m_ayFenPriceData.GetAt(i);
		CGeneralHelper::LongToString(pPriceData->m_lPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
		WORD wClr = TEXT_COLOR_FLAT;
		if (pPriceData->m_lPrice > m_stkInfo.m_lPrevClose)
			wClr = TEXT_COLOR_UP;
		else if (pPriceData->m_lPrice == m_stkInfo.m_lPrevClose)
			wClr = TEXT_COLOR_FLAT;
		else
			wClr = TEXT_COLOR_DOWN;
		CreateCell(strText, wClr, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, CELL_TIP, 0, nCellIndex++);
	}

	//////////////////////////////////////////////////////////////////////////
	// 第三列 成交量
	::LoadString(instance, IDS_STRING_CHENGJIAOLIANG, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM, 0, nCellIndex++);
	for (int i = 0; i < nRowCount - 1; i++)
	{
		pPriceData = m_ayFenPriceData.GetAt(i);
		CGeneralHelper::LongToString(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume, 0, m_stkInfo.m_nHandNum, 0, strText);
		CreateCell(strText, TEXT_COLOR_CHENGJIAO, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, CELL_TIP, 0, nCellIndex++);
	}

	//////////////////////////////////////////////////////////////////////////
	// 第四列 比例
	::LoadString(instance, IDS_STRING_BILI, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM, 0, nCellIndex++);
	for (int i = 0; i < nRowCount - 1; i++)
	{
		pPriceData = m_ayFenPriceData.GetAt(i);
		double dValue = (double)(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume) * 100 / (double)(m_lTotalVol);
		CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_SHOW_PERSENT);
		CreateCell(strText, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, ITEM_TEXT_LEFT, CELL_TIP, 0, nCellIndex++);
	}

	delete[] buf;
	// 重新设行数
	InitSize(nRowCount);
}

long CDrawFenPrice::FindMaxVolume()
{
	CFenPriceData* pPriceData = NULL;
	long lMaxVolume(0);
	for (int i = 0; i < m_ayFenPriceData.GetCount(); i++)
	{
		pPriceData = m_ayFenPriceData.GetAt(i);
		if (pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume > lMaxVolume)
		{
			lMaxVolume = pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume;
		}
	}
	return lMaxVolume;
}

CString CDrawFenPrice::GetTipText( int nCellID, int nItemID )
{
	int nRow = nCellID % GetRowCount();
	nRow = nRow - GetFixRowCount();

	if (nRow >= m_ayFenPriceData.GetCount() || nRow < 0)
		return "";

	CFenPriceData* pPriceData = m_ayFenPriceData.GetAt(nRow);
	char strText[64];
	CString str;
	// 标题
	CString strTitle;
	strTitle.Format("分价表%d", nRow+1);

	// 第一行 成交价
	CGeneralHelper::LongToString(pPriceData->m_lPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
	str.Format("成交价:%s \r\n", strText);

	// 第二行 成交量
	CGeneralHelper::LongToString(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume, 0, m_stkInfo.m_nHandNum, 0, strText);
	str.AppendFormat("成交量:%s \r\n", strText);

	// 第三行 比例
	double dValue = (double)(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume) * 100 / (double)(m_lTotalVol);
	CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_SHOW_PERSENT);
	str.AppendFormat("  比例:%s \r\n", strText);

	// 第四行 笔数
	str.AppendFormat("  成交: %d", pPriceData->m_lTurnover);

	return str;
}