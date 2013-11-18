// SelUnionDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "HSCompMan.h"
#include "SelUnionDlg.h"
#include "DlgUnionList.h"


// CSelUnionDlg dialog

IMPLEMENT_DYNAMIC(CSelUnionDlg, CDialog)
CSelUnionDlg::CSelUnionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelUnionDlg::IDD, pParent)
{
}

CSelUnionDlg::~CSelUnionDlg()
{
}

void CSelUnionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNION_LIST, m_wndUnionList);
}


BEGIN_MESSAGE_MAP(CSelUnionDlg, CDialog)
	ON_BN_CLICKED(IDC_UNION_DELETE, OnBnClickedUnionDelete)
	ON_LBN_DBLCLK(IDC_UNION_LIST, OnLbnDblclkUnionList)
END_MESSAGE_MAP()


// CSelUnionDlg message handlers

BOOL CSelUnionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CDlgUnionList* pParent = (CDlgUnionList*)this->GetOwner();

	if(pParent)
	{
		UnionSelCondition* pHSUnion;

		for(int i = 0;i < pParent->m_ayUnion.m_ayArray.GetSize();i++)
		{

			pHSUnion = pParent->m_ayUnion.m_ayArray.GetAt(i);
			if( pHSUnion != NULL)
			{
				CString strItem = pHSUnion->m_strName;
				int nItemIndex = this->m_wndUnionList.AddString(strItem);
				m_wndUnionList.SetItemData(nItemIndex,(DWORD)pHSUnion);
			}
		}
		m_wndUnionList.SetCurSel(0);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSelUnionDlg::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
	int nIndex = m_wndUnionList.GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	CDlgUnionList* pParent = (CDlgUnionList*)GetOwner();

	if(pParent)
	{
		UnionSelCondition* pHSUnion;
		pHSUnion = (UnionSelCondition*)m_wndUnionList.GetItemData(nIndex);
		if(pHSUnion)
		{
			pParent->SetUnion(pHSUnion);

			pParent->m_wndTree.DeleteAllItems();
			pParent->Insert(&pParent->m_wndTree,pParent->m_pHSUnion);
		}

	}

	CDialog::OnOK();
}

void CSelUnionDlg::OnBnClickedUnionDelete()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndUnionList.GetCurSel();
	if(nIndex == -1)
	{
		return;
	}

	CDlgUnionList* pParent = (CDlgUnionList*)GetOwner();

	if(pParent)
	{
		UnionSelCondition* pHSUnion;
		UnionSelCondition* pCurHSUnion;
		pHSUnion = (UnionSelCondition*)m_wndUnionList.GetItemData(nIndex);
		if(pHSUnion)
		{
			for(int i = 0;i < pParent->m_ayUnion.m_ayArray.GetSize();i++)
			{
				pCurHSUnion = pParent->m_ayUnion.m_ayArray.GetAt(i);
				if(pCurHSUnion)
				{
					if(pHSUnion == pCurHSUnion)
					{
						delete pCurHSUnion;
						pParent->m_ayUnion.m_ayArray.RemoveAt(i);
						m_wndUnionList.DeleteString(nIndex);
						return;
					}
				}
			}
		}

	}


}

void CSelUnionDlg::OnLbnDblclkUnionList()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
