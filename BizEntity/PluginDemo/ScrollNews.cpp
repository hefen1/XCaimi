// ScrollNews.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Transaction.h"
#include "ScrollNews.h"


// CScrollNews �Ի���

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


// CScrollNews ��Ϣ�������

void CScrollNews::OnBnClickedButton1()
{
	this->SetParent(this->m_statusBar);
}
