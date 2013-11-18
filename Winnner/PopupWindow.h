#pragma once

#include "WinnerView.h"
#include "MainFrm.h"

// CPopupWindow ¶Ô»°¿ò

class CPopupWindow : public CDialog
{
	DECLARE_DYNAMIC(CPopupWindow)

public:
	CString m_strLayoutName;
	CString m_strCaption;
	CRect   m_wndSize;
	CPanelInfo m_panelInfo;
	CPopupWindow(CWnd* pParent = NULL);
	virtual ~CPopupWindow();
	CWinnerView* GetWinnerView() { return m_pWinnerView; }

	enum { IDD = IDD_POPUPWND };

protected:
	CWinnerView*   m_pWinnerView;
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void AdjustLayout();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();
};
