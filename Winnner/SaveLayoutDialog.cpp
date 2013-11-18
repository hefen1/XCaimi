// SaveLayoutDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Winner.h"
#include "SaveLayoutDialog.h"
#include "..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSaveLayoutDialog �Ի���

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


// CSaveLayoutDialog ��Ϣ�������

void CSaveLayoutDialog::OnBnClickedOk()
{
	CWnd* pWnd = GetDlgItem(IDC_LAYOUTNAM);
	if(pWnd)
	{
		pWnd->GetWindowText(m_strLayoutName);
	}
	if(m_strLayoutName.IsEmpty())
	{
		MessageBox(_T("�������Ʋ���Ϊ��!"),_T("����"),MB_ICONWARNING | MB_OK);
		return;
	}
	CFileFind f;
	CString strTemp;
	strTemp.Format(_T("%s.xml"),CGeneralHelper::GetAppPath() + _T("layout\\self\\") 
		+ m_strLayoutName); 
	if( f.FindFile(strTemp) )
	{
		MessageBox(_T("�Ѵ���ͬ������!"),_T("����"),MB_ICONWARNING | MB_OK);
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
