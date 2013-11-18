// RichEditCtrlEx.cpp : implementation file
//

#include "stdafx.h"
#include "RichEditCtrlEx.h"


// CRichEditCtrlEx

IMPLEMENT_DYNAMIC(CRichEditCtrlEx, CRichEditCtrl)
CRichEditCtrlEx::CRichEditCtrlEx()
{
}

CRichEditCtrlEx::~CRichEditCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CRichEditCtrlEx, CRichEditCtrl)
END_MESSAGE_MAP()


// CRichEditCtrlEx message handlers
CHARFORMAT CRichEditCtrlEx::GetCharFormat(DWORD dwMask)
{
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);

	cf.dwMask = dwMask;

	GetSelectionCharFormat(cf);

	return cf;
}

BOOL CRichEditCtrlEx::Create(CWnd* pParentWnd, UINT nID)
{
	BOOL bRet = CRichEditCtrl::Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_LEFT|ES_READONLY|WS_VSCROLL|WS_HSCROLL, CRect(0,0,0,0), 
		pParentWnd, nID);

	if( bRet )
	{
		CFont fontNew;
		fontNew.CreateStockObject(DEFAULT_GUI_FONT);// SYSTEM_FIXED_FONT
		SetFont(&fontNew);
	}
	return bRet;
}

void CRichEditCtrlEx::AddItem(CString str, COLORREF clTextColor)
{
	int nLineStart = GetWindowTextLength();
	SetSel(nLineStart, nLineStart);

	//
	CString strWnd;
	GetWindowText(strWnd);
	nLineStart = LineFromChar(strWnd.GetLength()-1);

	CHARFORMAT cf  = GetCharFormat();
	
	if (cf.dwEffects & CFE_AUTOCOLOR) cf.dwEffects -= CFE_AUTOCOLOR;

	cf.dwMask = CFM_COLOR;
	cf.crTextColor = clTextColor;

	SetSelectionCharFormat(cf);

	ReplaceSel((LPCTSTR)str);
}

void CRichEditCtrlEx::ClearScreen()
{
	SetReadOnly(FALSE);
	SetSel(0, -1);
	Clear();
	SetReadOnly(TRUE);
}

void CRichEditCtrlEx::AddItemLN(CString str, COLORREF clTextColor)
{
	str += "\r\n";
	AddItem(str, clTextColor);
}
