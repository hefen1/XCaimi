/*******************************************************
  源程序名称:TradeStock9837Dlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  ETF网下股份换购
  作    者:  shenglq
  开发日期:  20100421
*********************************************************/
#include "stdafx.h"
#include "TradeStock9837Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeStock9837Dlg, CTradeStockFundTransDlg)

CTradeStock9837Dlg::CTradeStock9837Dlg(CWnd* pParent /* = NULL */)
	: CTradeStockFundTransDlg(pParent)
{

}

CTradeStock9837Dlg::~CTradeStock9837Dlg()
{

}


BEGIN_MESSAGE_MAP(CTradeStock9837Dlg, CTradeStockFundTransDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeStock9837Dlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "M";
		m_stEnableAmount.SetWindowText("可换数量");
		m_stTransCode.SetWindowText("ETF 代码");
		return TRUE;
	}
	else
		return FALSE;
}

