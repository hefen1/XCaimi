// MyComBox.cpp : 实现文件
//

#include "stdafx.h"
#include "MyComBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static WNDPROC m_pWndProc = 0;
static CMyComBox *m_pComboBox = 0;
// CMyComBox

//
// The subclassed COMBOLBOX message handler
//
extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	switch (nMsg) 
	{
		case LB_GETCURSEL: 
			return -1;
		case WM_LBUTTONDOWN: 
			{
				CRect rcClient;
				GetClientRect(hWnd, rcClient);

				CPoint pt;
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);

				if (PtInRect(rcClient, pt)) 
				{
					INT nItemHeight = SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0);
					INT nTopIndex   = SendMessage(hWnd, LB_GETTOPINDEX, 0, 0);

					// Compute which index to check/uncheck
					INT nIndex = nTopIndex + pt.y / nItemHeight;

					CRect rcItem;
					SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LONG)(VOID *)&rcItem);
					if (PtInRect(rcItem, pt)) 
					{
						m_pComboBox->SetCurSel(nIndex);
						m_pComboBox->ShowText();
						// Invalidate this window
						InvalidateRect(hWnd, rcItem, TRUE);
//						m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));
						// Notify that selection has changed
						m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
					}
				}
				// Do the default handling now (such as close the popup
				// window when clicked outside)
				break;
			}
		case WM_LBUTTONUP: 
			{
				// Don't do anything here. This causes the combobox popup
				// windows to remain open after a selection has been made
				//break;
				m_pComboBox->SendMessage(WM_KILLFOCUS, 0, 0);
				return TRUE;
			}
		default:
			break;
	}
	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}

IMPLEMENT_DYNAMIC(CMyComBox, CComboBox)

CMyComBox::CMyComBox()
{
	m_sEdit.SetCharFilter(Unlimited);
	m_sEdit.SetUsed(FALSE);
	m_sEdit.SetShowCount(2);

	m_bTextUpdated   = FALSE;
	m_hListBox = 0;
	m_nCurIndex = -1;
}

CMyComBox::~CMyComBox()
{
}

BEGIN_MESSAGE_MAP(CMyComBox, CComboBox)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CMyComBox 消息处理程序


//
// Make sure the combobox window handle is updated since
// there may be many CCheckComboBox windows active
//
void CMyComBox::OnDropDown() 
{
	m_pComboBox = this;
}

HBRUSH CMyComBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(nCtlColor == CTLCOLOR_EDIT)
	{
		if(m_sEdit.GetSafeHwnd() == NULL)
		{
			m_sEdit.SubclassWindow(pWnd->GetSafeHwnd());
			RecalcText();
			m_sEdit.SetRealText(m_strText);
			m_sEdit.OnShow();
		}
	}
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

LRESULT CMyComBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{
	// If the listbox hasn't been subclassed yet, do so...
	if (m_hListBox == 0) 
	{
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd)
		{
			// Save the listbox handle
			m_hListBox = hWnd;

			// Do the subclassing
			m_pWndProc = (WNDPROC)GetWindowLong(m_hListBox, GWL_WNDPROC);
			SetWindowLong(m_hListBox, GWL_WNDPROC, (LONG)ComboBoxListBoxProc);
		}
	}
 	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}

void CMyComBox::SetHide( BOOL bIsHide )
{
	while(GetCount() > 0)
		DeleteString(0);
	
	if (bIsHide)
	{// 替换上隐藏的文本
		for (int i = 0; i < m_ayHideText.GetCount(); i++)
			CComboBox::AddString(m_ayHideText.GetAt(i));
	}
	else
	{// 还原为正常的文本
		for (int i = 0; i < m_ayText.GetCount(); i++)
			CComboBox::AddString(m_ayText.GetAt(i));
	}
	
	m_sEdit.SetUsed(bIsHide);
	if(m_sEdit.GetSafeHwnd())
	{
		m_sEdit.OnShow();
	}
}

CString CMyComBox::GetEditText()
{
	if(m_sEdit.GetSafeHwnd())
		return m_sEdit.GetRealText();
	return "";
}
void CMyComBox::OnDestroy()
{
	if(m_sEdit.GetSafeHwnd() != NULL)
		m_sEdit.UnsubclassWindow();

	CComboBox::OnDestroy();
}

int CMyComBox::GetNextSel(int nDiff)
{
	int nCul = m_nCurIndex;//GetCurSel();
	nCul -= nDiff;
	
	nCul = (nCul <= -1)?0:nCul;
	nCul = (nCul >= GetCount())?GetCount()-1:nCul;
	return nCul;
}

void CMyComBox::RecalcText()
{
	if (!m_bTextUpdated) 
	{
		// Get the list count
		INT nCount = GetCount();
		int nIndex = m_nCurIndex;
		CString strText("");
		if (nIndex >= 0 && nIndex < nCount)
		{// 获取当前选中项的字符 刨去分隔符以及分隔符之前的字符
			strText = m_ayText.GetAt(nIndex);
			strText = strText.Right(strText.GetLength() - strText.Find(STR_DIVIDE, 0) - 1);
		}
		m_strText = strText;
		m_sEdit.SetRealText(m_strText);
		m_bTextUpdated = TRUE;
	}
}

int CMyComBox::SetCurSel(int nSelect)
{
	m_nCurIndex = nSelect;
	m_bTextUpdated = FALSE;
	return CComboBox::SetCurSel(nSelect);
}

int CMyComBox::AddString(LPCTSTR lpszString)
{
	m_ayText.Add(lpszString);
	
	CString strHide = lpszString;
	CString strAcc = strHide.Right(strHide.GetLength() - strHide.Find(STR_DIVIDE) - 1);
	CString strHideAcc("");
	for(int i = 0; i < strAcc.GetLength() - 2; i++)
		strHideAcc += "*";
	strHideAcc += strAcc.Right(2);
	strHide = strHide.Left(strHide.Find(STR_DIVIDE) + 1) + strHideAcc;
	m_ayHideText.Add(strHide);

	return CComboBox::AddString(lpszString);
}

void CMyComBox::ResetContent()
{
	m_ayHideText.RemoveAll();
	m_ayText.RemoveAll();
	m_sEdit.SetRealText("");
	m_strText = "";
	m_nCurIndex = -1;
	CComboBox::ResetContent();
}

void CMyComBox::RemoveAllItem()
{
	m_ayHideText.RemoveAll();
	m_ayText.RemoveAll();
	m_nCurIndex = -1;

	while(GetCount() > 0)
	{
		DeleteString(0);
	}
}

void CMyComBox::ShowText()
{
	RecalcText();
	if (m_sEdit.GetSafeHwnd())
		m_sEdit.OnShow();
	else
		m_bTextUpdated = FALSE;
}
LRESULT CMyComBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_MOUSEWHEEL)
	{
		CWnd* pFocusWnd = GetFocus();
		if (pFocusWnd == this || pFocusWnd == &m_sEdit)
		{
			short zDelta = (short)HIWORD(wParam);   
			short xPos = (short) LOWORD(lParam);    // horizontal position of pointer
			short yPos = (short) HIWORD(lParam);    // vertical position of pointer

			short nDiff = zDelta / 120;
			int nCul = GetNextSel(nDiff);
			TRACE("当前nCul %d \n", nCul);
			SetCurSel(nCul);
			ShowText();

			CRect rc;
			GetClientRect(&rc);
			InvalidateRect(rc, FALSE);
			return TRUE;
		}
	}
	return CComboBox::WindowProc(message, wParam, lParam);
}
