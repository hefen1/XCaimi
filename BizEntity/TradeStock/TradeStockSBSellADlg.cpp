/*******************************************************
  Դ��������:TradeStockSBSellADlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ������������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockSBSellADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSBSellADlg, CTradeStockSellDlg)

CTradeStockSBSellADlg::CTradeStockSBSellADlg(CWnd* pParent /* = NULL */)
	: CTradeStockSellDlg(pParent)
{

}

CTradeStockSBSellADlg::~CTradeStockSBSellADlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockSBSellADlg, CTradeStockSellDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSBSellADlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "a";
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSBSellADlg::LoadWnd()
{	
	__super::LoadWnd();
}

