/*******************************************************
  源程序名称:TradeGridBase.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易GRID基类实现
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "TradeGridBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeGridBase, CHSGridTable)
CTradeGridBase::CTradeGridBase(int nRows /* = 1 */, int nCols /* = 0 */, int nFixedRows /* = 1 */, int nFixedCols /* = 0 */)
:CHSGridTable(nRows,nCols,nFixedRows,nFixedCols)
{
	m_pTradeTable = NULL;
	m_pDataList = NULL;
	SetEditable(FALSE);
	SetListMode();
	EnableSelection();
	SetFixedRowSelection(TRUE);
	SetFixedColumnSelection(FALSE);
	SetHeaderSort();
	SetGridLines();
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
}

CTradeGridBase::~CTradeGridBase()
{
	int iColCount = GetColumnCount();
	for (int i = 0; i < iColCount; i++)
	{
		LPARAM lpTmp = GetItemData(0, i);
		if (lpTmp)
		{
			ITradeColumn* pColItem = (ITradeColumn*)GetItemData(0, i);
			pColItem->Release();
		}		
	}

}

int CTradeGridBase::GetCurrRow()
{
	return GetFocusCell().row;
}

int CTradeGridBase::GetCurrCol()
{
	return GetFocusCell().col;
}

void CTradeGridBase::FillGrid(IDataList* pDataList)
{
	if (m_pTradeTable)
	{
		m_pDataList = pDataList;
		int iRowCount = m_pDataList->GetCount() + 1;
		int iColCount = GetColumnCount();
		DWORD nBegin = GetTickCount();
		DWORD nEnd = 0;
		CString sTmp = "";
		sTmp.Format(_T("FillGrid GridCount：%d, Begin：%d\n"), iRowCount - 1, nBegin);
		TRACE(sTmp);
		BeginUpdateData();
		SetRowCount(iRowCount);
		for (int i = 1; i < iRowCount; i++)
		{
			IDataItem* pRowItem = m_pDataList->GetItems(i - 1);
			SetItemData(i, 0, (LPARAM)pRowItem);
			for (int j = 0; j < iColCount; j++)
			{
				ITradeColumn* pColItem = (ITradeColumn*)GetItemData(0, j);
				CString sColName = pColItem->GetColumnName();
				CString sText = pRowItem->GetStrByName(sColName);
				sText = pColItem->GetDictValue(sText);
				switch (pColItem->GetAlign())
				{
				case alLeft:
					{
						SetItemFormat(i, j, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
						break;
					}
				case alCenter:
					{
						SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
						break;
					}
				case alRight:
					{
						SetItemFormat(i, j, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
						break;
					}					
				}				
				SetItemText(i, j, sText);
			}					
		}
		int nRow = GetCurrRow();
		CCellID CellID; 
		if (-1 == nRow)
		{
			CellID.row = 1;
			CellID.col = 1;
			SetFocusCell(CellID);
			SelectRows(CellID);
		}
		else
			if (nRow >= iRowCount)
			{
				CellID.row = iRowCount - 1;
				CellID.col = 1;
				SetFocusCell(CellID);
				SelectRows(CellID);
			}
		EndUpdateData();

		nEnd = GetTickCount();

		sTmp.Format(_T("ThreadId：%d,FillGrid End：%d, UseTime：%d\n"), GetCurrentThreadId(), nEnd, nEnd - nBegin);
		TRACE(sTmp);

	}
	
}

void CTradeGridBase::FillHeader(ITradeTable* pTradeTable)
{
	if (pTradeTable)
	{
		m_pTradeTable = pTradeTable;
		int iCount = m_pTradeTable->GetCount();
		SetColumnCount(iCount);
		SetRowCount(1);
		SetFixedRowCount(1);
		SetFixedTextColor(m_crWindowText);	
		SetFixedBkColor(m_cr3DFace);
		SetTextColor(m_crWindowText);
		SetGridBkColor(0xFFFFFF);
		SetTextBkColor(m_crWindowColour);
		//SetAutoSizeStyle(GVS_DEFAULT);
		CClientDC dc(this);
		short tmLen = (short)dc.GetTextExtent("0").cx;
		for (int i = 0; i < iCount; i++)
		{
			ITradeColumn* pItem = m_pTradeTable->GetItems(i);
			if (pItem)
			{
				SetItemData(0, i, (LPARAM)pItem);
				CString sTmp = pItem->GetDispName();
				int nLen = sTmp.GetLength();
				SetItemText(0, i, pItem->GetDispName());
				SetColumnWidth(i, nLen * tmLen);
			}
		}
	}
	
}