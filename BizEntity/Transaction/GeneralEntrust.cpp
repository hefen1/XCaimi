// GeneralEntrust.cpp : 实现文件
//

#include "stdafx.h"
#include <afxvisualmanager.h>
#include <afxvisualmanagerwindows.h>

#include "Transaction.h"
#include "GeneralEntrust.h"
#include "..\..\Common\WinnerApplication.h"
#include "..\..\Common\WinnerApplication.h"


// CGeneralEntrust 对话框

IMPLEMENT_DYNAMIC(CGeneralEntrust, CHSBizDialog)

CGeneralEntrust::CGeneralEntrust(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CGeneralEntrust::IDD, pParent)
{

}

CGeneralEntrust::~CGeneralEntrust()
{
}

void CGeneralEntrust::DoDataExchange(CDataExchange* pDX)
{
	CHSBizDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edStockCode);
	DDX_Control(pDX, IDC_BTN_ENTRUST, m_btnEntrust);
}


BEGIN_MESSAGE_MAP(CGeneralEntrust, CHSBizDialog)
ON_BN_CLICKED(IDC_BTN_ENTRUST, &CGeneralEntrust::OnBnClickedBtnEntrust)
END_MESSAGE_MAP()


// CGeneralEntrust 消息处理程序
void CGeneralEntrust::OnBnClickedBtnEntrust()
{
}

BOOL CGeneralEntrust::OnInit()
{
	RegisterLinkageMsg(1000);
	return TRUE;
}


void CGeneralEntrust::OnHotKey(UINT nId)
{
}

void CGeneralEntrust::OnWindowFocused()
{
	OutputDebugString("[CGeneralEntrust::OnWindowFocused] enter...\n");
	GetDlgItem(IDC_EDIT1)->SetFocus();
}

BOOL CGeneralEntrust::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();

// 	m_btnEntrust.EnableFullTextTooltip();
// 	m_btnEntrust.EnableWindowsTheming();
// 	m_btnEntrust.SetFaceColor(RGB(255,0,0),true);
	return FALSE;
}
