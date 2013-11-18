#include "StdAfx.h"
#include "DiagramHolder.h"
#include <Shlwapi.h>
#include <stack>
#include "DiagramLayer.h"
#include "DiagramFactory.h"
#include "DiagramView.h"
#include "WinnerApplication.h"
#include "ConfigInterface.h"
#include "..\..\HelperLib\GeneralHelper.h"
#include "..\..\..\Resource\QuoteLang\resource.h"
#include "HSBizDialog.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*CRegionNode实现***************************************************************************************/
CRegionNode::CRegionNode()
{
	m_pLayer = NULL;
	m_cLocation = RNP_UNKNOW;
	m_nMeasure = 0;
	m_pLeft = NULL;
	m_pRight = NULL;
	m_pHostWnd = NULL;
	m_nActiveDiagramIndex = 0;
	m_nInitActiveDiagramIndex = 0;
	m_bFix = FALSE;
	m_tabBar.SetNotify(this);

	
	IHsFont*  pHSFont = (IHsFont*)CWinnerApplication::GetObject(OBJ_FONTCFG);
	if(pHSFont)
	{
		m_tabBar.m_pItemFont = pHSFont->GetDiagramLayoutTabFont();
	}	
	//
	IHsColor* pHSColor = (IHsColor*)CWinnerApplication::GetObject(OBJ_COLORCFG);
	if(pHSColor)
	{
		m_tabBar.m_clrBackground = pHSColor->GetDiagramLayoutBKColor();
 		m_tabBar.m_clrActiveFill= pHSColor->GetDiagramLayoutActiveFillColor();
 		m_tabBar.m_clrFill = pHSColor->GetDiagramLayoutFillColor();
 		m_tabBar.m_clrFore = pHSColor->GetDiagramLayoutForeColor();
 		m_tabBar.m_clrBorder = pHSColor->GetDiagramLayoutBorderColor();
	}

}

CRegionNode::~CRegionNode()
{
	for(int i = 0; i < m_arrDiagramInfo.GetCount(); i++)
	{
		CDiagramInfo *pInfo = m_arrDiagramInfo.GetAt(i);
		if(pInfo)
		{
			if(pInfo->m_pDraw)
				pInfo->m_pDraw->Delete();
			delete pInfo;
		}
	}
}

int CRegionNode::GetSplitStyle()
{
	if( this->m_pLeft == NULL )
		return SS_NOSPLIT;

	char cLocation = m_pLeft->m_cLocation;
	if('l' == cLocation || 'L' == cLocation || 'r' == cLocation || 'R' == cLocation)
		return SS_VERTICAL;
	else if ('t' == cLocation || 'T' == cLocation || 'b' == cLocation || 'B' == cLocation)
		return SS_HORIZONTAL;
	else
		return SS_NOSPLIT;
}

void CRegionNode::SetRegionRect(const CRect &rc)
{
	m_regionRect = rc;
	CRect rcDraw(m_regionRect);
	if(HasTabBar())
	{
		DWORD dwPos = m_tabBar.GetPostion();
		switch( dwPos )
		{
		case TS_LEFT:
			{
				CRect rcTabBar(m_regionRect.left,m_regionRect.top,
					m_regionRect.left + CDiagramTab::m_nDimensions,m_regionRect.bottom);
				m_tabBar.SetRect(rcTabBar);
				break;
			}
		case TS_RIGHT:
			{
				CRect rcTabBar(m_regionRect.right - CDiagramTab::m_nDimensions,m_regionRect.top,
					m_regionRect.right,m_regionRect.bottom);
				m_tabBar.SetRect(rcTabBar);
				break;
			}
		case TS_TOP:
			{
				CRect rcTabBar(m_regionRect.left,m_regionRect.top,
					m_regionRect.right,m_regionRect.top + CDiagramTab::m_nDimensions);
				m_tabBar.SetRect(rcTabBar);
				break;
			}
		case TS_BOTTOM:
			{
				CRect rcTabBar(m_regionRect.left,m_regionRect.bottom - CDiagramTab::m_nDimensions ,
					m_regionRect.right,m_regionRect.bottom);
				m_tabBar.SetRect(rcTabBar);
				break;
			}
		}
		m_tabBar.ReCalTabItemSize();	
		rcDraw.SubtractRect(m_regionRect,m_tabBar.GetRect());
	}
	//设置图元大小
	for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
	{
		IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
		if(pDraw)
		{
			pDraw->SetRect(rcDraw);
		}
	}	
}

BOOL CRegionNode::HasChild()
{
	return m_pLeft != NULL && m_pRight != NULL;
}

BOOL CRegionNode::HasTabBar()
{
	DWORD dwStyle = m_tabBar.GetStyle();
	dwStyle = dwStyle & 0x0000000F;
	if (dwStyle == TS_TOP || dwStyle == TS_BOTTOM || dwStyle == TS_LEFT || dwStyle == TS_RIGHT)
		return TRUE;
	else
		return FALSE;
}

void CRegionNode::LoadNodeInfo(TiXmlElement* pNodeXml)
{
	if(pNodeXml == NULL)
		return;

	const char* pLocation = pNodeXml->Attribute("location");
	if(pLocation)
		this->m_cLocation = pLocation[0];

	const char* pSize = pNodeXml->Attribute("size");
	if(pSize)
		this->m_nMeasure = (atoi(pSize));

	const char* pFix = pNodeXml->Attribute("fix");
	if (pFix)
		this->m_bFix = (atoi(pFix));

    CString  strTabBarPos = pNodeXml->Attribute("tab_bar_pos");
	DWORD dwStyle = 0;
	if(!strTabBarPos.IsEmpty())
	{
		if(strTabBarPos == "T")
			dwStyle |= TS_TOP;
		else if(strTabBarPos == "B")
			dwStyle |= TS_BOTTOM;
		else if(strTabBarPos == "L")
			dwStyle |= TS_LEFT;
		else if(strTabBarPos == "R")
			dwStyle |= TS_RIGHT;
		if( dwStyle != 0)
		{
			CString strTabBarStyle = pNodeXml->Attribute("tab_bar_style");
			if(strTabBarStyle == "adaptive")
				dwStyle |= TS_TABSIZEADAPTIVE;
			else
				dwStyle |= TS_TABSIZEEQUAL;
		}
		this->m_tabBar.SetStyle(dwStyle);

		CString  strActiveIndex = pNodeXml->Attribute("active_index");
		int nActiveIndex = 0;
		if( !strActiveIndex.IsEmpty())
			nActiveIndex = atoi(strActiveIndex);
		m_nInitActiveDiagramIndex = m_nActiveDiagramIndex = nActiveIndex;
		m_tabBar.SetActiveItem(nActiveIndex);
	}

	//图元列表
	TiXmlNode* pDiagramXmlNode = pNodeXml->FirstChild(_T("Diagram"));
	while(pDiagramXmlNode)
	{
		CDiagramInfo  *pDiagramInfo= new CDiagramInfo();
		TiXmlElement* pDiagramXmlElem = pDiagramXmlNode->ToElement();
		pDiagramInfo->m_strCaption = pDiagramXmlElem->Attribute(_T("caption"));
		pDiagramInfo->m_strLocation = pDiagramXmlElem->Attribute(_T("location"));
		pDiagramInfo->m_pDraw = GetDiagramfactory()->CreateDiagram(pDiagramInfo->m_strLocation);
	
		//参数
		TiXmlElement* pParamElem = pDiagramXmlElem->FirstChildElement(_T("Param"));
		while(pParamElem != NULL)
		{
			CString strParamName = pParamElem->Attribute(_T("name"));
			CString strParamValue = pParamElem->Attribute(_T("value"));
			pDiagramInfo->m_param.AddParam(strParamName,strParamValue);

			pParamElem = pParamElem->NextSiblingElement(_T("Param"));
		}

		if(pDiagramInfo->m_pDraw)
		{
			pDiagramInfo->m_pDraw->SetRegionNode(this);
			pDiagramInfo->m_pDraw->Create(&pDiagramInfo->m_param);
			pDiagramInfo->m_pDraw->SetName(pDiagramInfo->m_strCaption + _T(":") + pDiagramInfo->m_strLocation);
		}
		this->m_arrDiagramInfo.Add(pDiagramInfo);
		if( dwStyle != 0)
			m_tabBar.AddTabItem(pDiagramInfo->m_strCaption,m_arrDiagramInfo.GetCount() - 1);
		pDiagramXmlNode = pDiagramXmlNode->NextSibling(_T("Diagram"));
	}	
}


HWND CRegionNode::GetHostHandle()
{
	if(m_pHostWnd)
		return m_pHostWnd->GetSafeHwnd();
	else
		return NULL;
}

BOOL CRegionNode::ActiveDiagram(CString strDiagramName)
{
	return TRUE;	
}


BOOL CRegionNode::Invalidate(CRect* pRC /*= NULL*/,BOOL bErase/* = TRUE*/)
{
	if(!m_pLayer->IsVisable())
		return FALSE;

	if(m_pHostWnd != NULL && m_pHostWnd->GetSafeHwnd() != NULL)
	{
		//CClientDC dc(m_pHostWnd);
		//this->OnDraw(&dc);
		if(pRC == NULL)
			m_pHostWnd->InvalidateRect(m_regionRect,bErase);
		else
			m_pHostWnd->InvalidateRect(pRC,bErase);
		return TRUE;
	}
	return FALSE;
}

BOOL CRegionNode::SetCursor(LPCSTR lpCursorName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	if(lpCursorName)
	{
		if(lpCursorName == IDC_HAND)
		{
			static HINSTANCE h = CWinnerApplication::GetModuleHandle("QuoteLang");
			::SetCursor(LoadCursor(h,MAKEINTRESOURCE(IDC_QUOTE_HAND)));
		}
		else
		{
			::SetCursor(LoadCursor(NULL,lpCursorName));
		}	
		return TRUE;
	}
	else
		return FALSE;
}

void CRegionNode::OnActiveTabChange(CDiagramTabItem* pActiveItem,CDiagramTabItem* pOldActiveItem)
{
	if( pActiveItem == NULL )
		return;
	m_nActiveDiagramIndex = pActiveItem->GetIndex();
	for(int i = 0; i<m_arrDiagramInfo.GetCount(); i++ )
	{
		IDrawBase* pDraw = m_arrDiagramInfo.GetAt(i)->m_pDraw;
		if(pDraw && m_nActiveDiagramIndex == i)
		{
			CRequestCollector reqCollector;
			pDraw->UpdateCodeInfo(pDraw->GetCodeInfo());
			pDraw->GetRequestData(&reqCollector,pDraw->GetCodeInfo());
			reqCollector.DoRequest(m_pLayer->GetDataSource(),m_pLayer->GetDataSourceID());

			CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,
				_T("[%s|%s-%d] request data for '%s' has done."),__FUNCTION__,__FILE__,__LINE__,pDraw->GetName());

		}
		if( pDraw )
			pDraw->OnActiveChange(m_nActiveDiagramIndex == i);
	}
	
	Invalidate(FALSE);
}

IRequestCollector* CRegionNode::GetRequestCollector()
{
	return new CRequestCollector();
}

BOOL CRegionNode::SendRequest(IRequestCollector* pReqCollector, BOOL bAutoClear)
{
	if(pReqCollector)
	{
		CRequestCollector* pSender = (CRequestCollector*)pReqCollector;
		pSender->DoRequest(m_pLayer->GetDataSource(),m_pLayer->GetDataSourceID(), bAutoClear);
		return TRUE;
	}
	return FALSE;
}

void CRegionNode::SendNotifyMsg(UINT nMsgId,IDrawBaseParam* pParam)
{
	if(m_pLayer)
		m_pLayer->SendNotifyToDrawBase(nMsgId,pParam);
}

void CRegionNode::SendLinkageMsg(UINT nMsg,IDrawBaseParam* pParam,
								 CString strPanelName /*= _T("")*/,CString strLayout /*= _T("")*/,CString strOwner /*= PUBLIC_OWNER*/)
{
	CHSBizDialog* pBizDlg = m_pLayer->GetBizDialog();
	if(pBizDlg == NULL)
		return;

	if(!strPanelName.IsEmpty())
	{
		IMainWnd* pMain = pBizDlg->GetMain();
		pMain->ShowPanel(strPanelName,SP_SHOW,strOwner);
	}

	CLinkageParam param;
	POSITION pos;
	CString strParamName,strValue;
	for( pos = pParam->GetStartPosition(); pos != NULL; )
	{
		pParam->GetNextAssoc( pos, strParamName, strValue);
		param.AddParam(strParamName,strValue);
	}

	pBizDlg->SendLinkageMsg(nMsg,&param);
}

void CRegionNode::SetVisable(BOOL bVisable)
{
	if( !HasChild())
	{
		for( int i=0; i < m_arrDiagramInfo.GetCount(); i++)
		{
			IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
			if(pDraw)
				pDraw->SetVisible(bVisable);
		}	
	}
}

void CRegionNode::ResetActiveDiagram()
{
	if(m_nActiveDiagramIndex != m_nInitActiveDiagramIndex)
	{
		m_nActiveDiagramIndex = m_nInitActiveDiagramIndex;
		m_tabBar.SetActiveItem(m_nInitActiveDiagramIndex);
	}
}

BOOL CRegionNode::IsActive()
{
	if(m_pLayer)
	{
		if(m_pLayer->GetActiveNode() == this)
			return TRUE;
	}
	return FALSE;
}

void CRegionNode::GetRequestData(IRequestCollector* pReqtCollecter,StockUserInfo* pStockInfo, BOOL bForce)
{
	if( !HasChild())
	{
		for( int i=0; i < m_arrDiagramInfo.GetCount(); i++)
		{
			IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
			if(pDraw)
				pDraw->UpdateCodeInfo(pStockInfo);
			if(pDraw && m_nActiveDiagramIndex == i)
			{
				pDraw->GetRequestData(pReqtCollecter,pStockInfo,bForce);
				CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,
					_T("[%s|%s-%d] request data for '%s' has done."),__FUNCTION__,__FILE__,__LINE__,pDraw->GetName());
			}
		}	
	}
}

int	CRegionNode::OnUpdateData(DataHead* pData)
{
	if( !HasChild())
	{
		for( int i=0; i < m_arrDiagramInfo.GetCount(); i++)
		{
			IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
			if(pDraw)
			{
				pDraw->OnUpdateData(pData);
			}
		}	
	}
	return 0;
}

BOOL CRegionNode::HitTest(CPoint pt,UINT* pFlags)
{
	if( HasChild() )
	{
		if (m_bFix)
		{// 过滤掉固定孩子
			if (pFlags)
				*pFlags = RNHT_NOWHERE;
			return FALSE;
		}
		CRect rc = m_spliterRect;
		rc.InflateRect(2,2);
		if ( rc.PtInRect(pt) )
		{
			UINT nFlags = RNHT_NOWHERE;
			if( m_pLeft == NULL )
				return FALSE;
			if(m_pLeft->m_cLocation == RNP_LEFT)
				nFlags = RNHT_IN_VER_SPLITER;
			else if (m_pLeft->m_cLocation == RNP_TOP)
				nFlags = RNHT_IN_HOR_SPLITER;

			if(pFlags)
				*pFlags = nFlags;

			return TRUE;
		}
	}
	else
	{
		if(m_tabBar.GetRect().PtInRect(pt))
		{
			if(pFlags)
				*pFlags = RNHT_INTABBAR;
			return TRUE;
		}
		if(m_regionRect.PtInRect(pt))
		{
			if(pFlags)
				*pFlags = RNHT_INRECTUNIT;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRegionNode::OnCursor(UINT nFlags, CPoint point)
{
	if( HasChild() )
	{
		UINT nHitFlags = 0;
		if(HitTest(point,&nHitFlags))
		{	
			if(RNHT_IN_VER_SPLITER == nHitFlags) //垂直分割
				::SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			else if(RNHT_IN_HOR_SPLITER == nHitFlags) //水平分割
				::SetCursor(LoadCursor(NULL,IDC_SIZENS));
			else
				::SetCursor(LoadCursor(NULL,IDC_ARROW));
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

void CRegionNode::OnDraw(CDC* pDC)
{
	if(HasChild())
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(192,0,0));
		pDC->FillRect(m_spliterRect,&brush);
	}
	else
	{		
		//绘制当前激活图元
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					pDraw->Draw(pDC);
				}
			}
		}	
		if(HasTabBar())
		{
			m_tabBar.Draw(pDC);
		}
	}
}

BOOL CRegionNode::OnMouseDown(UINT nFlags, CPoint point,CWnd *pHostWnd)
{
	if( HasChild() )
	{
		if(pHostWnd == NULL)
			return FALSE;
		::SetCapture(pHostWnd->m_hWnd);
		this->OnCursor(nFlags,point);
		int nSplitStyle = GetSplitStyle();
		int nYDiff = 0;
		int nXDiff = 0;
		int nPreviousXDiff = 0;
		int nPreviousYDiff = 0;
		CPoint curpoint(point.x,point.y);

		CDC pDrawDC;
		HDC hdc = ::GetDC(pHostWnd->m_hWnd);
		pDrawDC.Attach(hdc);

		CPen pen(PS_SOLID, 1, RGB(255,0,0));
		CPen* pOldPen = pDrawDC.SelectObject(&pen);
		while(TRUE)
		{	
			MSG msg;
			VERIFY(::GetMessage(&msg, NULL, 0, 0));

			if (CWnd::GetCapture() != pHostWnd )
				break;

			BOOL bEixstLoop = FALSE;
			switch (msg.message) 
			{
			case WM_MOUSEMOVE:
				{
					curpoint.SetPoint(msg.pt.x,msg.pt.y);
					pHostWnd->ScreenToClient(&curpoint);

					int nOldRop = pDrawDC.SetROP2(R2_NOT);
					if(nSplitStyle == SS_HORIZONTAL)
					{
						int nYDiff = curpoint.y - point.y;
						pDrawDC.MoveTo(m_spliterRect.left,m_spliterRect.bottom + nYDiff);
						pDrawDC.LineTo(m_spliterRect.right,m_spliterRect.bottom + nYDiff);
						if(nPreviousYDiff != 0)
						{
							pDrawDC.MoveTo(m_spliterRect.left,m_spliterRect.bottom + nPreviousYDiff);
							pDrawDC.LineTo(m_spliterRect.right,m_spliterRect.bottom + nPreviousYDiff);
						}
						nPreviousYDiff = nYDiff;
					}
					else
					{
						int nXDiff = curpoint.x - point.x;
						pDrawDC.MoveTo(m_spliterRect.left + nXDiff,m_spliterRect.top);
						pDrawDC.LineTo(m_spliterRect.left + nXDiff,m_spliterRect.bottom);
						if(nPreviousXDiff != 0)
						{
							pDrawDC.MoveTo(m_spliterRect.left + nPreviousXDiff,m_spliterRect.top);
							pDrawDC.LineTo(m_spliterRect.left + nPreviousXDiff,m_spliterRect.bottom);
						}
						nPreviousXDiff = nXDiff;
					}
					pDrawDC.SetROP2(nOldRop);
					break;
				}			
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
				bEixstLoop = TRUE;
				break;
			default:
				DispatchMessage(&msg);
			}

			if(bEixstLoop)
			{
				break;
			}
		}

		//更新分割区位置,更新容器内元素的百分比
 		CRegionNode* pChild1 = m_pLeft;
 		CRegionNode* pChild2 = m_pRight;
 		if(nSplitStyle == SS_HORIZONTAL && nPreviousYDiff != 0)
 		{
 			m_spliterRect.top = m_spliterRect.top + nPreviousYDiff;
 			m_spliterRect.bottom = m_spliterRect.bottom + nPreviousYDiff;
 
 			float fHeight = abs(m_regionRect.top - m_spliterRect.top);
			if(m_pLeft->m_nMeasure != 0 )
				m_pLeft->m_nMeasure = int(fHeight);
			else
				m_pRight->m_nMeasure = m_regionRect.Height() - DIAGRAM_SPLITBAR_WIDTH -  static_cast<int>(fHeight);
 		}
 		else if (nSplitStyle == SS_VERTICAL && nPreviousXDiff != 0)
 		{
 			m_spliterRect.left = m_spliterRect.left + nPreviousXDiff;
 			m_spliterRect.right = m_spliterRect.right + nPreviousXDiff;
 
 			float fWidth = abs(m_regionRect.left - m_spliterRect.left);
			if(m_pLeft->m_nMeasure != 0 )
				m_pLeft->m_nMeasure = int(fWidth);
			else
				m_pRight->m_nMeasure = m_regionRect.Width() - DIAGRAM_SPLITBAR_WIDTH -  static_cast<int>(fWidth);
 		}
		::ReleaseCapture();
		return TRUE;
	}
	else
	{
		UINT nHitFlag = 0;
		if(HitTest(point,&nHitFlag))
		{
			if( nHitFlag == RNHT_INTABBAR )
			{
				m_tabBar.OnLButtonDown(nFlags,point);
			}
			else
			{
				for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
				{
					if( i == m_nActiveDiagramIndex)
					{
						IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
						if(pDraw)
						{
							pDraw->MouseDown(nFlags,point,NULL);
						}
					}
				}	
			}
		}
		return FALSE;
	}
}


BOOL CRegionNode::OnMouseMove(UINT nFlags, CPoint point)
{
	if( !HitTest(point) )
		return FALSE;

	if(HasChild())
	{
		return OnCursor(nFlags,point);
	}
	else
	{
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					pDraw->MouseMove(nFlags,point);
				}
			}
		}	
		return TRUE;
	}
}

BOOL CRegionNode::MouseDBDown(UINT nFlags, CPoint point, hsRefMouseMsg* pData /*= NULL*/)
{
	if( !HitTest(point) )
		return FALSE;

	if( !HasChild())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					pDraw->MouseDBDown(nFlags,point,pData);
				}
			}
		}	
	}
	return TRUE;
}

BOOL CRegionNode::RButtonDown(UINT nFlags,CPoint point, HWND hWndParent, hsRefMouseMsg* pData/* = NULL*/)
{
	if( !HitTest(point) )
		return FALSE;

	if( !HasChild())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					pDraw->RButtonDown(nFlags,point,hWndParent,pData);
				}
			}
		}	
	}
	return TRUE;
}

int	CRegionNode::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( !HasChild() && IsActive())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					return pDraw->OnKey(nChar,nRepCnt,nFlags);
				}
			}
		}	
	}
	return 0;
}

BOOL CRegionNode::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if( !IsActive() )
		return FALSE;

	if( !HasChild())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					return pDraw->OnMouseWheel(nFlags,zDelta,pt);
				}
			}
		}	
	}
	return 0;
}

BOOL CRegionNode::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	if( !HasChild())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
			 		return pDraw->OnKeyBoardNotify(wParam,lParam);
				}	
			}
		}	
	}
	return 0;
}

void CRegionNode::SendNotifyToDrawBase(UINT nMsgId,IDrawBaseParam* pParam)
{
	if( !HasChild())
	{
		for(int i = 0; i < m_arrDiagramInfo.GetSize(); i++)
		{
			if( i == m_nActiveDiagramIndex)
			{
				IDrawBase *pDraw = m_arrDiagramInfo[i]->m_pDraw; 
				if(pDraw)
				{
					return pDraw->OnLayoutNotify(nMsgId,pParam);
				}	
			}
		}	
	}
}

/*CDiagramLayer实现***************************************************************************************/
CDiagramLayer::CDiagramLayer(CString strLayoutName)
{
	m_bVisable = TRUE;
	m_pActiveNode = NULL;
	m_strLayoutName = strLayoutName;
	m_pHostWnd = NULL;
	m_pHSBizDialog = NULL;
	m_rootNode.SetOwnerLayer(this);
}

CDiagramLayer::~CDiagramLayer(void)
{
	ClearLayoutTree();
}

void CDiagramLayer::ClearLayoutTree()
{
	RemoveChildrenNode(&m_rootNode);
}

void CDiagramLayer::RemoveChildrenNode(CRegionNode* pNode)
{
	if(pNode->HasChild())
	{
		RemoveChildrenNode(pNode->m_pLeft);
		RemoveChildrenNode(pNode->m_pRight);

		pNode->m_pLeft = NULL;
		pNode->m_pRight = NULL;
	}

	if(pNode != &m_rootNode)
		delete pNode;
}

BOOL CDiagramLayer::LoadLayout(CWnd* pHostWnd,CString strStandardFile)
{
	m_pHostWnd = pHostWnd;
	if(strStandardFile.IsEmpty())
		return FALSE;

	TiXmlDocument xmldoc;
	TiXmlElement* pRoot = NULL;
	if( !xmldoc.LoadFile(strStandardFile) )
	{
		CString strError;
		strError.Format(_T("分析布局文件(%s)失败.\r\n行:%d 列:%d\r\n错识原因:%s"),
			strStandardFile,xmldoc.ErrorRow(),xmldoc.ErrorCol(),xmldoc.ErrorDesc());
		::MessageBox(NULL,strError,_T("Error"),MB_OK | MB_ICONERROR);
		return FALSE;
	}
	pRoot = xmldoc.FirstChildElement(_T("DiagramLayout"));

	ASSERT(pRoot != NULL);
	TiXmlElement* pFistNode = pRoot->FirstChildElement("Node");
	CString strTemp = pFistNode->Attribute("location");
	if(strTemp.IsEmpty())
	{
		m_rootNode.m_pHostWnd = this->m_pHostWnd;
		m_rootNode.LoadNodeInfo(pFistNode);
		return TRUE;
	}
	else
		return BuildLayoutTree(&m_rootNode,pFistNode);
}

BOOL CDiagramLayer::BuildLayoutTree(CRegionNode* pParentNode,TiXmlElement* pParentNodeXml)
{
	TiXmlElement* pElem = pParentNodeXml;
	while( pElem != NULL)
	{
		CRegionNode* pNode = new CRegionNode();
		pNode->SetOwnerLayer(this);
		pNode->m_pHostWnd = this->m_pHostWnd;
		pNode->LoadNodeInfo(pElem);
		
		CString strFilter1(_T("RrLlTtBb"));
		ASSERT( strFilter1.Find(pNode->m_cLocation) >= 0);

		CString strFilter2(_T("LlTt"));
		if (strFilter2.Find(pNode->m_cLocation) >= 0)
			pParentNode->m_pLeft = pNode;
		else 
			pParentNode->m_pRight = pNode;

		if(!pElem->NoChildren() && pElem->FirstChild("Node") != NULL)
		{
			TiXmlElement* pChildElem = pElem->FirstChild("Node")->ToElement();
			BuildLayoutTree(pNode,pChildElem);
		}
		pElem=pElem->NextSiblingElement();
	}
	return TRUE;
}

void CDiagramLayer::RecalcLayout(int X,int Y,int nWidth, int nHeight)
{
	RecalcLayout(&m_rootNode,X,Y,nWidth,nHeight);
}

void CDiagramLayer::ResetRegionNodeActiveDiagram()
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->ResetActiveDiagram();
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
}

void CDiagramLayer::RecalcLayout(CRegionNode* pNode,int X,int Y,int nWidth, int nHeight)
{
	//更新Node的m_rect
	CRect regionRect(X,Y,X+nWidth,Y+nHeight);
    pNode->SetRegionRect(regionRect);
	//更新子结点
	if(pNode->HasChild())
	{
		char cLocation = pNode->m_pLeft->m_cLocation;
		int nLeftMeasure = 0;
		if( cLocation == RNP_TOP  )
		{
			if(pNode->m_pLeft->m_nMeasure == 0 && pNode->m_pRight->m_nMeasure ==0 )
				pNode->m_pLeft->m_nMeasure = nHeight /2;
			int nLeftMeasure = pNode->m_pLeft->m_nMeasure == 0 ? nHeight - pNode->m_pRight->m_nMeasure : pNode->m_pLeft->m_nMeasure;
			if(nLeftMeasure == nHeight)
				nLeftMeasure = nHeight / 2;
			RecalcLayout(pNode->m_pLeft,X,Y,nWidth,nLeftMeasure);
			pNode->m_spliterRect.SetRect(X,Y + nLeftMeasure,
										 X + nWidth , Y + nLeftMeasure + DIAGRAM_SPLITBAR_WIDTH );
			RecalcLayout(pNode->m_pRight,X,Y + nLeftMeasure + DIAGRAM_SPLITBAR_WIDTH,nWidth,nHeight - nLeftMeasure - DIAGRAM_SPLITBAR_WIDTH);
		}
		else if( cLocation == RNP_LEFT )
		{
			if(pNode->m_pLeft->m_nMeasure == 0 && pNode->m_pRight->m_nMeasure ==0 )
				pNode->m_pLeft->m_nMeasure = nWidth /2;
			int nLeftMeasure = pNode->m_pLeft->m_nMeasure == 0 ? nWidth - pNode->m_pRight->m_nMeasure : pNode->m_pLeft->m_nMeasure;
			if(nLeftMeasure == nHeight)
				nLeftMeasure = nHeight / 2;
			RecalcLayout(pNode->m_pLeft,X,Y,nLeftMeasure,nHeight);

			pNode->m_spliterRect.SetRect(X + nLeftMeasure ,Y,
				                        X + nLeftMeasure + DIAGRAM_SPLITBAR_WIDTH , Y + nHeight);

			RecalcLayout(pNode->m_pRight,X + nLeftMeasure + DIAGRAM_SPLITBAR_WIDTH,Y,
				nWidth - nLeftMeasure - DIAGRAM_SPLITBAR_WIDTH,nHeight);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
}


void CDiagramLayer::SetDataSource(IDataSourceEx* pDataSource,long nSourceID)
{
	this->m_pDataSource = pDataSource;
	this->m_nDataSourceID = nSourceID;
}

void CDiagramLayer::SetVisable(BOOL bVisable)
{
	m_bVisable = bVisable;

	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->SetVisable(bVisable);
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
}

void CDiagramLayer::OnSize(int cx, int cy,CWnd* pWnd)
{
	RecalcLayout(0,0,cx,cy);
}

void CDiagramLayer::OnDraw(CDC* pDC)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->OnDraw(pDC);
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
}

void CDiagramLayer::GetRequestData(IRequestCollector* pReqtCollecter,StockUserInfo* pStockInfo, BOOL bForce)
{
	CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,
		_T("○[[%s|%s-%d]] request data for <%s> begin."),__FUNCTION__,__FILE__,__LINE__,this->m_strLayoutName);

	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->GetRequestData(pReqtCollecter,pStockInfo,bForce);
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}

	CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,
		_T("●[[%s|%s-%d]] request data for <%s> end."),__FUNCTION__,__FILE__,__LINE__,this->m_strLayoutName);
}

int	CDiagramLayer::OnUpdateData(DataHead* pData)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->OnUpdateData(pData);
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
	return 0;
}


void CDiagramLayer::OnMouseAction(ActionType action,UINT nFlags, CPoint point, hsRefMouseMsg* pData/* = NULL*/)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			switch(action)
			{
			case MouseMove:
				{
					UINT flags = 0;
					if( node->HitTest(point,&flags) )
					{
						if(flags == RNHT_INRECTUNIT)
						{
							SetActiveNode(node);
						}
					}

					BOOL bDeal = node->OnMouseMove(nFlags,point);
					if(bDeal)
						return;
					break;
				}
			case MouseDown:
				{
					if(node->HasChild())
					{
						if( node->HitTest(point,NULL) )
						{
							node->OnMouseDown(nFlags,point,m_pHostWnd);
							RecalcLayout(&m_rootNode,m_rootNode.m_regionRect.top,m_rootNode.m_regionRect.left,
								m_rootNode.m_regionRect.Width(),m_rootNode.m_regionRect.Height());
							m_pHostWnd->Invalidate(TRUE);
						}
					}
					else
					{
						node->OnMouseDown(nFlags,point,m_pHostWnd);
					}
					break;
				}
			case MouseUp:
				{
					::SetCursor(LoadCursor(NULL,IDC_ARROW));
					break;
				}
			case MouseDBDown:
				{
					node->MouseDBDown(nFlags,point,NULL);
					break;
				}	
			case RButtonDown:
				{
					node->RButtonDown(nFlags,point,m_pHostWnd->m_hWnd,NULL);
					break;
				}
			}
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
}

int	CDiagramLayer::OnKey(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			int nRet = node->OnKey(nChar,nRepCnt,nFlags);
			if(nRet)
				return nRet;

			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
	return 0;
}

BOOL CDiagramLayer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			if( node->IsActive())
			{
				return node->OnMouseWheel(nFlags,zDelta,pt);
			}

			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
	return FALSE;
}

BOOL CDiagramLayer::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	BOOL bDone = FALSE;
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			if( node->OnKeyBoardNotify(wParam,lParam) )
			{
				bDone = TRUE;
			}
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
	return 0;
}


void CDiagramLayer::SendNotifyToDrawBase(UINT nMsgId,IDrawBaseParam* pParam)
{
	CRegionNode* node = &m_rootNode;
	stack<CRegionNode*> s;

	BOOL bDone = FALSE;
	while ((NULL != node) || !s.empty())
	{
		if (NULL != node)
		{
			node->SendNotifyToDrawBase(nMsgId,pParam);
			s.push(node);
			node = node->m_pLeft;
		}
		else 
		{
			node = s.top();
			s.pop();
			node = node->m_pRight;
		}
	}
}