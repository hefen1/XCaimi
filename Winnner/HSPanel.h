#pragma once
#include "Winner.h"

// CPluginWndContainer ¶Ô»°¿ò

class CHSPanel : public CView
{
	DECLARE_DYNAMIC(CHSPanel)
private:
	HWND  m_hChildWnd;
	BOOL  m_bSeflFree;
public:
	CHSPanel(BOOL bSeflFree = TRUE,CWnd* pParent = NULL);
	virtual ~CHSPanel();
	void SetPluginHandle(HWND  hwnd);
	DECLARE_MESSAGE_MAP()	

protected:
	virtual void PostNcDestroy();
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);	
};
