///////////////////////////////////////////////////////////////////////////////////////////
// 文件名:			DlgAddProjectToBlock.cpp
// 创建者：				
// 创建时间：		2002-3-6
// 内容描述：		"板块分析"功能模块--"项目管理"
//////////////////////////////////////////////////////////////////////////////////////////
// DlgAddProjectToBlock.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAddProjectToBlock.h"
#include "DlgFormulaEdit.h"
#include "DlgFormulaMan.h"

#include "PubStruct.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddProjectToBlock dialog

// HS replace
//CDlgFormulaEdit* CDlgAddProjectToBlock::m_pEdiUpdatetExp    = NULL;	 // 公式

extern HWND g_hParentWnd;

CDlgAddProjectToBlock::CDlgAddProjectToBlock(CWnd* pParent /*=NULL*/, 
									 int iFormat, 
									 int iType, 
									 LPCSTR strColName, 
									 LPCSTR strDesc, 
									 LPCSTR strWinText)
	: CDialog(CDlgAddProjectToBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddProjectToBlock)
	m_name = _T("");
	m_desc = _T("");
	//}}AFX_DATA_INIT

	m_Form = (CDlgFormulaMan*)AfxGetMainWnd();//pParent; // HS replace
	
	m_iCurFormat = iFormat;
	m_iCurType	 = iType;
	
	m_strCurName = strColName;
	m_strCurDesc = strDesc;
	m_strWinText = strWinText;
}


void CDlgAddProjectToBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddProjectToBlock)
	DDX_Control(pDX, IDC_COM_FORMAT, m_comboFormat);
	DDX_Control(pDX, IDC_COM_TOTAL, m_conboTotal);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_DESC, m_desc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddProjectToBlock, CDialog)
	//{{AFX_MSG_MAP(CDlgAddProjectToBlock)
	ON_BN_CLICKED(ID_EXIT, OnExit)
	ON_BN_CLICKED(IDC_SELECT_EXPRESS, OnSelectExpress)
	ON_CBN_SELCHANGE(IDC_COM_FORMAT, OnSelchangeComFormat)
	ON_CBN_SELCHANGE(IDC_COM_TOTAL, OnSelchangeComTotal)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_NEW_EXPRESS, OnNewExpress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddProjectToBlock message handlers


BOOL CDlgAddProjectToBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_comboFormat.SetCurSel(m_iCurFormat);
	m_conboTotal.SetCurSel(m_iCurType);
	m_name = m_strCurName;
	m_desc = m_strCurDesc;

	if(m_strWinText.GetLength() > 0)
		SetWindowText(m_strWinText);

	Init();
	CWnd *pButton = this->GetDlgItem(IDOK);
	pButton->EnableWindow(FALSE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAddProjectToBlock::OnOK() 
{
	UpdateData();

	m_iCurFormat = m_comboFormat.GetCurSel();
	m_iCurType = m_conboTotal.GetCurSel();
	m_strCurName = m_name;
	m_strCurDesc = m_desc;

	if(m_strCurName.IsEmpty())
	{
		//AfxMessageBox(_T("名称不能为空"));
		MessageBox(_T("名称不能为空"),_T("提示"),MB_ICONINFORMATION);
		return;
	}
	CDialog::OnOK();
}

void CDlgAddProjectToBlock::OnExit() 
{
	CDialog::OnCancel();
}

void CDlgAddProjectToBlock::OnSelectExpress() 
{
	if( m_Form == NULL )
		return;

	HSOpenExpressUseData data(hxShowReturnByExpressType|Tech);
	data.m_strName = new CString(_T("选择指标!"));
	((CDlgFormulaMan*)m_Form)->Open(&data);

	if( data.m_pData != NULL )
	{
		CExpression* pExpression = (CExpression*)data.m_pData;
		m_desc = pExpression->GetName();
		UpdateData(FALSE);
		if(m_name.IsEmpty())
			return;
		CWnd *pButton = this->GetDlgItem(IDOK);
		if( !pButton->IsWindowEnabled() && !m_name.IsEmpty() )
			pButton->EnableWindow(TRUE);
	}
}

void CDlgAddProjectToBlock::OnSelchangeComFormat() 
{
	UpdateData();
	if( m_name.IsEmpty() || m_desc.IsEmpty() )
		return;
	CWnd *pButton = this->GetDlgItem(IDOK);
	if( !pButton->IsWindowEnabled() )
		pButton->EnableWindow(TRUE);
}

void CDlgAddProjectToBlock::OnSelchangeComTotal() 
{
	UpdateData();
	if( m_name.IsEmpty() || m_desc.IsEmpty() )
		return;
	CWnd *pButton = this->GetDlgItem(IDOK);
	if( !pButton->IsWindowEnabled() )
		pButton->EnableWindow(TRUE);
}


void CDlgAddProjectToBlock::OnChangeName() 
{
	CWnd *pButton = this->GetDlgItem(IDOK);

	UpdateData();
	if( !pButton->IsWindowEnabled() && !m_desc.IsEmpty() && !m_name.IsEmpty() )
		pButton->EnableWindow(TRUE);	
}


void CDlgAddProjectToBlock::Init()
{
	CTreeCtrlFormula* pCurTree = m_wndTab.GetTree();
	if( pCurTree )
	{
		CDlgFormulaMan* pDlg = (CDlgFormulaMan*)m_Form;
		
		pCurTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
		pCurTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		pCurTree->m_pFormulaMan = pDlg;
		
		pDlg->CopyTree(pCurTree);
		
		HTREEITEM hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strUnionCondition,
			CExpression::HS_Exp_Root, CExpression::HS_Exp_Root);
		hRootItem = pCurTree->InsertItem(CTreeCtrlFormula::g_strJiben,
			CExpression::HS_Exp_Condition, CExpression::HS_Exp_Condition);
	}
}

void CDlgAddProjectToBlock::OnDestroy() 
{
	CDialog::OnDestroy();
	
/*	if( m_pEdiUpdatetExp != NULL )
	{
		m_pEdiUpdatetExp->DestroyWindow();
		delete m_pEdiUpdatetExp;
		m_pEdiUpdatetExp = NULL;
	}	
	*/
}

void CDlgAddProjectToBlock::OnNewExpress() 
{
	/* by HS replace 2002.04.12
	CDlgFormulaEdit* pDlg = GetExpDlg();
	if( pDlg != NULL )
	{
		pDlg->OnNewExpress();
		pDlg->SetTitle(Tech,"新建");
		pDlg->ShowWindowEx( SW_SHOWNORMAL );
		if( pDlg->m_bOkFlag == TRUE )
		{
			pDlg->GetDlgItemText(IDC_NAMEEDIT, m_desc);
			if( !m_name.IsEmpty())
			{
				CWnd *pButton = this->GetDlgItem(IDOK);
				if( !pButton->IsWindowEnabled() && !m_desc.IsEmpty())
					pButton->EnableWindow(TRUE);	
			}
			UpdateData(FALSE);
		}
	}
	*/

	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		CDlgFormulaEdit* pDlg = pMain->NewExpress(Tech);
		if( pDlg != NULL )
		{
			pDlg->ShowWindow( SW_SHOWNORMAL );
			if( pDlg->m_bOkFlag == TRUE )
			{
				pDlg->GetDlgItemText(IDC_NAMEEDIT, m_desc);
				if( !m_name.IsEmpty())
				{
					CWnd *pButton = this->GetDlgItem(IDOK);
					if( !pButton->IsWindowEnabled() && !m_desc.IsEmpty() && !m_name.IsEmpty() )
						pButton->EnableWindow(TRUE);	
				}
				UpdateData(FALSE);
			}
		}
	}

}

/* // HS replace
CDlgFormulaEdit* CDlgAddProjectToBlock::GetExpDlg()
{
	if( m_pEdiUpdatetExp == NULL )
	{
		m_pEdiUpdatetExp = new CDlgFormulaEdit(m_Form);		
	}
	m_pEdiUpdatetExp->SetType(CDlgFormulaEdit::none);
	return m_pEdiUpdatetExp;
}
*/