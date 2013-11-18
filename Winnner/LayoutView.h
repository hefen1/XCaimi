
// WinnnerView.h : CWinnnerView ��Ľӿ�
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

	//ע��SetLayoutContext EnableDesignMode ����CLayoutView���󴴽����ڵ�����ͼ��Create
	//֮ǰ�͵���
	void EnableDesignMode(BOOL bEnable) { m_bDesignMode = bEnable; }
	void SetLayoutContext(CLayoutContext* pContext) { m_pLayout = pContext; }

	CHSSplitterWnd*  GetSplitterWnd();
	void AttachPluginWnd();
	void SetPostion(const CRect rc);

	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	CHSSplitterWnd*  m_pWndSplitter; //�ָ��
	CHSTabCtrl*      m_pWndTabs;     //tabҳǩ
	CLayoutContext*  m_pLayout;      //��ͼ���ڵĲ�����������
	BOOL             m_bDesignMode;  //�Ƿ������ģʽ

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

//����ʵ����Ϣ��
class CLayoutEntity
{
private:
	CString          m_strIdentifier;      //���ֱ�ʶ:[owner]��������
	CString          m_strLayoutName;         //��������
	CLayoutView*     m_pView;              //���ָ����ͼ
	CLayoutContext*  m_pLayout;            //������������:��������m_pView�ϵķָ�

public:
	CLayoutEntity(CString strIdentification,CString strLayoutFile,CLayoutView* pView);
	~CLayoutEntity();

	CLayoutView* GetView() { return m_pView;}
	CLayoutContext* GetLayoutContext() { return m_pLayout;}
	CString GetLayoutName() { return m_strLayoutName;}
};

