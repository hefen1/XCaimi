// InfomationForm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InfomationForm.h"


// CInfomationForm �Ի���

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


// CInfomationForm ��Ϣ�������
