/*******************************************************
  源程序名称:TradeStock9835Dlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  ETF网下现金认购
  作    者:  shenglq
  开发日期:  20110421
*********************************************************/
#include "stdafx.h"
#include "TradeStock9835Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStock9835Dlg, CTradeStockSubcribeDlg)

CTradeStock9835Dlg::CTradeStock9835Dlg(CWnd* pParent /* = NULL */)
	: CTradeStockSubcribeDlg(pParent)
{

}

CTradeStock9835Dlg::~CTradeStock9835Dlg()
{

}

void CTradeStock9835Dlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTradeStock9835Dlg, CTradeStockSubcribeDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStock9835Dlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "M";
		//m_stEnableAmount.SetWindowText("认购上限");
		//m_stAmount.SetWindowText("认购金额");
		return TRUE;
	}
	else
		return FALSE;	
}
