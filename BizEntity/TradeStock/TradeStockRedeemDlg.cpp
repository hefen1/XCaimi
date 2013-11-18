/*******************************************************
  源程序名称:TradeStockRedeemDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  场内基金赎回
  作    者:  shenglq
  开发日期:  20110316
*********************************************************/
#include "stdafx.h"
#include "TradeStockRedeemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockRedeemDlg, CTradeStockSellDlg)

CTradeStockRedeemDlg::CTradeStockRedeemDlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{

}

CTradeStockRedeemDlg::~CTradeStockRedeemDlg()
{

}

void CTradeStockRedeemDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStockRedeemDlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockRedeemDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "L";
		m_stEnableAmount.SetWindowText("赎回上限");
		m_stAmount.SetWindowText("赎回份额");
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockRedeemDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stStockAccount);
	m_paCWnd.Add(&m_stStockCode);
	m_paCWnd.Add(&m_stStockName);
	m_stPrice.ShowWindow(SW_HIDE);
	m_paCWnd.Add(&m_stEnableAmount);
	m_paCWnd.Add(&m_stAmount);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbStockAccount);
	m_paCWnd.Add(&m_edStockCode);
	m_paCWnd.Add(&m_edStockName);
	m_edPrice.SetWindowText("1");
	m_edPrice.ShowWindow(SW_HIDE);
	m_paCWnd.Add(&m_edEnableAmount);
	m_paCWnd.Add(&m_edAmount);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);	
	m_spPrice.ShowWindow(SW_HIDE);
	AdjustLayoutEx(&m_edAmount, &m_spAmount, 0, -1);
	AdjustLayoutEx(&m_spAmount, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbStockAccount);
	m_cbStockAccount.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edStockCode.SetWindowPos(&m_cbStockAccount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edAmount.SetWindowPos(&m_edStockCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edAmount, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}

BOOL CTradeStockRedeemDlg::Validate()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODEERROR);		
		sMsg.Format(sMsg, STOCKCODE_LENGTH);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edStockCode.SetFocus();
		m_edStockCode.SetSel(0, -1);
		return FALSE;
	}	
	int iAmount = m_edAmount.GetInt();
	if (0 == iAmount)
	{
		//CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_AMOUNTERROR);
		MessageBox("输入的赎回份额有误，请重输！", "提示", MB_ICONINFORMATION);
		m_edAmount.SetFocus();
		m_edAmount.SetSel(0, -1);
		return FALSE;
	}
	CString sHint = "";
	CString sTmp = "";
	CString sTmp1 = "";
	CString sTmp2 = "";
	sTmp.Format("%d", int(m_eBSDirection));
	sTmp = GetEntrustProp() + sTmp;
	if (m_StockEntrustType)
	{
		INodeData* pItem = m_StockEntrustType->GetItemsByName(sTmp);
		if (pItem)
		{
			sTmp = pItem->Value();
			pItem->Release();
		}
	}
	sHint.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_MMFX), sTmp);

	m_edStockCode.GetWindowText(sTmp1);
	m_edStockName.GetWindowText(sTmp2);
	sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODENAME), sTmp1, sTmp2);
	sHint += sTmp;
	int iStockIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
	sTmp1 = pItem->GetStockAccount();
	sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTACCOUNT), sTmp1);
	sHint += sTmp;
	//sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTAMOUNT), iAmount);
	sTmp.Format("赎回份额：%d\r\n", iAmount);
	sHint += sTmp;
	sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
	return MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
}