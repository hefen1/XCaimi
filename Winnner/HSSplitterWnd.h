#pragma once

// CHSSplitterWnd

class CHSSplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CHSSplitterWnd)
public:
	enum SplitType
	{
		XSplit,YSplit
	};
protected:
	BOOL      m_bSpliting;
	SplitType m_splitType;
	BOOL      m_bHasSpliter;
	//support for hide/show pane
	int m_nHiddenCol;   
	int m_nHiddenRow; 
	int m_nPaneSize[2];
	int m_nPaneMinSize[2];

	void RememberSize();
	BOOL HasSplieter();
public:
	CHSSplitterWnd();
	virtual ~CHSSplitterWnd();

	virtual CWnd* GetActivePane(int* pRow, int* pCol);

	void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg); 
	void OnLButtonDown(UINT /*nFlags*/, CPoint pt); 
	void TrackRowSize(int y, int row);

	BOOL SplitWindow(SplitType st,CWnd* pParent,
		CRuntimeClass* pClass1,CRuntimeClass* pClass2);
	void RecalcSize(CRect rect,int nPane1Size,int nPane2Size);

	BOOL ToggleSide(int nIndex);
	void ShowRow();               //显示行
	void HideRow(int rowHide);    //隐藏行
	void ShowColumn();            //显示列
	void HideColumn(int colHide); //隐藏列

	BOOL IsSplited();
	BOOL IsVerticalSplited();

	void EnableSpliter(BOOL bEnable) { m_bHasSpliter = bEnable; }

	COLORREF m_bkColor; 
	COLORREF m_lineColor; 
	BOOL m_bFixed; 

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
};

