// SheetExchMain.cpp : implementation file
//

#include "stdafx.h"
#include "SheetExchMain.h"
#include "WizPageExchBuy.h"
#include "WizPageExchModel.h"
#include "WizPageExchResult.h"
#include "WizPageExchSell.h"
#include "WizPageExchSelIndicator.h"
#include "WizPageUnionExchCaclResult.h"
#include "Express.h"
// CSheetExchMain dialog
extern HWND g_hParentWnd;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSheetExchMain::CSheetExchMain(CWnd* pParent /*=NULL*/)
	: CHSNewWizDialog(CSheetExchMain::IDD, pParent)
{
	
	m_dlgTable = NULL;
	m_pRefMainTech = NULL;
	/*Create(CSheetExchMain::IDD, pParent);*/
}

CSheetExchMain::~CSheetExchMain()
{
	if( m_dlgTable != NULL )
	{
		m_dlgTable->DestroyWindow();
		delete m_dlgTable;
		m_dlgTable = NULL;
	}
}

void CSheetExchMain::DoDataExchange(CDataExchange* pDX)
{
	//WRITE_LOG ::DoDataExchange(pDX);
	CHSNewWizDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSheetExchMain, CHSNewWizDialog)
	ON_BN_CLICKED(ID_WIZBACK, OnBnClickedWizback)
	ON_BN_CLICKED(ID_WIZNEXT, OnBnClickedWiznext)
	ON_BN_CLICKED(ID_WIZFINISH, OnBnClickedWizfinish)
	ON_BN_CLICKED(ID_EXIT, OnBnClickedExit)
	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA, OnUserDragData)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CSheetExchMain message handlers
BOOL CSheetExchMain::OnInitDialog()
{
	SetPlaceholderID(IDC_PAGEPOS);

	CExchCalcMain* pCalcData = GetData();
	if( pCalcData == NULL )
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}

	// 设置数据指针
	CWizPageExchBase::SetMainTech(pCalcData);
	GetData()->SetMsgWnd(this);
	GetData()->SetMsg(HX_USER_COMPILEDATA);

	// 初始化项面
	CWizPageExchBase* pPage = new CWizPageExchSelIndicator(this);
	if( pPage != NULL )
	{
		AddPage(pPage,IDD_EXCHANGE_SELECT);
	}
	else 
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}
	pPage->InitialData(pCalcData->GetExchExp());

	pPage = new CWizPageExchBuy(this);
	if( pPage != NULL )
	{
		AddPage(pPage,IDD_EXCHANGE_BUY);
	}
	else 
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}
	pPage->InitialData(pCalcData->GetBuy());

	pPage = new CWizPageExchSell(this);
	if( pPage != NULL )
	{
		AddPage(pPage,IDD_EXCHANGE_SELL);
	}
	else 
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}
	pPage->InitialData(pCalcData->GetSell());

	pPage = new CWizPageExchModel(this);
	if( pPage != NULL )
	{
		AddPage(pPage,IDD_EXCHANGE_MODEL);
	}
	else 
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}
	pPage->InitialData(pCalcData->GetMode());

	pPage = new CWizPageExchResult(this);
	if( pPage != NULL )
	{
		AddPage(pPage,IDD_EXCHANGE_RESULT);
	}
	else 
	{
		ASSERT(FALSE);
		return CHSNewWizDialog::OnInitDialog();
	}

	// 初始化进度条，并隐藏
	m_wndPress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(0, 0, 0, 0), this, PROGRESS_ID);
	m_wndPress.ShowWindow(SW_HIDE);

	// 初始化TAB项签 ①②③④⑤⑥⑦⑧⑨⑩
	m_wndPageTab.Create(this, TAB_ID, 5, CRect(0, 0, 0, 0));
	m_wndPageTab.SetItem(0, "①", IDD_EXCHANGE_SELECT);
	m_wndPageTab.SetItem(1, "②", IDD_EXCHANGE_BUY   );
	m_wndPageTab.SetItem(2, "③", IDD_EXCHANGE_SELL  );
	m_wndPageTab.SetItem(3, "④", IDD_EXCHANGE_MODEL );
	m_wndPageTab.SetItem(4, "⑤", IDD_EXCHANGE_RESULT);
	//m_wndPageTab.SetColors(RGB(255, 0, 0), RGB(0, 0, 0), ::GetSysColor(CTLCOLOR_DLG), RGB(0, 255, 0));
	m_wndPageTab.SetCallBreak((HS_Call_Notify)OnMsgPress, (DWORD)m_hWnd);
	m_wndPageTab.SetActive((int)0);

	// 设置位置
	CRect rtClient;

	GetClientRect(&rtClient);
	rtClient.left += 4;
	CWnd* pWnd = GetDlgItem(ID_WIZBACK);
	if( pWnd )
	{
		CRect rtButton;
		pWnd->GetClientRect(&rtButton);
		pWnd->ClientToScreen(&rtButton);
		ScreenToClient(&rtButton);
		rtClient.right = rtButton.left - 8;
		rtClient.top = rtButton.top + 1;
		rtClient.bottom = rtButton.bottom - 1;
	}
	else
	{
		rtClient.bottom -= 10;
		rtClient.right = rtClient.left + 100;
		rtClient.top = rtClient.bottom - 10;
	}

	m_wndPress.MoveWindow(rtClient);
	m_wndPageTab.MoveWindow(rtClient);
	m_wndPageTab.ShowWindow(SW_SHOW);

	return CHSNewWizDialog::OnInitDialog();
}

BOOL CSheetExchMain::DelCaclPage(UINT nIDTemplate)
{
 	try
 	{
 		CHSNewWizPage* pPage;
 		POSITION Pos = m_PageList.GetHeadPosition();
 		POSITION oldPos;
 		while (Pos)
 		{
 			oldPos = Pos;
 			pPage = (CHSNewWizPage*)m_PageList.GetNext(Pos);
 			if( pPage->GetDialogID() ==  nIDTemplate )
 			{
 				m_PageList.RemoveAt(oldPos);
 				pPage->OnKillActive(0);
 				delete pPage;
 				return true;
 			}
 		}
 	}
 	catch(...)
 	{
 		//WRITE_LOG (THIS_FILE, __FILE__, "exception...");
 	}

	return false;
}

void CSheetExchMain::OnBnClickedWizback()
{
	try
	{
		if( DelCaclPage() )
		{
			SetLastPage();
		}
		else
		{
			CHSNewWizDialog::OnWizardBack();
		}
		SetCurTab();
	}
	catch(...)
	{
		////WRITE_LOG (THIS_FILE, __LINE__, "Other exception...");
	}
}

void CSheetExchMain::SetCurTab()
{
 	try
 	{
 		CHSNewWizPage* pCurPage = GetActivePage();
 		if( pCurPage == NULL )
 			return;
 		m_wndPageTab.SetActive(pCurPage->GetDialogID());
 	}
 	catch(...)
 	{
 		//WRITE_LOG (THIS_FILE, __LINE__, "SetCurTab Other exception...");
 	}
}

void CSheetExchMain::SetCurTab(int nAdd)
{
	try
	{
		int iCur = m_wndPageTab.GetCurSel() + nAdd;
		int iCount = m_wndPageTab.GetItemCount();

		if( iCur < 0 )
			iCur = 0;
		if( iCur >= iCount )
			iCur = iCount - 1;

		m_wndPageTab.SetActive((int)iCur);
	}
	catch(...)
	{
		//WRITE_LOG (THIS_FILE, __LINE__, "SetCurTab Other exception...");
	}
}

void CSheetExchMain::OnBnClickedWiznext()
{
	try
	{
		CHSNewWizDialog::OnWizardNext();
		SetCurTab();
	}
	catch(...)
	{
		//WRITE_LOG (THIS_FILE, __LINE__, "Other exception...");
	}
}

void CSheetExchMain::OnBnClickedWizfinish()
{
	Select();
}

void CSheetExchMain::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	if( Stop() != TCaclReturn_Succ )
	{
		if( CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
		{
			MessageBox(_T("停止交易评测计算失败!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
			return;
		}
	}
	CHSNewWizDialog::OnCancel();
}

void CSheetExchMain::PostNcDestroy()
{
	//delete this;
}

void CSheetExchMain::SetStartBom(BOOL bStop)
{
	 //如果优选结果页打开，则为详情定样
 	if( GetPageCount() > 5 )
 	{
 		
 		if( bStop )
 		{			
 			SetFinishText(_T("停止"));
 			m_wndPageTab.ShowWindow(SW_HIDE);
 			m_wndPress.ShowWindow(SW_SHOW);
 		}
 		else
 		{
 			SetFinishText(_T("详情"));
 			m_wndPageTab.ShowWindow(SW_SHOW);
 			m_wndPress.ShowWindow(SW_HIDE);
 		}
 	}
 	else if( bStop )
 	{
 		SetFinishText(_T("停止"));
 		m_wndPageTab.ShowWindow(SW_HIDE);
 		m_wndPress.ShowWindow(SW_SHOW);
 	}
 	else
 	{
 		SetFinishText(_T("执行"));
 		m_wndPress.ShowWindow(SW_HIDE);
 		m_wndPageTab.ShowWindow(SW_SHOW);
 	}
 	m_wndPress.SetPos(0);
}

void CSheetExchMain::ShowExch()
{
	CenterWindow();
	DelCaclPage();
	if( CExchCalcMain::s_uTheradState != TCaclTherad_State_Start )
	{
		SetFirstPage();
		m_wndPageTab.SetActive((int)0);
	}
	ShowWindow(SW_SHOW);
}

void CSheetExchMain::FailDispose(LPARAM lp)
{
	switch( lp )
	{
	case TCaclReturn_NoData		:
		MessageBox(_T("计算失败:没有用于计算的数据!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_VainParam	:
		MessageBox(_T("计算失败:传入无效的参数!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_NoExperss	:
		MessageBox(_T("计算失败:没有指定公式或公式不正确!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_CalFail	:
		MessageBox(_T("计算失败:单支股票的计算不成功!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_MoneryFail	:
		MessageBox(_T("计算失败:系统内存不足或其它内存错误!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_TotalFail	:
		MessageBox(_T("计算失败:对各股票评测结果的统计不成功!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_Stop		:
		//MessageBox("计算失败:计算过程被停止!", FORMULAMAN_EXCH_PROMPT);
		break;
	case TCaclReturn_Fail		:
		MessageBox(_T("计算失败:未知的原因!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	}
}

LRESULT CSheetExchMain::OnExchangeData(WPARAM wp,LPARAM lp)
{
 	switch( wp )
 	{
 	case EXCH_MSG_WP_CACL_FAIL:// 计算失败
 		{
 			SetStartBom(FALSE);
 			GetData()->SetApplySeting(TRUE);
 			FailDispose(lp);
 			GetData()->ResetDefParam();
 		}
 		break;
 	case EXCH_MSG_WP_CACL_STOP:// 停止
 		{
 			SetStartBom(FALSE);
 			GetData()->SetApplySeting(TRUE);
 			GetData()->ResetDefParam();
 		}
 		break;
 	case EXCH_MSG_WP_CACL_TEMPO:// 进度通知
 		{
 			m_wndPress.SetPos(lp);
 		}
 		break;
 	case EXCH_MSG_WP_CACL_START:// 开始
 		{
 			SetStartBom(TRUE);
 			GetData()->SetApplySeting(FALSE);
 			GetData()->FromExpToDefParam();
 		}
 		break;
 
 	case EXCH_MSG_WP_CALC_SET_BUY : // 重新设置BUY方式
 		{
 			CExchCalcMain* pMain = GetData();
 			if( pMain != NULL )
 			{
 				CExpression* pExp = (CExpression*)lp;
 				if( pMain->GetExchExp()->m_pMainTech == pExp || pExp == NULL )
 				{
 					return 0;
 				}
 
 				pMain->GetBuy()->Default(pExp, CTreeCtrlFormula::g_strFirstTechLine);
 				CWizPageExchBuy* pPage = (CWizPageExchBuy*)GetPageByResourceID(IDD_EXCHANGE_BUY);
 				if( pPage != NULL )
 				{
 					pPage->InitialData(pMain->GetBuy());
 					if( ::IsWindow(pPage->GetSafeHwnd()) )
 					{
 						pPage->InitCondition();
 					}
 				}
 			}
 		}
 		break;
 
 	case EXCH_MSG_UP_CACL_SUCC:// 计算完成
 		{
 			GetData()->ResetDefParam();
 
 			if( GetData()->GetOptimize()->GetValidItemCount() > 0 )
 			{
 				// 打开隐含的第六页
 				CWizPageUnionExchCaclResult* pPage = (CWizPageUnionExchCaclResult*)GetPageByResourceID(IDD_EXCHANGE_CACL_RESULT);
 				if( pPage == NULL )
 				{
 					pPage = new CWizPageUnionExchCaclResult(this);
 					AddPage(pPage, IDD_EXCHANGE_CACL_RESULT);
 					SetActivePageByResource(IDD_EXCHANGE_CACL_RESULT);
 					if( !pPage->Initial() )
 					{
 						MessageBox(_T("显示计算结果失败!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
 						return 0;
 					}
 				}
 				else
 				{
 					SetActivePageByResource(IDD_EXCHANGE_CACL_RESULT);
 				}
 				pPage->ClearCell();
 				pPage->PlayData();
 			}
 			else
 			{
 				OpenTableWnd();
 			}
 			SetStartBom(FALSE);
 			GetData()->SetApplySeting(TRUE);
 		}
 		break;
 	}
 
 	CHSNewWizPage* pActivePage = GetActivePage();
 	if( pActivePage != NULL )
 	{
 		pActivePage->SendMessage( HX_USER_COMPILEDATA,wp,lp );
 	}
	return 0;
}

BOOL CSheetExchMain::OpenTableWnd(UINT uID /*= -1*/)
{
	// 打开报表
	if( m_dlgTable == NULL )
	{
		m_dlgTable = new CDlgSingleExchCalcResult(GetParent());
		m_dlgTable->Initial(GetData());
		m_dlgTable->CenterWindow(this);
	}
	m_dlgTable->ShowData(uID);

	return TRUE;
}

LRESULT CSheetExchMain::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
 	CHSNewWizPage* pActivePage = GetActivePage();
 	if( pActivePage != NULL )
 	{
 		pActivePage->SendMessage( HX_USER_DRAGDATA,wParam,lParam );
 	}
	return 0;
}

int CSheetExchMain::Start()
{
	return GetData()->Start();
}

int CSheetExchMain::Stop()
{
	return GetData()->Stop();
}

BOOL CSheetExchMain::IsRunCalc()
{
	if( CExchCalcMain::s_uTheradState != TCaclThread_State_Stop )
	{
		return TRUE;
	}

	return FALSE;
}

int CSheetExchMain::Select()
{
 	try
 	{
 		// 保存设置
 		Apply();
 
 		CWnd* pOKBut = this->GetDlgItem(ID_WIZFINISH);
 		if( pOKBut != NULL )
 		{
 			CString strText;
 			pOKBut->GetWindowText(strText);
 			if( strText.CompareNoCase(_T("停止")) == 0 )
 			{
 				if( Stop() != TCaclReturn_Succ )
 				{
 					if( CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
 					{
 						MessageBox(_T("停止交易评测计算失败!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
 						return TCaclReturn_Fail;
 					}
 				}
 			}		
 			else if(strText.CompareNoCase(_T("执行")) == 0)
 			{
 				if( Start() != TCaclReturn_Succ )
 				{
 					MessageBox(_T("开始交易评测计算失败!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
 					return TCaclReturn_Fail;
 				}
 			}
 			else //if(strText.CompareNoCase("详情") == 0)
 			{
 				CWizPageUnionExchCaclResult* pActivePage = (CWizPageUnionExchCaclResult*)GetPageByResourceID(IDD_EXCHANGE_CACL_RESULT);
 				if( pActivePage != NULL )
 				{
 					pActivePage->OpenDetailed();
 				}
 			}
 			return TCaclReturn_Succ;
 		}
 	}
 #ifdef VC_7_0
 	// 异常处理
 	catch(CMemoryException e) //　内存异常
 	{
 		TCHAR   szCause[255];
 		e.GetErrorMessage(szCause, 255);
 		CString strError;
 		strError.Format("MemoryException: %s", szCause);
 		// 调试日志
 		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
 	}
 #endif
 	catch(...) // 其它异常
 	{
 		// 调试日志
 		CString strError;
 		strError.Format("Other Exception: %s", ::strerror(::GetLastError()));
 		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
 	}

	return TCaclReturn_Fail;
}

int CSheetExchMain::Apply()
{
	int iEnd = GetPageCount();
 	int iRet = 0;
 	int iActive = GetActiveIndex();
 
 	CWizPageExchBase* pPage = (CWizPageExchBase*)GetPage(iActive);
 	if( pPage == NULL )
 		return -1;
 	pPage->SetData();
 	pPage->SaveOption();
	
	return 0;
}

LRESULT CSheetExchMain::OnMsgPress(DWORD dwMsg, WPARAM wp, LPARAM lp)
{
 	if( !::IsWindow((HWND)dwMsg) )
 		return TCaclReturn_Fail;
 
 	CSheetExchMain* pUnion = (CSheetExchMain*)CWnd::FromHandle((HWND)dwMsg);
 	if( pUnion == NULL )
 		return TCaclReturn_Fail;
 
 	int iCurSel = pUnion->GetActiveIndex();
 	pUnion->DelCaclPage();
 	BOOL bRet = FALSE;
 	if( iCurSel > (int)wp )
 	{
 		if( iCurSel < pUnion->GetPageCount() )
 		{
 			CHSNewWizPage* pActivePage = pUnion->GetPage(iCurSel);
 			if( pActivePage->OnWizardBack() == -1 )
 				return -1;
 		}
 		bRet = pUnion->ActivatePage(pUnion->GetPage((int)wp), 0);
 	}
 	else if( iCurSel < (int)wp )
 	{
 		if( iCurSel < pUnion->GetPageCount() )
 		{
 			CHSNewWizPage* pActivePage = pUnion->GetPage(iCurSel);
 			if( pActivePage->OnWizardNext() == -1 )
 				return -1;
 		}
 		bRet = pUnion->ActivatePage(pUnion->GetPage((int)wp), 1);
 	}
 
 	if( bRet == FALSE )
 		return TCaclReturn_Fail;
 	iCurSel = (int)wp;
 	if( iCurSel <= 0 )
 	{
 		pUnion->EnableBack(FALSE);
 		pUnion->EnableNext(TRUE);
 		pUnion->EnableFinish(FALSE);
 	}
 	else if( iCurSel >= (pUnion->GetPageCount()-1) )
 	{
 		pUnion->EnableBack(TRUE);
 		pUnion->EnableNext(FALSE);
 		pUnion->EnableFinish(TRUE);
 	}
 	else
 	{
 		pUnion->EnableBack(TRUE);
 		pUnion->EnableNext(TRUE);
 		pUnion->EnableFinish(FALSE);
 	}

	return TCaclReturn_Succ;
}

BOOL CSheetExchMain::PreTranslateMessage(MSG* pMsg)
 {
 	CHSNewWizPage* pActivePage = GetActivePage();
 	if( pActivePage != NULL )
 	{
 		if( pActivePage->PreTranslateMessage(pMsg) )
 			return TRUE;
 	}
	return CHSNewWizDialog::PreTranslateMessage(pMsg);
}

void CSheetExchMain::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	if( nChar == VK_RETURN )
	{
		CWnd* pWnd = GetDlgItem(ID_WIZNEXT);
		if( pWnd )
		{
			if( pWnd->IsWindowEnabled() )
				OnBnClickedWiznext();
			else
				OnBnClickedWizfinish();
			return;
		}
	}

	CHSNewWizDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
