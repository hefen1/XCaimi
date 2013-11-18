// DlgSaveTrig.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSaveTrig.h"
#include ".\dlgsavetrig.h"
#include "DlgStrategy.h"

// CDlgSaveTrig 对话框

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


// CDlgSaveTrig 消息处理程序
BOOL CDlgSaveTrig::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_wndName.SetWindowText(m_sName);
	m_wndName.SetLimitText(32);
	return TRUE;
}
void CDlgSaveTrig::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString name;
	m_wndName.GetWindowText(name);
	if (name.IsEmpty())
	{
		MessageBox("请填写名称！",_T("提示"),MB_ICONINFORMATION);
		m_wndName.SetFocus();
		return;
	}
	if (!IsValid(name))
	{
		MessageBox("名称不能含有非法字符！",_T("提示"),MB_ICONINFORMATION);
		m_wndName.SetFocus();
		return;
	}
	if (CDlgStrategy::GetStrategyTrig(m_pMap,name.GetBuffer()))
	{
		if(MessageBox("已存在该名称的策略，是否覆盖？","提示",MB_ICONQUESTION | MB_YESNO) != IDYES)
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
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CDlgSaveTrig::IsValid(CString str)
{
	if (str.Find("(") != -1 || str.Find(")") != -1)//名称不能包含括号
	{
		return FALSE;
	}
	return TRUE;
}
