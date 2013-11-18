// ExpTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExpTabCtrl.h"
#include "DlgFormulaMan.h"
#include "DlgCondition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWnd* GetMainWnd();

/////////////////////////////////////////////////////////////////////////////
// CExpTabCtrl

CExpTabCtrl::CExpTabCtrl()
{
}

CExpTabCtrl::~CExpTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CExpTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CExpTabCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TCN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnTcnSelchanging)
	ON_NOTIFY_REFLECT(TCN_FOCUSCHANGE, OnTcnFocusChange)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpTabCtrl message handlers

int CExpTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if( 1 )
	{
		InsertItem(0,CTreeCtrlFormula::m_strExpGroup);
		InsertItem(1,CTreeCtrlFormula::m_strExpAll  );
		InsertItem(2,CTreeCtrlFormula::m_strExpComm );
		InsertItem(3,CTreeCtrlFormula::m_strExpUser );	
	}

	m_wndTree.Create( WS_CHILD		  | 
		              WS_BORDER       | 
					  TVS_HASLINES    | 
					  TVS_LINESATROOT | 
					  WS_VISIBLE      |
					  TVS_HASBUTTONS  | 
					  TVS_SHOWSELALWAYS,// |
					  //TVS_SINGLEEXPAND, 
					  CRect(0,0,0,0),this,1001 );

	m_wndTree.SetType(CTreeCtrlFormula::HSBrowser);

	return 0;
}

void CExpTabCtrl::OnDestroy() 
{
	m_wndTree.Delete();

	CTabCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CExpTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CTabCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( (abs(cx) + abs(cy) > 0) && m_wndTree.m_hWnd )
	{
		CRect rect;
		this->GetClientRect(rect);
		m_wndTree.MoveWindow(rect);
	}
}

void CExpTabCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTabCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExpTabCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TC_KEYDOWN* pTCKeyDown = (TC_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CExpTabCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{

	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	//OnTcnSelchange(pNMHDR, pResult);

}

void CExpTabCtrl::PreSubclassWindow() 
{
	if( 1 )
	{
		InsertItem(0,CTreeCtrlFormula::m_strExpGroup);
		InsertItem(1,CTreeCtrlFormula::m_strExpAll  );
		InsertItem(2,CTreeCtrlFormula::m_strExpComm );
		InsertItem(3,CTreeCtrlFormula::m_strExpUser );	
	}

	m_wndTree.Create( WS_CHILD		  | 
		              WS_BORDER       | 
					  TVS_HASLINES    | 
					  TVS_LINESATROOT | 
					  WS_VISIBLE      |
					  TVS_HASBUTTONS  | 
					  TVS_SHOWSELALWAYS,// |
					  //TVS_SINGLEEXPAND, 
					  CRect(0,0,0,0),this,1001 );

	m_wndTree.SetType(CTreeCtrlFormula::HSBrowser);

	//m_wndTree.ModifyStyleEx(0,WS_EX_STATICEDGE,0);

	CRect rect,rcItem;
	GetItemRect(0,rcItem);
	GetClientRect(rect);
	rect.top = rcItem.bottom - 1;
	//m_wndTree.MoveWindow(CRect(0,0,0,0));
	m_wndTree.MoveWindow(rect);
	
	/*long lStyle = GetWindowLong(this->m_hWnd,GWL_STYLE);
	lStyle &= ~TCS_BUTTONS;
	SetWindowLong(this->m_hWnd,GWL_STYLE,lStyle);*/

	CTabCtrl::PreSubclassWindow();
}

CTreeCtrlFormula* CExpTabCtrl::GetTree()
{
	if( ::IsWindow(m_wndTree.m_hWnd))
		return &m_wndTree;
	return NULL;
}


void CExpTabCtrl::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{

	*pResult = 0;

	// TODO: 在此添加控件通知处理程序代码
	int nCopy  = 0;//CTreeCtrlFormula::CopyDelete;
	int nIndex = GetCurSel( );
	switch(nIndex)
	{
	case 0:
		break;
	case 1:
		nCopy |= CTreeCtrlFormula::CopyNotGroup;
		break;
	case 2:
		nCopy |= CTreeCtrlFormula::CopyNotGroup | HS_EXP_DEFAULT_TECH;
		break;
	case 3:
		nCopy |= CTreeCtrlFormula::CopyNotGroup | HS_EXP_USEREDIT;
		break;
	default:
		return;
	}

	this->InitTree(nCopy);
	
}

void CExpTabCtrl::OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CExpTabCtrl::OnTcnFocusChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Internet Explorer 5 或更高版本。
	// 符号 _WIN32_IE 必须是 >= 0x0500。
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CExpTabCtrl::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CExpTabCtrl::InitTree(int nCopy/* = CTreeCtrlFormula::CopyDelete*/)
{
	CTreeCtrlFormula* pCurTree = GetTree();
	if( pCurTree )
	{
		// 得到CDlgFormulaMan窗口
		CWnd* pWnd = GetMainWnd();

		/*CWnd* pWnd = this->GetParent();
		while( pWnd != NULL )
		{
			if( pWnd != NULL && 
				pWnd->IsKindOf(RUNTIME_CLASS(CDlgFormulaMan)) )
				break;

			pWnd = pWnd->GetParent();
		}
		*/
		
		if( pWnd == NULL )
			return;

		// 得到包含的类型
		long pJiben;
		long pUnion;
		long lExpressType = pCurTree->GetHasExpressType(&pJiben,&pUnion);

		// 重新设置
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)pWnd;

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		pCurTree->DeleteTree();

		// 基本面数据
		if( pJiben == JiBenMian )
		{
			HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strJiben,		
				CExpression::HS_Exp_Condition, CExpression::HS_Exp_Condition);
			CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)JiBenMian); 
			pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);

			//pCurTree->InitCWData(GB_GUBEN_NAME);
			//pCurTree->InitCWData(Cj_GUBEN_NAME);
		}

		// 组合条件数据
		if( pUnion == Union )
		{
			HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strUnionCondition,
				CExpression::HS_Exp_Root, CExpression::HS_Exp_Root);
			CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)Union); 
			pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);

			CDlgCondition::SaveLoad(CArchive::load,this);
		}

		CTreeCtrlFormula* pTree;
		WORD nType;
		for( int i = 0; i < 5; i++ ) //20090302 YJT 修改
		{
			switch(i)
			{
			case 0:
				nType = lExpressType & Tech;
				break;
			case 1:
				nType = lExpressType & Condition;
				break;
			case 2:
				nType = lExpressType & Exchange;
				break;
			case 3:
				nType = lExpressType & MoreKLine;
				break;
			case 4: //20090302 YJT 增加 Strategy
				nType = lExpressType & Strategy;
				break;
			}
			pTree = pDlg->GetCurTree( nType );

			if( pTree )
			{
				pCurTree->CopyTree(pTree,nCopy);
			}
		}

		HTREEITEM hItem = pCurTree->GetRootItem();
		if(hItem != NULL)
		{
			pCurTree->Expand(hItem,TVE_EXPAND);
			pCurTree->Select(hItem,TVGN_CARET);
		}
	}
}
