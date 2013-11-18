#include "stdafx.h"
#include "quotereporttable.h"
#include "hscoldefine.h"
#include "hsothercoldefine.h"
#include "quotestuctdefine.h"
#include "..\QuoteComm\pageid.h"
#include "WinnerApplication.h"
#include "QuoteReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CQuoteReportTable::CancelSort()
{
	if (GetSortColumn() >= 0)
	{
		KillAllTimer();
		SetSortColumn(-1);
		SetSortAscending(FALSE);
		RequestStockUserInfo();

		return 0;
	}

	return -1;
}

void CQuoteReportTable::SetSortColumn(int nCol)
{
	CHSGridTable::SetSortColumn(nCol);

	if (nCol >= 0)
	{
		int nColID = GetColHeadID(nCol);
		if (!IsSupportSort(nCol)
			|| nColID == COLUMN_HQ_BASE_NAME 
			|| nColID == COLUMN_HQ_BASE_CODE)
		{
			m_bNeedRequestSort = FALSE;
		} 
		else
		{
			m_bNeedRequestSort = TRUE;
		}
		CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,"行情报价-设置排序列，列序号：%d，是否向服务器请求排序数据：%d",
			nCol,m_bNeedRequestSort);
	}
	else
	{
		m_bNeedRequestSort = FALSE;
	}
}

BOOL CQuoteReportTable::GetSortInfoByPageInfo(const PageViewInfo* pvInfo,short nID, int& nCount,HSMarketDataType& cCodeType)
{
	if( pvInfo == NULL )
		return FALSE;

	nCount    = 0;
	cCodeType = -1;

	switch(pvInfo->m_nInPageTag & 0x0F00)
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
	case PT_BLOCK_INDEX:		//add by lxqi 20090702	板块指数
		{
			nCount    = m_pStock.GetCount();
			cCodeType = USERDEF_BOURSE;
		}
		break;
	default:
		return FALSE;
	}

	return (cCodeType != -1);

}

BOOL CQuoteReportTable::GetDefaultSortType(int nCol)
{
	return GetDefaultSortType((WORD)GetColHeadID(nCol));
}

BOOL CQuoteReportTable::GetDefaultSortType(WORD nColID)
{
	return FALSE;
}

BOOL CQuoteReportTable::IsSupportSort(int nCol)
{
	return IsSupportSort((WORD)GetColHeadID(nCol));
}

BOOL CQuoteReportTable::IsSupportSort(WORD nColID)
{
	return (nColID != COLUMN_HQ_BASE_INFO_MARK
		&& !(nColID >= COLUMN_BLOCK_BEGIN && nColID <= COLUMN_BLOCK_END)
		&& nColID != COLUMN_HQ_BASE_NUMBER
		&& nColID != COLUMN_HQ_BASE_ARROW);
}

void  CQuoteReportTable::OnFixedRowClick(CCellID& cell)
{
	WORD nColID = GetColHeadID(cell.col);
	if (IsSupportSort(nColID))
	{
		Sort(nColID,GetSortColumn()==cell.col ? !GetSortAscending() : GetDefaultSortType(nColID));
	} 
	else
	{
		CancelSort();
	}
	return;
}

void CQuoteReportTable::Sort(int nID, BOOL bAscending, BOOL bForce /* = FALSE */)
{
	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-开始排序，排序列ID：%d，是否升序：%d",nID,bAscending);

	int nPreSortCol = GetSortColumn();
	int nCol = GetColIndexByID(nID);
	if (GetSortColumn() == nCol && GetSortAscending() == bAscending)
	{
		return;
	}
	if (!bForce)
	{
		if (nPreSortCol < 0)
		{
			if (GetSortAscending() && nID==COLUMN_HQ_BASE_CODE)
			{
				bAscending = FALSE;
			}
		}
		else
		{
			int nPreSortID = GetColHeadID(nPreSortCol);
			if (!IsFixedColumn(nPreSortID) && IsFixedColumn(nID))
			{
				// 重新初始化代码列表
				RequestStockUserInfo();
			}
		}
	}

	SetSortAscending(bAscending);
	SetSortColumn(nCol);

	HSSortDataStr* pStockKey = NULL;
	StockUserInfo* pStock = NULL;
	int nDataCount = m_pStock.GetCount();

	if (nID == COLUMN_HQ_BASE_NAME
		|| nID == COLUMN_HQ_BASE_CODE)
	{
		pStockKey = new HSSortDataStr[nDataCount];
		if( pStockKey )
		{
			for(int i = 0; i < nDataCount; i++)
			{
				pStock = m_pStock.GetAt(i);
				if( pStock == NULL )
				{
					continue;
				}

				pStockKey[i].m_pKey = pStock;
				if( nID == COLUMN_HQ_BASE_NAME )
				{
					pStockKey[i].Copy(pStock->GetName());
				}
				else
				{
					pStockKey[i].Copy(pStock->m_ciStockCode.GetCode());
					if (bAscending) SetSortColumn(-1);
				}
			}
			m_pStock.RemoveAll();

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
			for(int i = 0; i < nDataCount; i++,pData++)
			{
				m_pStock.Add(pData->m_pKey);
			}
			delete[] pStockKey;
		}

		InitRowData();

		KillAllTimer();
		m_nDelayRealTimeRequestTimerID = SetTimer((long)this+1,DELAY_REQUEST_TIMESPAN,DelayRealTimeRequestTimerProc);
	} 
	else
	{
		KillAllTimer();
		m_nDelaySortRequestTimerID = SetTimer((long)this+2,DELAY_REQUEST_TIMESPAN,DelaySortRequestTimerProc);
	}
	CWinnerApplication::FormatLog(INFO_LOG_LEVEL,QuoteReportLoggerName,
		"行情报价列表-排序结束，开始延时请求数据，排序列ID：%d，是否升序：%d",nID,bAscending);

}
