/*******************************************************
  源程序名称:TradeStockSBSellADlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  三板意向卖出
  作    者:  shenglq
  开发日期:  20110323
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

