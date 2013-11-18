/*******************************************************
  源程序名称:TradeStockBaseDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  股票委托窗口基类实现
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeStockBaseDlg.h"
#include "TradeConst.h"
#include "TradeFunc.h"
#include "TradeAction.h"

#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeStockBaseDlg, CTradeDialogBase)

CTradeStockBaseDlg::CTradeStockBaseDlg(CWnd* pParent /* = NULL */)
	: CTradeDialogBase(CTradeStockBaseDlg::IDD, pParent)
{
	m_eBSDirection = bsBuy;
	m_sEntrustProp = "0";
	m_dLastPrice = 0;
	m_edAmount.SetDecimal(0);
	m_edAmount.SetStep(100);
	m_lCurrPrevClose = 0;
	m_pTradeMarketList = NULL;
	m_pSHSJType = NULL;
	m_pSZSJType = NULL;
	m_StockEntrustType = NULL;
	m_sEntrustType = "0";
	m_bQuickTrade = FALSE;
}

CTradeStockBaseDlg::~CTradeStockBaseDlg()
{
	UnRequestHQ();
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
	if (m_bQuickTrade)
	   UnregisterLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE);
	UnregisterLinkageMsg(PUBLIC_MESSAGE_TRADE);
	UnregisterLinkageMsg(TRADE_MESSAGE_STOCKCODE);
	if (m_pTradeMarketList)
	{
		m_pTradeMarketList->Release();
	}
	if (m_pSHSJType)
	{
		m_pSHSJType->Release();
	}
	if (m_pSZSJType)
	{
		m_pSZSJType->Release();
	}
	if (m_StockEntrustType)
	{
		m_StockEntrustType->Release();
	}
}

BOOL CTradeStockBaseDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		InitColor();
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
		m_pSHSJType = m_pAccountParamList->GetSHSJEntrustTypeList();
		m_pSZSJType = m_pAccountParamList->GetSZSJEntrustTypeList();
		LoadSJType();
		CString sCaption = m_pParam->GetStringParam("BtnCaption");
		if (!sCaption.IsEmpty())
		{
			m_btOK.SetWindowText(sCaption);
		}
		m_StockEntrustType = m_pAccountParamList->GetStockEntrustType();
		m_bQuickTrade = m_pParam->GetStringParam("QuickTrade") == "1";
		if (m_bQuickTrade)
		{
			RegisterLinkageMsg(PUBLIC_MESSAGE_QUICKTRADE);
		}
		RegisterLinkageMsg(PUBLIC_MESSAGE_TRADE);		
		RegisterLinkageMsg(TRADE_MESSAGE_STOCKCODE);
		return TRUE;
	}
	else
		return FALSE;
	 
}

void CTradeStockBaseDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{

}

BOOL CTradeStockBaseDlg::HSDataSourceSink_OnRecvData(void* pData, int nLen)
{	
	if ((!m_edPrice.IsWindowVisible()) || (!m_edPrice.IsWindowEnabled()) || m_edPrice.GetKeyFlag())
	{
		return TRUE;
	}
	if (!pData || IsBadReadPtr(pData,1))
	{
		return FALSE;
	}
	DataHead* pHead = (DataHead*)pData;
	if ((pHead->m_nType == RT_REALTIME_EXT) || (pHead->m_nType == RT_AUTOPUSH_EXT))
	{
		AnsRealTime*  pNowData  = (AnsRealTime*)pData;
		CommRealTimeData_Ext*  pNowDataEx  = (CommRealTimeData_Ext*)pNowData->m_pnowData;
		HSStockRealTime StockRealTime = pNowDataEx->GetShareData()->m_stStockDataExt.m_stockRealTime;
		/*ShareRealTimeData_Ext* pShareRealTimeData_Ext = pNowDataEx->GetShareData();	
		int iSize = 0;
		for (int i = 0; i < pNowData->m_nSize; ++i)
		{
			pNowDataEx->m_ciStockCode;
			//pShareRealTimeData_Ext = pNowDataEx->GetShareData();
			//pShareRealTimeData_Ext->m_stStockDataExt.

			pNowDataEx = pNowDataEx->GetNext(&pNowDataEx->m_ciStockCode, iSize);
		}*/	
		long nPrice = 0;
		if (bsBuy == m_eBSDirection)
		{
			nPrice = StockRealTime.m_lSellPrice1;
		}
		else
		{
			nPrice = StockRealTime.m_lBuyPrice1;
		}
		if (0 == nPrice)
		{
			nPrice = StockRealTime.m_lNewPrice;
			if (0 == nPrice)
			{
				nPrice = m_lCurrPrevClose;
			}
		}
		
		m_edPrice.SetValue((float)nPrice/(float)1000);
	}
	return TRUE;
}

void CTradeStockBaseDlg::OnStockCodeExit()
{

}

void CTradeStockBaseDlg::OnEntrustPriceExit()
{

}


BOOL CTradeStockBaseDlg::Validate()
{
	if(m_edStockCode.GetWindowTextLength() != STOCKCODE_LENGTH)
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODEERROR);		
		sMsg.Format(sMsg, STOCKCODE_LENGTH);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		/*MSGBOXPARAMS msgbox; 
		msgbox.cbSize = sizeof(MSGBOXPARAMS);
		msgbox.dwContextHelpId = 0; 
		msgbox.dwStyle = MB_USERICON; 
		msgbox.hInstance =  AfxGetApp()->m_hInstance;
		msgbox.hwndOwner = GetSafeHwnd();
		msgbox.lpfnMsgBoxCallback = NULL; 
		msgbox.lpszCaption = "提示"; 
		msgbox.lpszIcon = MAKEINTRESOURCE(IDI_ICON_BUY);
		msgbox.lpszText = sMsg; 
		MessageBoxIndirect(&msgbox); */
		m_edStockCode.SetFocus();
		m_edStockCode.SetSel(0, -1);
		return FALSE;
	}
	if(m_edTransCode.IsWindowVisible() && (m_edTransCode.GetWindowTextLength() != STOCKCODE_LENGTH))
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_CODEERROR);		
		sMsg.Format(sMsg, STOCKCODE_LENGTH);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edTransCode.SetFocus();
		m_edTransCode.SetSel(0, -1);
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
	int iAmount = m_edAmount.GetInt();
	if (0 == iAmount)
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_AMOUNTERROR);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edAmount.SetFocus();
		m_edAmount.SetSel(0, -1);
		return FALSE;
	}
	CString sHint = "";
	CString sTmp = "";
	CString sTmp1 = "";
	CString sTmp2 = "";
	sTmp.Format("%d", int(m_eBSDirection));
	sTmp = GetEntrustProp() + sTmp;
	if (m_StockEntrustType)
	{
		INodeData* pItem = m_StockEntrustType->GetItemsByName(sTmp);
		if (pItem)
		{
			sTmp = pItem->Value();
			pItem->Release();
		}
	}
	sHint.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_MMFX), sTmp);
	
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
		sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTPRICE), sTmp1);
		sHint += sTmp;
	}
	if (m_cbSJType.IsWindowVisible())
	{
		m_cbSJType.GetWindowText(sTmp1);
		sTmp.Format(m_pTradeLang->LoadStr(GVN_STOCK_SJENTRUSTTYPE), sTmp1);
		sHint += sTmp;
	}
	sTmp.Format(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTAMOUNT), iAmount);
	sHint += sTmp;

	if (m_edPrice.IsWindowVisible())
	{
		if (m_edPrice.IsUp())
		{
			sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTPRICE_UP);
		}
		else
			if (m_edPrice.IsDown())
			{
				sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTPRICE_DOWN);
			}
	}
	
	sHint += m_pTradeLang->LoadStr(GVN_TRADE_STOCK_ENTRUSTHINT);
	BOOL bReturn = MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
	m_btOK.SetFocus();
	return bReturn;
}
void CTradeStockBaseDlg::DoEntrust()
{
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	int iIndex = m_cbStockAccount.GetCurSel();
	IStockAccountItem* pItem = m_pStockAccountList->GetItems(iIndex);
	CString sStockAccount = pItem->GetStockAccount();
	CString sExchangeType = pItem->GetExchangeType();
	double dPrice = m_edPrice.GetDouble();
	int iAmount = m_edAmount.GetInt();
	CString sMsg = "";
	switch (m_iAction)
	{
	case TRADE_STOCK_ENTRUST:
		{
			if (m_pBusinessLogic->StockEntrust(sExchangeType, sStockAccount,sStockCode,m_eBSDirection, dPrice, iAmount, GetEntrustProp(), sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_STOCK_SBENTRUST:
		{
			CString sSeatNo;
			CString sConferNo;
			m_edSeatNo.GetWindowText(sSeatNo);
			m_edConferNo.GetWindowText(sConferNo);
			if (m_pBusinessLogic->StockSBEntrust(sExchangeType, sStockAccount,sStockCode,m_eBSDirection, dPrice, iAmount, GetEntrustProp(), sSeatNo,sConferNo, sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_STOCK_FUNDMERGE:
	case TRADE_STOCK_FUNDSPLIT:
	case TRADE_STOCK_FUNDTRANS:
		{
			CString sTransCode;
			m_edTransCode.GetWindowText(sTransCode);
			if (m_pBusinessLogic->StockFundEntrust(m_iAction, sExchangeType, sStockAccount, sStockCode, iAmount, sTransCode, sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_RZRQ_ENTRUST:
		{
			if (m_pBusinessLogic->RZRQEntrust(sExchangeType, sStockAccount,sStockCode,m_eBSDirection, dPrice, iAmount, GetEntrustProp(), m_sEntrustType, sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_RZRQ_ZJHQ:
		{
			if (m_pBusinessLogic->RZRQZJHQ(sExchangeType, sStockAccount,sStockCode, iAmount, sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_STOCK_9835:
		{
			if (m_pBusinessLogic->ETF9835(sExchangeType, sStockAccount,sStockCode, iAmount, GetEntrustProp(), sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	case TRADE_STOCK_9837:
		{
			CString sTransCode;
			m_edTransCode.GetWindowText(sTransCode);
			if (m_pBusinessLogic->ETF9837(sExchangeType, sStockAccount,sStockCode, iAmount, sTransCode, GetEntrustProp(), sMsg))
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
				PostRefreshMsg();
				Clear();
			}
			else
			{
				MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			}
			break;
		}
	}
	
	m_sLastStockCode = "";
	m_sLasStockCodePriceExit = "";
	m_dLastPrice = 0;
	m_edStockCode.SetFocus();
}

void CTradeStockBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_CB_STOCKACCOUNT, m_cbStockAccount);
	DDX_Control(pDX, IDC_ED_STOCKCODE, m_edStockCode);
	DDX_Control(pDX, IDC_ED_STOCKNAME, m_edStockName);
	DDX_Control(pDX, IDC_ED_PRICE, m_edPrice);
	DDX_Control(pDX, IDC_ST_ENABLEBALANCE, m_stEnableBalance);
	DDX_Control(pDX, IDC_ED_ENABLEBALANCE, m_edEnableBalance);
	DDX_Control(pDX, IDC_ST_ENABLEAMOUNT, m_stEnableAmount);
	DDX_Control(pDX, IDC_ED_ENABLEAMOUNT, m_edEnableAmount);
	DDX_Control(pDX, IDC_ST_AMOUNT, m_stAmount);
	DDX_Control(pDX, IDC_ED_AMOUNT, m_edAmount);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_SP_PRICE, m_spPrice);
	DDX_Control(pDX, IDC_SP_AMOUNT, m_spAmount);
	DDX_Control(pDX, IDC_ST_STOCKACCOUNT, m_stStockAccount);
	DDX_Control(pDX, IDC_ST_STOCK, m_stStockCode);
	DDX_Control(pDX, IDC_ST_PRICE, m_stPrice);
	DDX_Control(pDX, IDC_ST_STOCKNAME, m_stStockName);
	DDX_Control(pDX, IDC_ST_SJTYPE, m_stSJType);
	DDX_Control(pDX, IDC_CB_SJTYPE, m_cbSJType);
	DDX_Control(pDX, IDC_ST_SEATNO, m_stSeatNo);
	DDX_Control(pDX, IDC_ED_SEATNO, m_edSeatNo);
	DDX_Control(pDX, IDC_ST_CONFERNO, m_stConferNo);
	DDX_Control(pDX, IDC_ED_CONFERNO, m_edConferNo);
	DDX_Control(pDX, IDC_ST_TRANSCODE, m_stTransCode);
	DDX_Control(pDX, IDC_ED_TRANSCODE, m_edTransCode);
}

BEGIN_MESSAGE_MAP(CTradeStockBaseDlg, CTradeDialogBase)
ON_BN_CLICKED(IDC_BT_OK, &CTradeStockBaseDlg::OnBnClickedBtOk)
ON_EN_KILLFOCUS(IDC_ED_STOCKCODE, &CTradeStockBaseDlg::OnEnKillfocusEdStockcode)
ON_EN_KILLFOCUS(IDC_ED_PRICE, &CTradeStockBaseDlg::OnEnKillfocusEdPrice)
ON_EN_CHANGE(IDC_ED_STOCKCODE, &CTradeStockBaseDlg::OnEnChangeEdStockcode)
ON_STN_CLICKED(IDC_ED_ENABLEAMOUNT, &CTradeStockBaseDlg::OnStnClickedEdEnableamount)
ON_NOTIFY(UDN_DELTAPOS, IDC_SP_PRICE, &CTradeStockBaseDlg::OnDeltaposSpPrice)
ON_NOTIFY(UDN_DELTAPOS, IDC_SP_AMOUNT, &CTradeStockBaseDlg::OnDeltaposSpAmount)
ON_CBN_KILLFOCUS(IDC_CB_SJTYPE, &CTradeStockBaseDlg::OnCbnKillfocusCbSjtype)
ON_CBN_SELCHANGE(IDC_CB_STOCKACCOUNT, &CTradeStockBaseDlg::OnCbnSelchangeCbStockaccount)
ON_EN_SETFOCUS(IDC_ED_AMOUNT, &CTradeStockBaseDlg::OnEnSetfocusEdAmount)
END_MESSAGE_MAP()


void CTradeStockBaseDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (Validate())
	{
		m_btOK.EnableWindow(FALSE);
		DoEntrust();
		m_btOK.EnableWindow();
	}
}
void CTradeStockBaseDlg::OnEnKillfocusEdStockcode()
{
	// TODO: Add your control notification handler code here	
	CString sStockCode = "";
	m_edStockCode.GetWindowText(sStockCode);
	int nLength = sStockCode.GetLength();
	if(3 == nLength)
	{
		sStockCode = "600" + sStockCode;
		m_edStockCode.SetWindowText(sStockCode);
	}
	else if (4 == nLength)
		{
			sStockCode = "00" + sStockCode;
			m_edStockCode.SetWindowText(sStockCode);
		}
	SelectStockAccount();
	OnStockCodeExit();
	m_edStockCode.GetWindowText(m_sLastStockCode);
}

void CTradeStockBaseDlg::OnEnKillfocusEdPrice()
{
	// TODO: Add your control notification handler code here
	CString sPrice;
	m_edPrice.GetWindowText(sPrice);
	if (!sPrice.IsEmpty())
	{
		m_edPrice.SetKeyFlag(TRUE);
	}
	OnEntrustPriceExit();
	m_edStockCode.GetWindowText(m_sLasStockCodePriceExit);	
	m_dLastPrice = m_edPrice.GetDouble();
}

void CTradeStockBaseDlg::OnEnChangeEdStockcode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTradeDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if(m_edStockCode.GetWindowTextLength() == STOCKCODE_LENGTH)
	{
		CString strStockCode;
		m_edStockCode.GetWindowText(strStockCode);
		if (strStockCode != m_sLastStockCode)
		{
			Clear(1);
		}
		if (!m_bQuickTrade)
		{
			NotifyQuoteChangeCode(strStockCode);
		}
		if (m_edPrice.IsWindowVisible() && m_edPrice.IsWindowEnabled())
		{
			m_edPrice.SetFocus();
		}
		else if (m_cbSJType.IsWindowVisible() && m_cbSJType.IsWindowEnabled())
		{
			m_cbSJType.SetFocus();
		}
		else if (m_edTransCode.IsWindowVisible() && m_edTransCode.IsWindowEnabled())
		{
			m_edTransCode.SetFocus();
		}
		else if (m_edAmount.IsWindowVisible() && m_edAmount.IsWindowEnabled())
		{
			m_edAmount.SetFocus();
		}
	}
	else
	{
		Clear(1);
	}
}

void CTradeStockBaseDlg::OnStnClickedEdEnableamount()
{
	// TODO: Add your control notification handler code here
	CString sTmp = "";
	m_edEnableAmount.GetWindowText(sTmp);
	m_edAmount.SetWindowText(sTmp);
}

void CTradeStockBaseDlg::SelectStockAccount()
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
		if(m_pDataSourceEx->HSDataSourceEx_GetStockUserInfo(sStockCode.GetBuffer(), STOCK_MARKET, sStockInfo))
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
					LoadSJType();
				}				
			}
			m_edPrice.SetDecimal(sStockInfo.m_nDecimal);
			m_lCurrPrevClose = sStockInfo.m_lPrevClose;
			m_edPrice.SetUpValue(sStockInfo.m_fUpPrice / 1000);
			m_edPrice.SetDownValue(sStockInfo.m_fDownPrice / 1000);
			CString sStockName = sStockInfo.GetName();
			m_edStockName.SetWindowText(sStockName);
			RequestHQ(sStockInfo);

		}
		else
			m_edStockName.SetWindowText(m_pTradeLang->LoadStr(GVN_TRADE_STOCK_NOCODE));		
	}
}
void CTradeStockBaseDlg::RequestHQ(StockUserInfo sStockInfo)
{
	AskData*  paskData;
	int lRetLen = sizeof(AskData);
	paskData = (AskData*) new char[lRetLen];
	memset(paskData, 0, lRetLen);

	paskData->m_nType  = RT_REALTIME_EXT;
	paskData->m_nSize  = 1;
	CodeInfo* pinfo = (CodeInfo*)&paskData->m_pCode;
	memcpy(pinfo, &sStockInfo.m_ciStockCode, sizeof(CodeInfo));
	m_pDataSourceEx->HSDataSource_RequestAsyncData(m_DataSourceHandle, (char*)paskData, lRetLen, 0, e_DataSouceSend_Clear);
	paskData->m_nType = RT_AUTOPUSH_EXT;
	m_pDataSourceEx->HSDataSource_RequestAsyncData(m_DataSourceHandle, (char*)paskData, lRetLen, 0, e_DataSouceSend_Clear);	
	delete paskData;
}

void CTradeStockBaseDlg::UnRequestHQ()
{
	NotifyQuoteChangeCode("");
	m_pDataSourceEx->HSDataSource_RequestAsyncData(m_DataSourceHandle, NULL, 0, 0);
}

void CTradeStockBaseDlg::Clear(int iFlag /* = 0 */)
{
	if ("" != m_sLastStockCode)
	{
		m_sLasStockCodePriceExit = "";
		m_sLastStockCode = "";
		m_dLastPrice = 0;
		if (!iFlag)
		{
			m_edStockCode.SetWindowText("");
		}
		m_edStockName.SetWindowText("");
		if (m_edPrice.IsWindowVisible())
		{
			m_edPrice.SetKeyFlag();
			m_edPrice.SetWindowText("");
		}		
		m_edEnableBalance.SetWindowText("");
		m_edEnableAmount.SetWindowText("");
		m_edAmount.SetWindowText("");
		m_edTransCode.SetWindowText("");
		UnRequestHQ();
	}
	
}

void CTradeStockBaseDlg::OnWindowFocused()
{
	if (IsShow())
		m_edStockCode.SetFocus();
}

void CTradeStockBaseDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CTradeStockBaseDlg::LoadSJType()
{
	//if (m_cbSJType.IsWindowVisible())
	{
		int iStockIndex = m_cbStockAccount.GetCurSel();
		IStockAccountItem* pItem = m_pStockAccountList->GetItems(iStockIndex);
		CString sExchangeType = pItem->GetExchangeType();
		INodeTreeData* pSJType = NULL;
		if ((SHACCOUNT == sExchangeType) || (SHBACCOUNT == sExchangeType))
			pSJType = m_pSHSJType;
		else
			pSJType = m_pSZSJType;
		if (pSJType)
		{
			m_cbSJType.ResetContent();
			int iCount = pSJType->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				INodeData* pItem = pSJType->GetItems(i);
				m_cbSJType.AddString(pItem->Name());
				pItem->Release();
			}
			if (iCount > 0)
			{
				m_cbSJType.SetCurSel(0);
			}
		}
	}	
}
void CTradeStockBaseDlg::OnDeltaposSpPrice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_edPrice.StepIt(pNMUpDown->iDelta * -1);
	*pResult = 0;
}

void CTradeStockBaseDlg::OnDeltaposSpAmount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_edAmount.StepIt(pNMUpDown->iDelta * -1);
	*pResult = 0;
}

void CTradeStockBaseDlg::OnCbnKillfocusCbSjtype()
{
	// TODO: Add your control notification handler code here
	OnEntrustPriceExit();
	m_edStockCode.GetWindowText(m_sLasStockCodePriceExit);
}

CString CTradeStockBaseDlg::GetEntrustProp()
{
	if (m_cbSJType.IsWindowVisible())
	{
		int iIndex = m_cbStockAccount.GetCurSel();
		IStockAccountItem* pItem = m_pStockAccountList->GetItems(iIndex);
		CString sExchangeType = pItem->GetExchangeType();
		INodeTreeData* pSJType = NULL;
		if ((SHACCOUNT == sExchangeType) || (SHBACCOUNT == sExchangeType))
			pSJType = m_pSHSJType;			
		else
			pSJType = m_pSZSJType;
		iIndex = m_cbSJType.GetCurSel();
		INodeData* pData = pSJType->GetItems(iIndex);
		m_sEntrustProp = pData->Value();
		pData->Release();
	}
	return m_sEntrustProp;
}
void CTradeStockBaseDlg::OnCbnSelchangeCbStockaccount()
{
	// TODO: Add your control notification handler code here
	LoadSJType();
}

void CTradeStockBaseDlg::OnAmountEnter()
{

}

void CTradeStockBaseDlg::OnEnSetfocusEdAmount()
{
	// TODO: Add your control notification handler code here
	OnAmountEnter();
}

void CTradeStockBaseDlg::InitColor()
{
	COLORREF color = RGB(0, 0, 0);
	if (m_eBSDirection == bsBuy)
	{
		color = RGB(255, 0, 0);
	}
	else if (m_eBSDirection == bsSell)
	{
		color = RGB(0, 0, 255);
	}
	m_stStockAccount.SetTextColor(color);
	m_stStockCode.SetTextColor(color);
	m_stStockName.SetTextColor(color);
	m_stPrice.SetTextColor(color);
	m_stAmount.SetTextColor(color);
	m_stEnableAmount.SetTextColor(color);
	m_stEnableBalance.SetTextColor(color);
	m_stSJType.SetTextColor(color);
	m_stSeatNo.SetTextColor(color);
	m_stConferNo.SetTextColor(color);
	m_stTransCode.SetTextColor(color);
	m_btOK.SetForeColor(color);
}

void CTradeStockBaseDlg::OnShow(BOOL bShow)
{
}

void CTradeStockBaseDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	CString sStockCodeField;
	switch(pParam->m_nMsg)
	{
	case PUBLIC_MESSAGE_QUICKTRADE:
		MessageFillCodePrice(PARAM_QUICKTRADE_CODE, PARAM_QUICKTRADE_PRICE, pParam);
		break;
	case PUBLIC_MESSAGE_TRADE:
		MessageFillCodePrice(PARAM_TRADE_CODE, PARAM_TRADE_PRICE, pParam);
		break;
	case TRADE_MESSAGE_STOCKCODE:
		MessageFillCodePrice(TRADE_PARAM_STOCKCODE, "", pParam);
		break;
	}
}

void CTradeStockBaseDlg::OnOK()
{
	CWnd* pFocus = GetFocus();
	CWnd *pWnd = GetNextDlgTabItem(pFocus,FALSE);
	if (((&m_btOK) == pFocus) || ((&m_btOK) == pWnd))
	{
		OnBnClickedBtOk();
		return;
	}
	__super::OnOK();
}
void CTradeStockBaseDlg::OnParamChanged(IHSParam* pParam)
{
	if (pParam)
	{
		CString sStockCode = pParam->GetStringParam(STOCKCODE);
		if (!sStockCode.IsEmpty())
		{
			m_edStockCode.SetFocus();
			Clear();
			m_edStockCode.SetWindowText(sStockCode);
			CString sPrice = pParam->GetStringParam(ENTRUSTPRICE);
			if (!sPrice.IsEmpty())
			{
				m_edPrice.SetWindowText(sPrice);
				m_edPrice.SetKeyFlag(TRUE);
				m_edAmount.SetFocus();
				OnEntrustPriceExit();
			}
		}
	}
}

void CTradeStockBaseDlg::MessageFillCodePrice(CString sCodeField, CString sPriceField, CLinkageParam* pParam)
{
	if (IsShow())
	{
		CString sStockCode = pParam->GetStringField(sCodeField);
		if (!sStockCode.IsEmpty())
		{
			if(m_sLastStockCode != sStockCode)
			{
				m_edStockCode.SetFocus();
				Clear();
				m_edStockCode.SetWindowText(sStockCode);
			}
			if (!sPriceField.IsEmpty())
			{
				CString sPrice = pParam->GetStringField(sPriceField);
				if (!sPrice.IsEmpty())
				{
					m_edPrice.SetWindowText(sPrice);
					m_edPrice.SetKeyFlag(TRUE);
					m_edAmount.SetFocus();
					OnEntrustPriceExit();
				}
			}
		}
	}			
}