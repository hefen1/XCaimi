/*******************************************************
  源程序名称:TradeStockFundTransDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金盘后业务转换
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockFundTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockFundTransDlg, CTradeStockSellDlg)

CTradeStockFundTransDlg::CTradeStockFundTransDlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{

}

CTradeStockFundTransDlg::~CTradeStockFundTransDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockFundTransDlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockFundTransDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_stPrice.ShowWindow(SW_HIDE);
		m_edPrice.ShowWindow(SW_HIDE);
		m_spPrice.ShowWindow(SW_HIDE);
		m_stEnableAmount.ShowWindow(SW_HIDE);
		m_edEnableAmount.ShowWindow(SW_HIDE);
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockFundTransDlg::LoadWnd()
{	
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	//m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stTransCode);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	//m_paCWnd.Add(&m_edEnableAmount);
	m_paCWnd.Add(&m_edTransCode);
	m_paCWnd.Add(&m_edAmount);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_edAmount, &m_spAmount, 0, -1);
	AdjustLayoutEx(&m_spAmount, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbStockAccount);
	m_cbStockAccount.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edStockCode.SetWindowPos(&m_cbStockAccount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edTransCode.SetWindowPos(&m_edStockCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edAmount.SetWindowPos(&m_edTransCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edAmount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}

