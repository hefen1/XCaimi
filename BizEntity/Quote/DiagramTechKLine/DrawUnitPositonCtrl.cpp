#include "stdafx.h"
#include "DrawUnitPositonCtrl.h"
#include "TechKLineMgr.h"
#include "GraphicsData.h"
#include "..\..\..\Common\WinnerApplication.h"
#include "DrawTechLine.h"
#include "DrawTStockTrend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
//CDrawUnitPositonCtrl
CDrawUnitPositonCtrl::CDrawUnitPositonCtrl(CTechKLineMgr* pMgr, int nTotalHeight)
{
	m_pMgr = pMgr;
	m_nTotalHeight = nTotalHeight;
	m_fPosRatio = 0.0;
}

CDrawUnitPositonCtrl::~CDrawUnitPositonCtrl()
{
	m_listLinesAbove.empty();
	m_listLinesBelow.empty();
}

BOOL CDrawUnitPositonCtrl::MouseDown(UINT nFlags, CPoint point)
{
	if (!m_rectSelf.IsRectEmpty() && m_rectSelf.PtInRect(point) && m_pMgr != NULL  )
	{
		int nMinYPos = RAND_MAX, nMaxYPos = 0;
		if (m_listLinesAbove.size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL)
				{
					CRect rcImp = pImpCur->GetRect();
					if (nMinYPos > rcImp.top)
					{
						nMinYPos = rcImp.top;
					}
				}
			}
		}
		if (m_listLinesBelow.size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = m_listLinesBelow.begin(); it != m_listLinesBelow.end(); it ++)
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL)
				{
					CRect rcImp = pImpCur->GetRect();
					if (nMaxYPos < rcImp.bottom)
					{
						nMaxYPos = rcImp.bottom;
					}
				}
			}
		}

		MSG msg;
		CPoint pt;
		CWnd* pWnd = m_pMgr->GetParentWnd();
		CDC* pDC = pWnd->GetDC();
		if (pDC == NULL)
		{
			return FALSE;
		}
		CPen pen;
		pen.CreatePen(g_hsGraphicsData.clSplitLine.m_nPenStyle,
			g_hsGraphicsData.clSplitLine.m_nWidth,g_hsGraphicsData.clSplitLine.m_lColor);
		CPen* pOldPen = pDC->SelectObject(&pen);	

		CPoint ptLast = point;
		SetCapture(pWnd->m_hWnd);
		for(;;)
		{
			VERIFY(::GetMessage(&msg, NULL, 0, 0));

			switch(msg.message)
			{
			case WM_MOUSEMOVE:
				{
					pt.x = (int)(short)LOWORD(msg.lParam);
					pt.y = (int)(short)HIWORD(msg.lParam);
					if (pt.y >= nMaxYPos || pt.y <= nMinYPos)
					{
						break;
					}
					int nOldRop2 = pDC->SetROP2(R2_NOT);
					pDC->MoveTo(m_rectSelf.left, pt.y);
					pDC->LineTo(m_rectSelf.right, pt.y);
					pDC->MoveTo(m_rectSelf.left, ptLast.y);
					pDC->LineTo(m_rectSelf.right, ptLast.y);
					pDC->SetROP2(nOldRop2);
					ptLast = pt;
					break;
				}
			case WM_KEYDOWN:
				if (msg.wParam != VK_ESCAPE)
					break;
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:			
				goto __ExitLoop__;
			default:
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
					break;
				}
			}
		}
__ExitLoop__:
		ReleaseCapture();
		pDC->SelectObject(pOldPen);
		if (pen.m_hObject)
		{
			pen.DeleteObject();
		}
		if(pDC->m_hDC && pWnd)
		{
			pWnd->ReleaseDC(pDC);
		}
		return UpdatePostion(ptLast.y);
	}
	return FALSE;
}

void CDrawUnitPositonCtrl::SetRect(CRect rc)
{
	if (rc.IsRectEmpty())
	{
		return;
	}
	m_rectSelf = rc;
	int nYPos = (m_rectSelf.top + m_rectSelf.bottom) / 2;
	if (m_nTotalHeight > 0.01 && nYPos < (int)m_nTotalHeight)
	{
		m_fPosRatio = (float)(m_rectSelf.top + m_rectSelf.bottom) / (float)m_nTotalHeight / 2.0;
	}
}

void CDrawUnitPositonCtrl::SetRatio(float fRatio)
{
	m_fPosRatio = fRatio;
}

float CDrawUnitPositonCtrl::GetRatio()
{
	return m_fPosRatio;
}

CRect CDrawUnitPositonCtrl::GetRect()
{
	return m_rectSelf;
}

void CDrawUnitPositonCtrl::AddLinks(CTechKLineImp* pImp, BOOL bIsAbove)
{
	if(bIsAbove)
		m_listLinesAbove.push_back(pImp);
	else
		m_listLinesBelow.push_back(pImp);
}

BOOL CDrawUnitPositonCtrl::UpdatePostion(int nNewPosY)
{
	int nOldYPos = (m_rectSelf.top + m_rectSelf.bottom) / 2;
	if (nOldYPos != nNewPosY)		//不同，重新设置
	{
		if (m_listLinesAbove.size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL)
				{
					CRect rcImp = pImpCur->GetRect();
					rcImp.bottom = nNewPosY;
					pImpCur->SetRect(rcImp);
				}
			}
		}
		if (m_listLinesBelow.size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = m_listLinesBelow.begin(); it != m_listLinesBelow.end(); it ++)
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL)
				{
					CRect rcImp = pImpCur->GetRect();
					rcImp.top = nNewPosY;
					pImpCur->SetRect(rcImp);
				}
			}
		}

		if (m_nTotalHeight > 0.01 && nNewPosY < (int)m_nTotalHeight)
		{
			m_fPosRatio = (float)nNewPosY / (float)m_nTotalHeight;
		}
		m_rectSelf.top = nNewPosY - CALC_POSITION_SPLIT;
		m_rectSelf.bottom = nNewPosY + CALC_POSITION_SPLIT;
		return TRUE;
	}
	return FALSE;
}

int CDrawUnitPositonCtrl::GetResetPostion(CTechKLineImp* pImp, BOOL bTop, int nTotalHeightNew)
{
	if (m_listLinesAbove.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur == pImp)
			{
				CRect rcImp = pImpCur->GetRect();
				int nYPos = (m_rectSelf.top + m_rectSelf.bottom) / 2;
				if (bTop )
				{
					return rcImp.top;
				}
				else 
				{
					return (int)((float)nTotalHeightNew * m_fPosRatio + 0.5);
				}
			}
		}
	}
	if (m_listLinesBelow.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesBelow.begin(); it != m_listLinesBelow.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur == pImp)
			{
				CRect rcImp = pImpCur->GetRect();
				int nYPos = (m_rectSelf.top + m_rectSelf.bottom) / 2;
				if (bTop )
				{
					return (int)((float)nTotalHeightNew * m_fPosRatio + 0.5);
				}
				else 
				{
					//此处不能计算;等待下一个分隔条去计算;如果还是无法计算出来，那就是底部图元
				}
			}
		}
	}
	return -1;
}

int	CDrawUnitPositonCtrl::GetLinksName(int nType, CStringArray& ayLinesName, BOOL bTop)
{
	if (bTop && m_listLinesAbove.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && !(pImpCur->GetLineType() & etechLineType_stock))	//叠加的股票不保存
			{
				CString strName = pImpCur->GetTeckKLineName();
				if (strName.IsEmpty())
				{
					ayLinesName.Add(_T("MAIN"));
				}
				else
				{
					ayLinesName.Add(strName);
				}
			}
		}
	}
	else if (!bTop && m_listLinesBelow.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesBelow.begin(); it != m_listLinesBelow.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL)
			{
				CString strName = pImpCur->GetTeckKLineName();
				if (strName.IsEmpty())
				{
					if (nType == PKLINE_TYPE_FENSHI)
					{
						ayLinesName.Add(_T("VOLUMN"));
					}
				}
				else
				{
					ayLinesName.Add(strName);
				}
			}
		}
	}
	return ayLinesName.GetSize();
}

int CDrawUnitPositonCtrl::IsInMyCtrl(CTechKLineImp* pImp, BOOL bTop)
{
	if (bTop && m_listLinesAbove.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur == pImp)
			{
				return m_listLinesAbove.size();
			}
		}
	}
	else if (!bTop && m_listLinesBelow.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesBelow.begin(); it != m_listLinesBelow.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL && pImpCur == pImp)
			{
				return m_listLinesBelow.size();
			}
		}
	}
	return 0;
}

int CDrawUnitPositonCtrl::ResetLinkedImpPos(CRect rcDel, BOOL bFirst)
{
	if (m_listLinesAbove.size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = m_listLinesAbove.begin(); it != m_listLinesAbove.end(); it ++)
		{
			CTechKLineImp* pImpCur = *it;
			if (pImpCur != NULL )
			{
				CRect rcImp = pImpCur->GetRect();
				if (!bFirst)
				{
					rcImp.top += rcDel.Height();
				}
				rcImp.bottom += rcDel.Height();
				pImpCur->SetRect(rcImp);
			}
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//CDrawUnitPosCtrlMgr
CDrawUnitPosCtrlMgr::CDrawUnitPosCtrlMgr(CTechKLineMgr* pMgr, IRegionNode* pNode)
{
	m_pMgr = pMgr;
	m_nType = 0;
	m_bLastSetCursor = FALSE;
	m_hCursor = LoadCursor(NULL, IDC_SIZENS);
	m_hCursorOld = NULL;
	m_nTotalHeight = 0;
	m_nTotalWidth = 0;
	m_pConfig = NULL;
	m_pRegionNode = pNode;
}

CDrawUnitPosCtrlMgr::~CDrawUnitPosCtrlMgr()
{
	EmptyOldCtrl();
}

BOOL CDrawUnitPosCtrlMgr::Init()
{
	if (m_pConfig == NULL)
	{
		m_pConfig = (IHSKLinePosition*)CWinnerApplication::GetObject(OBJ_KLINEPOSCTRL);
	}
	if (m_pConfig)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDrawUnitPosCtrlMgr::Save(int nType)
{
	if (m_pConfig != NULL && m_listUnitLink.size() > 0)
	{
		int i = 0;
		CStringArray ayStrNames;
		for(std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
		{
			CDrawUnitPositonCtrl* pUnit = *it;
			if (pUnit != NULL )
			{
				ayStrNames.RemoveAll();
				if (i == 0)	//顶部
				{
					int nSize = pUnit->GetLinksName(nType, ayStrNames, TRUE);
					if (nSize > 0)
					{
						m_pConfig->SetTechLines(nType, ayStrNames, TRUE);
					}
				}
				ayStrNames.RemoveAll();
				m_pConfig->SetPosCtrolRatio(nType, pUnit->GetRatio());
				int nSize = pUnit->GetLinksName(nType, ayStrNames, FALSE);
				if (nSize > 0)
				{
					m_pConfig->SetTechLines(nType, ayStrNames, FALSE);
				}
				i ++;
			}
		}
		ayStrNames.RemoveAll();
		m_pConfig->SetTechLines(nType, ayStrNames, FALSE, TRUE);
	}
	return FALSE;
}

void CDrawUnitPosCtrlMgr::EmptyOldCtrl()
{
	while(m_listUnitLink.size() > 0)
	{
		CDrawUnitPositonCtrl* pUnit = m_listUnitLink.front();
		m_listUnitLink.pop_front();
		if (pUnit != NULL)
		{
			delete pUnit;
			pUnit = NULL;
		}
	}
}

BOOL CDrawUnitPosCtrlMgr::MouseMove(UINT nFlags, CPoint point)
{
	BOOL bInPosCtrl = FALSE;
	for(std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
	{
		CDrawUnitPositonCtrl* pUnit = *it;
		if (pUnit != NULL && pUnit->GetRect().PtInRect(point))
		{
			bInPosCtrl = TRUE;
			break;
		}
	}
	BOOL bRet = FALSE;
	if(bInPosCtrl && !m_bLastSetCursor)
	{
		bRet = TRUE;
		m_bLastSetCursor = TRUE;
		m_hCursorOld = SetCursor(m_hCursor);
	}
	else if (m_bLastSetCursor)
	{
		bRet = TRUE;
		SetCursor(m_hCursorOld);
		m_bLastSetCursor = FALSE;
	}
	return bRet;
}

BOOL CDrawUnitPosCtrlMgr::MouseDown(UINT nFlags, CPoint point)
{
	for(std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
	{
		CDrawUnitPositonCtrl* pUnit = *it;
		if (pUnit != NULL)
		{
			BOOL bRet = pUnit->MouseDown(nFlags, point);
			if (bRet)
			{
				return bRet;
			}
		}
	}
	return FALSE;
}

BOOL CDrawUnitPosCtrlMgr::Update(int nTotalHeight, int nTotalWidth, BOOL bUseOldRatio)
{
	m_nTotalHeight = nTotalHeight;
	m_nTotalWidth = nTotalWidth;
	int nOldTotalHeight = 0;
	int nOldTotalWidth = 0;
	if (m_pMgr != NULL)
	{
		short nType = m_pMgr->GetKLineType();
		EmptyOldCtrl();
		CRect rcLast(0, 0, 0, 0);
		std::list<CTechKLineImp*>* pList = m_pMgr->GetCurrentTechLines();
		if (pList != NULL && pList->size() > 0)
		{
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL )
				{
					CRect rcImpCur = pImpCur->GetRect();
					if (nOldTotalHeight < rcImpCur.bottom)
					{
						nOldTotalHeight = rcImpCur.bottom;
					}
					if (nOldTotalWidth < rcImpCur.right)
					{
						nOldTotalWidth = rcImpCur.right;
					}
				}
			}
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL )
				{
					AddLinks(pImpCur, bUseOldRatio ? nOldTotalHeight : m_nTotalHeight);
				}
			}
			if(m_listUnitLink.size() > 0)
			{
				CDrawUnitPositonCtrl* pUnitBack = m_listUnitLink.back();
				m_listUnitLink.pop_back();
				if (pUnitBack != NULL)
				{
					delete pUnitBack;
					pUnitBack = NULL;
				}
			}
		}
	}
	if (nOldTotalHeight != m_nTotalHeight || m_nTotalWidth != nOldTotalWidth)
	{
		OnResetRect(CRect(0, 0, m_nTotalWidth, m_nTotalHeight));
	}
	return FALSE;
}

void CDrawUnitPosCtrlMgr::AddLinks(CTechKLineImp* pImp, int nSetHeight)
{
	if (pImp == NULL)
	{
		return ;
	}
	CRect rcImp = pImp->GetRect();
	CDrawUnitPositonCtrl* pFindCtrl = NULL;
	if (m_listUnitLink.size() > 0 )
	{
		for (std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
		{
			CDrawUnitPositonCtrl* pCtrl = *it;
			if (pCtrl != NULL)
			{
				CRect rcCtrl = pCtrl->GetRect();
				int nMidPosY = (rcCtrl.top + rcCtrl.bottom) / 2;
				if (nMidPosY == rcImp.BottomRight().y)		//下对齐，标准
				{
					pFindCtrl = pCtrl;
					break;
				}
				else if (nMidPosY == rcImp.TopLeft().y)	//上对齐
				{
					pCtrl->AddLinks(pImp, FALSE);
				}
			}
		}
	}
	if (pFindCtrl == NULL)
	{
		pFindCtrl = new CDrawUnitPositonCtrl(m_pMgr, nSetHeight);
		CRect rcCtrl = CRect(rcImp.left, rcImp.bottom - CALC_POSITION_SPLIT, rcImp.right, rcImp.bottom + CALC_POSITION_SPLIT);
		pFindCtrl->SetRect(rcCtrl);
		if (rcImp.bottom == nSetHeight)
		{
			pImp->SetIsBottom(TRUE);
		}
		m_listUnitLink.push_back(pFindCtrl);
	}
	if (pFindCtrl != NULL)
	{
		pFindCtrl->AddLinks(pImp, TRUE);
	}
}

int CDrawUnitPosCtrlMgr::OnResetRect(CRect rcNew)
{
	if (m_pMgr != NULL)
	{
		int nCount = 0;
		std::list<CTechKLineImp*>* pList = m_pMgr->GetCurrentTechLines();

		if (pList != NULL && pList->size() > 0)
		{
			float fCellWidth = 0;
			CTechKLineImp* pImpTop = pList->front();
			if (pImpTop != NULL)
			{
				fCellWidth = pImpTop->GetHorizCellWidth();
			}
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL )
				{
					if(ResetLinksRect(pImpCur, rcNew, fCellWidth))
						nCount ++;
				}
			}
		}
		return nCount;
	}
	return 0;
}

BOOL CDrawUnitPosCtrlMgr::ResetLinksRect(CTechKLineImp* pImp, CRect rcNew, float fCellWidth)
{
	if (m_listUnitLink.size() > 0 && pImp != NULL)
	{
		int nTotalHeight = rcNew.Height();
		int nTop = -1, nBottom = -1;
		for (std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
		{
			CDrawUnitPositonCtrl* pCtrl = *it;
			if (pCtrl != NULL)
			{
				if(nTop < 0)
					nTop = pCtrl->GetResetPostion(pImp, TRUE, nTotalHeight);
				if (nBottom < 0)
				{
					nBottom = pCtrl->GetResetPostion(pImp, FALSE, nTotalHeight);
				}
				if (nTop >= 0 && nBottom >= 0)
				{
					break;
				}
			}
		}
		if (nTop >= 0 && nBottom == -1)	//这个图元是底部图元
		{
			nBottom = rcNew.bottom;
		}
		if (nTop >= 0 && nBottom >= 0)
		{
			CRect rcImp = CRect(rcNew.left, nTop, rcNew.right, nBottom);
			pImp->SetRect(rcImp);
			if (nBottom == m_nTotalHeight)
			{
				pImp->SetIsBottom(TRUE);
			}
//			pImp->SetHorizCellWidth(fCellWidth);
			return TRUE;
		}
	}
	return FALSE;
}

int	CDrawUnitPosCtrlMgr::CreateLines(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth)
{
	EmptyOldCtrl();
	int nRet(0);
	if (pListTarget != NULL && pListTarget->size() <= 0)
	{
		BOOL bCreateFromFile = TRUE;
		CStringArray strLines;
		if (m_pConfig == NULL )
		{
			bCreateFromFile = FALSE;
		}
		else 
		{
			int nRet = m_pConfig->GetTechLines(nType, strLines, TRUE);
			if(nRet <= 0 || strLines.GetSize() <= 0)
				bCreateFromFile = FALSE;
		}
		if (!bCreateFromFile)
		{
			nRet = CreateDefault(nType, pListTarget, fCellWidth);
		}
		else if (m_pConfig != NULL)
		{
			int nSize = strLines.GetSize();
			float fRaito = m_pConfig->GetPosCtrolRatio(nType);
			CTechKLineImp* pImpLast = NULL;

			while (fRaito > 0.01)
			{
				CDrawUnitPositonCtrl* pPosCtrl = new CDrawUnitPositonCtrl(m_pMgr, m_nTotalHeight);
				pPosCtrl->SetRatio(fRaito);
				m_listUnitLink.push_back(pPosCtrl);
				BOOL bAddLine = FALSE;

				if (pImpLast != NULL)
				{
					pPosCtrl->AddLinks(pImpLast, TRUE);
				}
				else
				{//创建主图
					for (int i = 0; i < nSize; i ++)
					{
						CString strLineName = strLines.GetAt(i);
						if (!strLines.IsEmpty())
						{
							CTechKLineImp* pImpCur = CreateFormFile(nType, strLineName, fCellWidth, bAddLine, FALSE);
							if(pImpCur != NULL)
							{
								pListTarget->push_back(pImpCur);
								pPosCtrl->AddLinks(pImpCur, TRUE);
							}
							bAddLine = TRUE;
						}
					}
				}
				strLines.RemoveAll();
				nRet = m_pConfig->GetTechLines(nType, strLines, FALSE);
				ASSERT(nRet == 1);		//主图后面的只允许一图
				CString strLineName = strLines.GetAt(0);
				float fCellWidthEx = fCellWidth;
				pImpLast = CreateFormFile(nType, strLineName, fCellWidthEx, FALSE, FALSE);
				if (pImpLast != NULL)
				{
					pListTarget->push_back(pImpLast);
					pPosCtrl->AddLinks(pImpLast, FALSE);
				}
				fRaito = m_pConfig->GetPosCtrolRatio(nType);
			}
			if (pImpLast != NULL)
			{
				pImpLast->SetIsBottom(TRUE);
			}
		}
		nRet = pListTarget->size();
	}
	return nRet;
}

CTechKLineImp* CDrawUnitPosCtrlMgr::CreateFormFile(int nType, CString strName, float fCellWidth, BOOL bAddLine, BOOL bBottom)
{
	CTechKLineImp* pImpRet = NULL;
	if (strName == _T("MAIN"))
	{
		if (nType == PKLINE_TYPE_K)
		{
			pImpRet = new CTechKLineImp(m_pMgr, m_pRegionNode, fCellWidth);
		}
		else if (nType == PKLINE_TYPE_FENSHI)
		{
			pImpRet = new CDrawTStockTrend(m_pMgr, m_pRegionNode, fCellWidth);
		}
	}
	else if (strName == _T("VOLUMN"))
	{
		pImpRet = new CDrawTStockTrendVolume(m_pMgr, m_pRegionNode, fCellWidth);
	}
	else
	{
		if(nType == PKLINE_TYPE_FENSHI)
		{
			pImpRet = new CDrawTechLine(m_pMgr, m_pRegionNode, fCellWidth, fCellWidth);
			pImpRet->ModifyLinearStyle(AXIS_RIGHTMARGIN , AXIS_BOTTOMMARGIN | AXIS_SOLIDANDDASH | AXIS_TOPMARGIN);
		}
		else
			pImpRet = new CDrawTechLine(m_pMgr, m_pRegionNode, fCellWidth);
		if (pImpRet != NULL)
		{
			pImpRet->SetTeckKLineName(strName);
		}
	}
	if (pImpRet != NULL)
	{
		pImpRet->SetVisible(TRUE);
		pImpRet->SetAddLine(bAddLine);
		pImpRet->SetIsBottom(bBottom);
	}
	return pImpRet;
}

int	CDrawUnitPosCtrlMgr::CreateDefault(int nType, std::list<CTechKLineImp*>* pListTarget, float fCellWidth)
{
	if (pListTarget == NULL)
	{
		return 0;
	}
	if (nType == PKLINE_TYPE_K)
	{
		CTechKLineImp* pImp = new CTechKLineImp(m_pMgr, m_pRegionNode, fCellWidth);
		if (pImp != NULL)
		{
			pImp->SetVisible(TRUE);
			pListTarget->push_back(pImp);
		}
		CDrawTechLine* pDrawTechKLine = new CDrawTechLine(m_pMgr, m_pRegionNode, fCellWidth);
		if (pDrawTechKLine != NULL)
		{
			pDrawTechKLine->SetVisible(TRUE);
			pDrawTechKLine->SetTeckKLineName(_T("MA"));
			pDrawTechKLine->SetAddLine(TRUE);
			pListTarget->push_back(pDrawTechKLine);
		}
		CDrawTechLine* pTVolume = new CDrawTechLine(m_pMgr, m_pRegionNode, fCellWidth);
		if (pTVolume != NULL)
		{
			pTVolume->SetVisible(TRUE);
			pTVolume->SetTeckKLineName(_T("VOL"));
			pTVolume->SetIsBottom(TRUE);
			pListTarget->push_back(pTVolume);
		}
	
		CDrawUnitPositonCtrl* pPosCtrl = new CDrawUnitPositonCtrl(m_pMgr, m_nTotalHeight);
		pPosCtrl->SetRatio((float)0.667);
		m_listUnitLink.push_back(pPosCtrl);
		pPosCtrl->AddLinks(pImp, TRUE);
		pPosCtrl->AddLinks(pDrawTechKLine, TRUE);
		pPosCtrl->AddLinks(pTVolume, FALSE);
	}
	else if (PKLINE_TYPE_FENSHI == nType)
	{
		CDrawTStockTrend* pImp = new CDrawTStockTrend(m_pMgr, m_pRegionNode, fCellWidth);
		if (pImp != NULL)
		{
			pImp->SetVisible(TRUE);
			pListTarget->push_back(pImp);
		}
		CDrawTStockTrendVolume* pTVolume = new CDrawTStockTrendVolume(m_pMgr, m_pRegionNode, fCellWidth);
		if (pTVolume != NULL)
		{
			pTVolume->SetVisible(TRUE);
			pTVolume->SetIsBottom(TRUE);
			pListTarget->push_back(pTVolume);
		}
		CDrawUnitPositonCtrl* pPosCtrl = new CDrawUnitPositonCtrl(m_pMgr, m_nTotalHeight);
		pPosCtrl->SetRatio((float)0.667);
		m_listUnitLink.push_back(pPosCtrl);
		pPosCtrl->AddLinks(pImp, TRUE);
		pPosCtrl->AddLinks(pTVolume, FALSE);
	}
	return pListTarget->size();
}

int CDrawUnitPosCtrlMgr::OnDeleteImp(CTechKLineImp* pImpDel)
{
	std::list<CTechKLineImp*>* pList = m_pMgr->GetCurrentTechLines();
	if (pList != NULL && pList->size() > 0)
	{
		for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
		{
			for (std::list<CTechKLineImp*>::iterator it = pList->begin(); it != pList->end(); it ++ )
			{
				CTechKLineImp* pImpCur = *it;
				if (pImpCur != NULL && pImpCur == pImpDel)
				{
					CDrawUnitPositonCtrl* pPosCtrl = FindBelowLinkedCtrl(pImpDel);
					pList->erase(it);
					if(pPosCtrl != NULL)
					{

					}
					delete pImpCur;
					pImpCur = NULL;
					break;
				}
			}
		}
	}
	return 0;
}

CDrawUnitPositonCtrl* CDrawUnitPosCtrlMgr::FindBelowLinkedCtrl(CTechKLineImp* pImp)
{
	BOOL bLinkedImpNum = 0;
	CDrawUnitPositonCtrl* pRetCtrl = NULL;
	if (m_listUnitLink.size() > 0 && pImp != NULL)
	{
		for (std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
		{
			CDrawUnitPositonCtrl* pUnit = *it;
			if (pUnit != NULL )
			{
				bLinkedImpNum = pUnit->IsInMyCtrl(pImp, FALSE);
				if (bLinkedImpNum > 0)
				{
					pRetCtrl = pUnit;
					break;
				}
			}
		}
	}
	if (bLinkedImpNum == 1 && pRetCtrl != NULL && pImp != NULL)		//需要重新排列位置
	{
		CRect rcImp = pImp->GetRect();
		if (!rcImp.IsRectEmpty())
		{
			BOOL bFirst = TRUE;
			for (std::list<CDrawUnitPositonCtrl*>::iterator it = m_listUnitLink.begin(); it != m_listUnitLink.end(); it ++)
			{
				CDrawUnitPositonCtrl* pUnit = *it;
				if (pUnit != NULL)
				{
					pUnit->ResetLinkedImpPos(rcImp, bFirst);
					bFirst = FALSE;
					if (pUnit == pRetCtrl)	//循环到此为止
					{
						break;
					}
				}
			}
			Update(m_nTotalHeight, m_nTotalWidth);
		}
	}
	return pRetCtrl;
}