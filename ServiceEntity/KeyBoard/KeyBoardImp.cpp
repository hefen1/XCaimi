#include "stdafx.h"
#include "KeyBoardImp.h"
#include "WndKeyboard.h"
#include "KeyboardDataList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CString m_OldstrKey;
extern HWND  g_hEditUpdateParentWnd;
extern WORD  g_nPageType;
CKeyBoard::CKeyBoard()
{
	m_pWndKeyboard = NULL;
}
CKeyBoard::~CKeyBoard()
{
	if ( m_pWndKeyboard)
	{
		if (::IsWindow(m_pWndKeyboard->m_hWnd))
		{
			m_pWndKeyboard->DestroyWindow();
		}
		delete m_pWndKeyboard;
	}
}

void CKeyBoard::CreateKeyboard(HWND hParent)
{
	if (m_pWndKeyboard == NULL)
	{
		CRect rect;
		CWnd* pWnd = NULL;//CWnd::FromHandle(hParent);
		rect.SetRectEmpty();
		m_pWndKeyboard = new CWndKeyboard;
		m_pWndKeyboard->AddStyle(DS_BYKEYBOARD|DS_NotBerth);
		m_pWndKeyboard->Create(pWnd,rect);
		m_pWndKeyboard->ShowWindow(SW_HIDE);
	}
}

void CKeyBoard::InitKeyboard()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_pWndKeyboard == NULL)
	{
		CreateKeyboard(NULL);
	}
	CKeyboardDataList* pKeyList = m_pWndKeyboard->GetKeyboard();
	if(pKeyList == NULL)
		return;
	pKeyList->InitKeyboard();
}

void CKeyBoard::ShowKeyboard(char nChar, HSShowKeyboardInfo* pKeyInfo, HWND hParent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_pWndKeyboard == NULL)
	{
		CreateKeyboard(hParent);
		InitKeyboard();
	}	
	HSShowKeyboardInfo* pInfo = pKeyInfo;
	CString strData;
	CRect rect;
	rect.SetRectEmpty();
	if (pInfo)
	{
		rect.CopyRect(pInfo->m_oRect);
		g_nPageType = pInfo->m_nPageType;
	}

	if(!m_pWndKeyboard->IsWindowVisible() &&
		!(nChar > 0x7f)	  && // ºº×Ö
		m_pWndKeyboard->GetEdit() != NULL )
	{
		m_pWndKeyboard->GetEdit()->SetWindowText("");
		m_pWndKeyboard->GetEdit()->SetFocus();
		m_OldstrKey.Empty();
	}

 	if ( nChar=='\0'  && pInfo && pInfo->m_cResever)
 	{
 		m_pWndKeyboard->GetEdit()->SetWindowText(pInfo->m_cResever);
 	}
	
	m_pWndKeyboard->SendMessage(hxUSER_EDITMSG,nChar,(LPARAM)m_pWndKeyboard->GetList());
		
	
	if( !m_pWndKeyboard->IsStyle(DS_ALWAYSSHOW) &&  pInfo &&
		(rect.left != 0 || rect.top != 0) )
	{
		CRect rc;
		m_pWndKeyboard->GetWindowRect(rc);
		
		if( pInfo->IsStyle(HSShowKeyboardInfo::left) )
		{
			rect.right = rect.left + rc.Width();
			if( rect.left < 0 )
			{
				rect.left  = 0;
				rect.right = rc.Width();
			}
		}
		if( pInfo->IsStyle(HSShowKeyboardInfo::top) )
		{
			rect.bottom = rect.top + rc.Height();
		}
		if( pInfo->IsStyle(HSShowKeyboardInfo::right) )
		{
			rect.left  = rect.right - rc.Width();
			if( rect.left < 0 )
			{
				rect.left  = 0;
				rect.right = rc.Width();
			}
		}
		if( pInfo->IsStyle(HSShowKeyboardInfo::bottom) )
		{
			rect.top = rect.bottom - rc.Height();
			if( rect.top < 0 )
			{
				rect.top = 0;
				rect.bottom = rc.Height();
			}
		}
		if (pInfo->IsStyle(HSShowKeyboardInfo::stock))
		{
			m_pWndKeyboard->GetKeyboard()->RemoveStyle(HSShowKeyboardInfo::all);
			m_pWndKeyboard->GetKeyboard()->AddStyle(HSShowKeyboardInfo::stock);
		}
		if (pInfo->IsStyle(HSShowKeyboardInfo::key))
		{
			m_pWndKeyboard->GetKeyboard()->RemoveStyle(HSShowKeyboardInfo::all);
			m_pWndKeyboard->GetKeyboard()->AddStyle(HSShowKeyboardInfo::key);
		}
		if (pInfo->IsStyle(HSShowKeyboardInfo::express))
		{
			m_pWndKeyboard->GetKeyboard()->RemoveStyle(HSShowKeyboardInfo::all);
			m_pWndKeyboard->GetKeyboard()->AddStyle(HSShowKeyboardInfo::express);
		}
		if ( pInfo->m_pWnd )
			g_hEditUpdateParentWnd = pInfo->m_pWnd->m_hWnd;
		if(rc != rect)
		{
			m_pWndKeyboard->MoveWindow(rect);
		}
	}
	if ( hParent )
		g_hEditUpdateParentWnd = hParent;
	//m_pWndKeyboard->ShowWindow(SW_SHOW);
}