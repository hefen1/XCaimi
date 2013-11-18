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
	CreateVerScroll(HSSB_VERT | HSSB_RIGHT); // ����������
	CreatSelf();
}

CDrawFenPrice::~CDrawFenPrice(void)
{
	RemoveAllData();
}

void CDrawFenPrice::CreatSelf()
{
	// �����һ�� һ����Ԫ���з�Ϊ�ĸ���ԪС��
	//////////////////////////////////////////////////////////////////////////
	// ��ȡ��Դ�ַ���
	CAtlBaseModule _Module;
	//CComModule _Module;
	HINSTANCE instance = _Module.GetResourceInstance();
	char* buf = new char[1000];
	int   nBufLenth = 1000;
	CString str;
	//

	//////////////////////////////////////////////////////////////////////////
	// �ּ۱�(���)
	::LoadString(instance, IDS_STRING_FENJIABIAO, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// �ɽ���
	::LoadString(instance, IDS_STRING_CHENGJIAOJIA, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// �ɽ���
	::LoadString(instance, IDS_STRING_CHENGJIAOLIANG, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	//////////////////////////////////////////////////////////////////////////
	// ����
	::LoadString(instance, IDS_STRING_BILI, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM);
	
	InitSize(1);
}

void CDrawFenPrice::InitSize(int nRowCount)
{
	// ������
	SetRowCount(nRowCount);
	SetFixRowCount(1);
	SetRowHeight(FENPRICE_ROW_HEIGHT);
	// ������
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
	TRACE("�ɽ���ϸ�յ� Draw \n");
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
	// ��һ�е� �����������е�Ԫ�� ��Ҫ����ɫ
	if (pos.y == 0)
	{
		if (pos.x > 0 && pos.x < 4)
		{
			// ������
			pDC->FillSolidRect(&rcItem, m_clrTitleBK);
		}
		return FALSE; // �õ�Ԫ�� ����Ҫ�ڽ��л���
	}
	// ���漸�е� ������ ��Ҫ������Ҫ���Ʊ���ͼ
	else
	{
		if (m_bIsShow == FALSE)
		{// ���û������ ����Ҫ���� ֱ�ӷ���
			return TRUE;
		}

		long lMaxVolume = FindMaxVolume();
		CPoint posMove = GetMove();
		if (pos.x == 3 && (pos.y + posMove.y - 1) < m_ayFenPriceData.GetCount())
		{
			// ������ͼ
			CFenPriceData* pPriceData = m_ayFenPriceData.GetAt(pos.y + posMove.y - 1);
			double dVolume = pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume;
			// ���㵱ǰ����ͼ�Ŀ��
			double dWidth = rcItem.Width() * dVolume / lMaxVolume;
			
			// �Ȼ�����(����)
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

			// �ٻ�����(����)
			rc = rcItem;
			rc.DeflateRect(1,3,0,3);

			int nWaiWidth = (int)(dWidth * pPriceData->m_lBuyVolume / dVolume);
			if (nWaiWidth > 0)
			{
				rc.left = rc.left + nNeiWidth; // �ȳ�ȥ���̵Ĵ�С
				rc.right = rc.left + nWaiWidth;
				
				pDC->FillSolidRect(&rc, RGB(255,255,255));
				rc.DeflateRect(1,1,1,1);
				pDC->FillSolidRect(&rc, m_pColor->GetUpColor());
				// ���Ʒָ���
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

			// ��ȡ��������
			char strText[64];
			double dValue = (double)(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume) * 100 / (double)(m_lTotalVol);
			CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_SHOW_PERSENT);
			CString str = strText;
			// ���û�ͼģʽ
			int nBkMode = pDC->SetBkMode(TRANSPARENT);
			// ��������
			CFont* pOldFont = pDC->SelectObject(m_pFont->GetRTableDynamicalFont());
			// ������ɫ �Ȼ���һ�ι̶���ɫ����
			COLORREF clrOld = pDC->SetTextColor(m_pColor->GetRTableFixedName());
			pDC->DrawText(str, str.GetLength(), &rcItem, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
			// ���ڱ���ͼ�� ������ɫ
			rc = rcItem;
			rc.right = rc.left + nNeiWidth + nWaiWidth;
			pDC->SetTextColor(RGB(0,0,255));
			pDC->DrawText(str, str.GetLength(), &rc, DT_VCENTER | DT_SINGLELINE | DT_LEFT);

			pDC->SetBkMode(nBkMode);
			pDC->SelectObject(pOldFont);
			pDC->SetTextColor(clrOld);
		
			return TRUE; // ���ƽ���
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
	// �ָ�һЩ��־�����ĳ�ʼֵ
	m_lPreNewPrice = m_stkInfo.m_lPrevClose;
	m_lPreSellPrice = m_lPreBuyPrice = -1;
	m_lStockTickCount = 0;
	RemoveAllData();

	//////////////////////////////////////////////////////////////////////////
	// ��StockTick��
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
	// ��������
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
		TRACE("�ɽ���ϸ�յ� RT_STOCKTICK \n");
		HSMarketDataType nMarket = m_stkInfo.m_ciStockCode.m_cCodeType;
		AnsStockTick *pTraceData = (AnsStockTick *)pData;

		// ���˰�����Ϊ0 �Լ����ǵ�ǰ��Ʊ��Ϣ�İ�
		if( pTraceData->m_nSize <= 0 || !m_stkInfo.m_ciStockCode.CompareCode(&pTraceData->m_dhHead.m_nPrivateKey.m_pCode))
			return -1;
		// �������ݻ������������ͬ�İ�
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

				InsertArray(pStockTick); // ��StockTick������ӵ��ֱʽṹ������

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
		// ͨ���õ������� ���µ�Ԫ��
		UpdateCell();
		m_bIsShow = TRUE;
		CalCulateSize();
		
		InValidate(NULL, FALSE);
	}
	else if(pData->m_nType == RT_AUTOPUSH_EXT)
	{
		TRACE("�ɽ���ϸ�յ� RT_AUTOPUSH_EXT \n");
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
				
					InsertArray(pStockTick); // ��StockTick������ӵ��ֱʽṹ������
					
					delete pStockTick;		
					break;
				}
				pRealData = pNextData;		
			}
			// ͨ���õ������� ���µ�Ԫ��
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
	// ��StockTick�м���� ����������
	long lBuyCount(0), lSellCount(0); // ����������
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
				lBuyCount = pStockTick->m_lCurrent; // ���һֱ�Ȳ����� ����Ϊ������
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
			lBuyCount = pStockTick->m_lCurrent;   // ���һֱ�Ȳ����� ����Ϊ������
	}
	m_lPreSellPrice = pStockTick->m_lSellPrice;
	m_lPreBuyPrice = pStockTick->m_lBuyPrice;
	//////////////////////////////////////////////////////////////////////////
	
	// ����������Ҫ �Ƿ�ü۸��Ѿ�����
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
	{// ����Ѿ�����
		pExitPriceData->m_lBuyVolume  += lBuyCount;
		pExitPriceData->m_lSellVolume += lSellCount;
		pExitPriceData->m_lTurnover ++; // �ü�λ�ϱ�����һ
	}
	else
	{// ���µļ۸�
		CFenPriceData* pPriceData = new CFenPriceData;
		pPriceData->m_lPrice = pStockTick->m_lNewPrice;
		pPriceData->m_lBuyVolume = lBuyCount;
		pPriceData->m_lSellVolume = lSellCount;
		pPriceData->m_lTurnover = 1;

		int i(0);
		for (i = 0; i < m_ayFenPriceData.GetCount(); i++)
		{// һֱ���ҵ��ȵ�ǰ�۸�С����Ϊֹ
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
	// ��ȡ��Դ�ַ���
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
	int nRowCount = m_ayFenPriceData.GetSize() + 1; // ��ǰ�������� + �̶���
	int nCellIndex(0);
	SetCellCount(nRowCount * FENPRICE_COL_COUNT, TRUE);

	//////////////////////////////////////////////////////////////////////////
	// ��һ�� �ּ۱�(���)
	::LoadString(instance, IDS_STRING_FENJIABIAO, buf, nBufLenth);
	str = buf;
	CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_TITLE, ITEM_TEXT_CENTER, CELL_BORDER_BOTTOM, 0, nCellIndex++);
	for (int i = 0; i < nRowCount - 1; i++)
	{
		str.Format("%d", i + 1);
		CreateCell(str, TEXT_COLOR_FIXEDNAME, TEXT_FONT_DYNAMICAL, ITEM_TEXT_RIGHT, CELL_TIP, 0, nCellIndex++);
	}

	//////////////////////////////////////////////////////////////////////////
	// �ڶ��� �ɽ���
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
	// ������ �ɽ���
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
	// ������ ����
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
	// ����������
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
	// ����
	CString strTitle;
	strTitle.Format("�ּ۱�%d", nRow+1);

	// ��һ�� �ɽ���
	CGeneralHelper::LongToString(pPriceData->m_lPrice, m_stkInfo.m_lPrevClose, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal, strText);
	str.Format("�ɽ���:%s \r\n", strText);

	// �ڶ��� �ɽ���
	CGeneralHelper::LongToString(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume, 0, m_stkInfo.m_nHandNum, 0, strText);
	str.AppendFormat("�ɽ���:%s \r\n", strText);

	// ������ ����
	double dValue = (double)(pPriceData->m_lBuyVolume + pPriceData->m_lSellVolume) * 100 / (double)(m_lTotalVol);
	CGeneralHelper::LongToString(dValue, 0, 0, 2, strText, LTOA_MASK_SHOW_PERSENT);
	str.AppendFormat("  ����:%s \r\n", strText);

	// ������ ����
	str.AppendFormat("  �ɽ�: %d", pPriceData->m_lTurnover);

	return str;
}