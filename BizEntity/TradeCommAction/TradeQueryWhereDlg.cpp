/*******************************************************
  源程序名称:TradeQueryWhereDlg.cpp
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易
  功能说明:  查询窗口基类
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
#include "stdafx.h"
#include "TradeQueryWhereDlg.h"
#include "HSMessage.h"
#include "TradeLangDefined.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTradeQueryWhereDlg, CTradeDialogBase)

CTradeQueryWhereDlg::CTradeQueryWhereDlg(CWnd* pParent /* = NULL */)
: CTradeDialogBase(CTradeQueryWhereDlg::IDD, pParent)
{
	m_bWithDrawShow = FALSE;
}

CTradeQueryWhereDlg::~CTradeQueryWhereDlg()
{
	UnregisterLinkageMsg(TRADE_MESSAGE_QUERYCOUNT);
}

BOOL CTradeQueryWhereDlg::OnInit()
{
	if (__super::OnInit())
	{
		//TODO:
		if(m_pParam->GetStringParam("whereflag") != "0")
		{
			m_dtBeginDate.SetFormat("yyyy-MM-dd");
			m_dtEndDate.SetFormat("yyyy-MM-dd");
			CTime tTime = CTime::GetCurrentTime();
			m_dtEndDate.SetTime(&tTime);
			tTime -= CTimeSpan(7, 0, 0, 0);
			m_dtBeginDate.SetTime(&tTime);
		}
		else
		{
			m_stBeginDate.ShowWindow(SW_HIDE);
			m_stEndDate.ShowWindow(SW_HIDE);
			m_dtBeginDate.ShowWindow(SW_HIDE);
			m_dtEndDate.ShowWindow(SW_HIDE);
			m_btQuery.ShowWindow(SW_HIDE);
		}
		if (m_pParam->GetStringParam("withdrawflag") == "1")
		{
			m_bWithDrawShow = TRUE;
			m_btWithDraw.ShowWindow(SW_SHOW);
		}
		SetSize();	
		RegisterLinkageMsg(TRADE_MESSAGE_QUERYCOUNT);
		return TRUE;
	}
	else
		return FALSE;
}

void CTradeQueryWhereDlg::OnShow(BOOL bShow)
{
	if (bShow)
	{
		//
	}
}

void CTradeQueryWhereDlg::SetSize()
{
	if (m_stCount && m_stCount.GetSafeHwnd() && m_stCount.IsWindowEnabled())
	{
		if (m_bWithDrawShow)
		{
			AdjustLayoutEx(&m_btWithDraw, 5);
			CRect rcComm;
			m_btWithDraw.GetClientRect(rcComm);
			AdjustLayoutEx(&m_stCount, rcComm.Width() + 5);
		}
		else
			AdjustLayoutEx(&m_stCount, 5);
	}	
}

BOOL CTradeQueryWhereDlg::Validate()
{	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CTradeQueryWhereDlg, CTradeDialogBase)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDQUERY, &CTradeQueryWhereDlg::OnBnClickedQuery)
	ON_BN_CLICKED(IDC_BT_WITHDRAW, &CTradeQueryWhereDlg::OnBnClickedBtWithdraw)
END_MESSAGE_MAP()


void CTradeQueryWhereDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	SetSize();
}

void CTradeQueryWhereDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DT_BEGIN, m_dtBeginDate);
	DDX_Control(pDX, IDC_DT_END, m_dtEndDate);
	DDX_Control(pDX, IDC_BT_QUERY, m_btQuery);
	DDX_Control(pDX, IDC_ST_COUNT, m_stCount);
	DDX_Control(pDX, IDC_BT_WITHDRAW, m_btWithDraw);
	DDX_Control(pDX, IDC_ST_BEGINDATE, m_stBeginDate);
	DDX_Control(pDX, IDC_ST_ENDDATE, m_stEndDate);
}


void CTradeQueryWhereDlg::OnBnClickedQuery()
{
	// TODO: Add your control notification handler code here
	m_btQuery.EnableWindow(FALSE);
	SYSTEMTIME stTime;
	m_dtBeginDate.GetTime(&stTime);
	DWORD nBegin = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
	m_dtEndDate.GetTime(&stTime);
	DWORD nEnd = stTime.wYear * 10000 + stTime.wMonth * 100 + stTime.wDay;
	if (nEnd >= nBegin)
	{
// 		CString sParam = "begin_date=";	
// 		CString sTmp;	
// 		sTmp.Format("%d", nBegin);
// 		sParam += sTmp;
// 		sParam += "|end_date=";	
// 		sTmp.Format("%d", nEnd);
// 		sParam += sTmp;
		CLinkageParam param(TRUE);
		CString sTmp;
		sTmp.Format("%d", nBegin);
		param.AddParam(_T("begin_date"),sTmp);
		sTmp.Format("%d", nEnd);
		param.AddParam(_T("end_date"),sTmp);
		this->SendLinkageMsg(TRADE_MESSAGE_REFRESH, &param);
	}
	else
	{
		MessageBox(m_pTradeLang->LoadStr(GVN_TRADE_QUERYDATEERROR), "提示", MB_ICONINFORMATION);
		m_dtBeginDate.SetFocus();
	}
	m_btQuery.EnableWindow();
}

void CTradeQueryWhereDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	switch (pParam->m_nMsg)
	{
	case TRADE_MESSAGE_QUERYCOUNT:
		{
			if (IsShow())
			{
				int nCount = 0;
				nCount = pParam->GetIntegerField(TRADE_PARAM_COUNT);
				CString sCount;
				sCount.Format("共%d条", nCount);
				m_stCount.SetWindowText(sCount);
			}
			break;
		}
	}
}
void CTradeQueryWhereDlg::OnBnClickedBtWithdraw()
{
	// TODO: Add your control notification handler code here
	CLinkageParam param;
	SendLinkageMsg(TRADE_MESSAGE_WITHDRAW, &param);
}
