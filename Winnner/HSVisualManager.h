#pragma once

// CHSVisualManager ����Ŀ��

class CHSVisualManager : public CMFCVisualManagerVS2005
{
	DECLARE_DYNCREATE(CHSVisualManager)
public:
	CHSVisualManager();
	virtual ~CHSVisualManager();

	virtual void OnFillBarBackground(CDC* pDC, CBasePane* pBar, CRect rectClient, CRect rectClip, BOOL bNCArea = FALSE);
};


