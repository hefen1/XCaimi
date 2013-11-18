/*******************************************************
  源程序名称:TradeStockSubcribeDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  场内基金认购
  作    者:  shenglq
  开发日期:  20110316
*********************************************************/
#include "stdafx.h"
#include "TradeStockSubcribeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSubcribeDlg, CTradeStockAllotDlg)

CTradeStockSubcribeDlg::CTradeStockSubcribeDlg(CWnd* pParent /* = NULL */)
	: CTradeStockAllotDlg(pParent)
{

}

CTradeStockSubcribeDlg::~CTradeStockSubcribeDlg()
{

}

void CTradeStockSubcribeDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStockSubcribeDlg, CTradeStockAllotDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSubcribeDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "0L";
		m_stEnableAmount.SetWindowText("认购上限");
		m_stAmount.SetWindowText("认购金额");
		return TRUE;
	}
	else
		return FALSE;	
}

BOOL CTradeStockSubcribeDlg::Validate()
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
		MessageBox("输入的认购金额有误，请重输！", "提示", MB_ICONINFORMATION);
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
	sTmp.Format("认购金额：%d\r\n", iAmount);
	sHint += sTmp;
	sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
	return MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
}