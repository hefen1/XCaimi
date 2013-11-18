// DlgUnionList.cpp : implementation file -- added by HS
//

#include "stdafx.h"
//#include "HSCompMan.h"
#include "DlgUnionList.h"

#include "resource.h"
#include "DlgFormulaBrowser.h"
#include "DlgFormulaMan.h"
#include "dlgCondition.h"

#include "SelUnionDlg.h"
#include "DlgInputUnionName.h"

#include "GeneralHelper.h"

// CDlgUnionList dialog
static char* m_strCond[] =
{
	"下列条件之一成立",
		"下列条件同时成立",
		"向前%i-%i天之间,下列条件之一成立",
		"向前%i-%i天之间,下列条件同时成立"
};

IMPLEMENT_DYNAMIC(CDlgUnionList, CDialog)
CDlgUnionList::CDlgUnionList(CWnd* pParent /*=NULL*/)
: CDialog(CDlgUnionList::IDD, pParent)
{
	m_pHSUnion = NULL;
	//	m_bCreate   = -1;
}

CDlgUnionList::~CDlgUnionList()
{
	SaveLoad(CArchive::store);
	
}

void CDlgUnionList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_TREE, m_wndTree);

}


BEGIN_MESSAGE_MAP(CDlgUnionList, CDialog)
	//{{AFX_MSG_MAP(CDlgUnionList)

	ON_BN_CLICKED(IDD_NEW, OnNew)
	ON_BN_CLICKED(IDD_DELETE, OnDelete)
	ON_BN_CLICKED(IDD_AND, OnAnd)
	ON_BN_CLICKED(IDD_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDD_OR, OnOr)

	//	ON_NOTIFY(NM_CLICK, IDD_TREE, OnClickTree)
	//	ON_NOTIFY(TVN_SELCHANGED, IDD_TREE, OnSelchangedTree)
	//	ON_NOTIFY(NM_DBLCLK, IDD_TREE, OnDblclkTree)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDD_SaveUnion, OnSaveUnion)
	ON_NOTIFY(NM_DBLCLK, IDD_TREE, OnDblclkTree)
	ON_NOTIFY(TVN_KEYDOWN, IDD_TREE, OnKeyDelete)
	ON_NOTIFY(TVN_SELCHANGED, IDD_TREE, OnSelChanged)

	ON_BN_CLICKED(IDD_LoadUnion, OnBnClickedLoadunion)
END_MESSAGE_MAP()



// CDlgUnionList message handlers
BOOL CDlgUnionList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndTree.SetImageList(&CDlgFormulaMan::m_expressImages,TVSIL_NORMAL);
	COLORREF color = RGB(255,255,255);
	m_wndTree.SetBkColor(color );

	m_pHSUnion = new UnionSelCondition;
	UnionSelConditionSub* pUnionGroup = new UnionSelConditionSub;
	pUnionGroup->m_strName = _T(m_strCond[1]);
	pUnionGroup->m_cAndOr  = 1;
	AddUnionGroup(pUnionGroup);


	/*	if( m_bCreate )
	{
	m_pHSUnion = new UnionSelCondition;
	UnionSelConditionSub* pUnionGroup = new UnionSelConditionSub;
	pUnionGroup->m_strName = m_strCond[1];
	pUnionGroup->m_cAndOr  = 0;

	AddUnionGroup(pUnionGroup);
	}
	else
	{
	Insert(&m_wndTree,m_pHSUnion);
	}

	//	OnAdvance();
	*/
	SaveLoad(CArchive::load);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgUnionList::OnNew() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = this->GetParent();
	if (pWnd == NULL) return;

	pWnd->SendMessage(HX_USER_LOADCUR,TEMPUNION_NEWCONDITION,(LPARAM)this);

	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem == NULL )
	{
		return;
	}


	//要改，基本上改成父窗口的 m_pSingleSel

	/*	CWnd* pWnd = GetParent();
	if(pWnd != NULL)
	pWnd = pWnd->GetParent();

	if(pWnd != NULL)
	{
	CDlgFormulaBrowser dlg(pWnd);
	dlg.m_nType = 1;
	if( dlg.DoModal() == IDOK )
	{
	*/	
	if(CDlgUnionList::JudgeConditionEmpty(this->m_pHSUnion) != 0)
	{
		HTREEITEM hChildItem =  hItem ;
		HTREEITEM hNextItem ;
		while(hChildItem != NULL)
		{
			hNextItem = hChildItem;
			hChildItem = m_wndTree.GetChildItem( hChildItem );
		}
		SingleSelCondition* pHSUnionData;

		while(hNextItem != NULL)
		{
			pHSUnionData = (SingleSelCondition*)(m_wndTree.GetItemData(hNextItem));
			if (pHSUnionData != NULL)
			{
				if((m_pSingleSel->m_nType == Condition) ||
					(m_pSingleSel->m_nType ==MoreKLine))
				{
					if(m_pSingleSel->m_strCondition)
					{
						m_pSingleSel->m_strCondition = "";
					}
				}

				if(m_pSingleSel->m_strExp && pHSUnionData->m_strExp &&
					m_pSingleSel->m_strCondition && pHSUnionData->m_strCondition)
				{
					if(((m_pSingleSel->m_strExp).CompareNoCase(pHSUnionData->m_strExp) == 0)
						&& (m_pSingleSel->m_nType == pHSUnionData->m_nType) &&
						(m_pSingleSel->m_strCondition.CompareNoCase(pHSUnionData->m_strCondition) == 0))
					{
						MessageBox(_T("条件重复，请重新选择！"),_T("提示"),MB_ICONINFORMATION);
						return;
					}
				}
			}

			hNextItem = m_wndTree.GetNextSiblingItem( hNextItem );
		}
	}


	HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
	if( hParentItem == NULL )
		hParentItem = hItem;

	UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
	if( pUnionGroup == NULL )
		return;

	SingleSelCondition* pUnionData = new SingleSelCondition;
	pUnionGroup->m_aySub->Add(pUnionData);
	//			pUnionData->Copy(&dlg.m_pSingleSel);
	pUnionData->Copy(this->m_pSingleSel);

	CString* strRet = NULL;
	CString  strDes;
	switch( m_pSingleSel->m_nType )      // 公式类型
	{
	case Tech:
		{
			strRet = &CTreeCtrlFormula::g_strTech;
			strDes = m_pSingleSel->GetDescribe(strRet);
		}
		break;
	case Condition:
		{
			strRet = &CTreeCtrlFormula::g_strCondition;
			strDes = m_pSingleSel->GetDescribe(strRet);
		}
		break;
	case Exchange:
		{
			strRet = &CTreeCtrlFormula::g_strExchange;
			strDes = m_pSingleSel->GetDescribe(strRet);
		}
		break;
	case Strategy: //20090302 YJT 增加 Strategy
		{
			strRet = &CTreeCtrlFormula::g_strStrategy;
			strDes = m_pSingleSel->GetDescribe(strRet);
		}
		break;
	case MoreKLine:
		{
			strRet = &CTreeCtrlFormula::g_strColorKLine;
			strDes = m_pSingleSel->GetDescribe(strRet);
		}
		break;
	case JiBenMian:
		{
			CString strCondition = m_pSingleSel->m_strCondition;
			m_pSingleSel->m_strCondition = CDlgCondition::GetNameByKey(m_pSingleSel->m_strCondition);
			strDes = m_pSingleSel->GetDescribe(strRet);
			m_pSingleSel->m_strCondition = strCondition;
		}
		break;
	}
	/*
	switch( m_pSingleSel->m_nType )      // 公式类型
	{
	case Tech:
	strRet = &CTreeCtrlFormula::g_strTech;
	break;
	case Condition:
	strRet = &CTreeCtrlFormula::g_strCondition;
	break;
	case Exchange:
	strRet = &CTreeCtrlFormula::g_strExchange;
	break;
	case MoreKLine:
	strRet = &CTreeCtrlFormula::g_strColorKLine;
	break;
	}			
	CString strCondition = m_pSingleSel->m_strCondition;
	m_pSingleSel->m_strCondition = CDlgCondition::GetNameByKey(m_pSingleSel->m_strCondition);
	CString strDes = m_pSingleSel->GetDescribe(strRet);
	m_pSingleSel->m_strCondition = strCondition;
	*/
	int nImg;
	if( m_pSingleSel->m_nType == -2 )
	{
		nImg = CExpression::HS_Exp_Condition;
	}
	else
	{
		nImg = CExpression::GetImgIndex(m_pSingleSel->m_nType);
	}
	hItem = m_wndTree.InsertItem( strDes, 
		nImg,
		nImg,
		hParentItem);

	m_wndTree.SetItemData(hItem,(DWORD)pUnionData);
	m_wndTree.Expand(hParentItem,TVE_EXPAND);
}
//	}
//}


void CDlgUnionList::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = GetParent();
	if(pWnd != NULL)
		pWnd = pWnd->GetParent();

	if(pWnd == NULL)
		return;

	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem == NULL )
		return;

	HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
	if( hParentItem == NULL )
		return;

	SingleSelCondition* pUnionGroup = (SingleSelCondition*)m_wndTree.GetItemData(hItem);
	if( pUnionGroup == NULL )
		return;

	CDlgFormulaBrowser dlg(pWnd);
	dlg.m_nType = 1;
	dlg.m_sSingleSel.Copy(pUnionGroup);
	if( dlg.DoModal() == IDOK )
	{
		pUnionGroup->Copy(&dlg.m_sSingleSel);

		CString* strRet = NULL;
		CString  strDes;
		switch( dlg.m_sSingleSel.m_nType )      // 公式类型
		{
		case Tech:
			{
				strRet = &CTreeCtrlFormula::g_strTech;
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
			}
			break;
		case Condition:
			{
				strRet = &CTreeCtrlFormula::g_strCondition;
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
			}
			break;
		case Exchange:
			{
				strRet = &CTreeCtrlFormula::g_strExchange;
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
			}
			break;
		case Strategy: //20090302 YJT 增加 Strategy
			{
				strRet = &CTreeCtrlFormula::g_strStrategy;
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
			}
			break;
		case MoreKLine:
			{
				strRet = &CTreeCtrlFormula::g_strColorKLine;
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
			}
			break;
		case JiBenMian:
			{
				CString strCondition = dlg.m_sSingleSel.m_strCondition;
				dlg.m_sSingleSel.m_strCondition = CDlgCondition::GetNameByKey(dlg.m_sSingleSel.m_strCondition);
				strDes = dlg.m_sSingleSel.GetDescribe(strRet);
				dlg.m_sSingleSel.m_strCondition = strCondition;
			}
			break;
		}
		/*
		switch( dlg.m_sSingleSel.m_nType )      // 公式类型
		{
		case Tech:
		strRet = &CTreeCtrlFormula::g_strTech;
		break;
		case Condition:
		strRet = &CTreeCtrlFormula::g_strCondition;
		break;
		case Exchange:
		strRet = &CTreeCtrlFormula::g_strExchange;
		break;
		case MoreKLine:
		strRet = &CTreeCtrlFormula::g_strColorKLine;
		break;
		}

		CString strCondition = dlg.m_sSingleSel.m_strCondition;
		dlg.m_sSingleSel.m_strCondition = CDlgCondition::GetNameByKey(dlg.m_sSingleSel.m_strCondition);
		CString strDes = dlg.m_sSingleSel.GetDescribe(strRet);
		dlg.m_sSingleSel.m_strCondition = strCondition;
		*/

		m_wndTree.SetItemText( hItem,strDes );
	}
}


void CDlgUnionList::OnDelete() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem != NULL )
	{
		UnionSelConditionSub* pUnionGroup;
		HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
		if( hParentItem != NULL )
		{
			SingleSelCondition* pUnionGroup = (SingleSelCondition*)m_wndTree.GetItemData(hItem);
			DeleteItemData(m_pHSUnion,pUnionGroup);

			m_wndTree.DeleteItem(hItem);
		}
		else
		{
			hParentItem = hItem;
			pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
			if( pUnionGroup /*&& 
							pUnionGroup->m_nEndDate == 0 && 
							pUnionGroup->m_nBeginDate == 0*/)
			{
				return;
			}

			DeleteItemData(m_pHSUnion,pUnionGroup);
			m_wndTree.DeleteItem(hItem);
		}
	}
}


int CDlgUnionList::DeleteItemData(UnionSelCondition* pHSUnion,void* pDelData)
{
	if( pHSUnion == NULL )
		return -1;
	if( pDelData == pHSUnion )
	{
		delete pHSUnion;
		return 1;
	}

	UnionSelConditionSub* pSub;
	for(int i = 0; i < pHSUnion->m_aySub->GetSize(); i++)
	{
		pSub = pHSUnion->m_aySub->GetAt(i);
		if( (void*)pSub == pDelData)
		{
			delete pHSUnion->m_aySub->GetAt(i);
			pHSUnion->m_aySub->RemoveAt(i);
			return 2;
		}

		for(int j = 0; j < pSub->m_aySub->GetSize(); j++)
		{
			if( (void*)(pSub->m_aySub->GetAt(j)) == pDelData )
			{
				delete pSub->m_aySub->GetAt(j);
				pSub->m_aySub->RemoveAt(j);
				return 3;
			}
		}
	}

	return -1;
}

void CDlgUnionList::OnAnd() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem != NULL )
	{
		HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
		if( hParentItem == NULL )
			hParentItem = hItem;
		m_wndTree.SetItemImage(hParentItem,
			CExpression::HS_Exp_Union_Add,CExpression::HS_Exp_Union_Add);

		UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
		/*	if( pUnionGroup->m_nBeginDate != 0 &&
		pUnionGroup->m_nEndDate != 0 )
		{
		pUnionGroup->m_strName.Format(m_strCond[2],
		pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		else
		{
		pUnionGroup->m_strName = m_strCond[1];
		}
		*/	
		pUnionGroup->m_strName = _T(m_strCond[1]);
		pUnionGroup->m_cAndOr  = 1;
		m_wndTree.SetItemText(hParentItem,pUnionGroup->m_strName);
	}
}

void CDlgUnionList::OnOr() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem != NULL )
	{
		HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
		if( hParentItem == NULL )
			hParentItem = hItem;
		m_wndTree.SetItemImage(hParentItem,
			CExpression::HS_Exp_Union_Sub,CExpression::HS_Exp_Union_Sub);

		UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);

		/*if( pUnionGroup->m_nBeginDate != 0 &&
		pUnionGroup->m_nEndDate != 0 )
		{
		pUnionGroup->m_strName.Format(m_strCond[3],
		pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		else
		{
		pUnionGroup->m_strName = m_strCond[0];
		}
		*/
		pUnionGroup->m_strName = _T(m_strCond[0]);
		pUnionGroup->m_cAndOr  = 0;
		m_wndTree.SetItemText(hParentItem,pUnionGroup->m_strName);
	}
}

void CDlgUnionList::AddUnionArray(UnionSelCondition* pUnion)
{
	this->m_ayUnion.m_ayArray.Add(pUnion);
}

UnionSelCondition* CDlgUnionList::GetUnion()
{
	return this->m_pHSUnion;
}

void CDlgUnionList::SetUnion(UnionSelCondition* pUnion)
{
	this->m_pHSUnion->Copy(pUnion);
}

void CDlgUnionList::SetSingleUnion(SingleSelCondition* pUnionData)
{
	this->m_pSingleSel = pUnionData;
}


void CDlgUnionList::Insert(CTreeCtrl* pTree,UnionSelCondition* pHSUnion)
{
	if( pTree == NULL || pHSUnion == NULL )
		return;

	//	m_wndName.SetWindowText(pHSUnion->m_strName);

	UnionSelConditionSub* pSelSub;
	SingleSelCondition*   pUnionData;
	int i,j;

	HTREEITEM hParentItem,hItem;
	for( i = 0; i < pHSUnion->m_aySub->GetSize(); i++ )
	{
		pSelSub = pHSUnion->m_aySub->GetAt(i);

		hParentItem = pTree->InsertItem( pSelSub->m_strName,
			!pSelSub->m_cAndOr?CExpression::HS_Exp_Union_Sub:CExpression::HS_Exp_Union_Add,
			!pSelSub->m_cAndOr?CExpression::HS_Exp_Union_Sub:CExpression::HS_Exp_Union_Add );

		pTree->SetItemData( hParentItem,(DWORD)pSelSub );
		pTree->SelectItem( hParentItem );

		for( j = 0; j < pSelSub->m_aySub->GetSize(); j++ )
		{
			pUnionData = pSelSub->m_aySub->GetAt(j);

			CString* strRet = NULL;
			CString strDes;
			switch( pUnionData->m_nType )      // 公式类型
			{
			case Tech:
				{
					strRet = &CTreeCtrlFormula::g_strTech;
					strDes = pUnionData->GetDescribe(strRet);
				}
				break;
			case Condition:
				{
					strRet = &CTreeCtrlFormula::g_strCondition;
					strDes = pUnionData->GetDescribe(strRet);
				}
				break;
			case Exchange:
				{
					strRet = &CTreeCtrlFormula::g_strExchange;
					strDes = pUnionData->GetDescribe(strRet);
				}
				break;
			case Strategy: //20090302 YJT 增加 Strategy
				{
					strRet = &CTreeCtrlFormula::g_strStrategy;
					strDes = pUnionData->GetDescribe(strRet);
				}
				break;
			case MoreKLine:
				{
					strRet = &CTreeCtrlFormula::g_strColorKLine;
					strDes = pUnionData->GetDescribe(strRet);
				}
				break;
			case JiBenMian:
				{
					CString strCondition = pUnionData->m_strCondition;
					pUnionData->m_strCondition = CDlgCondition::GetNameByKey(pUnionData->m_strCondition);
					strDes = pUnionData->GetDescribe(strRet);
					pUnionData->m_strCondition = strCondition;
				}
				break;
			}

			int nImg;
			if( pUnionData->m_nType == -2 )
			{
				nImg = CExpression::HS_Exp_Condition;
			}
			else
			{
				nImg = CExpression::GetImgIndex(pUnionData->m_nType);
			}

			hItem = pTree->InsertItem( strDes, 
				nImg,
				nImg,
				hParentItem);

			pTree->SetItemData(hItem,(DWORD)pUnionData);
		}

		pTree->Expand(hParentItem,TVE_EXPAND);
	}
}



void CDlgUnionList::EnableBut(int nIndex ,bool bShowOrHide)
{
	CWnd* pWnd;
	if( nIndex == CDlgUnionList::AddButIndex)
	{
		pWnd = GetDlgItem(IDD_NEW);
		if(pWnd == NULL) return ;
		pWnd->EnableWindow(bShowOrHide);
	}
	if( nIndex == CDlgUnionList::UpdateButIndex)
	{
		pWnd = GetDlgItem(IDD_UPDATE);
		if(pWnd == NULL) return ;
		pWnd->EnableWindow(bShowOrHide);
	}
	if( nIndex == CDlgUnionList::DeleteButIndex)
	{
		pWnd = GetDlgItem(IDD_DELETE);
		if(pWnd == NULL) return ;
		pWnd->EnableWindow(bShowOrHide);
	}

}

HTREEITEM CDlgUnionList::AddUnionGroup(UnionSelConditionSub* pUnionGroup)
{
	if( pUnionGroup == NULL )
		return NULL;

	m_pHSUnion->m_aySub->Add(pUnionGroup);

	HTREEITEM hItem = m_wndTree.InsertItem( pUnionGroup->m_strName,
		CExpression::HS_Exp_Union_Add,CExpression::HS_Exp_Union_Add );

	m_wndTree.SetItemData( hItem,(DWORD)pUnionGroup );
	m_wndTree.SelectItem(hItem);

	return hItem;
}

void CDlgUnionList::OnSaveUnion()
{
	if(CDlgUnionList::JudgeConditionEmpty(this->m_pHSUnion) == 0)
	{
		MessageBox(_T("条件为空，请输入条件再保存,谢谢！"),_T("提示"),MB_ICONINFORMATION);
		return;
	}
	
	CDlgInputUnionName dlg(this);
	dlg.m_strBlockName = GetDefaultUnionName(m_pHSUnion);
	if(dlg.DoModal() == IDCANCEL)
	{
		return;
	}

	for(int i = 0 ; i < m_ayUnion.m_ayArray.GetSize() ; i++)
	{
		UnionSelCondition* pHSUnion = m_ayUnion.m_ayArray.GetAt(i);
		if( pHSUnion != NULL)
		{
			if( pHSUnion->m_strName.CompareNoCase(dlg.m_strBlockName) == 0)
			{
				if( MessageBox(_T("有重名,请确认是否覆盖?"),_T("提示"),
					MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) != IDYES )
				{
					return;
				}
				else
				{
					delete pHSUnion;
					this->m_ayUnion.m_ayArray.RemoveAt(i);
					break;
				}

			}
		}
	}

	UnionSelCondition* pHSUnion = new UnionSelCondition;

	pHSUnion->Copy(m_pHSUnion);
	pHSUnion->m_strName = dlg.m_strBlockName;

	m_ayUnion.m_ayArray.Add(pHSUnion);


}

CString CDlgUnionList::GetDefaultUnionName(UnionSelCondition* pHSUnion)
{
	if( pHSUnion == NULL )
		return "";

	//	m_wndName.SetWindowText(pHSUnion->m_strName);

	CString strRet;
	UnionSelConditionSub* pSelSub;
	SingleSelCondition*   pUnionData;
	int i,j;

	for( i = 0; i < pHSUnion->m_aySub->GetSize(); i++ )
	{
		pSelSub = pHSUnion->m_aySub->GetAt(i);

		for( j = 0; j < pSelSub->m_aySub->GetSize(); j++ )
		{
			pUnionData = pSelSub->m_aySub->GetAt(j);

			if(strRet != "")
			{
				strRet += "_";
			}
			strRet += pUnionData->m_strExp;
		}
	}
	return strRet;
}


void CDlgUnionList::OnDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//因刷新窗口问题没被解决， 故此功能暂时取消
	//// TODO: Add your control notification handler code here
	//this->OnUpdate();
	/////刷新窗口问题没被解决
	//this->FlashWindow(true);
	//this->SetActiveWindow();
	//this->ShowWindow(SW_SHOW );
	//*pResult = 0;
}

void CDlgUnionList::OnKeyDelete(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	if(pTVKeyDown->wVKey == VK_DELETE)
	{
		this->OnDelete();
	}
	*pResult = 0;
}

//判断条件是否为空的程序 返回０为空
bool CDlgUnionList::JudgeConditionEmpty(UnionSelCondition* pHSUnion)
{
	if(pHSUnion == NULL) return 0;
	CArray<UnionSelConditionSub*,UnionSelConditionSub*>* pAySub = pHSUnion->m_aySub ;
	if( pAySub ==NULL) return 0;
	if( pAySub->GetSize() == 0) return 0;
	for(int i = 0;i < pAySub->GetSize();i++)
	{
		UnionSelConditionSub* pSelSub = pAySub->GetAt(i);
		if( pSelSub == NULL) return 0;
		CArray<SingleSelCondition*,SingleSelCondition*>* pAyUnion = pSelSub->m_aySub;
		if( pAyUnion->GetSize() != 0) return 1; 
	}
	return 0;


}

void CDlgUnionList::PostNcDestroy()
{
	if(m_pHSUnion)
	{
		m_pHSUnion->Free();
		delete m_pHSUnion;
	}
	//	delete this;
	//WRITE_LOG ::PostNcDestroy();
}


void CDlgUnionList::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem == NULL )
	{
		this->EnableBut(CDlgUnionList::UpdateButIndex ,false);
		this->EnableBut(CDlgUnionList::DeleteButIndex ,false);
		return;
	}


	HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
	if( hParentItem == NULL )
	{
		this->EnableBut(CDlgUnionList::UpdateButIndex ,false);
		this->EnableBut(CDlgUnionList::DeleteButIndex ,false);
		return;
	}
	this->EnableBut(CDlgUnionList::UpdateButIndex ,true);
	this->EnableBut(CDlgUnionList::DeleteButIndex ,true);
	return;

	*pResult = 0;
}

BOOL CDlgUnionList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return false;
}


void CDlgUnionList::OnBnClickedLoadunion()
{
	// TODO: Add your control notification handler code here
	CSelUnionDlg dlg(this);
	dlg.SetOwner(this);
	if(dlg.DoModal() == IDOK)
	{		
	}
}

BOOL CDlgUnionList::SaveLoad(UINT nMode,CString strFileName /*= ""*/,DWORD dStyle /*= 0*/)
{
	/*if( !IsType(Condition) )
	return 0;
	*/
	CFile fp;
	if(strFileName.IsEmpty())
	{
		strFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +"Union.exp";
	}

	BOOL bOpen = FALSE;
	if(nMode == CArchive::store) // 存储
	{
		CString strTempPath;
		GetTempPath (MAX_PATH, strTempPath.GetBuffer (MAX_PATH));
		strTempPath.ReleaseBuffer();

		CString strTempName;
		GetTempFileName (strTempPath, _T("hsExp"), 0, strTempName.GetBuffer (MAX_PATH));
		strTempName.ReleaseBuffer ();

		bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeCreate | CFile::modeReadWrite );
		if(!bOpen) 
		{
			return FALSE;
		}

		try
		{
			CArchive ar( &fp, nMode );
			CExpression::HSFormulaFileHead head;
			head.m_strHead = CTreeCtrlFormula::g_strUnionCondition;
			head.Serialize(ar);

			int nCount = 0;
			Serialize(ar,nCount);
			ar.Flush();

			fp.SeekToBegin();
			ar << nCount;

			ar.Close();
			fp.Close();
		}
#ifdef VC_7_0
		catch( CMemoryException memoryex)
		{
			return FALSE;
		}
		catch( CArchiveException archiveex )
		{
			return FALSE;
		}
		catch( CFileException fileex)
		{
			return FALSE;
		}
#endif
		catch(...)
		{
			return FALSE;
		}

		// copy
		bOpen = fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | CFile::modeReadWrite );
		if( !bOpen ) 
		{
			return FALSE;
		}

		int lLen = fp.GetLength();
		if(lLen > 0)
		{
			char* pData = new char[lLen];
			fp.Read(pData,lLen);
			fp.Close();

			CFile curFile;
			if( curFile.Open( strFileName, CFile::typeBinary | CFile::shareDenyNone | 
				CFile::modeCreate | CFile::modeReadWrite ) )
			{
				curFile.Write(pData,lLen);
				curFile.Close();
			}
			delete[] pData;
		}

		// delete 
		TRY
		{
			CFile::Remove( strTempName );
		}
		CATCH( CFileException, e )
		{
		}
		END_CATCH

	}
	else // 装入
	{
		CFileException fileEx;
		bOpen = fp.Open( strFileName, CFile::typeBinary | CFile::shareDenyNone | 
			CFile::modeReadWrite, &fileEx); 		
		if(!bOpen) 
		{
			return FALSE;
		}

		try
		{
			CArchive ar( &fp, nMode );

			CExpression::HSFormulaFileHead head;
			head.Serialize(ar);
			if( !head.IsValid() )
				return FALSE;

			if(head.m_lCount < 0)
			{
				return FALSE;
			}

			char cStatus;
//			HTREEITEM hItem;
			UnionSelCondition* pHSUnion;
			for(int i = 0; i < head.m_lCount; i++)
			{
				ar >> cStatus;
				if( cStatus )
				{
					pHSUnion = new UnionSelCondition;
					pHSUnion->Serialize(ar);

					m_ayUnion.m_ayArray.Add(pHSUnion);
				}
			}

			fp.Close();
		}
#ifdef VC_7_0
		catch( CMemoryException memoryex)
		{
			return FALSE;
		}
		catch( CArchiveException archiveex )
		{
			return FALSE;
		}
		catch( CFileException fileex)
		{
			return FALSE;
		}
#endif
		catch(...)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CDlgUnionList::Serialize(CArchive& ar,int& nCountRef)
{
	nCountRef = 0;

	UnionSelCondition* pHSUnion;
	char cStatus;
	for(int i = 0;i < this->m_ayUnion.m_ayArray.GetSize();i++)
	{
		pHSUnion = this->m_ayUnion.m_ayArray.GetAt(i);
		
		if( pHSUnion )
		{
			cStatus = 1;
			ar << cStatus;
			pHSUnion->Serialize(ar);
		}
		else
		{
			cStatus = 0;
			ar << cStatus;
		}

		nCountRef++;
	}

	
}

BOOL CDlgUnionList::ShowWindow( int nCmdShow )
{

	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CDialog::ShowWindow(nCmdShow);


	if( IsIconic() )
	{
		return CDialog::ShowWindow(SW_RESTORE);
	}

	if(  IsZoomed() ) // max
		return CDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CDialog::ShowWindow(nCmdShow);
	}

	return 1;
}