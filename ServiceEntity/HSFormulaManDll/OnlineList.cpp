// OnlineList.cpp : implementation file
//

#include "stdafx.h"
#include "OnlineList.h"
#include "DlgFormulaEdit.h"
#include "CompileEditView.h"
#include "Express.h"
#include "FunExpresion.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COnlineList

COnlineList::COnlineList()
{
	m_nBegin = 0;
	m_nEnd   = 0;
}

COnlineList::~COnlineList()
{
}


BEGIN_MESSAGE_MAP(COnlineList, CListCtrl)
	//{{AFX_MSG_MAP(COnlineList)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnlineList message handlers

void COnlineList::OnDestroy() 
{
	CListCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

int COnlineList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LV_COLUMN lvcolumn;
	CString str = _T("ÇëÄúÑ¡Ôñ");
	for(int i = 0; i < 1; i++)
	{	
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (char*)LPCTSTR(str);
		lvcolumn.iSubItem = i;
		lvcolumn.cx = 10000;
		InsertColumn(i, &lvcolumn);
	}
	
	SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	return 0;
}

void COnlineList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_ESCAPE:
		{
			ShowWindow(SW_HIDE);
		}
		return;
	case VK_PRIOR:
	case VK_NEXT:
	case VK_HOME:
	case VK_END:
	case VK_UP:
	case VK_DOWN:
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	case VK_RETURN:
		this->SendMessage(WM_LBUTTONDBLCLK);
		return;
	}

	CWnd* pWnd = this->GetParent();
	if(pWnd != NULL)
	{
		pWnd->SetFocus();
		pWnd->SendMessage(WM_KEYDOWN,nChar,MAKELONG(nRepCnt,nFlags));
	}
	else
	   CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COnlineList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListCtrl::OnLButtonDblClk(nFlags, point);

	CWnd* pWnd = GetParent();
	if( pWnd == NULL )
	{
		return;
	}
	if( GetItemCount() <= 0 )
	{
		return;
	}
	int nIndex = GetSelectionMark( );
	if( nIndex == -1 )
	{
		nIndex = 0;
		SetSelectionMark(0);
	}
	
	CValue* pValue = (CValue*)GetItemData(nIndex);
	
	CString strText = GetItemText(nIndex,0);
	strText.TrimLeft();
	nIndex = strText.Find("-");
	if(nIndex != -1)
	{
		strText = strText.Left(nIndex);
	}

	if( pValue != NULL && pValue->IsStyle(HX_EXPRESSFUNCTION) )
	{
		CFunExpresion* pFun = (CFunExpresion*)pValue;
		HSAddStrCompileEdit data;			
		//CString strFun;
		//data.m_nCharPos = pFun->GetFunDefMode(strFun);
		data.m_nCharPos = strText.GetLength();
		data.m_strText  = new CString(strText);
		
		((CCompileEditView*)pWnd)->GetRichEditCtrl().SetSel(m_nBegin,m_nEnd);

		pWnd->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&data);
	}
	else
	{
		HSAddStrCompileEdit data;
		data.m_nCharPos = strText.GetLength();
		data.m_strText  = new CString(strText);
		
		if( pValue != NULL && pValue->IsStyle(HX_EXPRESSSENTENCE) )
		{
			((CCompileEditView*)pWnd)->GetRichEditCtrl().SetSel(m_nBegin,m_nEnd);
		}

		pWnd->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&data);

	}

	this->ShowWindow(SW_HIDE);
	pWnd->SetFocus();
}

void COnlineList::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CListCtrl::OnRButtonDown(nFlags, point);
}

void COnlineList::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

void COnlineList::OnKillFocus(CWnd* pNewWnd) 
{

	CListCtrl::OnKillFocus(pNewWnd);
	
	ShowWindow(SW_HIDE);
	
}

void COnlineList::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pWnd = this->GetParent();
	if(pWnd != NULL)
	{
		pWnd->SetFocus();
		pWnd->SendMessage(WM_CHAR,nChar,MAKELONG(nRepCnt,nFlags));
	}
	else
		CListCtrl::OnChar(nChar, nRepCnt, nFlags);
}

BOOL COnlineList::AddToOnlineList(CString& strKey,BOOL bEdit,int nBegin,int nEnd)
{
	BOOL bAll = strKey.IsEmpty();

	m_nBegin = nBegin;
	m_nEnd   = nEnd;

	DeleteAllItems();

	if(bAll && !bEdit)
	{
		POSITION pos = CExpression::m_pExternFunction->GetStartPosition();
		CString sName,strText;
		CValue *pVal;
		CFunExpresion* pFun;
		int nIndex = 0;
		while (pos != NULL)
		{			
			CExpression::m_pExternFunction->GetNextAssoc (pos, sName, pVal);
			pFun = (CFunExpresion*)pVal;
			if(pFun != NULL)
			{
				strText.Format( "%s", pFun->GetFunName() );
				if( !pFun->GetDescribe().IsEmpty() )
				{
					sName.Format("%s-%s",strText,pFun->GetDescribe());
				}
				InsertItem(nIndex,sName);
				nIndex++;
			}
		}
	} 
	else if( !bAll )
	{
		strKey.MakeUpper();
		
		int nIndex = strKey.ReverseFind( '.');
		if(nIndex != -1)
		{
			strKey = strKey.Left(nIndex);
			AddParam(strKey,CExpression::m_pExternExpression);
		}
		else
		{
			Add(strKey,CExpression::m_pExternFunction);
			//Add(strKey,CExpression::m_pExternExpression);
		}
	}

	return this->GetItemCount();
}

void COnlineList::AddParam(CString& strKey,CMapVariabile* pMapVar)
{
	if(pMapVar == NULL) return;

	POSITION pos = pMapVar->GetStartPosition();
	CString sName;
	CValue *pVal;
	int nIndex = 0;
	while (pos != NULL)
	{			
		pMapVar->GetNextAssoc (pos, sName, pVal);
		CExpression* pExpression = pVal->GetExp();
		if( pExpression && pExpression->GetExpressType() == Tech &&
			!strKey.Compare( pExpression->GetName() ) )
		{
			CExpValue* pValue	 = (CExpValue*)pVal;
			pExpression			 = pValue->GetExpByCompile();
			if(pExpression == NULL) return;
			CMapVariabile* pVar = pExpression->GetExpDraw();
			if(pVar == NULL) return;

			CValue *pCurVal;
			pos = pVar->GetStartPosition();
			while (pos != NULL)
			{			
				pVar->GetNextAssoc (pos, sName, pCurVal);
				
				sName += "-";
				sName += _T("»æÍ¼");

				nIndex = InsertItem(0,sName);
				//if(nIndex != -1)
				//{
				//	SetItemData(nIndex,(DWORD)pVal);
				//}
			}
			return;			
		}
	}
}

void COnlineList::Add(CString& strKey,CMapVariabile* pMapVar)
{
	if(pMapVar == NULL) return;

	POSITION pos = pMapVar->GetStartPosition();
	CString sName;
	CValue *pVal;
	int nIndex = 0;
	while (pos != NULL)
	{			
		pMapVar->GetNextAssoc (pos, sName, pVal);
		if( pVal->Find(strKey,sName,FindInName) )
		{
			nIndex = InsertItem(0,sName);
			if(nIndex != -1)
			{
				SetItemData(nIndex,(DWORD)pVal);
			}
		}
	}
}

CWnd* COnlineList::GetParent()
{
	CWnd* pWnd = CWnd::GetParent();
	if(pWnd != NULL)
	{
		return ((CDlgFormulaEdit*)pWnd)->m_pRichEdit;
	}
	return pWnd;
}