#include "StdAfx.h"
#include "DrawExitRight.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDrawExitRight::CDrawExitRight(IDataSourceEx* pDataSource)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_WIDTH_CHANGABLE);
	m_pDataSource = pDataSource;	
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT);
	m_preCodeInfo.SetEmpty();
}

CDrawExitRight::~CDrawExitRight(void)
{
	
}
BOOL CDrawExitRight::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}

	if (pStockInfo->m_ciStockCode.CompareCode(&m_preCodeInfo))
	{
		return FALSE;
	}
	m_preCodeInfo = pStockInfo->m_ciStockCode;

	CArray<CodeInfo> ayCode;
	ayCode.Add(pStockInfo->m_ciStockCode);
	CArray<CurrentExRightData> ayExrightDat;
	m_pDataSource->HSDataSourceEx_GetStocksExrightData(ayCode,ayExrightDat);

	//请求新数据时清空原有数据
	RemoveAllCell();
	//更新新数据
	for(int i= 0; i< ayExrightDat.GetCount(); i++)
	{
		CurrentExRightData dataExright = ayExrightDat.GetAt(i);
		if(dataExright.GetCode()->GetCode() == pStockInfo->m_ciStockCode.GetCode() )
		{
			OnUpdateExitRightData(&dataExright);
			break;
		}
	}

	for(int  i= 0; i < ayExrightDat.GetCount(); i++)
	{
		CurrentExRightData dataExright = ayExrightDat.GetAt(i);
		dataExright.Release();
	}
	ayExrightDat.RemoveAll();

	CalCulateSize();
	m_pRegion->Invalidate(FALSE);

	return TRUE;
}

void CDrawExitRight::OnUpdateExitRightData(CurrentExRightData* pData)
{
	if(pData == NULL)
		return;

	SetRowCount(pData->GetCount());
	SetColCount(ExitRight_Table_ShowColCount);
	SetFixColCount(0);
	SetFixRowCount(0);
	SetRowHeight(ExitRight_Table_RowHeight);

	CString strText;
	for(int i = 0 ; i < pData->GetCount(); i++)
	{
		HSExRightItem* pExRightItem = pData->GetExRightDataItem(i);
		if(!pExRightItem)
			continue;

		strText.Format(" %02d/%02d/%02d",
			(pExRightItem->m_nTime/10000)%100,(pExRightItem->m_nTime % 10000)/100,
			(pExRightItem->m_nTime % 10000)%100);

		CString strTmp;
		if( pExRightItem->m_fGivingStock != 0 )
		{
			strTmp.Format(_T(" 10送%.3f"),pExRightItem->m_fGivingStock*10);
			strText += strTmp;
		}
		if( pExRightItem->m_fPlacingStock != 0 )
		{
			strTmp.Format(_T(" 10配%.3f"),pExRightItem->m_fPlacingStock*10);
			strText += strTmp;
		}
		if( pExRightItem->m_fGivingPrice != 0 )
		{
			strTmp.Format(_T(" 配价%.3f"),pExRightItem->m_fGivingPrice);
			strText += strTmp;
			strText += _T("元");
		}
		if( pExRightItem->m_fBonus != 0 )
		{
			strTmp.Format(_T(" 10派%.3f"),pExRightItem->m_fBonus*10);
			strText += strTmp;
			strText += _T("元");
		}
		if( strText[strText.GetLength()-1] == ',' )
			strText.Delete(strText.GetLength()-1,1);
		CreateCell(strText,TEXT_COLOR_FIXEDNAME,TEXT_FONT_FINANCE,ITEM_TEXT_LEFT);
	}
}
