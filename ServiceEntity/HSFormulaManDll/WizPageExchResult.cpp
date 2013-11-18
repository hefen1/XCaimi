// WizPageExchResult.cpp : implementation file
//

#include "stdafx.h"

#include "WizPageExchResult.h"
#include "SheetExchMain.h"
#include "DlgExchParamSet.h"
#include "WizPageExchSell.h"

// CWizPageExchResult dialog

//IMPLEMENT_DYNAMIC(CWizPageExchResult, CDialog)
CWizPageExchResult::CWizPageExchResult(CWnd* pParent /*=NULL*/)
	: CWizPageExchBase(CWizPageExchResult::IDD, pParent)
{
	m_pData = &m_sLocalOptimize;
}

CWizPageExchResult::~CWizPageExchResult()
{
}

void CWizPageExchResult::DoDataExchange(CDataExchange* pDX)
{
	CWizPageExchBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizPageExchResult, CWizPageExchBase)
	ON_BN_CLICKED(IDB_MODEFY, OnBnClickedModefy)
END_MESSAGE_MAP()


// CWizPageExchResult message handlers

BOOL CWizPageExchResult::OnInitDialog()
{
	CWizPageExchBase::OnInitDialog();

	Initial();
	PlayData();
	return TRUE; 
}

void CWizPageExchResult::OnSetActive()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	m_sLocalOptimize = *m_pMainTech->GetOptimize();
	if( pParent != NULL )
	{
		CString str(_T("系统评测-"));
		if( m_pMainTech == NULL )
			m_pMainTech = pParent->GetData();
		if( m_pMainTech == NULL )
			return;
		CExpression *pExp = m_pMainTech->GetExp();
		if( pExp != NULL )
		{
			
			str += pExp->GetName();
			str += "-";
		}
		str += _T("测试设置");
		pParent->SetWindowText(str);
	}
	PlayData();
}

BOOL CWizPageExchResult::OnKillActive(int nPos /*= -1*/)
{
	if(nPos < 0) return TRUE;

	SaveOption();
	return TRUE;
}

long CWizPageExchResult::PlayData()
{
	return m_wndResult.Display(m_pMainTech, TRUE, 0, &m_sLocalOptimize);
}

long CWizPageExchResult::Initial()
{
	CSheetExchMain* pParent = (CSheetExchMain*)GetParent();
	if( pParent == NULL )
		return -1;

	if( m_pMainTech == NULL )
		m_pMainTech = pParent->GetData();
	if( m_pMainTech == NULL )
		return -1;

	m_sLocalOptimize = *m_pMainTech->GetOptimize();

	m_wndResult.Create(this, IDC_RESULT_ID);
	CWnd* pWnd = GetDlgItem(IDC_RESULT);

	CRect rcResult;
	if( pWnd != NULL )
	{
		pWnd->GetWindowRect(rcResult);
	}
	else
	{
		GetWindowRect(rcResult);
		rcResult += CRect(2, 2, -2, -2); 
	}
	ScreenToClient(rcResult);
	m_wndResult.MoveWindow(rcResult);
	m_wndResult.ShowWindow(SW_SHOW);

	return 1;
}

void CWizPageExchResult::OnBnClickedModefy()
{
	if( m_pMainTech == NULL )
	{
		MessageBox(_T("内部数据设置错误!"), FORMULAMAN_EXCH_PROMPT, MB_OK);
		return ;
	}
	CDlgExchParamSet dlg(&m_sLocalOptimize, this);
	if( dlg.DoModal() != IDOK )
	{
		return;
	}

	PlayData();
}