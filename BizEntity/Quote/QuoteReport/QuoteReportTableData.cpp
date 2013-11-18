#include "stdafx.h"
#include "quotereporttable.h"
#include "hsothercoldefine.h"
#include "gridctrl\memdc.h"
#include "WinnerApplication.h"
#include "quotereport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���������
void CQuoteReportTable::InitNumber(int nBeginRow /* = -1 */, int nEndRow /* = -1 */)
{
	int nNumberCol = GetColIndexByID(COLUMN_HQ_BASE_NUMBER);
	if ( nNumberCol == -1 ) return;

	if (nEndRow < 0 || nEndRow < nBeginRow)
	{
		nEndRow = GetRowCount() - 1;
	}

	CString strText;
	CGridCellBase* pCell;

	for( int nRow = max(nBeginRow,GetFixedRowCount()); nRow <= min(nEndRow,GetRowCount()-1); nRow++)
	{
		pCell = GetCell(nRow, nNumberCol);
		if ( pCell != NULL )
		{
			strText.Format("%i",nRow);
			pCell->SetText(strText);
		}
	}
}

// �趨�̶�����
void CQuoteReportTable::SetFixedData(int nRow,int nCol,CGridCellBase* pCell, StockUserInfo* pStock /* = NULL */,
									 int nNumber /* = -1 */, BOOL bRefresh /* = TRUE */, CDC* pDC /* = NULL */)
{
	UINT nID = GetColHeadID(nCol);
	if (nID <= 0)
	{
		return;
	}

	CString strText; 
	//�������� �ɱ�����
	if( ( nID >= COLUMN_CAPITALIZATION_TOTAL && nID <= COLUMN_CAPITALIZATION_A2_GIVE ) 
		||	( nID >= COLUMN_FINANCE_TOTAL_ASSETS && nID <= COLUMN_FINANCE_ASSETS_YIELD ) )
	{
		//		pCell->Empty();

		//if( m_pYlsSort.IsValid() && pCol->m_dID == m_pYlsSort.m_nColumn )
		//{
		//	strText.Format("%.3f",m_pYlsSort.GetData(nNumber - m_pYlsSort.m_nFixedRow,pStock) );
		//	if(strText[0] == '-')
		//	{
		//		strText.Insert(0, ' ');
		//	}
		//	pCell->szText = strText;
		//}
		//else if( pStock )
		//{
		//	FinanceData* pFince = CHSDisk::GetFinaceData(&pStock->m_ciStockCode);
		//	if (pFince != NULL)
		//	{
		//		CHSFinanceData YlsFinaceData;
		//		memcpy(&YlsFinaceData,pFince,sizeof(FinanceData));

		//		HSDouble hxData = YlsFinaceData.GetDataByID(pCol->m_dID);
		//		if( (long)hxData == hx_DefMaxValues )
		//		{
		//			hxData = 0;
		//		}
		//		strText.Format("%.3f",hxData);

		//		if(strText[0] == '-')
		//		{
		//			strText.Insert(0, ' ');
		//		}
		//		pCell->szText = strText;//hxSubZero(strText);
		//	}
		//}
		//else
		//{
		//	bRefresh = FALSE;
		//}
	}
	else if (nID >= COLUMN_BLOCK_BEGIN && nID <= COLUMN_BLOCK_END)		//add by lxqi 20090706	�����Ϣ
	{
		//		pCell->Empty();

		//if( pStock )
		//{
		//	BlockInfoEx* pBlockInfo = NULL;
		//	if (g_hxData.m_mapBlockInfoEx.Lookup(m_strColumnEntry,pBlockInfo))
		//	{
		//	}
		//	else
		//	{
		//		pBlockInfo = new BlockInfoEx;
		//		pBlockInfo->m_pStock = pStock;
		//		g_hxData.GetBlockMember(m_strColumnEntry,NULL,NULL,&pBlockInfo->m_pMapBlockMember);
		//		g_hxData.m_mapBlockInfoEx.SetAt(m_strColumnEntry,pBlockInfo);
		//	}

		//	BlockMemberInfo* pMemberInfo = NULL;
		//	if (pBlockInfo->m_pMapBlockMember.Lookup(pStock->GetCode(),(void*&)pMemberInfo))
		//	{
		//		if (pMemberInfo != NULL && !pMemberInfo->m_mapUserItem.Lookup(pCol->m_dID,strText))
		//		{
		//			strText = "";
		//		}
		//	}
		//	pCell->szText = strText;
		//}
		//else
		//{
		//	bRefresh = FALSE;
		//}
	}
	else
	{
		switch(nID)
		{
		case COLUMN_HQ_BASE_INFO_MARK:
			if( pStock )
			{
				pCell->SetText(pStock->m_ciStockCode.GetCode());

				//// ������ױ��
				//void* pData;
				//if( m_mapCodeMark.Lookup(pCell->szText,pData) )
				//{
				//	pCell->szText = "*";
				//}
				//else
				//{
				//	pCell->szText = "";
				//}
			}
			else
			{
				pCell->SetText("");
			}
			break;
		case COLUMN_HQ_BASE_CODE: // ��Ʊ����
			if( pStock )
			{				
				pCell->SetText(pStock->GetCode());

				pCell->SetData((LPARAM)pStock);
			}
			break;
		case COLUMN_HQ_BASE_NAME: 		// ��Ʊ����
			if( pStock )
			{
				pCell->SetText(pStock->GetName());
				pCell->SetData((LPARAM)pStock);
			}
			break;
		case COLUMN_HQ_BASE_NUMBER:   	// ���
			{
				if( nNumber == -1 )
				{
					CString strTemp;
					strTemp.Format("%i",nRow - GetFixedRowCount() + 1);
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
		case COLUMN_HQ_BASE_PRECLOSE:   // ����
			{
				if( pStock == NULL )
					break;

				CString strTemp;
				strTemp.Format("%.2f",(float)(pStock->m_lPrevClose)/pStock->m_nPriceUnit);
				pCell->SetText(strTemp);
			}
			break;
		case COLUMN_HQ_BASE_ARROW:
			{
				pCell->SetText("!");
			}
			break;
		default:
			{
				pCell->SetText("-");
			}
			break;
		}
	}
	if( m_idCurrentCell.row == nRow )
	{
		pCell->SetState(pCell->GetState() | GVIS_SELECTED | GVIS_LIGHT);
		m_SelectedCellMap.SetAt(MAKELONG(nRow,nCol),CCellID(nRow,nCol));
	}
//	pCell->SetTextClr(m_iColProp->GetColumnColor(GetColHeadID(nCol)));
	DWORD dw = pCell->GetState()|m_iColProp->GetColumnMask(GetColHeadID(nCol));
// 	pCell->SetFormat(m_iColProp->GetColumnFormat(GetColHeadID(nCol)));
// 	pCell->SetFormat(m_iColProp->GetColumnFormat(nID));
	pCell->SetState(dw);
	pCell->SetSelectColor(m_iColorProt->GetQuoteHighLightBKColor());

	// ���⴦��
	if (GetColHeadID(nCol) == COLUMN_HQ_BASE_NUMBER)		// ���á���������
	{
		CFont* pFont = (CFont*)m_iFontProt->GetQuoteFixedFont();
		if (pFont)
		{
			LOGFONT font;
			ZeroMemory(&font,sizeof(LOGFONT));
			pFont->GetLogFont(&font);
			pCell->SetFont(&font);
		}
	}
	else if (GetColHeadID(nCol) == COLUMN_HQ_BASE_CODE)	// ���á����롱������
	{
		CFont* pFont = (CFont*)m_iFontProt->GetQuoteCodeFont();
		if (pFont)
		{
			LOGFONT font;
			ZeroMemory(&font,sizeof(LOGFONT));
			pFont->GetLogFont(&font);
			pCell->SetFont(&font);
		}
	} 
	else if (dw & GVIS_STRING)		// �����ַ���������
	{
		if (GetColHeadID(nCol) == COLUMN_HQ_BASE_NAME)
		{
			CString strKey;
			strKey.Format("%04x%s",pStock->m_ciStockCode.m_cCodeType,pStock->GetCode());
			const HsCodeMark* pMark = m_iUserDefCfg->SearchMark(strKey);
			if (pMark)
			{
				if (pMark->m_nType >= 0)
				{
					pCell->SetImageEx(pMark->m_nType);
				}
				else
				{
					pCell->SetImageEx(20);
				}
			}
			else
			{
				pCell->SetImageEx(-1);
			}
		}
		CFont* pFont = (CFont*)m_iFontProt->GetQuoteStringFont();
		if (pFont)
		{
			LOGFONT font;
			ZeroMemory(&font,sizeof(LOGFONT));
			pFont->GetLogFont(&font);
			pCell->SetFont(&font);
		}
	} 


	if( bRefresh )
	{
		if (GetDoubleBuffering())
		{
			CMemDC memDC(pDC);
			RedrawCell( nRow, nCol, &memDC );
		} 
		else
		{
			RedrawCell( nRow, nCol, pDC );
		}
	}
}

//////////////////////////////////////////////////////////////////////////Ӧ�����ݴ���
void CQuoteReportTable::HandleNowData(DataHead* pHead,const CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	long nUnit = 0;
	short nDecimal = 0;
	long  lClose = 0;
	StockUserInfo* pStock = NULL;
	int nPos = 0;

	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}

		pStock = FindRowByCode( &pData->m_ciStockCode,nPos );
		if( pStock == NULL || 
			pStock->m_ciStockCode.m_cCodeType == 0 ||
			(pStock->m_ciStockCode).m_cCode[0] == '\0' )
		{
			pData = pNextData;
			continue;
		}

		lClose   = pStock->m_lPrevClose;
		nUnit    = (long)pStock->m_nPriceUnit;
		nDecimal = pStock->m_nDecimal;

		UpdateDataEntry(nPos,pStock,nUnit,nDecimal,lClose,
			(HSStockRealTime_Ext*)pData->m_cNowData,
			&pData->m_othData);

		pData = pNextData;		
	}

	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
	this->Invalidate(FALSE);
}



void CQuoteReportTable::HandleAutoPushData(DataHead* pHead, const CommRealTimeData_Ext* pnowData, int nSize)
{
	if( pHead == NULL || nSize <= 0 )
		return;

	long nUnit = 0;
	short nDecimal = 0;
	long  lClose = 0;

	StockUserInfo* pStock = NULL;
	int nPos = 0;

	CommRealTimeData_Ext* pRealTimeData = (CommRealTimeData_Ext *)pnowData;
	CommRealTimeData_Ext* pNext = NULL;
	int nNowSize = 0;

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

		if( !bDeal ) bDeal = TRUE;

		lClose   = pStock->m_lPrevClose;
		nUnit    = (long)pStock->m_nPriceUnit;
		nDecimal = pStock->m_nDecimal;

		UpdateDataEntry(nPos,pStock,nUnit,nDecimal,lClose,
			&pRealTimeData->GetShareData()->m_nowDataExt,
			(StockOtherData *)&pRealTimeData->m_othData,
			GetRefreshDispTextProp(),
			MAKELONG(GRID_DATA_UPDATE_DEFAULT,LTOA_MASK_DEFAULT));

		pRealTimeData = pNext;
	}

	if( !bDeal )
		return;

	if (m_nRefreshTimerID == 0)
	{
		m_nRefreshTimerID = SetTimer((long)this,REFRESH_DISAPPEAR_TIMESPAN,CQuoteReportTable::TimerProc);
	}

	//// ����
	//if( this->IsSortStatus() )
	//{
	//	CCellID cell = GetCurrentCell();

	//	CDC* pDC = GetDC();
	//	HighlightSelectRow(NULL,cell.row,cell.row,FALSE);
	//	SelectPrice(m_LastMouseJYPoint,1);
	//	//qinhn 20091119  Add �����ID���ڻ������̵Ļ����յ���������ʱ�������е�������� ��ʱ�����̸������Ƚϴ�С�������ţ���ʾʱֻ��ʾ���ֲ���ʾ����
	//	//�����������������������ڽ������ţ����������ף������ж����⴦��
	//	int nColumnID = -1;
	//	hxTableColume*  pCol= (hxTableColume*)GetItemData(0, m_SortColumn);
	//	if (pCol && IsWindowVisible())
	//		nColumnID = pCol->m_dID;
	//	if (COLUMN_FUTURES_PRECLOSE != nColumnID) 
	//		SortTextItems(m_SortColumn, m_bAscending);
	//	//qinhn 20091119  Add End
	//	SelectPrice(m_LastMouseJYPoint,0);
	//	InitNumber();	

	//	//for(int i = 0; i < m_nFixedCols; i++)
	//	//	RedrawColumn(i);

	//	//CDC* pDC = GetDC();
	//	HighlightSelectRow(NULL,cell.row,cell.row);

	//	//OnDraw(pDC);
	//	ReleaseDC(pDC);

	//	//ghm 20080606 ǿ��ˢ�½���
	//	//������״̬�£����ݸ�������󣬸�����ʾ����ԭ�����д�����ֶ���Ϣ����������ú���ֶ�����
	//	Invalidate();
	//}

	//if(	IsLargeFont() )
	//{
	//	g_hxData.m_sYlsRefreshDispText.m_pFont = &g_hxData.ftHightText;
	//}



	//if( m_nRequestTimerID == 0 )
	//{
	//	if( this->IsSortStatus() )
	//		m_nRequestTimerID = SetTimer((long)this,2000,NULL);
	//	else
	//		m_nRequestTimerID = SetTimer((long)this,2000,NULL);
	//}
	////ghm 20080603 �����ʱ���ѿ�����ûˢ�£�����ǿ��ˢ��
	//else
	//{
	//	m_nRequestTimerID = 0;
	//	Invalidate();
	//}

}

void CQuoteReportTable::HandleSortData(DataHead* pHead,const AnsReportData* pReportData)
{
	if(pReportData == NULL || IsBadReadPtr(pReportData,1) ||pReportData->m_nSize <= 0)
	{
		CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-�յ��������������								<===");
		return;
	}

	CommRealTimeData_Ext* pData = (CommRealTimeData_Ext*)pReportData->m_prptData;
	int nSize = pReportData->m_nSize;	

	StockUserInfo* pStockInfo = NULL;
	static char  strText[64];
	long nUnit = 0;
	short nDecimal = 0;
	long  lClose = 0;
	CCellRange cellRange = GetVisibleNonFixedCellRange();
	short row = cellRange.GetMinRow();
	short col = 0;
	CGridCellBase* pCell = NULL;

	CommRealTimeData_Ext* pRealTimeData = pData;
	CommRealTimeData_Ext* pNext = NULL;
	int nNowSize = 0;

	for(int i=row; i < row + nSize; i++)
	{
		if( i >= GetRowCount() )
			break;

		pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);

		pStockInfo = GetStock(i - 1);
		if (!m_pDataSource->HSDataSourceEx_GetStockUserInfo(&pRealTimeData->m_ciStockCode,*pStockInfo))
		{
			pRealTimeData = pNext;
			continue;
		}
#ifdef _DEBUG
		CString strTest;
		strTest.Format("���鱨��-�յ��������ݣ����������%d pRealTimeData->m_ciStockCode.GetCode()=%s,&pRealTimeData=%x, Market=%x,Code=%s,Name=%s\n",
			i,pRealTimeData->m_ciStockCode.GetCode(),(long)pRealTimeData,
			pStockInfo->m_ciStockCode.m_cCodeType,pStockInfo->m_ciStockCode.GetCode(),pStockInfo->GetName());
		OutputDebugString(strTest);
#endif
		if(pStockInfo == NULL)
		{
			pRealTimeData = pNext;
			continue;
		}

		lClose   = pStockInfo->m_lPrevClose;
		nUnit    = (long)pStockInfo->m_nPriceUnit;
		nDecimal = pStockInfo->m_nDecimal;

		for ( col = 0; col < cellRange.GetMinCol(); col++)
		{
			pCell = GetCell(i, col);
			if( pCell == NULL )
				continue;

			SetFixedData(i,col,pCell,(StockUserInfo*)pStockInfo,-1,FALSE);
		}

		UpdateDataEntry(i,pStockInfo,
			nUnit,nDecimal,lClose,
			&pRealTimeData->GetShareData()->m_nowDataExt,
			&pRealTimeData->m_othData);

		pRealTimeData = pNext;
	}
	Invalidate(FALSE);

	if (m_nRefreshTimerID != 0)
	{
		KillTimer(m_nRefreshTimerID);
		m_nRefreshTimerID = 0;
		m_mapRefreshCells.RemoveAll();
	}
	// for test
	if (m_nDelaySortRequestTimerID != 0)
	{
		KillTimer(m_nDelaySortRequestTimerID);
		m_nDelaySortRequestTimerID = 0;
	}
	if (m_nDelayRealTimeRequestTimerID != 0)
	{
		KillTimer(m_nDelayRealTimeRequestTimerID);
		m_nDelayRealTimeRequestTimerID = 0;
	}
	m_nDelaySortRequestTimerID = SetTimer((long)this+2,3000 + DELAY_REQUEST_TIMESPAN,DelaySortRequestTimerProc);
}
//////////////////////////////////////////////////////////////////////////�����г�����
//���ݸ������
void CQuoteReportTable::UpdateDataEntry(int nPos, StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, HSStockRealTime_Ext* pData, StockOtherData* pOtherData, RefreshDispTextProp* pRefreshDispTextProp /* = NULL */, int nType /* = 0 */)
{
	if( MakeMarket(pStock->m_ciStockCode.m_cCodeType) == QH_Bourse ||
		WP_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) )		// �ڻ�
	{
		// 			UpdateFuturesData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSQHRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( MakeIndexMarket(pStock->m_ciStockCode.m_cCodeType) )			// ָ��
	{
		// 		UpdateIndexData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( (HSMarketDataType)FOREIGN_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) ) // ���
	{
		// 		UpdateForeignData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else if( HK_MARKET == MakeMarket(pStock->m_ciStockCode.m_cCodeType) )	// �۹�
	{
		// 		UpdateHKData(nPos, pStock,nUnit,nDecimal,lClose,
		// 			(HSStockRealTime*)pData,pOtherData,pRefreshDispTextProp, nType);
	}
	else																	// ��Ʊ
	{
		UpdateStockData(nPos, pStock,nUnit,nDecimal,lClose,
			pData,pOtherData,pRefreshDispTextProp, nType);
	}

}

//////////////////////////////////////////////////////////////////////////���¹�Ʊ����
void CQuoteReportTable::UpdateStockData(int nPos, StockUserInfo* pStock, long nUnit, short nDecimal, long lClose, 
										HSStockRealTime_Ext* pData, 
										StockOtherData* pOtherData, 
										RefreshDispTextProp* pRefreshDispTextProp /* = NULL */, 
										int nType /* = 0 */)
{
	int nCol = -1;
	int nRes = 0;
	BOOL isStop = TRUE;//�Ƿ�ͣ��
	if (pData->m_stockOther.m_lStopFlag == 0)//��ͣ��
	{
		isStop = FALSE;
	}
	// �۸�����¼�ͷ
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_ARROW)) != -1 )
	{
		if( pData->m_stockRealTime.m_lNewPrice > lClose )
		{
			if ( UpdateCellText(nPos, nCol, "��", nType, pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else if( pData->m_stockRealTime.m_lNewPrice < lClose )
		{
			if ( UpdateCellText(nPos, nCol, "��", nType, pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else
		{
			if ( UpdateCellText(nPos, nCol, " ", nType, pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
	}

	// ������
	if( (nCol=GetColIndexByID(COLUMN_HQ_BASE_INVERSE_PRICE)) != -1 )
	{
		if( pData->m_stockRealTime.m_lNewPrice == 0 )
		{
			if ( UpdateCellText(nPos,nCol,(long)0,lClose, nUnit, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else
		{
			if ( UpdateCellText(nPos,nCol,(long)(1.0*nUnit*nUnit)/pData->m_stockRealTime.m_lNewPrice, 
				lClose, nUnit, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
	}

	// ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OPEN)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lOpen, 
			lClose, nUnit, nDecimal, nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// �ɽ�
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_NEW_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lNewPrice, 
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// �ǵ�
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_VALUE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,
			pData->m_stockRealTime.m_lNewPrice == 0?0:(pData->m_stockRealTime.m_lNewPrice - lClose),
			0, nUnit, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_TOTAL_HAND)) != -1 )
	{

		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lTotal,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ����
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_HAND)) != -1 )
	{

		if (UpdateCellText(nPos,nCol,pOtherData->m_lCurrent, 0, 
			(!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ���
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MAX_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lMaxPrice, 
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ���
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_MIN_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lMinPrice, 
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_BUY_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice1,
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_SELL_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice1, 
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// �Ƿ�
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_RISE_RATIO)) != -1 )
	{
		if( lClose == 0 )
		{
			if (UpdateCellText(nPos,nCol,
				(long)0, 0, 0, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else
		{
			if (UpdateCellText(nPos,nCol,
				pData->m_stockRealTime.m_lNewPrice == 0?0:(pData->m_stockRealTime.m_lNewPrice - lClose)*100, 
				0, lClose, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
	}
	//����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_SPEEDUP)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockOther.m_fSpeedUp * nUnit, 
			0, nUnit, nDecimal,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_PRECLOSE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pStock->m_lPrevClose, 
			lClose, nUnit, nDecimal,nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ���� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_VOLUME_RATIO)) != -1 )
	{
		if( pStock->m_l5DayVol > 0 )
		{
			CArray<StockType> ayStockType;
			long nCount = m_pDataSource->HSDataSourceEx_GetSystemMarketInfo((WORD*)&pStock->m_ciStockCode.m_cCodeType,1,ayStockType);
			if (nCount > 0)
			{
				short nTotalTime = ayStockType.GetAt(0).m_nTotalTime;
				double dWBTemp = (double)((double long)pData->m_stockRealTime.m_lTotal * ayStockType.GetAt(0).m_nTotalTime / 
					((double long)pStock->m_l5DayVol * (pOtherData->m_nTime+1)));
				long lBi;
				if( dWBTemp > 0 )
				{
					lBi = (long)(dWBTemp+0.5);
				}
				else
				{
					lBi = (long)(dWBTemp-0.5);
				}

				if (UpdateCellText(nPos,nCol,lBi, 
					0, 100, 2,isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
				{
					m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
				}

			}
		}
	}

	// ί����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_VOLUME)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount1,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ί���
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_BUY_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice1,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ί����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_PRICE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice1,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ί����
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_SELL_VOLUME)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount1,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ���� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_IN_HANDS)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pOtherData->m_lInside,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ���� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_OUT_HANDS)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pOtherData->m_lOutside,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// �ܶ� 
	if(  (nCol = GetColIndexByID(COLUMN_HQ_BASE_MONEY)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_fAvgPrice,
			0, 10000, 2,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),
			pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ���� 
	//ReplaceListData(nPos,COLUMN_HQ_BASE_RISE_SPEED,pData->m_fAvgPrice,
	//	0, 10000, 2,
	//	nType,pRefreshDispTextProp);

	// ���� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_AVERAGE_PRICE)) != -1 )
	{
		long lAvgPrice = 0;
		HSMarketDataType cCodeType = pStock->m_ciStockCode.m_cCodeType;			//add by lxqi 20090304	ծȯ�Ĵ����в�ͬ
		long nHand	     = pData->m_stockRealTime.m_nHand;
		if(nHand <= 0) 
			nHand = 100;	

		if(pData->m_stockRealTime.m_lTotal != 0)
		{
			lAvgPrice = (long)((double long)pData->m_stockRealTime.m_fAvgPrice * nUnit / ((double long)pData->m_stockRealTime.m_lTotal  * 100 / nHand));
			// add by lxqi 20090304 ծȯ
			if( (MakeMarket(cCodeType) == STOCK_MARKET) && 
				(MakeSubMarket(cCodeType) == KIND_BOND) )
			{
				if (MakeMainMarket(cCodeType) == SH_Bourse)		//add by lxqi 20090304	�����г���������ʵ������
				{
					lAvgPrice *= 10;
				} 
				else
				{
					lAvgPrice *= 100;
				}
			}

		}
		if (UpdateCellText(nPos,nCol,lAvgPrice,
			lClose, nUnit, nDecimal,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ���
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_RANGE)) != -1 )
	{
		if( pStock->m_lPrevClose <= 0 )
		{
			if (UpdateCellText(nPos,nCol,(long)0,
				0, 0, 2,
				isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else
		{
			if (UpdateCellText(nPos,nCol,(pData->m_stockRealTime.m_lMaxPrice - pData->m_stockRealTime.m_lMinPrice) * 100,
				0, pStock->m_lPrevClose, 2,
				isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
	}

	long BuyCount  =  pData->m_stockRealTime.m_lBuyCount1 + pData->m_stockRealTime.m_lBuyCount2 
		+ pData->m_stockRealTime.m_lBuyCount3 + pData->m_stockRealTime.m_lBuyCount4 + pData->m_stockRealTime.m_lBuyCount5;
	long SellCount =  pData->m_stockRealTime.m_lSellCount1 + pData->m_stockRealTime.m_lSellCount2 
		+ pData->m_stockRealTime.m_lSellCount3 + pData->m_stockRealTime.m_lSellCount4 + pData->m_stockRealTime.m_lSellCount5;
	long A = BuyCount - SellCount;

	// ί��		
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_RATIO)) != -1 )
	{
		long B = BuyCount + SellCount;

		long lBi = 0;
		if((B != 0) && (A != 0))
		{
			double dWBTemp = 10000.0 * (double)A/(double)B;
			if( dWBTemp > 0 )
			{
				lBi = (long)(dWBTemp+0.5);
			}
			else
			{
				lBi = (long)(dWBTemp-0.5);
			}
		}
		if (UpdateCellText(nPos,nCol,lBi,
			0, 100, 2,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ί��
	if( (nCol = GetColIndexByID(COLUMN_HQ_BASE_ORDER_DIFF)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,A,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ����۸�һ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE1)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice1,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ��������һ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME1)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount1,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����۸��
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE2)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice2,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME2)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount2,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE3)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice3,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME3)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount3,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE4)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice4,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME4)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount4,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_PRICE5)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyPrice5,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BUY_VOLUME5)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lBuyCount5,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// �����۸�һ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE1)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice1,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ��������һ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME1)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount1,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// �����۸��
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE2)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice2,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME2)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount2,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// �����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE3)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice3,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME3)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount3,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// �����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE4)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice4,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME4)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount4,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// �����۸���
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_PRICE5)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellPrice5,
			lClose, nUnit, nDecimal,
			nType,pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ����������
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_SELL_VOLUME5)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lSellCount5,
			0, (!pData->m_stockRealTime.m_nHand)?100:pData->m_stockRealTime.m_nHand, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ����ֵ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_FUND_NETVALUE)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lNationalDebtRatio,
			0, 100000, nDecimal, isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	// ��ծ��Ϣ
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_BOND_ACCRUAL)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pData->m_stockRealTime.m_lNationalDebtRatio,
			0, 1000, nDecimal, isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}

	// ������:ĳһ��ʱ���ڵĳɽ���/�����ܹ�����100�� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_EXHAND_RATIO)) != -1 )
	{
		double nCapital = 0;
		//for test
		//CHSFinanceData* pFinaceData = (CHSFinanceData*)CStockManager::GetFinaceData(&pStock->m_ciStockCode);
		//if( pFinaceData )
		//{
		//	HSDouble dRet = pFinaceData->CAPITAL(&pStock->m_ciStockCode);
		//	if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
		//	{
		//		nCapital = dRet;
		//	}
		//	else
		//	{
		//		nCapital = 0;
		//	}

		//	if( nCapital > 10000 )
		//	{
		//		nCapital = pData->m_stockRealTime.m_lTotal/(nCapital/10000);
		//	}
		//	else if( nCapital != 0 )
		//	{
		//		nCapital = pData->m_stockRealTime.m_lTotal/nCapital;
		//	}
		//}
		if (UpdateCellText(nPos,nCol,(long)nCapital,
			0, 10000, 2,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO|LTOA_MASK_SHOW_PERSENT)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	//5��ƽ����
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_5DAY_AVGVOLUME)) != -1 )
	{
		if (UpdateCellText(nPos,nCol,pStock->m_l5DayVol,
			0, 1, 0,
			isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
		{
			m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
		}
	}
	//��ӯ�� 
	if( (nCol = GetColIndexByID(COLUMN_HQ_EX_PE_RATIO)) != -1 )
	{
		double nShiYing = 0;
		double nQuarter = 4; //20090605 YJT �޸���ӯ���㷨

		// for test
		//CHSFinanceData* pFinaceData = (CHSFinanceData*)CStockManager::GetFinaceData(&pStock->m_ciStockCode);
		//if( pFinaceData )
		//{
		//	HSDouble dRet = pFinaceData->GetDataByID(COLUMN_FINANCE_PER_INCOME);
		//	if ( dRet.m_dData != hx_DefMaxValues && dRet.m_dData != 0 )
		//	{
		//		nShiYing = dRet;
		//	}
		//	else
		//	{
		//		nShiYing = 0;
		//	}
		//	nShiYing *= nUnit;

		//	nQuarter = pFinaceData->GetDataByID(0); //20090605 YJT �޸���ӯ���㷨
		//}

		////20090605 YJT �޸���ӯ���㷨
		//if (abs(nQuarter - 1) < 0.0001)
		//	nQuarter = 0.25;
		//else if (abs(nQuarter - 2) < 0.0001)
		//	nQuarter = 0.5;
		//else if (abs(nQuarter - 3) < 0.0001)
		//	nQuarter = 0.75;
		//else
		//	nQuarter = 1;

		if( nShiYing > 0 && pData->m_stockRealTime.m_lNewPrice != 0 )
		{
			if (UpdateCellText(nPos,nCol,(long)((pData->m_stockRealTime.m_lNewPrice/nShiYing)*nUnit*nQuarter), //20090605 YJT �޸���ӯ���㷨
				0, nUnit, 2,
				isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
		else
		{
			if (UpdateCellText(nPos,nCol,(long)0,
				0, 1, 2,
				isStop?nType:(nType|MAKELONG(0,LTOA_MASK_FIXEDLEN_IFZERO)),pRefreshDispTextProp) == 2)
			{
				m_mapRefreshCells.SetAt(MAKELONG(nPos,nCol),REFRESH_DISAPPEAR_TIMES);
			}
		}
	}

}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CQuoteReportTable ��Ϣ�������
BOOL CQuoteReportTable::OnRecvData(void* pData, int nLen)
{
	if (!pData || IsBadReadPtr(pData,1))
	{
		CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-�յ����������									<===");
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;

	// ��Ʊ�б���ϢӦ��
	if (pHead->m_nType == RT_INITIALINFO)			// �����ʼ��
	{
		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-�յ������ʼ����Ϣ����ʼ��ʼ�����鱨�۱�													<===");
		RequestStockUserInfo();
	} 
	else if (pHead->m_nType == RT_STOCKINFOLIST)	// ��Ʊ������Ϣ
	{
		AnsStockInfoList* pStockList = (AnsStockInfoList*)pData;
		if (pStockList == NULL || IsBadReadPtr(pStockList,1))
		{
			CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
				"���鱨���б�-�յ������Ʊ������Ϣ������									<===");
			return FALSE;
		}
		PageViewInfo pageinfo;
		memcpy(&pageinfo,&m_pviInfo,sizeof(PageViewInfo));
		pageinfo.EmptyMarketType();
		HSMarketDataType m_cPreCodeType=0;
		for (int i=0; i<pStockList->m_nSize; i++)		//���г�֧��
		{
			int nType = pStockList->m_pnowData[i].m_ciStockCode.m_cCodeType;
			if (m_cPreCodeType != nType)
			{
				pageinfo.AddMarketType(nType);
			}
			m_cPreCodeType = nType;
		}
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"���鱨���б�-�յ������б����ݣ����ܺţ�0x%04X�����������%d����һ���г����ͣ�%x����ʼ���´���===>",
			pHead->m_nType,pStockList->m_nSize,pStockList->m_pnowData[0].m_ciStockCode.m_cCodeType);
		ChangeContent(&pageinfo, pStockList->m_pnowData, pStockList->m_nSize,m_strCurColGroup,TRUE);

		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-�����б����ݸ������													<===");
	}
	// ʵʱ����Ӧ��
	else if (pHead->m_nType == RT_REALTIME_EXT)
	{
		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
		if(pNowData->m_nSize <= 0)
		{
			CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
				"���鱨���б�-ʵʱ���ݴ������Ϊ0									<===");
			return FALSE;
		}

		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"���鱨���б�-�յ�ʵʱ���ݣ����ܺţ�0x%04X�����������%d����ʼ���´���===>",
			pHead->m_nType,pNowData->m_nSize);

		HandleNowData( pHead,(CommRealTimeData_Ext*)pNowData->m_pnowData, pNowData->m_nSize );

		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-ʵʱ���ݸ������									  <===");
	}
	// ��������
	else if (pHead->m_nType == RT_AUTOPUSH_EXT)
	{
		AnsHSAutoPushData* pEveryData = (AnsHSAutoPushData*)pData;
		if(pEveryData->m_nSize <= 0)
		{
			CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
				"���鱨���б�-�յ��������ư����ݣ��������Ϊ0									<===");
			return FALSE;
		}
		CommRealTimeData_Ext *pData = (CommRealTimeData_Ext *)pEveryData->m_pstData;

		//CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		//	"���鱨���б�-�յ��������ݣ����������%d����ʼ���´���===>",
		//	pEveryData->m_nSize);

		HandleAutoPushData(pHead,pData,pEveryData->m_nSize);

		//CWinnerApplication::SimpleLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		//	"���鱨���б�-�������ݸ������						  <===");
	}
	else if (pHead->m_nType == RT_REPORTSORT_EXT)			// ����������Ϣ
	{
		AnsReportData* pSortData = (AnsReportData*)pData;
		if(pSortData->m_nSize <= 0)
		{
			CWinnerApplication::SimpleLog(ERROR_LOG_LEVEL, QuoteReportLoggerName,
				"���鱨���б�-�յ����󱨼���������ݣ��������Ϊ0									<===");
			return FALSE;
		}

		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
			"���鱨���б�-�յ�����Ӧ�����ݣ����ܺţ�0x%04X�����������%d����ʼ���´���===>",
			pHead->m_nType,pSortData->m_nSize);
		
		HandleSortData(pHead,pSortData);

		CWinnerApplication::SimpleLog(INFO_LOG_LEVEL, QuoteReportLoggerName,
			"���鱨���б�-��������Ӧ�������									  <===");
	}
	return TRUE;
}
