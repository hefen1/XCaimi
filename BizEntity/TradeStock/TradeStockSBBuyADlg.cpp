/*******************************************************
  Դ��������:TradeStockSBBuyADlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ������������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockSBBuyADlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSBBuyADlg, CTradeStockBuyDlg)

CTradeStockSBBuyADlg::CTradeStockSBBuyADlg(CWnd* pParent /* = NULL */)
	: CTradeStockBuyDlg(pParent)
{

}

CTradeStockSBBuyADlg::~CTradeStockSBBuyADlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockSBBuyADlg, CTradeStockBuyDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSBBuyADlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "a";
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSBBuyADlg::LoadWnd()
{	
	__super::LoadWnd();
}

