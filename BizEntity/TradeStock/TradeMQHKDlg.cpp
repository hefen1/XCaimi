/*******************************************************
  Դ��������:TradeMQHKDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ����֤ȯ��ȯ����
  ��    ��:  shenglq
  ��������:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeMQHKDlg.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeMQHKDlg, CTradeStockSellDlg)

CTradeMQHKDlg::CTradeMQHKDlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{
	//m_nQueryEnableAction = TRADE_RZRQ_QUERYMAXSELL;
	m_sEntrustType = "6";
}

CTradeMQHKDlg::~CTradeMQHKDlg()
{

}

void CTradeMQHKDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeMQHKDlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeMQHKDlg::OnInit()
{
	if (__super::OnInit())
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeMQHKDlg::LoadWnd()
{
	__super::LoadWnd();
}