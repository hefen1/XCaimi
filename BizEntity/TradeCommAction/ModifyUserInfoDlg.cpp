/*******************************************************
  源程序名称:ModifyUserInfoDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  修改用户信息窗口基类
  作    者:  shenglq
  开发日期:  20110310
*********************************************************/
#include "stdafx.h"
#include "ModifyUserInfoDlg.h"
#include "HSMessage.h"
#include "TradeLangDefined.h"
#include "TradeAction.h"
#include "ModifyUserInfoIntf.h"

#define HOTKEY_ENTRUST_ID	1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CModifyUserInfoDlg, CTradeDialogBase)

CModifyUserInfoDlg::CModifyUserInfoDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CModifyUserInfoDlg::IDD, pParent)
{
	m_UserInfo = NULL;
	m_IDKindList = NULL;
}

CModifyUserInfoDlg::~CModifyUserInfoDlg()
{
	UnregisterHotKey(HOTKEY_ENTRUST_ID, 0, VK_F3);
	if (m_IDKindList)
	{
		m_IDKindList->Release();
	}
}

BOOL CModifyUserInfoDlg::OnInit()
{
	if (__super::OnInit())
	{
		CString sMsg = "";
		m_UserInfo = m_pAccount->GetBusinessLogic()->GetUserInfo(sMsg);
		m_IDKindList = m_pAccountParamList->GetIDKindList();
		if (NULL == m_UserInfo)
		{
			MessageBoxA(sMsg, "提示");
			return FALSE;
		}
		m_edClientName.EnableWindow(FALSE);
		m_edIDNo.EnableWindow(FALSE);
		m_edIDKind.EnableWindow(FALSE);
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

void CModifyUserInfoDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		CString sTmp = "";
		sTmp = m_pAccount->GetUserName();
		m_edClientName.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetEMail();
		m_edEMail.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetIdKind();
		if (m_IDKindList)
		{
			INodeData* pItem = m_IDKindList->GetItemsByName(sTmp);
			if (pItem)
			{
				sTmp = pItem->Value();
			}
		}
		m_edIDKind.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetIdNo();
		m_edIDNo.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetIdAddress();
		m_edIDAddress.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetPhoneCode();
		m_edPhoneCode.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetMobiletelePhone();
		m_edMobiletelePhone.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetFax();
		m_edFax.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetZipCode();
		m_edZipCode.SetWindowText(sTmp);
		sTmp = m_UserInfo->GetAddress();
		m_edAddress.SetWindowText(sTmp);
	}
}

void CModifyUserInfoDlg::SetSize()
{

}

BOOL CModifyUserInfoDlg::Validate()
{	
	if (m_edZipCode.IsWindowVisible() && (m_edZipCode.GetWindowTextLength() != 6))
	{
		MessageBox("请填写6位邮政编码！", "提示", MB_ICONINFORMATION);
		m_edZipCode.SetFocus();
		return FALSE;
	}
	return TRUE;
}

BEGIN_MESSAGE_MAP(CModifyUserInfoDlg, CTradeDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_OK, &CModifyUserInfoDlg::OnBnClickedBtOk)
END_MESSAGE_MAP()


void CModifyUserInfoDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
}
void CModifyUserInfoDlg::LoadWnd()
{
}

void CModifyUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ED_CLIENTNAME, m_edClientName);
	DDX_Control(pDX, IDC_ED_EMAIL, m_edEMail);
	DDX_Control(pDX, IDC_ED_IDKIND, m_edIDKind);
	DDX_Control(pDX, IDC_ED_IDNO, m_edIDNo);
	DDX_Control(pDX, IDC_ED_IDADDRESS, m_edIDAddress);
	DDX_Control(pDX, IDC_ED_PHONECODE, m_edPhoneCode);
	DDX_Control(pDX, IDC_ED_MOBILETELEPHONE, m_edMobiletelePhone);
	DDX_Control(pDX, IDC_ED_FAX, m_edFax);
	DDX_Control(pDX, IDC_ED_ZIPCODE, m_edZipCode);
	DDX_Control(pDX, IDC_ED_ADDRESS, m_edAddress);
	DDX_Control(pDX, IDC_BT_OK, m_btOK);
}


void CModifyUserInfoDlg::OnHotKey(UINT nId)
{
	if (IsShow())
	{
		if (HOTKEY_ENTRUST_ID == nId)
		{
			OnBnClickedBtOk();
		}
	}	
}

void CModifyUserInfoDlg::OnWindowFocused()
{
	if (IsShow())
		m_edEMail.SetFocus();
}
void CModifyUserInfoDlg::OnBnClickedBtOk()
{
	// TODO: Add your control notification handler code here
	if (Validate())
	{
		CString sMsg = "";
		try
		{
			IReqModifyUserInfo* pIntf = dynamic_cast<IReqModifyUserInfo*>(m_pTradeCoreObj->NewTradingByAction(TRADE_MODIFYUSERINFO));
			pIntf->Clear();
			pIntf->SetAccount(m_pAccount);
			pIntf->SetIdKind(m_UserInfo->GetIdKind());
			pIntf->SetIdNo(m_UserInfo->GetIdNo());
			pIntf->SetEMail(m_UserInfo->GetEMail());
			CString sTmp = "";
			m_edAddress.GetWindowText(sTmp);
			pIntf->SetAddress(sTmp);
			m_edIDAddress.GetWindowText(sTmp);
			pIntf->SetIdAddress(sTmp);
			m_edPhoneCode.GetWindowText(sTmp);
			pIntf->SetPhoneCode(sTmp);
			m_edMobiletelePhone.GetWindowText(sTmp);
			pIntf->SetMobiletelePhone(sTmp);
			m_edAddress.GetWindowText(sTmp);
			pIntf->SetAddress(sTmp);
			m_edFax.GetWindowText(sTmp);
			pIntf->SetFax(sTmp);
			m_edZipCode.GetWindowText(sTmp);
			pIntf->SetZipCode(sTmp);
			m_edEMail.GetWindowText(sTmp);
			pIntf->SetEMail(sTmp);
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
							sMsg = "修改用户信息成功！";
							IUserInfoItem* pUserInfoItem = m_pAccount->GetBusinessLogic()->GetUserInfo(sTmp, TRUE);
							if(pUserInfoItem)
								m_UserInfo = pUserInfoItem;
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
		MessageBox(sMsg, "提示", MB_ICONINFORMATION);
	}
}


void CModifyUserInfoDlg::OnOK()
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