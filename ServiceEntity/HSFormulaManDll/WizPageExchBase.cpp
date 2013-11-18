#include "stdafx.h"
#include "WizPageExchBase.h"

CExchCalcMain* CWizPageExchBase::m_pMainTech = NULL;

CWizPageExchBase::CWizPageExchBase(CWnd* pParent)  // standard constructor
	:CHSNewWizPage("",pParent)
{
	m_pData = NULL;
}

CWizPageExchBase::CWizPageExchBase(LPCTSTR lpszTemplateName, CWnd* pParent )
	:CHSNewWizPage(lpszTemplateName, pParent)
{
	m_pData = NULL;
}

CWizPageExchBase::CWizPageExchBase(UINT nIDTemplate, CWnd* pParent)
	:CHSNewWizPage(nIDTemplate, pParent)
{
	m_pData = NULL;
}

CWizPageExchBase::~CWizPageExchBase(void)
{
	m_pData = NULL;
}
BEGIN_MESSAGE_MAP(CWizPageExchBase, CHSNewWizPage)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CWizPageExchBase::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CHSNewWizPage::OnChar(nChar, nRepCnt, nFlags);
}

void CWizPageExchBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if( nChar == VK_RETURN )
	{
		CWnd* pParent = this->GetParent();
		if( pParent )
		{
			pParent->PostMessage(WM_KEYDOWN,VK_RETURN,0);
			return;
		}
	}
	CHSNewWizPage::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CWizPageExchBase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg 
		&& (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR) 
		&& pMsg->wParam == VK_RETURN )
	{
		CWnd* pParent = GetParent();
		if( pParent )
		{
			pParent->SendMessage(WM_KEYDOWN,VK_RETURN,0);
			return TRUE;
		}
	}

	return CHSNewWizPage::PreTranslateMessage(pMsg);
}
