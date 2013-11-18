/*******************************************************
  源程序名称:TradeStockSBBuyCDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  三板确定买入
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockSBBuyCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSBBuyCDlg, CTradeStockBuyDlg)

CTradeStockSBBuyCDlg::CTradeStockSBBuyCDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBuyDlg(pParent)
{

}

CTradeStockSBBuyCDlg::~CTradeStockSBBuyCDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockSBBuyCDlg, CTradeStockBuyDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSBBuyCDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "c";
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSBBuyCDlg::LoadWnd()
{	
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_paCWnd.Add(&m_stPrice);
	m_paCWnd.Add(&m_stEnableBalance);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	m_paCWnd.Add(&m_stSeatNo);
	m_paCWnd.Add(&m_stConferNo);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_paCWnd.Add(&m_edPrice);
	m_paCWnd.Add(&m_edEnableBalance);
	m_paCWnd.Add(&m_edEnableAmount);
	m_paCWnd.Add(&m_edAmount);
	m_paCWnd.Add(&m_edSeatNo);
	m_paCWnd.Add(&m_edConferNo);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_edPrice, &m_spPrice, 0, -1);
	AdjustLayoutEx(&m_edAmount, &m_spAmount, 0, -1);
	AdjustLayoutEx(&m_edConferNo, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbStockAccount);
	m_cbStockAccount.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edStockCode.SetWindowPos(&m_cbStockAccount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edPrice.SetWindowPos(&m_edStockCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edAmount.SetWindowPos(&m_edPrice, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edSeatNo.SetWindowPos(&m_edAmount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edConferNo.SetWindowPos(&m_edSeatNo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}

