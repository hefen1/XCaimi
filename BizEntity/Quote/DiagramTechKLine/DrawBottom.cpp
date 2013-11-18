#include "stdafx.h"
#include "DrawBottom.h"
#include "TechKLineMgr.h"
#include "..\..\..\Common\WinnerApplication.h"
#include "..\..\..\Resource\QuoteLang\Resource.h"
#include "..\..\..\Common\QuoteLangMngInterFace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////
//
CDrawBottom::CDrawBottom(CTechKLineMgr* pMgr, IRegionNode* pRegion)
{
	m_pTeckKLineMgr = pMgr;
	m_pRegion = pRegion;
	m_wPeriodType = PERIOD_TYPE_DAY;
	m_strPeriodName = _T("日线");

	m_pMenu = NULL;
	IQuoteLang* pQuoteLang = (IQuoteLang*)CWinnerApplication::GetObject(OBJ_QUOTELANG);
	if (pQuoteLang != NULL)
	{
		HMENU hMenu = pQuoteLang->LoadMenu(IDR_MENU_TECHKLINE_PERIOD);
		if (m_pMenu == NULL && hMenu != NULL)
		{
			m_pMenu = new CMenu;		
			m_pMenu->Attach(::GetSubMenu(hMenu, 0));
		}
	}
	m_hCursor = LoadCursor(NULL, IDC_SIZEWE);
}

CDrawBottom::~CDrawBottom()
{
	if (m_pMenu != NULL)
	{
		m_pMenu->Detach();
		delete m_pMenu;
		m_pMenu = NULL;
	}
	if(m_hCursor != NULL)
	{

	}
}

BOOL CDrawBottom::Create(IDrawBaseParam *pParam)
{
	return TRUE;
}

int	CDrawBottom::Draw(CDC* pDC)
{
	if (m_pTeckKLineMgr != NULL && pDC != NULL && GetVisible())
	{
		pDC->SetBkMode(TRANSPARENT);

		COLORREF clrOld = pDC->SetTextColor(g_hsGraphicsData.clHorTXT.m_lColor);
		CPen pen(g_hsGraphicsData.clChartTick.m_nPenStyle, g_hsGraphicsData.clChartTick.m_nWidth, g_hsGraphicsData.clChartTick.m_lColor);
		CPen *pOldPen = (CPen*)pDC->SelectObject(&pen);
		CFont *pOldFont = pDC->SelectObject(g_hsGraphicsData.ftXAxis.m_pFont);

		char szTime[40] = {0};
		int nStep = 1;
		int yPos = m_rect.top;
		int nLastPosX = -1;
		int nStart = 0, nEnd = 0;
		m_pTeckKLineMgr->GetStartEndPos(nStart, nEnd);
		for (int i = nStart; i < nEnd;  i += nStep)
		{
			i = m_pTeckKLineMgr->GetKLineNextTime(i, szTime, m_wPeriodType);
			if (i == -1)
			{
				break;
			}
			//			int xPos = m_pTeckKLineMgr->GetDataXPos(i);
			int xPos = m_pTeckKLineMgr->GetDataXPos(i - nStart);

			int nCharWidth = pDC->GetTextExtent(szTime, strlen(szTime)).cx; 
			if (nLastPosX > 0 && xPos < (nLastPosX + 2 * nCharWidth + 4))
			{
				continue;
			}
			pDC->MoveTo(xPos, yPos);
			pDC->LineTo(xPos, yPos + 5);
			xPos += 2;
			pDC->TextOut(xPos, yPos + 2, szTime, strlen(szTime));
			nLastPosX = xPos;
		}

		pDC->SetTextColor(g_hsGraphicsData.clMouseMoveMarkBK.GetInvertColor());

		if (!m_strTips.IsEmpty())
		{
			int nRectLeft = m_nHorPosition;
			CSize sizeTip = pDC->GetTextExtent(m_strTips);
			int nRectRight = sizeTip.cx + nRectLeft + 6;
			if (nRectRight > m_rect.right)
			{
				nRectRight = m_nHorPosition;
				nRectLeft = nRectRight - sizeTip.cx - 6;
			}
			int nRectTop = m_rect.top;
			int nRectBottom = nRectTop + sizeTip.cy + 2;
			CRect rcTips = CRect(nRectLeft, nRectTop, nRectRight, nRectBottom);
			pDC->Draw3dRect(rcTips,
				g_hsGraphicsData.clPageBK.GetInvertColor(),g_hsGraphicsData.clPageBK.GetInvertColor());
			rcTips.DeflateRect(1, 1, 1, 1);
			COLORREF clrBkColor = pDC->GetBkColor();
			pDC->FillSolidRect(rcTips, g_hsGraphicsData.clBottomBK.m_lColor);
			pDC->DrawText(m_strTips, rcTips, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			pDC->SetBkColor(clrBkColor);
		}

		pDC->SetTextColor(g_hsGraphicsData.clBottomChoose.m_lColor);
		CRect rect = m_rect;
		//		rect.right = m_pTeckKLineMgr->GetDataXPos(m_nStartPos);
		rect.right = m_pTeckKLineMgr->GetDataXPos(-1);
		pDC->SelectObject(g_hsGraphicsData.ftPeriod.m_pFont);	//add by lxqi	20100305 加入字体配置
		rect.top -= 1;
		rect.right += 1;
		pDC->DrawText(m_strPeriodName + " ▲", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->Draw3dRect(rect, g_hsGraphicsData.clHorLine.m_lColor,g_hsGraphicsData.clVerLine.m_lColor);

		pDC->SetTextColor(clrOld);
		pDC->SelectObject(pOldFont);
		pDC->SelectObject(pOldPen);
		if (pen.m_hObject)
		{
			pen.DeleteObject();
		}
	}
	return 1;
}

void CDrawBottom::UpdatePositon(int nIndex, CPoint pt)
{
	m_nHorPosition = pt.x;
	if (m_pTeckKLineMgr != NULL)
	{
		m_strTips = m_pTeckKLineMgr->FormatTip(nIndex, m_wPeriodType);
	}
}

void CDrawBottom::UpdatePeriodType(WORD wType)
{
	m_wPeriodType = wType;
	m_strPeriodName = _T("日线");
	switch(wType)
	{
	case PERIOD_TYPE_MINUTE0:
		{
			m_strPeriodName = _T("分笔");
			break;
		}
	case PERIOD_TYPE_MINUTE1:
		{
			m_strPeriodName = _T("1分钟");
			break;
		}
	case PERIOD_TYPE_MINUTE5:
		{
			m_strPeriodName = _T("5分钟");
			break;
		}
	case PERIOD_TYPE_MINUTE15:
		{
			m_strPeriodName = _T("15分钟");
			break;
		}
	case PERIOD_TYPE_MINUTE30:
		{
			m_strPeriodName = _T("30分钟");
			break;
		}
	case PERIOD_TYPE_MINUTE60:
		{
			m_strPeriodName = _T("60分钟");
			break;
		}
	case PERIOD_TYPE_MINUTE120:
		{
			m_strPeriodName = _T("120分钟");
			break;
		}
	case PERIOD_TYPE_WEEK:
		{
			m_strPeriodName = _T("周线");
			break;
		}
	case PERIOD_TYPE_MONTH:
		{
			m_strPeriodName = _T("月线");
			break;
		}
	default:
		break;
	}
}

WORD CDrawBottom::GetPeriodType()
{
	return m_wPeriodType;
}

BOOL CDrawBottom::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if (m_rect.PtInRect(point) && m_pTeckKLineMgr != NULL)
	{
		int nLeftMargin = m_pTeckKLineMgr->GetDataXPos(0);
		CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();
		if (nLeftMargin > 0 && point.x < nLeftMargin)
		{
			if (m_pMenu != NULL)
			{
				CPoint ptMenu = point;
				pWnd->ClientToScreen(&ptMenu);
				int nRet = m_pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, ptMenu.x, ptMenu.y, pWnd);
				if (nRet > 0)
				{
					CString strMenuString;
					m_pMenu->GetMenuString(nRet, strMenuString, MF_BYCOMMAND);
					strMenuString = strMenuString.Left(strMenuString.Find("图"));
					if (strMenuString == "TICK")
					{
						strMenuString = _T("分笔");
					}
					return TrackPopUpMenu(nRet, strMenuString);
				}
			}
		}
		else if (nLeftMargin > 0 && pWnd != NULL)
		{
			MSG msg;
			CPoint pt;

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
						if(pt == ptLast)
							break;

						SetCursor(m_hCursor);
						if(m_pTeckKLineMgr->MovePosition(CPoint(ptLast.x - pt.x, 0)) > 0)
						{
							ptLast = pt;
						}
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
		}
		return TRUE;
	}

	return FALSE;
}

int CDrawBottom::TrackPopUpMenu(int nTrackID, CString strMenuString)
{
	CString strOldPeroidName = m_strPeriodName;
	m_strPeriodName = strMenuString;
	if (m_pTeckKLineMgr != NULL && nTrackID > 0)
	{
		if(IDR_TECHKLINE_FENSHI == nTrackID)
		{
			m_strPeriodName = strOldPeroidName;
			m_pTeckKLineMgr->ChangeType();
		}
		else
		{
			m_wPeriodType = FormatPeriod(nTrackID);
			m_pTeckKLineMgr->SwitchPage(m_wPeriodType);
		}
	}
	return 0;
}

WORD CDrawBottom::FormatPeriod(int nTrackID)
{
	WORD wPeriod = PERIOD_TYPE_DAY;
	switch(nTrackID)
	{
	case IDR_TECHKLINE_TICK:
		{
			wPeriod = PERIOD_TYPE_MINUTE0;
			break;
		}
	case IDR_TECHKLINE_ONEMINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE1;
			break;
		}
	case IDR_TECHKLINE_FIVEMINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE5;
			break;
		}
	case IDR_TECHKLINE_FIFTEENMINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE15;
			break;
		}
	case IDR_TECHKLINE_THIRTYMINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE30;
			break;
		}
	case IDR_TECHKLINE_SIXTYMINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE60;
			break;
		}
	case IDR_TECHKLINE_180MINUTE:
		{
			wPeriod = PERIOD_TYPE_MINUTE120;
			break;
		}
	case IDR_TECHKLINE_PERIODWEEK:
		{
			wPeriod = PERIOD_TYPE_WEEK;
			break;
		}
	case IDR_TECHKLINE_PERIODMONTH:
		{
			wPeriod = PERIOD_TYPE_MONTH;
			break;
		}
	default:
		break;
	}
	return wPeriod;
}

int	CDrawBottom::HitTest(CPoint pt)
{
	if (m_rect.PtInRect(pt) && m_pTeckKLineMgr != NULL)
	{
		int nLeftMargin = m_pTeckKLineMgr->GetDataXPos(0);
		if (nLeftMargin > 0 && pt.x > nLeftMargin)	//显示手型
		{
			CWnd* pWnd = m_pTeckKLineMgr->GetParentWnd();
			if (m_hCursor != NULL)
			{
				SetCursor(m_hCursor);
			}
		}
	}
	return 0;
}