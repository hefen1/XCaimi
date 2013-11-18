/*******************************************************
  源程序名称:TradeStockBuyDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券买入
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include <math.h>
#include "TradeStockBuyDlg.h"
#include "QueryFundIntf.h"
#include "QueryStockMaxBuyIntf.h"
#include "TradeAction.h"
#include "TradeConst.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockBuyDlg, CTradeStockBaseDlg)

CTradeStockBuyDlg::CTradeStockBuyDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBaseDlg(pParent)
{
	m_nQueryEnableAction = TRADE_STOCK_QUERYMAXBUY;
}

CTradeStockBuyDlg::~CTradeStockBuyDlg()
{

}

BOOL CTradeStockBuyDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_eBSDirection = bsBuy;
		//CRect  rect;
		/*m_stEnableBalance.ShowWindow(SW_SHOW);
		m_edEnableBalance.ShowWindow(SW_SHOW);
		m_stEnableAmount.ShowWindow(SW_SHOW);
		m_stEnableAmount.SetWindowText(_T("可买数量"))
		m_edEnableAmount.ShowWindow(SW_SHOW);*/
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockBuyDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	switch (iAction)
	{
	case TRADE_QUERYFUND:
		{
			m_edEnableBalance.SetWindowText("");
			IReqQueryFund* pIntf = dynamic_cast<IReqQueryFund*>(pTrading);
			if (pIntf->GetSuccessful() && pIntf->HaveData())
			{
				IFundList* pList = pIntf->GetDataList();
				IFundItem* pItem = pList->GetItems(0);
				CString sTmp = "";
				sTmp.Format("%.2f", pItem->GetEnableBalance());
				m_edEnableBalance.SetWindowText(sTmp);			
			}
			pTrading->FreeDataList();
			break;
		}
	case TRADE_STOCK_QUERYMAXBUY:
	case TRADE_RZRQ_QUERYMAXBUY:
	case TRADE_RZRQ_QUERYMAXSELL:
		{
			m_edEnableAmount.SetWindowText("");
			IReqQueryStockMaxBuy* pIntf = dynamic_cast<IReqQueryStockMaxBuy*>(pTrading);
			if (pIntf->GetSuccessful() && pIntf->HaveData())
			{
				IStockMaxBuyList* pList = pIntf->GetDataList();
				IStockMaxBuyItem* pItem = pList->GetItems(0);
				CString sTmp = "";
				sTmp.Format("%d", pItem->GetEnableAmount());
				m_edEnableAmount.SetWindowText(sTmp);			
			}
			pTrading->FreeDataList();
			break;
		}
	}
}

void CTradeStockBuyDlg::QueryEnableBalance()
{
	if (m_stEnableBalance.IsWindowVisible() && (m_edStockCode.GetWindowTextLength() == STOCKCODE_LENGTH))
	{
		CString sStockCode = "";
		m_edStockCode.GetWindowText(sStockCode);
		if (sStockCode == m_sLastStockCode)
		{
			return;
		}
		IReqQueryFund* pIntf = dynamic_cast<IReqQueryFund*>(m_pTradeCoreObj->NewTradingByAction(TRADE_QUERYFUND));
		pIntf->SetAccount(m_pAccount);
		int iStockIndex = m_cbStockAccount.GetCurSel();
		IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
		CString sExchangeType = pItem->GetExchangeType();	
		ITradeMarketItem* pTradeMarketItem = m_pTradeMarketList->GetItemsByName(sExchangeType);
		if (pTradeMarketItem)
		{
			pIntf->SetMoneyType(pTradeMarketItem->GetMoneyType());
			pTradeMarketItem->Release();
		}
		else
			pIntf->SetMoneyType(mtCNY);		
		pIntf->TradingAsync(this);
	}
}

void CTradeStockBuyDlg::QueryEnableAmount()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
		return;
	double dPrice = m_edPrice.GetDouble();
	if (dPrice < ENTRUST_MINPRICE)
	{
		return;
	}
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	if ((sStockCode == m_sLasStockCodePriceExit)&& ((fabs(dPrice - m_dLastPrice)) < ENTRUST_MINPRICE))
	{
		return;
	}	
	
	IReqQueryStockMaxBuy* pIntf = dynamic_cast<IReqQueryStockMaxBuy*>(m_pTradeCoreObj->NewTradingByAction(m_nQueryEnableAction));
	pIntf->SetAccount(m_pAccount);
	int iStockIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
	CString sStockAccount = pItem->GetStockAccount();
	CString sExchangeType = pItem->GetExchangeType();
	pIntf->SetExchangeType(sExchangeType);
	pIntf->SetStockAccount(sStockAccount);
	pIntf->SetStockCode(sStockCode);
	pIntf->SetEntrustPrice(dPrice);
	pIntf->SetEntrustProp(GetEntrustProp());
	pIntf->SetEntrustType(m_sEntrustType);
	pIntf->TradingAsync(this);
}

void CTradeStockBuyDlg::OnStockCodeExit()
{
	QueryEnableBalance();
}

void CTradeStockBuyDlg::OnEntrustPriceExit()
{
	QueryEnableAmount();
}

void CTradeStockBuyDlg::DoEntrust()
{
	__super::DoEntrust();
}

void CTradeStockBuyDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStockBuyDlg, CTradeStockBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeStockBuyDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_paCWnd.Add(&m_stPrice);
	m_paCWnd.Add(&m_stEnableBalance);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_paCWnd.Add(&m_edPrice);
	m_paCWnd.Add(&m_edEnableBalance);
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