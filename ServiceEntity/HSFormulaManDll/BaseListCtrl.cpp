// BaseListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "BaseListCtrl.h"

#include "DlgFunctionMan.h"

#include "HeaderCtrlEx.h"
#include <afxwin.h>
#include <afxcmn.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseListCtrl

CBaseListCtrl::CBaseListCtrl()
{
	m_pWndHeader = NULL;
}

CBaseListCtrl::~CBaseListCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CBaseListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CBaseListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(HDN_ITEMCHANGED, OnItemchanged)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseListCtrl message handlers

int CBaseListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
		
	Init();
	COLORREF color = RGB(255,255,255);
	SetBkColor( color );

	return 0;
}

void CBaseListCtrl::Init()
{
	if(m_imaSmall.m_hImageList != NULL)
		return;

	m_imaSmall.Create(IDB_FUNIMAGELIST, 16, 0, RGB(0,128,128));
	SetImageList(&m_imaSmall,LVSIL_SMALL);

	SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_FULLROWSELECT/*|LVS_EX_FLATSB|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES */);	
}

void CBaseListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default		
	int nSel = GetSelectionMark();
	if( nSel != -1 )
	{
		CWnd* pWnd = GetParent();
		if(pWnd != NULL)
		{
			void* pData = (void*)GetItemData(nSel);
			pWnd->SendMessage( USERDEF_UPDATEDATA,0,(LPARAM)pData );
		}
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CBaseListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CBaseListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnMouseMove(nFlags, point);
}

void CBaseListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnRButtonDown(nFlags, point);
}

void CBaseListCtrl::AddHeader(int nKey)
{
	m_pWndHeader = new CHeaderCtrlEx(nKey);
	/*m_pWndHeader->SubclassWindow( GetDlgItem(0,m_hWnd) );*/
}

void CBaseListCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();

	if( m_pWndHeader != NULL )
	{
		m_pWndHeader->UnsubclassWindow();
		delete m_pWndHeader;
		m_pWndHeader = NULL;
	}		
}
