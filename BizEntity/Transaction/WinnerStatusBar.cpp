// WinnerStatusBar.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "WinnerStatusBar.h"

#include "..\..\Common\LogService.h"
#include "..\..\Common\WinnerApplication.h"


// CWinnerStatusBar 对话框

IMPLEMENT_DYNAMIC(CWinnerStatusBar, CHSBizDialog)

CWinnerStatusBar::CWinnerStatusBar(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CWinnerStatusBar::IDD, pParent)
{
	m_nLayoutCY = 30;
}

CWinnerStatusBar::~CWinnerStatusBar()
{
}

void CWinnerStatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWinnerStatusBar, CHSBizDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CWinnerStatusBar::OnBnClickedButton1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON2, &CWinnerStatusBar::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWinnerStatusBar 消息处理程序

void CWinnerStatusBar::OnPaint()
{
	CPaintDC dc(this);

	CBrush brush;
	brush.CreateSolidBrush(RGB(255,0,255));
	HGDIOBJ hOld = dc.SelectObject(&brush);

	CRect rc;
	GetClientRect(&rc);
	dc.Rectangle(&rc);

	dc.SelectObject(hOld);
}



void CWinnerStatusBar::OnBnClickedButton1()
{
// 	HMODULE h1 = CWinnerApplication::GetModuleHandle(_T("Transaction.dll"));
// 	HMODULE h2 = CWinnerApplication::GetModuleHandle(_T("WinnerStatus.dll"));
// 
// 	CString str;
// 	str.Format("Transaction's handle=%x,WinnerStatus's handle=%x",h1,h2);
// 	this->MessageBox(str);

	CWinnerApplication::SimpleLog(FATAL_LOG_LEVEL,_T("winner"),"simple log");
	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,_T("winner"),"CWinnerStatusBar::OnBnClickedButton1: %s--%d","format log test",2);
	HSSize s;
	memset(&s,0,sizeof(s));
	s.cx = 100;
	s.cy = 16;
	_tcscpy_s(s.name,_countof(s.name),TEXT("hs_size"));
	CWinnerApplication::HexdumpLog(FATAL_LOG_LEVEL,_T("winner"),(char*)&s,sizeof(s),"hs_size");

	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,_T("winner"),"size of double=%d",sizeof(double));
	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,_T("winner"),"size of long=%d",sizeof(long));

	long double a = 1;
	a = a + 1.5;
	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,_T("winner"),"size of long double=%d",sizeof(long double));

	double long b = 2 * 100;
	b = b + 1.5;
	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,_T("winner"),"size of double long=%d",sizeof(double long));
	return;

	m_scrollNews.SetParent(NULL);
	m_scrollNews.SetOwner(NULL);	
}

BOOL CWinnerStatusBar::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	CWinnerApplication::FormatLog(FATAL_LOG_LEVEL,"CWinnerStatusBar::OnInitDialog: simple log",_T("winner"));

	return TRUE;
}

void CWinnerStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	if(m_scrollNews.GetSafeHwnd())
		m_scrollNews.MoveWindow(cx/2,0,cx/2,cy);
}

BOOL CWinnerStatusBar::OnInit()
{
	ILayoutWindow *pLayoutWnd = GetLayoutWindow();
	if(pLayoutWnd)
	{
		//pLayoutWnd->ChangeSize(0,m_nLayoutCY);
	}
	if(m_pParam)
	{
		CString strBtnName = m_pParam->GetStringParam(_T("button1_name"));
		if(!strBtnName.IsEmpty())
			GetDlgItem(IDC_BUTTON1)->SetWindowText(strBtnName);
	}
	return TRUE;
}

void CWinnerStatusBar::OnBnClickedButton2()
{
// 	ILayoutWindow *pLayoutWnd = GetLayoutWindow();
// 	if(pLayoutWnd)
// 	{
// 		if(m_nLayoutCY == 30)
// 			m_nLayoutCY = 50;
// 		else
// 			m_nLayoutCY = 30;
// 		pLayoutWnd->ChangeSize(0,m_nLayoutCY);
// 	}
}
