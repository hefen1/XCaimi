/*************************************************************
*	Copyright (c)2010, 恒生电子股份有限公司
*	All rights reserved.
*
*	文件名称：	TabCtrlExch.h
*	文件标识：	交易设置向导界面自绘TAB
*	摘	  要：	交易测试界面相关,以后再写
*
*	当前版本：	1.0
*	作	  者：	
*	更新日期：
*
*	修改说明：	
*
***************************************************************/
#pragma once
#include <afxtempl.h>

typedef LRESULT (*HS_Call_Notify)(UINT umsg, WPARAM wp, LPARAM lp);
// CTabCtrlExch

class CTabCtrlExch : public CWnd
{
	DECLARE_DYNAMIC(CTabCtrlExch)

public:
	CTabCtrlExch();
	virtual ~CTabCtrlExch();

protected:
	DECLARE_MESSAGE_MAP()


public:
	struct tabItem
	{
		RECT	rcItem;
		CString strData;
		char	bActive;
		int		iImage;
		UINT	uParam;
		void DrawItem(CDC* pDc, COLORREF &crActive, COLORREF &crTxt, COLORREF& crMove, CImageList *pImage = NULL);
		BOOL HitTest(CPoint& pt);
	};

protected:
	CArray<tabItem*, tabItem*> m_ayItem;
	COLORREF m_crBlock;
	COLORREF m_crText;
	COLORREF m_crActive;
	COLORREF m_crMove;
	CRect	 m_rectVialid;
	HS_Call_Notify m_pCall;
	DWORD		m_dwMsg;

public:
	int  GetCurSel();
	void SetCallBreak(HS_Call_Notify pCall, DWORD dwMsg);
	BOOL Create(CWnd* pParent, UINT uID, int nItem, CRect rcClient);
	void SetColors(COLORREF crSelCol = RGB(255, 0, 0), COLORREF crUnSelCol = 0, COLORREF crBk = 0, COLORREF crMove = RGB(0, 255, 0));
	BOOL MousePress(CPoint& pt);
	void MouseMove(CPoint& pt);
	int	 HisTest(CPoint& pt);
	BOOL HitTestEx(CPoint& pt);
	BOOL SetSize(CRect rectClient);
	void AddItem(tabItem* pItem);
	void RemoveAt(int nIdx);
	void RemoveAll();
	BOOL SetItem(int nIdx, tabItem* pItem);
	BOOL SetItem(int nIdx, CString strData, UINT uP = 0, int iM = 0);
	tabItem* GetItem(int nIdx);
	int  GetItemCount() { return m_ayItem.GetSize(); }
	void SetActive(int nIdx);
	void SetActive(UINT uParam);

	BOOL ReDraw();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


