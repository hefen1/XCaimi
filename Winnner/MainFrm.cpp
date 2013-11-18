
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include <afximpl.h>

#include "Winner.h"
#include "MainFrm.h"
#include ".\PluginMgr.h"
#include ".\ChildFrm.h"
#include ".\HSVisualManager.h"
#include ".\MessagePromptDialog.h"
#include ".\PopupWindow.h"

#include "..\HelperLib\GeneralHelper.h"
#include "..\Common\TradeComm\TradeCoreIntf.h"
#include "..\Common\KeyboardInterface.h"
#include "WinnerApplication.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

CTime CMainFrame::m_tNoOperatorTime = CTime::GetCurrentTime();

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND_RANGE(MENUID_BEGIN,MENUID_END,&CMainFrame::OnMenuCommand)
	ON_MESSAGE (WM_CHILDEWNDCLOSE,OnChildWndClose)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_SYSKEYDOWN()
	ON_WM_CHAR()
	ON_WM_TIMER()
END_MESSAGE_MAP()

LRESULT CALLBACK HSWinnerGetMsgProc(int nCode,WPARAM wParam,LPARAM lParam)
{ 
	CMainFrame::OnNoOperatTimeOut(nCode,wParam,lParam);

	BOOL bResult = CMainFrame::OnHotKey(nCode,wParam,lParam);
	return bResult ? TRUE : CallNextHookEx(NULL,nCode,wParam,lParam);
}

BOOL CMainFrame::OnNoOperatTimeOut(int nCode,WPARAM wParam,LPARAM lParam)
{
	if( !HsGetHotKeyManager()->NeedNoOperatTimeoutNotify() )
		return FALSE;

	MSG *pMsg = (MSG*)lParam;
	//if (nCode == HC_ACTION) 
	{
		//热键
		switch (pMsg->message)
		{
		case WM_KEYDOWN:    
		case WM_SYSKEYDOWN:
		case WM_MOUSEMOVE:
			{
				m_tNoOperatorTime = CTime::GetCurrentTime();
			}	
		}
	}
	return TRUE;
}

BOOL CMainFrame::OnHotKey(int nCode,WPARAM wParam,LPARAM lParam)
{
	MSG *pMsg = (MSG*)lParam;
	BOOL bResult = FALSE;
	if (nCode == HC_ACTION) 
	{
		//热键
		switch (pMsg->message)
		{
		case WM_KEYDOWN:    
		case  WM_SYSKEYDOWN:   
			{
				DWORD vkCode = pMsg->wParam;
				UINT fsModifiers = 0;
				//control是否按下
				if( (::GetKeyState(VK_CONTROL) & 0x8000)  != 0 )
					fsModifiers |= MOD_CONTROL;
				//shift是否按下
				if( (::GetKeyState(VK_SHIFT) & 0x8000)  != 0 )
					fsModifiers |= MOD_SHIFT;
				//alt是否按下
				if( (::GetKeyState(VK_MENU) & 0x8000)  != 0 )
				{
					fsModifiers |= MOD_ALT;
				}
				bResult = HsGetHotKeyManager()->OnHotKey(fsModifiers,vkCode);
			}
		default:
			break;
		} 
	}
	return bResult;
}

CMainFrame::CMainFrame()
{
	m_nPaneID = BASE_PANE_ID;
	m_nPaneID2 = BASE_PANE_ID - 1;
	CPaneDivider::m_nDefaultWidth = 2;
	m_keyDownHook = NULL;
	m_bCanConvertControlBarToMDIChild = TRUE;
}

CMainFrame::~CMainFrame()
{
	if(m_keyDownHook)
		UnhookWindowsHookEx(m_keyDownHook);
}


BOOL CMainFrame::ClosePanel(CString strPanelName,CString strOwner)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
		return FALSE;

	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		pWnd->CloseWindow();
		m_inlineWndMap.RemoveKey(strKey);
		AdjustDockingLayout();
	}
	else if(pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))	
	{
		GetDockingManager()->RemovePaneFromDockManager((CDockableView*)pWnd,TRUE,TRUE);
		m_dockWndMap.RemoveKey(strKey);
		AdjustDockingLayout();
	}

	return TRUE;
}

BOOL CMainFrame::ShowPanel(CString strPanelName,UINT uFlag,CString strOwner)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CLogTimeSapn lts(_T("CMainFrame::ShowPanel") + strPanelName);

	if(uFlag == SP_SHOW)
	{
		OpenPanel(strPanelName,strOwner);
	}
	else if(uFlag == SP_MAX)
	{
		MaxPanel(strPanelName,strOwner);
	}
	else if(uFlag == SP_HIDE)
	{
		HidePanel(strPanelName);
	}
	else if(uFlag == SP_RESTORE)
	{

	}
	return TRUE;
}

// BOOL CMainFrame::NotifyPanel(CString strPanelName,IHSParam* pParam,CString strOwner/* = PUBLIC_OWNER*/)
// {
// 	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
// 	if(pInfo == NULL)
// 		return FALSE;
// 
// 	CString strKey;
// 	strKey.Format("%s",pInfo->GetPanelName());
// 	CWnd* pWnd = FindLayoutWindow(strKey);
// 	if(pWnd == NULL)
// 		return FALSE;
// 
// 	CLayoutEntity* pEntity = NULL;
// 	if(pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
// 	{
// 		CChildFrame* pChildFrame = (CChildFrame*)pWnd;
// 		CWinnerView* pWinnerView  = pChildFrame->GetWinnerView();
// 		pEntity = pWinnerView->GetLayoutEntity(pInfo->m_strLayoutName,strOwner);
// 		
// 	}
// 	else if(pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))	
// 	{
// 		CDockableView* pDockView = (CDockableView*)pWnd;
// 		CWinnerView* pWinnerView  = pDockView->GetWinnerView();
// 		pEntity = pWinnerView->GetLayoutEntity(pInfo->m_strLayoutName,strOwner);
// 	}
// 	else
// 		return FALSE;
// 	if(pEntity)
// 	{
// 		pEntity->GetLayoutContext()->NotifyToBizWindows(WM_BIZWINDOW_NOTIFY,
// 			BWN_EXTERNAL_CHANGE_CONTENT,LPARAM(pParam));
// 		return TRUE;
// 	}
// 	return FALSE;
// }

BOOL CMainFrame::IsPanelZoomed(CString strPanelName,CString strOwner)
{
	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
		return FALSE;

	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		return TRUE;
	}
	else if(pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))	
	{
		CDockableView* pDockView = (CDockableView*)pWnd;
		if (pDockView->IsMDITabbed())
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CMainFrame::DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost)
{
	return HsGetLinkageMsgManager()->DispatchLinkageMsg(nMsg,wParam,lParam,bPost);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	// 基于持久值设置视觉管理器和样式
	SetApplicationLook(0);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = FALSE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = FALSE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = FALSE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = FALSE; // 在选项卡区域的右边缘启用文档菜单
	mdiTabParams.m_bFlatFrame = TRUE;
	//EnableMDITabbedGroups(TRUE, mdiTabParams);


	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	//菜单条
// 	if (!m_wndMenuBar.Create(this))
// 	{
// 		return -1;
// 	}
// 	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
// 	CMFCPopupMenu::SetForceMenuFocus(FALSE);// 防止菜单栏在激活时获得焦点
// 	// 菜单栏可停靠
// 	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
// 	EnableDocking(CBRS_ALIGN_ANY);
// 	DockPane(&m_wndMenuBar);

	CDockingManager::SetDockingMode(DT_SMART);// 启用 Visual Studio 2005 样式停靠窗口行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为

	//构造主菜单以及对应的action
 	CString strMenuFile = CGeneralHelper::GetSpecifyPath(CGeneralHelper::SYSCONFIG_PATH) 
 		+ _T("sysconfig.xml");
 	m_panelInfoMgr.LoadAllPanelInfo(strMenuFile);

	m_keyDownHook = SetWindowsHookEx(WH_GETMESSAGE,HSWinnerGetMsgProc,
		NULL,GetCurrentThreadId());

	SetTimer(1, 1000, NULL);    // 设置定时器
	return 0;	
}

void CMainFrame::InitializeRibbon()
{
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(_T("hello"));
	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
// 
// 	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_T("aaa"), 1, 1);
// 	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_NEW, _T("bb"), 0, 0));
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;

// 	if(AfxGetApp()->m_nCmdShow == SW_MAXIMIZE)
// 		cs.style = MWS_ZOOMED;
// 	else
// 		cs.style = MWS_NORMAL; 

	return TRUE;
}

CWnd* CMainFrame::FindLayoutWindow(CString strKey)
{
	CWnd* pWnd = NULL;
	if( m_inlineWndMap.Lookup(strKey,(CObject*&)pWnd) != 0)
		return pWnd;
	if( m_dockWndMap.Lookup(strKey,(CObject*&)pWnd) != 0)
		return pWnd;
	if( m_popWndMap.Lookup(strKey,(CObject*&)pWnd) != 0)
		return pWnd;
	return NULL;
}

CWinnerView* CMainFrame::CreateDockingWindow(CPanelInfo *pInfo)
{
	//pInfo->GetPanelName();
	CLogTimeSapn ts(_T("CMainFrame::CreateDockingWindow ") + pInfo->GetPanelName());

	//检查窗口是否已存在
	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))
	{
		CDockableView* pDockPane = (CDockableView*)pWnd;
		if(pDockPane->IsMDITabbed())
		{
			pDockPane->GetParent()->ShowWindow(SW_MAXIMIZE);
		}
		else
		{
			ShowPane(pDockPane,TRUE,FALSE,TRUE);
		}
		return pDockPane->GetWinnerView();
	}

	//创建停靠窗口
	CDockableView* pDockPane = new CDockableView(TRUE);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
	DWORD dwPosStype = NULL;
	if(pInfo->m_strPostion.Find('L') >=0  || pInfo->m_strPostion.Find('l') >= 0)
		dwPosStype = CBRS_LEFT;
	else if(pInfo->m_strPostion.Find('R') >=0  || pInfo->m_strPostion.Find('r') >= 0)
		dwPosStype = CBRS_RIGHT;
	else if(pInfo->m_strPostion.Find('T') >=0  || pInfo->m_strPostion.Find('t') >= 0)
		dwPosStype = CBRS_TOP;
	else if(pInfo->m_strPostion.Find('B') >=0  || pInfo->m_strPostion.Find('b') >= 0)
		dwPosStype = CBRS_BOTTOM;	
	else
		dwPosStype = CBRS_BOTTOM;
	dwStyle |= dwPosStype;
	CRect rc(0,0,pInfo->m_size.cx,pInfo->m_size.cy);
	DWORD dwTabbedStyle = AFX_CBRS_REGULAR_TABS;
	DWORD dwControlBarStyle = AFX_DEFAULT_DOCKING_PANE_STYLE;
	UINT nPanelId = 0;	

	if(pInfo->m_strPostion.Find('F') >= 0 || pInfo->m_strPostion.Find('f') >= 0)
	{
		nPanelId = CreatePaneID2();
		dwControlBarStyle = 0;
	}
	else
	{
		nPanelId = CreatePaneID();
	}

	if (!pDockPane->Create(pInfo->m_strCaption,this,rc,
		FALSE,nPanelId,dwStyle,dwTabbedStyle,dwControlBarStyle) )
	{
		delete pDockPane;
		return NULL;
	}
	//pDockPane->SetMinSize(pInfo->m_size);
	pDockPane->EnableDocking(CBRS_ALIGN_ANY/*dwPosStype*/);
	m_dockWndMap.SetAt(strKey,pDockPane);	

	UINT nLeftID = 0;
	CDockablePane* pAttchPane = NULL;
	for(UINT i = BASE_PANE_ID; i < m_nPaneID; i++)
	{
	   CBasePane* pPane = GetPane(i);
	   if(pPane && pPane->IsKindOf(RUNTIME_CLASS(CDockableView)))
	   {
			DWORD dwPaneStye = pPane->GetStyle();
			if(dwPaneStye & dwPosStype)
			{ 
				pAttchPane = (CDockablePane*)pPane;
			}
	   }
	}

	if(pAttchPane != NULL)
	{// 如果在当前位置已经有停靠 附加上去
		CDockablePane* pTabbedBar = NULL;
		pDockPane->AttachToTabWnd(pAttchPane, DM_SHOW, TRUE, &pTabbedBar);
	}
	else
	{
		DockPane(pDockPane);
	}	

	return pDockPane->GetWinnerView();
}

CWinnerView* CMainFrame::CreateInlineWindow(CPanelInfo *pInfo)
{
	CLogTimeSapn ts(_T("CMainFrame::CreateInlineWindow ") + pInfo->GetPanelName());

	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		CChildFrame* pChildFram = (CChildFrame*)pWnd;
		pChildFram->ActivateFrame(SW_SHOW);

		CWinnerView* pWinnerView = pChildFram->GetWinnerView();
		ASSERT(pWinnerView != NULL);
		return pWinnerView;
	}

	CChildFrame *pChildFram = new CChildFrame(this);
	DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CHILD | WS_MAXIMIZE;
	dwStyle &= ~WS_SYSMENU;
	if( !pChildFram->Create(NULL,pInfo->m_strCaption,dwStyle,
		CRect(0,0,0,0),this)  )
	{
		delete pChildFram;
		return FALSE;
	}
	m_inlineWndMap.SetAt(strKey,pChildFram);

	CWinnerView* pWinnerView = (CWinnerView*)pChildFram->GetActiveView();
	return pWinnerView;
}

CWinnerView* CMainFrame::CreatePopupWindow(CPanelInfo *pInfo)
{
	CLogTimeSapn ts(_T("CMainFrame::CreatePopupWindow ") + pInfo->GetPanelName());

	CWinnerView* pWinnerView = NULL;
	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CPopupWindow)))
	{
		pWnd->ShowWindow(SW_SHOW);
		pWinnerView = ((CPopupWindow*)pWnd)->GetWinnerView();
		return pWinnerView;
	}

	CRect rc(0,0,pInfo->m_size.cx,pInfo->m_size.cy);
	CPopupWindow* pPopup = new CPopupWindow(this);
	pPopup->m_panelInfo.Copy(pInfo);
	pPopup->m_strCaption = pInfo->m_strCaption;
	pPopup->m_strLayoutName = pInfo->m_strLayoutName;
	pPopup->m_wndSize = rc;
	pPopup->Create(CPopupWindow::IDD,this);
	pPopup->ShowWindow(SW_HIDE);

	m_popWndMap.SetAt(strKey,pPopup);

	pWinnerView = pPopup->GetWinnerView();
	return pWinnerView;
}

void CMainFrame::GetClientArea(CRect* pRc)
{
	CRect rc1;
	GetClientRect(&rc1);
	CRect rc2;
	m_wndMenuBar.GetClientRect(&rc2);
	
	pRc->left = rc2.left;
	pRc->top = rc2.bottom + 2;
	pRc->right = rc1.right;
	pRc->bottom = rc1.bottom;
}

BOOL CMainFrame::IsTradeLogin()
{
	CObjectManager* pObjManager = HsGetObjectManager();
	ITradeCoreObj* pTradeCoreObj = (ITradeCoreObj*) pObjManager->GetObj(OBJ_TRADECORE);
	if(pTradeCoreObj)
	{
		IAccountMng* pAccountMng = pTradeCoreObj->GetAccountMng();
		if(pAccountMng != NULL && pAccountMng->GetCount(atTrade) > 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::SetApplicationLook(UINT id)
{
	CWaitCursor wait;
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
	//CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CHSVisualManager));
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
 	}
	SetWindowText(_T("投资赢家金融理财终端"));
	ShowWindow(SW_HIDE);
	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//return __super::OnCreateClient(lpcs,pContext);
	CMenu* pMenu = GetMenu();
	if(! CreateClient(lpcs, pMenu) )
	{
		return FALSE;
	}
	if (m_bDoSubclass)
	{
		m_wndClientArea.SubclassWindow(m_hWndMDIClient);
	}
	return TRUE;
}

void CMainFrame::OnMenuCommand(UINT nID)
{
	CString str = m_wndMenuBar.GetActionID(nID);
	if(str == MA_NEWPAGE)
	{
		CPanelInfo panelInfo(NEWINLINEPAGENAME);
		CWinnerView* pWinnerView = CreateInlineWindow(&panelInfo);
		ASSERT(pWinnerView != NULL);
		pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,panelInfo.m_strLayoutName,PUBLIC_OWNER);
		return;
	}
	else if(str == MA_EXITAPP)
	{
		SendMessage(WM_CLOSE);
		return;
	}
	m_wndMenuBar.OnMenuMsg(nID);
}

BOOL CMainFrame::ChangePageName(CString strOldName,CString strNewName)
{
	CWnd* pWnd = FindLayoutWindow(strNewName);
	if(pWnd)
	{
		CMessagePromptDialog dlg;
		CString strError;
		strError.Format(_T("命名失败.\n【%s】已存在."),strNewName);
		dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
		return FALSE;
	}

	pWnd = FindLayoutWindow(strOldName);
	if(pWnd && pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		m_inlineWndMap.RemoveKey(strOldName);
		m_inlineWndMap.SetAt(strNewName,pWnd);
		return TRUE;
	}
	return FALSE;
}

void CMainFrame::RemovePopupPanel(CString strPanelName)
{
	m_popWndMap.RemoveKey(strPanelName);
}


BOOL CMainFrame::OnBeforeOpenPanel(CPanelInfo* pInfo)
{
	if(pInfo == NULL)
	{
		return FALSE;
	}

	if( pInfo->m_strGrant.Find("T") != -1)
	{
		if( !IsTradeLogin())
		{
			CWinnerApp* pApp = (CWinnerApp*)AfxGetApp();
			LoginRet loginResult = pApp->DoLogin(LoginType_OnlyTrade);
			return loginResult != LoginRet_Error;
		}
	}

	return TRUE;
}

BOOL CMainFrame::IsPanelShow(CString strPanelName,CString strOwner/* = PUBLIC_OWNER*/)
{
	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
	{
		CMessagePromptDialog dlg;
		CString strError;
		strError.Format(_T("命令执行失败.\n错误:找不到面板描述信息(%s)."),strPanelName);
		dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
		return FALSE;
	}

	CString strPanelKey;
	strPanelKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strPanelKey);
	if(pWnd)
		return pWnd->IsWindowVisible();
	else
		return FALSE;
}

void CMainFrame::OpenPanel(CString strPanelName,CString strOwner /*= PUBLIC_OWNER*/)
{
	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
	{
		CMessagePromptDialog dlg;
		CString strError;
		strError.Format(_T("命令执行失败.\n错误:找不到面板描述信息(%s)."),strPanelName);
		dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
		return;
	}

	if( !OnBeforeOpenPanel(pInfo) )
		return;

	CString strPanelKey;
	strPanelKey.Format("%s",pInfo->GetPanelName());
	BOOL bAllreadyExist = FALSE;
	if(FindLayoutWindow(strPanelKey))
		bAllreadyExist = TRUE;

	if(pInfo->m_strPostion == _T("M"))
	{
		CWinnerView* pWinnerView = CreateInlineWindow(pInfo);
		ASSERT(pWinnerView != NULL);
		if(!bAllreadyExist)
			pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,pInfo->m_strLayoutName,strOwner);
		else
			pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,pWinnerView->GetCurrentLayoutName(),strOwner);
	}
	else if (pInfo->m_strPostion == _T("P"))
	{
		CWinnerView* pWinnerView = CreatePopupWindow(pInfo);
		ASSERT(pWinnerView != NULL);
		if(!bAllreadyExist)
			pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,pInfo->m_strLayoutName,strOwner);
		CString strKey;
		strKey.Format("%s",pInfo->GetPanelName());
		CPopupWindow* pPop = (CPopupWindow*)FindLayoutWindow(strKey);
		pPop->ShowWindow(SW_SHOW);
	}
	else
	{
		CWinnerView* pWinnerView = CreateDockingWindow(pInfo);
		ASSERT(pWinnerView != NULL);
		if(!bAllreadyExist)
			pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,pInfo->m_strLayoutName,strOwner);
		else
			pWinnerView->OpenLayout(CURRENT_LAYOUT_TARGET,pWinnerView->GetCurrentLayoutName(),strOwner);
	}
}

void CMainFrame::HidePanel(CString strPanelName)
{
	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
		return;

	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	if(pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	else if(pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))	
	{
		CDockableView* pDockView = (CDockableView*)pWnd;
		if (pDockView->IsMDITabbed())
		{
			pDockView->GetParent()->ShowWindow(SW_HIDE);
		}
		else
		{
			ShowPane((CDockableView*)pWnd,FALSE,FALSE,FALSE);
		}			
	}
}

void CMainFrame::MaxPanel(CString strPanelName,CString strOwner/* = PUBLIC_OWNER*/)
{
	CPanelInfo* pInfo = m_panelInfoMgr.GetPanelInfo(strPanelName);
	if(pInfo == NULL)
		return;

	CString strKey;
	strKey.Format("%s",pInfo->GetPanelName());
	CWnd* pWnd = FindLayoutWindow(strKey);
	CWinnerView* pWinnerView = NULL;
	if(pWnd->IsKindOf(RUNTIME_CLASS(CChildFrame)))
	{
		pWnd->ShowWindow(SW_MAXIMIZE);
		pWinnerView = (CWinnerView*)((CChildFrame*)pWnd)->GetActiveView();
	}
	else if(pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)))	
	{
		CDockableView* pDockView = (CDockableView*)pWnd;
		pWinnerView = pDockView->GetWinnerView();
		if (pDockView->IsMDITabbed())
		{
			CMDIChildWndEx* pMDIChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, pDockView->GetParent());
			if (pMDIChild == NULL)
			{
				ASSERT(FALSE);
				return;
			}

			CMDIFrameWndEx* pTabbedFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, pDockView->GetDockSiteFrameWnd());
			if (pTabbedFrame == NULL)
			{
				ASSERT(FALSE);
				return ;
			}

			pTabbedFrame->TabbedDocumentToControlBar(pMDIChild);
		}
		else
		{
			pDockView->LockWindowUpdate();
			pDockView->ConvertToTabbedDocument();
			pDockView->GetParentFrame()->ShowWindow(SW_MAXIMIZE);
			pDockView->UnlockWindowUpdate();
		}
	}
	CLayoutEntity* pLayoutEntity = pWinnerView->GetLayoutEntity(pInfo->m_strLayoutName,strOwner);
	pLayoutEntity->GetLayoutContext()->NotifyToBizWindows(WM_BIZWINDOW_NOTIFY,BWN_LAYOUT_STATE_CHANGE,0);
}


LRESULT CMainFrame::OnChildWndClose(WPARAM wParam, LPARAM lParam)
{
	CWnd *pWnd = NULL;
	POSITION pos;
	CString key;
	for( pos = m_inlineWndMap.GetStartPosition(); pos != NULL; )
	{
		m_inlineWndMap.GetNextAssoc( pos, key, (CObject*&)pWnd);
		if((WPARAM)pWnd == wParam)
		{
			m_inlineWndMap.RemoveKey(key);
			return 0;
		}
	}
	return 0;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
// 	if(nID == SC_RESTORE || nID == SC_MAXIMIZE)
// 	{
// 		if( IsZoomed() || \\\)
// 			ModifyStyle(0, WS_SIZEBOX);
// 		else 
// 			ModifyStyle(WS_SIZEBOX, 0);
// 	}
	
	__super::OnSysCommand(nID, lParam);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CLOSE)
	{
		if( MessageBox(_T("是否退出程序?"),_T("理财终端"),MB_YESNO) != IDYES)
			return 0;
	}
	return __super::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	AdjustDockingLayout();
}

void CMainFrame::OnClose()
{
	HsGetServicePluginMgr()->NotifyToPlugin(SN_BEFORE_FRAME_CLOSE);
	__super::OnClose();
}


void CMainFrame::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	__super::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYUP)
	{
		if(pMsg->wParam == VK_MENU || pMsg->wParam == VK_F10 ) 
			return TRUE;
	}
	return __super::PreTranslateMessage(pMsg);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	static BOOL bInitTime = FALSE;
	if(!bInitTime)
	{
		m_tNoOperatorTime = CTime::GetCurrentTime();
		bInitTime = TRUE;
	}
	CTimeSpan ts = CTime::GetCurrentTime() - m_tNoOperatorTime;
	HsGetHotKeyManager()->OnNoOperatTimeout(ts.GetTotalSeconds());
}
