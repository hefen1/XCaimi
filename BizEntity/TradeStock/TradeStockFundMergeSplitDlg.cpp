/*******************************************************
  Դ��������:TradeStockFundMergeSplitDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  �����̺�ҵ��ϲ����
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockFundMergeSplitDlg.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockFundMergeSplitDlg, CTradeStockSellDlg)

CTradeStockFundMergeSplitDlg::CTradeStockFundMergeSplitDlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{
	m_nQueryEnableAction = TRADE_STOCK_QUERYMERGESPLIT;
}

CTradeStockFundMergeSplitDlg::~CTradeStockFundMergeSplitDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockFundMergeSplitDlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockFundMergeSplitDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_stPrice.ShowWindow(SW_HIDE);
		m_edPrice.ShowWindow(SW_HIDE);
		m_spPrice.ShowWindow(SW_HIDE);
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockFundMergeSplitDlg::LoadWnd()
{	
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_paCWnd.Add(&m_edEnableAmount);
	m_paCWnd.Add(&m_edAmount);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_edAmount, &m_spAmount, 0, -1);
	AdjustLayoutEx(&m_spAmount, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbStockAccount);
	m_cbStockAccount.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edStockCode.SetWindowPos(&m_cbStockAccount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edAmount.SetWindowPos(&m_edStockCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edAmount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}
