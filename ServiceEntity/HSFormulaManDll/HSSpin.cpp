#include "StdAfx.h"
#include "HSNumEdit.h"
#include "HSSpin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CHSSpin,CSpinButtonCtrl)
BEGIN_MESSAGE_MAP(CHSSpin,CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CHSSpin)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltaPos)
END_MESSAGE_MAP()

// Class CHSSpin 
CHSSpin::CHSSpin ()
{
	m_delta = 1;
}

void CHSSpin::Change (int steps)
{
	CWnd* b = GetBuddy();
	CHSNumEdit* edit = DYNAMIC_DOWNCAST(CHSNumEdit,b);
	if(edit == NULL) // cast falied
		return;
	double fDelta = steps*m_delta;
	double f = edit->GetValue();
	// This messy bit of code gets around a floating point resolution problem.
	// For example, if your step is -1 and m_Delta is .1, you would end up
	// with -1.49xxx e-9 and it would never set the control to zero. The old
	// 0 != 0 problem.
	if( f == -fDelta )
		f = 0;
	else
		f += fDelta;

	edit->SetValue(f);
}

void CHSSpin::OnDeltaPos (NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	DWORD bWrap = (GetStyle() & UDS_WRAP);
	if ((pNMUpDown->iPos <= pNMUpDown->iDelta) ||
		(pNMUpDown->iPos >= 1000 - pNMUpDown->iDelta)) pNMUpDown->iPos = 500;
	Change(pNMUpDown->iDelta);
	*pResult = 0;
}

void CHSSpin::SetDelta (double x)
{
	m_delta = x;
}
