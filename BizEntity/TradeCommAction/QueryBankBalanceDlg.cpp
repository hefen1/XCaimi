t/*******************************************************
  源程序名称:QueryBankBalanceDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询银行余额窗口类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "QueryBankBalanceDlg.h"
#include "TradeFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNAMIC(CQueryBankBalanceDlg, CBankTransferBaseDlg)

CQueryBankBalanceDlg::CQueryBankBalanceDlg(CWnd* pParent /* = NULL */)
: CBankTransferBaseDlg(pParent)
{
	m_sDirection = "3";
}

CQueryBankBalanceDlg::~CQueryBankBalanceDlg()
{

}

void CQueryBankBalanceDlg::DoDataExchange(CDataExchange* pDX)
{
	CBankTransferBaseDlg::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQueryBankBalanceDlg, CBankTransferBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CQueryBankBalanceDlg::OnInit()
{
	if (CBankTransferBaseDlg::OnInit())
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CQueryBankBalanceDlg::LoadWnd()
{
	//m_btOK.SetWindowText("查询[F3]");
	m_stBalance.ShowWindow(SW_HIDE);
	m_edBalance.ShowWindow(SW_HIDE);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_stBankNo);
	m_paCWnd.Add(&m_stMoneyType);
	if (m_stBankPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_stBankPsw);
	}
	if (m_stFundPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_stFundPsw);
	}	
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbBankNo);
	m_paCWnd.Add(&m_cbMoneyType);
	if (m_edBankPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_edBankPsw);
	}
	if (m_edFundPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_edFundPsw);
	}	
	//m_paCWnd.Add(&m_edBalance);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);	
	if (m_btBank.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edBankPsw, &m_btBank, 0, -1);
	}	
	if (m_btFund.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edFundPsw, &m_btFund, 0, -1);
	}
	if (m_edFundPsw.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edFundPsw, &m_btOK, 0, 2, adxBottomRight);
	}
	else if (m_edBankPsw.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edBankPsw, &m_btOK, 0, 2, adxBottomRight);
	}
	else if (m_cbMoneyType.IsWindowVisible())
	{
		AdjustLayoutEx(&m_cbMoneyType, &m_btOK, 0, 2, adxBottomRight);
	}
	else
	{
		AdjustLayoutEx(&m_cbBankNo, &m_btOK, 0, 2, adxBottomRight);
	}	
	SetComoboxHeight(&m_cbBankNo);
	SetComoboxHeight(&m_cbMoneyType);
	m_cbBankNo.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_cbMoneyType.SetWindowPos(&m_cbBankNo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edBankPsw.SetWindowPos(&m_cbMoneyType, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edFundPsw.SetWindowPos(&m_edBankPsw, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edFundPsw, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}

void CQueryBankBalanceDlg::ShowPassword()
{
	int nIndex = m_cbBankNo.GetCurSel();
	IBankItem* pBankItem = (IBankItem*)m_cbBankNo.GetItemDataPtr(nIndex);
	if (pBankItem)
	{
		CStringArray arrShowPsw;
		CString sTmp = pBankItem->GetShowPassword();
		ExtractString(arrShowPsw, sTmp, "|");
		int nCount = arrShowPsw.GetCount();
		if (nCount >= 6)
		{
			sTmp = arrShowPsw.GetAt(4);
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
			sTmp = arrShowPsw.GetAt(5);
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