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

	CreateVerScroll(HSSB_VERT | HSSB_RIGHT | HSSB_BLOCK_ALWAYBOTTOM); // ����������
	CreateTitle(); // ��������
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
	m_pTitle->SetText(_T("��ʱ�ɽ��б�(��0��)"));
	//////////////////////////////////////////////////////////////////////////
	//  ����һЩ�������
	if (pStockInfo->m_ciStockCode.m_cCodeType & SH_BOURSE)
	{// ������Ϻ�
		m_bDispFour = FALSE;
		SetRowHeight(22);
		SetColCount(1);
		ColInfoStruct colInfo;
		colInfo.nWidth = TickCellWidth;
		SetColInfo(0, &colInfo);
	}
	else if(pStockInfo->m_ciStockCode.m_cCodeType & SZ_BOURSE)
	{// ���������
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
	// ��StockTick��
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
	// ��������
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
	TRACE("Tick ���� \n");
	return TRUE;
}

int CDrawTrendDetail::OnUpdateData( DataHead* pData )
{
	if (pData->m_nType == RT_STOCKTICK)
	{	
		TRACE("OnUpdateData - RT_STOCKTICK \n");
		HSMarketDataType nMarket = m_stkInfo.m_ciStockCode.m_cCodeType;
		AnsStockTick *pTraceData = (AnsStockTick *)pData;

		// ���˰�����Ϊ0 �Լ����ǵ�ǰ��Ʊ��Ϣ�İ�
		if( pTraceData->m_nSize <= 0 || !m_stkInfo.m_ciStockCode.CompareCode(&pTraceData->m_dhHead.m_nPrivateKey.m_pCode))
			return -1;
		if (m_lStockTickCount == pTraceData->m_nSize)
		{// ��ͬ��Stocktick��
			return -1;
		}
		m_lStockTickCount = pTraceData->m_nSize;

		//////////////////////////////////////////////////////////////////////////
		// �ָ�һЩ��־�����ĳ�ʼֵ
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
				// ���˲���1�ֵ����
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
					// 15.01�ֱ� 
					if(pPreStockTick && pPreStockTick->m_lNewPrice == pTraceData->m_traData[i].m_lNewPrice)
					{
						pPreStockTick = &pTraceData->m_traData[i];
						continue;
					}
					else // 15.01�ֱ� �۸�ض��б仯
					{
						// 15.01�ֱ� ʱ��ض���240 
						if(pTraceData->m_traData[i].m_nTime < m_StockType.m_nTotalTime)
						{
							pPreStockTick =  &pTraceData->m_traData[i];
							continue;	
						}
					}
				}

				pStockTick = new StockTick;
				memcpy(pStockTick, &pTraceData->m_traData[i], sizeof(StockTick));
				// ����			
				if( pPreStockTick )
				{
					pStockTick->m_lCurrent = pStockTick->m_lCurrent - pPreStockTick->m_lCurrent;
				}
				AddRowData(pStockTick, nIndex++);

				delete pStockTick;
				pPreStockTick = &pTraceData->m_traData[i];
			}
			SetCellCount(nIndex, TRUE); // ���õ�Ԫ������
			DWORD dwEnd = ::GetTickCount();
			TRACE("��Ԫ�񴴽���� ��ʱ %d \n\n", dwEnd - dwStart);
			TRACE("�ӷ������հ��ٴ��� ��ʱ %d \n\n", dwEnd - m_lTickCount);
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
		TRACE("��ʼ�ػ� \n");
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
			{// ���֮ǰ�Ĺ�����������ײ� �����ư�����֮�� ������һֱ����ײ�
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
	TRACE("Draw �ɽ���ϸ \n");
	BOOL bRet = TRUE;
	bRet = CDrawTableBase::Draw(pDC);
	if (bRet == FALSE)
	{
		return FALSE;
	}
	
	// ���Ʒָ���
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
	TRACE("�ӷ��������Ƴɹ�һ����ʱ %d \n\n", dwEnd - m_lTickCount);
	return bRet;
}
void CDrawTrendDetail::AddRowData(StockTick* pStockTick, int nIndex)
{
	if (m_bIsShow == FALSE)
	{
		//////////////////////////////////////////////////////////////////////////
		// �ָ�һЩ��־�����ĳ�ʼֵ
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
	// ʱ��
	pItemStruct = &ayConstructionStuct.GetAt(0);
	pItemStruct->dwItemStyle = ITEM_TEXT_CENTER;// ��ԪС����
	pItemStruct->wClr = TEXT_COLOR_TIME;        // �ı���ɫ
	pItemStruct->wFont = TEXT_FONT_TIME;        // �ı�����
	pItemStruct->dWidth = 0.45;                 // ��ԪС����
	CTime tm = CHSCalculate::GetTimer(&m_stkInfo, pStockTick, &m_StockType, TRUE, m_lDate, m_lPreTime, strText);
	m_lPreTime = pStockTick->m_nTime;

	// �Ƚ�ǰ������ʱ�� ȷ����Ԫ��ı߿�
 	DWORD dwCellSyle = 0;
	if (m_strPreTime[0] != '\0')
	{
		if ( strncmp(strText, m_strPreTime, 8) < 0)
		{// ����Ҫ�����⴦�� 
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
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // �ַ���
	strncpy(m_strPreTime, strText, strlen(strText)+1);
	//

	//////////////////////////////////////////////////////////////////////////
	// �ɽ���
	pItemStruct = &ayConstructionStuct.GetAt(1);
	pItemStruct->dwItemStyle = ITEM_TEXT_LEFT | ITEM_ARROW_NEED;  // ��ԪС����
	pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // �ı�����
	pItemStruct->dWidth = 0.2;  // ��ԪС����
	CGeneralHelper::LongToString(pStockTick->m_lNewPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // �ַ���

	pItemStruct->wClr = GetTextColorID(pStockTick->m_lNewPrice, m_stkInfo.m_lPrevClose); // ��ɫ
	// �۸����ļ�ͷ
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
	// ����
	pItemStruct = &ayConstructionStuct.GetAt(2);
	pItemStruct->dwItemStyle = ITEM_TEXT_RIGHT; // ��ԪС����
	pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // ��ԪС������
	pItemStruct->dWidth = 0.2; // ��ԪС����
	CGeneralHelper::LongToString(pStockTick->m_lCurrent, 0, m_stkInfo.m_nHandNum, 0, strText);
	strncpy(pItemStruct->str, strText, strlen(strText)+1); // ��ԪС���ı�����
	pItemStruct->wClr = TEXT_COLOR_UP;  // ��ԪС���ı���ɫ
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
	// �����Ʊ�ĵ��ʳɽ���
	if (m_bDispFour)
	{	
		pItemStruct = &ayConstructionStuct.GetAt(3);
		pItemStruct->dwItemStyle = ITEM_TEXT_RIGHT; // ��ԪС����
		pItemStruct->wFont = TEXT_FONT_DYNAMICAL;   // ��ԪС������
		pItemStruct->dWidth = 0.1; // ��ԪС����
		if(pStockTick->m_nChiCangLiang <= 0) //���Ӷ�m_nChiCangLiangΪ�յı���
			pStockTick->m_nChiCangLiang = m_nTotalHand; 
		CGeneralHelper::LongToString((pStockTick->m_nChiCangLiang < m_nTotalHand) ? 0 : (pStockTick->m_nChiCangLiang - m_nTotalHand), 0, 1, 0, strText);
		strncpy(pItemStruct->str, strText, strlen(strText)+1); // ��ԪС���ı�����
		pItemStruct->wClr = TEXT_COLOR_FLAT;  // ��ԪС���ı���ɫ
		m_nTotalHand = pStockTick->m_nChiCangLiang;
	}

	CDrawCell* pCell = CreateCell(&ayConstructionStuct, dwCellSyle, nIndex);
	pCell->m_logPen.lopnStyle = PS_DOT;

	SetRowCount(nIndex+1);
	CString str;
	str.Format("��ʱ�ɽ��б�(��%d��)", nIndex+1);
	m_pTitle->SetText(str);
}

WORD CDrawTrendDetail::GetTextColorID( double newValue, double preValue )
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
