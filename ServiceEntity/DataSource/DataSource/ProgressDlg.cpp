// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataSource.h"
#include "ProgressDlg.h"


// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{
	m_pBrush = new CBrush(GetSysColor(COLOR_BTNFACE));//::GetSysColor(COLOR_BACKGROUND/*COLOR_INFOBK*/));
	HINSTANCE hInst = AfxGetInstanceHandle( );

	WNDCLASS wndcls;
	wndcls.style         = CS_SAVEBITS | CS_DBLCLKS;
	wndcls.lpfnWndProc   = ::DefWindowProc;
	wndcls.cbClsExtra    = 0;
	wndcls.cbWndExtra    = 0;
	wndcls.hInstance     = hInst;
	wndcls.hIcon         = NULL;
	wndcls.hCursor       = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndcls.hbrBackground = *m_pBrush;
	wndcls.lpszMenuName  = NULL;
	wndcls.lpszClassName = _T("CProgressDlg");

	if (!AfxRegisterClass(&wndcls))
		AfxThrowResourceException();
}

CProgressDlg::~CProgressDlg()
{
	if ( m_pBrush )
		delete m_pBrush;
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	
	return TRUE;
}

void CProgressDlg::SetText( CString strText )
{
	if ( m_wndText.GetSafeHwnd() )
		m_wndText.SetWindowText(strText);
}

void CProgressDlg::SetPos( int nPos )
{
	if ( m_wndProgressCtrl.GetSafeHwnd() )
		m_wndProgressCtrl.SetPos(nPos);
	SetFocus();
}

BOOL CProgressDlg::Create( CWnd* pParentWnd )
{
	return CDialog::CreateEx( WS_EX_TOPMOST ,_T("CProgressDlg"),"请等待...",DS_CENTER|WS_POPUP | WS_BORDER |WS_CAPTION | WS_VISIBLE | WS_THICKFRAME,
		0,0,0,0,NULL, 	NULL);
}
// CProgressDlg 消息处理程序

int CProgressDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rcClient;
	GetClientRect(rcClient);
	SetWindowPos(NULL, 300,200,400,100, SWP_NOZORDER);
	m_wndText.Create(NULL, WS_CHILD | WS_VISIBLE ,CRect(0,0,0,0), this,0);
	m_wndProgressCtrl.Create( WS_CHILD | WS_VISIBLE|PBS_SMOOTH  ,CRect(0,0,0,0), this,1);
	if ( m_wndText.GetSafeHwnd() )
		m_wndText.SetWindowPos(NULL, rcClient.left,rcClient.top,200,30,SWP_NOZORDER);	
	if ( m_wndProgressCtrl.GetSafeHwnd() )
	{
		m_wndProgressCtrl.SetWindowPos(NULL, rcClient.left,rcClient.top+32,380,25,SWP_NOZORDER);
		m_wndProgressCtrl.SetRange(0,100);
		m_wndProgressCtrl.SetPos(0);
	}
	return 0;
}
