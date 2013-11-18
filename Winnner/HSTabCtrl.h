#pragma once

#define TABCTRL_NOTIFY  WM_USER + 100

#define TN_RBUTTONUP 1

struct TabCtrlNotify
{
	UINT nId;
	CPoint pt;
};


// CHSTabCtrl
class CHSTabCtrl : public CMFCTabCtrl
{
	DECLARE_DYNAMIC(CHSTabCtrl)

public:
	CHSTabCtrl();
	virtual ~CHSTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


