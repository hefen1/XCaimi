/*******************************************************
  源程序名称:TradeFund7432Dlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易基金定投
  作    者:  shenglq
  开发日期:  20110216
*********************************************************/
#include "stdafx.h"
#include "TradeFund7432Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeFund7432Dlg, CTradeFundBaseDlg)

CTradeFund7432Dlg::CTradeFund7432Dlg(CWnd* pParent /* = NULL */)
	: CTradeFundBaseDlg(pParent)
{

}

CTradeFund7432Dlg::~CTradeFund7432Dlg()
{

}


BOOL CTradeFund7432Dlg::OnInit()
{
	if (__super::OnInit())
	{
		m_stBalance.SetWindowText(_T("定投金额"));
		return TRUE;
	}
	else
		return FALSE;
}


void CTradeFund7432Dlg::OnFundCodeExit()
{
	QueryEnableBalance();
	__super::OnFundCodeExit();	
}
BOOL CTradeFund7432Dlg::Validate()
{
	if (__super::Validate())
	{
		CString sHint = "";
		CString sTmp = "";
		CString sTmp1 = "";
		CString sTmp2 = "";	
		m_edFundCode.GetWindowText(sTmp1);
		m_edFundName.GetWindowText(sTmp2);
		sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_FUND_CODENAME), sTmp1, sTmp2);
		sHint += sTmp;
		SYSTEMTIME stTime;
		m_dtBeginDate.GetTime(&stTime);
		DWORD nBeginDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
		sTmp.Format("起始日期：%d\r\n", nBeginDate);
		sHint += sTmp;
		m_dtEndDate.GetTime(&stTime);
		DWORD nEndDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
		sTmp.Format("终止日期：%d\r\n", nEndDate);
		sHint += sTmp;
		
		if (nEndDate < nBeginDate)
		{
			MessageBoxA("终止日期不能小于起始日期", "提示");
			m_dtBeginDate.SetFocus();
			return FALSE;
		}
		int nFundDate = m_edFundDate.GetInt();
		if ((nFundDate < 1) || (nFundDate > 31))
		{
			MessageBoxA("月扣款日无效，请重新输入一个[1-31]的数字！", "提示");
			m_edFundDate.SetFocus();
			return FALSE;
		}
		if (m_edBalance.IsWindowVisible())
		{
			double dBalance = m_edBalance.GetDouble();
			if (dBalance < ENTRUST_MINPRICE)
			{
				MessageBoxA("输入的定投金额有误，请重新输入！", "提示");
				m_edBalance.SetFocus();
				m_edBalance.SetSel(0, -1);
				return FALSE;
			}
			m_edBalance.GetWindowText(sTmp1);
			sTmp.Format("定投金额：%s\r\n", sTmp1);
			sHint += sTmp;
		}
		sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
		return MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
	}
	else
		return FALSE;
}

void CTradeFund7432Dlg::DoEntrust()
{
	CString sFundCode = "";
	m_edFundCode.GetWindowText(sFundCode);
	double dBalance = m_edBalance.GetDouble();
	CString sMsg = "";
	CString sFundCompany = "";
	CString sChargetype = "";
	if (m_FundInfoItem)
	{
		sFundCompany = m_FundInfoItem->GetFundCompany();
		sChargetype = m_FundInfoItem->GetChargeType();
	}
	SYSTEMTIME stTime;
	m_dtBeginDate.GetTime(&stTime);
	DWORD nBeginDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
	CString sBeginDate = "";
	sBeginDate.Format("%d", nBeginDate);
	m_dtEndDate.GetTime(&stTime);
	DWORD nEndDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
	CString sEndDate = "";
	sEndDate.Format("%d", nEndDate);
	CString sFundDate = "";
	sFundDate.Format("%d", m_edFundDate.GetInt());
	if (m_pBusinessLogic->Fund7432(sFundCompany, sFundCode, dBalance, sChargetype, sBeginDate, sEndDate, sFundDate, sMsg))
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		PostRefreshMsg();
		Clear();
	}
	else
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
	m_sLastFundCode = "";
	m_edFundCode.SetFocus();
}

void CTradeFund7432Dlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeFund7432Dlg, CTradeFundBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeFund7432Dlg::LoadWnd()
{
	m_paCWnd.Add(&m_stFundCode);
	m_paCWnd.Add(&m_stFundName);
	m_paCWnd.Add(&m_stFundNav);
	m_paCWnd.Add(&m_stEnableBalance);
	m_paCWnd.Add(&m_stBeginDate);
	m_paCWnd.Add(&m_stEndDate);
	m_paCWnd.Add(&m_stFundDate);
	m_paCWnd.Add(&m_stBalance);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_edFundCode);
	m_paCWnd.Add(&m_edFundName);
	m_paCWnd.Add(&m_edFundNav);
	m_paCWnd.Add(&m_edEnableBalance);
	m_paCWnd.Add(&m_dtBeginDate);
	m_paCWnd.Add(&m_dtEndDate);
	m_paCWnd.Add(&m_edFundDate);
	m_paCWnd.Add(&m_edBalance);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_edBalance, &m_btOK, 0, 2, adxBottomRight);
	//SetComoboxHeight(&m_cbStockAccount);
	m_edFundCode.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_dtBeginDate.SetWindowPos(&m_edFundCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_dtEndDate.SetWindowPos(&m_dtBeginDate, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edFundDate.SetWindowPos(&m_dtEndDate, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edBalance.SetWindowPos(&m_edFundDate, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edBalance, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}

void CTradeFund7432Dlg::OnCodeChangedFocus()
{
	m_dtBeginDate.SetFocus();
}