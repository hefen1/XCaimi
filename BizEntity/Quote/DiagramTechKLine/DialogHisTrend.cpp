// DialogHisTrend.cpp : implementation file
//

#include "stdafx.h"
#include "DiagramTechKLine.h"
#include "DialogHisTrend.h"
#include "DrawTStockTrend.h"
#include "TechKLineMgr.h"
#include "..\..\..\Common\WinnerApplication.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDialogHisTrend dialog

IMPLEMENT_DYNAMIC(CDialogHisTrend, CDialog)

CDialogHisTrend::CDialogHisTrend(StockUserInfo* pStockUserInfo, IRegionNode* pRegion, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHisTrend::IDD, pParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_pRegion = pRegion;
	m_stkInfo.Copy(pStockUserInfo);
	m_lDate = 0;
	m_pTeckKLineMgr = NULL;
	m_pHisTrendRTable = NULL;

	m_penBorder.lopnStyle = g_hsGraphicsData.clGridLine.m_nPenStyle;
	m_penBorder.lopnWidth.x = g_hsGraphicsData.clGridLine.m_nWidth;;
	m_penBorder.lopnWidth.y = g_hsGraphicsData.clGridLine.m_nWidth;;    
	m_penBorder.lHorColor = g_hsGraphicsData.clGridLine.m_lColor;
	m_penBorder.lVerColor = g_hsGraphicsData.clGridLine.m_lColor;

	Create(IDD, pParent);
}

CDialogHisTrend::~CDialogHisTrend()
{
}

void CDialogHisTrend::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogHisTrend, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_HISTREND_PREV, OnPrevTrend)
	ON_COMMAND(ID_HISTREND_NEXT, OnNextTrend)
END_MESSAGE_MAP()


// CDialogHisTrend message handlers

BOOL CDialogHisTrend::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CenterWindow();
	int nRequestPeriod = 241;
	if (m_listFenshi.size() <= 0)
	{
		CDrawTStockTrend* pImp = new CDrawTStockTrend(this, m_pRegion, nRequestPeriod);
		if (pImp != NULL)
		{
			pImp->SetVisible(TRUE);
			m_listFenshi.push_back(pImp);
		}
		CDrawTStockTrendVolume* pTVolume = new CDrawTStockTrendVolume(this, m_pRegion, nRequestPeriod);
		if (pTVolume != NULL)
		{
			pTVolume->SetVisible(TRUE);
			m_listFenshi.push_back(pTVolume);
		}
	}
	if(m_pHisTrendRTable == NULL)
	{
		m_pHisTrendRTable = new CDrawHisTrendRTable();
		m_pHisTrendRTable->SetVisible(TRUE);
	}
	SetChildRect();
	return TRUE; 
}

void CDialogHisTrend::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
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
	if (m_pHisTrendRTable != NULL)
	{
		delete m_pHisTrendRTable;
		m_pHisTrendRTable = NULL;
	}
	CDialog::PostNcDestroy();
	delete this;
}

void CDialogHisTrend::SetChildRect()
{
	CRect rc;
	GetClientRect(&rc);
	if (!rc.IsRectEmpty())
	{
		int nSplitY = rc.Height();
		int nSplitX = rc.Width();
		CRect rcLeft = rc;
		rcLeft.right = rcLeft.left + nSplitX * 3 / 4;
		CRect rcLeftTop = rcLeft;
		rcLeftTop.bottom = rcLeftTop.top + 2 * nSplitY / 3;
		CRect rcLeftBottom = rcLeft;
		rcLeftBottom.top = rcLeftBottom.top + 2 * nSplitY / 3;
		CRect rcRight = rc;
		rcRight.left = rcLeft.right + 2;
		rcRight.top += 12;

		int i = 0;
		for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL )
			{
				if(i == 0)
					pImpl->SetRect(rcLeftTop);
				else if (i == 1)
				{
					pImpl->SetRect(rcLeftBottom);
				}
			}
			i ++;
		}
		if (m_pHisTrendRTable != NULL)
		{
			m_pHisTrendRTable->SetRect(rcRight);
		}
	}
}

void CDialogHisTrend::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(cx > 0 && cy > 0)
	{
		SetChildRect();
	}
	// TODO: Add your message handler code here
}

void CDialogHisTrend::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	if (!rc.IsRectEmpty())
	{
		dc.FillSolidRect(&rc, RGB(0, 0, 0));
		
		CPen pen;
		if (pen.CreatePenIndirect(&m_penBorder.GetPen(HsLOGPEN::ver)))
		{
			int nX = rc.Width() * 3 / 4;
			CPen* pOldPen = (CPen*)dc.SelectObject(&pen);
			dc.MoveTo(nX, rc.top);
			dc.LineTo(nX, rc.bottom);
			dc.SelectObject(pOldPen);
		}
		if(pen.m_hObject != NULL)
		{
			pen.DeleteObject();
		}

		for (std::list<CTechKLineImp*>::iterator it = m_listFenshi.begin(); it != m_listFenshi.end(); it ++)
		{
			CTechKLineImp* pImpl = (CTechKLineImp*)*it;
			if (pImpl != NULL && pImpl->GetVisible())
			{
				pImpl->Draw(&dc);
			}
		}
		if (m_pHisTrendRTable != NULL)
		{
			m_pHisTrendRTable->Draw(&dc);
		}
	}
}
void CDialogHisTrend::SetMainMgr(CTechKLineMgr* pMgr)
{
	m_pTeckKLineMgr = pMgr;
}

void CDialogHisTrend::UpdateStock(StockUserInfo* pStockUserInfo)
{
	m_stkInfo.Copy(pStockUserInfo);
}
//////////////////////////////////////////////////////////////////////////
//interface
StockUserInfo* CDialogHisTrend::GetStockUserInfo()
{
	return &m_stkInfo;
}

void CDialogHisTrend::SetStockType(StockType* pStockType)
{
	memcpy(&m_stockType, pStockType, sizeof(StockType));
}

void CDialogHisTrend::GetTopMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	
}

int	CDialogHisTrend::DrawTitle(void* pRefDraw)
{
	return 0;
}

short CDialogHisTrend::GetKLineType()
{
	return PKLINE_TYPE_FENSHI;
}

CWnd* CDialogHisTrend::GetParentWnd()
{
	return this;
}

void CDialogHisTrend::SetHisTrendDate(long lDate)
{
	if (m_lDate == lDate)
	{
		return;
	}
	m_lDate = lDate;
	if (m_pRegion && m_lDate > 0)
	{
		IRequestCollector* pRequestCollector = m_pRegion->GetRequestCollector();
		if (pRequestCollector != NULL)
		{
			int nSize = sizeof(ReqHisTrend) - sizeof(CodeInfo);
			if(nSize % sizeof(CodeInfo))
			{
				nSize += sizeof(CodeInfo);
			}
			int nAskLen = sizeof(AskData) + nSize;
			AskData* pakData       = (AskData*)new char[nAskLen];
			memset(pakData, 0, nAskLen);
			pakData->m_nType	   = RT_HISTREND;
			pakData->m_nSize	   = (nSize / sizeof(CodeInfo)) + 1;
			// 设置请求包数据
			ReqHisTrend* pReqHisTrend = (ReqHisTrend*)pakData->m_pCode;
			memcpy(&pReqHisTrend->m_ciStockCode, &m_stkInfo.m_ciStockCode, sizeof(CodeInfo));
			pReqHisTrend->m_lDate = m_lDate;	
			pRequestCollector->AddRequest((char*)pakData, nAskLen);
			delete []pakData;
		}
		m_pRegion->SendRequest(pRequestCollector);
		pRequestCollector->Release();
		m_dataFenshi.Init(&m_stkInfo, m_dataFenshi.m_nOneDayDataNum + 1, FALSE);
	}
}

int	CDialogHisTrend::OnUpdateData(DataHead* pData, StockDay* pStockDayData, long lPreClose, long lFinaceTotalHand)
{
	if(RT_HISTREND == pData->m_nType)
	{
		m_dataFenshi.Refresh();
		AnsHisTrend * pTendData = (AnsHisTrend *)pData;
		int nDataValLen = 0;
		int nTrendNum = m_dataFenshi.UpdataHisTrend(pTendData, nDataValLen);
		m_nStartPos = 0;
		m_lDataVauleLen = nDataValLen;
		m_nEndPos = nTrendNum;
		m_lDate = pStockDayData->m_lDate;

		if (m_pHisTrendRTable != NULL)
		{
			m_pHisTrendRTable->UpdateData(&m_stkInfo, pStockDayData, lPreClose, lFinaceTotalHand);
		}

		char szText[256] = {0};
//		CGeneralHelper::LongToString(pStockDayData->m_lDate % 1000000, 0, 1, -6, szText); 
		sprintf(szText, "%d", pStockDayData->m_lDate);
		CString strWeek = " / " + CGeneralHelper::GetWeekDayString(pStockDayData->m_lDate);
		strcpy(szText,(char*)(LPCTSTR)(CString(szText) + strWeek));

		strcat(szText, _T(" - [历史回忆(PageUp前一日 PageDown后一日)]"));
		SetWindowText(szText);
		Invalidate(FALSE);
	}
	return 0;
}

void CDialogHisTrend::OnPrevTrend()
{
	if (m_pTeckKLineMgr)
	{
		m_pTeckKLineMgr->MoveCursorPos(FALSE);
		m_pTeckKLineMgr->ShowHisTrend();
	}
}

void CDialogHisTrend::OnNextTrend()
{
	if (m_pTeckKLineMgr)
	{
		m_pTeckKLineMgr->MoveCursorPos(TRUE);
		m_pTeckKLineMgr->ShowHisTrend();
	}
}

void CDialogHisTrend::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_NEXT )
	{
		OnNextTrend();
	}
	else if (nChar == VK_PRIOR)
	{
		OnPrevTrend();
	}
	
	return CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDialogHisTrend::GetMaxMinData(HSDouble& dMax, HSDouble& dMin, HSDouble& dOpenPrice)
{
	GetTopMaxMinData(dMax, dMin, dOpenPrice);
}