/*******************************************************
  源程序名称:TradeMQHQDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  交易证券卖券还券
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeMQHQDlg.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeMQHQDlg, CTradeStockBuyDlg)

CTradeMQHQDlg::CTradeMQHQDlg(CWnd* pParent /* = NULL */)
	: CTradeStockBuyDlg(pParent)
{
	m_nQueryEnableAction = TRADE_RZRQ_QUERYMAXBUY;
	m_sEntrustType = "7";
}

CTradeMQHQDlg::~CTradeMQHQDlg()
{

}

void CTradeMQHQDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeMQHQDlg, CTradeStockBuyDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeMQHQDlg::OnInit()
{
	if (__super::OnInit())
	{
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeMQHQDlg::LoadWnd()
{
	__super::LoadWnd(); 
}