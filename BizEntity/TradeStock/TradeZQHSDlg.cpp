/*******************************************************
  源程序名称:TradeZQHSDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  债券回售
  作    者:  shenglq
  开发日期:  20100317
*********************************************************/
#include "stdafx.h"
#include "TradeZQHSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeZQHSDlg, CTradeZQToStockDlg)

CTradeZQHSDlg::CTradeZQHSDlg(CWnd* pParent /* = NULL */)
	: CTradeZQToStockDlg(pParent)
{

}

CTradeZQHSDlg::~CTradeZQHSDlg()
{

}


BEGIN_MESSAGE_MAP(CTradeZQHSDlg, CTradeZQToStockDlg)
	//
	//ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBuy::OnBnClickedBtOk)
END_MESSAGE_MAP()

BOOL CTradeZQHSDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_sEntrustProp = "8";
		return TRUE;
	}
	else
		return FALSE;
}

