/*******************************************************
  源程序名称:TradeStockSellDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券买入
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeStockSellDlg.h"
#include "QueryStockMaxSellIntf.h"
#include "TradeAction.h"
#include "TradeConst.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSellDlg, CTradeStockBaseDlg)

CTradeStockSellDlg::CTradeStockSellDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBaseDlg(pParent)
{
	m_nQueryEnableAction = TRADE_STOCK_QUERYMAXSELL;
	m_eBSDirection = bsSell;
}

CTradeStockSellDlg::~CTradeStockSellDlg()
{

}

BOOL CTradeStockSellDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_stEnableBalance.ShowWindow(SW_HIDE);
		m_edEnableBalance.ShowWindow(SW_HIDE);
		m_stEnableAmount.SetWindowText(_T("可用数量"));		
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSellDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	switch (iAction)
	{
	case TRADE_STOCK_QUERYMAXSELL:
	case TRADE_STOCK_QUERYMERGESPLIT:
	case TRADE_RZRQ_QUERYRQAMOUNT:
		{
			m_edEnableAmount.SetWindowText("");
			IReqQueryStockMaxSell* pIntf = dynamic_cast<IReqQueryStockMaxSell*>(pTrading);
			if (pIntf->GetSuccessful() && pIntf->HaveData())
			{
				IStockMaxSellList* pList = pIntf->GetDataList();
				IStockMaxSellItem* pItem = pList->GetItems(0);
				CString sTmp = "";
				if (TRADE_RZRQ_QUERYRQAMOUNT == iAction)
				{
					sTmp.Format("%d", pItem->GetEnableAmount());
					m_edEnableBalance.SetWindowText(sTmp);	
					sTmp.Format("%d", pItem->GetShortsellDebitAmount());
					m_edEnableAmount.SetWindowText(sTmp);	
				}
				else
				{
					if (TRADE_STOCK_FUNDMERGE == m_iAction)
						sTmp.Format("%d", pItem->GetMergeAmount());
					else if ((TRADE_STOCK_FUNDSPLIT == m_iAction))
						sTmp.Format("%d", pItem->GetSplitAmount());
					else
						sTmp.Format("%d", pItem->GetEnableAmount());
					m_edEnableAmount.SetWindowText(sTmp);	
				}
						
			}
			pTrading->FreeDataList();
			break;
		}
	}
}

void CTradeStockSellDlg::DoEntrust()
{
	__super::DoEntrust();
}

void CTradeStockSellDlg::OnStockCodeExit()
{
	QueryEnableAmount();
}

void CTradeStockSellDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStockSellDlg, CTradeStockBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeStockSellDlg::QueryEnableAmount()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
		return;
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	if (sStockCode == m_sLastStockCode)
	{
		return;
	}	

	IReqQueryStockMaxSell* pIntf = dynamic_cast<IReqQueryStockMaxSell*>(m_pTradeCoreObj->NewTradingByAction(m_nQueryEnableAction));
	pIntf->SetAccount(m_pAccount);
	int iStockIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
	CString sStockAccount = pItem->GetStockAccount();
	CString sExchangeType = pItem->GetExchangeType();
	pIntf->SetExchangeType(sExchangeType);
	pIntf->SetStockAccount(sStockAccount);
	pIntf->SetStockCode(sStockCode);
	pIntf->SetEntrustProp(m_sEntrustProp);
	pIntf->TradingAsync(this);
}

void CTradeStockSellDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_paCWnd.Add(&m_stPrice);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_paCWnd.Add(&m_edPrice);
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