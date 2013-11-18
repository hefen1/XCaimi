// SaveLayoutDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "SaveLayoutDialog.h"
#include "..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSaveLayoutDialog 对话框

IMPLEMENT_DYNAMIC(CSaveLayoutDialog, CDialog)

CSaveLayoutDialog::CSaveLayoutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveLayoutDialog::IDD, pParent)
{

}

CSaveLayoutDialog::~CSaveLayoutDialog()
{
}

void CSaveLayoutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveLayoutDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSaveLayoutDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveLayoutDialog 消息处理程序

void CSaveLayoutDialog::OnBnClickedOk()
{
	CWnd* pWnd = GetDlgItem(IDC_LAYOUTNAM);
	if(pWnd)
	{
		pWnd->GetWindowText(m_strLayoutName);
	}
	if(m_strLayoutName.IsEmpty())
	{
		MessageBox(_T("布局名称不能为空!"),_T("错误"),MB_ICONWARNING | MB_OK);
		return;
	}
	CFileFind f;
	CString strTemp;
	strTemp.Format(_T("%s.xml"),CGeneralHelper::GetAppPath() + _T("layout\\self\\") 
		+ m_strLayoutName); 
	if( f.FindFile(strTemp) )
	{
		MessageBox(_T("已存在同名布局!"),_T("错误"),MB_ICONWARNING | MB_OK);
		return;
	}
	OnOK();
}

BOOL CSaveLayoutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd = GetDlgItem(IDC_LAYOUTNAM);
	if(pWnd)
	{
		pWnd->SetFocus();
	}

	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
