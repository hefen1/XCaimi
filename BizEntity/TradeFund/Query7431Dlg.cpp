/*******************************************************
  源程序名称:Query7431Dlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  TradeStock
  功能说明:  查询基金定投委托
  作    者:  shenglq
  开发日期:  20110323
*********************************************************/
#include "stdafx.h"
#include "Query7431Dlg.h"
#include "TradeLangDefined.h"
#include "TradeFundEntrustIntf.h"
#include "TradeAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CQuery7431Dlg, CTradeQueryBaseDlg)

CQuery7431Dlg::CQuery7431Dlg(CWnd* pParent /* = NULL */)
: CTradeQueryBaseDlg(pParent)
{

}

CQuery7431Dlg::~CQuery7431Dlg()
{
	if (m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}
}

void CQuery7431Dlg::DoDataCore(IDataList * pDataList)
{
	if (m_pDataList)
	{
		m_pDataList->Release();
		m_pDataList = NULL;
	}
	m_pDataList = pDataList;
	FillGrid();
}

void CQuery7431Dlg::DoPopupMenuEntrust()
{
	int iRow = m_pGrid->GetCurrRow();
	CString sMsg = "";
	if (iRow > 0)
	{
		LPARAM pParam = m_pGrid->GetItemData(iRow, 0);
		if (pParam)
		{
			IDataItem* pItem = (IDataItem*)pParam;
			IFundEntrustItem* pEntrustItem = dynamic_cast<IFundEntrustItem*>(pItem);	
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
				/*if(m_pBusinessLogic->StockFundWithDraw(pEntrustItem->GetExchangeType(), pEntrustItem->GetStockCode(), sEntrustNo, sMsg))
				{
					CString sTmp = sMsg;
					sMsg.Format(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_SUCCEEDHINT), sTmp);
					MessageBox(sMsg, "提示", MB_ICONINFORMATION);
					AsyncQuery();
				}
				else
					MessageBox(sMsg, "提示", MB_ICONINFORMATION);	*/
				IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(m_pTradeCoreObj->NewTradingByAction(TRADE_FUND_7434));
				pIntf->Clear();
				pIntf->SetAccount(m_pAccount);
				pIntf->SetEntrustNo(sEntrustNo);
				pIntf->SetFundCompany(pEntrustItem->GetFundCompany());
				pIntf->SetFundCode(pEntrustItem->GetFundCode());
				pIntf->SetAllotDate(pEntrustItem->GetAllotDate());
				pIntf->SetAllotNo(pEntrustItem->GetAllotNo());
				pIntf->SetStockAccount(pEntrustItem->GetStrByName("stock_account"));
				pIntf->SetTransAccount(pEntrustItem->GetStrByName("trans_account"));
				pIntf->SetChargeType(pEntrustItem->GetStrByName("charge_type"));
				pIntf->SetRequestARData("product_type", pEntrustItem->GetStrByName("product_type"));
				if (pIntf->TradingSync())
				{
					IDataList* pList = pIntf->GetDataList();
					if (pList)
					{
						IDataItem* pItem = pList->GetItems(0);
						if (pItem)
						{
							int iErrorNo = pItem->GetErrorNo();
							if ( 0 == iErrorNo)
							{	
								//sMsg = pItem->GetSerialNo();
								sMsg.Format(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_SUCCEEDHINT), pItem->GetSerialNo());
								MessageBox(sMsg, "提示", MB_ICONINFORMATION);
								AsyncQuery();
							}
							else
							{
								sMsg = pItem->GetErrorInfo();
								MessageBox(sMsg, "提示", MB_ICONINFORMATION);
							}
						}
						pIntf->FreeDataList();
					}		
				}
				else
				{
					sMsg = pIntf->GetLastError();
					MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				}
				pIntf->Release();
			}		
		}
		else
		{
			sMsg = _T(m_pTradeLang->LoadStr(GVN_STOCK_WITHDRAW_RECERROR));
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		}
	}
}