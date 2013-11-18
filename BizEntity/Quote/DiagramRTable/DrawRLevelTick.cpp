#include "StdAfx.h"
#include "DrawRLevelTick.h"
#include "GeneralHelper.h"

CDrawRLevelTick::CDrawRLevelTick(IDataSourceEx* pDataSource)
{
	SetStyle(TABLE_STYLE_SCREEN | TABLE_HEIGHT_CHANGABLE);
	m_bDispFour = FALSE;
	m_bFullTime = FALSE;

	m_pDataSource = pDataSource;
	memset(&m_preCodeInfo, 0, sizeof(CodeInfo));
}

CDrawRLevelTick::~CDrawRLevelTick(void)
{
}

BOOL CDrawRLevelTick::CreateSelf()
{
	m_nLineNum = 0; 
	DWORD lItemStyle(0);
	DWORD lCellStyle(0);
	////////////////////////////  时间  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	for (int i = 0; i < RLEVELTICK_ROW; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_TIME, lItemStyle, lCellStyle);
	}

	////////////////////////////  价格  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT | ITEM_ARROW_NEED;
	for (int i = 0; i < RLEVELTICK_ROW; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	////////////////////////////  现手  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < RLEVELTICK_ROW; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	////////////////////////////  成交  //////////////////////////////////////
	if (m_bDispFour)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < RLEVELTICK_ROW; i++)
		{
			CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
		}
		SetColCount(4);
	}
	else
		SetColCount(3);

	SetRowCount(RLEVELTICK_ROW);
	SetFixColCount(0);
	SetFixRowCount(0);

	if (m_bDispFour)
	{
		int ayWidth[4] = {80, 80, 40, 40};
		for (int i = 0; i < 4; i++)
		{
			ColInfoStruct colInfo;
			colInfo.nWidth = ayWidth[i];
			SetColInfo(i, &colInfo);
		}
	}
	else
	{
		int ayWidth[3] = {80, 100, 100};
		for (int i = 0; i < 3; i++)
		{
			ColInfoStruct colInfo;
			colInfo.nWidth = ayWidth[i];
			SetColInfo(i, &colInfo);
		}
	}
	return TRUE;
}

BOOL CDrawRLevelTick::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo )
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
	m_bIsShow = FALSE;

	BOOL bDispFour(TRUE);
	if (pStockInfo->m_ciStockCode.m_cCodeType & SH_BOURSE)
	{//如果是上海
		bDispFour = FALSE;
	}
	else if (pStockInfo->m_ciStockCode.m_cCodeType & SZ_BOURSE)
	{
		bDispFour = TRUE;
	}
	if (GetCellCount() == 0 || m_bDispFour != bDispFour)
	{// 如果单元格数量为0 或者 市场变更 需要重新生成单元格
		m_bDispFour = bDispFour;
		RemoveAllCell();
		CreateSelf();
		CalCulateSize();
	}

	//////////////////////////////////////////////////////////////////////////
	// 先发送Level2实时请求
	int lAskLen = sizeof(AskData);
	char* pData = new char[lAskLen]; 
	AskData*  pAskData = (AskData*)pData;
	memset(pAskData, 0, lAskLen);
	pAskData->m_nType  = RT_LEVEL_REALTIME;
	pAskData->m_nSize  = 1;
	memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
	pReqCollector->AddRequest((char*)pAskData,lAskLen);

	//////////////////////////////////////////////////////////////////////////
	// 请求主推
	pAskData->m_nType = RT_LEVEL_AUTOPUSH;
	pReqCollector->AddRequest((char*)pAskData, lAskLen);

	return TRUE;
}

int CDrawRLevelTick::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_LEVEL_REALTIME)
	{// level2 实时行情

		AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
		if (pLevelAuto->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
			if (pCodeInfo->CompareCode(&m_preCodeInfo))
			{
				m_lBuyPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lBuyPrice1; // 买一价
				m_lSellPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lSellPrice1; // 卖一价

				//////////////////////////////////////////////////////////////////////////
				// 逐笔请求发送
				AskData* pAskData = NULL;
				long lRetLen = sizeof(AskData) - sizeof(CodeInfo);
				int  nCount = (sizeof(ReqLevelTransaction) + sizeof(CodeInfo) - 1) / sizeof(CodeInfo);
				lRetLen += nCount * sizeof(CodeInfo);

				pAskData = (AskData*) new char[lRetLen];
				memset(pAskData, 0, lRetLen);
				pAskData->m_nType  = RT_LEVEL_TRANSACTION;
				pAskData->m_nSize  = nCount;

				ReqLevelTransaction *pRLT = (ReqLevelTransaction *)pAskData->m_pCode;
				memcpy(&pRLT->m_Codeinfo, &m_preCodeInfo, sizeof(CodeInfo));
				pRLT->m_nSize = RLEVELTICK_ROW;
				pRLT->m_nPostion = 0;

				IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
				if (pReqCollector)
				{
					pReqCollector->AddRequest((char*)pAskData,lRetLen);
					m_pRegion->SendRequest(pReqCollector);
					pReqCollector->Release();
				}
				delete[] pAskData;	

				//////////////////////////////////////////////////////////////////////////
				// 请求逐笔主推
				int Len = sizeof(AskData);
				AskData* ask = (AskData*)(new char[Len]);
				memset(ask,0,Len);
				ask->m_nType  = RT_LEVEL_TRANSACTION_AUTOPUSH;
				ask->m_nSize  = 1;
				memcpy(&ask->m_pCode, &m_preCodeInfo, sizeof(CodeInfo));

				pReqCollector = m_pRegion->GetRequestCollector();
				if (pReqCollector)
				{
					pReqCollector->AddRequest((char*)ask, Len);
					m_pRegion->SendRequest(pReqCollector);
					pReqCollector->Release();
				}
				delete[] ask;
			}
			return 1;
		}
	}
	else if (pData->m_nType == RT_LEVEL_AUTOPUSH)
	{
		AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
		if (pLevelAuto->m_nSize <= 0)
		{
			return -1;
		}
		else
		{
			CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
			if (pCodeInfo->CompareCode(&m_preCodeInfo))
			{// 更新买一价、卖一价
				m_lBuyPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lBuyPrice1; // 买一价
				m_lSellPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lSellPrice1; // 卖一价
			}
			return 1;
		}
	}
	else if (pData->m_nType == RT_LEVEL_TRANSACTION)
	{
		AnsLevelTick *pTickData = (AnsLevelTick *)pData;
		if (pTickData->m_nSize <= 0)
		{
			return -1;
		}
		
		m_bIsShow = TRUE;

		HSLevelTransaction* pLevel = pTickData->m_Data;
		int nCount = pTickData->m_nSize;
		m_nLineNum = 0;
		
		m_nHand = 100;
		// 上证权证
		int isQZHand = m_nHand;
		if( MakeMainMarket(m_preCodeInfo.m_cCodeType) == SH_Bourse &&
			MakeHexSubMarket(m_preCodeInfo.m_cCodeType) == KIND_QuanZhen )
			isQZHand = 100;

		int h, m, s;
		StockTick st;
		unsigned long lCurrentTotal = 0;
		for (int i = 0; i < nCount; i++)
		{
			h = pLevel->m_TradeTime / 10000;
			m = pLevel->m_TradeTime % 10000 / 100;
			s = pLevel->m_TradeTime % 100;
			memset(&st, 0, sizeof(StockTick));

			st.m_nTime = h * 100 + m;
			st.m_sDetailTime.m_nSecond = s;
			st.m_lBuyPrice = m_lBuyPrice;
			st.m_lSellPrice = m_lSellPrice;
			st.m_lCurrent = CGeneralHelper::GetDivision(pLevel->m_TradeQty, isQZHand, TRUE);
			m_lCurrentTotal += st.m_lCurrent;
			st.m_lNewPrice = pLevel->m_TradePrice;

			HandleStockTrace(&st);
		}

	}

	return 1;
}

void CDrawRLevelTick::HandleStockTrace( StockTick* pData )
{

}