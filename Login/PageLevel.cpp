// PageLevel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Login.h"
#include "PageLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPageLevel �Ի���

IMPLEMENT_DYNAMIC(CPageLevel, CPropertyPage)

CPageLevel::CPageLevel()
	: CPropertyPage(CPageLevel::IDD)
{

}

CPageLevel::~CPageLevel()
{
}

void CPageLevel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageLevel, CPropertyPage)
END_MESSAGE_MAP()


// CPageLevel ��Ϣ�������
BOOL CPageLevel::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	Init();
	return TRUE;
}

void CPageLevel::Init()
{

}
BOOL CPageLevel::OnApply()
{
	CPropertyPage::OnApply();
	//����level2�ʺ�
	return TRUE;
}