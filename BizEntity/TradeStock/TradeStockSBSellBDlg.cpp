/*******************************************************
  Դ��������:TradeStockSBSellBDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���嶨������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockSBSellBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSBSellBDlg, CTradeStockSellDlg)

CTradeStockSBSellBDlg::CTradeStockSBSellBDlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{

}

CTradeStockSBSellBDlg::~CTradeStockSBSellBDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockSBSellBDlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSBSellBDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "b";
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSBSellBDlg::LoadWnd()
{	
	__super::LoadWnd();
}

