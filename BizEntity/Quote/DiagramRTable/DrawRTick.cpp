#include "StdAfx.h"
#include "DrawRTick.h"
#include "GeneralHelper.h"
#include "HSCalculate.h"
#include "HSMessage.h"
#include "HSWinner.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RTick_Msg_Param_Name  "RowFlag"  

CDrawRTick::CDrawRTick(IDataSourceEx* pDataSource)
:CDrawTableBase()
{
	SetStyle(TABLE_STYLE_SCREEN);
	m_bFullTime = FALSE;
	m_bDispFour = FALSE;

	m_pDataSource = pDataSource;
	memset(&m_sLevelDataAuto, 0, sizeof(HSLevelTransaction));
	m_ayRowFlag = NULL;
	m_nFlag = 0;
}

CDrawRTick::~CDrawRTick(void)
{
	if (m_ayRowFlag)
	{
		delete[] m_ayRowFlag;
		m_ayRowFlag = NULL;
	}
}

BOOL CDrawRTick::CreateSelf()
{
	DWORD lItemStyle(0);
	DWORD lCellStyle(0);

	int nRowCount = m_bZhuBi?RTICK_ZHUBI_ROW:RTICK_ROW;
	////////////////////////////  ʱ��  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_LEFT;
	for (int i = 0; i < nRowCount; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_TIME, lItemStyle, lCellStyle);
	}

	////////////////////////////  �۸�  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT | ITEM_TEXT_SELECT | ITEM_ARROW_NEED;
	for (int i = 0; i < nRowCount; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	////////////////////////////  ����  //////////////////////////////////////
	lItemStyle = ITEM_TEXT_RIGHT;
	for (int i = 0; i < nRowCount; i++)
	{
		CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
	}

	////////////////////////////  �ɽ�  //////////////////////////////////////
	if (m_bDispFour)
	{
		lItemStyle = ITEM_TEXT_RIGHT;
		for (int i = 0; i < nRowCount; i++)
		{
			CreateCell("", TEXT_COLOR_TIME, TEXT_FONT_DYNAMICAL, lItemStyle, lCellStyle);
		}
		SetColCount(4);
	}
	else
		SetColCount(3);

	SetRowCount(nRowCount);
	SetFixColCount(0);
	SetFixRowCount(0);
	SetRowHeight(RTICK_ROW_HEIGHT);

	if (m_bDispFour)
	{
		int ayWidth[4] = {80, 80, 40, 30};
		for (int i = 0; i < 4; i++)
		{
			ColInfoStruct colInfo;
			colInfo.nWidth = ayWidth[i];
			SetColInfo(i, &colInfo);
		}
	}
	else
	{
		int ayWidth[3] = {80, 80, 74};
		for (int i = 0; i < 3; i++)
		{
			ColInfoStruct colInfo;
			colInfo.nWidth = ayWidth[i];
			SetColInfo(i, &colInfo);
		}
	}
	return TRUE;
}

BOOL CDrawRTick::Create(IDrawBaseParam *pParam)
{
	m_bZhuBi = pParam->GetIntegerParam("ZhuBi");
	m_bLevel = pParam->GetIntegerParam("Level");

	int nRow = m_bZhuBi?RTICK_ZHUBI_ROW:RTICK_ROW;
	m_ayRowFlag = new unsigned short[nRow];
	for (int i = 0; i < nRow; i++)
		m_ayRowFlag[i] = 0;

	return TRUE;
}

BOOL CDrawRTick::GetRequestData( IRequestCollector* pReqCollector,StockUserInfo* pStockInfo,BOOL bForce)
{
	if (pStockInfo == NULL)
	{
		return FALSE;
	}
	if (MakeIndexMarket(pStockInfo->m_ciStockCode.m_cCodeType))
	// ����ָ��
		return TRUE;

	BOOL bDispFour(TRUE);
	if (pStockInfo->m_ciStockCode.m_cCodeType & SH_BOURSE)
	{//������Ϻ�
		bDispFour = FALSE;
	}
	else if (pStockInfo->m_ciStockCode.m_cCodeType & SZ_BOURSE)
	{
		bDispFour = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	m_bDispFour = bDispFour;
	InitFlag(); // ��ʼ�����ֵ
	m_bIsShow = FALSE;
	m_lStockTickCount = 0;
	m_lTotalVol = 0;
	//////////////////////////////////////////////////////////////////////////
	// �Ѿ�һЩ����Ԥ����
	// ��ȡ��ǰ��Ʊ�����һЩ������Ϣ
	CArray<StockType> ayStockType;
	unsigned short marketType = MakeMainMarket(pStockInfo->m_ciStockCode.m_cCodeType);
	m_pDataSource->HSDataSourceEx_GetSystemMarketInfo(&marketType , 1, ayStockType);
	if (ayStockType.GetCount() == 1)
		m_StockType = ayStockType.GetAt(0);
	else
	{// ����Ҳ��� ������
		return TRUE;
	}
	//////////////////////////////////////////////////////////////////////////
	// LEVEL1
	if (!m_bLevel)
	{
		//////////////////////////////////////////////////////////////////////////
		// ����ֱ�
		int nSize = sizeof(ReqLimitTick) / sizeof(CodeInfo) +
			((sizeof(ReqLimitTick) / sizeof(CodeInfo))?1:0);

		int lRetLen = sizeof(AskData) - sizeof(CodeInfo) + sizeof(CodeInfo) * nSize;
		AskData* pakData       = (AskData*)new char[lRetLen];
		memset(pakData,0,lRetLen);

		pakData->m_lKey        = (long)this;
		pakData->m_nType	   = RT_LIMITTICK;
		pakData->m_nSize	   = nSize;
		pakData->m_nPrivateKey.m_pCode = m_stkInfo.m_ciStockCode;

		ReqLimitTick* pReqLimitTick = (ReqLimitTick*)pakData->m_pCode;
		memcpy(&pReqLimitTick->m_pCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));

		pReqLimitTick->m_nCount = m_bZhuBi?RTICK_ZHUBI_ROW+1:RTICK_ROW+1;
		pReqCollector->AddRequest((char*)pakData,lRetLen);
		delete[] pakData;
		//////////////////////////////////////////////////////////////////////////
		// ��������
		int lAskLen = sizeof(AskData);
		char* pData = new char[lAskLen]; 
		AskData*  pAskDataAuto = (AskData*)pData;
		memset(pAskDataAuto, 0, lAskLen);
		pAskDataAuto->m_nType  = RT_AUTOPUSH_EXT;
		pAskDataAuto->m_nSize  = 1;
		pAskDataAuto->m_nPrivateKey.m_pCode = m_stkInfo.m_ciStockCode;

		memcpy(pAskDataAuto->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
		pReqCollector->AddRequest((char*)pAskDataAuto,lAskLen);
		delete[] pData;
	}
	//////////////////////////////////////////////////////////////////////////
	// LEVEL2
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// �ȷ���Level2ʵʱ����
		int lAskLen = sizeof(AskData);
		char* pData = new char[lAskLen]; 
		AskData*  pAskData = (AskData*)pData;
		memset(pAskData, 0, lAskLen);
		pAskData->m_nType  = RT_LEVEL_REALTIME;
		pAskData->m_nSize  = 1;
		pAskData->m_nPrivateKey.m_pCode = m_stkInfo.m_ciStockCode;
		memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
		pReqCollector->AddRequest((char*)pAskData,lAskLen);

		//////////////////////////////////////////////////////////////////////////
		// ��������
		pAskData->m_nType = RT_LEVEL_AUTOPUSH;
		pReqCollector->AddRequest((char*)pAskData, lAskLen);
	}
	return TRUE;
}

int CDrawRTick::OnUpdateData( DataHead* pData )
{
	//////////////////////////////////////////////////////////////////////////
	// LEVEL1
	if (!m_bLevel)
	{
		if (pData->m_nType == RT_LIMITTICK)
		{
			m_bIsShow = FALSE; // ��Ҫ��ʼ�� 

			AnsStockTick *pTraceData = (AnsStockTick*)pData; 
			int nCurCount = pTraceData->m_nSize;
			if (nCurCount <= 0 && !m_stkInfo.m_ciStockCode.CompareCode(&pTraceData->m_dhHead.m_nPrivateKey.m_pCode))
			{// ���˵�������Ϊ0 �Լ� ���ǵ�ǰ��Ʊ�İ�
				return -1;
			}
			if (m_lStockTickCount == nCurCount)
			{// ������ͬ��StockTick��
				return -1;
			}
			m_lStockTickCount = nCurCount;
			
			int nBegin(0);
			if (m_bIsShow == FALSE)
			{// ���û�г�ʼ�� ��Ҫ��ʼ������
				InitData(&pTraceData->m_traData[0]); // ��һ���������ڳ�ʼ��
				if (nCurCount > 2)
				{
					m_lTotalVol = pTraceData->m_traData[0].m_lCurrent;
					if (pTraceData->m_traData[1].m_lCurrent != 0)
						nBegin = 1;
				}
			}

			for( int i = nBegin; i < nCurCount; i++ )
			{	
				if (pTraceData->m_traData[i].m_lCurrent != 0)
					HandleStockTrace(&pTraceData->m_traData[i]);
			}
			InValidate(NULL, FALSE);
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
				HandleNowData( (DataHead*)pData, pRealData, pEveryData->m_nSize);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// LEVEL2
	else
	{
		if (pData->m_nType == RT_LEVEL_REALTIME)
		{// level2 ʵʱ����

			AnsHSAutoPushLevel* pLevelAuto = (AnsHSAutoPushLevel*)pData;
			if (pLevelAuto->m_nSize <= 0)
			{
				return -1;
			}
			else
			{
				CodeInfo* pCodeInfo = &(pLevelAuto->m_pstData->m_ciStockCode);
				if (pCodeInfo->CompareCode(&m_stkInfo.m_ciStockCode))
				{
					m_lBuyPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lBuyPrice1;   // ��һ��
					m_lSellPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lSellPrice1; // ��һ��

					//////////////////////////////////////////////////////////////////////////
					// ���������
					AskData* pAskData = NULL;
					long lRetLen = sizeof(AskData) - sizeof(CodeInfo);
					int  nCount = (sizeof(ReqLevelTransaction) + sizeof(CodeInfo) - 1) / sizeof(CodeInfo);
					lRetLen += nCount * sizeof(CodeInfo);

					pAskData = (AskData*) new char[lRetLen];
					memset(pAskData, 0, lRetLen);
					pAskData->m_nType  = RT_LEVEL_TRANSACTION;
					pAskData->m_nSize  = nCount;

					ReqLevelTransaction *pRLT = (ReqLevelTransaction *)pAskData->m_pCode;
					memcpy(&pRLT->m_Codeinfo, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));
					pRLT->m_nSize = RTICK_ZHUBI_ROW;//GetRowCount();
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
					// �����������
					int Len = sizeof(AskData);
					AskData* ask = (AskData*)(new char[Len]);
					memset(ask,0,Len);
					ask->m_nType  = RT_LEVEL_TRANSACTION_AUTOPUSH;
					ask->m_nSize  = 1;
					memcpy(&ask->m_pCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));

					pReqCollector = m_pRegion->GetRequestCollector();
					if (pReqCollector)
					{
						pReqCollector->AddRequest((char*)ask, Len);
						m_pRegion->SendRequest(pReqCollector);
						pReqCollector->Release();
					}
					delete[] ask;
				}
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
				if (pCodeInfo->CompareCode(&m_stkInfo.m_ciStockCode))
				{// ������һ�ۡ���һ��
					m_lBuyPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lBuyPrice1;   // ��һ��
					m_lSellPrice = pLevelAuto->m_pstData->m_sLevelRealTime.m_lSellPrice1; // ��һ��
				}
			}
		}
		else if (pData->m_nType == RT_LEVEL_TRANSACTION)
		{
			AnsLevelTick *pTickData = (AnsLevelTick *)pData;
			if (pTickData->m_nSize <= 0)
			{
				return -1;
			}
			HandleNowData(pData, pTickData->m_Data, pTickData->m_nSize);
		}
		else if (pData->m_nType == RT_LEVEL_TRANSACTION_AUTOPUSH)
		{
			AnsHsLevelTransactionAuto *pTrans = (AnsHsLevelTransactionAuto *)pData;
			LevelTransactionData *pTranData = pTrans->m_Data;

			if (memcmp(&m_sLevelDataAuto, pTrans->m_Data->m_Data, sizeof(HSLevelTransaction)) == 0)
			// ���˵���ͬ�����ư�			
				return 0;
			
			memcpy(&m_sLevelDataAuto, pTranData->m_Data, sizeof(HSLevelTransaction));
			for(int i = 0; i < pTrans->m_nSize; i++)
			{
				if (pTranData->m_CodeInfo.CompareCode(&m_stkInfo.m_ciStockCode))
				{
					HandleNowData(pData, pTranData->m_Data, pTranData->m_nSize);
					break;
				}
				pTranData = (LevelTransactionData *)((char *)pTranData + sizeof(LevelTransactionData) + (pTranData->m_nSize - 1) * sizeof(HSLevelTransaction));
			}
		}
	}
	return 1;
}

void CDrawRTick::HandleNowData(DataHead* pHead, CommRealTimeData_Ext* pnowData, int nSize)
{// �������������� �ҵ���ǰ������Ϣ
	if( pnowData == NULL || nSize <= 0 )
		return;

	CommRealTimeData_Ext* pData = pnowData;
	CommRealTimeData_Ext* pNextData = NULL;

	int nNowSize = 0;
	for(int i = 0; i < nSize; i++)
	{
		pNextData = pData->GetNext(&pData->m_ciStockCode,nNowSize);
		if( pNextData == pData )
		{
			pNextData = pData->GetNext(&pHead->m_nPrivateKey.m_pCode,nNowSize);
		}

		if (pData->m_ciStockCode.CompareCode(&m_stkInfo.m_ciStockCode)
			&& 
			pData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal != m_lTotalVol)
		{
			StockTick staData;
			memset(&staData,0,sizeof(staData));
			staData.m_nTime  = pData->m_othData.m_nTime;	
			staData.m_sDetailTime.m_nSecond = (char)(pData->m_othData.m_sDetailTime.m_nSecond);

			staData.m_lNewPrice  = pData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lNewPrice;
			staData.m_lCurrent   = pData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lTotal;
			staData.m_lBuyPrice  = pData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lBuyPrice1;
			staData.m_lSellPrice = pData->GetShareData()->m_nowDataExt.m_stockRealTime.m_lSellPrice1;
			staData.m_nChiCangLiang = pData->m_othData.m_lKaiCang;
			
			if (m_bIsShow == FALSE)
				InitData(&staData);
			
			HandleStockTrace(&staData);
			
			InValidate(NULL, FALSE);
			break;
		}
		pData = pNextData;		
	}
}

void CDrawRTick::HandleNowData(DataHead *pHead, HSLevelTransaction *pData, int nSize)
{
	if( pData == NULL || nSize <= 0 )
		return;

	int h, m, s;
	StockTick st;
	unsigned long lCurrentTotal = 0;
	for (int i = 0; i < nSize; i++)
	{
		h = pData->m_TradeTime / 10000;
		m = pData->m_TradeTime % 10000 / 100;
		s = pData->m_TradeTime % 100;
		memset(&st, 0, sizeof(StockTick));

		st.m_nTime = h * 100 + m;
		st.m_sDetailTime.m_nSecond = s;
		st.m_lBuyPrice = m_lBuyPrice;
		st.m_lSellPrice = m_lSellPrice;
		st.m_lCurrent = CGeneralHelper::GetDivision(pData->m_TradeQty, m_stkInfo.m_nHandNum, TRUE);
		lCurrentTotal += st.m_lCurrent;
		st.m_lNewPrice = pData->m_TradePrice;

		if (m_bZhuBi)
			HandleStockTrace(&st);
		pData++;
	}
	if (!m_bZhuBi)
	{// �Ƿֱ�
		st.m_lCurrent = lCurrentTotal;
		HandleStockTrace(&st);
	}
	else
		m_nFlag++;
	InValidate(NULL, FALSE);
}

void CDrawRTick::HandleStockTrace(StockTick* pData)
{
	if (pData == NULL)
		return;
	
	if (pData->m_lNewPrice <= 0)
		return;

	if( ((pData->m_lCurrent == m_lTotalVol) || 
		( MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == STOCK_MARKET && ((pData->m_lCurrent - m_lTotalVol) < m_stkInfo.m_nHandNum))) 
		&& (m_lPreNewPrice == pData->m_lNewPrice) )
	{
		return;
	}
	else if(pData->m_lCurrent != m_lTotalVol)
	{
		if( pData->m_lCurrent < m_lTotalVol )
		{
			m_lTotalVol  = pData->m_lCurrent;
			m_nTotalHand = pData->m_nChiCangLiang;

			m_lBuyPrice    = pData->m_lBuyPrice;
			m_lSellPrice   = pData->m_lSellPrice;
			m_lPreNewPrice = pData->m_lNewPrice;
			return;
		}
	}
	// �ɽ���δ�仯 ������15.01�ķֱ� ���ӱ�
	else if(m_nDataLineNum > 0)
	{
		if(pData->m_nTime < m_StockType.m_nTotalTime || 
			m_lPreNewPrice == pData->m_lNewPrice)
		{
			return;
		}
	}

	WORD nColor(0); // ��ɫ
	CPoint pos(0, GetLineNum());
	m_ayRowFlag[pos.y] = m_nFlag + 1;  // ���ø��еı��ֵ
	if (!m_bZhuBi)
		m_nFlag++;
	
	BOOL bFullTime = FALSE;  // �Ƿ���ʾ����ʱ�� �������ļ�
	char strText[64];
	CString str;
	// ʱ��
	pos.x = 0;
	if (m_bLevel)
	{
		int h = pData->m_nTime / 100;
		int m = pData->m_nTime % 100;
		if (bFullTime)
		{
			sprintf(strText, "%02d:%02d:%02d", h, m, pData->m_sDetailTime.m_nSecond);
		}
		else if(m_lPreTime == pData->m_nTime && m_nDataLineNum != 1 )
		{
			sprintf(strText, "%-9s:%02d","",pData->m_sDetailTime.m_nSecond);
		}
		else
		{
			sprintf(strText, "%02d:%02d:%02d", h, m, pData->m_sDetailTime.m_nSecond);
		}
	}
	else
	{
		long lDate(0), lTime(0);
		m_pDataSource->HSDataSourceEx_GetServerDataTime(lDate, lTime);
		CTime tm = CHSCalculate::GetTimer(&m_stkInfo, pData, &m_StockType, bFullTime, lDate, m_lPreTime, strText);
		if (m_lPreTime == pData->m_nTime && m_lPreSecond > pData->m_sDetailTime.m_nSecond)
		{// ����Ҫ�����⴦��
			memset(strText, 0, sizeof(strText));
			strncpy(strText, tm.Format("%H:%M:%S").GetBuffer(), 8);
		}
		m_lPreTime = pData->m_nTime;
		m_lPreSecond = pData->m_sDetailTime.m_nSecond;
	}
	SetCellText(pos, 0, strText);

	// �۸�
	CGeneralHelper::LongToString(pData->m_lNewPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
	pos.x = 1;
	// ѡ��۸���ɫ�Լ�����ļ�ͷ
	if (pData->m_lNewPrice > m_lPreNewPrice)
	{
		SetCellText(pos, 0, strText, GetTextColorID(pData->m_lNewPrice, m_stkInfo.m_lPrevClose), 0, ITEM_ARROW_UP, ITEM_ARROW_DOWN);
	}
	else if (pData->m_lNewPrice == m_lPreNewPrice)
	{
		SetCellText(pos, 0, strText, GetTextColorID(pData->m_lNewPrice, m_stkInfo.m_lPrevClose), 0, 0, ITEM_ARROW_UP| ITEM_ARROW_DOWN);
	}
	else
	{
		SetCellText(pos, 0, strText, GetTextColorID(pData->m_lNewPrice, m_stkInfo.m_lPrevClose), 0, ITEM_ARROW_DOWN, ITEM_ARROW_UP);		
	}
	m_lPreNewPrice = pData->m_lNewPrice;

	// ����
	pos.x = 2;
	unsigned long lCurrent;
	nColor = TEXT_COLOR_FLAT;
	if (m_bLevel)
	{
		lCurrent = pData->m_lCurrent;
		CGeneralHelper::LongToString(lCurrent, 0, 1, 0, strText, LTOA_MASK_HORLINE_IFZERO);
	}
	else
	{
		if(MakeMarket(m_stkInfo.m_ciStockCode.m_cCodeType) == WP_MARKET && m_lTotalVol == 0 )
		{
			lCurrent = 0;
			CGeneralHelper::LongToString((long)0, 0, 1, 0, strText);
		}
		else if(m_lTotalVol == pData->m_lCurrent && m_nDataLineNum <= 0)
		{
			lCurrent = m_lTotalVol;
			CGeneralHelper::LongToString(pData->m_lCurrent, pData->m_lCurrent, m_stkInfo.m_nHandNum, 0, strText);
		}
		else
		{
			lCurrent = pData->m_lCurrent - m_lTotalVol;
			if(m_lBuyPrice == 0) 
			{
				m_lBuyPrice = pData->m_lBuyPrice;
			}
			if(m_lSellPrice == 0) 
			{
				m_lSellPrice = pData->m_lSellPrice;
			}
			if(pData->m_lNewPrice <= m_lBuyPrice)
				nColor = TEXT_COLOR_DOWN;
			else if(pData->m_lNewPrice >= m_lSellPrice)
				nColor = TEXT_COLOR_UP;
			else
			{
				if(pData->m_lNewPrice <= pData->m_lBuyPrice)
					nColor = TEXT_COLOR_DOWN;
				else if(pData->m_lNewPrice >= pData->m_lSellPrice)
					nColor = TEXT_COLOR_UP;
			}
			CGeneralHelper::LongToString(lCurrent, 0, m_stkInfo.m_nHandNum, 0, strText);
		}	
	}
	//TRACE3("m_lCurrent = %d;   m_lTotalVol = %d;   lCurrent = %d \n", pData->m_lCurrent, m_lTotalVol, lCurrent);
	m_lTotalVol = pData->m_lCurrent;
	SetCellText(pos, 0, strText, nColor);

	if (m_bDispFour)
	{	
		//�����Ʊ�ĵ��ʳɽ���
		pos.x = 3;
		if(pData->m_nChiCangLiang <= 0) //���Ӷ�m_nChiCangLiangΪ�յı���
			pData->m_nChiCangLiang = m_nTotalHand;  
		CGeneralHelper::LongToString((pData->m_nChiCangLiang < m_nTotalHand) ? 0 : (pData->m_nChiCangLiang - m_nTotalHand), 0, 1, 0, strText);
		SetCellText(pos, 0, strText, TEXT_COLOR_FLAT);
	}

	m_nTotalHand = pData->m_nChiCangLiang;      // �ֲ���
	m_lBuyPrice  = pData->m_lBuyPrice;		    // ���
	m_lSellPrice = pData->m_lSellPrice;		    // ����
}

unsigned int CDrawRTick::GetLineNum()
{
	if (m_nDataLineNum < 0)
	{
		return -1;
	}
	else if (m_nDataLineNum < GetRowCount()) 
	{
		return m_nDataLineNum++;
	}
	else
	{
		int nDiff = m_nDataLineNum - (GetRowCount() - 1); // �ƶ���Ԫ��ľ���
		CellMove(nDiff);
		
		m_nDataLineNum -= nDiff;
		// ÿ�еı����ҲҪ��Ӧ��ƽ��
		for (int i = nDiff; i < GetRowCount(); i++)
			m_ayRowFlag[i-nDiff] = m_ayRowFlag[i];
		return m_nDataLineNum++;
	}
}

WORD CDrawRTick::GetTextColorID(double newValue, double preValue)
{
	double diff = newValue - preValue;
	if (diff > 0)
	{
		return TEXT_COLOR_UP;   // �ǵ���ɫ
	}
	else if (diff < 0)
	{
		return TEXT_COLOR_DOWN; // ������ɫ
	}
	else
	{
		return TEXT_COLOR_FLAT; // ƽ����ɫ
	}
}

BOOL CDrawRTick::MouseDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	if (!m_bZhuBi)
	{
		EmptyCellHighLight();	
		CString strValue("");
		int nRowSelected = IndexToPoint(GetActiveCellID()).y;
		if (nRowSelected < m_nDataLineNum && nRowSelected >= 0)
		{
			for (int i = 0; i < GetColCount(); i++)
			{
				CDrawCell* pCell = GetCell(PointToIndex(CPoint(i, nRowSelected)));
				pCell->m_dwStyle |= CELL_BK_HIGHLIGHT;
			}
			strValue.Format("%d", m_ayRowFlag[nRowSelected]);			
		}
		//////////////////////////////////////////////////////////////////////////
		// ��ͼ�㷢��Ϣ
		CDrawBaseParam msgParam;
		msgParam.AddParam(RTick_Msg_Param_Name, strValue);
		m_pRegion->SendNotifyMsg(DiagramMsg_TickOnClick, &msgParam);
		InValidate(NULL, FALSE);
	}
	return TRUE;
}

BOOL CDrawRTick::MouseDBDown( UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/ )
{
	int nActiveCellID = GetActiveCellID();
	CDrawCell* pCell = GetCell( nActiveCellID );
	if ( pCell && GetSignal() == NEED_TEXTHIGHLIGHT)
	{
		CString str("");
		GetCellText(GetActiveCellID(), 0, str);
		double dPrice = strtod(str, NULL);
		if (dPrice - 0.0001 > 0)
		{// �۸�Ϊ0 
			CPoint pos = IndexToPoint(nActiveCellID);
			// �ֱʡ���� Ĭ������
			//////////////////////////////////////////////////////////////////////////
			// ��Ӳ���
			CString strFormat("%");
			strFormat.AppendFormat(".%df", m_stkInfo.m_nDecimal);

			CDrawBaseParam param;
			str.Format(strFormat, dPrice);
			param.AddParam(PARAM_QUICKTRADE_PRICE, str);
			param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());
			m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, PN_QUICKTRADE_SELL);
		}
	}
	return TRUE;
}

void CDrawRTick::OnLayoutNotify( UINT nMsgId,IDrawBaseParam* pParam )
{
	if (m_bZhuBi)
	{
		if (nMsgId == DiagramMsg_TickOnClick)
		{
			int nRowFlag(0);
			nRowFlag = pParam->GetIntegerParam(RTick_Msg_Param_Name);
			
			EmptyCellHighLight();
			for (int i = 0; i < GetRowCount(); i++)
			{
				if (m_ayRowFlag[i] == nRowFlag)
				{
					for (int nCol = 0; nCol < GetColCount(); nCol++)
					{
						CDrawCell* pCell = GetCell(PointToIndex(CPoint(nCol, i)));
						pCell->m_dwStyle |= CELL_BK_HIGHLIGHT;
					}
				}
			}
			InValidate(NULL, FALSE);
		}
	}
}
void CDrawRTick::EmptyCellHighLight()
{
	for (int i = 0; i < GetCellCount(); i++)
	{
		CDrawCell* pCell = GetCell(i);
		pCell->m_dwStyle &= ~CELL_BK_HIGHLIGHT;
	}
}

void CDrawRTick::InitData(StockTick* pTick)
{
	if (pTick == NULL)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ɾ����Ԫ�� ���¹���
	RemoveAllCell();
	CreateSelf();
	CalCulateSize();
	//////////////////////////////////////////////////////////////////////////
	m_lPreTime = -1;
	m_lPreNewPrice = m_stkInfo.m_lPrevClose;
	m_lBuyPrice    = pTick->m_lBuyPrice;
	m_lSellPrice   = pTick->m_lSellPrice;
	m_lTotalVol    = 0;
	m_nTotalHand   = 0;
	m_nDataLineNum = 0; 
	m_bIsShow      = TRUE;
}

void CDrawRTick::InitFlag()
{
	for (int i = 0; i < GetRowCount(); i++)
		m_ayRowFlag[i] = 0;
	m_nFlag = 0;
	EmptyCellHighLight();
}
