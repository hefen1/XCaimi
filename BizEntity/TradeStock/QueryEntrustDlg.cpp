/*******************************************************
  Դ��������:QueryEntrustDlg.cpp
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  TradeStock
  ����˵��:  ��ѯ����ί��
  ��    ��:  shenglq
  ��������:  20100910
*********************************************************/
#include "stdafx.h"
#include "QueryEntrustDlg.h"
#include "QueryStockEntrustIntf.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQueryEntrustDlg, CTradeQueryBaseDlg)

CQueryEntrustDlg::CQueryEntrustDlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQueryEntrustDlg::~CQueryEntrustDlg()
{

}


void CQueryEntrustDlg::DoEntrust()
{
	int iRow = m_pGrid->GetCurrRow();
	CString sMsg = "";
	if ((iRow > 0) && (m_pGrid->GetRowCount() > 1))
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
				if(m_pBusinessLogic->StockWithDraw(pEntrustItem->GetExchangeType(), pEntrustItem->GetStockAccount(), sEntrustNo, sMsg))
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

DWORD CQueryEntrustDlg::GetDataType()
{
	return TRADE_SUB_STOCK_ENTRUST;
}

void CQueryEntrustDlg::DoDataCore(IDataList * pDataList)
{
	IStockEntrustList* pList = dynamic_cast<IStockEntrustList*>(pDataList);	
	m_pAccount->GetTradeDataCore()->SetStockEntrust(pList);
}

void CQueryEntrustDlg::DoPopupMenuEntrust()
{
	DoEntrust();
}