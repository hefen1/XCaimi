// QuoteReport.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "QuoteReport.h"


// CQuoteReport 对话框

IMPLEMENT_DYNAMIC(CQuoteReport, CHSBizDialog)

CQuoteReport::CQuoteReport(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CQuoteReport::IDD, pParent)
{

}

CQuoteReport::~CQuoteReport()
{
}

void CQuoteReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CQuoteReport, CHSBizDialog)
END_MESSAGE_MAP()


// CQuoteReport 消息处理程序

BOOL CQuoteReport::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	this->SetBackgroundColor(RGB(0,0,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
