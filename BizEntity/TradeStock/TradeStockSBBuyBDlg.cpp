/*******************************************************
  Դ��������:TradeStockSBBuyBDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ����
  ����˵��:  ���嶨������
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "TradeStockSBBuyBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStockSBBuyBDlg, CTradeStockBuyDlg)

CTradeStockSBBuyBDlg::CTradeStockSBBuyBDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBuyDlg(pParent)
{

}

CTradeStockSBBuyBDlg::~CTradeStockSBBuyBDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStockSBBuyBDlg, CTradeStockBuyDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStockSBBuyBDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "b";
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeStockSBBuyBDlg::LoadWnd()
{	
	__super::LoadWnd();
}

