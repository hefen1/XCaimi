// PageProxyMan.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "PageProxyMan.h"
#include "TreePropSheet.h"
#include "DoLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPageProxyMan 对话框

IMPLEMENT_DYNAMIC(CPageProxyMan, CPropertyPage)

CPageProxyMan::CPageProxyMan()
	: CPropertyPage(CPageProxyMan::IDD)
{

}

CPageProxyMan::~CPageProxyMan()
{

}

void CPageProxyMan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, m_cProxy);
	DDX_Control(pDX, IDC_EDIT_PORT, m_cPort);
	DDX_Control(pDX, IDC_EDIT_USER, m_cUser);
	DDX_Control(pDX, IDC_EDIT_PWD, m_cPWD);
	DDX_Control(pDX, IDC_CHECK_PROXY, m_cCheckProxy);
	DDX_Control(pDX, IDC_CHECK_CHENNAL, m_cCheckChannel);
	DDX_Control(pDX, IDC_CHECK_AUTO, m_cCheckAuto);
}


BEGIN_MESSAGE_MAP(CPageProxyMan, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PROXY, &CPageProxyMan::OnBnClickedCheckProxy)
	ON_BN_CLICKED(IDC_CHECK_CHENNAL, &CPageProxyMan::OnBnClickedCheckChennal)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CPageProxyMan::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_RADIO_SOCKET4, &CPageProxyMan::OnBnClickedRadioSocket4)
	ON_BN_CLICKED(IDC_RADIO_HTTP, &CPageProxyMan::OnBnClickedRadioHttp)
	ON_BN_CLICKED(IDC_RADIO_SOCKET5, &CPageProxyMan::OnBnClickedRadioSocket5)
END_MESSAGE_MAP()


// CPageProxyMan 消息处理程序
BOOL CPageProxyMan::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_cPWD.SetLimitText(64);
	m_cUser.SetLimitText(64);
	m_cPort.SetLimitText(5);
	Init();
	return TRUE;
}

void CPageProxyMan::Init()
{
	BOOL bAuto = g_pDoLogin->m_commData.IsAuto();
	m_cCheckAuto.SetCheck(bAuto);
	ProxyInfo *proxy = g_pDoLogin->m_commData.GetProxy();
	if (proxy)
	{
		if(proxy->m_nUseChannel)
		{
			m_cCheckChannel.SetCheck(TRUE);
			Enable(FALSE);
		}
		if(proxy->m_nOptions != NoProxy)
		{
			m_cCheckProxy.SetCheck(TRUE);
			m_cProxy.SetWindowText(proxy->m_strProxyIP);
			CString str;
			str.Format("%d",proxy->m_nPort);
			m_cPort.SetWindowText(str);
			m_cUser.SetWindowText(proxy->m_strUser);
			m_cPWD.SetWindowText(proxy->m_strPwd);
			switch(proxy->m_nOptions)
			{
			case Socks4:
				((CButton*)GetDlgItem(IDC_RADIO_SOCKET4))->SetCheck(TRUE);
				break;
			case Http:
				((CButton*)GetDlgItem(IDC_RADIO_HTTP))->SetCheck(TRUE);
				break;
			case  Socks5:
				((CButton*)GetDlgItem(IDC_RADIO_SOCKET5))->SetCheck(TRUE);
				break;
			default:
				break;
			}
			Enable(TRUE);
		}
		else
			Enable(FALSE);
	}
	else
	{
		Enable(FALSE);
	}
}
void CPageProxyMan::OnBnClickedCheckProxy()
{
	// TODO: 在此添加控件通知处理程序代码
	BOOL check = m_cCheckProxy.GetCheck();
	Enable(check);
}
void CPageProxyMan::Enable(BOOL benable)
{
	m_cProxy.EnableWindow(benable);
	m_cPort.EnableWindow(benable);
	int check = GetCheckedRadioButton(IDC_RADIO_HTTP,IDC_RADIO_SOCKET5);
	switch(check)
	{
	case IDC_RADIO_HTTP:
		m_cUser.EnableWindow(benable);
		m_cPWD.EnableWindow(benable);
		break;
	case IDC_RADIO_SOCKET4:
		m_cUser.EnableWindow(FALSE);
		m_cPWD.EnableWindow(FALSE);
		break;
	case IDC_RADIO_SOCKET5:
		m_cUser.EnableWindow(benable);
		m_cPWD.EnableWindow(benable);
		break;
	default:
		m_cUser.EnableWindow(FALSE);
		m_cPWD.EnableWindow(FALSE);
	}
	
	CString strIP;
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(strIP); 
	if(strIP == "0.0.0.0")
		((CIPAddressCtrl*)GetDlgItem(IDC_EDIT_IP))->SetAddress(0,0,0,0);

	GetDlgItem(IDC_RADIO_HTTP)->EnableWindow(benable);
	GetDlgItem(IDC_RADIO_SOCKET4)->EnableWindow(benable);
	GetDlgItem(IDC_RADIO_SOCKET5)->EnableWindow(benable);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(benable);
}
void CPageProxyMan::OnBnClickedCheckChennal()
{
	// TODO: 在此添加控件通知处理程序代码
// 	BOOL check = m_cCheckChannel.GetCheck();
// 	BOOL checkp = m_cCheckProxy.GetCheck();
// 	//m_cCheckProxy.EnableWindow(!check);
// 	Enable((!check)&&checkp);
}

void CPageProxyMan::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_cCheckProxy.GetCheck())//代理
	{
		CString strText,strTitle;
		strTitle.LoadString(IDS_STRING_ProxyTest);
		ProxyInfo proxy;
		if(GetProxyInfo(&proxy))
		{
			if(g_pDoLogin->m_pManager->TestProxy(&proxy) == 0)
			{
				strText.LoadString(IDS_STRING_ProxySuccess);
				MessageBox(strText,strTitle);
			}
			else
			{
				strText.LoadString(IDS_STRING_ProxyFail);
				MessageBox(strText,strTitle);
			}
		}
	}
}

BOOL CPageProxyMan::GetProxyInfo(ProxyInfo *proxy)
{
	if (!proxy)
	{
		return FALSE;
	}
	CString strText,strTitle;
	strTitle.LoadString(IDS_STRING_Error);
	
	CString strProxy,strPort,strUser,strPWD;
	m_cProxy.GetWindowText(strProxy);
	m_cPort.GetWindowText(strPort);
	if (strProxy.IsEmpty() || strProxy == "0.0.0.0")
	{
		strText.LoadString(IDS_STRING_ProxyNameNone);
		MessageBox(strText,strTitle);
		m_cProxy.SetFocus();
		return FALSE;
	}
	if (strPort.IsEmpty())
	{
		strText.LoadString(IDS_STRING_ProxyPortNone);
		MessageBox(strText,strTitle);
		m_cPort.SetFocus();
		return FALSE;
	}
	int check = GetCheckedRadioButton(IDC_RADIO_HTTP,IDC_RADIO_SOCKET5);
	switch(check)
	{
	case IDC_RADIO_HTTP:
		proxy->m_nOptions = Http;
		break;
	case IDC_RADIO_SOCKET4:
		proxy->m_nOptions = Socks4;
		break;
	case IDC_RADIO_SOCKET5:
		proxy->m_nOptions = Socks5;
		break;
	default:
		strText.LoadString(IDS_STRING_ProxyKindNone);
		MessageBox(strText,strTitle);
		GetDlgItem(IDC_RADIO_HTTP)->SetFocus();
		return FALSE;
	}

	m_cUser.GetWindowText(strUser);
	m_cPWD.GetWindowText(strPWD);

	memcpy(proxy->m_strProxyIP,strProxy.GetBuffer(),128);
	memcpy(proxy->m_strUser,strUser.GetBuffer(),64);
	memcpy(proxy->m_strPwd,strPWD.GetBuffer(),64);
	proxy->m_nPort = atoi(strPort);
	return TRUE;
}

BOOL CPageProxyMan::OnApply()
{
	CPropertyPage::OnApply();
	g_pDoLogin->m_commData.SetAuto(m_cCheckAuto.GetCheck());

	ProxyInfo proxy;
	memset(&proxy,0,sizeof(ProxyInfo));
	proxy.m_nOptions = NoProxy;
	proxy.m_nUseChannel = 0;
	if (m_cCheckChannel.GetCheck())
	{
		proxy.m_nUseChannel = 1;
	}
	if (m_cCheckProxy.GetCheck())
	{
		if(GetProxyInfo(&proxy))
		{	
			g_pDoLogin->m_commData.SetProxy(&proxy);
			return TRUE;
		}
		else
		{
			((TreePropSheet::CTreePropSheet*)(this->GetParent()))->SetActivePage(this);
			return FALSE;
		}
	}
	g_pDoLogin->m_commData.SetProxy(&proxy);
	return TRUE;
}

void CPageProxyMan::OnBnClickedRadioSocket4()
{
	// TODO: 在此添加控件通知处理程序代码
	int check = GetCheckedRadioButton(IDC_RADIO_HTTP,IDC_RADIO_SOCKET5);
	switch(check)
	{
	case IDC_RADIO_SOCKET4:
		m_cUser.EnableWindow(FALSE);
		m_cPWD.EnableWindow(FALSE);
		break;
	default:
		m_cUser.EnableWindow(TRUE);
		m_cPWD.EnableWindow(TRUE);
	}
}

void CPageProxyMan::OnBnClickedRadioHttp()
{
	// TODO: 在此添加控件通知处理程序代码
	int check = GetCheckedRadioButton(IDC_RADIO_HTTP,IDC_RADIO_SOCKET5);
	switch(check)
	{
	case IDC_RADIO_HTTP:
		m_cUser.EnableWindow(check);
		m_cPWD.EnableWindow(check);
		break;
	default:
		m_cUser.EnableWindow(TRUE);
		m_cPWD.EnableWindow(TRUE);
	}
}

void CPageProxyMan::OnBnClickedRadioSocket5()
{
	// TODO: 在此添加控件通知处理程序代码
	int check = GetCheckedRadioButton(IDC_RADIO_HTTP,IDC_RADIO_SOCKET5);
	switch(check)
	{
	case IDC_RADIO_SOCKET4:
		m_cUser.EnableWindow(FALSE);
		m_cPWD.EnableWindow(FALSE);
		break;
	default:
		m_cUser.EnableWindow(TRUE);
		m_cPWD.EnableWindow(TRUE);
	}
}

