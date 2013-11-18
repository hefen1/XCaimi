
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

#include "DockableView.h"
#include ".\PanelInfoManager.h"
#include ".\HSMenuBar.h"
#include "..\..\Common\HSWinner.h"

#define NEWINLINEPAGENAME   _T("new_inline_page_name")
#define WM_CHILDEWNDCLOSE WM_USER+1
#define BASE_PANE_ID 1000

#define MWS_NORMAL  WS_POPUP  | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX
#define MWS_ZOOMED  WS_POPUP  | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX

class CMainFrame : public CMDIFrameWndEx ,public IMainWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	static CTime      m_tNoOperatorTime;
	CMapStringToOb    m_dockWndMap; //Key=panel name Value=DockableVie指针
	CMapStringToOb    m_inlineWndMap;//Key=panel name Value=CChildFrame指针
	CMapStringToOb    m_popWndMap;//Key=panel name Value=CChildFrame指针
	CMainFrame();
	virtual ~CMainFrame();

	//IMain接口实现
	virtual BOOL WINAPI ShowPanel(CString strPanelName,UINT uFlag,CString strOwner = PUBLIC_OWNER);
	virtual BOOL WINAPI ClosePanel(CString strPanelName,CString strOwner = PUBLIC_OWNER);
	//virtual BOOL WINAPI NotifyPanel(CString strPanelName,IHSParam* pParam,CString strOwner = PUBLIC_OWNER);
	virtual BOOL WINAPI IsPanelZoomed(CString strPanelName,CString strOwner = PUBLIC_OWNER);
	virtual int WINAPI  DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost);
	virtual HWND GetMainWndHandle() {return m_hWnd; };

	static BOOL OnHotKey(int nCode,WPARAM wParam,LPARAM lParam);
	static BOOL OnNoOperatTimeOut(int nCode,WPARAM wParam,LPARAM lParam);
    	
	//面板操作
	void OpenPanel(CString strPanelName,CString strOwner = PUBLIC_OWNER);
	void HidePanel(CString strPanelName);
	void MaxPanel(CString strPanelName,CString strOwner = PUBLIC_OWNER);
	BOOL IsPanelShow(CString strPanelName,CString strOwner = PUBLIC_OWNER);
	BOOL OnBeforeOpenPanel(CPanelInfo* pInfo);

	BOOL IsTradeLogin();
	BOOL ChangePageName(CString strOldName,CString strNewName);
	void RemovePopupPanel(CString strPanelName);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CHSMenuBar        m_wndMenuBar;
	CMFCStatusBar     m_mfcStatusBar;
	CPanelInfoManager    m_panelInfoMgr;
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void SetApplicationLook(UINT id);
	afx_msg void OnMenuCommand(UINT nID);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	LRESULT OnChildWndClose(WPARAM wParam,   LPARAM  lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()

	CWnd* FindLayoutWindow(CString strKey);
	CWinnerView* CreateDockingWindow(CPanelInfo *pInfo);
	CWinnerView* CreateInlineWindow(CPanelInfo *pInfo);
	CWinnerView* CreatePopupWindow(CPanelInfo *pInfo);

	void GetClientArea(CRect* pRc);
	void InitializeRibbon();

private:
	UINT  m_nPaneID;
	UINT  m_nPaneID2;
	HHOOK m_keyDownHook;
	UINT  CreatePaneID() {return m_nPaneID++;}
	UINT  CreatePaneID2() {return m_nPaneID2--;}	
public:
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


