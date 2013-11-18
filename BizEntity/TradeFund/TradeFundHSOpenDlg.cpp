/*******************************************************
  源程序名称:TradeFundHSOpenDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  基金委托窗口基类实现
  作    者:  shenglq
  开发日期:  20100903
*********************************************************/
#include "stdafx.h"
#include "TradeFundHSOpenDlg.h"
#include "TradeFunc.h"
#include "TradeFundEntrustIntf.h"

#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CTradeFundHSOpenDlg, CTradeDialogBase)

CTradeFundHSOpenDlg::CTradeFundHSOpenDlg(CWnd* pParent /* = NULL */)
	: CTradeDialogBase(CTradeFundHSOpenDlg::IDD, pParent)
{

}

CTradeFundHSOpenDlg::~CTradeFundHSOpenDlg()
{
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
}

BOOL CTradeFundHSOpenDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		RegisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
		CString sMsg = "";
		CString sCaption = m_pParam->GetStringParam("BtnCaption");
		if (!sCaption.IsEmpty())
		{
			m_btOK.SetWindowText(sCaption);
		}			
		return TRUE;
	}
	else
		return FALSE;
	 
}

void CTradeFundHSOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_CB_FUNDCOMPANY, m_cbFundCompany);
}

BEGIN_MESSAGE_MAP(CTradeFundHSOpenDlg, CTradeDialogBase)

	ON_BN_CLICKED(IDC_BT_OK, &CTradeFundHSOpenDlg::OnBnClickedBtOk)
END_MESSAGE_MAP()

void CTradeFundHSOpenDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		m_cbFundCompany.ResetContent();
		CString sMsg = "";
		IFundCompanyList* pCompanyList = m_pBusinessLogic->GetFundCompany(sMsg);
		IFundStockAccountList* pStockAccountList = m_pBusinessLogic->GetFundStockAccount(sMsg);
		if (pCompanyList)
		{
			for (int i = 0; i < pCompanyList->GetCount(); i++)
			{
				IFundCompanyItem* pCompanyItem = pCompanyList->GetItems(i);
				CString sFundCompany = pCompanyItem->GetFundCompany();
				if (pStockAccountList)
				{
					IFundStockAccountItem* pStockAccountItem = pStockAccountList->GetByFundCompany(sFundCompany);
					if (pStockAccountItem)
					{
						continue;
					}
				}
				int nIndex = m_cbFundCompany.AddString(pCompanyItem->GetCompanyName());
				m_cbFundCompany.SetItemDataPtr(nIndex, (void*)pCompanyItem);
			}
		}
		if (m_cbFundCompany.GetCount() > 0)
		{
			m_cbFundCompany.SetCurSel(0);
			m_btOK.EnableWindow();
		}
		else
			m_btOK.EnableWindow(FALSE);
	}
}

void CTradeFundHSOpenDlg::OnWindowFocused()
{
	if (IsShow())
	{
		m_cbFundCompany.SetFocus();
	}
}

void CTradeFundHSOpenDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CTradeFundHSOpenDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (m_btOK.IsWindowEnabled())
	{
		m_btOK.EnableWindow(FALSE);
		CString sMsg = "";
		try
		{
			CString sFundCompany = "";
			int nIndex = m_cbFundCompany.GetCurSel();
			IFundCompanyItem* pCompanyItem = (IFundCompanyItem*)m_cbFundCompany.GetItemDataPtr(nIndex);
			sFundCompany = pCompanyItem->GetFundCompany();
			IReqFundEntrust* pIntf = dynamic_cast<IReqFundEntrust*>(m_pTradeCoreObj->NewTradingByAction(m_iAction));
			pIntf->Clear();
			pIntf->SetAccount(m_pAccount);
			pIntf->SetFundCompany(sFundCompany);
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
							sMsg.Format("开户请求已提交，流水号：%s", pItem->GetSerialNo());
							CString sTmp = "";
							m_pBusinessLogic->GetFundStockAccount(sTmp, TRUE);
						}
						else
						{
							sMsg = pItem->GetErrorInfo();
						}
					}
					pIntf->FreeDataList();
				}		
			}
			else
			{
				sMsg = pIntf->GetLastError();
			}
			pIntf->Release();
		}
		catch (...)
		{
			sMsg = "获取接口错误！";
		}	
		MessageBoxA(sMsg, "提示");
		m_btOK.EnableWindow();
	}
}

void CTradeFundHSOpenDlg::OnOK()
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