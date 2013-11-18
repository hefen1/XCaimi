/*******************************************************
  Դ��������:QueryDealDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����ί��
  ��    ��:  shenglq
  ��������:  20100910
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