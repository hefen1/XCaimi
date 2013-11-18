t/*******************************************************
  源程序名称:StockToBankDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  证券转银行窗口类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "StockToBankDlg.h"
#include "TradeFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNAMIC(CStockToBankDlg, CBankTransferBaseDlg)

CStockToBankDlg::CStockToBankDlg(CWnd* pParent /* = NULL */)
: CBankTransferBaseDlg(pParent)
{
	m_sDirection = "2";
}

CStockToBankDlg::~CStockToBankDlg()
{

}

void CStockToBankDlg::DoDataExchange(CDataExchange* pDX)
{
	CBankTransferBaseDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStockToBankDlg, CBankTransferBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CStockToBankDlg::OnInit()
{
	if (__super::OnInit())
	{
		return TRUE;
	}
	else
		return FALSE;
}
void CStockToBankDlg::ShowPassword()
{
	int nIndex = m_cbBankNo.GetCurSel();
	IBankItem* pBankItem = (IBankItem*)m_cbBankNo.GetItemDataPtr(nIndex);
	if (pBankItem)
	{
		CStringArray arrShowPsw;
		CString sTmp = pBankItem->GetShowPassword();
		ExtractString(arrShowPsw, sTmp, "|");
		int nCount = arrShowPsw.GetCount();
		if (nCount >= 4)
		{
			sTmp = arrShowPsw.GetAt(2);
			if ("0" == sTmp)
			{
				m_stBankPsw.ShowWindow(SW_HIDE);
				m_edBankPsw.ShowWindow(SW_HIDE);
				m_btBank.ShowWindow(SW_HIDE);
			}
			else
			{
				m_stBankPsw.ShowWindow(SW_SHOW);
				m_edBankPsw.ShowWindow(SW_SHOW);
				m_btBank.ShowWindow(SW_SHOW);
			}
			sTmp = arrShowPsw.GetAt(3);
			if ("0" == sTmp)
			{
				m_stFundPsw.ShowWindow(SW_HIDE);
				m_edFundPsw.ShowWindow(SW_HIDE);
				m_btFund.ShowWindow(SW_HIDE);
			}
			else
			{
				m_stFundPsw.ShowWindow(SW_SHOW);
				m_edFundPsw.ShowWindow(SW_SHOW);
				m_btFund.ShowWindow(SW_SHOW);
			}
		}
	}
}