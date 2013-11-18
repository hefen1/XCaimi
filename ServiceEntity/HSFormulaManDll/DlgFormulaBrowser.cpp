// DlgFormulaBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFormulaBrowser.h"

#include "DlgFormulaMan.h"
#include "DlgFormulaFind.h"
#include "DlgFormulaEdit.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"

#include "dlgCondition.h"
#include "dlgCondition.h"
#include "PubStruct.h"
#include "Express.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern void     AddPeriodNameToWnd(CComboBox* pBox);
//extern int		GetPeriodNumByName(CString strPeriodName);

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaBrowser dialog

extern HWND g_hParentWnd;

CDlgFormulaBrowser::CDlgFormulaBrowser(CWnd* pParent /*=NULL*/,HSOpenExpressUseData* pData /*= NULL*/)
: CDialog(CDlgFormulaBrowser::IDD, pParent)
{
	m_pData = pData;
	m_nType = 0;
	m_hParent = g_hParentWnd;

	//{{AFX_DATA_INIT(CDlgFormulaBrowser)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFormulaBrowser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormulaBrowser)
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDD_PERIOD_STATIC, m_wndPeriodStc);
	DDX_Control(pDX, IDD_PERIOD, m_wndPeriod);

	DDX_Control(pDX, IDD_CONNOTES, m_wndConNotes);
	DDX_Control(pDX, IDD_CON, m_wndCon);
	DDX_Control(pDX, IDC_STATIC_CON, m_wndStaticCon);

	DDX_Control(pDX, IDD_COMPARE, m_wndCompare);
	DDX_Control(pDX, IDD_DATA2, m_wndData2);
	DDX_Control(pDX, IDD_AND_OR, m_wndAndOr);
	DDX_Control(pDX, IDD_DATA1, m_wndData1);
	DDX_Control(pDX, IDC_MIDDLE, m_wndMiddle);

}


BEGIN_MESSAGE_MAP(CDlgFormulaBrowser, CDialog)
	//{{AFX_MSG_MAP(CDlgFormulaBrowser)
	ON_BN_CLICKED(IDC_BUTFIND, OnButfind)
	ON_BN_CLICKED(IDD_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_NOTES, OnNotes)
	ON_CBN_SELCHANGE(IDD_CON, OnSelchangeCon)
	ON_CBN_SELCHANGE(IDD_COMPARE, OnSelchangeCompare)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)
	//}}AFX_MSG_MAP



	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaBrowser message handlers

void CDlgFormulaBrowser::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_nType != 1 && m_pData->m_wType & hxShowByExpressType)
	{
		if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_TechSort ) // 指标排序
		{
 			::SendMessage(m_hParent,HX_USER_COMPILEDATA,HS_SORT_ANALYSIS,
 				(LPARAM)-12);
		}
		else if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_AddTechSort ) // 添加指标排序
		{
 			::SendMessage(m_hParent,HX_USER_COMPILEDATA,HS_ADD_SORT_ANALYSIS,
 				(LPARAM)-12);
		}
	}
	CDialog::OnCancel();
}

void CDlgFormulaBrowser::OnOK() 
{
	if(m_nType == 1)
	{
		if( m_sSingleSel.m_nType == hx_DefMaxValues )
		{
			goto _Exit;
			//AfxMessageBox("请选择指标!");
			//return;
		}

		// 周期
		int nIndex = m_wndPeriod.GetCurSel();
		if( nIndex != -1 )
		{
			CString strPeriod;
			m_wndPeriod.GetLBText(nIndex,strPeriod);			
			m_sSingleSel.m_nPeriod = GetPeriodNumByName(strPeriod);    // 周期
		}

		//int nIndex = m_wndPeriod.GetCurSel();
		//if( nIndex != -1 )
		//{
		//	m_sSingleSel.m_nPeriod = nIndex;    // 周期
		//}

		m_wndCon.GetWindowText(m_sSingleSel.m_strCondition); // 技术指标指:哪一个指标线;交易系统指:哪种信号;基本面指:财务指标ID
		nIndex = m_wndCon.GetCurSel();
		if( nIndex != -1 )
		{
			DWORD dKey = m_wndCon.GetItemData(nIndex);
			if( dKey != 0 )
			{
				m_sSingleSel.m_strCondition.Format("%i",dKey);
			}
		}

		if(!m_sSingleSel.m_strCondition.Compare(CTreeCtrlFormula::g_strFirstTechLine))
		{
			m_sSingleSel.m_strCondition = "";
		}

		if( !(m_sSingleSel.m_nType == Condition || // 条件选股 
			  m_sSingleSel.m_nType == Exchange  || // 交易系统
			  m_sSingleSel.m_nType == MoreKLine || // 五彩K线
			  m_sSingleSel.m_nType == Strategy )) // 策略交易 //20090302 YJT 增加 Strategy
		{
			m_sSingleSel.m_SubData.m_cType = m_wndCompare.GetCurSel();
			m_wndData1.GetWindowText(m_sSingleSel.m_SubData.m_dData1);
			m_wndData2.GetWindowText(m_sSingleSel.m_SubData.m_dData2);
		}
		else
		{
			m_sSingleSel.m_strCondition.Empty();
			m_sSingleSel.m_SubData.m_dData1.Empty();
			m_sSingleSel.m_SubData.m_dData2.Empty();
		}

		CDialog::OnOK();
		return;
	}
	else
	{
		if( m_pData == NULL )
			goto _Exit;
		CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
		if( pParentDlg == NULL )
			goto _Exit;

		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( !pCurTree )
			goto _Exit;
		HTREEITEM hItem = pCurTree->GetSelectedItem();
		if( hItem == NULL )
			goto _Exit;

		CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
		if( (pTreeGroup == NULL)			|| 
			(pTreeGroup->GetData() == NULL) ||
			!pTreeGroup->IsStatus(hx_GroupItem) )
			goto _Exit;

		if( m_pData->m_wType & hxShowReturnByExpressType )
		{
			if( (m_pData->m_wType & 0xF) <= MoreKLine )
			{
				m_pData->m_pData = pTreeGroup->GetData();
			}
		}
		else if(m_pData->m_wType & hxShowByExpressType)
		{
			if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_AddTech )	     // 叠加指标
			{
				::SendMessage(m_hParent,HX_USER_DRAGDATA,
					HSCurKeyInfo::DragUpdateExpressData3/*(HX_USER_DRAGDATA-3)*/,
					(LPARAM)pTreeGroup->GetData());
			}
			else if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_TechSort ) // 指标排序
			{
 				::SendMessage(m_hParent,HX_USER_COMPILEDATA,HS_SORT_ANALYSIS,
 					(LPARAM)pTreeGroup->GetData());
			}
			else if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_AddTechSort ) // 添加指标排序
			{
 				::SendMessage(m_hParent,HX_USER_COMPILEDATA,HS_ADD_SORT_ANALYSIS,
 					(LPARAM)pTreeGroup->GetData());
			}
			else
			{
				::SendMessage(m_hParent,HX_USER_DRAGDATA,
					HSCurKeyInfo::DragUpdateExpressData2/*(HX_USER_DRAGDATA-2)*/,
					(LPARAM)pTreeGroup->GetData());
			}
		}
		CDialog::OnOK();
		return;
	}

_Exit:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
}

void CDlgFormulaBrowser::OnButfind() 
{
	// TODO: Add your control notification handler code here
	//by HS begin
	/*CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
	return;
	pParentDlg->Find(this);
	*/
	DWORD dwMask = 0xFFFF;
	if(this->m_nType == 0)
	{
		if( m_pData != NULL )
		{
			dwMask = (m_pData->m_wType & 0xf);
		}
	}
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pParentDlg)
	{
		pParentDlg->Find(dwMask,this);
	}

	//by HS end 
}

void CDlgFormulaBrowser::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
		return;
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	if( pExpression == NULL )
		return;

	CDlgFormulaEdit* pDlg = pParentDlg->GetExpDlg();
	if( pDlg != NULL )
	{
		if(pDlg->SendMessage(HX_USER_LOADCUR,CCompileEditView::UpdateExpress,(LPARAM)pExpression))
		{
			pDlg->ShowWindow(SW_SHOWNORMAL);
		}
	}
}

void CDlgFormulaBrowser::OnNotes() 
{
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
	{
		goto OnNotes_End;
	}

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( !pCurTree )
	{
		goto OnNotes_End;
	}
	HTREEITEM hItem = pCurTree->GetSelectedItem();
	if( hItem == NULL )
	{
		goto OnNotes_End;
	}

	CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
	if( (pTreeGroup == NULL)			|| 
		(pTreeGroup->GetData() == NULL) ||
		!pTreeGroup->IsStatus(hx_GroupItem) )
	{
		goto OnNotes_End;
	}

	CExpression* pExpression = (CExpression*)pTreeGroup->GetData();
	if( pExpression == NULL )
	{		
		goto OnNotes_End;
	}

	CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
	if( pDlgNotes != NULL )
	{
		pDlgNotes->m_pExpression = pExpression;
		pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		pDlgNotes->ShowWindow(SW_SHOW);
	}
	return;

OnNotes_End:
	::MessageBox( m_hWnd,_T(" 请选择一个公式! "),_T(" 提示 "), MB_OK);
}

BOOL CDlgFormulaBrowser::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//added  by HS 因现在分析周期只支持日线，暂屏蔽其它分析周期  20020614
//#ifdef _ONLY_SUPPORT_DAYLINE
//
//	m_wndPeriod.ResetContent();
//	m_wndPeriod.AddString("日线");
//
//#endif
	m_wndPeriod.ResetContent();
	AddPeriodNameToWnd(&m_wndPeriod);
	//end 

	m_EditEidolon.CreateEx(WS_EX_CLIENTEDGE,NULL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,
		CRect(0,0,0,0),this,1001);
	m_EditEidolon.SetFont( GetFont() );

	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_PARAM);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_EditEidolon.MoveWindow(rect);

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)AfxGetMainWnd();

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		if( m_nType == 1 ) // 选择条件
		{
			pDlg->CopyTree(pCurTree);

			//HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strUnionCondition, 0, 0);
			//	HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strJiben, CExpression::HS_Exp_Condition, CExpression::HS_Exp_Condition);
			HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strJiben,
				CExpression::HS_Exp_Condition, CExpression::HS_Exp_Condition);
			CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)JiBenMian); 
			pCurTree->SetItemData(hRootItem, (DWORD)pTreeGroup);

			CString strText;
			BOOL bSet = TRUE;
			CString strFind;

			this->SetWindowText(_T("选择条件"));

			switch(m_sSingleSel.m_nType)
			{
			case JiBenMian: //
				{
					if( hRootItem != NULL)
					{			
						pCurTree->SelectItem(hRootItem);
					}
					LoadCurExp(CTreeCtrlFormula::HSBrowser, (LPARAM)pTreeGroup);

 					strFind = CDlgCondition::GetNameByKey(m_sSingleSel.m_strCondition);
				}
				break;
			case Tech:
			case Condition:
			case Exchange:
			case Strategy: //20090302 YJT 增加 Strategy
			case MoreKLine:
				{
					CValue* valoare;
					CExpression* pExpress = NULL;
					if( CExpression::m_pExternExpression->Lookup( m_sSingleSel.m_strExp,
						m_sSingleSel.m_nType,valoare ) )
					{
						pExpress = valoare->GetExp();

						pCurTree->Show(pExpress,0);
						LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);
					}

					strFind = m_sSingleSel.m_strCondition;
				}
				break;
			default:
				bSet = FALSE;
				break;
			}
			if( bSet )
			{
				m_wndCon.SelectString(0,strFind);

				m_wndCompare.SetCurSel(m_sSingleSel.m_SubData.m_cType);
				OnSelchangeCompare();

				strText = hxSubZero(m_sSingleSel.m_SubData.m_dData1);
				m_wndData1.SetWindowText(strText);

				strText = hxSubZero(m_sSingleSel.m_SubData.m_dData2);
				m_wndData2.SetWindowText(strText);
 
				int nPos;
				GetPeriodStr(m_sSingleSel.m_nPeriod,&nPos);
				m_wndPeriod.SetCurSel(nPos);
			}
		}
		else
		{
			if( m_pData != NULL )
			{
				if( m_pData->m_strName != NULL )
				{
					this->SetWindowText(*m_pData->m_strName);
				}
				WORD nType = (m_pData->m_wType & 0xF);

				int nCopy = CTreeCtrlFormula::CopyNone;
				if( m_pData->m_wType & hxShowByExpressType )
				{
					if( (m_pData->m_wType & 0xF0) == hxShowByExpressType_AddTech )
					{
						nCopy = CTreeCtrlFormula::CopyNotGroup | CTreeCtrlFormula::CopyNone | HS_EXP_MAINCHART;
						m_wndTab.DeleteAllItems();
					}
				}

				if( HS_EXP_OUTTYPE(m_pData->m_dStyle) )
 				{
					nCopy |= CTreeCtrlFormula::CopyNotGroup; 
				}

				if( nType != 0 )
				{
					CTreeCtrlFormula* pTree;
					for( int i = 0; i < 5; i++ ) //20090302 YJT 修改
					{
						switch(i)
						{
						case 0:
							nType = ((m_pData->m_wType & 0xF) & Tech);
							break;
						case 1:
							nType = ((m_pData->m_wType & 0xF) & Condition);
							break;
						case 2:
							nType = ((m_pData->m_wType & 0xF) & Exchange);
							break;
						case 3:
							nType = ((m_pData->m_wType & 0xF) & MoreKLine);
							break;
						case 4:
							nType = ((m_pData->m_wType & 0xFF) & Strategy);
							break;
						}

						pTree = pDlg->GetCurTree( nType );
						pCurTree->CopyTree(pTree,nCopy,m_pData->m_dStyle);
					}

					if( m_pData->m_pData != NULL )
					{
						pCurTree->Show((CExpression*)m_pData->m_pData,0);
					}
					else
					{
						pCurTree->Show(NULL,0); // 显示第一个

						/*
						HTREEITEM hItem = pCurTree->GetRootItem();
						if(hItem != NULL)
						{
							pCurTree->Expand(hItem,TVE_EXPAND);
						}
						*/
					}

					//pCurTree->SetFocus();

				}
				else
				{
					pDlg->CopyTree(pCurTree,CTreeCtrlFormula::CopyDelete);
				}
			}
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgFormulaBrowser::ShowWindow( int nCmdShow )
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

long CDlgFormulaBrowser::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	m_sSingleSel.m_nType = hx_DefMaxValues;

	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pParentDlg == NULL )
		return 0;

	EnableItem(FALSE);

	if( wParam == CTreeCtrlFormula::HSBrowser )
	{
		if(m_nType == 1)
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)lParam;
			//	CString strItem = *(CString*)lParam;
			/*	if( !strItem.CompareNoCase(CTreeCtrlFormula::g_strUnionCondition) )
			{
			m_sSingleSel.m_nType = Union;
			m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strUnionCondition);

			m_wndPeriod.ShowWindow(SW_HIDE);
			m_wndPeriodStc.ShowWindow(SW_HIDE);

			m_wndConNotes.ShowWindow(SW_HIDE);
			m_wndCon.ShowWindow(SW_HIDE);
			m_wndStaticCon.ShowWindow(SW_HIDE);

			m_wndCompare.ShowWindow(SW_HIDE);
			m_wndData1.ShowWindow(SW_HIDE);

			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);

			}*/
			//	else if( !strItem.CompareNoCase(CTreeCtrlFormula::g_strJiben) )
			/*	else*/ if( pTreeGroup->m_dwID == JiBenMian )
			{
				m_sSingleSel.m_nType = JiBenMian;
				m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strJiben);

				m_wndPeriod.ShowWindow(SW_HIDE);

#ifdef _ONLY_SUPPORT_DAYLINE

				m_wndPeriod.SetCurSel(0);
#else

				m_wndPeriod.SetCurSel(5);
#endif
				m_wndPeriodStc.ShowWindow(SW_HIDE);

				m_wndConNotes.ShowWindow(SW_SHOW);
				m_wndConNotes.SetWindowText(_T("财务指标"));
				m_wndCon.ShowWindow(SW_SHOW);
				m_wndStaticCon.ShowWindow(SW_SHOW);

				m_wndCompare.ShowWindow(SW_SHOW);
				m_wndData1.ShowWindow(SW_SHOW);

				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);

				m_wndData1.ResetContent();
				m_wndData2.ResetContent();

				m_wndCon.ResetContent();
				CDlgCondition::InitCWData(&m_wndCon,_T("股本数据"));
				CDlgCondition::InitCWData(&m_wndCon,_T("财务数据"));

				CDlgCondition::InitCondition(&m_wndCompare,4);
			}
		}
	}
	else if( wParam == CCompileEditView::UpdateExpress )
	{
		CDlgFormulaEdit* pDlg = pParentDlg->GetExpDlg();
		if( pDlg != NULL )
		{
			if(pDlg->SendMessage(HX_USER_LOADCUR,wParam,lParam))
			{
				pParentDlg->Enable(FALSE);
				pDlg->ShowWindow(SW_SHOWNORMAL);
			}
		}
	}
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		//EnableItem(FALSE);

		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;

		//
		if(m_nType == 1)
		{
			m_sSingleSel.m_nType = pExpression->GetExpressType();
			m_sSingleSel.m_strExp.Format("%s",pExpression->GetName());
			CDlgCondition::InitCondition(&m_wndCompare,-1);

			switch ( m_sSingleSel.m_nType )
			{
			case Tech:
				{
					//"选出技术指标数值满足下列条件的股票:");

					m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

					m_wndPeriod.SetCurSel(0);
#else

					m_wndPeriod.SetCurSel(5);
#endif
					m_wndPeriodStc.ShowWindow(SW_SHOW);

					m_wndConNotes.ShowWindow(SW_SHOW);
					m_wndConNotes.SetWindowText(_T("指标线"));
					m_wndCon.ShowWindow(SW_SHOW);
					m_wndCon.ResetContent();
					m_wndStaticCon.ShowWindow(SW_SHOW);

					m_wndCompare.ShowWindow(SW_SHOW);
					m_wndData1.ShowWindow(SW_SHOW);

					CValue* valoare = NULL;
					if( CExpression::m_pExternExpression->Lookup(pExpression->GetName(),pExpression->GetExpressType(),valoare) ) // over
					{
						CExpValue* pExpValue = (CExpValue*)valoare;

						BOOL bFree = FALSE;
						CExpression* pCurExpression = pExpValue->GetExp();
						if( pCurExpression && pCurExpression == pExpression )
						{
							if( !pCurExpression->IsCompile() )
							{
								if( !pCurExpression->Compile() )
								{
									bFree = TRUE;
								}
							}
							CArray<CFormulaDrawParam*,CFormulaDrawParam*> ayDraw;
							CString strParam;
							int nCount = pCurExpression->GetDraw(strParam,ayDraw);
							CFormulaDrawParam* pParam;
							if( nCount > 0 )
							{
								for(int i = 0; i < nCount; i++)
								{
									pParam = ayDraw.GetAt(i);
									if( nCount == 1 && CString(pParam->m_strName).IsEmpty() )
									{
										m_wndCon.AddString(CTreeCtrlFormula::g_strFirstTechLine);
									}
									else
									{//yulx modify 去掉后三位的0
										CString name = pParam->m_strName;
										name = name.Left(name.GetLength() - 3);
										m_wndCon.AddString(name/*pParam->m_strName*/);
									}
									delete pParam;
								}
							}
							else
							{
								m_wndCon.AddString(CTreeCtrlFormula::g_strFirstTechLine);
							}
							m_wndCon.SetCurSel(m_wndCon.GetCount()-1);

							OnSelchangeCompare();
						}
						if( bFree )
						{
							pExpValue->Free();
						}
					}

				}
				break;
			case Condition:
				{
					//"选出满足下列条件的股票:");

					m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Condition;

					m_wndPeriod.ShowWindow(SW_SHOW);

#ifdef _ONLY_SUPPORT_DAYLINE

					m_wndPeriod.SetCurSel(0);
#else

					m_wndPeriod.SetCurSel(5);
#endif
					m_wndPeriodStc.ShowWindow(SW_SHOW);

					m_wndConNotes.ShowWindow(SW_HIDE);
					m_wndCon.ShowWindow(SW_HIDE);
					m_wndStaticCon.ShowWindow(SW_HIDE);

					m_wndCompare.ShowWindow(SW_HIDE);
					m_wndData1.ShowWindow(SW_HIDE);

					m_wndAndOr.ShowWindow(SW_HIDE);
					m_wndData2.ShowWindow(SW_HIDE);
					m_wndMiddle.ShowWindow(SW_HIDE);
				}
				break;
			case Exchange:
				{
					//"选出交易系统发出以下信号的股票:");

					m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Exchange;

					m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

					m_wndPeriod.SetCurSel(0);
#else

					m_wndPeriod.SetCurSel(5);
#endif
					m_wndPeriodStc.ShowWindow(SW_SHOW);

					m_wndConNotes.ShowWindow(SW_HIDE);
					m_wndConNotes.SetWindowText(_T("发出"));
					m_wndCon.ShowWindow(SW_HIDE);
					m_wndStaticCon.ShowWindow(SW_HIDE);

					m_wndCompare.ShowWindow(SW_HIDE);
					m_wndData1.ShowWindow(SW_HIDE);

					m_wndAndOr.ShowWindow(SW_HIDE);
					m_wndData2.ShowWindow(SW_HIDE);
					m_wndMiddle.ShowWindow(SW_HIDE);
				}
				break;
			case Strategy: //20090302 YJT 增加 Strategy
				{
					//"选出交易系统发出以下信号的股票:");

					m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Strategy;

					m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

					m_wndPeriod.SetCurSel(0);
#else

					m_wndPeriod.SetCurSel(5);
#endif
					m_wndPeriodStc.ShowWindow(SW_SHOW);

					m_wndConNotes.ShowWindow(SW_HIDE);
					m_wndConNotes.SetWindowText(_T("发出"));
					m_wndCon.ShowWindow(SW_HIDE);
					m_wndStaticCon.ShowWindow(SW_HIDE);

					m_wndCompare.ShowWindow(SW_HIDE);
					m_wndData1.ShowWindow(SW_HIDE);

					m_wndAndOr.ShowWindow(SW_HIDE);
					m_wndData2.ShowWindow(SW_HIDE);
					m_wndMiddle.ShowWindow(SW_HIDE);
				}
				break;
			case MoreKLine:
				{
					//"选出五彩Ｋ线发出指示的股票:");

					m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::MoreKLine;

					m_wndPeriod.ShowWindow(SW_SHOW);
#ifdef _ONLY_SUPPORT_DAYLINE

					m_wndPeriod.SetCurSel(0);
#else

					m_wndPeriod.SetCurSel(5);
#endif
					m_wndPeriodStc.ShowWindow(SW_SHOW);

					m_wndConNotes.ShowWindow(SW_HIDE);
					m_wndCon.ShowWindow(SW_HIDE);
					m_wndStaticCon.ShowWindow(SW_HIDE);

					m_wndCompare.ShowWindow(SW_HIDE);
					m_wndData1.ShowWindow(SW_HIDE);

					m_wndAndOr.ShowWindow(SW_HIDE);
					m_wndData2.ShowWindow(SW_HIDE);
					m_wndMiddle.ShowWindow(SW_HIDE);
				}
				break;
			}
		}

		// 注释/参数修改
		CWnd* pWnd = GetDlgItem(IDC_NOTES);
		pWnd->ShowWindow(SW_SHOW);
		m_EditEidolon.ShowWindow(SW_SHOW);

		CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
		CDlgNotesAndEidolon* pDlgParam = pParentDlg->GetDlgParam();
		if( pDlgNotes != NULL && 
			pDlgNotes->IsWindowVisible() )
		{
			pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		}

		if( m_EditEidolon.m_hWnd != NULL )
		{		
			CString strText;
			strText.Format("%s",pExpression->GetEidolon());
			if( !strText.IsEmpty() )
			{
				m_EditEidolon.SetText(strText,pExpression->GetParamVar(),FALSE);
			}
			else
			{
				CStringArray* pArray = pExpression->GetDefEidolon();
				if(pArray->GetSize() <= 1)
				{
					pArray = NULL;
				}
				m_EditEidolon.SetText(pArray,pExpression->GetParamVar(),FALSE);
			}
			m_EditEidolon.Invalidate();
		}
	}

	return 0;
}

void CDlgFormulaBrowser::EnableItem(BOOL bEnable)
{
	CWnd* pWnd = GetDlgItem(IDC_NOTES);
	pWnd->ShowWindow(SW_HIDE);

	pWnd = GetDlgItem(IDD_UPDATE);
	pWnd->ShowWindow(SW_HIDE);

	m_wndPeriodStc.ShowWindow(SW_HIDE);
	m_wndPeriod.ShowWindow(SW_HIDE);

	m_wndConNotes.ShowWindow(SW_HIDE);
	m_wndCon.ShowWindow(SW_HIDE);
	m_wndStaticCon.ShowWindow(SW_HIDE);

	m_wndCompare.ShowWindow(SW_HIDE);
	m_wndData1.ShowWindow(SW_HIDE);
	m_wndAndOr.ShowWindow(SW_HIDE);
	m_wndData2.ShowWindow(SW_HIDE);
	m_wndMiddle.ShowWindow(SW_HIDE);

	m_EditEidolon.ShowWindow(SW_HIDE);
}


void CDlgFormulaBrowser::OnSelchangeCon() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeCompare();
}

void CDlgFormulaBrowser::OnSelchangeCompare() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_wndCompare.GetCurSel();
	if( nIndex == -1 ) 
		goto __OnSelchangeCompare_;

	switch( nIndex )
	{
	case FormulaCompareSubData::DaYu:		//  大于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoYu:		//	小于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::DengYu:		//	等于
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::JieYu:		//	介于
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("与"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_SHOW);
		}
		break;
	case FormulaCompareSubData::ShangChuan: //	上穿
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::XiaoPo:		//	下破
		{
			m_wndAndOr.ShowWindow(SW_HIDE);
			m_wndData2.ShowWindow(SW_HIDE);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::WeiChi:		//	维持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后上穿"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	case FormulaCompareSubData::BiaoChi:		//	保持
		{
			m_wndAndOr.ShowWindow(SW_SHOW);
			m_wndAndOr.SetWindowText(_T("周期后下破"));
			m_wndData2.ShowWindow(SW_SHOW);
			m_wndMiddle.ShowWindow(SW_HIDE);
		}
		break;
	}

__OnSelchangeCompare_:
	m_wndData1.ResetContent();
	m_wndData2.ResetContent();

	if( m_sSingleSel.m_nType >= 0 &&
		!(m_sSingleSel.m_nType == JiBenMian || 
		  m_sSingleSel.m_nType == Union)
		)
	{
		int nCount = m_wndCon.GetCount();
		if( nCount > 1 )
		{
			int nCur = m_wndCon.GetCurSel();
			CString strTmp;
			for( int i = 0; i < nCount; i++)
			{
				if( i != nCur )
				{
					m_wndCon.GetLBText(i,strTmp);
					if( nIndex != FormulaCompareSubData::WeiChi && nIndex != FormulaCompareSubData::BiaoChi )
					{
						m_wndData1.AddString(strTmp);
					}
					m_wndData2.AddString(strTmp);
				}
			}
		}
	}
}

void CDlgFormulaBrowser::PostNcDestroy()
{
	CTreeCtrlFormula* pCurTree = this->m_wndTab.GetTree();
	if( pCurTree )
	{
		HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strJiben);
		if(hRootItem)
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)(pCurTree->GetItemData(hRootItem));
			delete pTreeGroup;
		}

	}

}

LRESULT CDlgFormulaBrowser::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/)
	{
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree == NULL )  return 0;

		CExpression* pExpress = (CExpression*)lParam;
		if( pExpress == NULL ) return 0;

		pCurTree->Show(pExpress,0);
		LoadCurExp(HX_EXPRESSSENTENCE, (LPARAM)pExpress);

		return 1;
	}
	return 0;
}