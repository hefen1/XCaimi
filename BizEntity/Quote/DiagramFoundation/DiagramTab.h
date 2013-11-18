#pragma once
#include "DiagramFoundationDef.h"

#define TS_TOP                 0x00000001
#define TS_BOTTOM              0x00000002
#define TS_LEFT                0x00000003
#define TS_RIGHT               0x00000004

#define TS_TABSIZEEQUAL        0x00000010
#define TS_TABSIZEADAPTIVE     0x00000020


class CDiagramTab;
class HS_EXT_CLASS CDiagramTabItem
{
public:
	friend class CDiagramTab;
	CDiagramTabItem()
	{
		m_bVisable = TRUE;
	}
	void		Draw(CDC* pDC,BOOL bActiveItem,CDiagramTab* pDiagarmTab);
	int			GetIndex() { return m_nIndex;}
	void		SetIndex(int nIndex) {m_nIndex = nIndex;}
	CRect		GetRect() {return m_rect;}
	void		SetRect(CRect rc) {m_rect = rc;}
	BOOL		GetVisible() {return m_bVisable;}
	void		SetVisible(BOOL bVisible) {m_bVisable = bVisible;}
	CString		GetCaption() {return m_strCaption;}
	void		SetCaption(CString strCaption) {m_strCaption = strCaption;}
//private:
protected:
	CString   m_strCaption;
	CRect     m_rect;
	int       m_nIndex;
	BOOL      m_bVisable;
};

class HS_EXT_CLASS IDiagramTabNotify
{
public:
	virtual void OnActiveTabChange(CDiagramTabItem* pActiveItem,CDiagramTabItem* pOldActiveItem) = 0;
};

typedef CTypedPtrArray<CPtrArray,CDiagramTabItem*> CDiagramTabItemArray;

class HS_EXT_CLASS CDiagramTab
{
public:
	static int m_nDimensions;
	COLORREF  m_clrBackground;
	COLORREF  m_clrActiveFill;
	COLORREF  m_clrFill;
	COLORREF  m_clrFore;
	COLORREF  m_clrBorder;
	CFont*    m_pItemFont;
public:
	CDiagramTab(void);
	virtual ~CDiagramTab(void);
	void SetStyle(DWORD dwStyle) { m_dwTabStyle = dwStyle; }
	DWORD GetStyle() { return m_dwTabStyle; }
	DWORD GetPostion() { return m_dwTabStyle & 0x0000000F; }
	DWORD GetTabSizeStyle() { return m_dwTabStyle & 0x000000F0; }
	void SetHostWnd(CWnd* pWnd) { m_pHostWnd = pWnd; }
	void SetRect(const CRect &rc) { m_rect = rc; }
	CRect GetRect() { return m_rect; }
	void SetNotify(IDiagramTabNotify* pNotify) { m_pDiagramTabNotify = pNotify; }
	void SetActiveItem(int nIndex) { m_nActiveIndex = nIndex; }

	int GetVisableItemCount();
	UINT AddTabItem(CString strCaption,int nIndex); 
	void RemoveTabItem(CString strCaption);
	void RemoveTabItem(int nIndex);
	void RemoveAllTabItem();
	void ReCalTabItemSize();
	void Draw(CDC *pDC);
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	CWnd*     m_pHostWnd;
	DWORD     m_dwTabStyle;
	CRect     m_rect;
	int       m_nActiveIndex;
	CDiagramTabItemArray   m_tabItems;
	IDiagramTabNotify*     m_pDiagramTabNotify;
};
