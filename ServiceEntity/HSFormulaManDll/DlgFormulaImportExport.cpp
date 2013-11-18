// DlgFormulaImportExport.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFormulaImportExport.h"

#include "DlgFormulaMan.h"
#include "InportPromptDlg.h"
#include "ExpPublic.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaImportExport dialog


CDlgFormulaImportExport::CDlgFormulaImportExport(CWnd* pParent /*=NULL*/,
												 long* pThis /*= NULL*/)
	: CDialog(CDlgFormulaImportExport::IDD, pParent)
{
	m_pFormulaMan = NULL;
	m_ayExpress   = NULL;
	m_wExternalExpress = 0;
	m_pExternExpression = NULL;
	m_bInport = 1;

	//{{AFX_DATA_INIT(CDlgFormulaImportExport)
	m_bIdeaPWD = FALSE;
	m_strIdeaPWD = _T("");
	m_bAllPWD = FALSE;
	m_strWant = _T("");
	//}}AFX_DATA_INIT

	if( pThis )
	{
		/*this->SetThisParam(pThis);*/
	}

}


void CDlgFormulaImportExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormulaImportExport)
	DDX_Control(pDX, IDC_ALLPWD, m_wndAllPWD);
	DDX_Control(pDX, IDC_IDERPWDSTR, m_wndIdeaEdit);
	DDX_Control(pDX, IDC_IDEAPWD, m_wndIdeaPWD);
	DDX_Control(pDX, IDC_FINDSAME, m_wndSameBut);
	DDX_Control(pDX, IDC_COMMENT, m_wndComment);
	DDX_Control(pDX, IDC_EXPTREE, m_wndExpTree);
	DDX_Control(pDX, IDC_NAME, m_wndName);
	DDX_Control(pDX, IDC_NOTE, m_wndNote);
	DDX_Check(pDX, IDC_IDEAPWD, m_bIdeaPWD);
	DDX_Text(pDX, IDC_IDERPWDSTR, m_strIdeaPWD);
	DDX_Check(pDX, IDC_ALLPWD, m_bAllPWD);
	DDX_Text(pDX, IDC_WANT, m_strWant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFormulaImportExport, CDialog)
	//{{AFX_MSG_MAP(CDlgFormulaImportExport)
	ON_BN_CLICKED(IDC_FINDSAME, OnFindsame)
	//}}AFX_MSG_MAP
	ON_MESSAGE(HX_USER_SENDADDSTR, OnSendAddStr)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaImportExport message handlers

void CDlgFormulaImportExport::AutoSetup() 
{
	// TODO: Add extra validation here
	
	if(m_pFormulaMan != NULL)
	{
		try
		{
			if( m_bInport )
			{
				long bSameName = m_wndExpTree.OperatorTree(EXTERN_EXPRESS_AUTO_SETUP | EXTERN_EXPRESS_SAME_NAME);
				if( bSameName & EXTERN_EXPRESS_SAME_NAME )
				{
					CInportPromptDlg dlg(this);
					if( dlg.DoModal() != IDOK )
					{
						goto AutoSetup_end;
					}
					if(m_wExternalExpress & EXTERN_EXPRESS_CHANGE_NAME)
						goto AutoSetup_end;
				}
				m_wndExpTree.LoadTree(m_pFormulaMan,m_wExternalExpress);
			}
		}
		catch(...)
		{
		}
	}

AutoSetup_end:

	m_pExternExpression = DeleteVarsMap(m_pExternExpression,1);

	CDialog::OnOK();
}


void CDlgFormulaImportExport::OnOK() 
{
	// TODO: Add extra validation here
	
	if(m_pFormulaMan != NULL)
	{
		try
		{
			if( m_bInport )
			{
				long bSameName = m_wndExpTree.OperatorTree(EXTERN_EXPRESS_SAME_NAME);
				if( bSameName & EXTERN_EXPRESS_SAME_NAME )
				{
					CInportPromptDlg dlg(this);
					if( dlg.DoModal() != IDOK )
					{
						return;
					}
					if(m_wExternalExpress & EXTERN_EXPRESS_CHANGE_NAME)
						return;
				}
				
				m_wndExpTree.LoadTree(m_pFormulaMan,m_wExternalExpress);
			}
			else
			{
				m_wndExpTree.SaveLoad(CArchive::store,m_strFileName);
			}
		}
		catch(...)
		{
		}
	}
	
	m_pExternExpression = DeleteVarsMap(m_pExternExpression,1);

	CDialog::OnOK();
}

void CDlgFormulaImportExport::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	m_pExternExpression = DeleteVarsMap(m_pExternExpression,1);

	CDialog::OnCancel();
}

void CDlgFormulaImportExport::OnFindsame() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_wndExpTree.GetSelectedItem();
	if(hItem == NULL)
		return;

	CTreeGroup* pTreeGroup = (CTreeGroup*)m_wndExpTree.GetItemData(hItem);
	if(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpress = pTreeGroup->GetData();
	if( pExpress == NULL )
		return;

	UINT nState = m_wndSameBut.GetState();
	if( nState == 9 )
	{
		CValue* valoare;
		if( CExpression::m_pExternExpression->Lookup( pExpress->GetName(),valoare ) )
		{
			pExpress = valoare->GetExp();
		}
		m_wndExpTree.EnableWindow(FALSE);
	}
	else
	{
		m_wndExpTree.EnableWindow();
	}
	
	if(pExpress == NULL)
		return;

	m_wndName.SetWindowText(pExpress->GetDescribe());
	m_wndNote.SetWindowText(pExpress->GetNotes());
	m_wndComment.SetWindowText(pExpress->GetSource());
	
}

BOOL CDlgFormulaImportExport::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	try
	{
		m_wndExpTree.SetType(CTreeCtrlFormula::Export);
		COLORREF color = RGB(255,255,255);
		m_wndExpTree.SetBkColor(color);

		// TODO: Add extra initialization here
		if(m_pFormulaMan != NULL)
		{
			m_pFormulaMan->SetTree(&m_wndExpTree);
			
			if( m_bInport )
			{
				m_pExternExpression = new CMapVariabile;
				if(m_ayExpress != NULL)  // fxj
				{
					FxjExpress*	   pFxjExpress;
					for(int i = 0; i < m_ayExpress->GetSize();i++)
					{
						pFxjExpress = m_ayExpress->GetAt(i);
						m_pFormulaMan->AddExpress(m_pExternExpression,m_wExternalExpress,pFxjExpress,&m_wndExpTree);
						delete pFxjExpress;
					}
					delete m_ayExpress;
					m_ayExpress = NULL;

					m_wndExpTree.m_pInExpression = m_pExternExpression;

				}
				else  if( !m_strFileName.IsEmpty() ) // handsome
				{
					m_wndExpTree.m_pInExpression = m_pExternExpression;
					m_wndExpTree.SaveLoad(CArchive::load,m_strFileName,NULL,m_wExternalExpress);
				}
			}
			else
			{
				if( !m_strFileName.IsEmpty() )
				{
					this->SetWindowText(_T(" 输出公式 - \" ")+m_strFileName+" \"");
					m_pFormulaMan->CopyTree(&m_wndExpTree);
					m_strWant = _T("请在要输出的公式名前或公式组前打钩。");
					this->UpdateData(FALSE);
					//m_wndAllPWD.ShowWindow(SW_SHOW);
					//m_wndIdeaPWD.ShowWindow(SW_SHOW);
					//m_wndIdeaEdit.ShowWindow(SW_SHOW);
				}
			}
		}
	}
	catch(...)
	{
		OnOK();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

long CDlgFormulaImportExport::OnSendAddStr(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(lParam == NULL) 
		return 0;
	CTreeGroup* pTreeGroup = (CTreeGroup*)lParam;
	if(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem) )
		return 0;

	CExpression* pExpress = pTreeGroup->GetData();
	if( pExpress == NULL )
		return 0;

	if( m_bInport )
	{
		CValue* valoare;
		if( CExpression::m_pExternExpression->Lookup( pExpress->GetName(),valoare ) )
		{
			m_strWant = _T("系统存在同名公式,再次点击名称可以更名。");
			m_wndSameBut.ShowWindow(SW_SHOW);
		}
		else
		{
			m_strWant.Empty();
			m_wndSameBut.ShowWindow(SW_HIDE);
		}
		this->UpdateData(FALSE);
	}
	
	m_wndName.SetWindowText(pExpress->GetDescribe());
	m_wndNote.SetWindowText(pExpress->GetNotes());
	m_wndComment.SetWindowText(pExpress->GetSource());

	return 0;
}

BOOL CDlgFormulaImportExport::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			break;
		case VK_RETURN:
			{
				CWnd* pWnd = this->GetFocus();
				if( pWnd == &m_wndExpTree )
				{
					m_wndExpTree.SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
				}
			}
			return FALSE;
		case VK_F2:
			{
				//CEdit* pEdit = m_wndExpTree.EditLabel(m_wndExpTree.GetSelectedItem());
				m_wndExpTree.SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			}
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
