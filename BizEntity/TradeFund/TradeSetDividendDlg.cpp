/*******************************************************
  源程序名称:TradeSetDividendDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易设置分红方式
  作    者:  shenglq
  开发日期:  20110420
*********************************************************/
#include "stdafx.h"
#include "TradeSetDividendDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeSetDividendDlg, CTradeFundBaseDlg)

CTradeSetDividendDlg::CTradeSetDividendDlg(CWnd* pParent /* = NULL */)
	: CTradeFundBaseDlg(pParent)
{

}

CTradeSetDividendDlg::~CTradeSetDividendDlg()
{

}


BOOL CTradeSetDividendDlg::OnInit()
{
	if (CTradeFundBaseDlg::OnInit())
	{
		//m_stBalance.SetWindowText(_T("申购金额"));
		return TRUE;
	}
	else
		return FALSE;
}


void CTradeSetDividendDlg::OnFundCodeExit()
{
	//QueryEnableBalance();
	__super::OnFundCodeExit();	
}

BOOL CTradeSetDividendDlg::Validate()
{
	if (__super::Validate())
	{
		CString sHint = "";
		CString sTmp = "";
		CString sTmp1 = "";
		CString sTmp2 = "";	
		m_edFundCode.GetWindowText(sTmp1);
		m_edFundName.GetWindowText(sTmp2);
		sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_FUND_CODENAME), sTmp1, sTmp2);
		sHint += sTmp;
		m_cbDividendMethod.GetWindowText(sTmp1);
		sTmp.Format("分红方式：%s\r\n", sTmp1);
		sHint += sTmp;
		sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
		BOOL bReturn = MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
		m_btOK.SetFocus();
		return bReturn;
	}
	else
		return FALSE;
}

void CTradeSetDividendDlg::DoEntrust()
{
	CString sFundCode = "";
	m_edFundCode.GetWindowText(sFundCode);
	//double dBalance = m_edBalance.GetDouble();
	CString sMsg = "";
	CString sFundCompany = "";
	CString sChargetype = "";
	CString sDividendMethod = "";
	if (m_FundInfoItem)
	{
		sFundCompany = m_FundInfoItem->GetFundCompany();
		sChargetype = m_FundInfoItem->GetChargeType();
	}
	int nIndex = m_cbDividendMethod.GetCurSel();
	INodeData* pItem = m_pDividendMethod->GetItems(nIndex);
	sDividendMethod = pItem->Value();
	pItem->Release();
	if (m_pBusinessLogic->FundSetDividend(sFundCompany, sFundCode, sDividendMethod, sChargetype, sMsg))
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		PostRefreshMsg();
		Clear();
	}
	else
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
	m_sLastFundCode = "";
	m_edFundCode.SetFocus();
}

void CTradeSetDividendDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeSetDividendDlg, CTradeFundBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeSetDividendDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stFundCode);
	m_paCWnd.Add(&m_stFundName);
	m_paCWnd.Add(&m_stFundNav);
	m_paCWnd.Add(&m_stDividendMethod);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_edFundCode);
	m_paCWnd.Add(&m_edFundName);
	m_paCWnd.Add(&m_edFundNav);
	m_paCWnd.Add(&m_cbDividendMethod);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_cbDividendMethod, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbDividendMethod);
	m_edFundCode.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_cbDividendMethod.SetWindowPos(&m_edFundCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_cbDividendMethod, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}