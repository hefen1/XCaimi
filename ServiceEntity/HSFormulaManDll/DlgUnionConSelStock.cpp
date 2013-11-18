// DlgUnionConSelStock.cpp : implementation file
//

#include "stdafx.h"
#include "DlgUnionConSelStock.h"
#include "DlgFormulaBrowser.h"
#include "DlgSetStockAndTime.h"
#include "DlgFormulaMan.h"
#include "dlgCondition.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUnionConSelStock dialog
//static char* m_strCond[] =
//{
//	"下列条件之一成立",
//	"下列条件同时成立",
//	"向前%i-%i天之间,下列条件之一成立",
//	"向前%i-%i天之间,下列条件同时成立"
//};

CDlgUnionConSelStock::CDlgUnionConSelStock(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUnionConSelStock::IDD, pParent)
{
	m_pHSUnion = NULL;
	m_bCreate   = -1;

	//{{AFX_DATA_INIT(CDlgUnionConSelStock)
	//}}AFX_DATA_INIT
}


void CDlgUnionConSelStock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUnionConSelStock)
	DDX_Control(pDX, IDD_BEGINDATE, m_wndBeginDate);
	DDX_Control(pDX, IDD_ENDDATE, m_wndEndDate);
	DDX_Control(pDX, IDD_TREE, m_wndTree);
	DDX_Control(pDX, IDD_NAME, m_wndName);
	DDX_Control(pDX, IDD_ADVANCE, m_wndAdvance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUnionConSelStock, CDialog)
	//{{AFX_MSG_MAP(CDlgUnionConSelStock)
	ON_BN_CLICKED(IDD_NEWSET, OnNewset)
	ON_BN_CLICKED(IDD_NEWADD, OnNewadd)
	ON_BN_CLICKED(IDD_NEW, OnNew)
	ON_BN_CLICKED(IDD_DELETE, OnDelete)
	ON_BN_CLICKED(IDD_AND, OnAnd)
	ON_BN_CLICKED(IDD_ADVANCE, OnAdvance)
	ON_BN_CLICKED(IDD_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDD_OR, OnOr)
	ON_NOTIFY(NM_CLICK, IDD_TREE, OnClickTree)
	ON_NOTIFY(TVN_SELCHANGED, IDD_TREE, OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, IDD_TREE, OnDblclkTree)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUnionConSelStock message handlers

void CDlgUnionConSelStock::OnNewset() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem == NULL )
		return;
	
	HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
	if( hParentItem == NULL )
		hParentItem = hItem;
	
	UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
	if( pUnionGroup == NULL )
		return;
	
	if( pUnionGroup->m_nBeginDate != 0 &&
		pUnionGroup->m_nEndDate != 0 )
	{
		CString strBeginDate,strEndDate;
		m_wndBeginDate.GetWindowText(strBeginDate);
		if( strBeginDate.IsEmpty() )
		{
			//AfxMessageBox(_T("请输入天数."));
			MessageBox(_T("请输入天数."),_T(" 提示 "), MB_OK);
			m_wndBeginDate.SetFocus();
			return;
		}
		
		m_wndEndDate.GetWindowText(strEndDate);
		if( strEndDate.IsEmpty() )
		{
			//AfxMessageBox(_T("请输入天数."));
			MessageBox(_T("请输入天数."),_T(" 提示 "), MB_OK);
			m_wndEndDate.SetFocus();
			return;
		}
		
		if( atol(strBeginDate) > atol(strEndDate) )
		{
			//AfxMessageBox(_T("请输入天数不合法."));
			MessageBox(_T("输入天数不合法."),_T(" 提示 "), MB_OK);
			m_wndEndDate.SetFocus();
			return;
		}
		
		pUnionGroup->m_nBeginDate = atoi(strBeginDate);
		pUnionGroup->m_nEndDate   = atoi(strEndDate);

		if( pUnionGroup->m_cAndOr )
		{
			pUnionGroup->m_strName.Format(CDlgUnionConSelStock_Cond_Or_Pre,//m_strCond[2],
				pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		else
		{
			pUnionGroup->m_strName.Format(CDlgUnionConSelStock_Cond_And_Pre,//m_strCond[3],
				pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		m_wndTree.SetItemText(hParentItem,pUnionGroup->m_strName);
	}
	else
	{
		//AfxMessageBox(_T("基础条件不能设置."));
		MessageBox(_T("基础条件不能设置."),_T(" 提示 "), MB_OK);
		m_wndEndDate.SetWindowText("");
		m_wndBeginDate.SetWindowText("");
	}
}

void CDlgUnionConSelStock::OnNewadd() 
{
	// TODO: Add your control notification handler code here

	CString strBeginDate,strEndDate;
	m_wndBeginDate.GetWindowText(strBeginDate);
	if( strBeginDate.IsEmpty() )
	{
		//AfxMessageBox(_T("请输入天数."));
		MessageBox(_T("请输入天数."),_T(" 提示 "), MB_OK);
		m_wndBeginDate.SetFocus();
		return;
	}

	m_wndEndDate.GetWindowText(strEndDate);
	if( strEndDate.IsEmpty() )
	{
		//AfxMessageBox(_T("请输入天数."));
		MessageBox(_T("请输入天数."),_T(" 提示 "), MB_OK);
		m_wndEndDate.SetFocus();
		return;
	}

	if( atol(strBeginDate) > atol(strEndDate) )
	{
		//AfxMessageBox(_T("请输入天数不合法."));
		MessageBox(_T("输入天数不合法."),_T("提示 "), MB_OK);
		m_wndEndDate.SetFocus();
		return;
	}

	UnionSelConditionSub* pUnionGroup = new UnionSelConditionSub;
	pUnionGroup->m_nBeginDate = atoi(strBeginDate);
	pUnionGroup->m_nEndDate = atoi(strEndDate);

	pUnionGroup->m_strName.Format(CDlgUnionConSelStock_Cond_And_Pre,//m_strCond[3],
		pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
	pUnionGroup->m_cAndOr  = 0;
	
	AddUnionGroup(pUnionGroup);
}

void CDlgUnionConSelStock::OnNew() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndTree.GetSelectedItem();
	if( hItem == NULL )
	{
		return;
	}

	CWnd* pWnd = GetParent();
    if(pWnd != NULL)
		pWnd = pWnd->GetParent();

	if(pWnd != NULL)
	{
		CDlgFormulaBrowser dlg(pWnd);
		dlg.m_nType = 1;
		if( dlg.DoModal() == IDOK )
		{
			HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
			if( hParentItem == NULL )
				hParentItem = hItem;
			
			UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
			if( pUnionGroup == NULL )
				return;
			
			SingleSelCondition* pUnionData = new SingleSelCondition;
			pUnionGroup->m_aySub->Add(pUnionData);
			pUnionData->Copy(&dlg.m_sSingleSel);
			
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
			int nImg;
			if( dlg.m_sSingleSel.m_nType == -2 )
			{
				nImg = CExpression::HS_Exp_Condition;
			}
			else
			{
				nImg = CExpression::GetImgIndex(dlg.m_sSingleSel.m_nType);
			}
			hItem = m_wndTree.InsertItem( strDes, 
				nImg,
				nImg,
				hParentItem);
			
			m_wndTree.SetItemData(hItem,(DWORD)pUnionData);
			m_wndTree.Expand(hParentItem,TVE_EXPAND);
		}
	}
}

void CDlgUnionConSelStock::OnUpdate() 
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

void CDlgUnionConSelStock::OnDelete() 
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
			if( pUnionGroup && 
				pUnionGroup->m_nEndDate == 0 && 
				pUnionGroup->m_nBeginDate == 0)
			{
				return;
			}

			DeleteItemData(m_pHSUnion,pUnionGroup);
			m_wndTree.DeleteItem(hItem);
		}
	}
}

int CDlgUnionConSelStock::DeleteItemData(UnionSelCondition* pHSUnion,void* pDelData)
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

void CDlgUnionConSelStock::OnAnd() 
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
		if( pUnionGroup->m_nBeginDate != 0 &&
			pUnionGroup->m_nEndDate != 0 )
		{
			pUnionGroup->m_strName.Format(CDlgUnionConSelStock_Cond_Or_Pre,//m_strCond[2],
				pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		else
		{
			pUnionGroup->m_strName = CDlgUnionConSelStock_Cond_And;//m_strCond[1];
		}
		pUnionGroup->m_cAndOr  = 1;
		m_wndTree.SetItemText(hParentItem,pUnionGroup->m_strName);
	}
}

void CDlgUnionConSelStock::OnOr() 
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

		if( pUnionGroup->m_nBeginDate != 0 &&
			pUnionGroup->m_nEndDate != 0 )
		{
			pUnionGroup->m_strName.Format(CDlgUnionConSelStock_Cond_And_Pre,//m_strCond[3],
				pUnionGroup->m_nBeginDate,pUnionGroup->m_nEndDate);
		}
		else
		{
			pUnionGroup->m_strName = CDlgUnionConSelStock_Cond_Or;//m_strCond[0];
		}
		
		pUnionGroup->m_cAndOr  = 0;
		m_wndTree.SetItemText(hParentItem,pUnionGroup->m_strName);
	}
}

void CDlgUnionConSelStock::OnAdvance() 
{
	// TODO: Add your control notification handler code here
	CWnd* pPosWnd = this->GetDlgItem(IDC_POS);
	if( pPosWnd == NULL ) return;

	CWnd* pWnd = this->GetDlgItem(IDD_ADVANCE);
	if( pWnd == NULL ) return;

	CRect rcClient,rcPos;
	pPosWnd->GetWindowRect(rcPos);
	this->GetWindowRect(rcClient);
	CString strText;
	pWnd->GetWindowText(strText);
	
	if( !strText.Compare(_T("高级设定&G<<")) ) 
	{
		rcClient.bottom -= rcPos.Height();
		pWnd->SetWindowText(_T("高级设定&G>>"));
	}
	else
	{
		rcClient.bottom += rcPos.Height();
		pWnd->SetWindowText(_T("高级设定&G<<"));
	}
	this->MoveWindow(rcClient);
}

void CDlgUnionConSelStock::OnCancel() 
{
	// TODO: Add extra cleanup here
	Delete();
	CDialog::OnCancel();
}

void CDlgUnionConSelStock::OnOK() 
{
	// TODO: Add extra validation here
	CString strName;
	m_wndName.GetWindowText(strName);
	if( strName.IsEmpty() )
	{
		MessageBox(CDlgUnionConSelStock_Not_Name,_T(" 提示 "), MB_OK);
		m_wndName.SetFocus();
		return;
	}
	
	/*
	CValue* valoare = NULL;
	if( CExpression::m_pExternExpression->Lookup(strName,Union,valoare) ) // over
	{
		//if(strName.CompareNoCase(m_pHSUnion->m_strName))
		//{
		//	AfxMessageBox("名称已经存在！");
		//	m_wndName.SetFocus();
		//	return;
		//}
		m_pHSUnion->m_strName = strName;
	}
	else if( m_bCreate )
	{
		valoare  = new CExpValue();
		valoare->SetStyle(HX_UNION_EXPRESS);
		valoare->SetValue(m_pHSUnion);
		m_pHSUnion->m_strName = strName;

		CExpression::m_pExternExpression->AddExpress(strName,valoare);
	}
	else
	{
		m_pHSUnion->m_strName = strName;
	}
	*/

	CDlgCondition* pp = NULL;
	if( !pp->IsValidateUnion(m_pHSUnion) )
	{
		if( MessageBox(CDlgUnionConSelStock_Ok_Prompt,"提示",
			MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) != IDYES )
		{
			return;
		}
	}

	m_pHSUnion->m_strName = strName;

	Delete();

	CDialog::OnOK();
}

HTREEITEM CDlgUnionConSelStock::AddUnionGroup(UnionSelConditionSub* pUnionGroup)
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

BOOL CDlgUnionConSelStock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndTree.SetImageList(&CDlgFormulaMan::m_expressImages,TVSIL_NORMAL);
	//m_wndTree.SetBkColor(globalData.crListTreeBkClr);

	if( m_bCreate )
	{
		m_pHSUnion = new UnionSelCondition;
		UnionSelConditionSub* pUnionGroup = new UnionSelConditionSub;
		pUnionGroup->m_strName = CDlgUnionConSelStock_Cond_And;//m_strCond[1];
		pUnionGroup->m_cAndOr  = 1;
		
		AddUnionGroup(pUnionGroup);
	}
	else
	{
		Insert(&m_wndTree,m_pHSUnion);
	}

	OnAdvance();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUnionConSelStock::Insert(CTreeCtrl* pTree,UnionSelCondition* pHSUnion)
{
	if( pTree == NULL || pHSUnion == NULL )
		return;

	m_wndName.SetWindowText(pHSUnion->m_strName);

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

void CDlgUnionConSelStock::Delete()
{
	
}


void CDlgUnionConSelStock::OnClickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDlgUnionConSelStock::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMTreeView != NULL)
	{
		HTREEITEM hItem = ((*pNMTreeView).itemNew).hItem;
		if( hItem == NULL )
			return;

		BOOL bEnable = TRUE;
		HTREEITEM hParentItem = m_wndTree.GetParentItem(hItem);
		if( hParentItem == NULL )
		{
			bEnable = FALSE;
			hParentItem = hItem;
		}

		CWnd* pWnd = this->GetDlgItem(IDD_DELETE);
		if(pWnd != NULL)
		{
			pWnd->EnableWindow(bEnable);
		}
		pWnd = this->GetDlgItem(IDD_UPDATE);
		if(pWnd != NULL)
		{
			pWnd->EnableWindow(bEnable);
		}

		UnionSelConditionSub* pUnionGroup = (UnionSelConditionSub*)m_wndTree.GetItemData(hParentItem);
		if( pUnionGroup == NULL )
			return;

		CString strDate;
		strDate.Format("%i",pUnionGroup->m_nBeginDate);
		if( pUnionGroup->m_nBeginDate == 0 )
			strDate.Empty();
		m_wndBeginDate.SetWindowText(strDate);
	
		strDate.Format("%i",pUnionGroup->m_nEndDate);
		if( pUnionGroup->m_nEndDate == 0 )
			strDate.Empty();
		m_wndEndDate.SetWindowText(strDate);
	}

	*pResult = 0;
}

void CDlgUnionConSelStock::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnUpdate();
	*pResult = 0;
}

void CDlgUnionConSelStock::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CDlgUnionConSelStock::ShowWindow( int nCmdShow )
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