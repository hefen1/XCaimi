
#include "StdAfx.h"
#include "HSNumRangeEdit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CHSNumRangeEdit,CHSNumEdit)
BEGIN_MESSAGE_MAP(CHSNumRangeEdit,CHSNumEdit)
	//{{AFX_MSG_MAP(CHSNumRangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CHSNumRangeEdit::CHSNumRangeEdit ()
{
}

double CHSNumRangeEdit::GetMax ()
{
	return m_max;
}

double CHSNumRangeEdit::GetMin ()
{
	return m_min;
}

void CHSNumRangeEdit::SetRange (double min, double max)
{
	m_max = max;
	m_min = min;
}

int CHSNumRangeEdit::CheckInput (double fData)
{
	if( fData < m_min )
		return -1;
	if(  fData > m_max )
		return 1;
	return 0;
}

bool CHSNumRangeEdit::CheckInput (UINT nChar)
{
	if ( !CHSNumEdit::CheckInput(nChar) )
		return false;
	if ( isdigit(nChar) )
	{
		int pos = CharFromPos(GetCaretPos()); 
		int nStartChar;
		int nEndChar;
		GetSel( nStartChar, nEndChar ) ;
		CString txt;
		GetWindowText(txt);
		CString input;
		input.Format("%c",nChar);
		if(nStartChar != nEndChar)
		{
			CString strLeft,strRight;
			strLeft  = txt.Mid(0,nStartChar);
			strRight = txt.Mid(nEndChar,txt.GetLength());
			txt.Format("%s%s%s",strLeft,input,strRight);
		}
		else
		{
			txt.Insert(pos,input);
		}
		double value = (double )atof(txt);
		if ( value > m_max || value < m_min )
		{
			::MessageBeep(MB_ICONQUESTION);
			SetValue(value);
			return false;
		}
	}
	return true;
  
}

void CHSNumRangeEdit::SetValue (double x)
{
	if (x > m_max)
	{
		if( m_wrap )
			x = m_min;
		else
			x = m_max;
	}
	else if (x < m_min)
	{
		if( m_wrap )
			x = m_max;
		else
			x = m_min;
	}
	CHSNumEdit::SetValue(x); 
}

void CHSNumRangeEdit::SetWrap (bool wrap)
{
	m_wrap = wrap;
}

//void CHSNumRangeEdit::OnUpdate()
//{
//	float f = GetValue();
//	SetValue(f);
//}
BOOL CHSNumRangeEdit::PreTranslateMessage(MSG* pMsg)
{
	BOOL bReturn = CHSNumEdit::PreTranslateMessage(pMsg);

	//if( pMsg && (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) )
	//{
	//	float f = GetValue();
	//	SetValue(f);
	//}
	return bReturn;
}
