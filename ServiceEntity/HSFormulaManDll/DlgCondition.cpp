// HSCondition.cpp : implementation file
//

#include "stdafx.h"
#include "DlgCondition.h"

#include "DlgFormulaMan.h"
#include "DlgFormulaEdit.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"
#include "FormulaCalc.h"

#include "RegMsg.h"
#include "DlgUnionConSelStock.h"
#include "DlgSetStockAndTime.h"
#include ".\dlgcondition.h"
#include "Express.h"
#include "GeneralHelper.h"
#include "PubStruct.h"
#include "ExpPublic.h"
#include "hscoldefine.h"
#include "ConfigStruct.h"
#include "hsothercoldefine.h"

UINT g_nGetHSMain    = RegisterWindowMessage(HS_GET_HSMAIN);
UINT g_nNotifyMessage = ::RegisterWindowMessage(HS_COMM_DATA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCondition dialog

extern HWND				g_hEditUpdateParentWnd;
extern CMapStringToPtr* g_pTableHeadData;
extern HWND g_hParentWnd;
extern CSystemFun* g_pSystemFun; 
extern BOOL g_bShowCmd;
extern CDlgFormulaMan* g_pFormulaMan; 
IMPLEMENT_DYNAMIC(CDlgCondition,CHSBaseDialog)
CDlgCondition::CDlgCondition(CWnd* pParent /*=NULL*/,
							 HSOpenExpressUseData* pData /*= NULL*/,
							 char cType /*= CDlgCondition::Condition*/,
							 long* pThis /*= NULL*/,
							 int nID /*= IDD_CONDITION_STOCK*/)
							 : CHSBaseDialog(nID, pParent)
{
	m_cType = cType;
	m_nID	= nID;
	m_hParentWnd = pParent->m_hWnd;
	m_lRefWnd = 0;
	m_hMsgWnd = g_hParentWnd;

	if( pData->m_strName != NULL )
	{
		m_strExpression = *pData->m_strName;
	}
	// gbq add end;

	m_nCurPos = 0;
	m_nStopAutoRun = 0;
}
CDlgCondition::~CDlgCondition()
{

}


INT_PTR CDlgCondition::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	// can be constructed with a resource template or InitModalIndirect
	ASSERT(m_lpszTemplateName != NULL || m_hDialogTemplate != NULL ||
		m_lpDialogTemplate != NULL);

	// load resource as necessary
	LPCDLGTEMPLATE lpDialogTemplate = m_lpDialogTemplate;
	HGLOBAL hDialogTemplate = m_hDialogTemplate;
	HINSTANCE hInst = AfxGetResourceHandle();
	if (m_lpszTemplateName != NULL)
	{
		hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);
		HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
		hDialogTemplate = LoadResource(hInst, hResource);
	}
	if (hDialogTemplate != NULL)
		lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

	// return -1 in case of failure to load the dialog template resource
	if (lpDialogTemplate == NULL)
		return -1;

	// disable parent (before creating dialog)
	HWND hWndParent = PreModal();
	AfxUnhookWindowCreate();
	BOOL bEnableParent = FALSE;
	if (hWndParent && hWndParent != ::GetDesktopWindow() && ::IsWindowEnabled(hWndParent))
	{
		::EnableWindow(hWndParent, FALSE);
		bEnableParent = TRUE;
	}

	TRY
	{
		// create modeless dialog
		AfxHookWindowCreate(this);
		if (CreateDlgIndirect(lpDialogTemplate,
			CWnd::FromHandle(hWndParent), hInst))
		{
			if (m_nFlags & WF_CONTINUEMODAL)
			{
				// enter modal loop
				DWORD dwFlags = MLF_SHOWONIDLE;
				if (GetStyle() & DS_NOIDLEMSG)
					dwFlags |= MLF_NOIDLEMSG;
				VERIFY(RunModalLoop(dwFlags) == m_nModalResult);
			}

			// hide the window before enabling the parent, etc.
			if (m_hWnd != NULL)
				SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
				SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
	CATCH_ALL(e)
	{
		DELETE_EXCEPTION(e);
		m_nModalResult = -1;
	}
	END_CATCH_ALL

		if (bEnableParent)
			::EnableWindow(hWndParent, TRUE);
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
		::SetActiveWindow(hWndParent);

	// destroy modal window
	DestroyWindow();
	PostModal();

	// unlock/free resources as necessary
	if (m_lpszTemplateName != NULL || m_hDialogTemplate != NULL)
		UnlockResource(hDialogTemplate);
	if (m_lpszTemplateName != NULL)
		FreeResource(hDialogTemplate);

	return m_nModalResult;
}

int CDlgCondition::RunModalLoop(DWORD dwFlags)
{
#ifdef VC_60
	return CHSBaseDialog::RunModalLoop(dwFlags);
#else

	ASSERT(::IsWindow(m_hWnd)); // window must be created
	ASSERT(!(m_nFlags & WF_MODALLOOP)); // window must not already be in modal state

	// for tracking the idle time state
	BOOL bIdle = TRUE;
	LONG lIdleCount = 0;
	BOOL bShowIdle = (dwFlags & MLF_SHOWONIDLE) && !(GetStyle() & WS_VISIBLE);
	HWND hWndParent = ::GetParent(m_hWnd);
	m_nFlags |= (WF_MODALLOOP|WF_CONTINUEMODAL);
	MSG *pMsg = AfxGetCurrentMessage();

	try
	{

	// acquire and dispatch messages until the modal state is done
	for (;;)
	{
		ASSERT(ContinueModal());

		// phase1: check to see if we can do idle work
		while (bIdle &&
			!::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE))
		{
			ASSERT(ContinueModal());

			// show the dialog when the message queue goes idle
			if (bShowIdle)
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			// call OnIdle while in bIdle state
			if (!(dwFlags & MLF_NOIDLEMSG) && hWndParent != NULL && lIdleCount == 0)
			{
				// send WM_ENTERIDLE to the parent
				::SendMessage(hWndParent, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)m_hWnd);
			}
			if ((dwFlags & MLF_NOKICKIDLE) ||
				!SendMessage(WM_KICKIDLE, MSGF_DIALOGBOX, lIdleCount++))
			{
				// stop idle processing next time
				bIdle = FALSE;
			}
		}

		// phase2: pump messages while available
		do
		{
			//ASSERT(ContinueModal());
			
			// pump message, but quit on WM_QUIT
			if (!AfxPumpMessage())
			{
				AfxPostQuitMessage(0);
				return -1;
			}

			// show the window when certain special messages rec'd
			if (bShowIdle &&
				(pMsg->message == 0x118 || pMsg->message == WM_SYSKEYDOWN))
			{
				ShowWindow(SW_SHOWNORMAL);
				UpdateWindow();
				bShowIdle = FALSE;
			}

			if (!ContinueModal())
				goto ExitModal;

			// reset "no idle" state after pumping "normal" message
			if (AfxIsIdleMessage(pMsg))
			{
				bIdle = TRUE;
				lIdleCount = 0;
			}

		} 
		while (::PeekMessage(pMsg, NULL, NULL, NULL, PM_NOREMOVE));
	}
	
	}
	catch(...)
	{
	}

ExitModal:
	m_nFlags &= ~(WF_MODALLOOP|WF_CONTINUEMODAL);

#endif
	return m_nModalResult;
}
void CDlgCondition::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCondition)
	DDX_Control(pDX, IDC_BOTTOMPOS, m_wndBottomPos);
	DDX_Control(pDX, IDC_CONDITIONPOS, m_wndConditionPos);
// 	DDX_Control(pDX, IDD_NEWUNION, m_wndNewUnion);
// 	DDX_Control(pDX, IDD_UPDATEUNION, m_wndUpdateUnion);
// 	DDX_Control(pDX, IDC_DELETEUNION, m_wndDeleteUnion);
// 	DDX_Control(pDX, IDD_USERNOTES, m_wndExpNotes);
	DDX_Control(pDX, IDC_STATIC_CON, m_wndStaticCon);
	DDX_Control(pDX, IDC_MIDDLE, m_wndMiddle);
	DDX_Control(pDX, IDD_PERIOD_STATIC, m_wndPeriodStc);
	DDX_Control(pDX, IDD_PERIOD, m_wndPeriod);
	DDX_Control(pDX, IDD_NOTES, m_wndNotes);
	DDX_Control(pDX, IDD_DATA2, m_wndData2);
	DDX_Control(pDX, IDD_DATA1, m_wndData1);
	DDX_Control(pDX, IDD_CONNOTES, m_wndConNotes);
	DDX_Control(pDX, IDD_CON, m_wndCon);
	DDX_Control(pDX, IDD_COMPARE, m_wndCompare);
	DDX_Control(pDX, IDD_AND_OR, m_wndAndOr);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	DDX_Control(pDX, IDC_EDITPARAM, m_wndParam);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_INDICATOR, m_cProgress);
	DDX_Control(pDX, IDC_STATICPROMPT, m_wndPromptText);
	

}


BEGIN_MESSAGE_MAP(CDlgCondition, CHSBaseDialog)
	//{{AFX_MSG_MAP(CDlgCondition)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDD_ADVANCE, OnAdvance)
	ON_BN_CLICKED(IDD_USERNOTES, OnUsernotes)
	ON_BN_CLICKED(IDD_PRESUCC, OnPresucc)

	ON_BN_CLICKED(IDD_TOBLOCK, OnToblock)
	ON_BN_CLICKED(IDOK2, OnAutoSelect)

	ON_CBN_SELCHANGE(IDD_COMPARE, OnSelchangeCompare)
	ON_CBN_SELCHANGE(IDD_CON, OnSelchangeCon)
	ON_BN_CLICKED(IDD_NEWUNION, OnNewunion)
	ON_BN_CLICKED(IDD_UPDATEUNION, OnUpdateunion)
	ON_BN_CLICKED(IDC_BUTHIDELEFT, OnButhideleft)
	ON_BN_CLICKED(IDC_SAVEASBLOCK, OnSaveasblock)
	ON_BN_CLICKED(IDC_BUTEXPRESS, OnButexpress)
	ON_BN_CLICKED(IDC_BUTPARAM, OnButparam)
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDD_GETNOWDATA, OnUpDateNowData)	

	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	ON_REGISTERED_MESSAGE(g_nNotifyMessage, OnCommNotify)

	ON_BN_CLICKED(IDC_BUTFIND, OnBnClickedButfind)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_DELETEUNION, OnBnClickedDeleteunion)
	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCondition message handlers

BOOL CDlgCondition::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();
	m_wndPeriod.ResetContent();
	AddPeriodNameToWnd(&m_wndPeriod);
	this->m_pParentWnd = CWnd::FromHandle((HWND)m_lRefWnd);	
	CWinApp* pApp = AfxGetApp();

	m_nDelayTimer = pApp->GetProfileInt(CDlgSetStockAndTime_Section,"选股之间间隔时间",1000*60);
	m_nDelayTaskTimer = pApp->GetProfileInt(CDlgSetStockAndTime_Section,"下次自动选股间隔时间",1000*60*5);
	m_strSelectSavePath = pApp->GetProfileString(CDlgSetStockAndTime_Section,"选股结果路径",CGeneralHelper::GetSpecifyPath(CGeneralHelper::BLOCK_PATH)); 

	// 时间
	CString str = pApp->GetProfileString(CDlgSetStockAndTime_Section,CDlgSetStockAndTime_Time,"");

	CStringArray ayRet;
	int nCount = CGeneralHelper::AnalyseStr(str,";",ayRet);
	if( nCount >= 2 )
	{
		m_curSelStockCondition.m_lDateCount = atol(ayRet.GetAt(0));
		m_curSelStockCondition.m_lEndDate   = strtoul(ayRet.GetAt(1),'\0',10);//--2010.11.12 litao修正2012时间问题
	}

	// 股票范围
	str = pApp->GetProfileString(CDlgSetStockAndTime_Section,CDlgSetStockAndTime_Stock,"");

//	CStringArray ayRet;
	nCount = CGeneralHelper::AnalyseStr(str,";",ayRet);
	BOOL bAdd = FALSE;
	if ( StockArea::ayStock.GetCount()<=0 )
	{
		bAdd = TRUE;
	}
	for( int i = 0; i < nCount; i++ )
	{
		CStringArray aySub;
		if( CGeneralHelper::AnalyseStr(ayRet.GetAt(i),",",aySub) >= 2 )
		{		
			StockArea* pCheckData = new StockArea;
			pCheckData->lData   = atol(aySub.GetAt(0));
			pCheckData->strText = aySub.GetAt(1);
			m_ayCheck.Add(pCheckData);
			if ( bAdd )
				StockArea::AddStock(pCheckData->lData);				
		}
	}


	if( m_ayCheck.GetSize() <= 0 )
	{
		StockArea* pCheckData = new StockArea;
		pCheckData->lData = SH_Bourse | KIND_STOCKA;
		pCheckData->strText = _T("上证Ａ股");
		m_ayCheck.Add(pCheckData);
		if ( bAdd )
			StockArea::AddStock(pCheckData->lData);
		pCheckData = new StockArea;
		pCheckData->lData = SZ_Bourse | KIND_STOCKA;
		pCheckData->strText = _T("深证Ａ股");
		if ( bAdd )
			m_ayCheck.Add(pCheckData);
		StockArea::AddStock(pCheckData->lData);
		pCheckData = new StockArea;
		pCheckData->lData = SZ_Bourse | KIND_SMALLSTOCK;
		pCheckData->strText = _T("中小盘股");
		m_ayCheck.Add(pCheckData);
		if ( bAdd )
			StockArea::AddStock(pCheckData->lData);
		pCheckData = new StockArea;
		pCheckData->lData = SZ_Bourse | KIND_STOCKGE;
		pCheckData->strText = _T("创业板");
		m_ayCheck.Add(pCheckData);
		if ( bAdd )
			StockArea::AddStock(pCheckData->lData);
		//--------------------------------------------------
	}

	// 选项
	m_curSelStockCondition.m_dStyle = pApp->GetProfileInt(CDlgSetStockAndTime_Section,
		CDlgSetStockAndTime_Option,SelectStock_InK_Disp_Mark);

	CRect rc;
	m_wndConditionPos.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndParam.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER);

	SetType(m_cType);

	m_wndPromptText.SetWindowText(_T("选股在盘后下载的日线中进行，请确保数据完整!"));


	if( !m_strExpression.IsEmpty() )
		ShowExpression( m_strExpression );

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCondition::Init()
{
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)(CWnd::FromHandle(m_hParentWnd));	

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		if( IsType(Sort) )
		{
			CTreeCtrlFormula* pTree = pDlg->GetCurTree( (WORD)Tech );
			pCurTree->CopyTree(pTree,CTreeCtrlFormula::CopyDelete);

			CWnd* pWnd = this->GetDlgItem(IDD_TOBLOCK);
			if( pWnd )
			{
				pWnd->ShowWindow(SW_HIDE);
			}

		}
		else
		{
			if( m_nID == IDD_CONDITION_STOCK_Auto )
			{
				m_wndTab.DeleteItem(1);
				m_wndTab.DeleteItem(1);
				m_wndTab.DeleteItem(1);
			}
			else
			{
				pDlg->CopyTree(pCurTree);
			}

			// 组合
			HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strUnionCondition,
				CExpression::HS_Exp_Root, CExpression::HS_Exp_Root);
			CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)Union); 
			pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);

			if( m_nID == IDD_CONDITION_STOCK_Auto )
			{
			}
			else
			{
				// 财务
				hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strJiben,
					CExpression::HS_Exp_Condition, CExpression::HS_Exp_Condition);
				pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)JiBenMian); 
				pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);
			}

			SaveLoad(CArchive::load,&m_wndTab,"",0,this);
		}
	}
}

void CDlgCondition::OnSize(UINT nType, int cx, int cy) 
{
	CHSBaseDialog::OnSize(nType, cx, cy);

	//if( abs(cx) + abs(cy) <= 0 )
	//	return;
	//if( !::IsWindow(m_wndTab.m_hWnd) )
	//	return;

	//SetSize();
}

void CDlgCondition::SetSize()
{
	//CWnd* pWnd;
	//CRect rect,rc,rc1;
	//this->GetClientRect(rect);

	//char cWidth = 5;

	//pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	//if(pWnd)
	//{
	//	CString strText;
	//	pWnd->GetWindowText(strText);
	//	if( !strText.CompareNoCase(">>") )
	//	{
	//		pWnd->GetWindowRect(rc);
	//		ScreenToClient(rc);
	//		pWnd->SetWindowPos(NULL,rect.left + cWidth,rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//		pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
	//		if(pWnd)
	//		{
	//			pWnd->GetWindowRect(rc1);
	//			ScreenToClient(rc1);
	//			pWnd->SetWindowPos(NULL,rect.right - rc1.Width() - cWidth,
	//				rect.bottom - cWidth - rc1.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	//		}
	//		int nXOff = rc.Width();
	//		m_wndSelStockResult.GetWindowRect(rc);
	//		ScreenToClient(rc);
	//		m_wndSelStockResult.SetWindowPos(NULL,rect.left + nXOff + cWidth,rc.top,
	//			rect.Width() - cWidth * 2 - rect.left - nXOff,
	//			rect.Height() - rc.top - rc1.Height() - cWidth*2 ,SWP_NOZORDER);
	//		pWnd = m_wndSelStockResult.GetDlgItem(1001);
	//		if( pWnd )
	//		{
	//			m_wndSelStockResult.GetClientRect(rc);
	//			pWnd->MoveWindow(rc);
	//		}
	//		return;	
	//	}
	//}

	//// bottom
	//CRect rcBottom;
	//m_wndBottomPos.GetWindowRect(rcBottom);
	//ScreenToClient(rcBottom);

	//m_wndTab.GetClientRect(rc);
	//m_wndTab.GetItemRect(0,rc1);	
	//int nTabHeight = rect.Height() - rcBottom.Height() - rc1.Height() - cWidth;
	//m_wndTab.SetWindowPos(NULL,0,0,rc.Width(),nTabHeight,SWP_NOMOVE|SWP_NOZORDER);

	//m_wndTab.GetWindowRect(rc);	
	//ScreenToClient(rc);
	//m_wndBottomPos.SetWindowPos(NULL,rc.left,rc.bottom + cWidth,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndBottomPos.GetWindowRect(rc);
	//ScreenToClient(rc);
	//int nHeightOff = rc.top - rcBottom.top;

	//m_wndNotes.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndNotes.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndConditionPos.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndConditionPos.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndConditionPos.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndParam.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER);

	//m_wndPeriod.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndPeriod.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndPeriodStc.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndPeriodStc.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndConNotes.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndConNotes.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndCon.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndCon.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndStaticCon.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndStaticCon.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndCompare.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndCompare.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndData1.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndData1.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndAndOr.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndAndOr.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndData2.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndData2.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//m_wndMiddle.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndMiddle.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	////
	//pWnd = this->GetDlgItem(IDD_ADVANCE);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDD_PRESUCC);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDD_TOBLOCK);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDOK);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}

	////
	//pWnd = this->GetDlgItem(IDD_UPDATEUNION);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDD_NEWUNION);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDD_USERNOTES);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDC_BUTPARAM);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}
	//pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//}

	//// right
	//pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc);
	//	ScreenToClient(rc);
	//}

	//pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
	//if(pWnd)
	//{
	//	pWnd->GetWindowRect(rc1);
	//	ScreenToClient(rc1);
	//	pWnd->SetWindowPos(NULL,rect.right - rc1.Width() - cWidth,rect.bottom - cWidth - rc1.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	//}

	//nHeightOff = rect.Width() - rc.right - cWidth;
	//m_wndSelStockResult.GetWindowRect(rc);
	//ScreenToClient(rc);
	//m_wndSelStockResult.SetWindowPos(NULL,0,0,nHeightOff,rect.Height() - rc.top - rc1.Height() - cWidth*2,
	//	SWP_NOMOVE|SWP_NOZORDER);

	//pWnd = m_wndSelStockResult.GetDlgItem(1001);
	//if( pWnd )
	//{
	//	m_wndSelStockResult.GetClientRect(rc);
	//	pWnd->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),0);
	//}	

	//this->Invalidate();
}

void CDlgCondition::OnAdvance() 
{
	CDlgSetStockAndTime dlg(this,&m_ayCheck,&m_curSelStockCondition);
	if( dlg.DoModal() == IDOK )
	{
	}	
}

void CDlgCondition::OnCancel() 
{
	CHSBaseDialog::OnCancel();
}

void CDlgCondition::OnUsernotes() 
{
	// TODO: Add your control notification handler code here
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)(CWnd::FromHandle(m_hParentWnd));
	if( pParentDlg == NULL )
		return;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		return;
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	if( pExpression == NULL )
		return;

	CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
	if( pDlgNotes != NULL )
	{
		pDlgNotes->m_pExpression = pExpression;
		pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		pDlgNotes->ShowWindow(SW_SHOW);
	}
}

void CDlgCondition::OnPresucc() 
{
	// TODO: Add your control notification handler code here

}

void CDlgCondition::OnOK() 
{	
	//m_strBlock.Empty();

	Select();
}

int CDlgCondition::Select() 
{
	CWnd* pOKBut = this->GetDlgItem(IDOK);
	if( pOKBut != NULL )
	{
		CString strText;
		pOKBut->GetWindowText(strText);
		if( !strText.CompareNoCase(CDlgCondition_But_Stop) )
		{
			if( MessageBox(CDlgCondition_But_Request,CDlgCondition_MessageBox_Caption,
				MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES )
			{
				pOKBut->SetWindowText(CDlgCondition_But_Run);
				/*::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,9,0);*/
				CFormulaCalc::Stop();
			}
			return 0;
		}		
	}
	
	m_AyLastStock.RemoveAll();

	if(m_sSingleSel.m_nType == hx_DefMaxValues)
	{
		MessageBox(CDlgCondition_Not_Select_Error,CDlgCondition_MessageBox_Caption);
		return 0;
	}

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree == NULL )
	{
		return 0;
	}

	HTREEITEM hItem	= pCurTree->GetSelectedItem();
	if( hItem == NULL )
	{
		MessageBox(CDlgCondition_Not_Select_Error,CDlgCondition_MessageBox_Caption);
		return 0;
	}

	m_curSelStockCondition.m_dStyle &= ~(SelectStock_Single | SelectStock_Union | SelectStock_Sort);

	HTREEITEM hRootItem   = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
	HTREEITEM hParentItem = pCurTree->GetParentItem(hItem);
	if( hParentItem == hRootItem && hParentItem != NULL ) // 组合条件
	{
		UnionSelCondition* pHSUnion = (UnionSelCondition*)pCurTree->GetItemData(hItem);
		m_pHSUnion.Copy(pHSUnion);
		m_curSelStockCondition.m_dStyle |= SelectStock_Union;
	}
	else if( m_sSingleSel.m_nType == Union )
	{
		MessageBox(CDlgCondition_Not_Select_Error,CDlgCondition_MessageBox_Caption);
		return 0;
	}
	else // 单个条件
	{
		// 周期
		int nIndex = m_wndPeriod.GetCurSel();
		if( nIndex != -1 )
		{
			CString strPeriod;
			m_wndPeriod.GetLBText(nIndex,strPeriod);			
			m_sSingleSel.m_nPeriod = GetPeriodNumByName(strPeriod);    // 周期
		}
		//m_sSingleSel.m_nPeriod;    

		// 技术指标指:哪一个指标线;交易系统指:哪种信号;基本面指:财务指标ID
		m_wndCon.GetWindowText(m_sSingleSel.m_strCondition); 

		//
		nIndex = m_wndCon.GetCurSel();
		if( nIndex != -1 )
		{
			DWORD dKey = m_wndCon.GetItemData(nIndex);
			if( dKey != 0 )
			{
				m_sSingleSel.m_strCondition.Format("%i",dKey);
			}
		}

		if(!m_sSingleSel.m_strCondition.Compare(CTreeCtrlFormula::g_strFirstTechLine))
		{
			m_sSingleSel.m_strCondition = "";
		}
		if( !(m_sSingleSel.m_nType == Condition || // 条件选股 
			  m_sSingleSel.m_nType == Exchange  || // 交易系统
			  m_sSingleSel.m_nType == MoreKLine || // 五彩K线
			  m_sSingleSel.m_nType == Strategy )) // 策略交易 //20090302 YJT 增加
		{
			m_sSingleSel.m_SubData.m_cType = m_wndCompare.GetCurSel();
			m_wndData1.GetWindowText(m_sSingleSel.m_SubData.m_dData1);
			m_wndData2.GetWindowText(m_sSingleSel.m_SubData.m_dData2);
		}
		else
		{
			m_sSingleSel.m_strCondition.Empty();
			m_sSingleSel.m_SubData.m_dData1.Empty();
			m_sSingleSel.m_SubData.m_dData2.Empty();
		}

		if( IsType(Sort) ) // 指标排序
		{
			m_sSingleSel.m_SubData.m_dData1.Empty();
			m_sSingleSel.m_SubData.m_dData2.Empty();
			m_curSelStockCondition.m_dStyle |= SelectStock_Sort;
		}
		else
		{
			m_curSelStockCondition.m_dStyle |= SelectStock_Single;
		}
	}

	m_curSelStockCondition.m_ayCheck	= &m_ayCheck;			// 股票范围
	m_curSelStockCondition.m_strBlock	= NULL;//&m_strBlock;          // 板块名

	m_curSelStockCondition.m_sSingleSel = &m_sSingleSel;		// 单个选股条件
	m_curSelStockCondition.m_pHSUnion  = &m_pHSUnion;			// 组合选股条件

	if( m_curSelStockCondition.m_lEndDate == 0 )
	{
		CString str;
		CTime timeDest = CTime::GetCurrentTime();
		str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
		m_curSelStockCondition.m_lEndDate = atol(str);   
	}

	m_curSelStockCondition.m_pData = NULL;

	m_nSelStockCount = 0;
	m_cProgress.SetPos(0);
	m_cProgress.ShowWindow(SW_SHOW);

	// 执行
	long lRet = DoSelectStock(&m_curSelStockCondition, HS_SELECTSTOCK_ANALYSIS);
	if( pOKBut != NULL )
	{
		if( lRet )
		{		
			pOKBut->SetWindowText(CDlgCondition_But_Stop);
			EnableBut(FALSE);
		}
		else
		{
			m_cProgress.ShowWindow(SW_HIDE);
		}
	}
	return 1;
}
int CDlgCondition::DoSelectStock( SelStockCondition* pSelStockCondition ,int style)
{
	if( pSelStockCondition == NULL )
	{
		return 0;
	}
	
	CFormulaCalc::m_pMsgWnd = (CWnd*)this;
	CArray<StockUserInfo*,StockUserInfo*>* payStock = &(StockArea::ayStock);

	if( payStock->GetSize() <= 0 )
	{
		MessageBox(_T("股票范围里没有股票列表，不能选股！"),"提示",MB_OK);
		return 0;
	}

	CFormulaCalc::m_curSelStockCondition  = pSelStockCondition;					// 条件
	CFormulaCalc::m_pExternExpression	  = CExpression::m_pExternExpression;		// 外部表达式表
	CFormulaCalc::m_pExternVariabile	  = CExpression::m_pExternVariabile;		// 外部变量表
	CFormulaCalc::m_pExternFunction		  = CExpression::m_pExternFunction;		    // 外部函数表
	CFormulaCalc::m_pSystemFun			  = g_pSystemFun;				// 函数指针

	// 启动
	int nRet = CFormulaCalc::Start();
	if( nRet != 1 )
	{
		return 0;
	}

	// 选股,由选股本身的进度条
	if( style != HS_SELECTSTOCK_ANALYSIS )
	{
	}
	return 1;

}
void CDlgCondition::OnUpDateNowData()
{
	m_curSelStockCondition.m_ayCheck	= &m_ayCheck;

	m_cProgress.SetPos(0);
	m_cProgress.ShowWindow(SW_SHOW);

// 	if( !::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,
// 		HS_SELECTSTOCK_ZhiNeng,(LPARAM)&m_curSelStockCondition) )
	if ( !DoSelectStock(&m_curSelStockCondition, HS_SELECTSTOCK_ZhiNeng))
	{
		m_cProgress.ShowWindow(SW_HIDE);
	}
}

int CDlgCondition::AutoSelect() 
{	
	m_AyLastStock.RemoveAll();
	
	m_curSelStockCondition.m_dStyle &= ~(SelectStock_Single | SelectStock_Union | SelectStock_Sort);
	
	// 组合条件
	if( m_nCurPos >= m_ayUnionSelCondition.GetSize() ||
		m_ayUnionSelCondition.GetSize() <= 0 )
		return 0;

	//
	UnionSelCondition* pHSUnion = (UnionSelCondition*)m_ayUnionSelCondition.GetAt(m_nCurPos);
	m_pHSUnion.Copy(pHSUnion);
	m_curSelStockCondition.m_dStyle |= SelectStock_Union;

	m_strBlock.Format("%s",pHSUnion->m_strName);

	//
	m_curSelStockCondition.m_ayCheck	= &m_ayCheck;			// 股票范围
	m_curSelStockCondition.m_strBlock	= NULL;//&m_strBlock;   // 板块名

	m_curSelStockCondition.m_sSingleSel = &m_sSingleSel;		// 单个选股条件
	m_curSelStockCondition.m_pHSUnion  = &m_pHSUnion;			// 组合选股条件

	// 日期
	if( m_curSelStockCondition.m_lEndDate == 0 )
	{
		CString str;
		CTime timeDest = CTime::GetCurrentTime();
		str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
		m_curSelStockCondition.m_lEndDate = atol(str);
	}

	//
	m_curSelStockCondition.m_pData = NULL;
	m_nSelStockCount = 0;
	
	// 执行
// 	long lRet = ::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,	HS_SELECTSTOCK_ANALYSIS,
// 		(LPARAM)&m_curSelStockCondition);
	DoSelectStock(&m_curSelStockCondition, HS_SELECTSTOCK_ANALYSIS);
	return 1;
}

void CDlgCondition::OnAutoSelect() 
{
	CButton* pOKBut = (CButton*)this->GetDlgItem(IDOK2);
	if( pOKBut != NULL )
	{
		if( pOKBut->GetCheck() == BST_UNCHECKED )
		{
			this->KillTimer((UINT)this);
			this->KillTimer((UINT)this+1);

			m_nStopAutoRun = 1;
			return;
		}		
	}

	m_nStopAutoRun = 0;
	m_nCurPos = -1;
	this->SetTimer((UINT)this,1000,NULL);
}

void CDlgCondition::OnToblock() 
{
	// TODO: Add your control notification handler code here
	//need to modify 打开板块 自选股
	//::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,1,(LPARAM)this);
}

long CDlgCondition::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	m_sSingleSel.m_nType = hx_DefMaxValues;

	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)(CWnd::FromHandle(m_hParentWnd));
	if( pParentDlg == NULL )
		return 0;

	if( wParam == HX_UNION_EXPRESS )
	{
		EnableItem(FALSE);

		
		GetDlgItem(IDD_NEWUNION)->EnableWindow(TRUE);
		GetDlgItem(IDD_UPDATEUNION)->EnableWindow(TRUE);
		GetDlgItem(IDC_DELETEUNION)->EnableWindow(TRUE);
		if( lParam )
		{
			m_sSingleSel.m_nType = Union;

			UnionSelCondition* pHSUnion = (UnionSelCondition*)lParam;

			m_wndParam.SetWindowText(GetDescribe(pHSUnion));
			m_wndParam.ShowWindow(SW_SHOW);

			m_wndNotes.ShowWindow(SW_SHOW);
			m_wndNotes.SetWindowText(_T("选出满足下列组合条件的股票:"));
		}
	}
	else if( wParam == CTreeCtrlFormula::HSBrowser )
	{
		EnableItem(FALSE);

		//		CString strItem;
		//		if( lParam )
		//		{
		//			strItem = *(CString*)lParam;
		//		}
		CTreeGroup* pGroup = (CTreeGroup*)lParam;

		if( pGroup->m_dwID == Union /*!strItem.CompareNoCase(CTreeCtrlFormula::g_strUnionCondition)*/ )
		{
			m_sSingleSel.m_nType = Union;
			m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strUnionCondition);

			m_wndNotes.ShowWindow(SW_HIDE);
			m_wndNotes.SetWindowText(_T("选出满足下列组合条件的股票:"));

			m_wndPeriod.ShowWindow(SW_HIDE);
			m_wndPeriodStc.ShowWindow(SW_HIDE);

			m_wndConNotes.ShowWindow(SW_HIDE);
			m_wndCon.ShowWindow(SW_HIDE);
			m_wndStaticCon.ShowWindow(SW_HIDE);

			m_wndCompare.ShowWindow(SW_HIDE);
			m_wndData1.ShowWindow(SW_HIDE);

			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);

			GetDlgItem(IDD_NEWUNION)->EnableWindow(TRUE);
			//m_wndUpdateUnion.ShowWindow(SW_SHOW);

		}
		else if( (pGroup->m_dwID == JiBenMian) /*||
															(pGroup->m_dwID >= COLUMN_CAPITALIZATION_BEGIN && pGroup->m_dwID < COLUMN_CAPITALIZATION_END ) || // 股本数据
															(pGroup->m_dwID >= COLUMN_FINANCE_BEGIN && pGroup->m_dwID < COLUMN_FINANCE_END )*/
															/*!strItem.CompareNoCase(CTreeCtrlFormula::g_strJiben)*/ )
		{
			if( pGroup->m_dwID == JiBenMian )
			{
				m_sSingleSel.m_nType = JiBenMian;
				m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strJiben);

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出财务指标满足条件的股票:"));

				m_wndPeriod.ShowWindow(SW_HIDE);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_HIDE);

				m_wndConNotes.ShowWindow(SW_SHOW);
				m_wndConNotes.SetWindowText(_T("财务指标"));
				m_wndCon.ShowWindow(SW_SHOW);
				m_wndStaticCon.ShowWindow(SW_SHOW);

				m_wndCompare.ShowWindow(SW_SHOW);
				m_wndData1.ShowWindow(SW_SHOW);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);

				m_wndData1.ResetContent();
				m_wndData2.ResetContent();

				m_wndCon.ResetContent();
				InitCWData(&m_wndCon,GB_GUBEN_NAME);
				InitCWData(&m_wndCon,Cj_GUBEN_NAME);

				int nIndex = m_wndCon.AddString("---------------");
				if( nIndex != -1 )
				{
					m_wndCon.SetItemData(nIndex,(DWORD)-1);
				}
#ifndef SUPPORT_WENJIAOSUO
				InitCWData(&m_wndCon,HQ_BASE_NAME);
#else
				InitCWData(&m_wndCon,"天艺份额");
#endif		

				InitCondition(&m_wndCompare,4);
			}
		}
	}
	else if( wParam == CCompileEditView::UpdateExpress )
	{
		CDlgFormulaEdit* pDlg = pParentDlg->GetExpDlg((CExpression*)lParam);
		if( pDlg != NULL )
		{
			if(pDlg->SendMessage(HX_USER_LOADCUR,wParam,lParam))
			{
				pParentDlg->Enable(FALSE);
				pDlg->ShowWindow(SW_SHOWNORMAL);

				CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
				if( pCurTree != NULL )
				{
					HTREEITEM hItem = pCurTree->GetSelectedItem( );
					if( hItem != NULL )
					{
						CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
						if( pTreeGroup != NULL &&
							pTreeGroup->IsStatus(hx_GroupItem) )
						{
							SendMessage(HX_USER_LOADCUR,HX_EXPRESSSENTENCE,(LPARAM)pTreeGroup->GetData());
						}
					}
				}
			}
		}
	}
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;

		EnableItem(FALSE);

		//
		m_sSingleSel.m_nType = pExpression->GetExpressType();
		m_sSingleSel.m_strExp.Format("%s",pExpression->GetName());
		InitCondition(&m_wndCompare,-1);

		GetDlgItem(IDD_NEWUNION)->EnableWindow(FALSE);
		GetDlgItem(IDD_UPDATEUNION)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETEUNION)->EnableWindow(FALSE);
		switch ( m_sSingleSel.m_nType )
		{
		case Tech:
			{
				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出技术指标数值满足下列条件的股票:"));

				if( IsType(Sort) )
				{
					m_wndNotes.SetWindowText(_T("按下面条件对指标进行排序:"));

				}

				m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_SHOW);

				m_wndConNotes.ShowWindow(SW_SHOW);
				m_wndConNotes.SetWindowText(_T("指标线"));

				m_wndCon.ShowWindow(SW_SHOW);
				m_wndCon.ResetContent();
				m_wndStaticCon.ShowWindow(SW_SHOW);

				CValue* valoare = NULL;
				if( CExpression::m_pExternExpression->Lookup(pExpression->GetName(),pExpression->GetExpressType(),valoare) ) // over
				{
					CExpValue* pExpValue = (CExpValue*)valoare;

					BOOL bFree = FALSE;
					CExpression* pCurExpression = pExpValue->GetExp();
					if( pCurExpression && pCurExpression == pExpression )
					{
						if( !pCurExpression->IsCompile() )
						{
							if( !pCurExpression->Compile() )
							{
								bFree = TRUE;
							}
						}
						CArray<CFormulaDrawParam*,CFormulaDrawParam*> ayDraw;
						CString strParam;
						int nCount = pCurExpression->GetDraw(strParam,ayDraw);
						CFormulaDrawParam* pParam;
						if( nCount > 0 )
						{
							for(int i = 0; i < nCount; i++)
							{
								pParam = ayDraw.GetAt(i);
								if( nCount == 1 && CString(pParam->m_strName).IsEmpty() )
								{
									m_wndCon.AddString(CTreeCtrlFormula::g_strFirstTechLine);
								}
								else
								{
									//yulx modify 去掉后三位的0
									CString name = pParam->m_strName;
									name = name.Left(name.GetLength() - 3);
									m_wndCon.AddString(name/*pParam->m_strName*/);
								}
								delete pParam;
							}
						}
						else
						{
							m_wndCon.AddString(CTreeCtrlFormula::g_strFirstTechLine);
						}
						m_wndCon.SetCurSel(m_wndCon.GetCount()-1);

						if( IsType(ConditionType) )
						{
							m_wndCompare.ShowWindow(SW_SHOW);
							m_wndData1.ShowWindow(SW_SHOW);
							OnSelchangeCompare();
						}
						else
						{
							m_wndStaticCon.ShowWindow(SW_HIDE);
							m_wndCompare.ShowWindow(SW_HIDE);
							m_wndData1.ShowWindow(SW_HIDE);
							m_wndAndOr.ShowWindow( SW_HIDE);
							m_wndData2.ShowWindow( SW_HIDE);
							m_wndMiddle.ShowWindow( SW_HIDE);

						}
					}
					if( bFree )
					{
						pExpValue->Free();
					}
				}				
			}
			break;
		case Condition:
			{
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Condition;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出满足下列条件的股票:"));

				m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_SHOW);

				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndCon.ShowWindow(SW_HIDE);
				m_wndStaticCon.ShowWindow(SW_HIDE);

				m_wndCompare.ShowWindow(SW_HIDE);
				m_wndData1.ShowWindow(SW_HIDE);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);
			}
			break;
		case Exchange:
			{
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Exchange;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出交易系统发出以下信号的股票:"));

				m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_SHOW);

				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndConNotes.SetWindowText(_T("发出"));
				m_wndCon.ShowWindow(SW_HIDE);
				m_wndStaticCon.ShowWindow(SW_HIDE);

				m_wndCompare.ShowWindow(SW_HIDE);
				m_wndData1.ShowWindow(SW_HIDE);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);
			}
			break;
		case Strategy: //20090302 YJT 增加 Strategy
			{
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Strategy;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出策略交易发出以下信号的股票:"));

				m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_SHOW);

				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndConNotes.SetWindowText(_T("发出"));
				m_wndCon.ShowWindow(SW_HIDE);
				m_wndStaticCon.ShowWindow(SW_HIDE);

				m_wndCompare.ShowWindow(SW_HIDE);
				m_wndData1.ShowWindow(SW_HIDE);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);
			}
			break;
		case MoreKLine:
			{
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::MoreKLine;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText(_T("选出五彩Ｋ线发出指示的股票:"));

				m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_SHOW);

				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndCon.ShowWindow(SW_HIDE);
				m_wndStaticCon.ShowWindow(SW_HIDE);

				m_wndCompare.ShowWindow(SW_HIDE);
				m_wndData1.ShowWindow(SW_HIDE);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);
			}
			break;
		}

		// 注释/参数修改

		GetDlgItem(IDD_USERNOTES)->EnableWindow(TRUE);
		CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
		if(pWnd)
		{
			/*pWnd->ShowWindow(SW_SHOW);*/
			pWnd->EnableWindow(true);
		}
		pWnd = this->GetDlgItem(IDC_BUTPARAM);
		if(pWnd)
		{
			/*pWnd->ShowWindow(SW_SHOW);*/
			pWnd->EnableWindow(true);
		}

		CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
		if( pDlgNotes != NULL && 
			pDlgNotes->IsWindowVisible() )
		{
			pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		}				
	}

	return 0;
}

void CDlgCondition::EnableBut(BOOL bEnable)
{	
	//
	//m_wndSwitchBut.EnableWindow(bEnable);

	//
	m_wndTab.EnableWindow(bEnable);

	// 
	CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	if(pWnd)
	{
		pWnd->EnableWindow( bEnable && !(m_sSingleSel.m_nType == JiBenMian || 
										m_sSingleSel.m_nType == Union) );
	}
	pWnd = this->GetDlgItem(IDC_BUTPARAM);
	if(pWnd)
	{
		pWnd->EnableWindow( bEnable && !(m_sSingleSel.m_nType == JiBenMian || 
										m_sSingleSel.m_nType == Union) );
	}

	GetDlgItem(IDD_USERNOTES)->EnableWindow( bEnable && !(m_sSingleSel.m_nType == JiBenMian || 
										     m_sSingleSel.m_nType == Union) );
	GetDlgItem(IDD_NEWUNION)->EnableWindow(!bEnable);
	GetDlgItem(IDD_UPDATEUNION)->EnableWindow(!bEnable);
	GetDlgItem(IDC_DELETEUNION)->EnableWindow(!bEnable);
	pWnd = this->GetDlgItem(IDD_ADVANCE);
	if(pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
	pWnd = this->GetDlgItem(IDD_PRESUCC);
	if(pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
	pWnd = this->GetDlgItem(IDD_TOBLOCK);
	if(pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}

	// right
	//m_wndSelStockResult.EnableWindow(bEnable);
	pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	if(pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
	pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
	if(pWnd)
	{
		pWnd->EnableWindow(bEnable);
	}
	if( bEnable )
	{
		if( IsIconic() )
		{
			this->ShowWindow(SW_SHOWNORMAL);
		}
		/*else
		{
			this->Invalidate();
		}*/
	}

	pWnd = this->GetDlgItem(IDCANCEL);
	if( pWnd != NULL )
	{
		pWnd->EnableWindow(bEnable);
	}

	pWnd = this->GetDlgItem(IDC_BUTFIND);
	if( pWnd != NULL )
	{
		pWnd->EnableWindow(bEnable);
	}

	pWnd = this->GetDlgItem(IDD_GETNOWDATA);
	if( pWnd != NULL )
	{
		pWnd->EnableWindow(bEnable);
	}	
	
}

void CDlgCondition::EnableItem(BOOL bEnable)
{
	CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	if(pWnd)
	{
		/*pWnd->ShowWindow(SW_HIDE);*/
		pWnd->EnableWindow(false);
	}
	pWnd = this->GetDlgItem(IDC_BUTPARAM);
	if(pWnd)
	{
		/*pWnd->ShowWindow(SW_HIDE);*/
		pWnd->EnableWindow(false);
	}
	pWnd = GetDlgItem(IDD_USERNOTES);
	if (pWnd)
		pWnd->EnableWindow(false);

	GetDlgItem(IDD_NEWUNION)->EnableWindow(FALSE);
	GetDlgItem(IDD_UPDATEUNION)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETEUNION)->EnableWindow(FALSE);
	//
	m_wndNotes.ShowWindow(SW_HIDE);	

	m_wndPeriod.ShowWindow(SW_HIDE);
	m_wndPeriodStc.ShowWindow(SW_HIDE);

	m_wndConNotes.ShowWindow(SW_HIDE);
	m_wndCon.ShowWindow(SW_HIDE);
	m_wndStaticCon.ShowWindow(SW_HIDE);

	m_wndCompare.ShowWindow(SW_HIDE);
	m_wndData1.ShowWindow(SW_HIDE);

	m_wndAndOr.ShowWindow(SW_HIDE);
	m_wndData2.ShowWindow(SW_HIDE);
	m_wndMiddle.ShowWindow(SW_HIDE);

	m_wndParam.ShowWindow(SW_HIDE);
}

void CDlgCondition::OnSelchangeCompare() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndCompare.GetCurSel();
	if( nIndex == -1 ) 
		goto __OnSelchangeCompare_;

	switch( nIndex )
	{
	case FormulaCompareSubData::DaYu:		//  大于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoYu:		//	小于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::DengYu:		//	等于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::JieYu:		//	介于
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("与"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_SHOW);
		}
		break;
	case FormulaCompareSubData::ShangChuan: //	上穿
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoPo:		//	下破
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::WeiChi:		//	维持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后上穿"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::BiaoChi:		//	保持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后下破"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	}

__OnSelchangeCompare_:
	m_wndData1.ResetContent();
	m_wndData2.ResetContent();

	if( m_sSingleSel.m_nType >= 0 &&
		!(m_sSingleSel.m_nType == JiBenMian || 
		  m_sSingleSel.m_nType == Union)
		)
	{
		int nCount = m_wndCon.GetCount();
		if( nCount > 1 )
		{
			int nCur = m_wndCon.GetCurSel();
			CString strTmp;
			for( int i = 0; i < nCount; i++)
			{
				if( i != nCur )
				{
					m_wndCon.GetLBText(i,strTmp);
					if( nIndex != FormulaCompareSubData::WeiChi && nIndex != FormulaCompareSubData::BiaoChi )
					{
						m_wndData1.AddString(strTmp);
					}
					m_wndData2.AddString(strTmp);
				}
			}
		}
	}
}

void CDlgCondition::OnSelchangeCon() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeCompare();
}

CString CDlgCondition::GetNameByKey(CString& strFindKeyName) 
{
	CString strRet;
	if( g_pTableHeadData )
	{
		void* pValue;
		CMapWordToPtr* pMap;

		CString strName = GB_GUBEN_NAME;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				if( pMap->Lookup(atol(strFindKeyName),pValue))
				{
					strRet.Format("%s",((TableColProperty*)pValue)->strName );
				}
			}
		}

		strName = Cj_GUBEN_NAME;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				if( pMap->Lookup(atol(strFindKeyName),pValue) )
				{
					strRet.Format("%s",((TableColProperty*)pValue)->strName);
				}
			}
		}
	}
	return strRet;
}

void CDlgCondition::InitCWData(CComboBox* pBox,CString strName) 
{
	if( g_pTableHeadData )
	{
		void* pValue;
		CMapWordToPtr* pMap;
		int nIndex;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				TableColProperty* pCol;
				POSITION pos = pMap->GetStartPosition();
				WORD wKey;
				while(pos)
				{
					pMap->GetNextAssoc(pos, wKey, pValue);
					if(pValue != NULL)
					{
						if( wKey == COLUMN_HQ_BASE_NAME ||
							wKey == COLUMN_HQ_BASE_CODE ||
							wKey == COLUMN_HQ_BASE_RISE_SPEED ||
							wKey == COLUMN_HQ_EX_DIRECTION ||
							wKey == COLUMN_HQ_BASE_NUMBER ||
							wKey == COLUMN_HQ_BASE_TECH_SORT
							)
						{
							continue;
						}

						pCol = (TableColProperty*)pValue;
						nIndex = pBox->AddString(pCol->strName);
						if( nIndex != -1 )
						{
							pBox->SetItemData(nIndex,(DWORD)wKey);
							if( wKey == COLUMN_CAPITALIZATION_TOTAL )
							{
								pBox->SetCurSel(nIndex);
							}
						}
					}
				}
			}
		}
	}
}

void CDlgCondition::InitCondition(CComboBox* pBox,int nType)
{
	pBox->ResetContent();
	int nCount;
	char** pData = CExpValue::GetConditionStr(nCount);
	if( nType == -1 )
		nType = nCount;
	for(int i = 0; i < nType; i++)
	{
		pBox->AddString(pData[i]);
	}
	pBox->SetCurSel(0);
}

void CDlgCondition::OnNewunion() 
{
	// TODO: Add your control notification handler code here
	CDlgUnionConSelStock dlg(this);
	dlg.m_bCreate = 1;
	if( dlg.DoModal() == IDOK )
	{
		if( dlg.m_pHSUnion != NULL )
		{
			if( IsValidateUnion(dlg.m_pHSUnion) )
			{
				CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
				if( pCurTree != NULL )
				{
					HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
					if( hRootItem != NULL )
					{
						hRootItem = pCurTree->InsertItem( dlg.m_pHSUnion->m_strName,CExpression::HS_Exp_Union,
							CExpression::HS_Exp_Union,hRootItem );
						pCurTree->SetItemData(hRootItem,(DWORD)dlg.m_pHSUnion);
					}
				}
			}
			else
			{
				delete dlg.m_pHSUnion;
			}
		}

		SaveLoad(CArchive::store,&m_wndTab,"",0,this); // save current
	}
}

BOOL CDlgCondition::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CHSBaseDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CHSBaseDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CHSBaseDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CHSBaseDialog::ShowWindow(nCmdShow);
	}

	return 1;
}

void CDlgCondition::OnUpdateunion() 
{
	// TODO: Add your control notification handler code here
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		return;

	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		return;
	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( pTreeGroup == NULL )
		return;

	CDlgUnionConSelStock dlg(this);

	dlg.m_pHSUnion = (UnionSelCondition*)pTreeGroup;
	dlg.m_bCreate = 0;
	if( dlg.DoModal() == IDOK )
	{
		if( dlg.m_pHSUnion != NULL )
		{
			UnionSelCondition* pHSUnion = (UnionSelCondition*)dlg.m_pHSUnion;
			if( IsValidateUnion(pHSUnion) )
			{
				CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
				if( pCurTree != NULL )
				{
					pCurTree->SetItemText(hItem,dlg.m_pHSUnion->m_strName);
				}
				m_wndParam.SetWindowText(GetDescribe(pHSUnion));
			}
			else
			{
				pCurTree->DeleteItem(hItem);
				delete dlg.m_pHSUnion;
			}
		}

		SaveLoad(CArchive::store,&m_wndTab,"",0,this); // save current
	}
}

BOOL CDlgCondition::IsValidateUnion(UnionSelCondition* pHSUnion)
{
	if( pHSUnion == NULL )
		return FALSE;

	CArray<UnionSelConditionSub*,UnionSelConditionSub*>* paySub = pHSUnion->m_aySub;
	if( paySub && paySub->GetSize() > 0 )
	{
		UnionSelConditionSub* pUnionSelConditionSub;
		for(int i = 0;i < paySub->GetSize(); i++)
		{
			pUnionSelConditionSub = paySub->GetAt(i);
			if( pUnionSelConditionSub->m_aySub->GetSize() > 0 )
				return TRUE;
		}
	}

	return FALSE;
}

LRESULT CDlgCondition::OnCommNotify(WPARAM wParam, LPARAM lParam)
{
	if(lParam == 0)
		return 0;

// 	CNI_ReceiveData *pInfo = (CNI_ReceiveData *)lParam;
// 	
// 
// 	switch (pInfo->m_hd.m_nEvent)
// 	{
// 	case CEV_RECEIVE_DATA:
// 		{
// 			DataHead* pHead = (DataHead*)pInfo->m_pszData;
// 			if( pHead->m_nType != RT_ZIPDATA )
// 			{
// 				if( (pHead->m_lKey != (long)this ) || (pHead->m_nType != RT_REALTIME) )
// 					return 0;
// 			}
// 			if( pInfo->m_lDataTotal == 0 )
// 				return 0;
// 
// 			float fTransmited = (float)pInfo->m_lDataTransmited / pInfo->m_lDataTotal;
// 			
// 			if( pInfo->m_lDataTransmited == pInfo->m_lDataTotal )
// 			{
// 				
// 			}
// 		}
// 		break;
// 	}

	return 0;
}

LRESULT CDlgCondition::OnExchangeData(WPARAM wp,LPARAM lp)
{
	if( wp == 1 ) // 股票列表
	{		
	}
	else if (wp == 2) // 板块名称
	{
		m_strBlock.Format("%s",*(CString*)lp);
		Select();
	}
	else if (wp == 3) // 返回当前选中股票
	{
		return (long)&m_ayCheck;
	}
	else if( wp == SelectingStock ) // 选股过程中
	{
 		CNI_ReceiveData* pInfo2 = (CNI_ReceiveData*)lp;
 		
 		SelStockResult* pHSSelectStockResult = (SelStockResult*)pInfo2->m_pszData;
 
 		float fTransmited = (float)pInfo2->m_lDataTransmited / pInfo2->m_lDataTotal;
 		if ( m_cProgress.m_hWnd )
 		{
 			CString strPrompt;
 
 			if( pHSSelectStockResult )
 			{
 				m_AyLastStock.Add((StockUserInfo*)pHSSelectStockResult->pStock);
 				m_nSelStockCount ++;
 			}
 			
 			if( pInfo2->m_lDataTotal > 0 )
 			{
 				if( m_nID == IDD_CONDITION_STOCK_Auto )
 				{
 					strPrompt.Format("[%s] 选中数: %i,选中比例: %.2f%%",
 						m_strBlock,
 						m_nSelStockCount,
 						(m_nSelStockCount/(float)pInfo2->m_lDataTotal*100));
 				}
 				else
 				{
 					strPrompt.Format(CDlgCondition_Text_Prompt,
 						m_nSelStockCount,
 						(m_nSelStockCount/(float)pInfo2->m_lDataTotal*100));
 				}
 			}
 			m_wndPromptText.SetWindowText(strPrompt);
 			m_cProgress.SetPos(fTransmited);
 		}
	}
	else if( wp == SelectStockOver ) // 选股结束
	{
		if( m_nID == IDD_CONDITION_STOCK_Auto )
		{
			if( m_nStopAutoRun )
			{
			}
			else
			{
				this->SetTimer((UINT)this,m_nDelayTimer,NULL);
			}

			WriteBlock();

			return 0;
		}

		//
		CString strPrompt;
		CWnd* pOKBut = this->GetDlgItem(IDOK);
		if( pOKBut != NULL )
		{
			pOKBut->GetWindowText(strPrompt);
			if( !strPrompt.CompareNoCase(CDlgCondition_But_Run) )
			{
				strPrompt = _T("选股中止!");
			}
			else
			{
				strPrompt = _T("选股完成!");
			}

			pOKBut->SetWindowText(CDlgCondition_But_Run);
		}
		EnableBut(TRUE);

		//		
		CString strText;
		m_wndPromptText.GetWindowText(strText);
		strPrompt = strPrompt + strText;
		m_wndPromptText.SetWindowText(strPrompt);

		m_cProgress.ShowWindow(SW_HIDE);
		
		//
		if( !WriteBlock() )
			return 0;

		//发送给外部模块
		
		if ( g_pFormulaMan )
			g_pFormulaMan->SendLinkMsg(DiagramMsg_BlockChange,(LPARAM)CDlgCondition_Default_Name);
// 		if( m_hMsgWnd )
// 		{
// 			HSCurKeyInfo keyinfo;
// 			keyinfo.Copy(CDlgCondition_Default_Name);
// 			::SendMessage(m_hMsgWnd,HX_USER_DRAGDATA,
// 				HSCurKeyInfo::OpenBlock,(LPARAM)&keyinfo);	
// 
// 			if( m_sSingleSel.m_nType == JiBenMian )
// 			{
// 				long nID = atol(m_sSingleSel.m_strCondition);
// 				if( nID )
// 				{
// 					::SendMessage(m_hMsgWnd,g_nGetHSMain,g_nGetHSMain+4,(LPARAM)nID);
// 				}
// 			}
// 		}	

		m_AyLastStock.RemoveAll();

		this->PostMessage(WM_COMMAND,IDCANCEL,0);
	}

	return 0;
}

int CDlgCondition::WriteBlock()
{
	int nCount = m_AyLastStock.GetSize();
	if( nCount <= 0 )
	{
		m_strBlock.Empty();
		return 0;
	}

	// 写入文件
	CString strPath = m_strSelectSavePath;
	strPath += CDlgCondition_Default_Name;
	strPath += ".blk";

	CFile fpBlock;
	BOOL bBlock = !m_strBlock.IsEmpty();
	if( bBlock )
	{
		fpBlock.Open(m_strSelectSavePath + m_strBlock + ".blk",
			CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary);
		m_strBlock.Empty();
	}

	CFile fp;
	if( fp.Open(strPath,CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate | CFile::typeBinary) )
	{
		long lMark = 888; // mark
		fp.Write( &lMark, sizeof(long) );
		if( bBlock )
		{
			fpBlock.Write( &lMark, sizeof(long) );
		}

		StockUserInfo* pStock;
		for(int i = 0; i < nCount; i++)
		{
			pStock = m_AyLastStock.GetAt(i);
			if( pStock )
			{
				fp.Write(&pStock->m_ciStockCode,sizeof(CodeInfo));
				if( bBlock )
				{
					fpBlock.Write(&pStock->m_ciStockCode,sizeof(CodeInfo));
				}
			}
		}

		fp.Close();
		if( bBlock )
		{
			fpBlock.Close();
		}
	}

	return nCount;
}

void CDlgCondition::Delete()
{
	SaveLoad(CArchive::store,&m_wndTab,"",0,this);

	//
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree != NULL )
	{
		HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
		CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData( hRootItem );
		if( pTreeGroup != NULL)
		{
			delete pTreeGroup;
		}
		hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strJiben);
		pTreeGroup = (CTreeGroup*)pCurTree->GetItemData( hRootItem );
		if( pTreeGroup != NULL)
		{
			delete pTreeGroup;
		}
		if( hRootItem != NULL )
		{
			HTREEITEM hChildItem = pCurTree->GetChildItem( hRootItem );
			while(hChildItem != NULL)
			{
				UnionSelCondition* pHSUnion = (UnionSelCondition*)pCurTree->GetItemData( hChildItem );
				if( pHSUnion )
				{
					delete pHSUnion;
				}
				hChildItem = pCurTree->GetNextSiblingItem( hChildItem );
			}
		}
	}
	//
	for(int i = 0; i < m_ayCheck.GetSize(); i++)
	{
		delete m_ayCheck.GetAt(i);
	}
	m_ayCheck.RemoveAll();
}

BOOL CDlgCondition::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	/*if(pMsg->message == WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_F7 )
		{
			this->ShowWindow(SW_SHOW);
			return 1;
		}
	}*/

	return CHSBaseDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgCondition::SaveLoad(UINT nMode,CExpTabCtrl* pwndTab,
							 CString strFileName /*= ""*/,DWORD dStyle /*= 0*/,
							 CDlgCondition* pCDlgCondition /*= NULL*/)
{
//	if( !IsType(Condition) )
//		return 0;

	CFile fp;
	if(strFileName.IsEmpty())
	{
		strFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +"Union.exp";
	}

	BOOL bOpen = FALSE;
	if(nMode == CArchive::store) // 存储
	{
		CString strTempPath;
		GetTempPath (MAX_PATH, strTempPath.GetBuffer (MAX_PATH));
		strTempPath.ReleaseBuffer();

		CString strTempName;
		GetTempFileName (strTempPath, _T("hsExp"), 0, strTempName.GetBuffer (MAX_PATH));
		strTempName.ReleaseBuffer ();

		bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeCreate | CFile::modeReadWrite );
		if(!bOpen) 
		{
			return FALSE;
		}

		try
		{
			CArchive ar( &fp, nMode );
			CExpression::HSFormulaFileHead head;
			head.m_strHead = CTreeCtrlFormula::g_strUnionCondition;
			head.Serialize(ar);

			int nCount = 0;
			Serialize(ar,nCount,pwndTab);
			ar.Flush();

			fp.SeekToBegin();
			ar << nCount;

			ar.Close();
			fp.Close();
		}
#ifdef VC_7_0
		catch( CMemoryException memoryex)
		{
			return FALSE;
		}
		catch( CArchiveException archiveex )
		{
			return FALSE;
		}
		catch( CFileException fileex)
		{
			return FALSE;
		}
#endif
		catch(...)
		{
			return FALSE;
		}

		// copy
		bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | CFile::modeReadWrite );
		if( !bOpen ) 
		{
			return FALSE;
		}

		int lLen = fp.GetLength();
		if(lLen > 0)
		{
			char* pData = new char[lLen];
			fp.Read(pData,lLen);
			fp.Close();

			CFile curFile;
			if( curFile.Open( strFileName, CFile::typeBinary | CFile::shareDenyNone | 
				CFile::modeCreate | CFile::modeReadWrite ) )
			{
				curFile.Write(pData,lLen);
				curFile.Close();
			}
			delete[] pData;
		}

		// delete 
		TRY
		{
			CFile::Remove( strTempName );
		}
		CATCH( CFileException, e )
		{
		}
		END_CATCH

	}
	else // 装入
	{
		CFileException fileEx;
		bOpen = fp.Open( strFileName, CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeReadWrite, &fileEx); 		
		if(!bOpen) 
		{
			return FALSE;
		}

		try
		{
			CArchive ar( &fp, nMode );

			CExpression::HSFormulaFileHead head;
			head.Serialize(ar);
			if( !head.IsValid() )
				return FALSE;

			CTreeCtrlFormula* pCurTree = pwndTab->GetTree();
			if( pCurTree == NULL )
				return FALSE;

			HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
			if( hRootItem == NULL )
				return FALSE;

			if(head.m_lCount < 0)
			{
				return FALSE;
			}

			char cStatus;
			HTREEITEM hItem;
			UnionSelCondition* pHSUnion;
			for(int i = 0; i < head.m_lCount; i++)
			{
				ar >> cStatus;
				if( cStatus )
				{
					pHSUnion = new UnionSelCondition;
					pHSUnion->Serialize(ar);

					hItem = pCurTree->InsertItem( pHSUnion->m_strName,
						CExpression::HS_Exp_Union, CExpression::HS_Exp_Union,hRootItem);
					pCurTree->SetItemData(hItem,(DWORD)pHSUnion);

					if(pCDlgCondition != NULL)
						pCDlgCondition->m_ayUnionSelCondition.Add(pHSUnion);						
				}
			}

			fp.Close();
		}

#ifdef VC_7_0
		catch( CMemoryException memoryex)
		{
			return FALSE;
		}
		catch( CArchiveException archiveex )
		{
			return FALSE;
		}
		catch( CFileException fileex)
		{
			return FALSE;
		}
#endif
		catch(...)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CDlgCondition::Serialize(CArchive& ar,int& nCountRef,CExpTabCtrl* pwndTab)
{
	nCountRef = 0;

	CTreeCtrlFormula* pCurTree = pwndTab->GetTree();
	if( pCurTree == NULL )
		return;

	HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
	if( hRootItem == NULL )
		return;

	UnionSelCondition* pHSUnion;
	char cStatus;
	HTREEITEM hChildItem = pCurTree->GetChildItem( hRootItem );
	while(hChildItem != NULL)
	{
		pHSUnion = (UnionSelCondition*)pCurTree->GetItemData( hChildItem );

		if( pHSUnion )
		{
			cStatus = 1;
			ar << cStatus;
			pHSUnion->Serialize(ar);
		}
		else
		{
			cStatus = 0;
			ar << cStatus;
		}

		nCountRef++;
		hChildItem = pCurTree->GetNextSiblingItem( hChildItem );
	}
}

CString CDlgCondition::GetDescribe(UnionSelCondition* pHSUnion)
{
	if( pHSUnion == NULL ) return "";

	UnionSelConditionSub* pSelSub;
	SingleSelCondition*   pUnionData;
	int i,j;

	CString strRet,strTmp;
	for( i = 0; i < pHSUnion->m_aySub->GetSize(); i++ )
	{
		pSelSub = pHSUnion->m_aySub->GetAt(i);

		strTmp.Format("%i.",i+1);
		strRet += strTmp;
		strRet += pSelSub->m_strName;
		strRet += "\r\n";

		for( j = 0; j < pSelSub->m_aySub->GetSize(); j++ )
		{
			pUnionData = pSelSub->m_aySub->GetAt(j);

			CString* strCurRet = NULL;
			switch( pUnionData->m_nType )      // 公式类型
			{
			case Tech:
				strCurRet = &CTreeCtrlFormula::g_strTech;
				break;
			case Condition:
				strCurRet = &CTreeCtrlFormula::g_strCondition;
				break;
			case Exchange:
				strCurRet = &CTreeCtrlFormula::g_strExchange;
				break;
			case Strategy: //20090302 YJT 增加 Strategy
				strCurRet = &CTreeCtrlFormula::g_strStrategy;
				break;
			case MoreKLine:
				strCurRet = &CTreeCtrlFormula::g_strColorKLine;
				break;
			case JiBenMian:
				{
					CString strCondition = pUnionData->m_strCondition;
					pUnionData->m_strCondition = CDlgCondition::GetNameByKey(pUnionData->m_strCondition);

					strTmp.Format("%i.%i",i+1,j+1);
					strRet += strTmp;
					strRet += pUnionData->GetDescribe(strCurRet);
					strRet += "\r\n";

					pUnionData->m_strCondition = strCondition;
				}
				continue;
			}

			strTmp.Format("%i.%i",i+1,j+1);
			strRet += strTmp;
			strRet += pUnionData->GetDescribe(strCurRet);
			strRet += "\r\n";
		}
	}
	return strRet;
}

void CDlgCondition::OnButhideleft() 
{
	CWnd* pWnd;		
	CString strText;
	pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	pWnd->GetWindowText(strText);

	CRect rect,rc,rc1;
	this->GetClientRect(rect);

	char cWidth = 5;

	int nCmdShow;
	if( !strText.CompareNoCase("<<") )
	{
		nCmdShow = SW_HIDE;
		strText = ">>";
		pWnd->SetWindowText(strText);

		
		pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rect.left + cWidth,rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

		pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
		if(pWnd)
		{
			pWnd->GetWindowRect(rc1);
			ScreenToClient(rc1);
		}		
		int nWidth = rc.Width();
		m_wndSelStockResult.GetWindowRect(rc);
		ScreenToClient(rc);
		m_wndSelStockResult.SetWindowPos(NULL,rect.left + nWidth + cWidth,
			rc.top,rect.Width() - cWidth * 2 - rect.left - nWidth,
			rect.Height() - rc.top - rc1.Height() - cWidth*2 ,SWP_NOZORDER);
		pWnd = m_wndSelStockResult.GetDlgItem(1001);
		if( pWnd )
		{
			m_wndSelStockResult.GetClientRect(rc);
			pWnd->MoveWindow(rc);
		}

		m_wndParam.ShowWindow(SW_HIDE);
	}
	else
	{
		nCmdShow = SW_SHOW;
		strText = "<<";
		pWnd->SetWindowText(strText);

	

		m_wndBottomPos.GetWindowRect(rc1);
		ScreenToClient(rc1);

		pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc1.right + 1,rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);

		int nLeft = rc1.right + 1 + rc.Width() + cWidth;

		pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
		if(pWnd)
		{
			pWnd->GetWindowRect(rc1);
			ScreenToClient(rc1);			
		}

		int nHeightOff = rect.Width() - rc.right - cWidth;
		m_wndSelStockResult.GetWindowRect(rc);
		ScreenToClient(rc);
		m_wndSelStockResult.SetWindowPos(NULL,nLeft,rc.top,nHeightOff,
			rect.Height() - rc.top - rc1.Height() - cWidth*2,
			SWP_NOZORDER);

		pWnd = m_wndSelStockResult.GetDlgItem(1001);
		if( pWnd )
		{
			m_wndSelStockResult.GetClientRect(rc);
			pWnd->MoveWindow(rc);
		}

		SetSize();
	}	

	// bottom
	m_wndTab.ShowWindow( nCmdShow );
	m_wndBottomPos.ShowWindow( nCmdShow );

	m_wndBottomPos.GetWindowRect(rc);
	ScreenToClient(rc);

	m_wndNotes.ShowWindow( nCmdShow );
	m_wndConditionPos.ShowWindow( nCmdShow );

	m_wndPeriod.ShowWindow( nCmdShow );
	m_wndPeriodStc.ShowWindow( nCmdShow );

	m_wndConNotes.ShowWindow( nCmdShow );
	m_wndCon.ShowWindow( nCmdShow );
	m_wndStaticCon.ShowWindow( nCmdShow );

	m_wndCompare.ShowWindow( nCmdShow );
	m_wndData1.ShowWindow( nCmdShow );	
	m_wndAndOr.ShowWindow( nCmdShow );
	m_wndData2.ShowWindow( nCmdShow );
	m_wndMiddle.ShowWindow( nCmdShow );

	if( nCmdShow == SW_SHOW )
	{
		BOOL bChange = TRUE;
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree != NULL )
		{
			HTREEITEM hItem = pCurTree->GetSelectedItem( );
			if( hItem != NULL )
			{
				CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
				if( pTreeGroup != NULL &&
					pTreeGroup->IsStatus(hx_GroupItem) )
				{
					SendMessage(HX_USER_LOADCUR,HX_EXPRESSSENTENCE,(LPARAM)pTreeGroup->GetData());
				}
				else
				{
					bChange = FALSE;
				}
			}
			else
			{
				bChange = FALSE;
			}
		}	
		if( !bChange )
		{
			if( IsType(Sort) )
			{
				SendMessage(HX_USER_LOADCUR,CTreeCtrlFormula::HSBrowser,0);
			}
			else
			{
				SendMessage(HX_USER_LOADCUR,CTreeCtrlFormula::HSBrowser,(LPARAM)&CTreeCtrlFormula::g_strJiben);
			}
		}
	}

	//
	pWnd = this->GetDlgItem(IDD_ADVANCE);
	if(pWnd)
	{
		pWnd->ShowWindow( nCmdShow );
	}
	/*
	pWnd = this->GetDlgItem(IDD_PRESUCC);
	if(pWnd)
	{
	pWnd->ShowWindow( nCmdShow );
	}
	*/

	if( IsType(ConditionType) )
	{
		pWnd = this->GetDlgItem(IDD_TOBLOCK);
		if(pWnd)
		{
			pWnd->ShowWindow( nCmdShow );
		}
	}

	pWnd = this->GetDlgItem(IDOK);
	if(pWnd)
	{
		pWnd->ShowWindow( nCmdShow );
	}

	//
	pWnd = this->GetDlgItem(IDD_UPDATEUNION);
	if(pWnd)
	{
		pWnd->ShowWindow( SW_HIDE );
	}
	pWnd = this->GetDlgItem(IDD_NEWUNION);
	if(pWnd)
	{
		pWnd->ShowWindow( SW_HIDE );
	}

	//
	pWnd = this->GetDlgItem(IDD_USERNOTES);
	if(pWnd)
	{
		/*pWnd->ShowWindow( nCmdShow );*/
		pWnd->EnableWindow(nCmdShow);
	}
	pWnd = this->GetDlgItem(IDC_BUTPARAM);
	if(pWnd)
	{
		/*pWnd->ShowWindow( nCmdShow );*/
		pWnd->EnableWindow(nCmdShow);
	}
	pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	if(pWnd)
	{
		/*pWnd->ShowWindow( nCmdShow );*/
		pWnd->EnableWindow(nCmdShow);
	}

	this->Invalidate();
}

void CDlgCondition::OnSaveasblock() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,8,(LPARAM)this);
}

void CDlgCondition::OnButexpress() 
{
	// TODO: Add your control notification handler code here
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		pCurTree->SendMessage(WM_COMMAND,IDM_EDITEXPRESS);
	}
}

void CDlgCondition::OnButparam() 
{
	// TODO: Add your control notification handler code here
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)(CWnd::FromHandle(m_hParentWnd));
	if( pParentDlg == NULL )
		return;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		return;
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	if( pExpression == NULL )
		return;

	CDlgNotesAndEidolon* pDlgParam = pParentDlg->GetDlgParam();
	if( pDlgParam != NULL )
	{
		pDlgParam->m_pExpression = pExpression;
		CString strText;
		strText.Format("%s",pExpression->GetEidolon());
		if( !strText.IsEmpty() )
		{
			pDlgParam->SetEidolon(strText,pExpression->GetParamVar(),pExpression->GetName());
		}
		else
		{
			CStringArray* pArray = pExpression->GetDefEidolon();
			if(pArray->GetSize() <= 1)
			{
				pArray = NULL;
			}
			pDlgParam->SetEidolon(pArray,pExpression->GetParamVar(),pExpression->GetName());
		}					
		pDlgParam->Invalidate();
		pDlgParam->ShowWindow(SW_SHOW);
	}
}

void CDlgCondition::SetType(char cType) 
{
//	if( m_cType == cType )
//		return;

	m_cType = cType;
	/*if( m_cType == Sort ) 
	{
		m_wndSwitchBut.SetWindowText("排序状态");
		this->SetWindowText("指标排序");
	}
	else
	{
		m_wndSwitchBut.SetWindowText("选股状态");
		this->SetWindowText("条件选股");
	}*/

	Init();

	if( IsType(ConditionType) )
	{
		CWnd* pWnd = this->GetDlgItem(IDD_TOBLOCK);
		if(pWnd)
		{
			pWnd->ShowWindow( SW_SHOW );
		}
	}
	EnableItem(FALSE);

	//------2009.12.08 litao修改默认条件选股显示条件选股节点--------------------------------------
	//为了界面的美观，一打开此对话框便选择第一个Item
	CTreeCtrlFormula* pCurTree = this->m_wndTab.GetTree();
	/*
	HTREEITEM hSelItem;
	if(pCurTree)
	{
		HTREEITEM hItem = pCurTree->GetRootItem();
		//HTREEITEM hSelItem;
		while(hItem)
		{
			hSelItem = hItem;
			hItem = pCurTree->GetChildItem(hItem);
		}
		pCurTree->SelectItem(hSelItem);
	}
	//end */

	//开始默认选中条件选股
	HTREEITEM  root = pCurTree->GetRootItem();

	root = pCurTree->GetNextItem(root,TVGN_NEXT);
	HTREEITEM hSelItem = NULL;

	while(root != NULL)
	{
		if (pCurTree->GetItemText(root) == G_STRCONDITION)   
		{
			pCurTree->Expand(root,TVE_EXPAND);
			pCurTree->SelectItem(root);
			
			HTREEITEM hItem = pCurTree->GetChildItem(root);
			//HTREEITEM hSelItem;
			while(hItem)
			{
				pCurTree->SelectItem(hSelItem);
				hSelItem = hItem;
				hItem = pCurTree->GetChildItem(hItem);
			}
			//pCurTree->SelectItem(hSelItem);
			//end
			break;
		}
		root = pCurTree->GetNextItem(root,TVGN_NEXT);
	}	
	//--------------------end-------------------------------------------------------------------------

	if(pCurTree)
	{
		CTreeGroup* pTreeGroup = (CTreeGroup*)(pCurTree->GetItemData(hSelItem));
		if( pTreeGroup != NULL &&
			pTreeGroup->IsStatus(hx_GroupItem) )
		{
			EnableBut(TRUE);
		}
	}
}

void CDlgCondition::MyNcDestroy()
{
	/*::SendMessage(m_hMsgWnd,HX_USER_COMPILEDATA,20,0);*/
}

void CDlgCondition::OnBnClickedButfind()
{
	// TODO: Add your control notification handler code here
	DWORD dwMask = 0;
	dwMask = Tech;

	CDlgFormulaMan* pWnd = (CDlgFormulaMan*)(CWnd::FromHandle(m_hParentWnd));
	if(pWnd)
	{
		pWnd->Find(dwMask,this);
	}
}

LRESULT CDlgCondition::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/)
	{
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree == NULL )  return 0;

		CExpression* pExpress = (CExpression*)lParam;
		if( pExpress == NULL ) return 0;

		pCurTree->Show(pExpress,0);
		LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);

		return 1;
	}
	return 0;
}

// gbq add 20060209, 公式查找
CExpression* CDlgCondition::FindExpression(CString strKey)
{
	strKey.MakeUpper();
	CMapVariabile* pVarsMap = CDlgFormulaMan::m_pExternExpression;
	if(pVarsMap != NULL)
	{
		POSITION pos = pVarsMap->GetStartPosition();

		CString sName;
		CValue *pVal;
		while (pos != NULL)
		{			
			pVarsMap->GetNextAssoc (pos, sName, pVal);

			if(pVal != NULL)
			{
				if( pVal->IsStyle(HX_EXPRESSSENTENCE) )
				{
					CString strRet;
					if( ((CExpValue*)pVal)->Find(strKey,strRet, 512 /*CExpression::FindInName*/) )
					{
						CExpression* pExpression = ((CExpValue*)pVal)->GetExp();
						if(pExpression )
							return pExpression;
					}
				}
			}
		}
	}
	return NULL;
}

CExpression*  CDlgCondition::ShowExpression(CString strName)
{
	CExpression *pExpression = FindExpression(strName);
	if( pExpression == NULL )
		return NULL;

	CTreeCtrlFormula* pTree = m_wndTab.GetTree();
	pTree->Show(pExpression, 1);

	return pExpression;
}
// gbq add end

void CDlgCondition::OnTimer(UINT nIDEvent)
{
	if( nIDEvent == (UINT)this )
	{
		this->KillTimer(nIDEvent);

		if( m_ayUnionSelCondition.GetSize() <= 0 )
			return;

		m_nCurPos++;
		if( m_nCurPos >= m_ayUnionSelCondition.GetSize() )
		{	
			this->SetTimer((UINT)this+1,m_nDelayTaskTimer,NULL);
			m_nCurPos = 0;
			return;
		}

		AutoSelect();

		return;
	}
	else if( nIDEvent == ((UINT)this + 1) )
	{
		this->KillTimer(nIDEvent);

		AutoSelect();
		return;
	}

	CHSBaseDialog::OnTimer(nIDEvent);
}



void CDlgCondition::OnBnClickedDeleteunion()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox("确定要删除此组合吗？","提示",MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES)
	{
		return;
	}
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		return;

	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		return;
	
	pCurTree->DeleteItem(hItem);
	SaveLoad(CArchive::store,&m_wndTab,"",0,this); 
}

CWnd*	CDlgCondition::GetParentEx()
{
	return CWnd::FromHandle((HWND)m_lRefWnd);
}

