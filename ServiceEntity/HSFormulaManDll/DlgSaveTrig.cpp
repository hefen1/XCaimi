// DlgSaveTrig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSaveTrig.h"
#include ".\dlgsavetrig.h"
#include "DlgStrategy.h"

// CDlgSaveTrig �Ի���

IMPLEMENT_DYNAMIC(CDlgSaveTrig, CDialog)
CDlgSaveTrig::CDlgSaveTrig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveTrig::IDD, pParent)
{
	m_pMap = NULL;
}

CDlgSaveTrig::~CDlgSaveTrig()
{
}

void CDlgSaveTrig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITNAME, m_wndName);
}


BEGIN_MESSAGE_MAP(CDlgSaveTrig, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSaveTrig ��Ϣ�������
BOOL CDlgSaveTrig::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndName.SetWindowText(m_sName);
	m_wndName.SetLimitText(32);
	return TRUE;
}
void CDlgSaveTrig::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString name;
	m_wndName.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox("����д���ƣ�",_T("��ʾ"),MB_ICONINFORMATION);
		m_wndName.SetFocus();
		return;
	}
	if (!IsValid(name))
	{
		MessageBox("���Ʋ��ܺ��зǷ��ַ���",_T("��ʾ"),MB_ICONINFORMATION);
		m_wndName.SetFocus();
		return;
	}
	if (CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer()))
	{
		if(MessageBox("�Ѵ��ڸ����ƵĲ��ԣ��Ƿ񸲸ǣ�","��ʾ",MB_ICONQUESTION | MB_YESNO) != IDYES)
		{
			m_wndName.SetFocus();
			m_wndName.SetSel(name.GetLength());
			return;
		}
	}
	m_sName = name;
	OnOK();
}

void CDlgSaveTrig::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgSaveTrig::IsValid(CString str)
{
	if (str.Find("(") != -1 || str.Find(")") != -1)//���Ʋ��ܰ�������
	{
		return FALSE;
	}
	return TRUE;
}
