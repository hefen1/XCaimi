/*******************************************************
  源程序名称:QueryHoldDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询持仓
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryHoldDlg.h"
//#include "QueryStockEntrustIntf.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryHoldDlg, CTradeQueryBaseDlg)

CQueryHoldDlg::CQueryHoldDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQueryHoldDlg::~CQueryHoldDlg()
{

}


DWORD CQueryHoldDlg::GetDataType()
{
	return TRADE_SUB_STOCK_HOLD;
}

void CQueryHoldDlg::DoDataCore(IDataList * pDataList)
{
	IStockHoldList* pList = dynamic_cast<IStockHoldList*>(pDataList);	
	m_pAccount->GetTradeDataCore()->SetStockHold(pList);
}