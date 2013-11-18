// PropSheet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WinnerStatus.h"
#include "PropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// CPropSheet

IMPLEMENT_DYNAMIC(CPropSheet, CPropertySheet)

CPropSheet::CPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

	AddPage(&m_prop);
}

CPropSheet::CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_prop);
}

CPropSheet::~CPropSheet()
{
	
}


BEGIN_MESSAGE_MAP(CPropSheet, CPropertySheet)
END_MESSAGE_MAP()


// CPropSheet ��Ϣ�������

BOOL CPropSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  �ڴ��������ר�ô���
	GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);
	return bResult;
}
