// DlgIndicatorSelStock.cpp : implementation file --added by HS
//

#include "stdafx.h"
//#include "HSCompMan.h"
#include "DlgIndicatorSelStock.h"

#include "DlgFormulaMan.h"
#include "DlgFormulaEdit.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"
#include "DlgCondition.h"

#include "DlgUnionConSelStock.h"
#include "DlgSetStockAndTime.h"
#include "ExpPublic.h"
#include "PubStruct.h"
#include "GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgIndicatorSelStock dialog

extern CMapStringToPtr* g_pTableHeadData;
extern CSystemFun* g_pSystemFun; 
extern HWND g_hParentWnd;

// CDlgIndicatorSelStock dialog

IMPLEMENT_DYNAMIC(CDlgIndicatorSelStock, CDialog)
CDlgIndicatorSelStock::CDlgIndicatorSelStock(CWnd* pParent /*=NULL*/,
						 HSOpenExpressUseData* pData/* = NULL*/,
						 long* pThis /*= NULL*/)
	: CDialog(CDlgIndicatorSelStock::IDD, pParent)
	, m_bCheckChuQuan(FALSE)
{
// 	SetThisParam(pThis);
// 
// 	m_cDeleteThis = 2;

	this->m_bSelStockState = false;

	m_pData = pData;
	if( pThis )
	{
		CDialog::Create(CDlgIndicatorSelStock::IDD,pParent);
		CenterWindow(CWnd::FromHandle(g_hParentWnd));
	}
}

CDlgIndicatorSelStock::~CDlgIndicatorSelStock()
{
}

void CDlgIndicatorSelStock::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);

	//	DDX_Control(pDX, IDC_SWITCHBUT, m_wndSwitchBut);
	//	DDX_Control(pDX, IDC_BOTTOMPOS, m_wndBottomPos);
	//	DDX_Control(pDX, IDC_CONDITIONPOS, m_wndConditionPos);
	DDX_Control(pDX, IDC_UNIONDLGPOS, m_wndUnionDlgPos);
	//	DDX_Control(pDX, IDD_NEWUNION, m_wndNewUnion);
	//	DDX_Control(pDX, IDD_UPDATEUNION, m_wndUpdateUnion);
	//	DDX_Control(pDX, IDD_USERNOTES, m_wndExpNotes);

	DDX_Control(pDX, IDC_STATIC_CON, m_wndStaticCon);
	DDX_Control(pDX, IDC_MIDDLE, m_wndMiddle);
	//	DDX_Control(pDX, IDD_PERIOD_STATIC, m_wndPeriodStc);
	//	DDX_Control(pDX, IDD_PERIOD, m_wndPeriod);
	//	DDX_Control(pDX, IDD_NOTES, m_wndNotes);
	DDX_Control(pDX, IDD_DATA2, m_wndData2);
	DDX_Control(pDX, IDD_DATA1, m_wndData1);
	DDX_Control(pDX, IDD_CONNOTES, m_wndConNotes);
	DDX_Control(pDX, IDD_CON, m_wndCon);
	DDX_Control(pDX, IDD_COMPARE, m_wndCompare);
	DDX_Control(pDX, IDD_AND_OR, m_wndAndOr);
	DDX_Control(pDX, IDC_TAB, m_wndTab);
	//	DDX_Control(pDX, IDC_EDITPARAM, m_wndParam);
	DDX_Control(pDX, IDC_EDITTOTALSTOCK, m_wndSelectStock);
	DDX_Control(pDX, IDC_EDITSELECTSTOCK, m_wndTotalStock);
	DDX_Control(pDX, IDD_INCLUDE, m_wndIncludeStock);
	DDX_Check(pDX, IDC_CHECK_CHUQUAN, m_bCheckChuQuan);
}


BEGIN_MESSAGE_MAP(CDlgIndicatorSelStock, CDialog)
	//{{AFX_MSG_MAP(CDlgIndicatorSelStock)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDD_ADVANCE, OnAdvance)
	ON_BN_CLICKED(IDD_USERNOTES, OnUsernotes)
//	ON_BN_CLICKED(IDD_PRESUCC, OnPresucc)
	ON_BN_CLICKED(IDD_TOBLOCK, OnToblock)
	ON_CBN_SELCHANGE(IDD_COMPARE, OnSelchangeCompare)
	ON_CBN_SELCHANGE(IDD_CON, OnSelchangeCon)
//	ON_BN_CLICKED(IDD_NEWUNION, OnNewunion)
//	ON_BN_CLICKED(IDD_UPDATEUNION, OnUpdateunion)
//	ON_BN_CLICKED(IDC_BUTHIDELEFT, OnButhideleft)
//	ON_BN_CLICKED(IDC_SAVEASBLOCK, OnSaveasblock)
	ON_BN_CLICKED(IDC_BUTEXPRESS, OnButexpress)
	ON_BN_CLICKED(IDC_BUTPARAM, OnButparam)
//	ON_BN_CLICKED(IDC_SWITCHBUT, OnSwitchbut)
	ON_BN_CLICKED(IDCLOSE, OnBnClickedClose)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)

	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)

//	ON_REGISTERED_MESSAGE(HX_USER_COMPILEDATA, OnExchangeData)
ON_BN_CLICKED(IDC_BUTFIND, OnBnClickedButfind)
END_MESSAGE_MAP()

BOOL CDlgIndicatorSelStock::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//
	Init();

	m_wndTempUnionDlg.Create(IDD_TEMPUNION,this);
	CRect rect;
	CWnd* pWnd = GetDlgItem(IDC_UNIONDLGPOS);
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_wndTempUnionDlg.MoveWindow(rect);
	m_wndTempUnionDlg.ShowWindow(SW_SHOW);

	pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	if(pWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	pWnd = this->GetDlgItem(IDC_BUTPARAM);
	if(pWnd)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
//	m_wndExpNotes.ShowWindow(SW_HIDE);
	this->m_wndTempUnionDlg.EnableBut(CDlgUnionList::AddButIndex,false);

	CString strInclude;
	CTime tTime = CTime::GetCurrentTime();
	//begin replaced by HS
	strInclude.Format(_T("不使用历史时段选股!"));
    /*
	strInclude.Format("选股时段为：所有 交易日 至 %d-%d-%d 止",
		tTime.GetYear(),tTime.GetMonth(),tTime.GetDay());
		*/
	//end
	strInclude += "\r\n";
	
	strInclude += _T("上海Ａ股,深圳Ａ股");

	m_wndIncludeStock.SetWindowText(strInclude);
	//
	/*	CRect rc;
	m_wndConditionPos.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndParam.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER);
	*/	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgIndicatorSelStock::Init(int nCopy/* = CTreeCtrlFormula::CopyDelete*/)
{
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)this->GetParent();

		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;

		CTreeCtrlFormula* pTree = pDlg->GetCurTree( (WORD)Tech );
		pCurTree->CopyTree(pTree,nCopy);

		HTREEITEM hItem = pCurTree->GetRootItem();
		if(hItem != NULL)
		{
			pCurTree->Expand(hItem,TVE_EXPAND);
		}

	}

}


/*void CDlgIndicatorSelStock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( abs(cx) + abs(cy) <= 0 )
		return;
	if( !::IsWindow(m_wndTab.m_hWnd) )
		return;

	SetSize();
}

void CDlgIndicatorSelStock::SetSize()
{
	CWnd* pWnd;
	CRect rect,rc,rc1;
	this->GetClientRect(rect);
	
	char cWidth = 5;

	pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	if(pWnd)
	{
		CString strText;
		pWnd->GetWindowText(strText);
		if( !strText.CompareNoCase(">>") )
		{
			pWnd->GetWindowRect(rc);
			ScreenToClient(rc);
			pWnd->SetWindowPos(NULL,rect.left + cWidth,rc.top,0,0,SWP_NOSIZE|SWP_NOZORDER);
			
			pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
			if(pWnd)
			{
				pWnd->GetWindowRect(rc1);
				ScreenToClient(rc1);
				pWnd->SetWindowPos(NULL,rect.right - rc1.Width() - cWidth,
					rect.bottom - cWidth - rc1.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
			}
			int nXOff = rc.Width();
			m_wndSelStockResult.GetWindowRect(rc);
			ScreenToClient(rc);
			m_wndSelStockResult.SetWindowPos(NULL,rect.left + nXOff + cWidth,rc.top,
				rect.Width() - cWidth * 2 - rect.left - nXOff,
				rect.Height() - rc.top - rc1.Height() - cWidth*2 ,SWP_NOZORDER);
			pWnd = m_wndSelStockResult.GetDlgItem(1001);
			if( pWnd )
			{
				m_wndSelStockResult.GetClientRect(rc);
				pWnd->MoveWindow(rc);
			}
			return;	
		}
	}

	// bottom
	CRect rcBottom;
	m_wndBottomPos.GetWindowRect(rcBottom);
	ScreenToClient(rcBottom);

	m_wndTab.GetClientRect(rc);
	m_wndTab.GetItemRect(0,rc1);	
	int nTabHeight = rect.Height() - rcBottom.Height() - rc1.Height() - cWidth;
	m_wndTab.SetWindowPos(NULL,0,0,rc.Width(),nTabHeight,SWP_NOMOVE|SWP_NOZORDER);

	m_wndTab.GetWindowRect(rc);	
	ScreenToClient(rc);
	m_wndBottomPos.SetWindowPos(NULL,rc.left,rc.bottom + cWidth,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	m_wndBottomPos.GetWindowRect(rc);
	ScreenToClient(rc);
	int nHeightOff = rc.top - rcBottom.top;

	m_wndNotes.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndNotes.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndConditionPos.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndConditionPos.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndConditionPos.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndParam.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER);

	m_wndPeriod.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndPeriod.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndPeriodStc.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndPeriodStc.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndConNotes.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndConNotes.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndCon.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndCon.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndStaticCon.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndStaticCon.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndCompare.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndCompare.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndData1.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndData1.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);
	
	m_wndAndOr.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndAndOr.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndData2.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndData2.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);

	m_wndMiddle.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndMiddle.SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	//
	pWnd = this->GetDlgItem(IDD_ADVANCE);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDD_PRESUCC);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDD_TOBLOCK);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDOK);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}

	//
	pWnd = this->GetDlgItem(IDD_UPDATEUNION);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDD_NEWUNION);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDD_USERNOTES);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDC_BUTPARAM);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}
	pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top + nHeightOff,0,0,SWP_NOSIZE|SWP_NOZORDER);	
	}

	// right
	pWnd = this->GetDlgItem(IDC_BUTHIDELEFT);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
	}

	pWnd = this->GetDlgItem(IDC_SAVEASBLOCK);
	if(pWnd)
	{
		pWnd->GetWindowRect(rc1);
		ScreenToClient(rc1);
		pWnd->SetWindowPos(NULL,rect.right - rc1.Width() - cWidth,rect.bottom - cWidth - rc1.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
	
	nHeightOff = rect.Width() - rc.right - cWidth;
	m_wndSelStockResult.GetWindowRect(rc);
	ScreenToClient(rc);
	m_wndSelStockResult.SetWindowPos(NULL,0,0,nHeightOff,rect.Height() - rc.top - rc1.Height() - cWidth*2,
		SWP_NOMOVE|SWP_NOZORDER);

	pWnd = m_wndSelStockResult.GetDlgItem(1001);
	if( pWnd )
	{
		m_wndSelStockResult.GetClientRect(rc);
		pWnd->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),0);
	}	
	
	this->Invalidate();
}
*/
void CDlgIndicatorSelStock::OnAdvance() 
{
	CDlgSetStockAndTime dlg(this,&m_ayCheck,&m_curSelStockCondition);
	if( dlg.DoModal() == IDOK )
	{
	}	

	CString strInclude;
	//begin:Replaced by HS 20020628
	if( !(m_curSelStockCondition.m_dStyle & SelectStock_HisDate))
	{
		strInclude.Format(_T("不使用历史时段选股!"));
	}
	else
	{
		if( m_curSelStockCondition.m_lDateCount == 0)
		{
			strInclude.Format(_T("使用历史时段选股!\r\n选股时段为：所有 交易日 至 %d-%d-%d 止"),
				m_curSelStockCondition.m_lEndDate/10000,
				m_curSelStockCondition.m_lEndDate%10000/100,m_curSelStockCondition.m_lEndDate%10000%100);
		}
		else
		{
			strInclude.Format(_T("使用历史时段选股!\r\n选股时段为：%d 个交易日 至 %d-%d-%d 止"),
				m_curSelStockCondition.m_lDateCount,m_curSelStockCondition.m_lEndDate/10000,
				m_curSelStockCondition.m_lEndDate%10000/100,m_curSelStockCondition.m_lEndDate%10000%100);
		}

	}

	/*
	if( m_curSelStockCondition.m_lDateCount == 0)
	{
		strInclude.Format("选股时段为：所有 交易日 至 %d-%d-%d 止",
			m_curSelStockCondition.m_lEndDate/10000,
			m_curSelStockCondition.m_lEndDate%10000/100,m_curSelStockCondition.m_lEndDate%10000%100);
	}
	else
	{
		strInclude.Format("选股时段为：%d 个交易日 至 %d-%d-%d 止",
			m_curSelStockCondition.m_lDateCount,m_curSelStockCondition.m_lEndDate/10000,
			m_curSelStockCondition.m_lEndDate%10000/100,m_curSelStockCondition.m_lEndDate%10000%100);
	}
	*/
	//end
	strInclude += "\r\n";

	int nCount = m_ayCheck.GetSize();
	if (nCount ==0) strInclude += _T("上海Ａ股,深圳Ａ股");

	for(int i = 0; i < nCount; i++)
	{
		StockArea* pCurData = m_ayCheck.GetAt(i);
		
	/*	if( bAdd )
		{
		//	pData = new StockArea;
		//	pData->strText.Format("%s",pCurData->strText);
		//	pData->lData = pCurData->lData;
		//	m_ayCheck->Add(pData);
			strInclude += pCurData->strText;
		}
		else
		{
		*/	strInclude += pCurData->strText;
	//	}
				
		if( i != nCount - 1 )
		{
			strInclude += ",";
		}				
	}




	m_wndIncludeStock.SetWindowText(strInclude);
}

void CDlgIndicatorSelStock::OnCancel() 
{
	// TODO: Add extra cleanup here
	//ShowWindowEx(SW_MINIMIZE);
	//ShowWindowEx(SW_HIDE);
	//WRITE_LOG ::OnCancel();
	if(this->m_bSelStockState)
	{
		return;
	}
	CDialog::OnCancel();

	//Delete();

	////this->ShowWindowEx(WM_QUIT);

	//if( ::IsWindow(m_hWnd) )
	//{
	//	DestroyWindow();
	//}
}


void CDlgIndicatorSelStock::OnUsernotes() 
{
	// TODO: Add your control notification handler code here
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)this->GetParent();
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

	CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
	if( pDlgNotes != NULL )
	{
		pDlgNotes->m_pExpression = pExpression;
		pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		pDlgNotes->ShowWindow(SW_SHOW);
	}
}

//要改动
void CDlgIndicatorSelStock::OnOK() 
{
	m_strBlock.Empty();
	Select();
}

int CDlgIndicatorSelStock::Select() 
{
	CWnd* pOKBut = this->GetDlgItem(IDOK);
	if( pOKBut != NULL )
	{
		CString strText;
		pOKBut->GetWindowText(strText);
		if( !strText.CompareNoCase(_T("终止")) )
		{
			pOKBut->SetWindowText(_T("执行"));
			::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,9,0);
			return 0;
		}		
	}
	m_curSelStockCondition.m_dStyle &= ~(SelectStock_Single | SelectStock_Union | SelectStock_Sort | SelectStock_ChuQuan);

	m_curSelStockCondition.m_dStyle |= SelectStock_Union;//by HS

	m_curSelStockCondition.m_ayCheck	= &m_ayCheck;			// 股票范围
	m_curSelStockCondition.m_strBlock	= &m_strBlock;          // 板块名
	
//	m_curSelStockCondition.m_sSingleSel = &m_sSingleSel;		// 单个选股条件
	m_pHSUnion.Copy(this->m_wndTempUnionDlg.GetUnion());
	if(CDlgUnionList::JudgeConditionEmpty(&m_pHSUnion) != 0)
	{
		m_curSelStockCondition.m_pHSUnion  = &m_pHSUnion;			// 组合选股条件
	}
	else
	{
		if(m_sSingleSel.m_nType == hx_DefMaxValues)
		{
			MessageBox(_T("请输入选股条件再执行,谢谢！"),_T("提示"),MB_ICONINFORMATION);
			return 0;
		}
		else
		{
			if(m_sSingleSel.m_nType == Union)
			{
				CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
				if( pCurTree == NULL )
					return FALSE;

				HTREEITEM hItem = pCurTree->GetSelectedItem();
				if(hItem)
				{
					UnionSelCondition* pUnion =(UnionSelCondition*)(pCurTree->GetItemData(hItem));
					if(pUnion)
					{
						m_pHSUnion.Copy(pUnion);
						m_curSelStockCondition.m_pHSUnion  = &m_pHSUnion;
					}
				}
			}
			else
			{
				m_curSelStockCondition.m_dStyle |= SelectStock_Single;
				
				
				// 周期
				m_sSingleSel.m_nPeriod;    

				// 技术指标指:哪一个指标线;交易系统指:哪种信号;基本面指:财务指标ID
				m_wndCon.GetWindowText(m_sSingleSel.m_strCondition); 

				//
				int nIndex = m_wndCon.GetCurSel();
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
					  m_sSingleSel.m_nType == MoreKLine) ) // 五彩K线
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

				m_curSelStockCondition.m_sSingleSel = &m_sSingleSel;
			}

		}

	}

	if( m_curSelStockCondition.m_lEndDate == 0 )
	{
		CString str;
		CTime timeDest = CTime::GetCurrentTime();
		str.Format("%04d%02d%02d",timeDest.GetYear(),timeDest.GetMonth(),timeDest.GetDay());
		m_curSelStockCondition.m_lEndDate = atol(str);   
	}
	m_curSelStockCondition.m_pData = NULL;
	
	this->m_wndSelectStock.SetWindowText("0");
	m_wndTotalStock.SetWindowText(""); 
	this->UpdateData();
	if(m_bCheckChuQuan)
	{
		m_curSelStockCondition.m_dStyle |= SelectStock_ChuQuan;
	}	
	// 执行
	long lRet = DoSelectStock(&m_curSelStockCondition, HS_SELECTSTOCK_ZhiBiao);
	CDialog::OnOK();

	return 1;
}

int CDlgIndicatorSelStock::DoSelectStock( SelStockCondition* pSelStockCondition ,int style)
{
	if( pSelStockCondition == NULL )
	{
		return 0;
	}
// 	CArray<StockArea*,StockArea*>* ayCheck = pSelStockCondition->m_ayCheck;
// 	if( ayCheck == NULL )
// 		return 0;

	CFormulaCalc::m_pMsgWnd = (CWnd*)this;
	CArray<StockUserInfo*,StockUserInfo*>* payStock = &(StockArea::ayStock);

	if( payStock->GetSize() <= 0 )
	{
		MessageBox(_T("股票范围里没有股票列表，不能选股！"),"提示",MB_OK);
		return 0;
	}

	CFormulaCalc::m_curSelStockCondition  = pSelStockCondition;					// 条件
	CFormulaCalc::m_pExternExpression	  = CExpression::m_pExternExpression;		// 外部表达式表
	CFormulaCalc::m_pExternVariabile	  = CExpression::m_pExternVariabile;		// 外部变量表
	CFormulaCalc::m_pExternFunction		  = CExpression::m_pExternFunction;		    // 外部函数表
	CFormulaCalc::m_pSystemFun			  = g_pSystemFun;				// 函数指针

	// 启动
	int nRet = CFormulaCalc::Start();
	if( nRet != 1 )
	{
		return 0;
	}

	// 选股,由选股本身的进度条
	if( style != HS_SELECTSTOCK_ANALYSIS )
	{
	}
	return 1;

}
void CDlgIndicatorSelStock::OnToblock() 
{
	// TODO: Add your control notification handler code here
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,1,(LPARAM)this);
}

long CDlgIndicatorSelStock::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{	
	if( wParam == TEMPUNION_NEWCONDITION || wParam== TEMPUNION_SAVEUNION
		||wParam== TEMPUNION_DELUNION)
	{
		CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
		if( pCurTree == NULL )
			return FALSE;

	

		if( wParam == TEMPUNION_NEWCONDITION)
		{
			CDlgUnionList* pTempUnion = (CDlgUnionList*)lParam;
			if ( pTempUnion == NULL) return 0;

			m_wndCon.GetWindowText(m_sSingleSel.m_strCondition); 
			if(!m_sSingleSel.m_strCondition.Compare(CTreeCtrlFormula::g_strFirstTechLine))
			{
				m_sSingleSel.m_strCondition = "";
			}
			m_sSingleSel.m_SubData.m_cType = m_wndCompare.GetCurSel();
			m_wndData1.GetWindowText(m_sSingleSel.m_SubData.m_dData1);
			m_wndData2.GetWindowText(m_sSingleSel.m_SubData.m_dData2);

			pTempUnion->SetSingleUnion(&m_sSingleSel);
			return 0;
		}

			HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
		if( hRootItem == NULL )
			return FALSE;


		else if( wParam == TEMPUNION_SAVEUNION)
		{
			CDlgUnionList* pTempUnion = (CDlgUnionList*)lParam;
			if ( pTempUnion == NULL) return 0;

			CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();

			HTREEITEM hItem = pCurTree->InsertItem( pTempUnion->m_pHSUnion->m_strName,
				CExpression::HS_Exp_Union, CExpression::HS_Exp_Union,hRootItem);
			pCurTree->SetItemData(hItem,(DWORD)pTempUnion->m_pHSUnion);
			return 0;
		}
		else if( wParam == TEMPUNION_DELUNION)
		{
			UnionSelCondition* pHSUnion = (UnionSelCondition*)lParam;

			if( hRootItem != NULL )
			{
				HTREEITEM hItem = pCurTree->GetChildItem( hRootItem );

				while( hItem != NULL )
				{
					UnionSelCondition* pCompareUnion =(UnionSelCondition*)(pCurTree->GetItemData(hItem));
					if(pCompareUnion)
					{
						if(pHSUnion == pCompareUnion)
						{
							pCurTree->DeleteItem(hItem);
							return 1;
						}
					}

					hItem = pCurTree->GetNextSiblingItem( hItem );
				}
			}
			return 0;
		}
		
	}
	m_sSingleSel.m_nType = hx_DefMaxValues;

	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)this->GetParent();
	if( pParentDlg == NULL )
		return 0;

	if( wParam == HX_UNION_EXPRESS )
	{
//		EnableItem(FALSE);
		CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
		if(pWnd)	
		{
			pWnd->ShowWindow(SW_HIDE);
		}
		pWnd = this->GetDlgItem(IDC_BUTPARAM);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
	//	m_wndExpNotes.ShowWindow(SW_HIDE);
		this->m_wndTempUnionDlg.EnableBut(CDlgUnionList::AddButIndex,false);


		//		m_wndNewUnion.ShowWindow(SW_SHOW);
//		m_wndUpdateUnion.ShowWindow(SW_SHOW);

		if( lParam )
		{
			m_sSingleSel.m_nType = Union;

			UnionSelCondition* pHSUnion = (UnionSelCondition*)lParam;
			
/*			m_wndParam.SetWindowText(GetDescribe(pHSUnion));
			m_wndParam.ShowWindow(SW_SHOW);
			
			m_wndNotes.ShowWindow(SW_SHOW);
			m_wndNotes.SetWindowText("选出满足下列组合条件的股票:");
*/		}
	}
	else if( wParam == CTreeCtrlFormula::HSBrowser )
	{
//		EnableItem(FALSE);
			CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
		pWnd = this->GetDlgItem(IDC_BUTPARAM);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_HIDE);
		}
	//	m_wndExpNotes.ShowWindow(SW_HIDE);
		this->m_wndTempUnionDlg.EnableBut(CDlgUnionList::AddButIndex,false);



	/*	CString strItem;
		if( lParam )
		{
			strItem = *(CString*)lParam;
		}
	*/
		CTreeGroup* pGroup = (CTreeGroup*)lParam;
		if (pGroup == NULL) return 0;
		if( pGroup->m_dwID == Union /*!strItem.CompareNoCase(CTreeCtrlFormula::g_strUnionCondition)*/ )
		{
			m_sSingleSel.m_nType = Union;
			m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strUnionCondition);

/*			m_wndNotes.ShowWindow(SW_HIDE);
			m_wndNotes.SetWindowText("选出满足下列组合条件的股票:");
			
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

			m_wndNewUnion.ShowWindow(SW_SHOW);
			//m_wndUpdateUnion.ShowWindow(SW_SHOW);
*/
		}
/*		else if( !strItem.CompareNoCase(CTreeCtrlFormula::g_strJiben) )
		{
			m_sSingleSel.m_nType = JiBenMian;
			m_sSingleSel.m_strExp.Format("%s",CTreeCtrlFormula::g_strJiben);

			m_wndNotes.ShowWindow(SW_SHOW);
			m_wndNotes.SetWindowText("选出财务指标满足条件的股票:");
			
			m_wndPeriod.ShowWindow(SW_HIDE);
			m_wndPeriod.SetCurSel(6);
			m_wndPeriodStc.ShowWindow(SW_HIDE);
			
			m_wndConNotes.ShowWindow(SW_SHOW);
			m_wndConNotes.SetWindowText("财务指标");
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
			InitCWData(&m_wndCon,GB_GUBEN_NAME);
			InitCWData(&m_wndCon,Cj_GUBEN_NAME);

			InitCondition(&m_wndCompare,4);
		}
	}
*/
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

				CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
				if( pCurTree != NULL )
				{
					HTREEITEM hItem = pCurTree->GetSelectedItem( );
					if( hItem != NULL )
					{
						CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hItem);
						if( pTreeGroup != NULL &&
							pTreeGroup->IsStatus(hx_GroupItem) )
						{
							SendMessage(HX_USER_LOADCUR,HX_EXPRESSSENTENCE,(LPARAM)pTreeGroup->GetData());
						}
					}
				}
			}
		}
	}
	
	else if( wParam == HX_EXPRESSSENTENCE )
	{
		CExpression* pExpression = (CExpression*)lParam;
		if( pExpression == NULL )
			return 0;
		
		//
		m_sSingleSel.m_nType = pExpression->GetExpressType();
		m_sSingleSel.m_strExp.Format("%s",pExpression->GetName());
		InitCondition(&m_wndCompare,-1);
		
//		m_wndNewUnion.ShowWindow(SW_HIDE);
//		m_wndUpdateUnion.ShowWindow(SW_HIDE);

		switch ( m_sSingleSel.m_nType )
		{
		case Tech:
			{
		/*		m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText("选出技术指标数值满足下列条件的股票:");
				
				m_wndPeriod.ShowWindow(SW_SHOW);
				m_wndPeriod.SetCurSel(6);
				m_wndPeriodStc.ShowWindow(SW_SHOW);
		*/		
				m_wndConNotes.ShowWindow(SW_SHOW);
				m_wndConNotes.SetWindowText(_T("指标线"));
				
				m_wndCon.ShowWindow(SW_SHOW);
				m_wndCon.ResetContent();
				m_wndStaticCon.ShowWindow(SW_SHOW);
								
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
								{
									//yulx modify 去掉后三位的0
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
						m_wndCompare.ShowWindow(SW_SHOW);
						m_wndData1.ShowWindow(SW_SHOW);

						OnSelchangeCompare();
					/*	if( IsType(Condition) )
						{
							m_wndCompare.ShowWindow(SW_SHOW);
							m_wndData1.ShowWindow(SW_SHOW);
							OnSelchangeCompare();
						}
						else
						{
							m_wndStaticCon.ShowWindow(SW_HIDE);
							m_wndCompare.ShowWindow(SW_HIDE);
							m_wndData1.ShowWindow(SW_HIDE);
							m_wndAndOr.ShowWindow( SW_HIDE);
							m_wndData2.ShowWindow( SW_HIDE);
							m_wndMiddle.ShowWindow( SW_HIDE);

						}*/
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
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Condition;
				this->m_wndTempUnionDlg.EnableBut(CDlgUnionList::AddButIndex,true);
		/*		m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText("选出满足下列条件的股票:");
				
				m_wndPeriod.ShowWindow(SW_SHOW);
				m_wndPeriod.SetCurSel(6);
				m_wndPeriodStc.ShowWindow(SW_SHOW);
				
				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndCon.ShowWindow(SW_HIDE);
				m_wndStaticCon.ShowWindow(SW_HIDE);
				
				m_wndCompare.ShowWindow(SW_HIDE);
				m_wndData1.ShowWindow(SW_HIDE);
				
				m_wndAndOr.ShowWindow(SW_HIDE);
				m_wndData2.ShowWindow(SW_HIDE);
				m_wndMiddle.ShowWindow(SW_HIDE);
		*/	}
			break;
/*		case Exchange:
			{
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::Exchange;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText("选出交易系统发出以下信号的股票:");
				
				m_wndPeriod.ShowWindow(SW_SHOW);
				m_wndPeriod.SetCurSel(6);
				m_wndPeriodStc.ShowWindow(SW_SHOW);
				
				m_wndConNotes.ShowWindow(SW_HIDE);
				m_wndConNotes.SetWindowText("发出");
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
				m_sSingleSel.m_SubData.m_cType = FormulaCompareSubData::MoreKLine;

				m_wndNotes.ShowWindow(SW_SHOW);
				m_wndNotes.SetWindowText("选出五彩Ｋ线发出指示的股票:");
				
				m_wndPeriod.ShowWindow(SW_SHOW);
				m_wndPeriod.SetCurSel(6);
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
	*/		break;
		}
		
		// 注释/参数修改
	//	m_wndExpNotes.ShowWindow(SW_SHOW);
		CWnd* pWnd = this->GetDlgItem(IDC_BUTEXPRESS);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_SHOW);
		}
		pWnd = this->GetDlgItem(IDC_BUTPARAM);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_SHOW);
		}
		
		CDlgNotesAndEidolon* pDlgNotes = pParentDlg->GetDlgNotes();
		if( pDlgNotes != NULL && 
			pDlgNotes->IsWindowVisible() )
		{
			pDlgNotes->SetNotes(pExpression->GetNotes(), pExpression->GetName());
		}	

/*		pWnd = this->GetDlgItem(IDC_BUTPARAM);
		if(pWnd)
		{
			pWnd->ShowWindow(SW_SHOW);
		}
	*/
	//	this->m_wndTempUnionDlg.SetSingleUnion(&m_sSingleSel);
		this->m_wndTempUnionDlg.EnableBut(CDlgUnionList::AddButIndex,true);
}

	
	return 0;
}


BOOL CDlgIndicatorSelStock::ShowWindow( int nCmdShow )
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
CString CDlgIndicatorSelStock::GetNameByKey(CString& strFindKeyName) 
{
	CString strRet;
	if( g_pTableHeadData )
	{
		void* pValue;
		CMapWordToPtr* pMap;

		CString strName = GB_GUBEN_NAME;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				if( pMap->Lookup(atol(strFindKeyName),pValue))
				{
					strRet.Format("%s",((TableColProperty*)pValue)->strName );
				}
			}
		}

		strName = Cj_GUBEN_NAME;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				if( pMap->Lookup(atol(strFindKeyName),pValue) )
				{
					strRet.Format("%s",((TableColProperty*)pValue)->strName);
				}
			}
		}
	}
	return strRet;
}


BOOL CDlgIndicatorSelStock::SaveLoad(UINT nMode,CString strFileName /*= ""*/,DWORD dStyle /*= 0*/)
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
			
			CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
			if( pCurTree == NULL )
				return FALSE;
			
			HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
			if( hRootItem == NULL )
				return FALSE;

			if(head.m_lCount < 0)
			{
				return FALSE;
			}
			
			char cStatus;
			HTREEITEM hItem;
			UnionSelCondition* pHSUnion;
			for(int i = 0; i < head.m_lCount; i++)
			{
				ar >> cStatus;
				if( cStatus )
				{
					pHSUnion = new UnionSelCondition;
					pHSUnion->Serialize(ar);

					hItem = pCurTree->InsertItem( pHSUnion->m_strName,
						CExpression::HS_Exp_Union, CExpression::HS_Exp_Union,hRootItem);
					pCurTree->SetItemData(hItem,(DWORD)pHSUnion);
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

void CDlgIndicatorSelStock::Serialize(CArchive& ar,int& nCountRef)
{
	nCountRef = 0;

	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree == NULL )
		return;

	HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
	if( hRootItem == NULL )
		return;

	UnionSelCondition* pHSUnion;
	char cStatus;
	HTREEITEM hChildItem = pCurTree->GetChildItem( hRootItem );
	while(hChildItem != NULL)
	{
		pHSUnion = (UnionSelCondition*)pCurTree->GetItemData( hChildItem );
		
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
		hChildItem = pCurTree->GetNextSiblingItem( hChildItem );
	}
}

void CDlgIndicatorSelStock::OnButexpress() 
{
	// TODO: Add your control notification handler code here
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		pCurTree->SendMessage(WM_COMMAND,IDM_EDITEXPRESS);
	}
}

void CDlgIndicatorSelStock::OnButparam() 
{
	// TODO: Add your control notification handler code here
	CDlgFormulaMan* pParentDlg  = (CDlgFormulaMan*)this->GetParent();
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

	CDlgNotesAndEidolon* pDlgParam = pParentDlg->GetDlgParam();
	if( pDlgParam != NULL )
	{
		pDlgParam->m_pExpression = pExpression;
		CString strText;
		strText.Format("%s",pExpression->GetEidolon());
		if( !strText.IsEmpty() )
		{
			pDlgParam->SetEidolon(strText,pExpression->GetParamVar(),pExpression->GetName());
		}
		else
		{
			CStringArray* pArray = pExpression->GetDefEidolon();
			if(pArray->GetSize() <= 1)
			{
				pArray = NULL;
			}
			pDlgParam->SetEidolon(pArray,pExpression->GetParamVar(),pExpression->GetName());
		}					
		pDlgParam->Invalidate();
		pDlgParam->ShowWindow(SW_SHOW);
	}
}

void CDlgIndicatorSelStock::MyNcDestroy()
{
	::SendMessage(g_hParentWnd,HX_USER_COMPILEDATA,20,0);
}

void CDlgIndicatorSelStock::Delete()
{
	SaveLoad(CArchive::store);

	//
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree != NULL )
	{
		HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
		CTreeGroup* pTreeGroup = (CTreeGroup*)pCurTree->GetItemData(hRootItem);
		if(pTreeGroup == NULL)
		{
			delete pTreeGroup;
		}

		if( hRootItem != NULL )
		{
			HTREEITEM hChildItem = pCurTree->GetChildItem( hRootItem );
			while(hChildItem != NULL)
			{
				UnionSelCondition* pHSUnion = (UnionSelCondition*)pCurTree->GetItemData( hChildItem );
				if( pHSUnion )
				{
					delete pHSUnion;
				}
				hChildItem = pCurTree->GetNextSiblingItem( hChildItem );
			}
		}
	}
	//
	for(int i = 0; i < m_ayCheck.GetSize(); i++)
	{
		delete m_ayCheck.GetAt(i);
	}
	m_ayCheck.RemoveAll();
}
void CDlgIndicatorSelStock::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
}


LRESULT CDlgIndicatorSelStock::OnExchangeData(WPARAM wp,LPARAM lp)
{
	if( wp == 1 ) // 股票列表
	{		
	}
	else if (wp == 2) // 板块名称
	{
		m_strBlock.Format("%s",*(CString*)lp);
		Select();
	}
	else if (wp == 3) // 返回当前选中股票
	{
		return (long)&m_ayCheck;
	}
	else if( wp == 4 ) // 选股结束
	{
		CWnd* pOKBut = this->GetDlgItem(IDOK);
		if( pOKBut != NULL )
		{
			pOKBut->SetWindowText(_T("执行"));
		}
		/*CString strSelectStockNum;
		m_wndSelectStock.GetWindowText(strSelectStockNum);
		if( strSelectStockNum == "0")
		{
			MessageBox("很遗憾，没找到您想要的股票！","提示");
		}*/
//		EnableBut(TRUE);
		this->m_bSelStockState = false;
	}

	else if( wp == 5 ) // 返回当前选中股票数
	{
		CNI_ReceiveData* pInfo2 = (CNI_ReceiveData*)lp;
		
		CString strText;
		strText.Format("%d",pInfo2->m_nStatus);
		m_wndSelectStock.SetWindowText(strText);

		m_wndTotalStock.GetWindowText(strText);
		if( strText.IsEmpty() )
		{
			strText.Format("%d",pInfo2->m_lDataTotal);
			m_wndTotalStock.SetWindowText(strText);
		}
	}
	else if( wp == 6 ) // 返回所有被查找的股票数
	{
		CString strText;
		strText.Format("%d",lp);
		this->m_wndTotalStock.SetWindowText(strText);
	
	}

	return 0;
}

void CDlgIndicatorSelStock::InitCondition(CComboBox* pBox,int nType)
{
	pBox->ResetContent();
	int nCount;
	char** pData = CExpValue::GetConditionStr(nCount);
	if( nType == -1 )
		nType = nCount;
	for(int i = 0; i < nType; i++)
	{
		pBox->AddString(pData[i]);
	}
	pBox->SetCurSel(0);
}

void CDlgIndicatorSelStock::OnSelchangeCompare() 
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
			m_wndAndOr.SetWindowText("与");
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

	m_wndData1.SetWindowText("0");
	m_wndData2.SetWindowText("0");

	if( m_sSingleSel.m_nType >= 0 )
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
			m_wndData1.SetCurSel(0);
			m_wndData2.SetCurSel(0);
		}
	}
}

void CDlgIndicatorSelStock::OnSelchangeCon() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeCompare();
}


void CDlgIndicatorSelStock::OnBnClickedButfind()
{
	// TODO: Add your control notification handler code here
	DWORD dwMask = 0;
	dwMask = Tech;
	
	CDlgFormulaMan* pWnd = (CDlgFormulaMan*)this->GetParent();
	if(pWnd)
	{
		pWnd->Find(dwMask,this);
	}
}

LRESULT CDlgIndicatorSelStock::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HX_USER_DRAGDATA-1)
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

void CDlgIndicatorSelStock::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nCopy = CTreeCtrlFormula::CopyDelete;

	int nIndex = m_wndTab.GetCurSel( );
	switch(nIndex)
	{
	case 0:
		break;
	case 1:
		nCopy |= CTreeCtrlFormula::CopyNotGroup;
		break;
	case 2:
		nCopy |= CTreeCtrlFormula::CopyNotGroup | HS_EXP_DEFAULT_TECH;
		break;
	case 3:
		nCopy |= CTreeCtrlFormula::CopyNotGroup | HS_EXP_USEREDIT;
		break;
	default:
		break;
	}
	this->Init(nCopy);

	*pResult = 0;
}
