#pragma once

#include ".\LayoutView.h"
#include ".\LayoutContext.h"
#include ".\HSPanel.h"

// WinnerView ��ͼ
class CWinnerView : public CView,ILayoutWindow
{
	DECLARE_DYNCREATE(CWinnerView)
public:
	CWinnerView(); 
	virtual ~CWinnerView();  

	CLayoutEntity* GetLayoutEntity(CString strLayoutName,CString strOWner);
	CString GenerateLayoutEntityIdentifier(CString strLayoutName,CString  strOwner);
	CString GetCurrentLayoutName();

	virtual BOOL WINAPI OpenLayout(CString strTarget,CString strLayoutName,CString strOwner,IHSParam *pParam = NULL);
	virtual BOOL WINAPI ChangeSize(int cx,int cy);
	virtual BOOL WINAPI RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd);
	virtual BOOL WINAPI UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd);
	virtual int WINAPI DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost);
	virtual BOOL WINAPI RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd);
	virtual BOOL WINAPI UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd);

	BOOL CreateWinnerView(CWnd* pParent);
	void AdjustLayout();
	virtual void OnDraw(CDC* pDC);
	virtual BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PostNcDestroy();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	DECLARE_MESSAGE_MAP()

	CString          m_strLayoutFile;           //���ڲ��������ļ�
	CString          m_strMenuFile;             //�˵��������ļ�
	CLayoutEntity*   m_pActiveLayoutEntity;     //����ʵ����Ϣ��Ϣ
	CMapStringToOb   m_layoutEntityMap;         //�Ӳ���ʵ����Ϣ��Ϣ�б�

	//���������ݲ�����Ϣ�ָ���ͼ
	CLayoutView* CreateLayoutView(CString strLayoutName,CString strOwner);
	void SetActiveView(CLayoutView* pView);
};


