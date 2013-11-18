/*******************************************************
  Դ��������:QueryStockFundEntrustDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ�����̺�ҵ��ί��
  ��    ��:  shenglq
  ��������:  20110323
*********************************************************/
#include "stdafx.h"
#include "QueryStockFundEntrustDlg.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryStockFundEntrustDlg, CTradeQueryBaseDlg)

CQueryStockFundEntrustDlg::CQueryStockFundEntrustDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQueryStockFundEntrustDlg::~CQueryStockFundEntrustDlg()
{
	if (m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}
}

void CQueryStockFundEntrustDlg::DoDataCore(IDataList * pDataList)
{
	if (m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}
	m_pDataList = pDataList;
	FillGrid();
}

void CQueryStockFundEntrustDlg::DoPopupMenuEntrust()
{
	int iRow = m_pGrid->GetCurrRow();
	CString sMsg = "";
	if (iRow > 0)
	{
		LPARAM pParam = m_pGrid->GetItemData(iRow, 0);
		if (pParam)
		{
			IDataItem* pItem = (IDataItem*)pParam;
			IStockEntrustItem* pEntrustItem = dynamic_cast<IStockEntrustItem*>(pItem);	
			if (m_EntrustStatusList)
			{
				CString sEntrustStatus = pEntrustItem->GetEntrustStatus();
				INodeData* pNodeData = m_EntrustStatusList->GetItemsByName(sEntrustStatus);
				if (pNodeData)
				{
					CString sCanWithDraw = pNodeData->ValueEx();
					pNodeData->Release();
					if ("0" == sCanWithDraw)
					{
						return;
					}					
				}
			}
			CString sEntrustNo = pEntrustItem->GetEntrustNo();
			CString sHint;
			sHint.Format(m_pTradeLang->LoadStr(GVN_TRADE_WITHDRAW_HINT), sEntrustNo);
			if (MessageBox(sHint,"��ʾ",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
			{
				if(m_pBusinessLogic->StockFundWithDraw(pEntrustItem->GetExchangeType(), pEntrustItem->GetStockCode(), sEntrustNo, sMsg))
				{
					CString sTmp = sMsg;
					sMsg.Format(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_SUCCEEDHINT), sTmp);
					MessageBox(sMsg, "��ʾ", MB_ICONINFORMATION);
					m_nQueryBegin = 0;
					AsyncQuery();
				}
				else
					MessageBox(sMsg, "��ʾ", MB_ICONINFORMATION);	
			}		
		}
		else
		{
			sMsg = _T(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_RECERROR));
			MessageBox(sMsg, "��ʾ", MB_ICONINFORMATION);
		}
	}
}