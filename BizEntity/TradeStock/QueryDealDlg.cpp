/*******************************************************
  源程序名称:QueryDealDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询当日委托
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryDealDlg.h"
//#include "QueryStockEntrustIntf.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryDealDlg, CTradeQueryBaseDlg)

CQueryDealDlg::CQueryDealDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQueryDealDlg::~CQueryDealDlg()
{

}


DWORD CQueryDealDlg::GetDataType()
{
	return TRADE_SUB_STOCK_DEAL;
}

void CQueryDealDlg::DoDataCore(IDataList * pDataList)
{
	IStockDealList* pList = dynamic_cast<IStockDealList*>(pDataList);	
	m_pAccount->GetTradeDataCore()->SetStockDeal(pList);
}