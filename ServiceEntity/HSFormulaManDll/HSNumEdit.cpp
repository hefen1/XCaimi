
#include "StdAfx.h"
#include <math.h>
#include "HSNumEdit.h"
#include "HSNumSpinEdit.h"
#include "RegMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND			 g_hParentWnd;
extern CString hxSubZero(CString str);
UINT CHSNumEdit_ONEDITUPDATE = ::RegisterWindowMessage (HS_NUMEDIT_EDITUPDATE);

IMPLEMENT_DYNAMIC(CHSNumEdit, CEdit)
BEGIN_MESSAGE_MAP(CHSNumEdit, CEdit)
	//{{AFX_MSG_MAP(CHSNumEdit)
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

CHSNumEdit::CHSNumEdit ()
{
	m_digits = 3;
}

void CHSNumEdit::SetDigits (int noOfDigitsAfterZero)
{
	m_digits = 3;//noOfDigitsAfterZero;
}

int CHSNumEdit::GetDigits ()
{
	return m_digits;
}

double CHSNumEdit::GetValue ()
{
	CString txt;
	GetWindowText(txt);
	double fTemp = atof(txt);
	int nFlag = CheckInput((double)fTemp);
	if( nFlag == 0 )
	{
		return fTemp;
	}
	else if( nFlag > 0 )
	{
		return GetMax();
	}
	else
	{
		return GetMin();
	}
	//return (double) atof(txt);
}

void CHSNumEdit::SetValue (double x)
{
	CString tmp;
	CString fmt;
	fmt.Format("%%0.%df",m_digits);
	tmp.Format (fmt,(x+(0.5/pow( (double)10, m_digits+1) ) ) );

	tmp = hxSubZero(tmp);
	SetWindowText (tmp);
}

void CHSNumEdit::OnChar (UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( !isdigit(nChar) && nChar != 8 && nChar != '.' && nChar != '-' && nChar != '+' )
		return;

	CString strWin;
	GetWindowText(strWin);

	CEdit::OnChar(nChar,nRepCnt,nFlags);
	CString strCurText;
	GetWindowText(strCurText);

	if( !CheckInput(strCurText) )
	{
		SetWindowText(strWin);
	}

		/*
	if( isdigit(nChar) && CheckInput(nChar) )
		CEdit::OnChar(nChar,nRepCnt,nFlags);
	else
	{
		CString txt;
		GetWindowText(txt);
		switch (nChar) 
		{
		case '+' : // set to absolute value
			{
				double x = (double) atof(txt);
				if (x > 0)
					break; // only break if we already >0, else do "case -"
			}
		case '-' : // change sign.
			{
				double x = (double) atof(txt);
//				if(x == 0)
//				{
//					::MessageBeep(MB_ICONQUESTION);
//					break;
//				}
				x = -x;
				SetValue(x);
				break;
			}
		case '.' :	 
				if ( txt.Find('.') == -1 && CheckInput(nChar))
					CEdit::OnChar(nChar, nRepCnt, nFlags);
				break; 
		case VK_BACK:
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			break;
		}
	}
	*/
}

bool CHSNumEdit::CheckInput (UINT nChar)
{
	// checks no digit or dots before a minus sign
	// and no more than m_digits after '.'
	int pos = CharFromPos(GetCaretPos()); 
	CString txt;
	GetWindowText(txt);
	int len = txt.GetLength();
	//////// no digits or dot before a minus sign
	int sign = txt.Find('-');
	if ( pos <= sign )
		return false;
	//// no dot before m_digit from end of string
	int dot = txt.ReverseFind('.');
	if ( dot == -1 )
	{
		// no dot - check position before accepting
		if ( pos >= len -m_digits )
			return true;
		//else
		//	return false;
	}
	/////// limit digits after '.' 
	if ( len - pos < dot) 
	{
		// no insert if 
		// there is more than m_digits digits after zero.
		if ( pos -dot > m_digits )
			return false;
		// the insert will cause the same
		if ( len - dot> m_digits) 
			return false;
	}
	return true;
}

void CHSNumEdit::OnUpdate()
{
	CWnd* pWnd = this->GetParent();
	if(pWnd != NULL)
	{
		((CHSNumSpinEdit*)pWnd)->OnEditUpdate(0,0);
	}
}

void CHSNumEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	double fTemp1 = GetValue();
	CString strWin;
	GetWindowText(strWin);
	double fTemp2 = atof(strWin);

	if( fTemp2 != fTemp1 )
	{
		SetWindowText(strWin);
	}
}

BOOL CHSNumEdit::CheckInput(CString strNum)
{
	if( strNum.IsEmpty() )
		return TRUE;
	//if( strNum.GetLength() >= 8 )
	//	return FALSE;

	//  «∑ÒŒ™ ‰»Î∑∂Œß
	//{
	//	double fTemp = atof(strNum);
	//	int nFlag = CheckInput((double)fTemp);
	//	if( nFlag == 0 )
	//	{
	//	}
	//	else if( nFlag > 0 )
	//	{
	//		//AfxMessageBox("≥¨π˝ ‰»Î∑∂Œß");
	//		return FALSE;
	//	}
	//	else
	//	{
	//		//AfxMessageBox("≥¨π˝ ‰»Î∑∂Œß");
	//		return FALSE;
	//	}
	//}

	char *pTemp;

	pTemp = strNum.GetBuffer();

	if( !isdigit(pTemp[0]) && pTemp[0] != '.' && pTemp[0] != '-' && pTemp[0] != '+' )
	{
		return FALSE;
	}

	BOOL bDut = FALSE;
	for(int i = 1; i < strNum.GetLength(); i++ )
	{
		if( pTemp[i] == '.' )
		{
			if( bDut == TRUE )
			{
				return FALSE;
			}
			bDut = TRUE;
		}

		if( pTemp[i] == '-' || pTemp[i] == '+' )
		{
			return FALSE;
		}
	}
	return TRUE;
}
