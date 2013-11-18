t/*******************************************************
  Դ��������:BankToStockDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����ת֤ȯ������
  ��    ��:  shenglq
  ��������:  20110310
*********************************************************/
#include "stdafx.h"
#include "BankToStockDlg.h"
#include "TradeFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IMPLEMENT_DYNAMIC(CBankToStockDlg, CBankTransferBaseDlg)

CBankToStockDlg::CBankToStockDlg(CWnd* pParent /* = NULL */)
: CBankTransferBaseDlg(pParent)
{
	m_sDirection = "1";
}

CBankToStockDlg::~CBankToStockDlg()
{

}

void CBankToStockDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBankToStockDlg, CBankTransferBaseDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CBankToStockDlg::OnInit()
{
	if (__super::OnInit())
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CBankToStockDlg::ShowPassword()
{
	int nIndex = m_cbBankNo.GetCurSel();
	IBankItem* pBankItem = (IBankItem*)m_cbBankNo.GetItemDataPtr(nIndex);
	if (pBankItem)
	{
		CStringArray arrShowPsw;
		CString sTmp = pBankItem->GetShowPassword();
		ExtractString(arrShowPsw, sTmp, "|");
		int nCount = arrShowPsw.GetCount();
		if (nCount >= 2)
		{
			sTmp = arrShowPsw.GetAt(0);
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
			sTmp = arrShowPsw.GetAt(1);
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