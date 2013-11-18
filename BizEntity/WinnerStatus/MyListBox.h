#pragma once

// MyListBox

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	int   m_Oldposition; 
	int   m_ifmove;

private:
	int    m_MovingIndex; 
	int    m_MoveToIndex; 
	BOOL   m_Captured; 

	void   InsertDraggedItem(); 
	void   DrawTheLines(int   Index); 
};


