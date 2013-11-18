#pragma once

// CHSSplitterWnd
#define  SPLITTERGAP 2
class CQuoteSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CQuoteSplitterWnd)
public:
	enum SplitType
	{
		XSplit,YSplit
	};
protected:
	BOOL      m_bSpliting;
	SplitType m_splitType;
	COLORREF m_bkColor; 
	COLORREF m_lineColor; 
	//support for hide/show pane
	BOOL m_bHideItem;
	int m_nPaneSize[2];
	int m_nPaneMinSize[2];

	void RememberSize();
public:
	CQuoteSplitterWnd();
	virtual ~CQuoteSplitterWnd();

	void HideSplitter();

	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg); 
	void OnInvertTracker(CRect &rect) ;
	void OnLButtonDown(UINT /*nFlags*/, CPoint pt); 


	// special command routing to frame
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) ;
	afx_msg void OnPaint();


// 	BOOL m_bFixed; 

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


