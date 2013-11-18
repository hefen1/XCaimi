/*******************************************************
  源程序名称:TradeStockSetCostPriceDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  重置成本价窗口基类实现
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeStockSetCostPriceDlg.h"
#include "TradeConst.h"
#include "TradeFunc.h"
#include "TradeAction.h"

#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeStockSetCostPriceDlg, CTradeDialogBase)

CTradeStockSetCostPriceDlg::CTradeStockSetCostPriceDlg(CWnd* pParent /* = NULL */)
	: CTradeDialogBase(CTradeStockSetCostPriceDlg::IDD, pParent)
{
	m_pTradeMarketList = NULL;
	m_pCurrHoldList = NULL;
}

CTradeStockSetCostPriceDlg::~CTradeStockSetCostPriceDlg()
{
	if (m_pTradeMarketList)
	{
		m_pTradeMarketList->Release();
	}
	if (m_pCurrHoldList)
	{
		m_pCurrHoldList->Release();
	}
	UnregisterLinkageMsg(TRADE_MESSAGE_STOCKCODE);
}

BOOL CTradeStockSetCostPriceDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		CString sMsg = "";
		m_pTradeMarketList = m_pAccountParamList->GetMarketList();
		m_pStockAccountList = m_pAccount->GetBusinessLogic()->GetStockAccount(sMsg);
		if (NULL == m_pStockAccountList)
		{
			MessageBoxA(sMsg, "提示");
			return FALSE;
		}
		else
		{
			int iCount = m_pStockAccountList->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				IStockAccountItem* pItem = m_pStockAccountList->GetItems(i);
				CString sTmp;
				if(m_pTradeMarketList)
				{
					ITradeMarketItem* pMarketItem = m_pTradeMarketList->GetItemsByName(pItem->GetExchangeType());
					if (pMarketItem)
					{
						sTmp = pMarketItem->GetDispName();
						pMarketItem->Release();
					}	
				}							 
				sTmp += "-";
				sTmp += pItem->GetStockAccount();				
				m_cbStockAccount.AddString(sTmp);				
			}
			if (iCount > 0)
			{
				m_cbStockAccount.SetCurSel(0);
			}
		}
		RegisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
		CString sCaption = m_pParam->GetStringParam("BtnCaption");
		if (!sCaption.IsEmpty())
		{
			m_btOK.SetWindowText(sCaption);
		}
		RegisterLinkageMsg(TRADE_MESSAGE_STOCKCODE);
		return TRUE;
	}
	else
		return FALSE;
	 
}

void CTradeStockSetCostPriceDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	IReqQueryStockHold* pIntf = dynamic_cast<IReqQueryStockHold*>(pTrading);
	if (pIntf->GetSuccessful() && pIntf->HaveData())
	{
		if (m_pCurrHoldList)
		{
			m_pCurrHoldList->Release();
			m_pCurrHoldList = NULL;
		}
		m_pCurrHoldList = pIntf->GetDataList();	
	}
}

void CTradeStockSetCostPriceDlg::OnStockCodeExit()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
		return;
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	if (sStockCode == m_sLastStockCode)
	{
		return;
	}	
	m_sLastStockCode = sStockCode;
	IReqQueryStockHold* pIntf = dynamic_cast<IReqQueryStockHold*>(m_pTradeCoreObj->NewTradingByAction(TRADE_STOCK_QUERYHOLD));
	pIntf->SetAccount(m_pAccount);
	int iStockIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
	CString sStockAccount = pItem->GetStockAccount();
	CString sExchangeType = pItem->GetExchangeType();
	pIntf->SetExchangeType(sExchangeType);
	pIntf->SetStockAccount(sStockAccount);
	pIntf->SetStockCode(sStockCode);
	pIntf->TradingAsync(this);
}



BOOL CTradeStockSetCostPriceDlg::Validate()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODEERROR);		
		sMsg.Format(sMsg, STOCKCODE_LENGTH);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edStockCode.SetFocus();
		m_edStockCode.SetSel(0, -1);
		return FALSE;
	}
	if (m_edPrice.IsWindowVisible())
	{
		double dPrice = m_edPrice.GetDouble();
		if ( dPrice < ENTRUST_MINPRICE)
		{
			CString sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_PRICEERROR);
			MessageBox(sMsg, "提示",  MB_ICONINFORMATION);
			m_edPrice.SetFocus();
			m_edPrice.SetSel(0, -1);
			return FALSE;
		}
	}	
	CString sHint = "";
	CString sTmp = "";
	CString sTmp1 = "";
	CString sTmp2 = "";	
	m_edStockCode.GetWindowText(sTmp1);
	m_edStockName.GetWindowText(sTmp2);
	sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODENAME), sTmp1, sTmp2);
	sHint += sTmp;
	int iStockIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
    sTmp1 = pItem->GetStockAccount();
	sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTACCOUNT), sTmp1);
	sHint += sTmp;
	if (m_edPrice.IsWindowVisible())
	{
		m_edPrice.GetWindowText(sTmp1);
		//sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTPRICE), sTmp1);
		sTmp.Format("成本价格：%s\r\n", sTmp1);
		sHint += sTmp;
	}
	
	//sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
	return MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
}
void CTradeStockSetCostPriceDlg::DoEntrust()
{
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	int iIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iIndex);
	CString sStockAccount = pItem->GetStockAccount();
	CString sExchangeType = pItem->GetExchangeType();
	double dPrice = m_edPrice.GetDouble();
	CString sMsg = "";
	CString sSeat = "";
	double dEntrustBalance = 0;
	double dCurrCostPrice = 0;
	CString sStockName = "";
	CString sHint = "";
	if (m_pCurrHoldList && (m_pCurrHoldList->GetCount() > 0))
	{
		IStockHoldItem* pItem = m_pCurrHoldList->GetItems(0);
		sSeat = pItem->GetSeat();
		dEntrustBalance = dPrice * pItem->GetCurrentAmount();
		dCurrCostPrice = pItem->GetCostPrice();
		m_edStockName.GetWindowText(sStockName);
		sHint.Format("%s(%s)的参考成本价由%.3f改为%.3f", sStockName, sStockCode, dCurrCostPrice, dPrice);
	}
	if (m_pBusinessLogic->SetStockCostPrice(sExchangeType, sStockAccount, sStockCode, dPrice, sSeat, dEntrustBalance, sHint, sMsg))
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		PostRefreshMsg();
		Clear();
	}
	else
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
	m_sLastStockCode = "";
	m_edStockCode.SetFocus();
}

void CTradeStockSetCostPriceDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);	

	DDX_Control(pDX, IDC_CB_STOCKACCOUNT, m_cbStockAccount);
	DDX_Control(pDX, IDC_ED_STOCKCODE, m_edStockCode);
	DDX_Control(pDX, IDC_ED_PRICE, m_edPrice);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_ED_STOCKNAME, m_edStockName);
}

BEGIN_MESSAGE_MAP(CTradeStockSetCostPriceDlg, CTradeDialogBase)

	ON_BN_CLICKED(IDC_BT_OK, &CTradeStockSetCostPriceDlg::OnBnClickedBtOk)
	ON_EN_KILLFOCUS(IDC_ED_STOCKCODE, &CTradeStockSetCostPriceDlg::OnEnKillfocusEdStockcode)
	ON_EN_CHANGE(IDC_ED_STOCKCODE, &CTradeStockSetCostPriceDlg::OnEnChangeEdStockcode)
END_MESSAGE_MAP()


void CTradeStockSetCostPriceDlg::SelectStockAccount()
{
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	if((STOCKCODE_LENGTH == sStockCode.GetLength())&&(m_pDataSourceEx))
	{
		if (sStockCode == m_sLastStockCode)
		{
			return;
		}
		StockUserInfo sStockInfo;
		if(m_pDataSourceEx->HSDataSourceEx_GetStockUserInfo((LPTSTR)(LPCTSTR)sStockCode, STOCK_MARKET, sStockInfo))
		{
			TradeStockMarket sMarket = MakeStockMarket(sStockInfo.m_ciStockCode.m_cCodeType);
			if (m_pStockAccountList)
			{
				int iStockIndex = m_cbStockAccount.GetCurSel();
				IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
				if (sMarket != pItem->GetExchangeType())
				{
					for (int i = 0; i < m_pStockAccountList->GetCount(); i++)
					{
						IStockAccountItem* pItem = m_pStockAccountList->GetItems(i);
						if (pItem->GetExchangeType() == sMarket) 
						{
							iStockIndex = i;
							if (pItem->GetMainFlag())
							{
								break;
							}
						}						 
					}
					m_cbStockAccount.SetCurSel(iStockIndex);
				}				
			}
			m_edStockName.SetWindowText(sStockInfo.GetName());
		}
		else
			m_edStockName.SetWindowText(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_NOCODE));		
	}
}


void CTradeStockSetCostPriceDlg::Clear(int iFlag /* = 0 */)
{
	if ("" != m_sLastStockCode)
	{
		m_sLastStockCode = "";
		if (!iFlag)
		{
			m_edStockCode.SetWindowText("");
		}
		m_edStockName.SetWindowText("");
		if (m_edPrice.IsWindowVisible())
		{
			m_edPrice.SetWindowText("");
		}		
	}
	
}

void CTradeStockSetCostPriceDlg::OnWindowFocused()
{
	if (IsShow())
		m_edStockCode.SetFocus();
}

void CTradeStockSetCostPriceDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}


void CTradeStockSetCostPriceDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (Validate())
	{
		DoEntrust();
	}
}

void CTradeStockSetCostPriceDlg::OnEnKillfocusEdStockcode()
{
	// TODO: Add your control notification handler code here
	SelectStockAccount();
	OnStockCodeExit();
}

void CTradeStockSetCostPriceDlg::OnEnChangeEdStockcode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTradeDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if(m_edStockCode.GetWindowTextLength() == STOCKCODE_LENGTH)
	{
		if (m_edPrice.IsWindowVisible() && m_edPrice.IsWindowEnabled())
		{
			m_edPrice.SetFocus();
		}
	}
	else
	{
		Clear(1);
	}
}

void CTradeStockSetCostPriceDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	CString sStockCodeField;
	switch(pParam->m_nMsg)
	{
	case TRADE_MESSAGE_STOCKCODE:
		if (IsShow())
		{
			CString sStockCode = pParam->GetStringField(TRADE_PARAM_STOCKCODE);
			if (!sStockCode.IsEmpty())
			{
				if(m_sLastStockCode != sStockCode)
				{
					m_edStockCode.SetFocus();
					Clear();
					m_edStockCode.SetWindowText(sStockCode);
				}
			}
		}			
		break;
	}
}