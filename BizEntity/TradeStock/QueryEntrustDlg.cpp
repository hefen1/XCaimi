/*******************************************************
  源程序名称:QueryEntrustDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询当日委托
  作    者:  shenglq
  开发日期:  20100910
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
			if (MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
			{
				if(m_pBusinessLogic->StockWithDraw(pEntrustItem->GetExchangeType(), pEntrustItem->GetStockAccount(), sEntrustNo, sMsg))
				{
					CString sTmp = sMsg;
					sMsg.Format(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_SUCCEEDHINT), sTmp);
					MessageBox(sMsg, "提示", MB_ICONINFORMATION);
					m_nQueryBegin = 0;
					AsyncQuery();
				}
				else
					MessageBox(sMsg, "提示", MB_ICONINFORMATION);	
			}		
		}
		else
		{
			sMsg = _T(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_RECERROR));
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
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