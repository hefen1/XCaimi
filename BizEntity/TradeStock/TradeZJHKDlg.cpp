/*******************************************************
  源程序名称:TradeZJHKDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  直接还款窗口类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "TradeZJHKDlg.h"
#include "HSMessage.h"
#include "TradeLangDefined.h"
#include "TradeAction.h"


#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeZJHKDlg, CTradeDialogBase)

CTradeZJHKDlg::CTradeZJHKDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CTradeZJHKDlg::IDD, pParent)
{
	m_MoneyTypeList = NULL;
}

CTradeZJHKDlg::~CTradeZJHKDlg()
{
	if (m_MoneyTypeList)
	{
		m_MoneyTypeList->Release();
	}
}

BOOL CTradeZJHKDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_MoneyTypeList = m_pAccountParamList->GetMoneyTypeList();
		m_edBalance.SetDecimal(2);
		if (m_MoneyTypeList)
		{
			for (int j = 0; j < m_MoneyTypeList->GetCount(); j++)
			{
				CString sTmp = "";
				INodeData* pItem = m_MoneyTypeList->GetItems(j);
				sTmp = pItem->Name(); 
				pItem->Release();
				m_cbMoneyType.AddString(sTmp);
			}
			if (m_cbMoneyType.GetCount() > 0)
			{
				m_cbMoneyType.SetCurSel(0);
			}	
		}
		CString sCaption = m_pParam->GetStringParam("BtnCaption");
		if (!sCaption.IsEmpty())
		{
			m_btOK.SetWindowText(sCaption);
		}			
		RegisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
		return TRUE;
	}
	else
		return FALSE;
}
void CTradeZJHKDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	if (TRADE_RZRQ_QUERYDEBITTOTAL == iAction)
	{
		m_edEnableBalance.SetWindowText("");
		IReqQueryFund* pIntf = dynamic_cast<IReqQueryFund*>(pTrading);
		if (pIntf->GetSuccessful() && pIntf->HaveData())
		{
			IFundList* pList = pIntf->GetDataList();
			IFundItem* pItem = pList->GetItems(0);
			CString sTmp = "";
			sTmp.Format("%.2f", pItem->GetEnableBalance());
			m_edEnableBalance.SetWindowText(sTmp);		
			sTmp.Format("%.2f", pItem->GetFinanceDebit());
			m_edRealBalance.SetWindowText(sTmp);		
		}
		pTrading->FreeDataList();
	}
}

void CTradeZJHKDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		QueryEnableBalance();
	}
}


BOOL CTradeZJHKDlg::Validate()
{
	double dBalance = m_edBalance.GetDouble();
	if ( dBalance < ENTRUST_MINPRICE)
	{
		//CString sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_PRICEERROR);
		MessageBox("请输入还款金额！", "提示",  MB_ICONINFORMATION);
		m_edBalance.SetFocus();
		m_edBalance.SetSel(0, -1);
		return FALSE;
	}	
	CString sHint;
	sHint.Format("还款金额：%.2f\r\n\r\n确定要还款吗？", dBalance);
	return MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK;
}

BEGIN_MESSAGE_MAP(CTradeZJHKDlg, CTradeDialogBase)

	ON_BN_CLICKED(IDC_BT_OK, &CTradeZJHKDlg::OnBnClickedBtOk)
END_MESSAGE_MAP()


void CTradeZJHKDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CB_MONEYTYPE, m_cbMoneyType);
	DDX_Control(pDX, IDC_ED_BALANCE, m_edBalance);
	DDX_Control(pDX, IDC_ED_ENABLEBALANCE, m_edEnableBalance);
	DDX_Control(pDX, IDC_ED_REALBALANCE, m_edRealBalance);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
}

void CTradeZJHKDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CTradeZJHKDlg::OnWindowFocused()
{
	if (IsShow())
		m_edBalance.SetFocus();
}
void CTradeZJHKDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (Validate())
	{
		m_btOK.EnableWindow(FALSE);
		CString sMoneyType = "0";
		if (m_MoneyTypeList)
		{
			int nIndex = m_cbMoneyType.GetCurSel();
			INodeData* pItem = m_MoneyTypeList->GetItems(nIndex);
			sMoneyType = pItem->Value();
			pItem->Release();
		}
		CString sMsg = "";
		if (m_pAccount->GetBusinessLogic()->RZRQZJHK(sMoneyType, m_edBalance.GetDouble(), sMsg))
		{
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			m_edBalance.SetWindowText("");
			QueryEnableBalance();
			PostRefreshMsg();
		}
		else
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_btOK.EnableWindow();
	}
}

void CTradeZJHKDlg::QueryEnableBalance()
{
	IReqQueryFund* pIntf = dynamic_cast<IReqQueryFund*>(m_pTradeCoreObj->NewTradingByAction(TRADE_RZRQ_QUERYDEBITTOTAL));
	pIntf->SetAccount(m_pAccount);
	pIntf->TradingAsync(this);
}