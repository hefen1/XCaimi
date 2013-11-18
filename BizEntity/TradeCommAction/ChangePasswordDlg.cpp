/*******************************************************
  源程序名称:ChangePasswordDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  修改密码窗口基类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "ChangePasswordDlg.h"
#include "HSMessage.h"
#include "TradeLangDefined.h"


#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CChangePasswordDlg, CTradeDialogBase)

CChangePasswordDlg::CChangePasswordDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CChangePasswordDlg::IDD, pParent)
{
	m_sDirection = "1";
	m_keyboardDlg = NULL;
	m_PswTypeList = NULL;
}

CChangePasswordDlg::~CChangePasswordDlg()
{
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
	if(m_keyboardDlg)
		delete m_keyboardDlg;
	if (m_PswTypeList)
	{
		m_PswTypeList->Release();
	}
}

BOOL CChangePasswordDlg::OnInit()
{
	if (__super::OnInit())
	{
		m_PswTypeList = m_pAccountParamList->GetModifyPassowrdTypeList();
		if (m_PswTypeList)
		{
			for (int i = 0; i < m_PswTypeList->GetCount(); i++)
			{
				IModifyPassowrdTypeItem* pItem = m_PswTypeList->GetItems(i);
				m_cbPswType.AddString(pItem->Name());
				pItem->Release();
			}
			if (m_cbPswType.GetCount() > 0)
			{
				m_cbPswType.SetCurSel(0);
			}
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
	else
		return FALSE;
}

void CChangePasswordDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		//
	}
}

void CChangePasswordDlg::SetSize()
{

}

BOOL CChangePasswordDlg::Validate()
{	
	CString sOldPsw = m_edOldPsw.GetRealText();	
	if (sOldPsw.IsEmpty())
	{
		MessageBox("请输入原密码！", "提示", MB_ICONINFORMATION);
		m_edOldPsw.SetFocus();
		return FALSE;
	}
	CString sNewPsw = m_edNewPsw.GetRealText();
	if (sNewPsw.IsEmpty())
	{
		MessageBox("请输入新密码！", "提示", MB_ICONINFORMATION);
		m_edNewPsw.SetFocus();
		return FALSE;
	}
	CString sConfirmPsw = m_edConfirmPsw.GetRealText();
	if (sConfirmPsw.IsEmpty())
	{
		MessageBox("请输入确认密码！", "提示", MB_ICONINFORMATION);
		m_edConfirmPsw.SetFocus();
		return FALSE;
	}

	if (m_PswTypeList)
	{
		int nIndex = m_cbPswType.GetCurSel();
		IModifyPassowrdTypeItem* pItem = m_PswTypeList->GetItems(nIndex);
		CString sPswType = pItem->Value();
		pItem->Release();
		if ((PASSWORD_TYPE_TRADE == sPswType) && (sOldPsw != m_pAccount->GetPassword()))
		{
			MessageBox("原密码错误！", "提示", MB_ICONINFORMATION);
			m_edOldPsw.SetFocus();
			return FALSE;
		}
	}
	if (sNewPsw != sConfirmPsw)
	{
		MessageBox("您所填写的两次新密码不一致，请重新输入！", "提示", MB_ICONINFORMATION);
		m_edNewPsw.SetFocus();
		return FALSE;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CChangePasswordDlg, CTradeDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_OK, &CChangePasswordDlg::OnBnClickedBtOk)
	ON_BN_CLICKED(IDC_BT_OLD, &CChangePasswordDlg::OnBnClickedBtOld)
	ON_BN_CLICKED(IDC_BT_NEW, &CChangePasswordDlg::OnBnClickedBtNew)
	ON_BN_CLICKED(IDC_BT_CONFIRM, &CChangePasswordDlg::OnBnClickedBtConfirm)
	ON_EN_SETFOCUS(IDC_ED_OLDPSW, &CChangePasswordDlg::OnEnSetfocusEdOldpsw)
	ON_EN_SETFOCUS(IDC_ED_NEWPSW, &CChangePasswordDlg::OnEnSetfocusEdNewpsw)
	ON_EN_SETFOCUS(IDC_ED_CONFIRMPSW, &CChangePasswordDlg::OnEnSetfocusEdConfirmpsw)
END_MESSAGE_MAP()


void CChangePasswordDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
}
void CChangePasswordDlg::LoadWnd()
{
	/*m_paCWnd.RemoveAll();
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
	m_btOK.SetWindowPos(&m_edBalance, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); */

}

void CChangePasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
	DDX_Control(pDX, IDC_CB_PSWTYPE, m_cbPswType);
	DDX_Control(pDX, IDC_ED_OLDPSW, m_edOldPsw);
	DDX_Control(pDX, IDC_ED_NEWPSW, m_edNewPsw);
	DDX_Control(pDX, IDC_ED_CONFIRMPSW, m_edConfirmPsw);
	DDX_Control(pDX, IDC_BT_OLD, m_btOld);
	DDX_Control(pDX, IDC_BT_NEW, m_btNew);
	DDX_Control(pDX, IDC_BT_CONFIRM, m_btConfirm);
}

void CChangePasswordDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (!Validate())
	{
		return;
	}
	m_btOK.EnableWindow(FALSE);
	CString sMsg = "";
	int nIndex = m_cbPswType.GetCurSel();
	CString sPswType = PASSWORD_TYPE_TRADE;
	if (m_PswTypeList)
	{
		IModifyPassowrdTypeItem* pItem = m_PswTypeList->GetItems(nIndex);
		sPswType = pItem->Value();
		pItem->Release();
	}	
	CString sOldPsw = m_edOldPsw.GetRealText();
	CString sNewPsw = m_edNewPsw.GetRealText();
	if (m_pAccount->GetBusinessLogic()->ChangePassword(sPswType, sOldPsw, sNewPsw, sMsg))
	{
		if (PASSWORD_TYPE_TRADE == sPswType)
		{
			m_pAccount->SetPassword(sNewPsw);
		}		
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
		m_edOldPsw.SetWindowText("");
		m_edNewPsw.SetWindowText("");
		m_edConfirmPsw.SetWindowText("");
	}
	else
	{
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
	m_edOldPsw.SetFocus();
	m_btOK.EnableWindow();
}

void CChangePasswordDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

BOOL CChangePasswordDlg::MoveKeyboard(CWnd* pWnd)
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
void CChangePasswordDlg::OnBnClickedBtOld()
{
	// TODO: Add your control notification handler code here
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
	}
	m_keyboardDlg->SetWnd(&m_edOldPsw);
	m_keyboardDlg->SetCharFilter(m_edOldPsw.GetCharFilter());
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
		MoveKeyboard(&m_btOld);
	}
}

void CChangePasswordDlg::OnBnClickedBtNew()
{
	// TODO: Add your control notification handler code here
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
	}
	m_keyboardDlg->SetWnd(&m_edNewPsw);
	m_keyboardDlg->SetCharFilter(m_edNewPsw.GetCharFilter());
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
		MoveKeyboard(&m_btNew);
	}
}

void CChangePasswordDlg::OnBnClickedBtConfirm()
{
	// TODO: Add your control notification handler code here
	if(!m_keyboardDlg)
	{
		m_keyboardDlg = new CKeyboardDlg();
	}
	m_keyboardDlg->SetWnd(&m_edConfirmPsw);
	m_keyboardDlg->SetCharFilter(m_edConfirmPsw.GetCharFilter());
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
		MoveKeyboard(&m_btConfirm);
	}

}

void CChangePasswordDlg::OnEnSetfocusEdOldpsw()
{
	// TODO: Add your control notification handler code here
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{		
		m_keyboardDlg->SetWnd(&m_edOldPsw);
		m_keyboardDlg->SetCharFilter(m_edOldPsw.GetCharFilter());
		MoveKeyboard(&m_btOld);
	}
}

void CChangePasswordDlg::OnEnSetfocusEdNewpsw()
{
	// TODO: Add your control notification handler code here
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{		
		m_keyboardDlg->SetWnd(&m_edNewPsw);
		m_keyboardDlg->SetCharFilter(m_edNewPsw.GetCharFilter());
		MoveKeyboard(&m_btNew);
	}
}

void CChangePasswordDlg::OnEnSetfocusEdConfirmpsw()
{
	// TODO: Add your control notification handler code here
	if (m_keyboardDlg && m_keyboardDlg->IsWindowVisible())
	{		
		m_keyboardDlg->SetWnd(&m_edConfirmPsw);
		m_keyboardDlg->SetCharFilter(m_edConfirmPsw.GetCharFilter());
		MoveKeyboard(&m_btConfirm);
	}
}

void CChangePasswordDlg::OnWindowFocused()
{
	if (IsShow())
		m_cbPswType.SetFocus();
}

void CChangePasswordDlg::OnOK()
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