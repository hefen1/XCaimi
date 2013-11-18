
// WinnnerView.cpp : CWinnnerView 类的实现
//

#include "stdafx.h"
#include "Winner.h"

#include ".\LayoutView.h"
#include ".\ChildFrm.h"
#include ".\PluginMgr.h"
#include ".\HSPanel.h"
#include ".\BizWindowContext.h"
#include ".\SaveLayoutDialog.h"
#include ".\WinnerView.h"
#include ".\MainFrm.h"

#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinnnerView
#define IDR_DESIGN_BEGIN    1
#define IDR_DESIGN_END      IDR_DESIGN_BEGIN + 1000
IMPLEMENT_DYNCREATE(CLayoutView, CView)

BEGIN_MESSAGE_MAP(CLayoutView, CView)
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEACTIVATE()
	ON_COMMAND_RANGE(IDR_DESIGN_BEGIN, IDR_DESIGN_END, &CLayoutView::OnDesignAction)
	ON_WM_CREATE()
	ON_MESSAGE (TABCTRL_NOTIFY,OnTabCtrlNotify)
	ON_REGISTERED_MESSAGE(AFX_WM_CHANGE_ACTIVE_TAB,&CLayoutView::OnTabChangeActive)
	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CWinnnerView 构造/析构
CLayoutView::CLayoutView()
{
	m_pWndSplitter = new CHSSplitterWnd();
	m_pLayout = NULL;
	m_pWndTabs = NULL;
	m_bDesignMode = FALSE;
}

CLayoutView::~CLayoutView()
{
	if(m_pWndSplitter)
		delete m_pWndSplitter;

	if(m_pWndTabs)
		delete m_pWndTabs;
}

ILayoutWindow* CLayoutView::GetLayoutWindow()
{
	ASSERT(m_pLayout != NULL);
	return (ILayoutWindow*)m_pLayout->GetLayoutWindow();
}

BOOL CLayoutView::ActiveBizWindow(IBizWindow *pBizWnd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	if(m_pWndTabs == NULL)
		return FALSE;

	int nTabNum = m_pWndTabs->GetTabsNum();
	if(nTabNum <= 1)
		return TRUE;

	CLayoutNode* pNode = m_pLayout->GetLayoutNode(this);
	ASSERT(pNode != NULL);
	for(int i = 0; i < nTabNum; i++)
	{
		CString strTabLable;
		m_pWndTabs->GetTabLabel(i,strTabLable);
		
		for(int j = 0 ; j < pNode->m_bizWndArray.GetCount(); j ++)
		{
			CNodeWndDescription *pDescription = pNode->m_bizWndArray.GetAt(j);
			if( (strTabLable == pDescription->m_strCaption) && 
				(pBizWnd == pDescription->m_pBizWindow) )
			{
				m_pWndTabs->SetActiveTab(i);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CLayoutView::ShowLayout(BOOL bShow)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CWnd* pWnd = this->GetParent();
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CHSSplitterWnd)));
	CHSSplitterWnd* pSpliter = (CHSSplitterWnd*)pWnd;
	ASSERT(pSpliter != NULL);

	CLayoutNode *pLayoutNode =  m_pLayout->GetLayoutNode(this);
	ASSERT(pLayoutNode != NULL);
	TCHAR cLocation = pLayoutNode->GetLocation();
	if(cLocation == 'L' || cLocation == 'T')
		pSpliter->ToggleSide(0);
	else
		pSpliter->ToggleSide(1);
}

BOOL CLayoutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CWinnnerView 绘制

void CLayoutView::OnDraw(CDC* pDC)
{
}

BOOL CLayoutView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CLayoutView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLayoutView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if(!m_bDesignMode)
		return;

	CBizWindowContext* pWndContext = (CBizWindowContext*)HsGetObjectManager()->GetObj(CBizWindowContext::m_strClassName);
	BOOL bHasAttach = FALSE;
	if(m_pWndTabs)
		bHasAttach = m_pWndTabs->GetTabsNum() > 0;
	pWndContext->EnableSplitAction(!bHasAttach);
	CMenu* pMenu = pWndContext->GetDesignMenu();
	CMFCPopupMenu* pPopupMenu = theApp.GetContextMenuManager()->ShowPopupMenu(pMenu->m_hMenu, point.x, point.y, 
		this,TRUE);
	//SetFocus();
}

LRESULT CLayoutView::OnTabCtrlNotify(WPARAM wParam, LPARAM lParam)
{
	this->SetFocus();
	TabCtrlNotify *pTN = (TabCtrlNotify*)lParam;
	if(pTN->nId == TN_RBUTTONUP)
	{
		CPoint pt = pTN->pt;
		this->ClientToScreen(&pt);
		OnContextMenu(NULL,pt);
	}
	
	return 0;
}

LRESULT CLayoutView::OnTabChangeActive(WPARAM wp, LPARAM lp)
{	
	CLayoutNode *pNode = m_pLayout->GetLayoutNode(this);
	if(pNode)
	{
		if( pNode->m_bizWndArray.GetCount() <= 0 )
			return -1;

		int nNewTab = (int)wp;
		for(int i = 0; i < pNode->m_bizWndArray.GetCount(); i++ )
		{
			CNodeWndDescription* pNodeWndDescrip = pNode->m_bizWndArray.GetAt(i);
			if(pNodeWndDescrip == NULL)
				continue;

			IBizWindow *pBizWnd = pNodeWndDescrip->m_pBizWindow;
			if( pBizWnd != NULL)
				::SendMessage(pBizWnd->GetHandle(),WM_BIZWINDOW_NOTIFY,BWN_WINDOW_SHOW,i == nNewTab);
		}
	}
	return 0;
}

// CWinnnerView 消息处理程序

void CLayoutView::OnSize(UINT nType, int cx, int cy)
{
	ASSERT(m_pWndSplitter != NULL);

	CRect rc(0,0,cx,cy);
	if(m_pLayout != NULL)
	{	//保存变动到布局信息
		CLayoutNode* pNode = m_pLayout->GetLayoutNode(this);
		ASSERT(pNode != NULL);
		CString strTemp(_T("TtBb"));
		if(strTemp.Find(pNode->GetLocation()) >= 0 ) //上下分取cy
			pNode->SetSize(cy);
		else
			pNode->SetSize(cx);

		//更新内部窗口位置
		if( m_pWndSplitter->IsSplited() )
		{	
			if(pNode && !pNode->IsLeafNode())
			{
				int nPane1Size = pNode->GetLeftNode()->GetSize();
				int nPane2Size = pNode->GetRightNode()->GetSize();
				m_pWndSplitter->RecalcSize(rc,nPane1Size,nPane2Size);
			}
		}
	}
	else
	{
		m_pWndSplitter->RecalcSize(rc,0,0);
	}

	if(m_pWndTabs != NULL)
	{
		m_pWndTabs->SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
		m_pWndTabs->RecalcLayout();
	}
}

void CLayoutView::SetPostion(const CRect rc)
{
	MoveWindow (0, 0, rc.Width(),rc.Height());
	//SetWindowPos (NULL, -1, -1, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	OnSize(NULL,rc.Width(),rc.Height());
}


int CLayoutView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CWnd *pParent = GetParent();
	if(pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CChildFrame)))
		return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return TRUE;
}

void CLayoutView::OnDesignAction(UINT id)
{		
	CBizWindowContext* pWndContext = (CBizWindowContext*)HsGetObjectManager()->GetObj(CBizWindowContext::m_strClassName);
	ASSERT(pWndContext != NULL);

	if( id == pWndContext->GetYSplitActionID())
		SplitView(CHSSplitterWnd::YSplit);
	else if(id == pWndContext->GetXSplitActionID())
		SplitView(CHSSplitterWnd::XSplit);
	else if(id == pWndContext->GetSaveLayoutActionID())
	{
		CSaveLayoutDialog dlg;
		if( dlg.DoModal() == IDOK)
		{
			CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
			ASSERT(pMain != NULL);
			BOOL bSucess = pMain->ChangePageName(NEWINLINEPAGENAME,dlg.m_strLayoutName);
			if(bSucess)
			{
				CString strTemp;
				strTemp.Format(_T("%s.xml"),CGeneralHelper::GetAppPath() + _T("layout\\self\\") 
					+ dlg.m_strLayoutName); 
				m_pLayout->SaveLayoutInfo(strTemp);
			}
			
		}
	}
	else
	{
	    CBizWindowInfo *pInfo =	pWndContext->GetBizWndInfo(id);
		if(pInfo)
			AttachPluginWnd(pInfo->GetCaption(),pInfo->GetLocation());
	}
}

BOOL CLayoutView::SplitView(CHSSplitterWnd::SplitType st)
{
	CRuntimeClass* pClass = RUNTIME_CLASS(CLayoutView);
	CLayoutView* pLeftView = NULL;
	CLayoutView* pRightView = NULL;
	CLayoutNode* pNode = m_pLayout->GetLayoutNode(this);
	TCHAR cLeftLocation,cRightLocation;
	ASSERT(pNode != NULL);
	if(st == CHSSplitterWnd::XSplit)
	{
		m_pWndSplitter->SplitWindow(CHSSplitterWnd::XSplit,this,pClass,pClass);
		pLeftView = (CLayoutView*)m_pWndSplitter->GetPane(0,0);
		pRightView = (CLayoutView*)m_pWndSplitter->GetPane(1,0);	
		cLeftLocation = 'T';
		cRightLocation = 'B';
	}
	else
	{
		m_pWndSplitter->SplitWindow(CHSSplitterWnd::YSplit,this,pClass,pClass);	
		pLeftView = (CLayoutView*)m_pWndSplitter->GetPane(0,0);
		pRightView = (CLayoutView*)m_pWndSplitter->GetPane(0,1);
		cLeftLocation = 'L';
		cRightLocation = 'R';
	}
	pLeftView->EnableDesignMode(TRUE);
	pRightView->EnableDesignMode(TRUE);
	pLeftView->SetLayoutContext(m_pLayout);
	pRightView->SetLayoutContext(m_pLayout);

	//增加布局节点信息
	CLayoutNode* pLeftNode = new CLayoutNode();
	pLeftNode->SetLocation(cLeftLocation);
	pLeftNode->SetView(pLeftView);

	CLayoutNode* pRightNode = new CLayoutNode();
	pRightNode->SetLocation(cRightLocation);
	pRightNode->SetView(pRightView);

	pNode->SetLeftNode(pLeftNode);
	pNode->SetRightNode(pRightNode);

	CRect rc;
	GetClientRect(&rc);
	m_pWndSplitter->RecalcSize(rc,0,0);	

	return TRUE;
}


void CLayoutView::AttachPluginWnd()
{
	//m_pLayout的值需在视图创建后调用[CLayoutView::SetLayoutContext()] 注入
	ASSERT(m_pLayout != NULL);
	if(m_pLayout == NULL) 
		return;

	CLayoutNode* pNode = m_pLayout->GetLayoutNode(this);
	ASSERT(pNode != NULL);
	if(pNode == NULL)
		return;

	if(m_pWndTabs != NULL)
		return;

	if(pNode->m_bizWndArray.GetCount() <= 0)
		return;

	CreateTabCtrl();

	CBizPluginMgr* pPluginMgr =  (CBizPluginMgr*)HsGetObjectManager()->GetObj(CBizPluginMgr::m_strClassName);
	for(int i=0; i < pNode->m_bizWndArray.GetCount(); i++)
	{
		CNodeWndDescription* pWndDescrip = pNode->m_bizWndArray[i]; 
		CString strCaption = pWndDescrip->m_strCaption;
		CString strLocation = pWndDescrip->m_strLocation;

		//创建插件窗口容器
		CHSPanel *pContainer = new CHSPanel();
		CCreateContext context;
		context.m_pNewViewClass = RUNTIME_CLASS(CWinnerView);
		if (!pContainer->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
			CRect(0,0,0,0), m_pWndTabs, AFX_IDW_PANE_FIRST, &context))
		{
			return;
		}

		//创建插件窗口 or 子布局
		IBizWindow* pBizWnd = NULL;
		HWND hBizWnd = NULL;
		{
			if(strLocation != _T("ChildLayout"))
			{
				ASSERT(pPluginMgr != NULL);
				pBizWnd =  pPluginMgr->CreateBizWnd(strLocation,pContainer->m_hWnd);
				if(pBizWnd !=NULL )
				{
					CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
					pBizWnd->Init(pMain,this,&pWndDescrip->m_param);
					hBizWnd = pBizWnd->GetHandle();
					pContainer->SetPluginHandle(hBizWnd);
				}
			}
			else
			{
				CWinnerView* pWinnerView = new CWinnerView();
				if(pWinnerView->CreateWinnerView(pContainer))
				{
					pContainer->SetPluginHandle(pWinnerView->m_hWnd);
					pWndDescrip->m_pChildLayoutWindow = (ILayoutWindow*)pWinnerView;
				}
			}

		}

		//附加biz window 
		m_pWndTabs->AddTab(pContainer,strCaption,-1,FALSE);	
		if(hBizWnd != NULL)
		{
			pWndDescrip->m_pBizWindow = pBizWnd;
		}
	}
}


void CLayoutView::AttachPluginWnd(CString strCaption,CString strLocation)
{
	if(m_pWndTabs == NULL)
		CreateTabCtrl();
	//创建biz window 容器窗口
	CHSPanel *pContainer = new CHSPanel();
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CWinnerView);
	if (!pContainer->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0,0,0,0), m_pWndTabs, AFX_IDW_PANE_FIRST, &context))
	{
		return;
	}
	
	//创建biz window
	CBizPluginMgr* pPluginMgr =  (CBizPluginMgr*)HsGetObjectManager()->GetObj(CBizPluginMgr::m_strClassName);
	ASSERT(pPluginMgr != NULL);
	IBizWindow* pBizWnd = pPluginMgr->CreateBizWnd(strLocation,pContainer->m_hWnd);
	if(pBizWnd != NULL)
	{
		HWND hwnd = pBizWnd->GetHandle();
		pContainer->SetPluginHandle(hwnd);
		//通知biz window 
		if(hwnd)
			::SendMessage(hwnd,WM_BIZWINDOW_NOTIFY,BWN_WINDOW_SHOW,TRUE);
	}
	//附加biz window 
	m_pWndTabs->AddTab(pContainer,strCaption,-1,FALSE);	

	CLayoutNode* pNode = m_pLayout->GetLayoutNode(this);
	if(pNode)
	{
		CNodeWndDescription *pInfo = new CNodeWndDescription();
		pInfo->m_strCaption = strCaption;
		pInfo->m_strLocation = strLocation;
		pInfo->m_pBizWindow = pBizWnd;
		pNode->m_bizWndArray.Add(pInfo);
	}
	
	CRect rc;
	GetClientRect(&rc);
	m_pWndTabs->MoveWindow (0, 0, rc.Width(),rc.Height());
	///m_pWndTabs->SetWindowPos (NULL, -1, -1, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}



int CLayoutView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

CHSSplitterWnd*  CLayoutView::GetSplitterWnd()
{	
	return m_pWndSplitter;
}

BOOL CLayoutView::CreateTabCtrl()
{
	if(m_pWndTabs != NULL)
		return TRUE;

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	m_pWndTabs = new CHSTabCtrl();
	m_pWndTabs->Create(CMFCTabCtrl::STYLE_3D_ONENOTE,rectDummy,this,1,CMFCTabCtrl::LOCATION_TOP,FALSE);
	if(m_bDesignMode == TRUE)
		m_pWndTabs->HideSingleTab(FALSE);
	else
		m_pWndTabs->HideSingleTab(TRUE);
	m_pWndTabs->EnableTabSwap(FALSE);
		

	return TRUE;
}

//CLayoutEntity
///////////////////////////////////////////////////////////////////////////////////////////

CLayoutEntity::CLayoutEntity(CString strIdentifier,CString strLayoutName,CLayoutView* pView)
{
	m_strIdentifier = m_strIdentifier;
	m_strLayoutName = strLayoutName;
	m_pView = pView;
	m_pLayout = new CLayoutContext(strLayoutName);	
}

CLayoutEntity::~CLayoutEntity()
{
	if(m_pLayout)
		delete m_pLayout;
}