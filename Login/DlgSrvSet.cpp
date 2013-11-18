// DlgSrvSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Login.h"
#include "DlgSrvSet.h"
#include "DoLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgSrvSet �Ի���

IMPLEMENT_DYNAMIC(CDlgSrvSet, CDialog)

CDlgSrvSet::CDlgSrvSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSrvSet::IDD, pParent)
{

}

CDlgSrvSet::~CDlgSrvSet()
{
}

void CDlgSrvSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_cSrvName);
	DDX_Control(pDX, IDC_EDIT_PORT, m_cSrvPort);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_cSrvAddr);
}


BEGIN_MESSAGE_MAP(CDlgSrvSet, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSrvSet::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSrvSet::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSrvSet ��Ϣ�������
BOOL CDlgSrvSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_cSrvName.SetLimitText(128);
	m_cSrvPort.SetLimitText(5);
	if (m_pConn && m_pConn->m_nPort > 0)
	{
		m_cSrvName.SetWindowText(CString(m_pConn->m_strServerID));
		m_cSrvAddr.SetWindowText(CString(m_pConn->m_strServerIP));
		CString str;
		str.Format(_T("%d"),m_pConn->m_nPort);
		m_cSrvPort.SetWindowText(str);
		SetWindowText(_T("�޸�վ��"));
	}
	else
		SetWindowText(_T("����վ��"));
	return TRUE;
}
void CDlgSrvSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strText,strTitle;
	strTitle.LoadString(IDS_STRING_Error);

	CString strName,strIP,strPort;
	m_cSrvName.GetWindowText(strName);
	m_cSrvAddr.GetWindowText(strIP);
	m_cSrvPort.GetWindowText(strPort);
	if (strName.IsEmpty())
	{
		strText.LoadString(IDS_STRING_SrvNameNone);
		MessageBox(strText,strTitle);
		m_cSrvName.SetFocus();
		return;
	}
	if (strIP.IsEmpty())
	{
		strText.LoadString(IDS_STRING_SrvIPNone);
		MessageBox(strText,strTitle);
		m_cSrvAddr.SetFocus();
		return;
	}
	if (strPort.IsEmpty())
	{
		strText.LoadString(IDS_STRING_SrvPortNone);
		MessageBox(strText,strTitle);
		m_cSrvPort.SetFocus();
		return;
	}
	if (CheckSrv()) 
	{
		strText.LoadString(IDS_STRING_SrvNameRepeart);
		MessageBox(strText,strTitle);
		m_cSrvName.SetFocus();
		m_cSrvName.SetSel(0,m_cSrvName.GetWindowTextLength());
		return;
	}
	strncpy(m_pConn->m_strServerID,strName.GetBuffer(),128);
	strncpy(m_pConn->m_strServerIP,strIP.GetBuffer(),128);
	m_pConn->m_nPort = _ttoi(strPort.GetBuffer());
	OnOK();
}

void CDlgSrvSet::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgSrvSet::CheckSrv()
{
 	CArray<CommInfo*,CommInfo*> *pSites = g_pDoLogin->m_commData.GetUsrServerInfo();
 	if (pSites)
	{
 		CString strName,strIP,strPort;
 		m_cSrvName.GetWindowText(strName);
		for(int i=0; i<pSites->GetCount(); i++)
 		{
			CommInfo *pComm = pSites->GetAt(i);
 			if (pComm->m_cType == m_type)
 			{
				if (!strcmp(pComm->m_strServerID,strName.GetBuffer()) && m_pConn != pComm)
				{
					return TRUE;
				}
 			}
		}
	}
	return FALSE;
}
