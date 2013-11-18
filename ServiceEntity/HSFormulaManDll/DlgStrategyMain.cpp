// DlgStrategyMain.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgStrategyMain.h"
#include ".\dlgstrategymain.h"


// CDlgStrategyMain 对话框
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HWND g_hParentWnd;
IMPLEMENT_DYNAMIC(CDlgStrategyMain, CDialog)
CDlgStrategyMain::CDlgStrategyMain(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
: CDialog(CDlgStrategyMain::IDD, pParent)
{
	m_pData = pData;
	m_nType = 0;
	m_hParent = g_hParentWnd;
}

CDlgStrategyMain::~CDlgStrategyMain()
{
}

void CDlgStrategyMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_wndTab);
}


BEGIN_MESSAGE_MAP(CDlgStrategyMain, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgStrategyMain 消息处理程序
BOOL CDlgStrategyMain::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bHQOK = FALSE;
	m_bJYOK = FALSE;
	m_wndTab.InsertItem(0,"行情相关");
//	m_wndTab.InsertItem(1,"交易相关");
	//CDlgStrategyHQ m_nStrHqTab(this,m_pData);
	m_nStrHqTab=new CDlgStrategyHQ(this,m_pData);
	m_nStrHqTab->m_pStrategy = m_pStrategy;
	if( m_pData->m_hParent != NULL )
		m_nStrHqTab->m_hParent = m_pData->m_hParent;
	m_nStrHqTab->Create(IDD_DIALOGStrategyHQ,this);
 	CRect rc;
 	m_wndTab.GetClientRect(&rc);
	rc.top += 35;
 	rc.left += 15;
 	m_nStrHqTab->MoveWindow(&rc);
	m_nStrHqTab->ShowWindow(TRUE);
	m_wndTab.SetCurSel(0);
	return TRUE;
}



void CDlgStrategyMain::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nStrHqTab->OnOK();
	if (m_bHQOK)
	{
		CDialog::OnOK();
	}
}

void CDlgStrategyMain::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int nCurSel = m_wndTab.GetCurSel();
	switch(nCurSel)
	{
	case 0:
		m_nStrHqTab->ShowWindow(TRUE);
		
		break;
	case 1:
		m_nStrHqTab->ShowWindow(FALSE);
		
		break;
	}
}

void CDlgStrategyMain::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nStrHqTab->OnCancel();
	CString code = m_pStrategy->m_cCode;
	if (code.IsEmpty())
	{
		m_pStrategy->m_nID = 0;
	}
	CDialog::OnCancel();
}
