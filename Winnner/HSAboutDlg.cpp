// HSAboutDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Winner.h"
#include "HSAboutDlg.h"


// CHSAboutDlg �Ի���

IMPLEMENT_DYNAMIC(CHSAboutDlg, CDialog)

CHSAboutDlg::CHSAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHSAboutDlg::IDD, pParent)
{

}

CHSAboutDlg::~CHSAboutDlg()
{
}

void CHSAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}	


BEGIN_MESSAGE_MAP(CHSAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CHSAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rcClient;
	GetClientRect(rcClient);
	if ( m_list.GetSafeHwnd() )
	{
		m_list.InsertColumn(0, "ģ������",LVCFMT_CENTER,rcClient.Width()/2);
		m_list.InsertColumn(1, "�汾��",LVCFMT_CENTER,rcClient.Width()/2);
		m_list.InsertItem(0, "");
		m_list.SetItemText(0,0, "��Ӧ�ó���");
		m_list.SetItemText(0,1, "1.0.0.9");

	}
	return TRUE;
}

// CHSAboutDlg ��Ϣ�������
