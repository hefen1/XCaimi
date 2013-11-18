#include "stdafx.h"
#include "DrawInfo.h"
#include "..\..\..\Common\NetEngineInterface.h"
#include "TechKLineMgr.h"
#include "GraphicsData.h"
#include "..\DiagramFoundation\HSCalculate.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDT_INFOSHOW_DELAY	200
//////////////////////////////////////////////////////////////////////////
//InfoIndexEx
InfoIndexEx::InfoIndexEx()
{
	memset(this, 0, sizeof(InfoIndexEx));
}

BOOL InfoIndexEx::Copy(InfoIndex* pInfo)
{
	BOOL bRet = InfoIndex::Copy(pInfo);
	return bRet;
}

void InfoIndexEx::SetRect(CRect rc/* = CRect(0, 0, 0, 0)*/)
{
	m_rect = rc;
}
//////////////////////////////////////////////////////////////////////////
//CDrawInfo
CDrawInfo::CDrawInfo(CTechKLineMgr* pMgr, IHsInfo* pInfo, IRegionNode* pNode)
{
	m_pMgr = pMgr;
	m_pRegion = pNode;
	m_strInfoPath  = INFO_PATH_KEY_TECH;
	m_pInfo = pInfo;
	m_nSelInfoIndex = -1;
	m_rcInvalidateOld = CRect(0, 0, 0, 0);
	m_nTipTimeID = 0;
	m_pDlgShowHtml = NULL;
	m_nItemHeight = 0;
}

CDrawInfo::~CDrawInfo()
{
	EmptyData();
	EmptyChuquanData();
	if(m_pDlgShowHtml != NULL && m_pDlgShowHtml->m_hWnd != NULL && ::IsWindow(m_pDlgShowHtml->m_hWnd))
	{
		m_pDlgShowHtml->DestroyWindow();
	}
	if(m_pDlgShowHtml != NULL)
	{
		delete m_pDlgShowHtml;
		m_pDlgShowHtml = NULL;
	}
}

int CDrawInfo::EmptyData()
{
	int nRet = m_ayInfoIndex.GetSize();
	while (m_ayInfoIndex.GetSize() > 0)
	{
		InfoIndex* pInfo = m_ayInfoIndex.GetAt(0);
		m_ayInfoIndex.RemoveAt(0);
		if (pInfo)
		{
			delete pInfo;
			pInfo = NULL;
		}
	}
	m_nSelInfoIndex = -1;
	m_ayInfoIndexSelect.RemoveAll();
	return nRet;
}

int	CDrawInfo::EmptyChuquanData()
{
/*	while (m_ayInfoChuquan.GetSize() > 0)
	{
		InfoIndex* pInfo = m_ayInfoChuquan.GetAt(0);
		m_ayInfoChuquan.RemoveAt(0);
		if (pInfo)
		{
			delete pInfo;
			pInfo = NULL;
		}
	}*/
	return 0;
}

BOOL CDrawInfo::GetRequestData(IRequestCollector* pReqCollector,StockUserInfo* pStockInfo, BOOL bForce)
{
	if (pReqCollector == NULL || pStockInfo == NULL)
	{
		return FALSE;
	}
	if( !m_stkInfo.m_ciStockCode.CompareCode(&pStockInfo->m_ciStockCode) )
	{//保存请求的股票信息
		m_stkInfo.Copy(pStockInfo);
		EmptyData();
	}
	else //相同代码，不请求第二次
	{
		return FALSE;
	}
	AskData* pakData = NULL;
	short  lAskLen = 0;

	int nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 
		((sizeof(ReqTextData) % sizeof(CodeInfo))?1:0);

	lAskLen = sizeof(AskData) + (nCodeCount - 1)* sizeof(CodeInfo);
	pakData = (AskData*)new char[lAskLen];
	memset(pakData, 0, lAskLen);
	CString strRefName = INFO_PATH_INI_FILE;
	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_TEXTDATAWITHINDEX_NEGATIVE;
	pakData->m_cOperator = GET_SRVTYPE_BYNAME(strRefName);	
	memcpy(&pakData->m_nPrivateKey.m_pCode, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo));
	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	CString strFileName,strLocalPath,strLocalFileName; // 文件名
	CString strCFg;
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, m_strInfoPath, &strFileName, &strLocalPath, m_pInfo->GetInfoProvider()); 
	// 从文件读取的数据拷贝到请求包中
	strncpy(pDataReq->m_sMarkData.m_cFilePath, strFileName, sizeof(pDataReq->m_sMarkData.m_cFilePath));

	strCFg = CAnalysisTextData::GetUnionString(m_strInfoPath, "", "", strRefName); 
	strncpy(pDataReq->m_sMarkData.m_szInfoCfg, strCFg, sizeof(pDataReq->m_sMarkData.m_szInfoCfg));

	pReqCollector->AddRequest((char*)pakData,lAskLen);
	delete[] pakData;

	return TRUE;	
}

int	CDrawInfo::OnUpdateData(DataHead* pData)
{
	if (pData != NULL && pData->m_nType == RT_TEXTDATAWITHINDEX_NEGATIVE)
	{
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return 0;
		}
		AnsTextData* pTextData = (AnsTextData*)pData;
		if (pTextData->m_nSize <= 0)
		{
			return 0;
		}
		int nCount(0);
		InfoIndex* pRefIndex = CAnalysisTextData::GetInfoIndexData(pTextData, pTextData->m_nSize, nCount);
		if (pRefIndex != NULL)
		{
			// 加入到指针
			InfoIndexEx* pIndexEx = NULL;
			for(int i = 0; i < nCount; i++,pRefIndex++ )
			{
				pIndexEx = new InfoIndexEx;
				pIndexEx->Copy(pRefIndex);
				m_ayInfoIndex.Add(pIndexEx);
			}
		}
	}
	else if (pData != NULL && pData->m_nType == RT_BYINDEXRETDATA )
	{
		if ( memcmp(&m_stkInfo.m_ciStockCode, &pData->m_nPrivateKey, sizeof(CodeInfo)) != 0)
		{
			return 0;
		}
		AnsTextData* pTextData = (AnsTextData*)pData;
		if (pTextData->m_nSize <= 0 || m_nSelInfoIndex < 0 || m_nSelInfoIndex >= m_ayInfoIndex.GetSize())
		{// 王超 修改 && 改成 || 
			return FALSE;
		}
		InfoIndex* pInfoIndex = m_ayInfoIndex.GetAt(m_nSelInfoIndex);
		CString strText(pTextData->m_cData, pTextData->m_nSize);
		CString strUrl = CHSCalculate::CreateHtml(strText, pInfoIndex);
		if (!strUrl.IsEmpty())
		{
			if(m_pDlgShowHtml == NULL || m_pDlgShowHtml->m_hWnd == NULL || !::IsWindow(m_pDlgShowHtml->m_hWnd))
			{
				CWnd* pWnd = m_pMgr->GetParentWnd();
				m_pDlgShowHtml = new CDHtmlShowDlg(pWnd);
			}
			if (m_pDlgShowHtml != NULL)
			{
				m_pDlgShowHtml->SetUserUrl(strUrl);
				m_pDlgShowHtml->SetCaption(pInfoIndex->m_cTitle);
				m_pDlgShowHtml->Show();
			}
		}
	}
	return 0;
}

int CDrawInfo::DrawInfo(CDC* pDC)
{
	int nSelectedCount = m_ayInfoIndexSelect.GetSize();
	if (pDC != NULL && nSelectedCount > 0)
	{
		CSize sizeText(0, 0);
		InfoIndexEx* pInfoStart = m_ayInfoIndexSelect.GetAt(0);
		for (int i = 0; i < nSelectedCount; i ++)
		{
			InfoIndexEx* pInfo = m_ayInfoIndexSelect.GetAt(i);
			if (pInfo != NULL)
			{
				CRect rc(0, 0, 0, 0);
				pDC->DrawText(pInfo->m_cTitle, strlen(pInfo->m_cTitle), rc, DT_SINGLELINE | DT_CALCRECT | DT_LEFT);

				if (sizeText.cx < rc.Width())
				{
					sizeText.cx = rc.Width();
				}
				sizeText.cy += rc.Height();
			}
		}
		int nLeft = pInfoStart->m_rect.left - 1;
		int nRight = pInfoStart->m_rect.left + sizeText.cx + 1;
		int nTop = pInfoStart->m_rect.top - 1;
		int nBottom = pInfoStart->m_rect.top + sizeText.cy + 1;
		if (nRight > m_rect.right )
		{
			nRight = m_rect.right;
			nLeft = nRight - sizeText.cx;
			if (nLeft < 0)
			{
				nLeft = 0;
			}
		}
		if (nBottom > m_rect.bottom)
		{
			nBottom = m_rect.bottom;
			nTop = nBottom = sizeText.cy;
			if (nTop < 0)
			{
				nTop = 0;
			}
		}
		CRect rcDraw(nLeft, nTop, nRight, nBottom);
		m_rcInvalidateOld = rcDraw;
		pDC->Rectangle(&rcDraw);
		rcDraw.DeflateRect(1, 1, 1, 1);

		m_nItemHeight = sizeText.cy / nSelectedCount;
		CRect rcItem = rcDraw;
		rcItem.bottom = rcItem.top + m_nItemHeight;
		for (int i = 0; i < nSelectedCount; i ++)
		{
			InfoIndexEx* pInfo = m_ayInfoIndexSelect.GetAt(i);
			if (pInfo != NULL)
			{
				if (pInfo->m_bSelected)
				{
					pDC->FillSolidRect(rcItem, g_hsGraphicsData.clInfoHilight.m_lColor);
				}
				else
				{
					pDC->FillSolidRect(rcItem, g_hsGraphicsData.clInfoBk.m_lColor);
				}
				pDC->DrawText( pInfo->m_cTitle, strlen(pInfo->m_cTitle), rcItem, DT_SINGLELINE | DT_LEFT);
			}
			rcItem.top = rcItem.bottom;
			rcItem.bottom = rcItem.top + m_nItemHeight;
		}
	}
	return 0;
}

int	CDrawInfo::Draw(CDC* pDC)
{
	if (m_pMgr == NULL)
		return 0;
	int nInfoSize = m_ayInfoIndex.GetSize();
	if (!m_rect.IsRectEmpty() && nInfoSize > 0)
	{
		EmptyChuquanData();
		int nType = PKLINE_TYPE_FENSHI;
		long lDataLast = 0;
		
		InfoIndexEx* pInfoLast = m_ayInfoIndex.GetAt(nInfoSize - 1);
		if(pInfoLast != NULL)
		{
			lDataLast = pInfoLast->m_lDate;
			nType = m_pMgr->UpdateDrawInfoData(m_ayUnitData, lDataLast);
		}
		int nInfoIndex = 0;
		int nUnitSize = m_ayUnitData.GetSize();
		int nChuquanSize = 0; 
		CArray<ChuQuanData*,ChuQuanData*>* pChuquanData = m_pMgr->GetChuquanData();
		if (pChuquanData != NULL )
		{
			 nChuquanSize = pChuquanData->GetSize();
		}
		int nChuquanIndex = 0;
	
		if (nUnitSize > 0 || nChuquanSize > 0)
		{
			int nBottom = 0;
			CTechKLineImp* pImpTop = m_pMgr->GetTopLine();
			if (pImpTop != NULL)
			{
				nBottom = pImpTop->GetRect().bottom;
			}
			for (int i = 0; i < nUnitSize; i ++)
			{
				//画出信息地雷
				CDrawInfoUnitData dataCur = m_ayUnitData.GetAt(i);
				for (int j = nInfoIndex; j < nInfoSize; j ++)
				{
					InfoIndexEx* pInfo = m_ayInfoIndex.GetAt(j);
					if(nType == PKLINE_TYPE_K)
					{
						if (pInfo != NULL && pInfo->m_lDate == dataCur.m_lDateTime)
						{
							nInfoIndex = j + 1;
							CPoint pt = dataCur.m_ptPosition;
							pt.x -= 8;
							g_hsGraphicsData.m_expressImages->Draw(pDC, 2, pt, ILD_TRANSPARENT);
							pInfo->m_bIsShow = TRUE;
							pInfo->SetRect(CRect(pt.x, pt.y, pt.x + 16, pt.y + 16));
							break;
						}
						else	//
						{
							pInfo->m_bIsShow = FALSE;
							pInfo->SetRect();
						}
					}
					else
					{
						if (pInfo != NULL && pInfo->m_lTime / 100 == dataCur.m_lDateTime && pInfo->m_lDate == lDataLast)
						{
							nInfoIndex = j + 1;
							CPoint pt = dataCur.m_ptPosition;
							pt.x -= 8;
							g_hsGraphicsData.m_expressImages->Draw(pDC, 2, pt, ILD_TRANSPARENT);
							pInfo->m_bIsShow = TRUE;
							pInfo->SetRect(CRect(pt.x, pt.y, pt.x + 16, pt.y + 16));
							break;
						}
						else	//
						{
							pInfo->m_bIsShow = FALSE;
							pInfo->SetRect();
						}
					}
				}
				//画除权标记
				if(nType == PKLINE_TYPE_K)
				{
					for (int nIndex = nChuquanIndex; nIndex < nChuquanSize; nIndex ++)
					{
						ChuQuanData* pData = pChuquanData->GetAt(nIndex);
						if (pData != NULL && pData->m_lDate == dataCur.m_lDateTime)
						{
							nChuquanIndex = nInfoIndex + 1;
							CPoint pt = dataCur.m_ptChuquan;
							pt.x -= 8;
							pt.y = nBottom - 12;
							g_hsGraphicsData.m_expressImages->Draw(pDC, 3, pt, ILD_TRANSPARENT);
							
/*							InfoIndexEx* pChuquanInfo = new InfoIndexEx;
							pChuquanInfo->m_bIsShow = TRUE;
							pChuquanInfo->SetRect(CRect(pt.x, pt.y - 4, pt.x + 16, pt.y + 12));
							m_ayInfoChuquan.Add(pChuquanInfo);*/
						}
					}					
				}
			}
			if (m_nSelInfoIndex >= 0 && m_nSelInfoIndex < nInfoSize )	//画选中的对象
			{				
				CFont* pOldFont = pDC->SelectObject(g_hsGraphicsData.ftDrawInText.m_pFont);
				COLORREF clrOld = pDC->SetTextColor(g_hsGraphicsData.clDrawBoxValue.m_lColor);
				COLORREF clrBkOld = pDC->GetBkColor();
				CPen pen;
				pen.CreatePen(g_hsGraphicsData.clInfoLine.m_nPenStyle,
					g_hsGraphicsData.clInfoLine.m_nWidth,g_hsGraphicsData.clInfoLine.m_lColor);
				CPen* pOldPen = pDC->SelectObject(&pen);	

				DrawInfo(pDC);

				pDC->SelectObject(pOldPen);
				pDC->SetBkColor(clrBkOld);
				pDC->SetTextColor(clrOld);
				pDC->SelectObject(pOldFont);
				if (pen.m_hObject)
				{
					pen.DeleteObject();
				}
			}
		}
		if(nUnitSize <= 0 && nChuquanSize > 0)		
		{
			for (int j = 0; j < nInfoSize; j ++)
			{
				InfoIndexEx* pInfo = m_ayInfoIndex.GetAt(j);
				if(pInfo != NULL)
				{
					pInfo->m_bIsShow = FALSE;
					pInfo->SetRect();
				}
			}
		}
	}
	return 0;
}

BOOL CDrawInfo::MouseDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if (m_pMgr == NULL)
	{
		return FALSE;
	}
	BOOL bRet = FALSE;
	if(!m_rcInvalidateOld.IsRectEmpty() && m_rcInvalidateOld.PtInRect(point))
	{
		bRet = TRUE;

		int nSelectItem = 0;
		int nSelectInfo = m_ayInfoIndexSelect.GetSize();
		if (nSelectInfo > 0 && m_nItemHeight > 0)
		{
			CRect rcItem = m_rcInvalidateOld;
			rcItem.bottom = rcItem.top + m_nItemHeight;
			for (int i = 0; i < nSelectInfo; i ++)
			{
				InfoIndexEx* pInfo = m_ayInfoIndex.GetAt(i);
				if (pInfo != NULL )
				{
					if(rcItem.PtInRect(point))
					{
						nSelectItem = i;
						break;
					}
				}
				rcItem.top = rcItem.bottom;
				rcItem.bottom = rcItem.top + m_nItemHeight;
			}
		}

		if(nSelectItem >= 0 && nSelectItem < m_ayInfoIndex.GetSize())
		{
			RequestText(nSelectItem);  
		}
	}

	return bRet;
}

BOOL CDrawInfo::MouseMove(UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	if (m_pMgr == NULL)
	{
		return FALSE;
	}
	BOOL bFind = FALSE;

	if(!m_rcInvalidateOld.IsRectEmpty() && m_rcInvalidateOld.PtInRect(point)) //移动在信息地雷的提示框里
	{
		bFind = TRUE;
		int nSelectInfo = m_ayInfoIndexSelect.GetSize();
		if (nSelectInfo > 0 && m_nItemHeight > 0)
		{
			BOOL bResetSelect = FALSE;
			CRect rcItem = m_rcInvalidateOld;
			rcItem.bottom = rcItem.top + m_nItemHeight;
			for (int i = 0; i < nSelectInfo; i ++)
			{
				InfoIndexEx* pInfo = m_ayInfoIndexSelect.GetAt(i);
				if (pInfo != NULL )
				{
					if(rcItem.PtInRect(point) && !pInfo->m_bSelected)
					{
						bResetSelect = TRUE;
						pInfo->m_bSelected = TRUE;
					}
					else if(pInfo->m_bSelected && !rcItem.PtInRect(point))
					{
						bResetSelect = TRUE;
						pInfo->m_bSelected = FALSE;
					}
				}
				rcItem.top = rcItem.bottom;
				rcItem.bottom = rcItem.top + m_nItemHeight;
			}
			if(bResetSelect)
			{
				m_pMgr->DiagramInValidate(m_rcInvalidateOld);
			}
		}
	}
	else
	{
		int nOldSelIndex = m_nSelInfoIndex;
		int nInfoSize = m_ayInfoIndex.GetSize();
		if(nInfoSize > 0)		//先查看是否落在信息地雷上
		{
			for (int i = 0; i < nInfoSize; i ++)
			{
				InfoIndexEx* pInfo = m_ayInfoIndex.GetAt(i);
				if (pInfo != NULL && pInfo->m_bIsShow)
				{
					if (!pInfo->m_rect.IsRectEmpty() && pInfo->m_rect.PtInRect(point))
					{
						m_nSelInfoIndex = i;
						bFind = TRUE;
						break;
					}
				}
			}
		}
		if (!bFind)		//没有落在信息地雷上，再看是否落在除权标记上
		{
		}
		BOOL bRefresh = FALSE;
		if (bFind)
		{
			if (nOldSelIndex != m_nSelInfoIndex)
			{
				m_ayInfoIndexSelect.RemoveAll();
				bRefresh = TRUE;
				m_nTipTimeID = SetTimer(m_pMgr->GetParentHWnd(), (UINT)this, IDT_INFOSHOW_DELAY, &CDrawInfo::ShowTipProc);
			}
		}
		else
		{
			bRefresh = TRUE;
			m_ayInfoIndexSelect.RemoveAll();
			m_nSelInfoIndex = -1;
			KillTipTimer();
		}

		if (bRefresh)
		{
			if (!m_rcInvalidateOld.IsRectEmpty())
			{
				m_pMgr->DiagramInValidate(m_rcInvalidateOld);
				m_rcInvalidateOld = CRect(0, 0, 0, 0);
			}
		}
	}
	return bFind;
}

void CDrawInfo::RequestText(int nSelIndex)
{
	int nSelectedCount = m_ayInfoIndexSelect.GetSize();
	if (nSelIndex < 0 || nSelIndex >= nSelectedCount || m_pRegion == NULL)
	{
		return;
	}

	InfoIndex* pInfoIndex = m_ayInfoIndexSelect.GetAt(nSelIndex);

	AskData* pakData = NULL;
	short  lAskLen = 0;
	int    nCodeCount = sizeof(ReqTextData) / sizeof(CodeInfo) + 
		((sizeof(ReqTextData) % sizeof(CodeInfo))?1:0);

	lAskLen = sizeof(AskData) + (nCodeCount - 1)* sizeof(CodeInfo);
	pakData = (AskData*)new char[lAskLen];
	memset(pakData, 0, lAskLen);

	pakData->m_nSize  = nCodeCount;
	pakData->m_nType  = RT_BYINDEXRETDATA;
	memcpy(&pakData->m_nPrivateKey.m_pCode, &m_stkInfo.m_ciStockCode,sizeof(CodeInfo));

	ReqTextData* pDataReq = (ReqTextData*)pakData->m_pCode;
	// 读取infoPath.ini文件
	CString strFileName,strLocalPath,strLocalFileName; // 文件名
	CAnalysisTextData::GetZXFileReqName(&m_stkInfo.m_ciStockCode, pInfoIndex, m_strInfoPath, &strFileName, &strLocalPath, m_pInfo->GetInfoProvider()); 
	// 从文件读取的数据拷贝到请求包中
	strncpy(pDataReq->m_sMarkData.m_cTitle, pInfoIndex->m_cTitle, sizeof(pDataReq->m_sMarkData.m_cTitle));
	strncpy(pDataReq->m_sMarkData.m_cFilePath, strFileName, sizeof(pDataReq->m_sMarkData.m_cFilePath));

	CString strCFg = CAnalysisTextData::GetUnionString(m_strInfoPath, "", "", INFO_PATH_INI_FILE);
	strncpy(pDataReq->m_sMarkData.m_szInfoCfg, strCFg, sizeof(pDataReq->m_sMarkData.m_szInfoCfg));

	pDataReq->m_sMarkData.m_lBeginPos = pInfoIndex->m_lOffset;
	pDataReq->m_sMarkData.m_lEndPos   = pInfoIndex->m_lLength + pInfoIndex->m_lOffset;

	IRequestCollector* pReqCollector = m_pRegion->GetRequestCollector();
	if (pReqCollector)
	{
		pReqCollector->AddRequest((char*)pakData,lAskLen);
		m_pRegion->SendRequest(pReqCollector);
		pReqCollector->Release();
	}

	delete[] pakData;
}

void CDrawInfo::KillTipTimer()
{
	if (m_nTipTimeID != 0 && m_pMgr != NULL)
	{
		KillTimer(m_pMgr->GetParentHWnd(), m_nTipTimeID);
	}
}


int	CDrawInfo::ResetSelectInfoIndex()
{
	int nInfoSize = m_ayInfoIndex.GetSize();
	if (m_nSelInfoIndex >= 0 && m_nSelInfoIndex < nInfoSize )
	{
		InfoIndexEx* pInfo = m_ayInfoIndex.GetAt(m_nSelInfoIndex);
		if (pInfo != NULL)
		{
			m_ayInfoIndexSelect.Add(pInfo);
			long lDate = pInfo->m_lDate;
			for (int i = m_nSelInfoIndex + 1; i < nInfoSize; i ++)
			{
				InfoIndexEx* pInfoAdd = m_ayInfoIndex.GetAt(i);
				if (lDate != pInfoAdd->m_lDate)
				{
					break;
				}
				else
				{
					m_ayInfoIndexSelect.Add(pInfoAdd);
				}
			}
		}
	}
	return m_ayInfoIndexSelect.GetSize();
}

VOID CALLBACK CDrawInfo::ShowTipProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	KillTimer(hwnd, idEvent);
	CDrawInfo* pDrawInfo = (CDrawInfo*)idEvent;
	if (pDrawInfo != NULL)
	{
		pDrawInfo->ResetSelectInfoIndex();
		CRect rc = pDrawInfo->GetRect();
		::InvalidateRect(hwnd, &rc, FALSE);
	}
}
