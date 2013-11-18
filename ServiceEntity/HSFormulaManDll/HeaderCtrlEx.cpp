// HeaderCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "HeaderCtrlEx.h"
#include "DlgUniteAnalysis.h"

#include "Express.h"
#include "hsothercoldefine.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx

extern CMapStringToPtr* g_pTableHeadData;

CHeaderCtrlEx::CHeaderCtrlEx(int nKey)
{
	m_nKey = nKey;
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlEx)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlEx message handlers

void CHeaderCtrlEx::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CHeaderCtrl::OnRButtonDown(nFlags, point);

	if( m_nKey == 0 )
	{
		CMenu *pMenu;
		pMenu = new CMenu;
		pMenu->LoadMenu(IDR_CHAGEPROJECT);

		CMenu *pSubMenu;
		pSubMenu = pMenu->GetSubMenu(0);

		ClientToScreen(&point);
		CRect	rect;
		int		iEnd = this->GetItemCount();
		int i;
		for( i = 0; i < iEnd; i++)
		{
			GetItemRect(i, &rect);
			ClientToScreen(&rect);
			if(rect.PtInRect(point))
				break;
		}

		if( i < 2)
		{
			delete pMenu;
			return;
		}

		if( i > iEnd )
		{
			pSubMenu->EnableMenuItem(2, 0);
		}

		::SendMessage(((this->GetParent())->GetParent())->m_hWnd, HX_USER_COMPILEDATA, 10, (LPARAM)i);
		pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, point.x, point.y, (this->GetParent())->GetParent(), NULL);
		delete pMenu;
	}
	else if( m_nKey == 1 )
	{
		CMenu pMain;
		CMenu pSub1,pSub2,pSub3,pSub4;

		CString strRet;
		if( g_pTableHeadData )
		{
			void* pValue;
			CMapWordToPtr* pMap;
			CString strName = LOCK_NAME;
			if( g_pTableHeadData->Lookup(strName,pValue) )
			{
				pMap = (CMapWordToPtr*)pValue;
				if( !pMap->IsEmpty() )
				{
					pMain.CreatePopupMenu();

					WORD dKey;
					TableColProperty *pVal;

					dKey = (WORD)DELETE_ID;

					if( pMap->Lookup(dKey,pValue) )
					{
						pVal = (TableColProperty*)pValue;
						if( !pMain.m_hMenu )
						{
							pMain.CreateMenu();
						}
						pMain.AppendMenu(MF_STRING, dKey,pVal->strName);
					}
					for(int i = COLUMN_LINKAGE_BEGIN; i <= EVERY_CB_ID; i++)
					{
						dKey = (WORD)i;
						if( pMap->Lookup(dKey,pValue) )
						{
							pVal = (TableColProperty*)pValue;
							if( dKey >= AU_CODE_ID && dKey <= AU_LTPSZ_ID )
							{
								if( !pSub1.m_hMenu )
								{
									pSub1.CreatePopupMenu();
									pMain.AppendMenu(MF_POPUP, (UINT_PTR)pSub1.m_hMenu,_T("A股"));
								}
								pSub1.AppendMenu(MF_STRING, dKey,pVal->strName);
							}
							else if( dKey >= BU_CODE_ID && dKey <= BU_LTPSZ_ID )
							{
								if( !pSub2.m_hMenu )
								{
									pSub2.CreatePopupMenu();
									pMain.AppendMenu(MF_POPUP, (UINT_PTR)pSub2.m_hMenu,_T("B股"));
								}
								pSub2.AppendMenu(MF_STRING, dKey,pVal->strName);
							}
							else if( dKey >= HU_CODE_ID && dKey <= HU_LTPSZ_ID )
							{
								if( !pSub3.m_hMenu )
								{
									pSub3.CreatePopupMenu();
									pMain.AppendMenu(MF_POPUP, (UINT_PTR)pSub3.m_hMenu,_T("H股"));
								}
								pSub3.AppendMenu(MF_STRING, dKey,pVal->strName);
							}
							else if( dKey >= ABU_PRICE_ID && dKey <= EVERY_CB_ID )
							{	
								if( !pSub4.m_hMenu )
								{
									pSub4.CreatePopupMenu();
									pMain.AppendMenu(MF_POPUP, (UINT_PTR)pSub4.m_hMenu,_T("其他"));
								}
								pSub4.AppendMenu(MF_STRING, dKey,pVal->strName);

							}
						}
					}
				}
			}
		}

		if( pMain.m_hMenu )
		{
			ClientToScreen(&point);
			CRect	rect;
			int		iEnd = this->GetItemCount();
			int i;
			for( i = 0; i < iEnd; i++)
			{
				GetItemRect(i, &rect);
				ClientToScreen(&rect);
				if(rect.PtInRect(point))
					break;
			}
			::SendMessage(((this->GetParent())->GetParent())->m_hWnd, HX_USER_COMPILEDATA, 10, (LPARAM)i);
			pMain.TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, point.x, point.y, (this->GetParent())->GetParent(), NULL);
		}

		/*
		CMenu *pMenu;
		pMenu = new CMenu;
		pMenu->LoadMenu(IDR_UNITE_AYALYSE);

		CMenu *pSubMenu;
		pSubMenu = pMenu->GetSubMenu(0);

		ClientToScreen(&point);
		CRect	rect;
		int		iEnd = this->GetItemCount();

		for(int i = 0; i < iEnd; i++)
		{
			GetItemRect(i, &rect);
			ClientToScreen(&rect);
			if(rect.PtInRect(point))
				break;
		}
		if(i >= iEnd || i < 2)
			return;
		::SendMessage(((this->GetParent())->GetParent())->m_hWnd, HX_USER_COMPILEDATA, 10, (LPARAM)i);
		pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_LEFTALIGN, point.x, point.y, (this->GetParent())->GetParent(), NULL);
		delete pMenu;
		*/
	}
}

void CHeaderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CHeaderCtrlEx::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnRButtonUp(nFlags, point);
}

void CHeaderCtrlEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CHeaderCtrlEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CHeaderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
