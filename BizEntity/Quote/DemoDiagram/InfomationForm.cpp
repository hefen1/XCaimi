// InfomationForm.cpp : 实现文件
//

#include "stdafx.h"
#include "InfomationForm.h"


// CInfomationForm 对话框

IMPLEMENT_DYNAMIC(CInfomationForm, CDialog)

CInfomationForm::CInfomationForm(CWnd* pParent /*=NULL*/)
	: CDialog(CInfomationForm::IDD, pParent)
{

}

CInfomationForm::~CInfomationForm()
{
}

void CInfomationForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfomationForm, CDialog)
END_MESSAGE_MAP()


// CInfomationForm 消息处理程序
