// PageLevel.cpp : 实现文件
//

#include "stdafx.h"
#include "Login.h"
#include "PageLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPageLevel 对话框

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


// CPageLevel 消息处理程序
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
	//保存level2帐号
	return TRUE;
}