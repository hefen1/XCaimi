/*******************************************************
  Դ��������:QueryHoldDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ�ֲ�
  ��    ��:  shenglq
  ��������:  20100910
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