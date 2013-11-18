/*******************************************************
  Դ��������:TradeFund7432Dlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���׻���Ͷ
  ��    ��:  shenglq
  ��������:  20110216
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
		m_stBalance.SetWindowText(_T("��Ͷ���"));
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
		sTmp.Format("��ʼ���ڣ�%d\r\n", nBeginDate);
		sHint += sTmp;
		m_dtEndDate.GetTime(&stTime);
		DWORD nEndDate = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
		sTmp.Format("��ֹ���ڣ�%d\r\n", nEndDate);
		sHint += sTmp;
		
		if (nEndDate < nBeginDate)
		{
			MessageBoxA("��ֹ���ڲ���С����ʼ����", "��ʾ");
			m_dtBeginDate.SetFocus();
			return FALSE;
		}
		int nFundDate = m_edFundDate.GetInt();
		if ((nFundDate < 1) || (nFundDate > 31))
		{
			MessageBoxA("�¿ۿ�����Ч������������һ��[1-31]�����֣�", "��ʾ");
			m_edFundDate.SetFocus();
			return FALSE;
		}
		if (m_edBalance.IsWindowVisible())
		{
			double dBalance = m_edBalance.GetDouble();
			if (dBalance < ENTRUST_MINPRICE)
			{
				MessageBoxA("����Ķ�Ͷ����������������룡", "��ʾ");
				m_edBalance.SetFocus();
				m_edBalance.SetSel(0, -1);
				return FALSE;
			}
			m_edBalance.GetWindowText(sTmp1);
			sTmp.Format("��Ͷ��%s\r\n", sTmp1);
			sHint += sTmp;
		}
		sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
		return MessageBox(sHint,"��ʾ",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
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
		MessageBox(sMsg, "��ʾ", MB_ICONINFORMATION);
		PostRefreshMsg();
		Clear();
	}
	else
	{
		MessageBox(sMsg, "��ʾ", MB_ICONINFORMATION);
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