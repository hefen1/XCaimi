#include "stdafx.h"
#include "TechKlineMgr.h"
#include "DrawTechLine.h"
#include "DrawTStockTrend.h"
#include "TechKLineImpAdd.h"
#include "..\..\..\Common\WinnerApplication.h"
#include "..\..\..\Common\KeyBoardStruct.h"
#include "..\..\Quote\QuoteComm\QuoteBaseDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SPLIT_COLUMN  2
#define IDT_DELAY_DELETELINE 100
//////////////////////////////////////////////////////////////////////////
//CTechKLineMgr
CTechKLineMgr::CTechKLineMgr()
{
	m_pRegion = NULL;
	m_bVisable = TRUE;
	m_stkInfo.SetEmpty();

	m_nKLineType = PKLINE_TYPE_FENSHI;

	m_pFormulaMan = (IFormulaMan *)CWinnerApplication::GetObject(OBJ_FORMULAMAN);
	m_pDataSourceEx = (IDataSourceEx*)CWinnerApplication::GetObject(OBJ_HSDATAENGINE);
	m_pBlockManager = (IBlockManager*)CWinnerApplication::GetObject(OBJ_BLOCKMANAGER);

	m_pDrawBox = NULL;
	m_pDrawBottom = NULL;
	m_pDrawInfo = NULL;
	m_lDate = 0;
	m_nSelDataIndex = 0;
	m_ptMouseFocus = CPoint(0, 0);
	m_ptCursorLine = CPoint(0, 0);
	m_wPeriodType = PERIOD_TYPE_DAY;

	m_nEndPos = m_nStartPos = 0;
	m_lDataVauleLen = 0;
	m_pDialogHisTrend = NULL;

	m_tabNotify.SetNotifyMgr(this, &m_bottomTab);
	m_bottomTab.SetTechKLineMgr(this);
	m_bottomTab.SetNotify(&m_tabNotify);
	m_bottomTab.SetStyle(TS_BOTTOM | TS_TABSIZEADAPTIVE);

	m_pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	if (m_pQuoteLang != NULL)
	{
		m_strAppName = m_pQuoteLang->LoadStr(IDS_APP_NAME);
		m_strYPerMsg = m_pQuoteLang->LoadStr(IDS_YPER_MSG);
		m_strPosChangeMsg = m_pQuoteLang->LoadStr(IDS_POSCHANGED_MSG);
	}
	m_pDrawInfo = NULL;
	m_pPostionCtrlMgr = NULL;
	m_bPosChanged = FALSE;
	m_nDelayDeleteTimerID = 0;
	m_ayChuQuanData.RemoveAll();
	m_dataFenshi.SetTechMgr(this);
}

CTechKLineMgr::~CTechKLineMgr()
{
	if (m_pDialogHisTrend != NULL && m_pDialogHisTrend->m_hWnd != NULL && ::IsWindow(m_pDialogHisTrend->m_hWnd))
	{
		m_pDialogHisTrend->DestroyWindow();
	}
	while(m_listKLine.size() > 0)
	{
		CTechKLineImp* pLine = m_listKLine.back();
		m_listKLine.pop_back();
		if(pLine != NULL)
		{
			delete pLine;
			pLine = NULL;
		}
	}
	while(m_listFenshi.size() > 0)
	{
		CTechKLineImp* pLine = m_listFenshi.back();
		m_listFenshi.pop_back();
		if(pLine != NULL)
		{
			delete pLine;
			pLine = NULL;
		}
	}
	if(m_pDrawBox != NULL)
	{
		delete m_pDrawBox;
		m_pDrawBox = NULL;
	}
	if (m_pDrawBottom != NULL)
	{
		delete m_pDrawBottom;
		m_pDrawBottom = NULL;
	}
	if (m_pDrawInfo != NULL)
	{
		delete m_pDrawInfo;
		m_pDrawInfo = NULL;
	}
	if (m_pPostionCtrlMgr != NULL)
	{
		delete m_pPostionCtrlMgr;
		m_pPostionCtrlMgr = NULL;
	}
	ClearOldChuquanData();
}

//////////////////////////////////////////////////////////////////////////
BOOL CTechKLineMgr::Create(IDrawBaseParam *pParam)
{
	if (pParam != NULL)
	{
		CString strDefault = pParam->GetStringParam(_T("OpenDefault"), _T(""));
		if (!strDefault.IsEmpty())
		{
			short nKLineType = PKLINE_TYPE_FENSHI;
			if (strDefault == PKLINE_NAME_K)
			{
				nKLineType = PKLINE_TYPE_K;
			}
			m_nKLineType = nKLineType;
		}
	}
	if (m_pPostionCtrlMgr == NULL)
	{
		m_pPostionCtrlMgr = new CDrawUnitPosCtrlMgr(this, m_pRegion);
		if (m_pPostionCtrlMgr != NULL)
		{
			m_pPostionCtrlMgr->Init();
		}
	}

	ReCreate(pParam);
	return TRUE;
}

void CTechKLineMgr::ReCreate(IDrawBaseParam *pParam)
{
	m_ptCursorLine = CPoint(-1, -1);
	m_nSelDataIndex = 0;
	m_lDataVauleLen = 0;
	if (m_nKLineType == PKLINE_TYPE_K)
	{
		if (m_listKLine.size() <= 0)
		{
			m_wPeriodType = PERIOD_TYPE_DAY;
			float fHorizCellWidth = m_dataKLine.GetKLineHorizCellWidth();
			if (m_pPostionCtrlMgr != NULL)
			{
				m_pPostionCtrlMgr->CreateLines(m_nKLineType, &m_listKLine, fHorizCellWidth);
			}

			if(m_pDrawBottom == NULL)
			{
				m_pDrawBottom = new CDrawBottom(this, m_pRegion);
			}
			DiagramInValidate();
		}
		if (m_pDrawBottom != NULL)
		{
			m_pDrawBottom->SetVisible(TRUE);
			m_pDrawBottom->UpdatePeriodType(PERIOD_TYPE_DAY);
		}
		CTechKLineImp* pImpTop = GetTopLine();
		if (pImpTop != NULL)
		{
			m_nRequestPeriod = pImpTop->GetMaxHorizCellNum();
		}
		g_hsGraphicsData.AddStyle(GL_DayHorAxes);
	}
	else if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		if(m_pDrawBottom != NULL)
		{
			m_pDrawBottom->SetVisible(FALSE);
		}
		m_nRequestPeriod = m_dataFenshi.m_nCurrentDataLen;
		if (m_nRequestPeriod <= 241)
		{
			m_nRequestPeriod = 241;
		}
		if (m_listFenshi.size() <= 0)
		{
			if (m_pPostionCtrlMgr != NULL)
			{
				m_pPostionCtrlMgr->CreateLines(m_nKLineType, &m_listFenshi, m_nRequestPeriod);
			}
			DiagramInValidate();
		}
		CTechKLineImp* pImpTop = GetTopLine();
		if (pImpTop != NULL)
		{
			m_nRequestPeriod = pImpTop->GetHorizCellNum();
		}
		m_dataFenshi.Init(&m_stkInfo, m_nRequestPeriod, FALSE);
		g_hsGraphicsData.RemoveStyle(GL_DayHorAxes);
	}
	if(m_pDrawBox == NULL)
	{
		m_pDrawBox = new CDrawBox(this, m_pRegion);
	}
	if (m_pDrawBox != NULL)
	{
		m_pDrawBox->ReCreate(m_nKLineType);
		m_pDrawBox->SetVisible(FALSE);
	}
	SetChildRect();
}

void CTechKLineMgr::SetRegionNode(IRegionNode* pNode)
{
	m_pRegion = pNode;
	if (m_pDrawInfo == NULL)
	{
		IHsInfo* pInfo = (IHsInfo*)CWinnerApplication::GetObject(OBJ_INFO);
		m_pDrawInfo = new CDrawInfo(this, pInfo, m_pRegion);
	}
}

void CTechKLineMgr::SetChildRect()
{
	if (m_rect.IsRectEmpty())
	{
		return;
	}
	BOOL bResetUnitRect = FALSE;
	if(m_pPostionCtrlMgr != NULL)
	{
		CRect rcTotal = m_rect;
		rcTotal.bottom -= TAB_ITEM_HEIGHT;
		if (m_nKLineType == PKLINE_TYPE_K)
		{
			if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
			{
				CRect rcBottom = CRect(rcTotal.left, rcTotal.bottom - DRAWBOTTOM_HEIGHT, rcTotal.right, rcTotal.bottom);
				m_pDrawBottom->SetRect(rcBottom);
			}
			rcTotal.bottom -= DRAWBOTTOM_HEIGHT;
		}
		if (rcTotal.Height() < 10)
		{
			return;
		}
		if (!rcTotal.IsRectEmpty())
		{
			bResetUnitRect = (m_pPostionCtrlMgr->OnResetRect(rcTotal) > 0);
		}
		
	}
	CRect rc = m_rect;
	CRect rcTab = CRect(rc.left, rc.bottom - TAB_ITEM_HEIGHT, rc.right, rc.bottom);
	m_bottomTab.SetRect(rcTab);
	m_bottomTab.ReCalTabItemSize();
	rc.bottom -= TAB_ITEM_HEIGHT;

	CTechKLineImp* pTechLineTop = GetTopLine();
	CRect rcTop(0, 0, 0, 0);
	int nCellNum = 0;
	if (m_pPostionCtrlMgr != NULL )
	{
		m_pPostionCtrlMgr->Update(m_nKLineType == PKLINE_TYPE_K ? m_rect.Height() - TAB_ITEM_HEIGHT - DRAWBOTTOM_HEIGHT : m_rect.Height() - TAB_ITEM_HEIGHT, m_rect.Width());
	}
	if (m_pDrawBox != NULL && pTechLineTop != NULL)
	{
		nCellNum = pTechLineTop->GetHorizCellNum();
		rcTop = pTechLineTop->GetMarginRect();
		m_pDrawBox->SetRect(CRect(m_rect.left + 6, rcTop.top + 8, rcTop.left, rcTop.top + 260));
	}
	if (m_pDrawInfo != NULL && pTechLineTop != NULL)
	{
		m_pDrawInfo->SetRect(pTechLineTop->GetRect());
	}
	if (nCellNum > 0)
	{
		m_nEndPos = m_nStartPos + nCellNum;
		if (m_nEndPos >= m_lDataVauleLen)
		{
			m_nEndPos = m_lDataVauleLen;
			m_nStartPos = m_nEndPos - nCellNum;
			if (m_nStartPos < 0)
			{
				m_nStartPos = 0;
			}
		}
	}

}

BOOL CTechKLineMgr::SetRect(CRect rc)
{
	m_rect = rc;
	SetChildRect();
	return TRUE;
}

CRect CTechKLineMgr::GetRect()
{
	return m_rect;
}

void CTechKLineMgr::SetVisible(BOOL bVisible)
{
	m_bVisable = bVisible;
}

BOOL CTechKLineMgr::GetVisible()
{
	return m_bVisable;
}

void CTechKLineMgr::UpdateCodeInfo(StockUserInfo* pCodeInfoNew)
{
	if (memcmp(&pCodeInfoNew->m_ciStockCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo)) != 0)
	{
		m_dataKLine.m_bNeedRefresh = TRUE;	//K线数据需要刷新
		ClearOldChuquanData();
	}
	m_stkInfo.Copy(pCodeInfoNew);
	if (m_ayChuQuanData.GetSize() <= 0 && m_pDataSourceEx != NULL)
	{
		CArray<CodeInfo> arInfo;
		CArray<CurrentExRightData> ayExrightData;
		CodeInfo info(m_stkInfo.m_ciStockCode.m_cCodeType, m_stkInfo.m_ciStockCode.m_cCode);
		arInfo.Add(info);
		int nRet = m_pDataSourceEx->HSDataSourceEx_GetStocksExrightData(arInfo, ayExrightData);
		int nSize = ayExrightData.GetSize();
		ASSERT(nSize == nRet);
		if (nSize > 0)
		{
			CurrentExRightData pExRightData = ayExrightData.GetAt(0);
			int nCount = pExRightData.GetCount();
			for (int i = 0; i < nCount; i ++)
			{
				HSExRightItem* pItem = pExRightData.GetExRightDataItem(i);
				if (pItem != NULL)
				{
					ChuQuanData* pChuquanData = new ChuQuanData;
					pChuquanData->m_cMask = ChuQuanData::Calc;
					pChuquanData->m_lDate = pItem->m_nTime;		        //除权日

					pChuquanData->m_fSGBL = pItem->m_fGivingStock;		//送股比率
					pChuquanData->m_fPGBL = pItem->m_fPlacingStock;		//配股比率
					pChuquanData->m_fPGJ  = pItem->m_fGivingPrice;		//配股价
					pChuquanData->m_fXJHL = pItem->m_fBonus;	    	//现金红利
					//相乘的数据
					pChuquanData->m_fMulit = 1 + pChuquanData->m_fSGBL + pChuquanData->m_fPGBL;
					if( pChuquanData->m_fMulit == 0 )
						pChuquanData->m_fMulit = 1;
					//相加的数据
					pChuquanData->m_fAdd   = 0 - pChuquanData->m_fXJHL + pChuquanData->m_fPGBL * pChuquanData->m_fPGJ;

					m_ayChuQuanData.Add(pChuquanData);
				}
			}
			pExRightData.Release();
		}
	}
	if(m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		CTechKLineImp* pImpTop = GetTopLine();
		int nCellNum = 241;
		if (pImpTop != NULL)
		{
			nCellNum = pImpTop->GetHorizCellNum();
		}
		m_dataFenshi.Init(&m_stkInfo, nCellNum, FALSE);
	}
	if (m_pDialogHisTrend != NULL && m_pDialogHisTrend->m_hWnd != NULL && ::IsWindow(m_pDialogHisTrend->m_hWnd))
	{
		m_pDialogHisTrend->ShowWindow(SW_HIDE);
		m_pDialogHisTrend->UpdateStock(&m_stkInfo);
	}
	if (m_pDrawBox != NULL)
	{
		m_pDrawBox->SetVisible(FALSE);
	}
	if (m_pDataSourceEx != NULL)
	{
		CArray<StockType> asType;
		unsigned short nMarketype = m_stkInfo.m_ciStockCode.m_cCodeType;
		long lDate, lTime;
		BOOL bRet = m_pDataSourceEx->HSDataSourceEx_GetServerDataTime(lDate, lTime);
		int nRet = m_pDataSourceEx->HSDataSourceEx_GetSystemMarketInfo(&nMarketype, 1, asType);
		if (bRet && nRet > 0 && nRet == asType.GetSize())
		{
			StockType stocktype = asType.GetAt(0);
			memcpy(&m_stockType, &stocktype, sizeof(StockType));
			if(m_pDialogHisTrend != NULL)
			{
				m_pDialogHisTrend->SetStockType(&m_stockType);
			}
		}
		m_lDate = lDate;

		memset(&m_curFinance, 0, sizeof(CurrentFinanceData));
		m_lFinaceHand = 0;
		CArray<CodeInfo> ayCode;
		ayCode.Add(m_stkInfo.m_ciStockCode);
		CArray<CurrentFinanceData>ayFin;

		nRet = m_pDataSourceEx->HSDataSourceEx_GetStocksFinanceData(ayCode, ayFin);
		if(nRet > 0)
		{
			int nSize = ayFin.GetSize();
			ASSERT(nRet == nSize);
			memcpy(&m_curFinance, &ayFin.GetAt(nSize - 1), sizeof(CurrentFinanceData));
			m_lFinaceHand = m_curFinance.m_fFinanceData[8];
		}
	}
}

StockUserInfo* CTechKLineMgr::GetCodeInfo()
{
	return &m_stkInfo;
}

short CTechKLineMgr::GetDrawType()
{
	return 0;
}

BOOL CTechKLineMgr::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	CRect rcTab = m_bottomTab.GetRect();
	if (rcTab.PtInRect(point))
	{
		m_bottomTab.OnLButtonDown(nFlags, point);
		DiagramInValidate(rcTab);
	}
	else
	{
		if (m_pDrawInfo != NULL)
		{
			if(m_pDrawInfo->MouseDown(nFlags, point, pData))
				return TRUE;
		}
		if (m_pPostionCtrlMgr != NULL)
		{
			if(m_pPostionCtrlMgr->MouseDown(nFlags, point))
			{
				m_bPosChanged = TRUE;
				DiagramInValidate();
				return TRUE;
			}
		}
		if (m_pDrawBox != NULL)
		{
			if(m_pDrawBox->MouseDown(nFlags, point, pData))
				return TRUE;
		}
		if(m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
		{
			if(m_pDrawBottom->MouseDown(nFlags, point, pData))
				return TRUE;
		}
		std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
		if (pList != NULL && pList->size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL && pImpCur->GetVisible())
				{
					BOOL bRet = pImpCur->MouseDown(nFlags, point, pData);
					if (bRet)
					{
						return TRUE;
					}
				}
			}
		}

		HitTest(point);
		m_ptMouseFocus = point;
	}
	return FALSE;
}

BOOL CTechKLineMgr::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	CRect rcTab = m_bottomTab.GetRect();
	if (rcTab.PtInRect(point))
	{
		m_bottomTab.OnLButtonDown(nFlags, point);
		DiagramInValidate(rcTab);
	}
	else
	{
		if (m_nKLineType == PKLINE_TYPE_K)
		{
			CTechKLineImp* pImpTop = GetTopLine();
			if (pImpTop != NULL)
			{
				BOOL bSel = pImpTop->MouseDBDown(nFlags, point, pData);
				if(bSel)
				{
					ShowHisTrend();
				}
			}
		}
		ChangeType();
		m_ptMouseFocus = point;
	}
	return FALSE;
}

BOOL CTechKLineMgr::MouseUp(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	m_ptMouseFocus = point;
	return FALSE;
}

int	CTechKLineMgr::HitTest(CPoint point)
{
	CPoint ptCursorLineOld = m_ptCursorLine;
	CTechKLineImp* pImplSel = NULL;
	if (m_pDrawBox != NULL )
	{
		m_pDrawBox->SetVisible(TRUE);
		std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
		if (pList != NULL && pList->size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL && pImpCur->GetVisible())
				{
					m_nSelDataIndex = pImpCur->HitTest(point);
					if (m_nSelDataIndex >= 0)
					{
						pImplSel = pImpCur;
						break;
					}
				}
			}
		}

		if (pImplSel != NULL)
		{
			if (m_nSelDataIndex <= m_lDataVauleLen && m_lDataVauleLen > 0)
			{
//				m_ptCursorLine.y = point.y;
				m_ptCursorLine.x = pImplSel->GetDataXPos(m_nSelDataIndex);
				m_ptCursorLine.y = pImplSel->GetInMarginYPos(point.y);

				m_nSelDataIndex += m_nStartPos;
				if (m_nSelDataIndex > m_lDataVauleLen - 1)
				{
					m_nSelDataIndex = m_lDataVauleLen - 1;
				}
				pImplSel->GetTipText(&m_sMouseTipTextInfo, m_nSelDataIndex, point);
				m_sMouseTipTextInfo.m_pPoint = &m_ptCursorLine;

				if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					CString strDate, strTime;
					m_dataFenshi.GetPosTime(m_nSelDataIndex, strDate, FALSE);
					if (strDate.IsEmpty() || strDate == "0")
					{
						strDate.Format("%d", m_lDate);
					}
					GetTimeByPos(m_nSelDataIndex, strTime, 5);
					m_pDrawBox->SetDateTime(strDate.Right(4), strTime);
					m_pDrawBox->UpdateData(&m_stkInfo, &m_dataFenshi.m_pPriceValues[m_nSelDataIndex], &m_dataFenshi.m_pAvePriceValues[m_nSelDataIndex], &m_dataFenshi.m_pChengJiaoValues[m_nSelDataIndex]);
				}
				else if (m_nKLineType == PKLINE_TYPE_K)
				{
					WORD nPeriodType = PERIOD_TYPE_DAY;
					if (m_pDrawBottom != NULL)
					{
						nPeriodType = m_pDrawBottom->GetPeriodType();
					}
					long lPreClose = m_stkInfo.m_lPrevClose;
					StockDay* pDayData = m_dataKLine.GetCurrentDrawData();
					if (m_nSelDataIndex >= 0 && pDayData != NULL && m_nSelDataIndex <= m_dataKLine.m_nCurrentDataLen)
					{
						if (m_nSelDataIndex > 0)
						{
							lPreClose = pDayData[m_nSelDataIndex - 1].m_lClosePrice;
						}
						else
						{
							lPreClose = 0;
						}
						m_pDrawBox->UpdateData(&pDayData[m_nSelDataIndex], &m_stkInfo, lPreClose, 
							m_lFinaceHand * 100, m_sMouseTipTextInfo.m_strY);
					}
				}
			}
			if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
			{
				if (m_nSelDataIndex == 0 || m_nSelDataIndex == m_nStartPos)
				{
					point.x = GetTopLine()->GetMarginRect().left;
				}
				m_pDrawBottom->UpdatePositon(m_nSelDataIndex, point);
			}
			if (m_ptCursorLine != ptCursorLineOld)
			{
				DiagramInValidate();
				return 1;
			}
		}
		else //没有选中的图元
		{
			if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
			{
				m_pDrawBottom->HitTest(point);
			}
			if (m_pDrawBox != NULL )//&& m_pDrawBox->GetVisible())
			{
				m_pDrawBox->SetVisible(FALSE);
			}
			DiagramInValidate();
		}
	}
	if (m_ptCursorLine == CPoint(-1, -1))
	{
		if (m_pDrawBox != NULL )//&& m_pDrawBox->GetVisible())
		{
			m_pDrawBox->SetVisible(FALSE);
		}
		DiagramInValidate();
	}

	return 0;
}

BOOL CTechKLineMgr::MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if (m_pDrawInfo != NULL)
	{
		if(m_pDrawInfo->MouseMove(nFlags, point, pData))
			return TRUE;
	}
	if (m_pPostionCtrlMgr != NULL)
	{
		if(m_pPostionCtrlMgr->MouseMove(nFlags, point))
			return TRUE;
	}
	if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
	{
		m_pDrawBottom->HitTest(point);
	}
	if(nFlags & MK_LBUTTON)
	{
		HitTest(point);
	}
	m_ptMouseFocus = point;
	return FALSE;
}


BOOL CTechKLineMgr::RButtonDown(UINT nFlags, CPoint point, HWND hWndParent, hsRefMouseMsg* pData/* = NULL*/)
{
	BOOL bInMarginRect = FALSE;
	CTechKLineImp* pImpSelect = NULL;
	std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
	if (pList != NULL && pList->size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur->GetVisible())
			{
				CRect rcVarTitle = pImpCur->GetVarTitleRect();
				if (!rcVarTitle.IsRectEmpty() && rcVarTitle.PtInRect(point))
				{
					pImpSelect = pImpCur;
					break;
				}
			}
		}
	}

	int nMenuSelect = 2;
	BOOL bMain = FALSE;
	if (pImpSelect != NULL)
	{
		bMain = pImpSelect->IsMainLine();
		if (etechLineType_stock & pImpSelect->GetLineType())
		{
			nMenuSelect = 1;
		}
		else if(etechLineType_tech & pImpSelect->GetLineType())
		{
			nMenuSelect = 0;
		}
		else if (bMain && m_nKLineType == PKLINE_TYPE_K)
		{
			nMenuSelect = 1;
		}
	}
	if ( m_pRegion != NULL)
	{
		CWnd* pWnd = GetParentWnd();
		CMenu menuCur;
		if(m_pQuoteLang != NULL)
		{
			HMENU hMenu = m_pQuoteLang->LoadMenu(IDR_MENU_TECHKLINE_RBUTTON);
			if (hMenu != NULL)
			{
				menuCur.Attach(::GetSubMenu(hMenu, nMenuSelect));
			}
		}
		if (bMain && menuCur.m_hMenu)
		{
			menuCur.DeleteMenu(IDR_TECHKLINE_DELETE, MF_BYCOMMAND);
			menuCur.DeleteMenu(ID_SEPARATOR, MF_BYCOMMAND);
		}
		if (m_nKLineType == PKLINE_TYPE_FENSHI)
		{
			menuCur.DeleteMenu(IDR_TECHKLINE_FORWARDFUQUAN, MF_BYCOMMAND);
			menuCur.DeleteMenu(IDR_TECHKLINE_BACKWARDFUQUAN, MF_BYCOMMAND);
			menuCur.DeleteMenu(IDR_TECHKLINE_NOFUQUAN, MF_BYCOMMAND);
			int nCount = menuCur.GetMenuItemCount();
			if (nCount > 0)
			{
				menuCur.DeleteMenu(nCount - 1, MF_BYPOSITION);
			}
		}
		if ( pWnd != NULL && menuCur.m_hMenu)
		{
			menuCur.CheckMenuItem(IDR_TECHKLINE_FORWARDFUQUAN, ((m_dataKLine.GetChuquanMask() & HsTechLine_Left_ChuQuan) ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND);
			menuCur.CheckMenuItem(IDR_TECHKLINE_BACKWARDFUQUAN, ((m_dataKLine.GetChuquanMask() & HsTechLine_Back_ChuQuan) ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND);
			menuCur.CheckMenuItem(IDR_TECHKLINE_NOFUQUAN, ((m_dataKLine.GetChuquanMask() == 0) ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND);
			CPoint pt;
			::GetCursorPos(&pt); 
			int nRet = menuCur.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, pWnd);

			if (nRet == IDR_TECHKLINE_MAIRU || nRet == IDR_TECHKLINE_MAICHU)
			{
				CDrawBaseParam param;

				CString strPrice = m_dataFenshi.GetLastNewPrice(FALSE);
				param.AddParam(PARAM_QUICKTRADE_PRICE, strPrice);
				param.AddParam(PARAM_QUICKTRADE_CODE, m_stkInfo.GetCode());

				m_pRegion->SendLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE, &param, (nRet == IDR_TECHKLINE_MAIRU) ? PN_QUICKTRADE_BUY : PN_QUICKTRADE_SELL);
			}
			else if (nRet == ID_BLOCK_ADSCRIPTION)
			{
				CString strCodeType(""), strCode("");
				strCodeType.Format("%d", m_stkInfo.m_ciStockCode.m_cCodeType);
				strCode = m_stkInfo.m_ciStockCode.GetCode();
				CDrawBaseParam param;
				param.AddParam(PARAM_BLOCK_ADCRIPTION_CODETYPE, strCodeType);
				param.AddParam(PARAM_BLOCK_ADCRIPTION_CODE, strCode);
				m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_BLOCK_ADSCRIPTION, &param, PN_BLOCKADSCRIPTION);
			}
			else if(IDR_TECHKLINE_STOCKINFO == nRet)
			{
				CString strMsgID("");
				CDrawBaseParam param;
				strMsgID.Format("%d", DiagramMsg_OpenLayout);
				param.AddParam(PARAM_DIAGRAMMSGID, strMsgID);         
				param.AddParam(PARAM_DIAGRAM_LAYOUT_NAME, Layout_F10Info);
				CString strCodeType(""), strCode("");
				strCodeType.Format("%d", m_stkInfo.m_ciStockCode.m_cCodeType);
				strCode = m_stkInfo.m_ciStockCode.GetCode();
				param.AddParam(PARAM_BLOCK_ADCRIPTION_CODETYPE, strCodeType);
				param.AddParam(PARAM_BLOCK_ADCRIPTION_CODE, strCode);
				m_pRegion->SendLinkageMsg(QUOTE_MESSAGE_NOTIFY_DIAGRAM_LAYOUT, &param);
			}
			else if (nRet >= IDR_TECHKLINE_SELECTINDEX && nRet <= IDR_TECHKLINE_FINDINDEX)
			{
				if(m_pFormulaMan != NULL)
				{
					DWORD dwIndexType = hxOpenFindDataDlg;
					m_pFormulaMan->OpenExpWindow(_T(""), Tech, dwIndexType);
				}
			}
			else if (ID_ADD_TO_SELFBLOCK == nRet)
			{
				if (m_pBlockManager != NULL)
				{
					m_pBlockManager->AddStockToBlock(m_pQuoteLang->LoadStr(IDS_SELF_BLOCK), &m_stkInfo.m_ciStockCode);
				}
			}
			else if (IDR_TECHKLINE_DELETE == nRet)
			{
				if (pImpSelect != NULL)
				{
					CRect rcInvalidate = pImpSelect->GetRect();
					DeleteLine(pImpSelect);
					DiagramInValidate(rcInvalidate);
				}
			}
			else if (nRet >= IDR_TECHKLINE_KLINE && nRet <= IDR_TECHKLINE_FORK)
			{
				if (pImpSelect != NULL)
				{
					CRect rcInvalidate = pImpSelect->GetRect();
					if (nRet == IDR_TECHKLINE_KLINE)//K线图
					{
						pImpSelect->SetChartType(IDR_hexinKLineChart);
					}
					else if (nRet == IDR_TECHKLINE_AMIRICA)//美国线
					{
						pImpSelect->SetChartType(IDR_hexinAmericaChart);
					}
					else if (nRet == IDR_TECHKLINE_PAGODA)//宝塔线
					{
						pImpSelect->SetChartType(IDR_hexinTowerChart);
					}
					else if (nRet == IDR_TECHKLINE_COMPRESS)//压缩线
					{
						pImpSelect->SetChartType(IDR_hexinAxisCndeqv);
					}
					else if (nRet == IDR_TECHKLINE_FORK)//圈叉线
					{
						pImpSelect->SetChartType(IDR_hexinOXChart);
					}
					DiagramInValidate(rcInvalidate);
				}
			}
			else if (nRet >= IDR_TECHKLINE_INDEXNOTE && nRet <= IDR_TECHKLINE_ADDNEWINDEX)
			{
				if(m_pFormulaMan != NULL && pImpSelect != NULL)
				{
					DWORD dwIndexType = hxOpenExpress_Notes;
					if (nRet == IDR_TECHKLINE_PARAMMODIFY)
					{
						dwIndexType = hxOpenExpress_Eidolon;
					}
					else if (nRet == IDR_TECHKLINE_MODIFYINDEX)
					{
						dwIndexType = hxOpenExpress_Edit;
					}
					else if (nRet == IDR_TECHKLINE_ADDNEWINDEX)
					{
						dwIndexType = hsOpenExpress_Create;
					}
					CString strName = pImpSelect->GetTeckKLineName();
					if (!strName.IsEmpty())
					{
						m_pFormulaMan->OpenExpWindow(strName, Tech, dwIndexType);
					}
				}
			}
			else if (nRet >= IDR_TECHKLINE_FORWARDFUQUAN && nRet <= IDR_TECHKLINE_NOFUQUAN)
			{		 
				if (m_nKLineType == PKLINE_TYPE_K)
				{
					CalcChuquanMark(nRet);
				}
			}		 
		}
		menuCur.Detach();
		menuCur.DestroyMenu();
	}
	return FALSE;
}

BOOL CTechKLineMgr::RButtonUp(UINT nFlags, CPoint point, HWND hWndParent, hsRefMouseMsg* pData/* = NULL*/)
{
	return FALSE;
}

BOOL CTechKLineMgr::OnCursor(UINT nFlags, CPoint point)
{
	return FALSE;
}

int	CTechKLineMgr::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nRet = 1;
	switch(nChar)
	{
	case VK_LEFT: //前
	case VK_RIGHT: //后
		{
			MoveCursorPos(nChar == VK_RIGHT);
		}
		break;
	case VK_UP: //上
	case VK_DOWN: //下
		{
			m_ptCursorLine = CPoint(-1, -1);
			if (m_nKLineType == PKLINE_TYPE_K)
			{
				float fNewCellWidth = m_dataKLine.GetNextWidth(nChar == VK_UP);
				for (std::list<CTechKLineImp*>::iterator it = m_listKLine.begin(); it != m_listKLine.end(); it ++)
				{
					CTechKLineImp* pImpl = (CTechKLineImp*)*it;
					if (pImpl != NULL && pImpl->GetVisible())
					{
						pImpl->SetHorizCellWidth(fNewCellWidth);
					}
				}
				BOOL bNeedRequest = FALSE;
				CTechKLineImp* pImplTop = m_listKLine.front();
				int nHorizCellNum = 0;
				if (pImplTop != NULL)
				{
					nHorizCellNum = pImplTop->GetHorizCellNum();
					if (nHorizCellNum > 0)
					{
						bNeedRequest = !m_dataKLine.IsKLineDataFinished(nHorizCellNum, 0);
					}
				}
				m_nRequestPeriod = nHorizCellNum;
				m_dataKLine.m_nCurrentRequestLen = m_nRequestPeriod;
				if(bNeedRequest)
				{
					if (m_pRegion != NULL)
					{
						IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
						if (pRequestCollector != NULL)
						{
							GetRequestData(pRequestCollector, &m_stkInfo, FALSE);
						}
						std::list<CTechKLineImp*> listLines;
						int nSize = 0;
						nSize = GetAddLines(listLines, etechLineType_stock);
						if (nSize > 0)
						{
							for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
							{
								CTechKLineImpAdd* pImp = (CTechKLineImpAdd*)*it;
								if (pImp != NULL)
								{
									pImp->GetRequestData(pRequestCollector, NULL, m_nRequestPeriod, m_wPeriodType);
								}
							}
						}
						m_pRegion->SendRequest(pRequestCollector);
						pRequestCollector->Release();
						listLines.clear();
					}
				}
				else	//从多到少，重新排
				{
					if (m_nEndPos - m_nStartPos < m_dataKLine.m_nCurrentDataLen)
					{
						m_nEndPos = m_dataKLine.m_nCurrentDataLen;
						m_nStartPos = m_nEndPos - nHorizCellNum;;
						if (m_nStartPos < 0)
						{
							m_nStartPos = 0;
						}
					}
					else if(m_nEndPos - m_nStartPos > nHorizCellNum)
					{
						m_nStartPos = m_nEndPos - nHorizCellNum;
						if (m_nStartPos < 0)
						{
							m_nStartPos = 0;
						}
					}
					DiagramInValidate();			
				}
			}
			else if (m_nKLineType == PKLINE_TYPE_FENSHI)
			{
				BOOL bNewRequest = FALSE;
				int nReqDay = m_dataFenshi.m_nLastReqDay;
				if (nChar == VK_DOWN)
				{
					std::list<CTechKLineImp*> listLines;
					int nSize = 0;
					nSize = GetAddLines(listLines, etechLineType_stock);
					BOOL bAddComplete = TRUE;
					if (nSize > 0)
					{
						for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
						{
							CDrawTStockTrendAdd* pImp = (CDrawTStockTrendAdd*)*it;
							if (pImp != NULL)
							{
								if (!pImp->CheckRecvComplete())
								{
									bAddComplete = FALSE;
									break;
								}
							}
						}
					}
					if (bAddComplete && m_dataFenshi.m_nLastReqDay <= m_dataFenshi.m_nLastRecvDay)
					{
						nReqDay ++;
						if (nReqDay > m_dataFenshi.m_nLastRecvDay && nReqDay <= MAX_FENSHI_DAYS)
						{
							m_dataFenshi.m_nLastReqDay = nReqDay;
							bNewRequest = TRUE;
						}
					}
				}
				else if (nChar == VK_UP)
				{
					nReqDay --;
				}
				if(bNewRequest)
				{
					if (m_pRegion != NULL)
					{
						IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
						if (pRequestCollector != NULL)
						{
							GetRequestData(pRequestCollector, &m_stkInfo, FALSE);
						}
						std::list<CTechKLineImp*> listLines;
						int nSize = 0;
						nSize = GetAddLines(listLines, etechLineType_stock);
						if (nSize > 0)
						{
							for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
							{
								CDrawTStockTrendAdd* pImp = (CDrawTStockTrendAdd*)*it;
								if (pImp != NULL)
								{
									pImp->GetRequestData(pRequestCollector, NULL, m_nRequestPeriod, m_dataFenshi.m_nLastReqDay);
								}
							}
						}
						m_pRegion->SendRequest(pRequestCollector);
						pRequestCollector->Release();
						listLines.clear();
					}
				}
				else
				{
					int nDataValueLen = 0;
					int nTrendNum = m_dataFenshi.UpdateFenshiDataPos(nReqDay, nDataValueLen);
					std::list<CTechKLineImp*> listLines;
					int nSize = 0;
					nSize = GetAddLines(listLines, etechLineType_stock);
					if (nSize > 0)
					{
						for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
						{
							CDrawTStockTrendAdd* pImp = (CDrawTStockTrendAdd*)*it;
							if (pImp != NULL)
							{
								pImp->UpdateDataPos(nReqDay, nDataValueLen);
							}
						}
					}

					m_dataFenshi.GetMinDataRequest(NULL, nDataValueLen);
					m_lDataVauleLen = nDataValueLen;
					m_nEndPos = m_lDataVauleLen;
					m_nStartPos = 0;
					if (nTrendNum > 0)
					{
						for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
						{
							CTechKLineImp* pImpl = (CTechKLineImp*)*it;
							if (pImpl != NULL && pImpl->GetVisible())
							{
								pImpl->SetHorizCellNum(nTrendNum);
							}
						}
					}
					DiagramInValidate();
				}
			}
		}
		break;
	case VK_SPACE:
		{
			ShowHisTrend();
		}
		break;
	case VK_RETURN:
	case VK_F5:
		{
			BOOL bNeedChange = TRUE;
			if (nFlags == 1)
			{
				if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					bNeedChange = FALSE;
				}
			}
			else if (nFlags == 2)
			{
				if (m_nKLineType == PKLINE_TYPE_K)
				{
					bNeedChange = FALSE;
				}
			}
			if(bNeedChange)
				ChangeType();
		}
		break;
	case VK_F7:
	case VK_F8:
		{
			if (m_nKLineType == PKLINE_TYPE_K)
			{
				WORD wPeriod = GetNextPageType(nChar == VK_F8);
				if (wPeriod > 0)
				{
					SwitchPage(wPeriod);
					if (m_pDrawBottom != NULL)
					{
						m_pDrawBottom->UpdatePeriodType(wPeriod);
					}
				}
			}
		}
		break;
	case VK_ESCAPE:
		{
			if (m_pDrawBox->GetVisible())
			{
				m_pDrawBox->SetVisible(FALSE);
				nRet = 1;
				DiagramInValidate();
			}
			else
			{
				nRet = 0;
			}
		}
		break;
	case VK_HOME:
	case VK_END:
		{
			int nOldSteps = m_nEndPos - m_nStartPos;
			int nCurrentStep = nOldSteps;
			CTechKLineImp* pImpTop = GetTopLine();
			if (pImpTop != NULL)
			{
				int nHorNum = pImpTop->GetHorizCellNum();
				if (nHorNum > 0)
				{
					nCurrentStep = max(nOldSteps, nHorNum);
				}
			}

			if (nChar == VK_HOME)
			{
				m_nStartPos = 0;
				m_nEndPos = nCurrentStep;
				if (m_nKLineType == PKLINE_TYPE_K)
				{
					if (m_nEndPos > m_dataKLine.m_nCurrentDataLen - m_dataKLine.m_nCurrentDataPos)
					{
						m_nEndPos = m_dataKLine.m_nCurrentDataLen - m_dataKLine.m_nCurrentDataPos;
					}
				}
				else if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					if (m_nEndPos > m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos)
					{
						m_nEndPos = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
					}
				}
				m_nSelDataIndex = 0;
				
			}
			else
			{
				if (m_nKLineType == PKLINE_TYPE_K)
				{
					m_nEndPos = m_dataKLine.m_nCurrentDataLen - m_dataKLine.m_nCurrentDataPos;
				}
				else if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					m_nEndPos = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
				}
				m_nStartPos = m_nEndPos - nCurrentStep;
				if (m_nStartPos < 0)
				{
					m_nStartPos = 0;
				}
				m_nSelDataIndex = (m_nEndPos - m_nStartPos - 1);
			}
			if (pImpTop != NULL)
			{
				CPoint pt;
				pt.x = pImpTop->GetDataXPos(m_nSelDataIndex);
				pt.y = pImpTop->GetDataYPos(m_nSelDataIndex);
				HitTest(pt);
			}
		}
		break;
	default:
		{
			nRet = 0;
		}
		break;
	}
	return nRet;
}

WORD CTechKLineMgr::GetNextPageType(BOOL bNext)
{
	WORD wPeriod = m_wPeriodType;
	switch(m_wPeriodType)
	{
	case PERIOD_TYPE_MINUTE0:
		{
			if(bNext)
				wPeriod = PERIOD_TYPE_MINUTE1;
			break;
		}
	case PERIOD_TYPE_MINUTE1:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MINUTE5;
			else
				wPeriod = PERIOD_TYPE_MINUTE0;
			break;
		}
	case PERIOD_TYPE_MINUTE5:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MINUTE15;
			else
				wPeriod = PERIOD_TYPE_MINUTE1;
			break;
		}
	case PERIOD_TYPE_MINUTE15:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MINUTE30;
			else
				wPeriod = PERIOD_TYPE_MINUTE5;
			break;
		}
	case PERIOD_TYPE_MINUTE30:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MINUTE60;
			else
				wPeriod = PERIOD_TYPE_MINUTE15;
			break;
		}
	case PERIOD_TYPE_MINUTE60:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MINUTE120;
			else
				wPeriod = PERIOD_TYPE_MINUTE30;
			break;
		}
	case PERIOD_TYPE_MINUTE120:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_DAY;
			else
				wPeriod = PERIOD_TYPE_MINUTE60;
			break;
		}
	case PERIOD_TYPE_DAY:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_WEEK;
			else
				wPeriod = PERIOD_TYPE_MINUTE120;
			break;
		}
	case PERIOD_TYPE_WEEK:
		{
			if (bNext)
				wPeriod = PERIOD_TYPE_MONTH;
			else
				wPeriod = PERIOD_TYPE_DAY;
			break;
		}
	case PERIOD_TYPE_MONTH:
		{
			if(!bNext)
				wPeriod = PERIOD_TYPE_WEEK;
			break;
		}
	default:
		break;
	}
	return wPeriod;
}

BOOL CTechKLineMgr::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData || wParam == HSCurKeyInfo::KeyStockAdd)
	{
		CTechKLineImp* pImpTop = GetTopLine();
		if (pImpTop == NULL)
		{
			return FALSE;
		}

		CRect rcAdd = pImpTop->GetRect();
		ExpPropery* pExp = NULL;
		StockUserInfo* pStockAdd = NULL;
		if (wParam == HSCurKeyInfo::UpdateExpressData)
		{
			pExp = (ExpPropery*)lParam;
		}
		if (wParam == HSCurKeyInfo::KeyStockAdd)
		{
			pStockAdd = (StockUserInfo*)lParam;
		}
		if(pExp != NULL || pStockAdd != NULL)
		{
			int nRet = 0;
			if (pExp && (HS_EXP_MAINCHART & pExp->m_bStyle))
			{
				nRet = ModifyTechLine(pExp);
			}
			else if (pStockAdd != NULL)
			{
				nRet = ModifyTechLine(pStockAdd);
			}

			if (nRet > 0)
			{
				DiagramInValidate();
			}
			if (m_pPostionCtrlMgr != NULL)
			{
				m_pPostionCtrlMgr->Update(m_nKLineType == PKLINE_TYPE_K ? m_rect.Height() - TAB_ITEM_HEIGHT - DRAWBOTTOM_HEIGHT : m_rect.Height() - TAB_ITEM_HEIGHT, m_rect.Width());
			}			
			return TRUE;
		}
	}
	else if (wParam == HSCurKeyInfo::KeyCommand)
	{
		WORD wPeriod = 0;
		switch(lParam)
		{
		case KeyOneMinute:
			{
				wPeriod = PERIOD_TYPE_MINUTE1;
				break;
			}
		case KeyFiveMinute:
			{
				wPeriod = PERIOD_TYPE_MINUTE5;
				break;
			}
		case Key15Minute:
			{
				wPeriod = PERIOD_TYPE_MINUTE15;
				break;
			}
		case Key30Minute:
			{
				wPeriod = PERIOD_TYPE_MINUTE30;
				break;
			}
		case Key60Minute:
			{
				wPeriod = PERIOD_TYPE_MINUTE60;
				break;
			}
		case KeyDayData:
			{
				wPeriod = PERIOD_TYPE_DAY;
				break;
			}
		case KeyWeekData:
			{
				wPeriod = PERIOD_TYPE_WEEK;
				break;
			}
		case KeyYearData:
			{
				wPeriod = PERIOD_TYPE_MONTH;
				break;
			}

		default:
			break;
		}
		if (wPeriod > 0)
		{
			SwitchPage(wPeriod);
			if (m_pDrawBottom != NULL)
			{
				m_pDrawBottom->UpdatePeriodType(wPeriod);
			}
		}
		return TRUE;
	}

	return FALSE;
}

long CTechKLineMgr::OnMenuMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CTechKLineMgr::DrawCursorLine(CDC* pDC)
{
	if (m_ptCursorLine.x < 0 || m_ptCursorLine.y < 0)
	{
		return ;
	}
	CTechKLineImp* pImplTop = GetTopLine();
	CTechKLineImp* pImplBottom = GetBottomLine();

	if (pImplTop != NULL && pImplBottom != NULL)
	{
		CPen pen(g_hsGraphicsData.clMoveLine.m_nPenStyle, g_hsGraphicsData.clMoveLine.m_nWidth, g_hsGraphicsData.clMoveLine.GetColor());
		CPen *pOldPen = pDC->SelectObject(&pen);
		int nOldRop = pDC->SetROP2(R2_XORPEN);

		CRect rcMarginTop = pImplTop->GetMarginRect();
		CRect rcMarginBottom = pImplBottom->GetMarginRect();
		int nBottom = m_rect.bottom - rcMarginBottom.bottom - 16;
		if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
		{
			nBottom -= DRAWBOTTOM_HEIGHT;
		}
		pDC->MoveTo(m_ptCursorLine.x, m_rect.top + rcMarginTop.top + 1);
		pDC->LineTo(m_ptCursorLine.x, nBottom);

		pDC->MoveTo(m_rect.left + rcMarginTop.left + 1, m_ptCursorLine.y);
		pDC->LineTo(m_rect.right - rcMarginBottom.right, m_ptCursorLine.y);
		pDC->SelectObject(pOldPen);
		pDC->SetROP2(nOldRop);
		pen.DeleteObject();
	}
}

int	CTechKLineMgr::Draw(CDC* pDC)
{
	std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
	if (pList != NULL && pList->size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur->GetVisible())
			{
				pImpCur->Draw(pDC);
			}
		}
	}

	DrawCursorLine(pDC);
	if (m_pDrawBox != NULL && m_pDrawBox->GetVisible())
	{
		m_pDrawBox->Draw(pDC);
	}
	if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
	{
		m_pDrawBottom->Draw(pDC);
	}
	m_bottomTab.Draw(pDC);
	if (m_pDrawInfo != NULL)
	{
		m_pDrawInfo->Draw(pDC);
	}
	return 0;
}

int	CTechKLineMgr::UpdateDrawInfoData(CArray<CDrawInfoUnitData>& ayData, long lDateLast)
{
	BOOL bReCalc = TRUE;
	if (ayData.GetSize() > 0)
	{
		CDrawInfoUnitData dataStart = ayData.GetAt(0);
		CDrawInfoUnitData dataEnd = ayData.GetAt(ayData.GetSize() - 1);

		CPoint ptStart, ptEnd;
		ptStart.x = GetDataXPos(0);
		ptEnd.x = GetDataXPos(m_nEndPos - m_nStartPos);
		if (m_nKLineType == PKLINE_TYPE_K)
		{
			if (m_nStartPos < m_dataKLine.m_nCurrentDataLen)
			{
				if (m_dataKLine.m_pStockDayData[m_nStartPos].m_lDate == dataStart.m_lDateTime && dataStart.m_ptPosition.x == ptStart.x)
				{
					bReCalc = FALSE;
				}
				if (!bReCalc)
				{
					bReCalc = TRUE;
					if (m_dataKLine.m_pStockDayData[m_nEndPos - 1].m_lDate == dataEnd.m_lDateTime && dataEnd.m_ptPosition.x == ptEnd.x)
					{
						bReCalc = FALSE;
					}
				}
			}
		}
	}
	if(bReCalc)
	{
		//先清除掉原有的
		ayData.RemoveAll();
		for (int i = m_nStartPos; i < m_nEndPos; i ++)
		{
			CPoint pt;
			pt.x = GetDataXPos(i - m_nStartPos);
			pt.y = 14;
			if (m_nKLineType == PKLINE_TYPE_K)
			{
				if (i < m_dataKLine.m_nCurrentDataLen)
				{
					CDrawInfoUnitData dataCur;
					dataCur.m_lDateTime = m_dataKLine.m_pStockDayData[i].m_lDate;
					dataCur.m_ptPosition = pt;
					dataCur.m_ptChuquan = pt;

					ayData.Add(dataCur);
				}
			}
			else if(lDateLast == m_lDate && m_nKLineType == PKLINE_TYPE_FENSHI)
			{
				if (i + m_dataFenshi.m_nCurFenshiPos < m_dataFenshi.m_nCurrentDataLen)
				{
					CDrawInfoUnitData dataCur;
					dataCur.m_ptPosition = pt;

					CString str;
					CString strTime = GetTimeByPos(i, str);//GetTimeByPos(i + m_dataFenshi.m_nCurFenshiPos, str);
					if (!strTime.IsEmpty())
					{
						strTime = strTime.Right(4);
						dataCur.m_lDateTime = atol(strTime);
					}
					ayData.Add(dataCur);
				}
			}
		}
	}
	return m_nKLineType;
}

BOOL CTechKLineMgr::GetRequestData(IRequestCollector* pReqCollector, StockUserInfo* pStockInfo, BOOL bForce)
{
	m_ptCursorLine = CPoint(-1, -1);
	if (pReqCollector == NULL || pStockInfo == NULL)
	{
		return FALSE;
	}
	if (bForce)
	{
		m_dataFenshi.Release();
		m_dataKLine.Release();
		m_nStartPos = m_nEndPos = 0;
		m_lDataVauleLen = 0;
	}
	if( !m_stkInfo.m_ciStockCode.CompareCode(&pStockInfo->m_ciStockCode) )
	{//保存请求的股票信息
		m_stkInfo.Copy(pStockInfo);
		ClearOldChuquanData();
	}
	if (m_pDrawInfo != NULL)
	{
		m_pDrawInfo->GetRequestData(pReqCollector, pStockInfo, bForce);
	}

	if (m_nKLineType == PKLINE_TYPE_K)
	{
		if (m_wPeriodType == PERIOD_TYPE_MINUTE0)		//tick
		{
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
			pReqLimitTick->m_nCount = m_nRequestPeriod;
			pReqCollector->AddRequest((char*)pakData, lRetLen);
			delete []pakData;
			//主推
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
		else
		{
			short nCount = sizeof(ReqDayData) / sizeof(CodeInfo);
			if( sizeof(ReqDayData) % sizeof(CodeInfo) )
			{
				nCount++;
			}
			int lAskLen = sizeof(AskData) + sizeof(CodeInfo) * (nCount - 1);
			char* pData = new char[lAskLen]; 
			AskData*  pAskData = (AskData*)pData;
			memset(pAskData, 0, lAskLen);
			pAskData->m_nType  = RT_TECHDATA_EX;
			pAskData->m_nSize  = nCount;
			memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
			memcpy(&pAskData->m_nPrivateKey, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));

			ReqDayData* pRequest = (ReqDayData*)pAskData->m_pCode;
			pRequest->m_nDay = m_nRequestPeriod;
			pRequest->m_lBeginPosition = 0;
			memcpy(&pRequest->m_ciCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
			pRequest->m_nPeriodNum = 1;
			pRequest->m_cPeriod = m_wPeriodType;
			pRequest->m_nSize = 0;
			pAskData->m_lKey = m_wPeriodType;

			pReqCollector->AddRequest((char*)pAskData,lAskLen);
			delete[] pData;
		}
	}
	else if (PKLINE_TYPE_FENSHI == m_nKLineType)
	{
		BOOL bRefresh = m_dataFenshi.GetNeedRefresh(&m_stkInfo);
		if(bRefresh || bForce)
		{
			CTechKLineImp* pImpTop = GetTopLine();
			int nCellNum = 241;
			if (pImpTop != NULL)
			{
				nCellNum = pImpTop->GetHorizCellNum();
			}
			m_dataFenshi.Init(&m_stkInfo, nCellNum, bForce);
		}

		if (m_dataFenshi.m_nLastReqDay >= 1 && m_dataFenshi.m_nLastRecvDay == 0) 
		{
			int lAskLen = sizeof(AskData) + sizeof(CodeInfo);
			AskData* pskData = (AskData*) new char[lAskLen];
			memset(pskData,0,lAskLen);

			pskData->m_nType  = RT_TREND_EXT;
			pskData->m_nSize  = 1;

			memcpy(pskData->m_pCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
			memcpy(&pskData->m_nPrivateKey, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));

			pReqCollector->AddRequest((char*)pskData,lAskLen);
			delete []pskData;

			lAskLen = sizeof(AskData);
			char* pData = new char[lAskLen]; 
			AskData*  pAskData = (AskData*)pData;
			memset(pAskData, 0, lAskLen);
			pAskData->m_nType  = RT_REALTIME_EXT;
			pAskData->m_nSize  = 1;
			memcpy(pAskData->m_pCode, &pStockInfo->m_ciStockCode,sizeof(CodeInfo));
			pReqCollector->AddRequest((char*)pAskData,lAskLen);

			pAskData->m_nType  = RT_AUTOPUSH_EXT;
			pReqCollector->AddRequest((char*)pAskData,lAskLen);
			delete[] pData;
		}
		if(m_dataFenshi.m_nLastReqDay > 1 && m_dataFenshi.m_nLastReqDay > m_dataFenshi.m_nLastRecvDay)
		{
			// 计算长度	
			int nSize = sizeof(ReqHisTrend) - sizeof(CodeInfo);
			if(nSize % sizeof(CodeInfo))
			{
				nSize += sizeof(CodeInfo);
			}
			int nAskLen = sizeof(AskData) + nSize;
			int nStart = m_dataFenshi.m_nLastRecvDay;
			if (nStart <= 0)
			{
				nStart = 1;
			}
			for (int i = nStart; i < m_dataFenshi.m_nLastReqDay; i ++)
			{
				AskData* pakData       = (AskData*)new char[nAskLen];
				memset(pakData, 0, nAskLen);
				pakData->m_nType	   = RT_HISTREND;
				pakData->m_nSize	   = (nSize / sizeof(CodeInfo)) + 1;
				// 设置请求包数据
				ReqHisTrend* pReqHisTrend = (ReqHisTrend*)pakData->m_pCode;
				if( pStockInfo != NULL )
				{
					memcpy(&pReqHisTrend->m_ciStockCode, &pStockInfo->m_ciStockCode, sizeof(CodeInfo));
				}
				pReqHisTrend->m_lDate = - i;	
				pReqCollector->AddRequest((char*)pakData, nAskLen);
				delete []pakData;
			}
		}
		else	//数据已经全有了，只要再刷新一次就行
		{
			m_lDataVauleLen = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
			DiagramInValidate();
		}
	}

	return TRUE;
}

int	CTechKLineMgr::SendRealTimeRequest()
{
	if (m_pRegion != NULL)
	{
		IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
		if (pRequestCollector != NULL)
		{
			GetRequestData(pRequestCollector, &m_stkInfo, FALSE);
		}
		m_pRegion->SendRequest(pRequestCollector);
		pRequestCollector->Release();
	}
	return 0;
}

int	CTechKLineMgr::OnUpdateData(DataHead* pData)
{
	if(pData == NULL)
		return -1;
	if(pData->m_nType == RT_TECHDATA_EX && m_nKLineType == PKLINE_TYPE_K)
	{
		CTechKLineImp* pTop = GetTopLine();
		int nPeriod = 0;
		if (pTop != NULL)
		{
			nPeriod = pTop->GetHorizCellNum();
		}
		AnsDayDataEx* pDayData = (AnsDayDataEx*)pData;
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			TRACE("收到叠加技术线数据\n");
			return UpdateAddLinesData(pData);
		}
		if (pDayData->m_nSize <= 0 )
		{
			return -1;
		}            
		TRACE("收到技术线数据\n");
		StockCompDayDataEx* pDataRecv = pDayData->m_sdData;
		int nCurrentDataLen = pDayData->m_nSize;
		int nMinDay = min(m_nRequestPeriod, pDayData->m_nSize);
		
		if (nCurrentDataLen != m_lDataVauleLen)
		{
			m_lDataVauleLen = nCurrentDataLen;
			m_dataKLine.Refresh(nCurrentDataLen);

			m_nEndPos = m_lDataVauleLen;
			m_nStartPos = m_nEndPos - nPeriod;
			if (m_nStartPos < 0)
			{
				m_nStartPos = 0;
			}
		}

		if(!m_dataKLine.CopyData(pDataRecv, nCurrentDataLen))		
			return -1;

		if (m_pFormulaMan != NULL)
		{
			if(m_pFormulaMan->SetDayData(m_dataKLine.m_pStockDayData, m_lDataVauleLen, 10, 10, 10, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal))
			{
			}
		}
		for (std::list<CTechKLineImp*>::iterator it = m_listKLine.begin(); it != m_listKLine.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible())
			{
				pImpl->OnUpdateData(pData);
			}
		}
	}
	else if (RT_LIMITTICK == pData->m_nType && m_nKLineType == PKLINE_TYPE_K)
	{
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) == 0)
		{
			TRACE("收到tick数据\n");
		}
	}
	else if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		if(RT_TECHDATA_EX == pData->m_nType)
		{
			if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
			{
				TRACE("收到叠加分时线（分钟线）数据\n");
				return UpdateAddLinesData(pData);
			}
			int nLen = m_dataFenshi.UpdateMinTechData(pData);
			CTechKLineImp* pImpTop = GetTopLine();
			if (pImpTop != NULL)
			{
				int nCellNum = pImpTop->GetHorizCellNum();
				if (nLen > nCellNum)
				{
					nLen = nCellNum;
				}
			}
			if (m_pFormulaMan != NULL && nLen > 0)
			{
				if(m_pFormulaMan->SetDayData(m_dataFenshi.m_minData.m_pStockDayData, nLen, 10, 10, 10, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal))
				{
				}
			}
			for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
			{
				CTechKLineImp* pImpl = (CTechKLineImp*)*it;
				if (pImpl != NULL && pImpl->GetVisible() )
				{
					pImpl->OnUpdateData(pData);
				}
			}
		}
		if (RT_TREND_EXT == pData->m_nType)
		{
			if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
			{
				TRACE("收到叠加分时线数据\n");
				return UpdateAddLinesData(pData);
			}
			TRACE("收到分时线数据\n");
			AnsTrendData_Ext *pTrenad = (AnsTrendData_Ext *)pData;
			m_lDataVauleLen = m_dataFenshi.UpdataTreadEx(pTrenad);
			m_nEndPos = m_lDataVauleLen;
			m_nStartPos = 0;

			if (m_pRegion != NULL)
			{
				IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
				if (pRequestCollector != NULL)
				{
					TRACE("分时线请求分钟线数据,长度 =%d\n", m_lDataVauleLen);
					m_dataFenshi.GetMinDataRequest(pRequestCollector, m_lDataVauleLen);
				}
				m_pRegion->SendRequest(pRequestCollector);
				pRequestCollector->Release();
			}
		}
		if (RT_AUTOPUSH_EXT == pData->m_nType)
		{
			AnsHSAutoPushData_Ext *pEverytimeData = (AnsHSAutoPushData_Ext *)pData;
			CommRealTimeData_Ext *pCommRealTimeData = NULL;
			CommRealTimeData_Ext* pRealTimeData = pEverytimeData->m_pstData;
			if (pRealTimeData != NULL && memcmp(&m_stkInfo.m_ciStockCode, &pRealTimeData->m_ciStockCode, sizeof(CodeInfo)) != 0)
			{
				return UpdateAddLinesData(pData);
			}

			int nNowSize = 0;
			for (int i = 0; i < pEverytimeData->m_nSize; i ++)
			{
				CommRealTimeData_Ext* pNext = pRealTimeData->GetNext(&pRealTimeData->m_ciStockCode,nNowSize);;
				if(memcmp(&pRealTimeData->m_ciStockCode, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo)) == 0)
				{
					pCommRealTimeData = pRealTimeData;
					break;
				}
				pRealTimeData = pNext;
			}
			if (pCommRealTimeData != NULL)
			{
				m_lDataVauleLen = m_dataFenshi.UpdateRealTimeData(pCommRealTimeData);
				m_nEndPos = m_lDataVauleLen;
			}
		}

		if(RT_HISTREND == pData->m_nType)
		{
			if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
			{
				TRACE("收到叠加历史分时线数据\n");
				return UpdateAddLinesData(pData);
			}
			TRACE("收到历史分时线数据\n");
			AnsHisTrend * pTendData = (AnsHisTrend *)pData;
			int nDataValLen = 0;
			int nTrendNum = m_dataFenshi.UpdataHisTrend(pTendData, nDataValLen);
			m_nStartPos = 0;
			m_lDataVauleLen = nDataValLen;
			m_nEndPos = nTrendNum;
			
			if (m_pRegion != NULL && (m_lDataVauleLen > m_dataFenshi.m_nOneDayDataNum || m_lDataVauleLen == m_dataFenshi.m_nCurrentDataLen))
			{
				IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
				if (pRequestCollector != NULL)
				{
					if(m_dataFenshi.GetMinDataRequest(pRequestCollector, m_lDataVauleLen))
					{
						TRACE("历史分时线请求分钟线数据,长度 =%d\n", m_lDataVauleLen);
						m_pRegion->SendRequest(pRequestCollector);
					}
					pRequestCollector->Release();
				}
			}

			if (nTrendNum > 0)
			{
				for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
				{
					CTechKLineImp* pImpl = (CTechKLineImp*)*it;
					if (pImpl != NULL && pImpl->GetVisible())
					{
						pImpl->SetHorizCellNum(nTrendNum);
					}
				}
			}
		}

		for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible())
			{
				pImpl->OnUpdateData(pData);
			}
		}
	}
	else if ( m_nKLineType == PKLINE_TYPE_K && RT_HISTREND == pData->m_nType)	//在K线中看历史回忆
	{
		if (m_pDialogHisTrend != NULL && m_pDialogHisTrend->m_hWnd != NULL && ::IsWindow(m_pDialogHisTrend->m_hWnd))
		{
			long lPreClose = m_stkInfo.m_lPrevClose;
			if(m_nSelDataIndex > 0)
				lPreClose = m_dataKLine.m_pStockDayData[m_nSelDataIndex - 1].m_lClosePrice;
			m_pDialogHisTrend->OnUpdateData(pData, &m_dataKLine.m_pStockDayData[m_nSelDataIndex], lPreClose, 
				m_lFinaceHand * 100);
		}
	}

	if (RT_TEXTDATAWITHINDEX_NEGATIVE == pData->m_nType || RT_BYINDEXRETDATA == pData->m_nType )		//信息地雷
	{
		if(m_pDrawInfo != NULL)
			m_pDrawInfo->OnUpdateData(pData);
	}
//	if(HitTest(m_ptMouseFocus) <= 0)
	{
		DiagramInValidate();
		return 1;
	}
	return 0;
}

void CTechKLineMgr::OnLayoutNotify(UINT nMsgId, IDrawBaseParam* pParam)
{
	if (nMsgId == DiagramMsg_FormulaParamChanged && pParam != NULL)
	{
		CString strTechName = pParam->GetStringParam(Param_TechAnalyas_ExpressName);
		int nTechType = pParam->GetIntegerParam(Param_TechAnalyas_ExpressType);
		int nStyle = pParam->GetIntegerParam(Param_TechAnalyse_ExpressStyle);

		if (!strTechName.IsEmpty())
		{
			CTechKLineImp* pImpChanged = NULL;
			std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
			if (pList != NULL && pList->size() > 0)
			{
				for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
				{
					CTechKLineImp* pImpCur = *it;
					if (pImpCur != NULL && pImpCur->GetVisible())
					{
						CString strTechCur = pImpCur->GetTeckKLineName();
						if (!strTechCur.IsEmpty() && strTechCur.CompareNoCase(strTechName) == 0)
						{
							pImpChanged = pImpCur;
							break;
						}
						else if (!strTechName.IsEmpty())
						{
							ExpPropery expNew;
							expNew.m_strName = strTechName;
							expNew.m_dExpType = nTechType;
							expNew.m_bStyle = nStyle;

							ModifyTechLine(&expNew, FALSE);
						}
					}
				}
			}
			if (pImpChanged != NULL)
			{
				pImpChanged->OnUpdateData(NULL);
			}
		}
	}
	else if (nMsgId == DiagramMsg_OpenLayout && pParam != NULL)
	{
		int nType = pParam->GetIntegerParam(Param_Open_Mark);
		BOOL bNeedChange = TRUE;
		if (nType == 1)
		{
			if (m_nKLineType == PKLINE_TYPE_FENSHI)
			{
				bNeedChange = FALSE;
			}
		}
		else if (nType == 2)
		{
			if (m_nKLineType == PKLINE_TYPE_K)
			{
				bNeedChange = FALSE;
			}
		}
		if(bNeedChange)
			ChangeType();
	}

}

int	CTechKLineMgr::Delete()
{
	if(m_bPosChanged)
	{
		m_bPosChanged = FALSE;
		if(m_pPostionCtrlMgr != NULL)
		{
			if(MessageBox(GetParentHWnd(), m_strPosChangeMsg, m_strAppName, MB_OKCANCEL) == IDOK)
			{
				m_pPostionCtrlMgr->Save(m_nKLineType);
			}
		}
	}
	delete this;
	return 0;
}

int	CTechKLineMgr::GetStockDayData(StockDay*& pStockDaya)
{
	if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		pStockDaya = m_dataFenshi.m_minData.GetStockDayData();
		int nRet = 0;//m_dataFenshi.m_minData.GetStockCompDayDataLen();
		if (nRet <= 0)
		{
			nRet = m_dataFenshi.m_minData.m_nCurrentDataLen - m_dataFenshi.m_minData.m_nCurrentDataPos;
		}
		return nRet;
	}
	else if (m_nKLineType == PKLINE_TYPE_K)
	{
		pStockDaya = m_dataKLine.GetCurrentDrawData();
		return m_lDataVauleLen;
	}
	return 0;
}

StockUserInfo* CTechKLineMgr::GetStockUserInfo()
{
	return &m_stkInfo;
}

short CTechKLineMgr::GetKLineType()
{
	return m_nKLineType;
}

std::list<CTechKLineImp*>* CTechKLineMgr::GetCurrentTechLines()
{
	if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		return &m_listFenshi;
	}
	else if (m_nKLineType == PKLINE_TYPE_K)
	{
		return &m_listKLine;
	}
	return NULL;
}

int CTechKLineMgr::DrawTitle(void* pRefDraw)
{
	CHSLinear<float>* pDraw = (CHSLinear<float>*) pRefDraw;
	if (pDraw == NULL)
	{
		return 0;
	}
	if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		CTechKLineImp* pImp = pDraw->GetPKLine();
		CArray< CToptextType<double>*,CToptextType<double>* > ayAdd;
		if (pImp != NULL && pImp->IsMainLine())
		{
			CString strText = _T("分时"); 
			ayAdd.Add( new CToptextType<double>(m_stkInfo.GetName() + " ", 0, g_hsGraphicsData.clFixedNameTXT.m_lColor, pImp, 0));//名称的颜色用‘名称文本’
			ayAdd.Add( new CToptextType<double>(m_stkInfo.GetCode() + " ", 0, g_hsGraphicsData.clFixedCodeTXT.m_lColor, pImp, 0));//代码的颜色用‘代码文本’
			ayAdd.Add( new CToptextType<double>(strText, 0, g_hsGraphicsData.clJsPriceLine.m_lColor, pImp, 0) );
			ayAdd.Add( new CToptextType<double>(_T("均线"), 0, g_hsGraphicsData.clJsAvgLine.m_lColor, pImp, 0) );
			ayAdd.Add( new CToptextType<double>(_T("成交量"), 0, g_hsGraphicsData.clVerAmountTXT.m_lColor, pImp, 0) );

			std::list<CTechKLineImp*> listLines;
			CTechKLineImp* pImp = pDraw->GetPKLine();
			int nSize = 0;
			if(pImp != NULL && pImp->IsMainLine())
			{
				nSize = GetAddLines(listLines, etechLineType_all);
			}
			if (nSize > 0)
			{
				for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
				{
					CTechKLineImp* pImp = (CTechKLineImp*)*it;
					if (pImp != NULL)
					{
						pImp->GetVarTitle(ayAdd, m_nSelDataIndex);
					}
				}
			}
			listLines.clear();
		}
		else if (pImp != NULL)
		{
			pImp->GetVarTitle(ayAdd, m_nSelDataIndex);
		}
		pDraw->DrawVarTitle(ayAdd);

		while(ayAdd.GetSize() > 0)
		{
			CToptextType<double>* pTopText = ayAdd.GetAt(0);
			ayAdd.RemoveAt(0);
			if (pTopText != NULL)
			{
				delete pTopText;
				pTopText = NULL;
			}
		}
	}
	else if (m_nKLineType == PKLINE_TYPE_K)
	{
		std::list<CTechKLineImp*> listLines;
		CTechKLineImp* pImp = pDraw->GetPKLine();
		if (pImp != NULL)
		{
			listLines.push_back(pImp);
			CArray< CToptextType<double>*,CToptextType<double>* > ayAdd;
			int nSize = 1;
			if(pImp->IsMainLine())
			{
				nSize = GetAddLines(listLines, etechLineType_all);
			}
			if (nSize > 0)
			{
				for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
				{
					CTechKLineImp* pImp = (CTechKLineImp*)*it;
					if (pImp != NULL)
					{
						pImp->GetVarTitle(ayAdd, m_nSelDataIndex);
					}
				}
			}
			if (ayAdd.GetSize() > 0)
			{
				pDraw->DrawVarTitle(ayAdd);
			}
			while(ayAdd.GetSize() > 0)
			{
				CToptextType<double>* pTopText = ayAdd.GetAt(0);
				ayAdd.RemoveAt(0);
				if (pTopText != NULL)
				{
					delete pTopText;
					pTopText = NULL;
				}
			}
			listLines.clear();
		}
	}
	return 0;
}

int	CTechKLineMgr::GetKLineNextTime(int nIndex, char* pszTime, WORD wPeriodType)
{
	if(m_nKLineType == PKLINE_TYPE_FENSHI)
		return -1;
	if((nIndex < 0 ) || (nIndex >= m_lDataVauleLen) || m_dataKLine.m_pStockDayData == NULL)
		return -1;

	long lMonthDay = 0;
	CTime tm1;
	if(nIndex == 0)
	{
		tm1 = CGeneralHelper::LongToDate(19800101L);
		lMonthDay = 0101;
	}
	else
	{
		tm1 = CGeneralHelper::LongToDate(m_dataKLine.m_pStockDayData[nIndex-1].m_lDate);
		lMonthDay = m_dataKLine.m_pStockDayData[nIndex-1].m_lDate%10000;
	}
	CTime tm2;
	for(int i = nIndex; i < m_lDataVauleLen; i++)
	{
		tm2 = CGeneralHelper::LongToDate(m_dataKLine.m_pStockDayData[i].m_lDate);
		if( tm2 != -1 )
		{
			switch(wPeriodType)
			{
			case PERIOD_TYPE_MINUTE1:
			case PERIOD_TYPE_MINUTE5:
			case PERIOD_TYPE_MINUTE15:
			case PERIOD_TYPE_MINUTE30:
			case PERIOD_TYPE_MINUTE60:
				{
					int nDay1 = lMonthDay;//tm1.GetDay();
					int nDay2 = m_dataKLine.m_pStockDayData[i].m_lDate%10000;//tm2.GetDay();
					if(nDay1 != nDay2)
					{
						sprintf(pszTime, "%02d:%02d", nDay2/100, nDay2%100);
						return i;
					}
					break;
				}
			case PERIOD_TYPE_DAY:
				{
					int nMonth1 = tm1.GetMonth();
					int nMonth2 = tm2.GetMonth();
					if(nMonth1 != nMonth2)
					{
						sprintf(pszTime, "%04d/%02d", tm2.GetYear(),tm2.GetMonth());
						return i;
					}
					break;
				}
			case PERIOD_TYPE_WEEK:
			case PERIOD_TYPE_MONTH:
			case PERIOD_TYPE_HISDAY:
				{
					int nYear1 = tm1.GetYear();
					int nYear2 = tm2.GetYear();
					if(nYear1 != nYear2)
					{
						sprintf(pszTime, "%04d", nYear2);
						return i;
					}
					break;
				}
			}
		}
	}

	return -1;
}

int	CTechKLineMgr::GetDataXPos(int nDataIndex)
{
	CTechKLineImp* pImplTop = GetTopLine();
	if (pImplTop != NULL)
	{
		return pImplTop->GetDataXPos(nDataIndex);
	}
	return -1;
}

CString CTechKLineMgr::FormatTip(int nDataIndex, WORD wPeriodType)
{
	if((nDataIndex <0 ) || (nDataIndex >= m_lDataVauleLen) || m_dataKLine.m_pStockDayData == NULL)
		return "";

	CString strDate;
	strDate.Format("%i",m_dataKLine.m_pStockDayData[nDataIndex].m_lDate);
	
	if( strDate.IsEmpty() )
		return "";

	CString str;
	try
	{
		unsigned long lTime = strtoul(strDate,'\0',10);//--2010.11.12 litao修正2012时间问题
		if( lTime <= 0 )
			return "";

		CTime tm = CGeneralHelper::LongToDate(lTime);
		switch(wPeriodType)
		{
		case PERIOD_TYPE_MINUTE0:		//add by lxqi 20090624	添加K线对Tick图支持
			{
				tm = CTime::GetCurrentTime();
				str.Format("%04d/%02d/%02d %02d:%02d:%02d, %s",tm.GetYear(),tm.GetMonth(),tm.GetDay(),
					lTime/10000,lTime%10000/100,lTime%10000%100,CGeneralHelper::ConvertChinaOfWeek(tm.GetDayOfWeek()) );
				break;
			}
		case PERIOD_TYPE_MINUTE1:
		case PERIOD_TYPE_MINUTE5:
		case PERIOD_TYPE_MINUTE15:
		case PERIOD_TYPE_MINUTE30:
		case PERIOD_TYPE_MINUTE60:
		case PERIOD_TYPE_MINUTE120:
			{
				lTime = g_hsGraphicsData.HsGetYearMothDay(lTime);

				str.Format( "%04d/%02d/%02d/%02d:%02d,%s", 
					lTime/10000,lTime%10000/100,lTime%10000%100,tm.GetHour(),tm.GetMinute(),
					CGeneralHelper::ConvertChinaOfWeek(tm.GetDayOfWeek()) );
				break;
			}
		case PERIOD_TYPE_DAY:
		case PERIOD_TYPE_WEEK:
		case PERIOD_TYPE_MONTH:
		case PERIOD_TYPE_HISDAY:
			{
				str.Format( "%04d/%02d/%02d,%s", tm.GetYear(),tm.GetMonth(),tm.GetDay(),CGeneralHelper::ConvertChinaOfWeek(tm.GetDayOfWeek()) );
				break;
			}
		}
		if(str.IsEmpty())
			return strDate;
	}
	catch(...){}

	return str;
}

CWnd* CTechKLineMgr::GetParentWnd()
{
	if (m_pRegion != NULL)
	{
		return CWnd::FromHandle(m_pRegion->GetHostHandle());
	}
	return NULL;
}

HWND  CTechKLineMgr::GetParentHWnd()
{
	if (m_pRegion != NULL)
	{
		return m_pRegion->GetHostHandle();
	}
	return NULL;
}

int	 CTechKLineMgr::DelayDeleteTechLine(CTechKLineImp* pTechLine)
{
	m_pTechLineDelayDelete = pTechLine;
	m_nDelayDeleteTimerID = SetTimer(GetParentHWnd(), (UINT)this, IDT_DELAY_DELETELINE, &CTechKLineMgr::DelayDelete);
	return 0;
}

VOID CALLBACK CTechKLineMgr::DelayDelete(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	KillTimer(hwnd, idEvent);
	CTechKLineMgr* pMgr = (CTechKLineMgr*)idEvent;
	if (pMgr != NULL)
	{
		pMgr->DeleteLine();
	}

}

int CTechKLineMgr::SwitchPage(WORD wPageType)
{
	if (m_wPeriodType == wPageType)
	{
		return -1;
	}
	m_wPeriodType = wPageType;
	SendRealTimeRequest();
	return 0;
}

int CTechKLineMgr::ChangeType()
{
	if(m_bPosChanged)
	{
		m_bPosChanged = FALSE;
		if(m_pPostionCtrlMgr != NULL)
		{
			if(MessageBox(GetParentHWnd(), m_strPosChangeMsg, m_strAppName, MB_OKCANCEL) == IDOK)
			{
				m_pPostionCtrlMgr->Save(m_nKLineType);
			}
		}
	}

	BOOL bNewRequest = FALSE;
	if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		m_nKLineType = PKLINE_TYPE_K;
		bNewRequest = m_dataKLine.m_bNeedRefresh;
	}
	else
	{
		m_nKLineType = PKLINE_TYPE_FENSHI;
		bNewRequest = m_dataFenshi.GetNeedRefresh(&m_stkInfo);
	}

	BOOL bShowBottom = TRUE;
	if (m_nKLineType == PKLINE_TYPE_FENSHI )
	{
		bShowBottom = FALSE;
	}
	if (m_pDrawBottom != NULL)
	{
		m_pDrawBottom->SetVisible(bShowBottom);
	}
	CTechKLineImp* pImpTop = GetTopLine();
	if (pImpTop != NULL )
	{
		int nCellNum = pImpTop->GetHorizCellNum();
		if (!bNewRequest)
		{
			if (m_nKLineType == PKLINE_TYPE_FENSHI)
			{
				m_lDataVauleLen = m_dataFenshi.m_nRecvDataLen - m_dataFenshi.m_nCurFenshiPos;
				m_nEndPos = m_lDataVauleLen;
				m_nStartPos = 0;
				if (m_pFormulaMan != NULL)
				{
					if(m_pFormulaMan->SetDayData(m_dataFenshi.m_minData.m_pStockDayData, m_dataFenshi.m_minData.m_nCurrentDataLen, 10, 10, 10, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal))
					{
					}
				}
			}
			else if (PKLINE_TYPE_K == m_nKLineType )
			{
				m_lDataVauleLen = m_dataKLine.m_nCurrentDataLen;
				ASSERT(m_lDataVauleLen >= nCellNum);
				m_nEndPos = m_dataKLine.m_nCurrentDataLen;
				m_nStartPos = m_nEndPos - nCellNum;
				if (m_nStartPos < 0)
				{
					m_nStartPos = 0;
				}
				if (m_pFormulaMan != NULL)
				{
					if(m_pFormulaMan->SetDayData(m_dataKLine.m_pStockDayData, m_lDataVauleLen, 10, 10, 10, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal))
					{
					}
				}
			}
		}
		if (m_pDrawBox != NULL)
		{
			m_pDrawBox->SetVisible(FALSE);
		}
		if (bNewRequest)
		{
			m_lDataVauleLen = 0;
			m_nRequestPeriod = pImpTop->GetMaxHorizCellNum();
			SendRealTimeRequest();
		}
		if (m_pPostionCtrlMgr != NULL)
		{
			CRect rcTotal = m_rect;
			rcTotal.bottom -= TAB_ITEM_HEIGHT;
			if (m_nKLineType == PKLINE_TYPE_K)
			{
				if (m_pDrawBottom != NULL && m_pDrawBottom->GetVisible())
				{
					CRect rcBottom = CRect(rcTotal.left, rcTotal.bottom - DRAWBOTTOM_HEIGHT, rcTotal.right, rcTotal.bottom);
					m_pDrawBottom->SetRect(rcBottom);
				}
			}
			m_pPostionCtrlMgr->Update(m_nKLineType == PKLINE_TYPE_K ? m_rect.Height() - TAB_ITEM_HEIGHT - DRAWBOTTOM_HEIGHT : m_rect.Height() - TAB_ITEM_HEIGHT, m_rect.Width(), TRUE);
		}
		m_ptCursorLine = CPoint(-1, -1);
		DiagramInValidate();
		return 1;
	}

	ReCreate(NULL);
	SendRealTimeRequest();

	return 0;
}

CTechKLineImp*	CTechKLineMgr::GetTopLine()
{
	CTechKLineImp* pImplTop = NULL;
	if (m_nKLineType == PKLINE_TYPE_K && m_listKLine.size() > 0)
	{
		pImplTop = m_listKLine.front();
	}
	else if (m_nKLineType == PKLINE_TYPE_FENSHI && m_listFenshi.size() > 0)
	{
		pImplTop = m_listFenshi.front();
	}
	return pImplTop;
}

CTechKLineImp* CTechKLineMgr::GetBottomLine()
{
	CTechKLineImp* pImplBottom = NULL;
	if (m_nKLineType == PKLINE_TYPE_K)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listKLine.begin(); it != m_listKLine.end(); it ++ )
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur->GetIsBottom())
			{
				pImplBottom = pImpCur;
				break;
			}
		}
	}
	else if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++ )
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL)
			{
				if (pImpCur != NULL && pImpCur->GetIsBottom())
				{
					pImplBottom = pImpCur;
					break;
				}
			}
		}
	}
	return pImplBottom;
}

void CTechKLineMgr::GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	CTechKLineImp* pImplTop = GetTopLine();
	if (pImplTop != NULL)
	{
		return pImplTop->GetMaxMinData(dMax, dMin, dOpenPrice);
	}
}
int	CTechKLineMgr::MoveCursorPos(BOOL bNext)
{
	int nIndex = m_nSelDataIndex - m_nStartPos;
	if(!bNext)
	{
		nIndex --;
	}
	else
		nIndex ++;
	CTechKLineImp* pImp = GetTopLine();
	int nCellNum = m_lDataVauleLen;
	if (pImp != NULL)
	{
		nCellNum = pImp->GetHorizCellNum();
	}
	if (nIndex < 0)
	{
		nIndex = 0;
		if (m_nStartPos > 0)
		{
			int nOldPos = m_nEndPos - m_nStartPos;
			m_nStartPos --;
			m_nEndPos = m_nStartPos + min(nCellNum, m_lDataVauleLen);
		}
	}
	if (nIndex >= nCellNum)
	{
		if (m_nEndPos < m_lDataVauleLen)
		{
			m_nStartPos += (nIndex - nCellNum + 1);
			m_nEndPos += (nIndex - nCellNum + 1);
			nIndex = nCellNum - 1;
		}
		else 
		{
			nIndex = 0;
		}
	}
	if (nIndex >= m_nEndPos - m_nStartPos)
	{
		nIndex = 0;
	}
	if (pImp != NULL)
	{
		CPoint pt;
		pt.x = pImp->GetDataXPos(nIndex);
		pt.y = pImp->GetDataYPos(nIndex);
		HitTest(pt);
	}
	return nIndex;
}

int	CTechKLineMgr::ShowHisTrend()
{
	if (m_nKLineType == PKLINE_TYPE_K)
	{
		if(m_pDialogHisTrend == NULL)
		{
			m_pDialogHisTrend = new CDialogHisTrend(&m_stkInfo, m_pRegion/*CWnd::FromHandle(GetParentWnd())*/);
			m_pDialogHisTrend->SetMainMgr(this);
			m_pDialogHisTrend->SetStockType(&m_stockType);
		}
		if (m_pDialogHisTrend != NULL && m_pDialogHisTrend->m_hWnd != NULL && ::IsWindow(m_pDialogHisTrend->m_hWnd))
		{
			long lDate = m_dataKLine.GetIndexDate(m_nSelDataIndex);
			if (lDate != m_lDate)
			{
				m_pDialogHisTrend->ShowWindow(SW_SHOW);
				m_pDialogHisTrend->SetHisTrendDate(lDate);
			}
		}
	}
	return 0;
}

int	CTechKLineMgr::MovePosition(CPoint ptMove)
{
	CTechKLineImp* pImplTop = GetTopLine();
	if (pImplTop != NULL)
	{
		float fHorizCellWidth = pImplTop->GetHorizCellWidth();
		if (fHorizCellWidth < 1)
		{
			fHorizCellWidth = 1;
		}
		int nRanger = pImplTop->GetHorizCellNum();
		int nMoveX = int((float)ptMove.x / fHorizCellWidth);
		if (abs(nMoveX) >= 1)
		{
			m_nStartPos -= nMoveX;
			if (m_nStartPos < 0)
			{
				m_nStartPos = 0;
			}
			if (m_nStartPos > m_lDataVauleLen - 1)
			{
				m_nStartPos = m_lDataVauleLen - 1;
			}
			m_nEndPos = m_nStartPos + nRanger;
			if (m_nEndPos >= m_lDataVauleLen)
			{
				m_nEndPos = m_lDataVauleLen;
			}
			DiagramInValidate();
			return 1;
		}
	}
	return 0;
}

void CTechKLineMgr::DiagramInValidate(CRect rcInvalidate/* = CRect(0, 0, 0, 0)*/)
{
	CRect rc = rcInvalidate;;
	if (rcInvalidate.IsRectEmpty())
	{
		rc = m_rect;
	}
	CWnd* pWnd = GetParentWnd();
	if (pWnd != NULL && pWnd->m_hWnd != NULL && ::IsWindow(pWnd->m_hWnd))
	{
		pWnd->InvalidateRect(rc, FALSE);
	}
}

int CTechKLineMgr::GetAddLines(std::list<CTechKLineImp*>& listKLine, short nType)
{
	if (m_nKLineType == PKLINE_TYPE_K)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listKLine.begin(); it != m_listKLine.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible() && pImpl->IsAddLine() && pImpl->GetLineType() & nType)
			{
				listKLine.push_back(pImpl);
			}
		}
	}
	else if (m_nKLineType == PKLINE_TYPE_FENSHI)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible() && pImpl->IsAddLine() && pImpl->GetLineType() & nType)
			{
				listKLine.push_back(pImpl);
			}
		}
	}
	return listKLine.size();
}

void CTechKLineMgr::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	std::list<CTechKLineImp*> listLines;
	listLines.push_back(GetTopLine());
	int nSize = GetAddLines(listLines, etechLineType_all);
	if (nSize > 0)
	{
		HSDouble dblMaxRet = hx_DefMaxValues; //pLine->m_lMaxValue;
		HSDouble dblMinRet = hx_DefMinValues; //pLine->m_lMinValue;
		HSDouble dMaxTemp, dMinTemp, dOpenTemp;
		for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
		{
			CTechKLineImp* pImp = (CTechKLineImp*)*it;
			if (pImp != NULL)
			{
				pImp->GetMaxMinData(dMaxTemp, dMinTemp, dOpenTemp);

				if(dblMinRet > dMinTemp || dblMinRet == hx_DefMinValues)
				{
					dblMinRet = dMinTemp;
				}
				if (dblMaxRet < dMaxTemp || dblMaxRet == hx_DefMaxValues)
				{
					dblMaxRet = dMaxTemp;
				}

				if(it == listLines.begin())
					dOpenPrice = dOpenTemp;
			}
		}
		dMax = dblMaxRet;
		dMin = dblMinRet;
	}
	if (dMax == hx_DefMinValues)
	{
		dMax = 0;
	}
	if (dMin == hx_DefMaxValues)
	{
		dMin = 0;
	}
	listLines.clear();
}

int CTechKLineMgr::ModifyTechLine(ExpPropery* pExp, BOOL bDelOld)
{
	int nRet = 0;
	if (pExp && HS_EXP_MAINCHART & pExp->m_bStyle)
	{
		std::list<CTechKLineImp*> listLines;
		int nSize = 0;
		nSize = GetAddLines(listLines, etechLineType_tech);
		if (nSize > 0)
		{
			CTechKLineImp* pImpOld = listLines.front();
			if (pExp->m_strName == pImpOld->GetTeckKLineName() )
			{
				if (!pImpOld->GetIsBottom() && bDelOld)
				{
					listLines.remove(pImpOld);
					if (m_nKLineType == PKLINE_TYPE_K)
					{
						m_listKLine.remove(pImpOld);
					}
					else if (m_nKLineType == PKLINE_TYPE_FENSHI)
					{
						m_listFenshi.remove(pImpOld);
					}
					if (pImpOld != NULL)
					{
						delete pImpOld;
						pImpOld = NULL;
						nRet = eTechLineOperator_mainDel;
					}
				}
			}
			else
			{
				pImpOld->SetTeckKLineName(pExp->m_strName);
				pImpOld->OnUpdateData(NULL);
				nRet = eTechLineOperator_mainModify;
			}
		}
		else
		{
			CTechKLineImp* pImpTop = GetTopLine();
			if (pImpTop != NULL)
			{
				float fHorizCellWidth = pImpTop->GetHorizCellWidth();
				CRect rcTop = pImpTop->GetRect();
				CDrawTechLine* pDrawTechKLine = NULL;
				if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					int nHorizCellNum = pImpTop->GetHorizCellNum();
					pDrawTechKLine = new CDrawTechLine(this, m_pRegion, fHorizCellWidth, nHorizCellNum);
					pDrawTechKLine->ModifyLinearStyle(AXIS_RIGHTMARGIN , AXIS_BOTTOMMARGIN | AXIS_SOLIDANDDASH | AXIS_TOPMARGIN);
				}
				else if (m_nKLineType == PKLINE_TYPE_K)
				{
					pDrawTechKLine = new CDrawTechLine(this, m_pRegion, fHorizCellWidth);
				}
				if (pDrawTechKLine != NULL)
				{
					pDrawTechKLine->SetVisible(TRUE);
					pDrawTechKLine->SetTeckKLineName(pExp->m_strName);
					pDrawTechKLine->SetAddLine(TRUE);
					pDrawTechKLine->SetRect(rcTop);
					if(m_nKLineType == PKLINE_TYPE_K)
						m_listKLine.push_back(pDrawTechKLine);
					else
						m_listFenshi.push_back(pDrawTechKLine);
					pDrawTechKLine->OnUpdateData(NULL);
					nRet = eTechLineOperator_mainAddnew;
				}
			}
		}
		listLines.clear();
	}
	else 
	{
		CTechKLineImp* pImpBottom = GetBottomLine();
		if (pImpBottom != NULL && !pImpBottom->IsMainLine())
		{
			CString strName = pImpBottom->GetTeckKLineName();
			if (strName.IsEmpty() || strName.CollateNoCase("VOL") == 0)
			{
				m_bottomTab.OpenSpecifyTemplete(pExp);
				nRet = eTechLineOperator_subAddnew;
			}
			else
			{
				pImpBottom->SetTeckKLineName(pExp->m_strName);
				pImpBottom->OnUpdateData(NULL);
				nRet = eTechLineOperator_subModify;
			}
		}
	}
	return nRet;
}

int CTechKLineMgr::ModifyTechLine(StockUserInfo* pStock, BOOL bDelOld)
{
	if ( memcmp(&m_stkInfo.m_ciStockCode, &pStock->m_ciStockCode, sizeof(CodeInfo)) == 0)
	{
		return -1;
	}
	if(MessageBox(GetParentHWnd(), m_strYPerMsg, m_strAppName, MB_OKCANCEL) == IDOK)
	{
		m_nAxesType = CGraphics::CAxesType::YPER;
	}

	int nRet = 0;
	std::list<CTechKLineImp*> listLines;
	int nSize = 0;
	nSize = GetAddLines(listLines, etechLineType_stock);
	IRequestCollector* pRequestCollector = NULL;
	if (m_pRegion != NULL)
	{
		pRequestCollector = m_pRegion->GetRequestCollector();
	}

	CTechKLineImp* pImpTop = GetTopLine();
	if (pImpTop != NULL && pRequestCollector != NULL)
	{
		float fHorizCellWidth = pImpTop->GetHorizCellWidth();
		CRect rcTop = pImpTop->GetRect();
		if (m_nKLineType == PKLINE_TYPE_K)
		{
			CTechKLineImpAdd* pKLineAdd = new CTechKLineImpAdd(this, m_pRegion, fHorizCellWidth);
			if (pKLineAdd != NULL)
			{
				pKLineAdd->SetVisible(TRUE);
				pKLineAdd->SetAddLine(TRUE);
				pKLineAdd->SetRect(rcTop);
				pKLineAdd->SetColor(g_hsGraphicsData.GetDefTechColor(nSize + 1));
				m_listKLine.push_back(pKLineAdd);
				nRet = eTechLineOperator_mainAddnew;

				if(pKLineAdd->GetRequestData(pRequestCollector, pStock, m_nRequestPeriod, m_wPeriodType))
					m_pRegion->SendRequest(pRequestCollector, FALSE);
			}
		}
		else if (m_nKLineType == PKLINE_TYPE_FENSHI)
		{
			CDrawTStockTrendAdd* pTreadAdd = new CDrawTStockTrendAdd(this, m_pRegion, fHorizCellWidth);
			if (pTreadAdd != NULL)
			{
				pTreadAdd->SetVisible(TRUE);
				pTreadAdd->SetAddLine(TRUE);
				pTreadAdd->SetRect(rcTop);
				pTreadAdd->SetColor(g_hsGraphicsData.GetDefTechColor(nSize + 7));
				m_listFenshi.push_back(pTreadAdd);
				nRet = eTechLineOperator_mainAddnew;

				if(pTreadAdd->GetRequestData(pRequestCollector, pStock, m_nRequestPeriod, m_dataFenshi.m_nLastReqDay))
					m_pRegion->SendRequest(pRequestCollector, FALSE);
			}
		}
		pRequestCollector->Release();

	}
	listLines.clear();
	return nRet;
}

int	CTechKLineMgr::UpdateAddLinesData(DataHead* pData)
{
	std::list<CTechKLineImp*> listLines;
	int nSize = 0;
	nSize = GetAddLines(listLines, etechLineType_stock);
	if (nSize > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
		{
			CTechKLineImp* pImp = (CTechKLineImp*)*it;
			if (pImp != NULL)
			{
				pImp->OnUpdateData(pData);
			}
		}
	}
	listLines.clear();
	DiagramInValidate();
	return 0;
}

int	CTechKLineMgr::OnTabItemChanged(TechIndex* pTechIndex)
{
	if (pTechIndex != NULL)
	{
		ExpPropery expNew;
		expNew.m_strName = pTechIndex->m_szName;
		expNew.m_dExpType = pTechIndex->m_nIndexType;
		expNew.m_bStyle = pTechIndex->m_nLineType;

		int nRet = ModifyTechLine(&expNew);
		if (nRet > 0)
		{
			DiagramInValidate();
		}
		if (nRet == eTechLineOperator_mainAddnew || nRet == eTechLineOperator_mainDel || nRet == eTechLineOperator_mainModify)
		{
			m_bottomTab.SetActiveItem(eTabItemIndex_RootTech);
		}
	}
	return 0;
}

int	CTechKLineMgr::OnTabItemChanged(TechTemplete* pTechTemplete)
{
	CStringArray ayStrTemplete;
	if (pTechTemplete != NULL)
	{
		CString strIndexCur = pTechTemplete->m_szDefaultIndex ;
		if (!strIndexCur.IsEmpty())
		{
			CString strAnalyze = strIndexCur;
			int nPos = strAnalyze.Find(":");
			while (nPos >= 0)
			{
				CString str = strAnalyze.Left(nPos);
				ayStrTemplete.Add(str);
				strAnalyze = strAnalyze.Mid(nPos + 1);
				nPos = strAnalyze.Find(":");
			}
			if (!strAnalyze.IsEmpty()) //最后一段
			{
				ayStrTemplete.Add(strAnalyze);
			}
		}
		if (ayStrTemplete.GetSize() > 0)
		{
			SwitchMultiDiagram(ayStrTemplete, pTechTemplete->m_dRaito);
		}
	}
	return 0;
}

int	CTechKLineMgr::SwitchMultiDiagram(CStringArray& ayStrtemplete, float fRadio)
{
	std::list<CTechKLineImp*>* pList = GetCurrentTechLines();
	if (pList != NULL && pList->size() > 0)
	{
		std::list<CTechKLineImp*>::iterator it = pList->begin();
		while( it != pList->end())
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && !pImpCur->IsMainLine() && !pImpCur->IsAddLine())
			{
				std::list<CTechKLineImp*>::iterator itCur = it;
				it ++;
				if (it == pList->end())
				{
					pList->erase(itCur);
					delete pImpCur;
					pImpCur = NULL;
					break;
				}
				pList->erase(itCur);
				delete pImpCur;
				pImpCur = NULL;
				continue;
			}
			it ++;
		}
	}

	CTechKLineImp* pImplTop = GetTopLine();
	int nDiagramCount = ayStrtemplete.GetSize();
	if (nDiagramCount > 0 && pImplTop != NULL && m_pRegion != NULL)
	{
		float fHorizCellWidth = pImplTop->GetHorizCellWidth();
		int nHorizCellNum = pImplTop->GetHorizCellNum();
		CRect rcGrid = m_rect;
		rcGrid.bottom -= TAB_ITEM_HEIGHT;
		if (m_nKLineType == PKLINE_TYPE_K)
		{
			rcGrid.bottom -= DRAWBOTTOM_HEIGHT;
		}
		
		CRect rcTop = rcGrid;
		rcTop.bottom = rcGrid.top + (int)((float)rcGrid.Height() * fRadio);

		std::list<CTechKLineImp*> listLines;
		listLines.push_back(pImplTop);
		int nSize = GetAddLines(listLines, etechLineType_all);
		if (nSize > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = listLines.begin(); it != listLines.end(); it ++)
			{
				CTechKLineImpAdd* pImp = (CTechKLineImpAdd*)*it;
				if (pImp != NULL)
				{
					pImp->SetRect(rcTop);
				}
			}
		}
		listLines.clear();
		if (m_pDrawInfo != NULL)
		{
			m_pDrawInfo->SetRect(rcTop);
		}

		int nDiagramHeight = (rcGrid.Height() - rcTop.Height()) / nDiagramCount;
		CRect rcDiagram = rcGrid;
		rcDiagram.top = rcTop.bottom;
		rcDiagram.bottom = rcDiagram.top + nDiagramHeight;

		for (int i = 0; i < nDiagramCount; i ++)
		{
			CString strCur = ayStrtemplete.GetAt(i);
			int nPos = strCur.Find(",");
			CString strTechLineName;
			CString strIndexType, strLineType;
			if (nPos >= 0)
			{
				strTechLineName = strCur.Left(nPos);
				strCur = strCur.Mid(nPos + 1);
				nPos = strCur.Find(",");

				if (nPos >= 0)
				{
					strIndexType = strCur.Left(nPos);
					strLineType = strCur.Mid(nPos + 1);
				}
				else
					strIndexType = strCur;
			}
			if (!strTechLineName.IsEmpty())
			{
				CTechKLineImp* pImp = NULL;
				if (m_nKLineType == PKLINE_TYPE_FENSHI)
				{
					if(strTechLineName == "VOL")
					{
						pImp = new CDrawTStockTrendVolume(this, m_pRegion, nHorizCellNum);
						pImp->SetHorizCellNum(nHorizCellNum);
					}
					else
					{
						pImp =  new CDrawTechLine(this, m_pRegion, fHorizCellWidth, nHorizCellNum);
					}
				}
				else
				{
					pImp =  new CDrawTechLine(this, m_pRegion, fHorizCellWidth);
				}
				if (pImp != NULL)
				{
					pImp->SetVisible(TRUE);
					if (i == nDiagramCount - 1)//最后一个
					{
						pImp->SetIsBottom(TRUE);
						rcDiagram.bottom = rcGrid.bottom;
					}
					if (m_nKLineType == PKLINE_TYPE_K)
					{
						m_listKLine.push_back(pImp);
					}
					else if (m_nKLineType == PKLINE_TYPE_FENSHI)
					{
						pImp->SetFenshiDrawType(TRUE);
						if(strTechLineName != "VOL" )
							pImp->ModifyLinearStyle(AXIS_RIGHTMARGIN , AXIS_BOTTOMMARGIN | AXIS_SOLIDANDDASH | AXIS_TOPMARGIN);
						m_listFenshi.push_back(pImp);
					}
					pImp->SetRect(rcDiagram);
					pImp->SetTeckKLineName(strTechLineName);
					pImp->OnUpdateData(NULL);
				}
			}
			rcDiagram.top = rcDiagram.bottom;
			rcDiagram.bottom = rcDiagram.top + nDiagramHeight;
		}
		DiagramInValidate();
		if (m_pPostionCtrlMgr != NULL)
		{
			m_pPostionCtrlMgr->Update(m_nKLineType == PKLINE_TYPE_K ? m_rect.Height() - TAB_ITEM_HEIGHT - DRAWBOTTOM_HEIGHT : m_rect.Height() - TAB_ITEM_HEIGHT, m_rect.Width());
		}
	}
	m_bPosChanged = TRUE;
	return 0;
}

int	CTechKLineMgr::DeleteLine(CTechKLineImp* pImpDel)
{
	if (pImpDel == NULL)
	{
		pImpDel = m_pTechLineDelayDelete;
		m_pTechLineDelayDelete = NULL;
	}
	if (m_pPostionCtrlMgr != NULL)
	{
		m_pPostionCtrlMgr->OnDeleteImp(pImpDel);
		m_bPosChanged = TRUE;
	}
	if (m_pPostionCtrlMgr != NULL )
	{
		m_pPostionCtrlMgr->Update(m_nKLineType == PKLINE_TYPE_K ? m_rect.Height() - TAB_ITEM_HEIGHT - DRAWBOTTOM_HEIGHT : m_rect.Height() - TAB_ITEM_HEIGHT, m_rect.Width());
	}
	DiagramInValidate();
	return 0;
}

int	CTechKLineMgr::CalcChuquanMark(int nMenuID)
{
	if (m_ayChuQuanData.GetSize() > 0)
	{
		long lChuquanMask = 0;
		if (nMenuID == IDR_TECHKLINE_FORWARDFUQUAN)
		{
			lChuquanMask = HsTechLine_Left_ChuQuan;
		}
		else if (nMenuID == IDR_TECHKLINE_BACKWARDFUQUAN)
		{
			lChuquanMask = HsTechLine_Back_ChuQuan;
		}
		BOOL bRet = m_dataKLine.CalcChuQuan(m_wPeriodType, m_ayChuQuanData, lChuquanMask, m_stkInfo.m_nPriceUnit);

		if (m_pFormulaMan != NULL)
		{
			if(m_pFormulaMan->SetDayData(m_dataKLine.GetCurrentDrawData(), m_lDataVauleLen, 10, 10, 10, m_stkInfo.m_nPriceUnit, m_stkInfo.m_nDecimal))
			{
			}
		}
		for (std::list<CTechKLineImp*>::iterator it = m_listKLine.begin(); it != m_listKLine.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible())
			{
				pImpl->OnUpdateData(NULL);
			}
		}
		DiagramInValidate();
	}
	return 0;
}

int CTechKLineMgr::ClearOldChuquanData()
{
	m_dataKLine.InitChuquanDayData(TRUE);
	while(m_ayChuQuanData.GetSize() > 0)
	{
		ChuQuanData* pData = m_ayChuQuanData.GetAt(0);
		m_ayChuQuanData.RemoveAt(0);
		if (pData != NULL)
		{
			delete pData;
			pData = NULL;
		}
	}
	return 0;
}

CArray<ChuQuanData*,ChuQuanData*>*	CTechKLineMgr::GetChuquanData()
{
	return &m_ayChuQuanData;
}