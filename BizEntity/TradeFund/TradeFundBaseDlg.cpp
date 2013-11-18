/*******************************************************
  源程序名称:TradeFundBaseDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金委托窗口基类实现
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeFundBaseDlg.h"
#include "TradeConst.h"
#include "TradeFunc.h"
#include "QueryFundIntf.h"
#include "TradeAction.h"

#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeFundBaseDlg, CTradeDialogBase)

CTradeFundBaseDlg::CTradeFundBaseDlg(CWnd* pParent /* = NULL */)
	: CTradeDialogBase(CTradeFundBaseDlg::IDD, pParent)
{
	m_FundInfoList = NULL;
	m_FundInfoItem = NULL;
	m_pExceedFlag = NULL;
	m_pChargeType = NULL;
	m_pDividendMethod = NULL;
}

CTradeFundBaseDlg::~CTradeFundBaseDlg()
{
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
	UnregisterLinkageMsg(TRADE_MESSAGE_FUNDCODE);
	if (m_pExceedFlag)
	{
		m_pExceedFlag->Release();
	}
	if (m_pChargeType)
	{
		m_pChargeType->Release();
	}
	if (m_pDividendMethod)
	{
		m_pDividendMethod->Release();
	}

}

BOOL CTradeFundBaseDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		m_dtHopeDate.SetFormat("yyyy-MM-dd");
		m_dtBeginDate.SetFormat("yyyy-MM-dd");
		m_dtEndDate.SetFormat("yyyy-MM-dd");
		CTime tTime = CTime::GetCurrentTime();
		m_dtHopeDate.SetTime(&tTime);
		m_dtBeginDate.SetTime(&tTime);
		m_dtEndDate.SetTime(&tTime);
		m_edBalance.SetDecimal(0);
		m_edFundDate.SetDecimal(0);
		m_edFundDate.SetMax(31);
		m_edFundDate.SetMin(1);
		m_edFundDate.SetValue(1);
		RegisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
		CString sMsg = "";
		m_FundInfoList = m_pAccount->GetBusinessLogic()->GetFundInfo(sMsg);
		if (NULL == m_FundInfoList)
		{
			MessageBoxA(sMsg, "提示");
			return FALSE;
		}
		m_pExceedFlag = m_pAccountParamList->GetExceedFlagList();
		LoadList(m_pExceedFlag, m_cbExceedFlag);
		m_pChargeType = m_pAccountParamList->GetChargeTypeList();
		LoadList(m_pChargeType, m_cbChargeType);
		m_pDividendMethod = m_pAccountParamList->GetDividendMethodList();
		LoadList(m_pDividendMethod, m_cbDividendMethod);
		CString sCaption = m_pParam->GetStringParam("BtnCaption");
		if (!sCaption.IsEmpty())
		{
			m_btOK.SetWindowText(sCaption);
		}	
		RegisterLinkageMsg(TRADE_MESSAGE_FUNDCODE);
		return TRUE;
	}
	else
		return FALSE;
	 
}

void CTradeFundBaseDlg::TradingAsyncCallBack(ITrading* pTrading, int iAction)
{
	switch (iAction)
	{
	case TRADE_QUERYFUND:
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
			}
			pTrading->FreeDataList();
			break;
		}
	case TRADE_FUND_QUERYHOLD:
		{
			m_edEnableBalance.SetWindowText("");
			IReqQueryFundHold* pIntf = dynamic_cast<IReqQueryFundHold*>(pTrading);
			if (pIntf->GetSuccessful() && pIntf->HaveData())
			{
				IFundHoldList* pList = pIntf->GetDataList();
				for (int i = 0; i < pList->GetCount(); i++)
				{
					IFundHoldItem* pItem = pList->GetItems(i);
					if (pItem->GetFundCode() == m_sLastFundCode)
					{
						m_sAllotDate = pItem->GetAllotDate();
						m_sAllotNo = pItem->GetAllotNo();
						CString sTmp = pItem->GetEnableShares();
						m_edEnableBalance.SetWindowText(sTmp);
						break;
					}					
				}		
			}
			pTrading->FreeDataList();
			break;
		}
	}

}

void CTradeFundBaseDlg::OnFundCodeExit()
{
	if(m_edFundCode.GetWindowTextLength() == STOCKCODE_LENGTH)
	{
		CString sFundCode;
		m_edFundCode.GetWindowText(sFundCode);
		if (m_sLastFundCode == sFundCode)
		{
			return;
		}
		m_FundInfoItem = m_FundInfoList->GetByFundCode(sFundCode);	
		if (m_FundInfoItem)
		{
			CString sTmp;
			sTmp = m_FundInfoItem->GetFundName(); 
			m_edFundName.SetWindowText(sTmp);
			sTmp = m_FundInfoItem->GetNav();
			m_edFundNav.SetWindowText(sTmp);
		}		
	}
}


BOOL CTradeFundBaseDlg::Validate()
{
	if(m_edFundCode.GetWindowTextLength() != FUNDCODE_LENGTH)
	{
		CString  sMsg = m_pTradeLang->LoadStr(GVN_TRADE_FUND_CODEERROR);		
		sMsg.Format(sMsg, FUNDCODE_LENGTH);
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edFundCode.SetFocus();
		m_edFundCode.SetSel(0, -1);
		return FALSE;
	}
	else
		return TRUE;
}
void CTradeFundBaseDlg::DoEntrust()
{
	CString sFundCode = "";
	m_edFundCode.GetWindowText(sFundCode);
	double dBalance = m_edBalance.GetDouble();
	CString sMsg = "";
	/*if (m_pBusinessLogic->StockEntrust(sExchangeType, sStockAccount,sStockCode,m_eBSDirection, dPrice, iAmount, GetEntrustProp(), sMsg))
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		PostRefreshMsg();
		Clear();
	}
	else
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}*/
	m_sLastFundCode = "";
	m_edFundCode.SetFocus();
}

void CTradeFundBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_ED_FUNDCODE, m_edFundCode);
	DDX_Control(pDX, IDC_ED_FUNDNAME, m_edFundName);
	DDX_Control(pDX, IDC_ED_FUNDNAV, m_edFundNav);
	DDX_Control(pDX, IDC_ED_BALANCE, m_edBalance);
	DDX_Control(pDX, IDC_ST_ENABLEBALANCE, m_stEnableBalance);
	DDX_Control(pDX, IDC_ED_ENABLEBALANCE, m_edEnableBalance);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_ST_FUND, m_stFundCode);
	DDX_Control(pDX, IDC_ST_BALANCE, m_stBalance);
	DDX_Control(pDX, IDC_ST_FUNDNAME, m_stFundName);
	DDX_Control(pDX, IDC_ST_FUNDNAV, m_stFundNav);
	DDX_Control(pDX, IDC_CB_CHARGETYPE, m_cbChargeType);
	DDX_Control(pDX, IDC_ST_CHARGETYPE, m_stChargeType);
	DDX_Control(pDX, IDC_ST_DIVIDENDMETHOD, m_stDividendMethod);
	DDX_Control(pDX, IDC_CB_DIVIDENDMETHOD, m_cbDividendMethod);
	DDX_Control(pDX, IDC_ST_EXCEEDFLAG, m_stExceedFlag);
	DDX_Control(pDX, IDC_CB_EXCEEDFLAG, m_cbExceedFlag);
	DDX_Control(pDX, IDC_ST_TRANSCODE, m_stTransCode);
	DDX_Control(pDX, IDC_ED_TRANSCODE, m_edTransCode);
	DDX_Control(pDX, IDC_ST_HOPEDATE, m_stHopeDate);
	DDX_Control(pDX, IDC_DT_HOPEDATE, m_dtHopeDate);
	DDX_Control(pDX, IDC_ST_BEGINDATE, m_stBeginDate);
	DDX_Control(pDX, IDC_DT_BEGINDATE, m_dtBeginDate);
	DDX_Control(pDX, IDC_ST_ENDDATE, m_stEndDate);
	DDX_Control(pDX, IDC_DT_ENDDATE, m_dtEndDate);
	DDX_Control(pDX, IDC_ST_FUNDDATE, m_stFundDate);
	DDX_Control(pDX, IDC_ED_FUNDDATE, m_edFundDate);
}

BEGIN_MESSAGE_MAP(CTradeFundBaseDlg, CTradeDialogBase)
ON_BN_CLICKED(IDC_BT_OK, &CTradeFundBaseDlg::OnBnClickedBtOk)
ON_EN_KILLFOCUS(IDC_ED_FUNDCODE, &CTradeFundBaseDlg::OnEnKillfocusEdFundcode)
ON_EN_CHANGE(IDC_ED_FUNDCODE, &CTradeFundBaseDlg::OnEnChangeEdFundcode)
END_MESSAGE_MAP()


void CTradeFundBaseDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here	
	if (Validate())
	{
		m_btOK.EnableWindow(FALSE);
		DoEntrust();
		m_btOK.EnableWindow();
	}
}
void CTradeFundBaseDlg::OnEnKillfocusEdFundcode()
{
	// TODO: Add your control notification handler code here	
	OnFundCodeExit();
	m_edFundCode.GetWindowText(m_sLastFundCode);
}


void CTradeFundBaseDlg::OnEnChangeEdFundcode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CTradeDialogBase::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if(m_edFundCode.GetWindowTextLength() == FUNDCODE_LENGTH)
	{
		CString sFundCode;
		m_edFundCode.GetWindowText(sFundCode);
		if (m_sLastFundCode != sFundCode)
		{
			Clear(1);
		}

		if (m_edBalance.IsWindowVisible() && m_edBalance.IsWindowEnabled())
		{
			OnCodeChangedFocus();
		}
	}
	else
	{
		Clear(1);
	}
}

void CTradeFundBaseDlg::Clear(int iFlag /* = 0 */)
{
	if ("" != m_sLastFundCode)
	{
		m_sLastFundCode = "";
		if (!iFlag)
		{
			m_edFundCode.SetWindowText("");
		}
		m_edFundName.SetWindowText("");
		m_edFundNav.SetWindowText("");
		if (m_edBalance.IsWindowVisible())
		{
			m_edBalance.SetKeyFlag();
			m_edBalance.SetWindowText("");
		}		
		m_edEnableBalance.SetWindowText("");
	}	
}

void CTradeFundBaseDlg::OnWindowFocused()
{
	if (IsShow())
		m_edFundCode.SetFocus();
}

void CTradeFundBaseDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CTradeFundBaseDlg::QueryEnableBalance()
{
	if(m_edFundCode.GetWindowTextLength() != FUNDCODE_LENGTH)
		return;
	CString sFundCode = "";
	m_edFundCode.GetWindowText(sFundCode);
	if (sFundCode == m_sLastFundCode)
	{
		return;
	}
	IReqQueryFund* pIntf = dynamic_cast<IReqQueryFund*>(m_pTradeCoreObj->NewTradingByAction(TRADE_QUERYFUND));
	pIntf->SetAccount(m_pAccount);
	pIntf->SetMoneyType(mtCNY);		
	pIntf->TradingAsync(this);
}

void CTradeFundBaseDlg::QueryEnableAmount()
{
	if(m_edFundCode.GetWindowTextLength() != FUNDCODE_LENGTH)
		return;

	CString sStockCode = "";
	m_edFundCode.GetWindowText(sStockCode);
	if ((sStockCode == m_sLastFundCode))
	{
		return;
	}
	m_sAllotNo = "";
	m_sAllotDate = "";
	if (m_FundInfoItem)
	{
		IReqQueryFundHold* pIntf = dynamic_cast<IReqQueryFundHold*>(m_pTradeCoreObj->NewTradingByAction(TRADE_FUND_QUERYHOLD));
		pIntf->SetAccount(m_pAccount);
		pIntf->SetFundCode(sStockCode);
		pIntf->SetFundCompany(m_FundInfoItem->GetFundCompany());
		pIntf->TradingAsync(this);
	}
}

void CTradeFundBaseDlg::LoadList(INodeTreeData* pList, CComboBox& AComboBox)
{
	if (pList)
	{
		AComboBox.ResetContent();
		int iCount = pList->GetCount();
		for (int i = 0; i < iCount; i++)
		{
			INodeData* pItem = pList->GetItems(i);
			AComboBox.AddString(pItem->Name());
			pItem->Release();
		}
		if (iCount > 0)
		{
			AComboBox.SetCurSel(0);
		}
	}
}

CString CTradeFundBaseDlg::GetListValue(INodeTreeData* pList, CComboBox& AComboBox)
{
	if (pList)
	{
		int iIndex = AComboBox.GetCurSel();
		INodeData* pData = pList->GetItems(iIndex);
		CString sValue = pData->Value();
		pData->Release();
		return sValue;
	}
	else
		return "";
}

void CTradeFundBaseDlg::OnCodeChangedFocus()
{
	m_edBalance.SetFocus();
}

void CTradeFundBaseDlg::OnOK()
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

void CTradeFundBaseDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	switch(pParam->m_nMsg)
	{
	case TRADE_MESSAGE_FUNDCODE:
		if (IsShow())
		{
			CString sFundCode = pParam->GetStringField(TRADE_PARAM_FUNDCODE);
			if (!sFundCode.IsEmpty())
			{
				if(m_sLastFundCode != sFundCode)
				{
					m_edFundCode.SetFocus();
					Clear();
					m_edFundCode.SetWindowText(sFundCode);
				}
			}
		}		
		break;
	}
}