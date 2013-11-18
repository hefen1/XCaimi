/*******************************************************
  源程序名称:TradeFundHopeRedeemDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易基金预约赎回
  作    者:  shenglq
  开发日期:  20110216
*********************************************************/
#include "stdafx.h"
#include "TradeFundHopeRedeemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeFundHopeRedeemDlg, CTradeFundBaseDlg)

CTradeFundHopeRedeemDlg::CTradeFundHopeRedeemDlg(CWnd* pParent /* = NULL */)
	: CTradeFundBaseDlg(pParent)
{

}

CTradeFundHopeRedeemDlg::~CTradeFundHopeRedeemDlg()
{

}


BOOL CTradeFundHopeRedeemDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_stEnableBalance.SetWindowText(_T("可赎份额"));
		m_stBalance.SetWindowText(_T("赎回份额"));
		return TRUE;
	}
	else
		return FALSE;
}


void CTradeFundHopeRedeemDlg::OnFundCodeExit()
{
	__super::OnFundCodeExit();	
	QueryEnableAmount();
}

BOOL CTradeFundHopeRedeemDlg::Validate()
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
		m_dtHopeDate.GetTime(&stTime);
		DWORD nHopeDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
		sTmp.Format("预约日期：%d\r\n", nHopeDate);
		sHint += sTmp;
		if (m_edBalance.IsWindowVisible())
		{
			double dBalance = m_edBalance.GetDouble();
			if (dBalance < ENTRUST_MINPRICE)
			{
				MessageBoxA("输入的赎回份额有误，请重新输入！", "提示");
				m_edBalance.SetFocus();
				m_edBalance.SetSel(0, -1);
				return FALSE;
			}
			m_edBalance.GetWindowText(sTmp1);
			sTmp.Format("赎回份额：%s\r\n", sTmp1);
			sHint += sTmp;
		}
		sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
		BOOL bReturn = MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
		m_btOK.SetFocus();
		return bReturn;
	}
	else
		return FALSE;
}

void CTradeFundHopeRedeemDlg::DoEntrust()
{
	CString sFundCode = "";
	m_edFundCode.GetWindowText(sFundCode);
	double dBalance = m_edBalance.GetDouble();
	CString sMsg = "";
	CString sFundCompany = "";
	CString sChargetype = "";
	CString sAllotDate = "";
	CString sAllotNo = "";
	if (m_FundInfoItem)
	{
		sFundCompany = m_FundInfoItem->GetFundCompany();
		sChargetype = m_FundInfoItem->GetChargeType();
	}
	CString sExceedFlag = GetListValue(m_pExceedFlag, m_cbExceedFlag);
	SYSTEMTIME stTime;
	m_dtHopeDate.GetTime(&stTime);
	DWORD nHopeDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
	CString sHopeDate = "";
	sHopeDate.Format("%d", nHopeDate);
	if (m_pBusinessLogic->FundHopeRedeem(sFundCompany, sFundCode, dBalance, m_sAllotDate, m_sAllotNo, sExceedFlag, sChargetype, sHopeDate, sMsg))
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

void CTradeFundHopeRedeemDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeFundHopeRedeemDlg, CTradeFundBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeFundHopeRedeemDlg::LoadWnd()
{
	m_paCWnd.Add(&m_stFundCode);
	m_paCWnd.Add(&m_stFundName);
	m_paCWnd.Add(&m_stFundNav);
	m_paCWnd.Add(&m_stEnableBalance);
	m_paCWnd.Add(&m_stBalance);
	m_paCWnd.Add(&m_stHopeDate);
	m_paCWnd.Add(&m_stExceedFlag);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_edFundCode);
	m_paCWnd.Add(&m_edFundName);
	m_paCWnd.Add(&m_edFundNav);
	m_paCWnd.Add(&m_edEnableBalance);
	m_paCWnd.Add(&m_edBalance);
	m_paCWnd.Add(&m_dtHopeDate);
	m_paCWnd.Add(&m_cbExceedFlag);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);
	AdjustLayoutEx(&m_cbExceedFlag, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbExceedFlag);
	m_edFundCode.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edBalance.SetWindowPos(&m_edFundCode, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_dtHopeDate.SetWindowPos(&m_edBalance, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_cbExceedFlag.SetWindowPos(&m_dtHopeDate, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_cbExceedFlag, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
}