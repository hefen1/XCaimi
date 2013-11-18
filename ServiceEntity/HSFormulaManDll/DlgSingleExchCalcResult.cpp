// DlgSingleExchCalcResult.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSingleExchCalcResult.h"
#include "SheetExchMain.h"

#include "resource.h"


// CDlgSingleExchCalcResult dialog
#define TAB_ITEM_ID_SUMARY		"摘要"
#define TAB_ITEM_ID_PARTICULAR	"详细"
#define TAB_ITEM_ID_Flow		"明细"
#define TAB_ITEM_ID_DIS			"分布"
#define TAB_ITEM_ID_Profit		"收益"
#define TAB_ITEM_ID_Seting		"设定"
#define TAB_ITEM_ID_Prompt		"提示"

const CHAR _tabItem[][6] = { TAB_ITEM_ID_SUMARY, TAB_ITEM_ID_PARTICULAR, 
								TAB_ITEM_ID_Flow,/*TAB_ITEM_ID_DIS,TAB_ITEM_ID_Profit,*/
								TAB_ITEM_ID_Seting, TAB_ITEM_ID_Prompt};

IMPLEMENT_DYNAMIC(CDlgSingleExchCalcResult, CDialog)
CDlgSingleExchCalcResult::CDlgSingleExchCalcResult(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSingleExchCalcResult::IDD, pParent)
{
	m_pwndBMP = NULL;
	m_pCaclData = NULL;
	m_uParamID = 0;
	Create(IDD_EXCH_CACLTABLE, pParent);
}

CDlgSingleExchCalcResult::~CDlgSingleExchCalcResult()
{
}

void CDlgSingleExchCalcResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_wndTab);
	DDX_Control(pDX, IDC_RECT_LIST, m_wndFlowDeal);
	DDX_Control(pDX, IDC_STOCKLIST, m_wndList);
}


BEGIN_MESSAGE_MAP(CDlgSingleExchCalcResult, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(HDN_ITEMCLICK, IDC_STOCKLIST, OnHdnItemclickStocklist)
	ON_NOTIFY(NM_CLICK, IDC_STOCKLIST, OnNMClickStocklist)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnTcnSelchangingTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_SAVE_PARAM, OnBnClickedSaveParam)
	ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
END_MESSAGE_MAP()


// CDlgSingleExchCalcResult message handlers

BOOL CDlgSingleExchCalcResult::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSingleExchCalcResult::ShowData(UINT uID)
{
	if( m_pCaclData == NULL )
		return;

	m_uParamID = uID;

	TExchParam* pParam = m_pCaclData->GetCurParam(uID);
	if( pParam == NULL )
		return;
	CString strTech = m_pCaclData->m_sExchExp.GetExpTypeName();
	strTech += "-";
	strTech += m_pCaclData->m_sExchExp.m_pMainTech->GetName();
	strTech += pParam->ToStr();
	strTech += "-";
	strTech += _T("评测结果");
	SetWindowText(strTech);

	m_wndTab.SetCurSel(0);
	PlayStock(uID);
	m_wndList.SetSelectionMark(0);
	PlayData(uID);
	ShowWindow(SW_SHOW);
}

long CDlgSingleExchCalcResult::PlayStock(UINT uID)
{
	if( m_pCaclData == NULL )
		return -1;

	if( uID == -1 )
	{
		uID = m_uParamID;
	}
	TCaclResult* pCacl = m_pCaclData->GetOneResult(uID);
	if( pCacl != NULL )
	{
		pCacl->ToListCtrl(&m_wndList);
	}
	return 1;
}

long CDlgSingleExchCalcResult::Initial(CExchCalcMain* pData, UINT uID)
{
	m_uParamID = uID;
    CString str;
	for( int i = 0; i < _countof(_tabItem); i++ )
	{
		str =  _T(_tabItem[i]);
		m_wndTab.InsertItem(i, (char*)LPCTSTR(str));
	}

	if( pData == NULL )
	{
		m_pCaclData = CExchCalcMain::s_pThis;
	}
	else
	{
		m_pCaclData = pData;
	}

	if( m_pCaclData == NULL )
		return -1;

	TDealFlow *pFlow = m_pCaclData->GetDealFlow();
	if( pFlow != NULL )
		pFlow->Initital(&m_wndFlowDeal, &m_pCaclData->m_oImage);

	TCaclResult* pCacl = m_pCaclData->GetOneResult();
	if( pCacl != NULL )
	{
		pCacl->Initital(&m_wndList, &m_pCaclData->m_oImage);
	}
	m_wndResult.Create(this, IDC_RICH_ID);
	m_wndResult.InitialTip();

	SetSize();
	return PlayData(uID);
}

long CDlgSingleExchCalcResult::SetSize()
{
	if( m_pCaclData == NULL )
		return -1;

	TExchModel* pModel = m_pCaclData->GetMode();
	if( pModel == NULL )
		return -1;

	CRect rcClient;
	CRect rcFlow;
	CRect rcStockList;

	GetClientRect(rcClient);
	m_wndFlowDeal.GetWindowRect(rcFlow);
	ScreenToClient(rcFlow);
	rcFlow.right = rcClient.right - 6;

	m_wndList.GetWindowRect(rcStockList);
	ScreenToClient(rcStockList);
	rcStockList.right = rcClient.right - 6;
	
	CWnd* pWnd = NULL;

	int iCurTab = m_wndTab.GetCurSel();
	if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_SUMARY) == 0
		|| stricmp(_tabItem[iCurTab], TAB_ITEM_ID_PARTICULAR) == 0
		|| stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Seting) == 0
		|| stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Prompt) == 0)
	{
		pWnd = &m_wndResult;
		m_wndFlowDeal.ShowWindow(SW_HIDE);
		if( m_pwndBMP != NULL && ::IsWindow(m_pwndBMP->m_hWnd) )
			m_pwndBMP->ShowWindow(SW_HIDE);
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Flow) == 0 )
	{
		pWnd = &m_wndFlowDeal;
		m_wndResult.ShowWindow(SW_HIDE);
		if( m_pwndBMP != NULL && ::IsWindow(m_pwndBMP->m_hWnd) )
			m_pwndBMP->ShowWindow(SW_HIDE);
	}
	else
	{
		m_wndResult.ShowWindow(SW_HIDE);
		m_wndFlowDeal.ShowWindow(SW_HIDE);
		pWnd = m_pwndBMP;
	}

	if( pModel->m_ayStkList.GetSize() > 1 )
	{
		m_wndList.ShowWindow(SW_SHOW);
		rcFlow.bottom = rcClient.bottom - rcStockList.Height() - 10;
		rcStockList.bottom = rcClient.bottom - 5;
		rcStockList.top = rcFlow.bottom + 5;
		m_wndList.MoveWindow(rcStockList);
	}
	else
	{
		rcFlow.bottom = rcClient.bottom - 5;
		m_wndList.ShowWindow(SW_HIDE);
	}
	if( pWnd != NULL && ::IsWindow(pWnd->m_hWnd) )
	{
		pWnd->MoveWindow(rcFlow);
		pWnd->ShowWindow(SW_SHOW);
	}

	return 1;
}

long CDlgSingleExchCalcResult::PlayData(UINT uID)
{
	if( uID == -1 )
	{
		uID = m_uParamID;
	}

	int iCurTab = m_wndTab.GetCurSel();
	int iCurStock = m_wndList.GetSelectionMark()-1;

	if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_SUMARY) == 0 )
	{
		m_wndResult.Display(m_pCaclData, iCurStock, uID,TRUE);
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_PARTICULAR) == 0 )
	{
		m_wndResult.Display(m_pCaclData, iCurStock, uID, FALSE);
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Prompt) == 0 )
	{
		m_wndResult.Display();
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Seting) == 0)
	{
		m_wndResult.Display(m_pCaclData, FALSE, uID);
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Flow) == 0 )
	{
		if( m_pCaclData != NULL )
		{
			m_pCaclData->GetDealFlow(uID)->ToListCtrl(&m_wndFlowDeal, iCurStock);
		}
	}
	else if( stricmp(_tabItem[iCurTab], TAB_ITEM_ID_Profit) == 0 )
	{
		if( m_pwndBMP != NULL )
			m_pwndBMP->RedrawWindow();
	}
	else 
	{
		if( m_pwndBMP != NULL )
			m_pwndBMP->RedrawWindow();
	}
	SetSize();

	return 1;
}

void CDlgSingleExchCalcResult::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	SetSize();
}

void CDlgSingleExchCalcResult::OnHdnItemclickStocklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	*pResult = 0;
}

void CDlgSingleExchCalcResult::OnNMClickStocklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	PlayData();
}

void CDlgSingleExchCalcResult::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgSingleExchCalcResult::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	PlayData();
}

void CDlgSingleExchCalcResult::OnBnClickedSaveParam()
{
	ShowWindow(SW_HIDE);
	CWnd* pParent = GetParent();
	if( pParent != NULL )
	{
		pParent->SetFocus();
	}
}

void CDlgSingleExchCalcResult::OnBnClickedSave()
{
	if( m_pCaclData != NULL )
	{
		m_pCaclData->Save(this);
	}
}

void CDlgSingleExchCalcResult::Open()
{
	if( m_pCaclData != NULL )
	{
		m_pCaclData->Open(this);
	}
	ShowData();
}


void CDlgSingleExchCalcResult::OnCancel()
{
	ShowWindow(SW_HIDE);
	CWnd* pParent = GetParent();
	if( pParent != NULL )
	{
		pParent->SetFocus();
	}
	//WRITE_LOG ::OnCancel();
}
