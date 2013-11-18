#include "StdAfx.h"
#include "HSTitle.h"
#include "DiagramFoundation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern CDiagramFoundationApp theFoundationApp;

CHSTitle::CHSTitle()
{
	m_pFont = theFoundationApp.m_iFont->GetDiagramTableTitleFont();
	m_clrBK = theFoundationApp.m_iColor->GetTableTitleBK();
	m_clrFG = theFoundationApp.m_iColor->GetTableTitle();
	m_str.Empty();
}

CHSTitle::~CHSTitle()
{
}

CRect CHSTitle::SetRect( CRect rc )
{
	if (rc.Height() < TITLE_HEIGHT)
	{// ���������ĸ߶�С�ڱ���Ĭ�ϸ߶� ����ⲻ��ʾ ֱ�ӷ���
		m_rcTitle.SetRectEmpty();
		return rc;
	}

	m_rcTitle = rc;
	m_rcTitle.bottom = m_rcTitle.top + TITLE_HEIGHT;

	rc.top = m_rcTitle.bottom;
	return rc;
}
void CHSTitle::SetText( CString str )
{
	m_str = str;
}
void CHSTitle::Draw( CDC* pDC )
{
	if (m_rcTitle.IsRectEmpty())
	{
		return;
	}

	// ������
	pDC->FillSolidRect(&m_rcTitle, m_clrBK);

	// ������
	int nBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF fgClr = pDC->SetTextColor(m_clrFG);
	CFont* oldFont = pDC->SelectObject(m_pFont);
	pDC->DrawText(m_str, m_str.GetLength(), &m_rcTitle, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(fgClr);
	pDC->SetBkMode(nBkMode);

}
