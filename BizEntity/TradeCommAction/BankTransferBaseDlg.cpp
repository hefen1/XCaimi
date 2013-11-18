/*******************************************************
  源程序名称:BankTransferBaseDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  银证转帐窗口基类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "BankTransferBaseDlg.h"
#include "HSMessage.h"
#include "TradeLangDefined.h"


#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CBankTransferBaseDlg, CTradeDialogBase)

CBankTransferBaseDlg::CBankTransferBaseDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CBankTransferBaseDlg::IDD, pParent)
{
	m_BankList = NULL;
	m_sDirection = "1";
	m_MoneyTypeList = NULL;
	m_keyboardDlg = NULL;
}

CBankTransferBaseDlg::~CBankTransferBaseDlg()
{
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
	if (m_MoneyTypeList)
	{
		m_MoneyTypeList->Release();
	}
	if(m_keyboardDlg)
		delete m_keyboardDlg;
}

BOOL CBankTransferBaseDlg::OnInit()
{
	if (__super::OnInit())
	{
		CString sMsg = "";
		m_BankList = m_pAccount->GetBusinessLogic()->GetBankInfo(sMsg);
		if (NULL == m_BankList)
		{
			MessageBoxA(sMsg, "提示");
			return FALSE;
		}
		else
		{
			m_MoneyTypeList = m_pAccountParamList->GetMoneyTypeList();
			m_edBalance.SetDecimal(2);
			int iCount = m_BankList->GetCount();
			for (int i = 0; i < iCount; i++)
			{
				CString sTmp;
				IBankItem* pItem = m_BankList->GetItems(i);
				sTmp = "[" + pItem->GetBankNo();
				sTmp += "]";
				sTmp += pItem->GetBankName();
				if (m_cbBankNo.FindString(-1, sTmp) == CB_ERR)
				{
					int nIndex = m_cbBankNo.AddString(sTmp);
					m_cbBankNo.SetItemDataPtr(nIndex, (void*)pItem);
				}
			}
			if (iCount > 0)
			{
				m_cbBankNo.SetCurSel(0);
				OnCbnSelchangeCbBankno();
			}
			SetSize();
			CString sCaption = m_pParam->GetStringParam("BtnCaption");
			if (!sCaption.IsEmpty())
			{
				m_btOK.SetWindowText(sCaption);
			}			
			RegisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
			return TRUE;
		}
	}
	else
		return FALSE;
}

void CBankTransferBaseDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		//
	}
}

void CBankTransferBaseDlg::SetSize()
{

}

BOOL CBankTransferBaseDlg::Validate()
{	
	BOOL bReturn = TRUE;
	if (m_edBalance.IsWindowVisible())
	{
		double dBalance = m_edBalance.GetDouble();
		if ( dBalance < ENTRUST_MINPRICE)
		{
			//CString sMsg = m_pTradeLang->LoadStr(GVN_TRADE_STOCK_PRICEERROR);
			MessageBox("请输入转帐金额！", "提示",  MB_ICONINFORMATION);
			m_edBalance.SetFocus();
			m_edBalance.SetSel(0, -1);
			bReturn = FALSE;
		}
		CString sHint;
		sHint.Format("转帐金额：%.2f\r\n\r\n确定要转帐吗？", dBalance);
		bReturn = (MessageBox(sHint,"提示",MB_ICONQUESTION|MB_OKCANCEL) == IDOK);
	}
	return bReturn;
}

BEGIN_MESSAGE_MAP(CBankTransferBaseDlg, CTradeDialogBase)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_CB_BANKNO, &CBankTransferBaseDlg::OnCbnSelchangeCbBankno)
	ON_BN_CLICKED(IDC_BT_OK, &CBankTransferBaseDlg::OnBnClickedBtOk)
	ON_BN_CLICKED(IDC_BT_BANK, &CBankTransferBaseDlg::OnBnClickedBtBank)
	ON_BN_CLICKED(IDC_BT_FUND, &CBankTransferBaseDlg::OnBnClickedBtFund)
	ON_EN_SETFOCUS(IDC_ED_FUNDPSW, &CBankTransferBaseDlg::OnEnSetfocusEdFundpsw)
	ON_EN_SETFOCUS(IDC_ED_BANKPSW, &CBankTransferBaseDlg::OnEnSetfocusEdBankpsw)
END_MESSAGE_MAP()


void CBankTransferBaseDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
}
void CBankTransferBaseDlg::LoadWnd()
{
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_stBankNo);
	m_paCWnd.Add(&m_stMoneyType);
	if (m_stBankPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_stBankPsw);
	}
	if (m_stFundPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_stFundPsw);
	}	
	m_paCWnd.Add(&m_stBalance);
	AdjustLayout(FIRST_COL_H, FIRST_COL_X, FIRST_COL_Y, FIRST_COL_Z);
	m_paCWnd.RemoveAll();
	m_paCWnd.Add(&m_cbBankNo);
	m_paCWnd.Add(&m_cbMoneyType);
	if (m_edBankPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_edBankPsw);
	}
	if (m_edFundPsw.IsWindowVisible())
	{
		m_paCWnd.Add(&m_edFundPsw);
	}	
	m_paCWnd.Add(&m_edBalance);
	AdjustLayout(SECOND_COL_H, SECOND_COL_X, SECOND_COL_Y, SECOND_COL_Z);	
	if (m_btBank.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edBankPsw, &m_btBank, 0, -1);
	}	
	if (m_btFund.IsWindowVisible())
	{
		AdjustLayoutEx(&m_edFundPsw, &m_btFund, 0, -1);
	}	
	AdjustLayoutEx(&m_edBalance, &m_btOK, 0, 2, adxBottomRight);
	SetComoboxHeight(&m_cbBankNo);
	SetComoboxHeight(&m_cbMoneyType);
	m_cbBankNo.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_cbMoneyType.SetWindowPos(&m_cbBankNo, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edBankPsw.SetWindowPos(&m_cbMoneyType, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edFundPsw.SetWindowPos(&m_edBankPsw, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_edBalance.SetWindowPos(&m_edFundPsw, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
	m_btOK.SetWindowPos(&m_edBalance, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 

}

void CBankTransferBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_BANKNO, m_cbBankNo);
	DDX_Control(pDX, IDC_CB_MONEYTYPE, m_cbMoneyType);
	DDX_Control(pDX, IDC_ED_BANKPSW, m_edBankPsw);
	DDX_Control(pDX, IDC_ED_FUNDPSW, m_edFundPsw);
	DDX_Control(pDX, IDC_ED_BALANCE, m_edBalance);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_ST_BANKNO, m_stBankNo);
	DDX_Control(pDX, IDC_ST_MONEYTYPE, m_stMoneyType);
	DDX_Control(pDX, IDC_ST_BANKPSW, m_stBankPsw);
	DDX_Control(pDX, IDC_ST_FUNDPSW, m_stFundPsw);
	DDX_Control(pDX, IDC_ST_BALANCE, m_stBalance);
	DDX_Control(pDX, IDC_BT_BANK, m_btBank);
	DDX_Control(pDX, IDC_BT_FUND, m_btFund);
}
void CBankTransferBaseDlg::OnCbnSelchangeCbBankno()
{
	// TODO: Add your control notification handler code here
	if (m_cbMoneyType.IsWindowVisible())
	{
		int nIndex = m_cbBankNo.GetCurSel();
		IBankItem* pBankItem = (IBankItem*)m_cbBankNo.GetItemDataPtr(nIndex);
		if (pBankItem)
		{
			m_cbMoneyType.ResetContent();
			CString sBankNo = pBankItem->GetBankNo();
			for (int i = 0; i < m_BankList->GetCount(); i++)
			{
				IBankItem* pItem = m_BankList->GetItems(i);
				if (pItem->GetBankNo() == sBankNo)
				{
					CString sTmp = "";
					sTmp.Format("%d", int(pItem->GetMoneyType()));
					if (m_MoneyTypeList)
					{
						for (int j = 0; j < m_MoneyTypeList->GetCount(); j++)
						{
							INodeData* pItem = m_MoneyTypeList->GetItems(j);
							CString sValue = pItem->Value();
							if (sTmp == sValue)
							{
								sTmp = pItem->Name(); 
								pItem->Release();
								break;
							}							
							else
								pItem->Release();
						}
						
					}
					if (m_cbMoneyType.FindString(-1, sTmp) == CB_ERR)
					{
						int nIndex = m_cbMoneyType.AddString(sTmp);
						m_cbMoneyType.SetItemDataPtr(nIndex, (void*)pItem);
					}
				}
			}
			if (m_cbMoneyType.GetCount() > 0)
			{
				m_cbMoneyType.SetCurSel(0);
			}	

		}
	}
	ShowPassword();
	LoadWnd();
}

void CBankTransferBaseDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (!Validate())
	{
		return;
	}
	m_btOK.EnableWindow(FALSE);
	int nIndex = m_cbMoneyType.GetCurSel();
	IBankItem* pBankItem = (IBankItem*)m_cbMoneyType.GetItemDataPtr(nIndex);
	if (pBankItem)
	{
		CString sMsg = "";
		CString sBankPsw = "";
		CString sFundPsw = "";
		double dBalance = 0;
		if (m_edBankPsw.IsWindowVisible())
		{
			sBankPsw = m_edBankPsw.GetRealText();
		}
		if (m_edFundPsw.IsWindowVisible())
		{
			sFundPsw = m_edFundPsw.GetRealText();
		}
		if (m_edBalance.IsWindowVisible())
		{
			dBalance = m_edBalance.GetDouble();
		}
		if (m_pAccount->GetBusinessLogic()->BankStockTransfer(pBankItem->GetMoneyType(), pBankItem->GetBankNo(), sBankPsw, sFundPsw, m_sDirection, dBalance, sMsg))
		{
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
			m_edBankPsw.SetWindowText("");
			m_edFundPsw.SetWindowText("");
			m_edBalance.SetValue(0);
		}
		else
		{
			MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		}
		m_cbBankNo.SetFocus();
	}
	m_btOK.EnableWindow();
}

void CBankTransferBaseDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CBankTransferBaseDlg::ShowPassword()
{
}
void CBankTransferBaseDlg::OnBnClickedBtBank()
{
	// TODO: Add your control notification handler code here
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
	}
	m_keyboardDlg->SetWnd(&m_edBankPsw);
	m_keyboardDlg->SetCharFilter(m_edBankPsw.GetCharFilter());
	if(!::IsWindow(m_keyboardDlg->m_hWnd))
	{
		m_keyboardDlg->Create(NULL);
	}
	if(m_keyboardDlg->IsWindowVisible())
	{
		m_keyboardDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		m_keyboardDlg->ShowWindow(SW_SHOW);
		MoveKeyboard(&m_btBank);
	}
}

void CBankTransferBaseDlg::OnBnClickedBtFund()
{
	// TODO: Add your control notification handler code here
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
	}
	m_keyboardDlg->SetWnd(&m_edFundPsw);
	m_keyboardDlg->SetCharFilter(m_edFundPsw.GetCharFilter());
	if(!::IsWindow(m_keyboardDlg->m_hWnd))
	{
		m_keyboardDlg->Create(NULL);
	}
	if(m_keyboardDlg->IsWindowVisible())
	{
		m_keyboardDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		m_keyboardDlg->ShowWindow(SW_SHOW);
		MoveKeyboard(&m_btFund);
	}
}

BOOL CBankTransferBaseDlg::MoveKeyboard(CWnd* pWnd)
{
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{	
		CRect rectBtn; 
		if(pWnd && ::IsWindow(pWnd->m_hWnd))
			pWnd->GetClientRect(&rectBtn);
		else
			return FALSE;

		CRect rectKeyboard; //软键盘矩形
		if(m_keyboardDlg && ::IsWindow(m_keyboardDlg->m_hWnd))
			m_keyboardDlg->GetClientRect(rectKeyboard);
		else
			return FALSE;

		int xScreen = ::GetSystemMetrics(SM_CXSCREEN); //获取当前屏幕宽度
		CPoint rb(rectBtn.right,rectBtn.bottom);   //获取按钮右下角坐标
		CPoint lb(rectBtn.left,rectBtn.bottom);    //获取按钮左下角坐标
		::ClientToScreen(pWnd->m_hWnd,&rb);
		::ClientToScreen(pWnd->m_hWnd,&lb);

		CPoint startPoint;
		if(rb.x + rectKeyboard.Width() < xScreen)
		{//显示在右边 可以不超过屏幕
			startPoint.x = rb.x;
			startPoint.y = rb.y - rectBtn.Height();
		}
		else if(lb.x - rectKeyboard.Width() > 0)
		{//显示在左边 可以不小于屏幕

			startPoint.x = lb.x - rectKeyboard.Width();
			startPoint.y = lb.y - rectBtn.Height();	
		}

		CPoint oldPoint(rectKeyboard.left,rectKeyboard.top);
		::ClientToScreen(m_keyboardDlg->m_hWnd,&oldPoint); //将软键盘矩形坐标转换为屏幕坐标
		if(startPoint == oldPoint) //不需要移动 
			return FALSE;

		m_keyboardDlg->MoveWindow(startPoint.x,startPoint.y,rectKeyboard.Width(),rectKeyboard.Height());
		return TRUE;
	}
	else
		return FALSE;
}

void CBankTransferBaseDlg::OnEnSetfocusEdFundpsw()
{
	// TODO: Add your control notification handler code here
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{		
		m_keyboardDlg->SetWnd(&m_edFundPsw);
		m_keyboardDlg->SetCharFilter(m_edFundPsw.GetCharFilter());
		MoveKeyboard(&m_btFund);
	}
	
}

void CBankTransferBaseDlg::OnEnSetfocusEdBankpsw()
{
	// TODO: Add your control notification handler code here
	MoveKeyboard(&m_btBank);
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{
		m_keyboardDlg->SetWnd(&m_edBankPsw);
		m_keyboardDlg->SetCharFilter(m_edBankPsw.GetCharFilter());
		MoveKeyboard(&m_btBank);
	}
}
void CBankTransferBaseDlg::OnWindowFocused()
{
	if (IsShow())
		m_cbBankNo.SetFocus();
}

void CBankTransferBaseDlg::OnOK()
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