// WinnerView.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "WinnerView.h"
#include "ChildFrm.h"
#include "MessagePromptDialog.h"
#include "DockableView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CWinnerView, CView)

BEGIN_MESSAGE_MAP(CWinnerView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

CWinnerView::CWinnerView()
{
	m_pActiveLayoutEntity = NULL;
}

CWinnerView::~CWinnerView()
{
	POSITION pos;
	CString key;
	CLayoutEntity* pTempViewInfo;
	for( pos = m_layoutEntityMap.GetStartPosition(); pos != NULL; )
	{
		m_layoutEntityMap.GetNextAssoc( pos, key, (CObject*&)pTempViewInfo );
		if(pTempViewInfo)
			delete pTempViewInfo;
	}
	m_layoutEntityMap.RemoveAll();
}

BOOL CWinnerView::CreateWinnerView(CWnd* pParent)
{
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CWinnerView);

	if (!this->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0,0,0,0), pParent, AFX_IDW_PANE_FIRST, &context))
	{
		TRACE(traceAppMsg, 0, "Warning: could not create CWinnerView for frame.\n");
		return FALSE;
	}
	return TRUE;
}

CString CWinnerView::GenerateLayoutEntityIdentifier(CString strLayoutName,CString  strOwner)
{
	CString strIdentifier;
	strIdentifier.Format(_T("[%s]%s"),strOwner,strLayoutName);
	return strIdentifier;
}

BOOL CWinnerView::OpenLayout(CString strTarget,CString strLayoutName,CString strOwner,IHSParam *pParam /*== NULL*/)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	CLogTimeSapn lts(_T("CWinnerView::OpenLayout: strTarget=") + strTarget
		+ _T(" strLayoutName=") + strLayoutName);

	if(strTarget == CURRENT_LAYOUT_TARGET)
	{
		CString strRealLayoutName = strLayoutName;
		CLayoutView* pView = CreateLayoutView(strLayoutName,strOwner);
		SetActiveView(pView);//设置活动视图
		AdjustLayout();
// 		pView->ShowWindow(SW_HIDE);
// 		pView->ShowWindow(SW_SHOW);
		CLayoutEntity* pLayoutEntity = GetLayoutEntity(strLayoutName,strOwner);
		if( pLayoutEntity != NULL)
		{
			pLayoutEntity->GetLayoutContext()->NotifyToBizWindows(WM_BIZWINDOW_NOTIFY,BWN_WINDOW_SHOW,0,TRUE);
			if(pParam)
			{
				pLayoutEntity->GetLayoutContext()->NotifyFocusWindow(TRUE);
				pLayoutEntity->GetLayoutContext()->NotifyToBizWindows(WM_BIZWINDOW_NOTIFY,BWN_PARAM_CHANGED,LPARAM(pParam),FALSE);
			}
			else
				pLayoutEntity->GetLayoutContext()->NotifyFocusWindow(TRUE);
		}
	}
	else //在活动布局中打开子布局
	{
		if(m_pActiveLayoutEntity == NULL)
			return FALSE;
		ILayoutWindow *pLayoutWnd =	m_pActiveLayoutEntity->GetLayoutContext()->GetLayoutWindow(strTarget);
		if(pLayoutWnd != NULL)
			pLayoutWnd->OpenLayout(CURRENT_LAYOUT_TARGET,strLayoutName,strOwner,pParam);
		else
		{
			CString strError;
			strError.Format(_T("打开子布局[%s]失败.\ntarget=%s"),strLayoutName,strTarget);
			CMessagePromptDialog dlg;
			dlg.ShowMsg(CMessagePromptDialog::mtError,strError);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CWinnerView::ChangeSize(int cx,int cy)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());
	CWnd* pWnd = GetParent();
	if ( !pWnd->IsKindOf(RUNTIME_CLASS(CDockableView)) )
		return FALSE;
	CDockableView* pDockableView = (CDockableView*)pWnd;
	pWnd->MoveWindow(0,0,cx,cy);
	
	if(AfxGetMainWnd())
	{
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->RecalcLayout();
	}
	pDockableView->ShowPane(FALSE,FALSE,FALSE);
	pDockableView->ShowPane(TRUE,FALSE,FALSE);

	return TRUE;
}

BOOL CWinnerView::RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	return HsGetLinkageMsgManager()->RegisterLinkageMsg(nMsg,pBizWnd);
}

BOOL CWinnerView::UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	return HsGetLinkageMsgManager()->UnregisterLinkageMsg(nMsg,pBizWnd);
}

int CWinnerView::DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	return HsGetLinkageMsgManager()->DispatchLinkageMsg(nMsg,wParam,lParam,bPost);
}

BOOL CWinnerView::RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	return HsGetHotKeyManager()->RegisterHotKey(id,fsModifiers,vk,pBizWnd);
}

BOOL CWinnerView::UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	return HsGetHotKeyManager()->UnregisterHotKey(fsModifiers,vk,pBizWnd);
}

void CWinnerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

void CWinnerView::PostNcDestroy()
{
	delete this;
}

BOOL CWinnerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

CLayoutView* CWinnerView::CreateLayoutView(CString strLayoutName,CString strOwner)
{
	CLayoutView* pView = NULL;
	CString strIdentifier = GenerateLayoutEntityIdentifier(strLayoutName,strOwner);
	CLayoutEntity* pLayoutEntity = GetLayoutEntity(strLayoutName,strOwner);
	if( pLayoutEntity != NULL)
	{
		pView = pLayoutEntity->GetView();
		return pView; 
	}

	pView = new CLayoutView(); 

	if(strLayoutName.IsEmpty())
		pView->EnableDesignMode(TRUE);

	CRect rect;
	GetClientRect(&rect);
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN;
	if (!pView->Create(NULL,strIdentifier,dwViewStyle, rect,this, 1))
	{
		TRACE0("create CDockableView Failed\n");
		delete pView;
		return NULL; 
	}

	pLayoutEntity = new CLayoutEntity(strIdentifier,strLayoutName,pView);
	m_layoutEntityMap.SetAt(strIdentifier,(CObject*)pLayoutEntity);

	//设置布局视图相关的布局描述
	CLayoutContext* pContext = pLayoutEntity->GetLayoutContext();
	pView->SetLayoutContext(pContext);

	//加载布局描述信息并分割视图
	ASSERT(pContext != NULL);
	pContext->LoadLayoutInfo(strLayoutName);
	pContext->SetRootView(pView);
	pContext->SetWinnerView(this);
	pContext->SplitWindow(pView,rect);
	return pView;
}

CLayoutEntity* CWinnerView::GetLayoutEntity(CString strLayoutName,CString strOWner)
{
	CString strIdentifier = GenerateLayoutEntityIdentifier(strLayoutName,strOWner);
	CLayoutEntity* pLayoutEntity = NULL; 
	if( m_layoutEntityMap.Lookup(strIdentifier,( CObject*& )pLayoutEntity) == 0 )
		return NULL;
	else 
		return pLayoutEntity;
}

void CWinnerView::SetActiveView(CLayoutView* pView)
{
	POSITION pos;
	CString key;
	CLayoutEntity* pLayoutEntity;

	for( pos = m_layoutEntityMap.GetStartPosition(); pos != NULL; )
	{
		m_layoutEntityMap.GetNextAssoc( pos, key, (CObject*&)pLayoutEntity );
		if(pLayoutEntity->GetView() == pView)
		{
			m_pActiveLayoutEntity = pLayoutEntity;
			if( !pLayoutEntity->GetView()->IsWindowVisible())
				pLayoutEntity->GetView()->ShowWindow(SW_SHOW);
		}
		else
		{
			if( pLayoutEntity->GetView()->IsWindowVisible())
				pLayoutEntity->GetView()->ShowWindow(SW_HIDE);
		}
	}
}

CString CWinnerView::GetCurrentLayoutName()
{
	if(m_pActiveLayoutEntity)
	   return m_pActiveLayoutEntity->GetLayoutName();
	else
		return _T("");
}

int CWinnerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CWinnerView::OnSize(UINT nType, int cx, int cy)
{
	AdjustLayout();
}

void CWinnerView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || m_pActiveLayoutEntity == NULL) 
		return;

	if(m_pActiveLayoutEntity->GetView() == NULL)
		return;

	if(AfxGetMainWnd()->IsIconic())
		return;

	CRect rectClient;
	GetClientRect(rectClient);
	m_pActiveLayoutEntity->GetView()->SetPostion(rectClient);
}

int CWinnerView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CWnd *pParent = GetParent();
	if(pParent != NULL && pParent->IsKindOf(RUNTIME_CLASS(CChildFrame)))
		return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
	else
		return TRUE;
}