// QuoteReport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Transaction.h"
#include "QuoteReport.h"


// CQuoteReport �Ի���

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


// CQuoteReport ��Ϣ�������

BOOL CQuoteReport::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

	this->SetBackgroundColor(RGB(0,0,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
