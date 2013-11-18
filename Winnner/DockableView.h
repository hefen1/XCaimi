#pragma once

#include ".\WinnerView.h"
#include ".\LayoutContext.h"
#include ".\HSPanel.h"
// CDockableView

class CDockableView : public CDockablePane
{
	DECLARE_DYNAMIC(CDockableView)
protected:
	CWinnerView*     m_pWinnerView;
	BOOL             m_bFreeSelf;
public:
	CDockableView(BOOL bFreeSelf = FALSE);
	virtual ~CDockableView();

	void AdjustLayout();
	CWinnerView*  GetWinnerView() { return m_pWinnerView;}

	virtual void EnableGripper(BOOL bEnable);
	virtual CTabbedPane* CreateTabbedPane();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PostNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


