/*******************************************************
  Դ��������:QueryHoldDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeFund
  ����˵��:  ��ѯ�ֲ�
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryHoldDlg.h"
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
	return TRADE_SUB_FUND_HOLD;
}

void CQueryHoldDlg::DoDataCore(IDataList * pDataList)
{
	IFundHoldList* pList = dynamic_cast<IFundHoldList*>(pDataList);	
	m_pAccount->GetTradeDataCore()->SetFundHold(pList);
}
