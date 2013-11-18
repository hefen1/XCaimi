// DlgLevelFrame.cpp : 实现文件
//
#include "stdafx.h"
#include "Login.h"
#include "DlgLevelFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CDlgLevelFrame 对话框

IMPLEMENT_DYNAMIC(CDlgLevelFrame, CDialogEx)

CDlgLevelFrame::CDlgLevelFrame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgLevelFrame::IDD, pParent)
{
	m_pLevelInputDlg = NULL;
	m_bLogin = FALSE;
}

CDlgLevelFrame::~CDlgLevelFrame()
{
	if(m_pLevelInputDlg)
	{
		if (::IsWindow(m_pLevelInputDlg->GetSafeHwnd()))
		{
			m_pLevelInputDlg->DestroyWindow();
		}
		delete m_pLevelInputDlg;
		m_pLevelInputDlg = NULL;
	}
}

void CDlgLevelFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLevelFrame, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CDlgLevelFrame::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgLevelFrame::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgLevelFrame 消息处理程序

BOOL CDlgLevelFrame::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, "face/Login_bkJY.bmp", IMAGE_BITMAP, 0,0, LR_LOADFROMFILE);
	this->SetBackgroundImage(bitmap);
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgLevelFrame::Init()
{
	if (!m_pLevelInputDlg)
	{
		m_pLevelInputDlg = new CDlgLevelInput;
	}
	if(!::IsWindow(m_pLevelInputDlg->m_hWnd))
	{
		m_pLevelInputDlg->Create(IDD_DIALOG_LEVELINPUT,this);
		CRect rc;
		GetDlgItem(IDC_STC_ZONE)->GetWindowRect(rc);
		ScreenToClient(rc);	
		m_pLevelInputDlg->MoveWindow(&rc);
	}
	m_pLevelInputDlg->ShowWindow(SW_SHOW);
}

void CDlgLevelFrame::OnBnClickedBtnLogin()
{
	m_bLogin = m_pLevelInputDlg->LoginLevel();
	OnOK();
}

void CDlgLevelFrame::OnBnClickedBtnCancel()
{
	m_bLogin = FALSE;
	OnCancel();
}

