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

	// ��������ָ��
	CWizPageExchBase::SetMainTech(pCalcData);
	GetData()->SetMsgWnd(this);
	GetData()->SetMsg(HX_USER_COMPILEDATA);

	// ��ʼ������
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

	// ��ʼ����������������
	m_wndPress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(0, 0, 0, 0), this, PROGRESS_ID);
	m_wndPress.ShowWindow(SW_HIDE);

	// ��ʼ��TAB��ǩ �٢ڢۢܢݢޢߢ���
	m_wndPageTab.Create(this, TAB_ID, 5, CRect(0, 0, 0, 0));
	m_wndPageTab.SetItem(0, "��", IDD_EXCHANGE_SELECT);
	m_wndPageTab.SetItem(1, "��", IDD_EXCHANGE_BUY   );
	m_wndPageTab.SetItem(2, "��", IDD_EXCHANGE_SELL  );
	m_wndPageTab.SetItem(3, "��", IDD_EXCHANGE_MODEL );
	m_wndPageTab.SetItem(4, "��", IDD_EXCHANGE_RESULT);
	//m_wndPageTab.SetColors(RGB(255, 0, 0), RGB(0, 0, 0), ::GetSysColor(CTLCOLOR_DLG), RGB(0, 255, 0));
	m_wndPageTab.SetCallBreak((HS_Call_Notify)OnMsgPress, (DWORD)m_hWnd);
	m_wndPageTab.SetActive((int)0);

	// ����λ��
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
			MessageBox(_T("ֹͣ�����������ʧ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
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
	 //�����ѡ���ҳ�򿪣���Ϊ���鶨��
 	if( GetPageCount() > 5 )
 	{
 		
 		if( bStop )
 		{			
 			SetFinishText(_T("ֹͣ"));
 			m_wndPageTab.ShowWindow(SW_HIDE);
 			m_wndPress.ShowWindow(SW_SHOW);
 		}
 		else
 		{
 			SetFinishText(_T("����"));
 			m_wndPageTab.ShowWindow(SW_SHOW);
 			m_wndPress.ShowWindow(SW_HIDE);
 		}
 	}
 	else if( bStop )
 	{
 		SetFinishText(_T("ֹͣ"));
 		m_wndPageTab.ShowWindow(SW_HIDE);
 		m_wndPress.ShowWindow(SW_SHOW);
 	}
 	else
 	{
 		SetFinishText(_T("ִ��"));
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
		MessageBox(_T("����ʧ��:û�����ڼ��������!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_VainParam	:
		MessageBox(_T("����ʧ��:������Ч�Ĳ���!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_NoExperss	:
		MessageBox(_T("����ʧ��:û��ָ����ʽ��ʽ����ȷ!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_CalFail	:
		MessageBox(_T("����ʧ��:��֧��Ʊ�ļ��㲻�ɹ�!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_MoneryFail	:
		MessageBox(_T("����ʧ��:ϵͳ�ڴ治��������ڴ����!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_TotalFail	:
		MessageBox(_T("����ʧ��:�Ը���Ʊ��������ͳ�Ʋ��ɹ�!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	case TCaclReturn_Stop		:
		//MessageBox("����ʧ��:������̱�ֹͣ!", FORMULAMAN_EXCH_PROMPT);
		break;
	case TCaclReturn_Fail		:
		MessageBox(_T("����ʧ��:δ֪��ԭ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		break;
	}
}

LRESULT CSheetExchMain::OnExchangeData(WPARAM wp,LPARAM lp)
{
 	switch( wp )
 	{
 	case EXCH_MSG_WP_CACL_FAIL:// ����ʧ��
 		{
 			SetStartBom(FALSE);
 			GetData()->SetApplySeting(TRUE);
 			FailDispose(lp);
 			GetData()->ResetDefParam();
 		}
 		break;
 	case EXCH_MSG_WP_CACL_STOP:// ֹͣ
 		{
 			SetStartBom(FALSE);
 			GetData()->SetApplySeting(TRUE);
 			GetData()->ResetDefParam();
 		}
 		break;
 	case EXCH_MSG_WP_CACL_TEMPO:// ����֪ͨ
 		{
 			m_wndPress.SetPos(lp);
 		}
 		break;
 	case EXCH_MSG_WP_CACL_START:// ��ʼ
 		{
 			SetStartBom(TRUE);
 			GetData()->SetApplySeting(FALSE);
 			GetData()->FromExpToDefParam();
 		}
 		break;
 
 	case EXCH_MSG_WP_CALC_SET_BUY : // ��������BUY��ʽ
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
 
 	case EXCH_MSG_UP_CACL_SUCC:// �������
 		{
 			GetData()->ResetDefParam();
 
 			if( GetData()->GetOptimize()->GetValidItemCount() > 0 )
 			{
 				// �������ĵ���ҳ
 				CWizPageUnionExchCaclResult* pPage = (CWizPageUnionExchCaclResult*)GetPageByResourceID(IDD_EXCHANGE_CACL_RESULT);
 				if( pPage == NULL )
 				{
 					pPage = new CWizPageUnionExchCaclResult(this);
 					AddPage(pPage, IDD_EXCHANGE_CACL_RESULT);
 					SetActivePageByResource(IDD_EXCHANGE_CACL_RESULT);
 					if( !pPage->Initial() )
 					{
 						MessageBox(_T("��ʾ������ʧ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
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
	// �򿪱���
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
 		// ��������
 		Apply();
 
 		CWnd* pOKBut = this->GetDlgItem(ID_WIZFINISH);
 		if( pOKBut != NULL )
 		{
 			CString strText;
 			pOKBut->GetWindowText(strText);
 			if( strText.CompareNoCase(_T("ֹͣ")) == 0 )
 			{
 				if( Stop() != TCaclReturn_Succ )
 				{
 					if( CExchCalcMain::s_uTheradState != TCaclThread_State_Stoping )
 					{
 						MessageBox(_T("ֹͣ�����������ʧ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
 						return TCaclReturn_Fail;
 					}
 				}
 			}		
 			else if(strText.CompareNoCase(_T("ִ��")) == 0)
 			{
 				if( Start() != TCaclReturn_Succ )
 				{
 					MessageBox(_T("��ʼ�����������ʧ��!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
 					return TCaclReturn_Fail;
 				}
 			}
 			else //if(strText.CompareNoCase("����") == 0)
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
 	// �쳣����
 	catch(CMemoryException e) //���ڴ��쳣
 	{
 		TCHAR   szCause[255];
 		e.GetErrorMessage(szCause, 255);
 		CString strError;
 		strError.Format("MemoryException: %s", szCause);
 		// ������־
 		//WRITE_LOG (__FILE__, __LINE__, _T(strError));
 	}
 #endif
 	catch(...) // �����쳣
 	{
 		// ������־
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
