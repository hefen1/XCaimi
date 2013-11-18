// ScrollNews.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "ScrollNews.h"


// CScrollNews 对话框

IMPLEMENT_DYNAMIC(CScrollNews, CDialog)

CScrollNews::CScrollNews(CWnd* pParent /*=NULL*/)
	: CDialog(CScrollNews::IDD, pParent)
{	

}

CScrollNews::~CScrollNews()
{
}

void CScrollNews::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScrollNews, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CScrollNews::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScrollNews 消息处理程序

void CScrollNews::OnBnClickedButton1()
{
	this->SetParent(this->m_statusBar);
}
