#include "StdAfx.h"
#include "HSNumSpinEdit.h"
#include "HSNumRangeEdit.h"
#include "HSSpin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT CHSNumEdit_ONEDITUPDATE;
extern CString hxSubZero(CString);

IMPLEMENT_DYNAMIC(CHSNumSpinEdit,CWnd  )
BEGIN_MESSAGE_MAP(CHSNumSpinEdit ,CWnd)
	//{{AFX_MSG_MAP(CHSNumSpinEdit )
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	
	ON_MESSAGE(CHSNumEdit_ONEDITUPDATE,OnEditUpdate)

END_MESSAGE_MAP()	

// Class CHSNumSpinEdit 
CHSNumSpinEdit::CHSNumSpinEdit ()
{
	m_spin = new CHSSpin();
	m_edit = new CHSNumRangeEdit();
}

CHSNumSpinEdit::~CHSNumSpinEdit ()
{
	delete m_spin;
	delete m_edit;
}

int CHSNumSpinEdit::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
	CWnd::OnCreate(lpCreateStruct);
	int cx = lpCreateStruct->cx;
	int cy = lpCreateStruct->cy;
	int x = lpCreateStruct->x;
	int y = lpCreateStruct->y;
	
	//int v = m_edit->Create(WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE| ES_AUTOHSCROLL |ES_RIGHT,CRect(x,y,cx-15,cy),this,1);
	int v = m_edit->CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
		WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE| ES_AUTOHSCROLL |ES_RIGHT,CRect(x,y,cx-15,cy),
		this, 1, NULL);
	
	m_spin->Create(WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|UDS_ALIGNRIGHT|UDS_ARROWKEYS,CRect(x+cx,y,15,y+cy),this,2);
	m_spin->SetBuddy(m_edit);
	return 0;
}

void CHSNumSpinEdit::OnSize (UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType,cx,cy);
	if ( cx < 60 )
	{
		m_edit->MoveWindow(0,0,cx,cy);
		m_spin->MoveWindow(cx-15,0,0,cy);
	}
	else
	{
		m_edit->MoveWindow(0,0,cx-15,cy);
		m_spin->MoveWindow(cx-15,0,15,cy);
	}
}

void CHSNumSpinEdit::MoveWindow(int x, int y, int nWidth, int nHeight,
				BOOL bRepaint /*= TRUE*/)
{
	CWnd::MoveWindow(x, y, nWidth, nHeight,bRepaint);
	if ( nWidth < 20 )
	{
		m_edit->MoveWindow(0,0,nWidth,nHeight);
		m_spin->MoveWindow(nWidth-15,0,0,nHeight);
	}
	else
	{
		m_edit->MoveWindow(0,0,nWidth-15,nHeight);
		m_spin->MoveWindow(nWidth-15,0,15,nHeight);
	}
}

void CHSNumSpinEdit::MoveWindow(LPCRECT lpRect, BOOL bRepaint /*= TRUE*/)
{
	MoveWindow(lpRect->left,lpRect->top,abs(lpRect->right - lpRect->left),
	  		   abs(lpRect->bottom - lpRect->top),bRepaint);
}

void CHSNumSpinEdit::OnKeyDown (UINT nChar, UINT nRepCnt, UINT flags)
{
	switch ( nChar) 
	{
	case VK_PRIOR :	 
		m_spin->Change(10);
		break;
	case VK_NEXT :	
		m_spin->Change(-10);
		break;
	default :
		CWnd::OnKeyDown(nChar,nRepCnt,flags);
	}
}

CHSSpin* CHSNumSpinEdit::GetSpinner ()
{
	return m_spin;
}

CHSNumRangeEdit* CHSNumSpinEdit::GetEdit ()
{
	return m_edit;
}

void CHSNumSpinEdit::OnSetFocus (CWnd* pOldWnd)
{
	m_edit->SetFocus();
}

BOOL CHSNumSpinEdit::PreTranslateMessage (MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN  )
	{
		if (pMsg->wParam == VK_PRIOR  || pMsg->wParam == VK_NEXT )
		{
			OnKeyDown(pMsg->wParam , LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
			return TRUE;
		}
	}
	return FALSE;
}

void CHSNumSpinEdit::SetDigits (int no)
{
	ASSERT(m_edit);
	m_edit->SetDigits(no);
}

void CHSNumSpinEdit::SetRange (double min, double max,double def, bool wrap)
{
	ASSERT(m_edit);

	CString strDefault;
	strDefault.Format("%.3f",def);	
	strDefault = hxSubZero(strDefault);

	// BY HS CHAGE BEGIN

	// Move Run Location From _TABLE_FROM To _TABLE_TO
	// _TABLE_FROM
	// m_edit->SetWindowText(strDefault);

	m_edit->SetRange(min,max);
	m_edit->SetWrap(wrap);
	m_spin->SetRange((int)min - 1,(int)max + 1);

	// _TABLE_TO
	m_edit->SetWindowText(strDefault);

	// BY HS CHAGE END
}

void CHSNumSpinEdit::SetStepSize (double dx)
{
	ASSERT(m_spin);
	m_spin->SetDelta(dx);
}

void CHSNumSpinEdit::SetFont (CFont* pFont, BOOL bRedraw)
{
	m_edit->SetFont(pFont,bRedraw);
}

HBRUSH CHSNumSpinEdit::OnCtlColor (CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH brush;
	brush = CWnd::OnCtlColor (pDC,pWnd,nCtlColor);
	
	if (pWnd == m_edit)
		pDC->SetTextColor(m_textColor);
	
	return brush ;
}

void CHSNumSpinEdit::SetTextColor (int r, int b, int g)
{
	m_textColor = RGB(r,g,b);
}

double CHSNumSpinEdit::GetValue ()
{
	return m_edit->GetValue();
}

void CHSNumSpinEdit::SetValue (double val)
{
	m_edit->SetValue(val);
}

long CHSNumSpinEdit::OnEditUpdate(WPARAM wParam, LPARAM lParam) 
{ 
	return 0; 
}