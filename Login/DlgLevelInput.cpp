// DlgLevelInput.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "DlgLevelInput.h"
#include "DataSourceDefine.h"
#include "hsds_comudata.h"
#include "DoLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgLevelInput 对话框


//extern void WINAPI FSetProgressMsg(CDataSourceInitDataStauts *msg, CWnd* pWnd);

IMPLEMENT_DYNAMIC(CDlgLevelInput, CDialogEx)

CDlgLevelInput::CDlgLevelInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLevelInput::IDD, pParent)
{

}

CDlgLevelInput::~CDlgLevelInput()
{
}

void CDlgLevelInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_SHPWD,m_sEdit);
}


BEGIN_MESSAGE_MAP(CDlgLevelInput, CDialogEx)
END_MESSAGE_MAP()


// CDlgLevelInput 消息处理程序
BOOL CDlgLevelInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, "face/Login_bkJY.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	this->SetBackgroundImage(bitmap);
	Init();
	return TRUE;
}
void CDlgLevelInput::Init()
{
	m_sEdit.SetLimitText(8); //需要从配置文件中获得

	//////////////////////////////////////////////////////////////////////////
	// for test
	GetDlgItem(IDC_EDIT_SHACC)->SetWindowText("1001");
	m_sEdit.SetRealText("1111");
	m_sEdit.OnShow();
}

BOOL CDlgLevelInput::LoginLevel(const CString &strAccName, const CString &strAccPwd)
{
	CString strUserAcc(""), strPwd("");
	if (strAccName.IsEmpty() || strAccPwd.IsEmpty())
	{
		EnableInput(FALSE);
		CString strText;
		CString strTitle;
		strTitle.LoadString(IDS_STRING_Error);
		//////////////////////////////////////////////////////////////////////////
		// 获取账号、密码
		
		GetDlgItem(IDC_EDIT_SHACC)->GetWindowText(strUserAcc);
		if (strUserAcc.IsEmpty())
		{
			strText.LoadString(IDS_STRING_AccountError);
			MessageBox(strText,strTitle);
			EnableInput(TRUE);
			GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
			return FALSE;
		}
		strPwd = m_sEdit.GetRealText();
		if (strPwd.IsEmpty())
		{
			strText.LoadString(IDS_STRING_PasswardError);
			MessageBox(strText,strTitle);
			EnableInput(TRUE);
			m_sEdit.SetFocus();
			return FALSE;
		}
	}
	else
	{
		strUserAcc = strAccName;
		strPwd = strAccPwd;
	}
	long handle = g_pDoLogin->ConnectServer(CEV_Connect_LEVEL2);
	if (handle)
	{// level2服务连接成功 

		IAccountMng *pTradeAccountMng = g_pDoLogin->m_pTradeCore->GetAccountMng();
		if(pTradeAccountMng && pTradeAccountMng->GetCount(atLevel2) == 0)
		{
			//IAccount* pQHAccount = pTradeAccountMng->Add(atLevel2);
			IAccount* pQHAccount = pTradeAccountMng->NewAccount(atLevel2);
			if(pQHAccount)
			{
				pQHAccount->SetLoginAccount(strUserAcc);
				pQHAccount->SetCellNo("");
				pQHAccount->SetLoginAccountType("");
				pQHAccount->SetPassword(strPwd);
				pQHAccount->SetServerID(0);
				pQHAccount->SetBranchNo("0");
				pQHAccount->SetConnectHandle(handle);
				CString sMsg = "";
				if( pQHAccount->Login(sMsg) )
				{
					pTradeAccountMng->Add(pQHAccount);
					pTradeAccountMng->SetCurrentAccount(pQHAccount);
					return TRUE;
				}
				else
				{
					pQHAccount->Release();
					//pTradeAccountMng->DeleteAccount(pQHAccount);
// 					CDataSourceInitDataStauts msg;
// 					strncpy(msg.m_szMsg, sMsg, min(sMsg.GetLength()+1, 256));
// 					FSetProgressMsg(&msg);
					MessageBox(sMsg, "Error", MB_OK | MB_ICONERROR);
				}
			}
		}
	}
	EnableInput(TRUE);
	return TRUE;
}

BOOL CDlgLevelInput::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) 
	{ 
		if(pMsg->wParam == VK_TAB) 
		{ 
			UINT nID = GetFocus()->GetDlgCtrlID(); 
			switch(nID)
			{
			case IDC_EDIT_SHPWD:
				this->GetParent()->GetDlgItem(IDC_BUTTON_LOGIN)->SetFocus();
				break;
			default:
				NextDlgCtrl(); 
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			UINT nID = GetFocus()->GetDlgCtrlID();
			switch(nID)
			{
			case IDC_EDIT_SHPWD:
				{
					CWnd* pWnd = GetParent()->GetDlgItem(IDC_BUTTON_LOGIN);
					if (pWnd)
					{
						pWnd->SetFocus();
						::PostMessage(GetParent()->m_hWnd, WM_KEYDOWN, VK_RETURN, 1);
					} 
					break;
				}
			default:
				NextDlgCtrl();
			}
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			::PostMessage(GetParent()->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgLevelInput::EnableInput( BOOL bEnable )
{
	GetDlgItem(IDC_EDIT_SHACC)->EnableWindow(bEnable);
	m_sEdit.EnableWindow(bEnable);
	if(bEnable)
		GetDlgItem(IDC_EDIT_SHACC)->SetFocus();
}
