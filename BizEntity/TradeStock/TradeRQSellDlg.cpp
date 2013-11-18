/*******************************************************
  源程序名称:TradeRQSellDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券融券卖出
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeRQSellDlg.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeRQSellDlg, CTradeStockBuyDlg)

CTradeRQSellDlg::CTradeRQSellDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBuyDlg(pParent)
{
	m_nQueryEnableAction = TRADE_RZRQ_QUERYMAXSELL;
	m_sEntrustType = "7";
}

CTradeRQSellDlg::~CTradeRQSellDlg()
{

}

void CTradeRQSellDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeRQSellDlg, CTradeStockBuyDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeRQSellDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_eBSDirection = bsSell;
		m_stEnableAmount.SetWindowText(_T("可卖数量"));
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeRQSellDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_paCWnd.Add(&m_stPrice);
	//m_paCWnd.Add(&m_stEnableBalance);
	m_stEnableBalance.ShowWindow(SW_HIDE);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_paCWnd.Add(&m_edPrice);
	//m_paCWnd.Add(&m_edEnableBalance);
	m_edEnableBalance.ShowWindow(SW_HIDE);
	m_paCWnd.Add(&m_edEnableAmount);
	m_paCWnd.Add(&m_edAmount);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_edPrice, &m_spPrice, 0, -1);
	AdjustLayoutEx(&m_edAmount, &m_spAmount, 0, -1);
	AdjustLayoutEx(&m_spAmount, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbStockAccount);
	m_cbStockAccount.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edStockCode.SetWindowPos(&m_cbStockAccount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edPrice.SetWindowPos(&m_edStockCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edAmount.SetWindowPos(&m_edPrice, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edAmount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}