
// WinnnerView.h : CWinnnerView 类的接口
//


#pragma once

#include "HSSplitterWnd.h"
#include "LayoutContext.h"
#include "HSTabCtrl.h"

#include "..\..\Common\HSWinner.h"

class CLayoutContext;
class CLayoutView : public CView,ILayoutPanel
{
protected: 
	DECLARE_DYNCREATE(CLayoutView)
public:
	CLayoutView();
	virtual ~CLayoutView();

	virtual ILayoutWindow* WINAPI GetLayoutWindow();
	virtual BOOL WINAPI ActiveBizWindow(IBizWindow *pBizWnd);
	virtual void WINAPI ShowLayout(BOOL bShow);

	//注：SetLayoutContext EnableDesignMode 需在CLayoutView对象创建后，在调用视图的Create
	//之前就调用
	void EnableDesignMode(BOOL bEnable) { m_bDesignMode = bEnable; }
	void SetLayoutContext(CLayoutContext* pContext) { m_pLayout = pContext; }

	CHSSplitterWnd*  GetSplitterWnd();
	void AttachPluginWnd();
	void SetPostion(const CRect rc);

	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	CHSSplitterWnd*  m_pWndSplitter; //分割窗口
	CHSTabCtrl*      m_pWndTabs;     //tab页签
	CLayoutContext*  m_pLayout;      //视图所在的布局描述对象
	BOOL             m_bDesignMode;  //是否是设计模式

	BOOL CreateTabCtrl();
	BOOL SplitView(CHSSplitterWnd::SplitType st);
	void AttachPluginWnd(CString strCaption,CString strLocation);	

protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDesignAction(UINT id);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTabChangeActive(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnTabCtrlNotify(WPARAM wParam, LPARAM lParam);
};

//布局实体信息类
class CLayoutEntity
{
private:
	CString          m_strIdentifier;      //布局标识:[owner]布局名称
	CString          m_strLayoutName;         //布局名称
	CLayoutView*     m_pView;              //被分割的视图
	CLayoutContext*  m_pLayout;            //布局描述对象:描述了在m_pView上的分割

public:
	CLayoutEntity(CString strIdentification,CString strLayoutFile,CLayoutView* pView);
	~CLayoutEntity();

	CLayoutView* GetView() { return m_pView;}
	CLayoutContext* GetLayoutContext() { return m_pLayout;}
	CString GetLayoutName() { return m_strLayoutName;}
};

