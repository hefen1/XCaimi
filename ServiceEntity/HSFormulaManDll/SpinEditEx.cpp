// SpinEditEx.cpp : implementation file
//

#include "stdafx.h"
#include "SpinEditEx.h"
#include "Value.h"
#include "VarValue.h"
#include "Express.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinEditEx

CSpinEditEx::CSpinEditEx()
{
	m_pParam = NULL;
}

CSpinEditEx::~CSpinEditEx()
{
}


BEGIN_MESSAGE_MAP(CSpinEditEx, CHSNumSpinEdit)
	//{{AFX_MSG_MAP(CSpinEditEx)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSpinEditEx message handlers

BOOL CSpinEditEx::Create(CWnd* pWnd,CRect rect,UINT nID)
{
	BOOL bRet = CWnd::Create(NULL,"",WS_CHILD|/*WS_VISIBLE|*/WS_TABSTOP|WS_CLIPSIBLINGS,
		rect,pWnd,nID);

	SetDigits(0);			  // set number of trailing digits
	SetRange(0,100,0,true);	  // set range, min, max, wrap around at ends 
	SetStepSize (1);		  // set min step size /deltea
	SetTextColor (255,0,0);   // set texty color - usefull near limits.

	return bRet;
}

void CSpinEditEx::SetEditDef(CValue* pValue)
{
	if( pValue != NULL && pValue->IsStyle(HX_EXPRESSNUMBER) )
	{
		m_pParam = (CNumericValue*)pValue;		
		SetRange(m_pParam->GetMinValue(),m_pParam->GetMaxValue(),m_pParam->GetValue(),1);
	}
	else
	{
		m_pParam = NULL;
	}
}

int	CSpinEditEx::Width()
{
	CRect rect;
	this->GetWindowRect(rect);
	return rect.Width();
}

void CSpinEditEx::OffSet(int x,int y)
{
	CRect rect;
	GetWindowRect(rect);
	MoveWindow(x,y,rect.Width(),rect.Height());
	if(!IsWindowVisible())
	{
		this->ShowWindow(SW_SHOW);
	}
}

long CSpinEditEx::OnEditUpdate(WPARAM wParam, LPARAM lParam)
{
	if(m_pParam != NULL)
	{
		double fValue = GetValue ();
		CString strText;
		strText.Format("%.3f",fValue);
		
		CNumericValue* pValue = (CNumericValue*)m_pParam;
		CString strData;
		strData.Format("%s",strText);
		pValue->SetValue(atof(strData));
		
		if(GetParent() != NULL)
		{
			//GetParent()->SendMessage(CExpression::HX_USER_UPDATEDATA,0,0);
			GetParent()->SendMessage(HX_USER_UPDATEDATA,0,0);
		}
	}
	return 0;
}
