// MyListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "MyListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMyListBox
IMPLEMENT_DYNAMIC(CMyListBox, CListBox)

CMyListBox::CMyListBox():m_MovingIndex(-1)
                        ,m_MoveToIndex(-1)
					    ,m_Captured(FALSE)
					    ,m_Oldposition(-1)
                        ,m_ifmove(0)
{

}

CMyListBox::~CMyListBox()
{
}


BEGIN_MESSAGE_MAP(CMyListBox, CListBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// MyListBox 消息处理程序



void CMyListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnLButtonDown(nFlags, point);

	//clear   all   the   flags 
	m_MovingIndex   =   LB_ERR; 
	m_MoveToIndex   =   LB_ERR; 
	m_Captured   =   FALSE; 

	BOOL   Outside; 
	//find   the   item   that   they   want   to   drag 
	//and   keep   track   of   it.   Later   in   the   mouse   move 
	//we   will   capture   the   mouse   if   this   value   is   set 
	int   Index   = ItemFromPoint(point,Outside);       
	m_Oldposition = GetCurSel(); 
	if(Index != LB_ERR && !Outside) 
	{ 
		m_MovingIndex = Index; 
		SetSel(Index); 
	} 
}

void CMyListBox::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_MovingIndex != LB_ERR && m_Captured) 
	{ 
		m_Captured   =   FALSE; 
		ReleaseCapture(); 

		CRect   Rect; 
		GetClientRect(&Rect); 
		//if   they   are   still   within   the   listbox   window 
		if(Rect.PtInRect(point)) 
		{ 
			InsertDraggedItem(); 
		} 
		else 
		{ 
			Invalidate(); 
			UpdateWindow(); 
		} 
		m_MovingIndex   =   LB_ERR; 
		m_MoveToIndex   =   LB_ERR; 
	} 

	CListBox::OnLButtonUp(nFlags, point);
}

void CMyListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnMouseMove(nFlags, point);

	if(nFlags & MK_LBUTTON) 
	{ 
		if(m_MovingIndex != LB_ERR && !m_Captured) 
		{ 
			SetCapture(); 
			m_Captured = TRUE; 
		} 
		BOOL  Outside; 
		int   Index = ItemFromPoint(point,Outside); 
		//if   they   our   not   on   a   particular   item 
		if(Outside) 
		{ 
			CRect  ClientRect; 
			GetClientRect(&ClientRect); 

			//if   they   are   still   within   the   listbox   window,   then 
			//simply   select   the   last   item   as   the   drop   point 
			//else   if   they   are   outside   the   window   then   scroll   the   items 
			if(ClientRect.PtInRect(point)) 
			{ 
				Index  =  LB_ERR; 
				Outside = FALSE; 
			} 
		} 

		if(Index != m_MoveToIndex && !Outside) 
		{ 
			DrawTheLines(Index); 
		} 
	} 
}

void CMyListBox::DrawTheLines(int Index) 
{ 
	CRect ClientRect; 
	GetClientRect(&ClientRect); 

	CDC *pDC = GetDC(); 

	CRect Rect; 
	int Width = 2; 
	if(m_MoveToIndex != m_MovingIndex) 
	{ 
		CPen  Pen(PS_SOLID,Width,RGB(255,255,255)); 
		CPen  *pOldPen = pDC-> SelectObject(&Pen); 
		if(m_MoveToIndex != LB_ERR) 
		{ 
			GetItemRect(m_MoveToIndex,&Rect); 
			if   (ClientRect.PtInRect(Rect.TopLeft())) 
			{ 
				pDC-> MoveTo(Rect.left,Rect.top+1); 
				pDC-> LineTo(Rect.right-(Width/2),Rect.top+1); 
			} 
		} 
		else 
		{ 
			GetItemRect(GetCount()-1,&Rect); 
			if(ClientRect.PtInRect(CPoint(0,Rect.bottom))) 
			{ 
				pDC-> MoveTo(Rect.left,Rect.bottom); 
				pDC-> LineTo(Rect.right-(Width/2),Rect.bottom); 
			} 
		} 
		pDC-> SelectObject(pOldPen); 
	} 
	m_MoveToIndex = Index; 
	if(m_MoveToIndex != m_MovingIndex) 
	{ 
		CPen Pen(PS_SOLID,Width,RGB(0,0,0)); 
		CPen *pOldPen = pDC-> SelectObject(&Pen); 
		if(m_MoveToIndex != LB_ERR) 
		{ 
			GetItemRect(Index,&Rect); 
			if(ClientRect.PtInRect(Rect.TopLeft())) 
			{ 
				pDC-> MoveTo(Rect.left,Rect.top+1); 
				pDC-> LineTo(Rect.right-(Width/2),Rect.top+1); 
			} 
		} 
		else 
		{ 
			GetItemRect(GetCount()-1,&Rect); 
			if(ClientRect.PtInRect(CPoint(0,Rect.bottom))) 
			{ 
				pDC-> MoveTo(Rect.left,Rect.bottom); 
				pDC-> LineTo(Rect.right-(Width/2),Rect.bottom); 
			} 
		} 
		pDC-> SelectObject(pOldPen); 
	} 
	ReleaseDC(pDC); 
} 

void CMyListBox::InsertDraggedItem() 
{ 
	CString Text; 
	DWORD ItemData;//_PTR 
	GetText(m_MovingIndex,Text); 
	ItemData = GetItemData(m_MovingIndex); 
	if(m_MovingIndex < m_MoveToIndex) 
	{ 
		SetRedraw(FALSE); 
		int TopIndex = GetTopIndex(); 
		int NewIndex = InsertString(m_MoveToIndex,Text); 
		SetItemData(NewIndex,ItemData); 
		DeleteString(m_MovingIndex); 
		SetCurSel(NewIndex-1); 
		SetTopIndex(TopIndex); 
		SetRedraw(TRUE); 
		m_ifmove=1; 
		//we   have   to   redraw   the   window   since   we   are   deleting   a   string 
		//after   we   set   the   current   selection.   DeleteString   causes   a   
		//focus   rect   to   show   up   under   the   current   selection 
		Invalidate(); 
		UpdateWindow(); 
	} 
	else if(m_MovingIndex > m_MoveToIndex) 
	{ 
		SetRedraw(FALSE); 
		int TopIndex = GetTopIndex(); 
		DeleteString(m_MovingIndex); 
		int NewIndex = InsertString(m_MoveToIndex,Text); 
		SetItemData(NewIndex,ItemData); 
		SetCurSel(NewIndex); 
		SetTopIndex(TopIndex); 
		SetRedraw(TRUE); 
		m_ifmove=1; 
		//we   have   to   redraw   the   window   since   we   are   deleting   a   string 
		//after   we   set   the   current   selection.   DeleteString   causes   a   
		//focus   rect   to   show   up   under   the   current   selection 
		Invalidate(); 
		UpdateWindow(); 
	} 
	GetParent()-> SendMessage(WM_COMMAND,MAKEWPARAM(GetDlgCtrlID(),LBN_SELCHANGE),(LPARAM)m_hWnd); 
} 
