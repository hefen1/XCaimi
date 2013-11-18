// DlgTechExpress.cpp : implementation file
//

#include "stdafx.h"
#include "DlgNotesAndEidolon.h"
#include "CompileEditView.h"

#include "DlgFormulaMan.h"
#include "DlgFormulaEdit.h"
#include "HSMessage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNotesAndEidolon property page

//IMPLEMENT_DYNCREATE(CDlgNotesAndEidolon, CHSBaseDialog)

extern HWND  g_hParentWnd;
extern CDlgFormulaMan* g_pFormulaMan;

CDlgNotesAndEidolon::CDlgNotesAndEidolon(CWnd* pParent /*= NULL*/) :
	CHSBaseDialog(CDlgNotesAndEidolon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNotesAndEidolon)
	//}}AFX_DATA_INIT
	m_pExpression = NULL;	
}

CDlgNotesAndEidolon::~CDlgNotesAndEidolon()
{
}

void CDlgNotesAndEidolon::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNotesAndEidolon)
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_pEditCtrl);
//	DDX_Control(pDX, IDC_EDIT_EIDOLON1, m_pEidolonCtrl);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNotesAndEidolon, CHSBaseDialog)
	//{{AFX_MSG_MAP(CDlgNotesAndEidolon)
	ON_WM_SIZE()
	ON_WM_DESTROY()	
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(HX_USER_UPDATEDATA, OnCompileUpdateData)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgNotesAndEidolon::OnDeltaposSpin1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNotesAndEidolon message handlers

void CDlgNotesAndEidolon::OnCancel() 
{
	m_EditEidolon.ResetParam();
	if(IsType(Eidolon))
	{
		m_EditEidolon.Empty();
	}
	else if(IsType(Notes))
	{
		this->UpdateData(TRUE);
		if(m_pExpression != NULL)
		{
			/*
			CString strText;
			m_pEditCtrl.GetWindowText(strText);
			CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
			if( pMainWnd != NULL && pMainWnd->m_pCompileApi != NULL )
			{
				HSOpenExpressUseData data(hxOpenExpress_Notes);
				data.m_strName = &strText;
				data.m_pData   = m_pExpression;
				pMainWnd->m_pCompileApi->OpenExpress(0,(LPARAM)&data);
			}
			*/
		}
	}

	m_pExpression = NULL;
	//CHSBaseDialog::OnCancel();
	this->ShowWindow(SW_HIDE);
}

void CDlgNotesAndEidolon::OnOK() 
{
	if( m_pExpression != NULL)
	{
		ExpPropery NameProp;
		NameProp.m_dExpType = m_pExpression->GetExpressType();
		NameProp.m_strName.Format("%s", m_pExpression->GetName());
		NameProp.m_strDescribe.Format("%s",  m_pExpression->GetDescribe());
		NameProp.m_bStyle = m_pExpression->GetStyle();		
		if ( g_pFormulaMan )
			g_pFormulaMan->SendLinkMsg(DiagramMsg_FormulaParamChanged,(LPARAM)&NameProp);
	}

	if(IsType(Eidolon))
	{
		m_EditEidolon.Empty();
	}
	else if(IsType(Notes))
	{
		this->UpdateData(TRUE);
		if(m_pExpression != NULL)
		{
			CString strText;
			m_pEditCtrl.GetWindowText(strText);
			m_pExpression->SetNotes(strText);
		}
	}
	m_pExpression = NULL;
	//CHSBaseDialog::OnOK();
	this->ShowWindow(SW_HIDE);

	if( g_pFormulaMan )
		g_pFormulaMan->Save();
}

BOOL CDlgNotesAndEidolon::Create(CWnd* pParent)
{
	return CHSBaseDialog::Create(CDlgNotesAndEidolon::IDD,pParent);
}

BOOL CDlgNotesAndEidolon::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();

	m_pEidolonCtrl.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN,CRect(0,0,0,0),this,IDC_EDIT_EIDOLON);
	m_pEidolonCtrl.ModifyStyleEx(0,WS_EX_CLIENTEDGE,0);
	m_pEidolonCtrl.SetFont(m_pEditCtrl.GetFont());

	this->CenterWindow();

	m_EditEidolon.CreateEx(WS_EX_CLIENTEDGE,NULL,NULL,WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,
		CRect(0,0,0,0),this,1001);
	m_EditEidolon.SetFont( GetFont() );

 	CRect rect;
	GetClientRect(&rect);
	CWnd* pWnd = this->GetDlgItem(IDOK);
	if(pWnd != NULL)
	{
		CRect rc;
		pWnd->GetWindowRect(rc);
		rect.bottom -= (rc.Height() + 4);
		pWnd->MoveWindow(rect.right - rc.Width() - 5,rect.bottom + 4,rc.Width(),rc.Height());
	}
	if(IsType(EidolonEdit))
	{
		m_pEditCtrl.ShowWindow(SW_HIDE);
		m_EditEidolon.ShowWindow(SW_HIDE);
		m_pEidolonCtrl.MoveWindow(CRect(0,0,rect.Width(),rect.Height()));
	}
	else if(IsType(Notes))
	{
		m_pEidolonCtrl.ShowWindow(SW_HIDE);
		m_EditEidolon.ShowWindow(SW_HIDE);
		m_pEditCtrl.MoveWindow(CRect(0,0,rect.Width(),rect.Height()));
	}
	else if(IsType(Eidolon))
	{
		m_pEidolonCtrl.ShowWindow(SW_HIDE);
		m_pEditCtrl.ShowWindow(SW_HIDE);
		m_EditEidolon.MoveWindow(CRect(0,0,rect.Width(),rect.Height()));
	}

	SetSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgNotesAndEidolon::SetNotes(CString strNotes,CString strCaption)
{
	strCaption += _T("指标用法");
	Show(strCaption,&m_pEditCtrl);
	m_pEditCtrl.SetWindowText(strNotes);
}

void CDlgNotesAndEidolon::SetEidolonEdit(CString strNotes,CString strCaption)
{
	strCaption += _T("参数精灵");
	Show(strCaption,&m_pEidolonCtrl);
	m_pEidolonCtrl.SetWindowText(strNotes);
}

void CDlgNotesAndEidolon::SetEidolon(CString strText,CMapVariabile* pParamVar,CString strCaption)
{
	strCaption += _T("参数修改");
	Show(strCaption,&m_EditEidolon);
	m_EditEidolon.SetText(strText,pParamVar);
}

void CDlgNotesAndEidolon::SetEidolon(CStringArray* pArray,CMapVariabile* pParamVar,CString strCaption)
{
	strCaption += _T("参数修改");
	Show(strCaption,&m_EditEidolon);
	m_EditEidolon.SetText(pArray,pParamVar);
}

void CDlgNotesAndEidolon::Show(CString strCaption,CWnd* pWnd)
{
	SetWindowText(strCaption);
	
	if(pWnd != &m_pEidolonCtrl)
	{
		m_pEidolonCtrl.ShowWindow(SW_HIDE);
	}
	if(pWnd != &m_pEditCtrl)
	{
		m_pEditCtrl.ShowWindow(SW_HIDE);
	}
	if(pWnd != &m_EditEidolon)
	{
		m_EditEidolon.ShowWindow(SW_HIDE);
	}
	if( pWnd )
	{
		if( !pWnd->IsWindowVisible() )
		{
			pWnd->ShowWindow(SW_SHOW);
		}
	}

	SetSize();
}

void CDlgNotesAndEidolon::OnSize(UINT nType, int cx, int cy) 
{	
	CHSBaseDialog::OnSize(nType, cx, cy);
	SetSize();
}

void CDlgNotesAndEidolon::OnDestroy() 
{
	CHSBaseDialog::OnDestroy();
}

BOOL CDlgNotesAndEidolon::ShowWindow( int nCmdShow )
{
	Focus();
	return CHSBaseDialog::ShowWindow( nCmdShow );
}

void CDlgNotesAndEidolon::Focus()
{
	return;

	if(IsType(EidolonEdit))
	{
		m_pEidolonCtrl.SetFocus();
	}
	else if(IsType(Notes))
	{
		m_pEditCtrl.SetFocus();
	}
}

void CDlgNotesAndEidolon::SetSize()
{
	CRect rect,rc;
	GetClientRect(&rect);	
	CWnd* pWnd = this->GetDlgItem(IDOK);
	if(pWnd != NULL)
	{		
		pWnd->GetWindowRect(rc);
		rect.bottom -= (rc.Height() + 4);
		pWnd->MoveWindow(rect.right - rc.Width()*2 - 5*2,rect.bottom + 4,rc.Width(),rc.Height());
	}
	pWnd = this->GetDlgItem(IDCANCEL);
	if(pWnd != NULL)
	{		
		pWnd->GetWindowRect(rc);
//		rect.bottom -= (rc.Height() + 4);
		pWnd->MoveWindow(rect.right - rc.Width() - 5,rect.bottom + 4,rc.Width(),rc.Height());
	}
	pWnd = this->GetDlgItem(IDOK);
	if(pWnd != NULL && IsType(EidolonEdit))
	{		
		pWnd->GetWindowRect(rc);
//		rect.bottom -= (rc.Height() + 4);
		pWnd->MoveWindow(rect.right - rc.Width()*2 - 5,rect.bottom + 4,rc.Width(),rc.Height());
	}

	if(IsType(EidolonEdit))
	{
		m_pEidolonCtrl.MoveWindow(CRect(0,0,rect.Width(),rect.Height()),TRUE);
	}
	else if(IsType(Notes))
	{
		m_pEditCtrl.MoveWindow(CRect(0,0,rect.Width(),rect.Height()),TRUE);
	}
	else if(IsType(Eidolon))
	{
		m_EditEidolon.MoveWindow(CRect(0,0,rect.Width(),rect.Height()),TRUE);
	}
	/*
	CRgn rgn1,rgn2,rgn3;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.right += abs(rect.left);
	rect.left = 0;	
	rect.bottom += abs(rect.top);
	rect.top = 0;
	int nCYEdge = GetSystemMetrics(SM_CYEDGE);
	int nBottom = rect.bottom;
	rect.bottom -= (rc.Height() + nCYEdge);
	rgn1.CreateRectRgnIndirect(rect);
	int nWidth = rc.Width();
	rc.right  = rect.right;
	rc.left   = rc.right - nWidth - 15;
	rc.top    = rect.bottom - 1;
	rc.bottom = nBottom;
	rgn2.CreateRectRgnIndirect(rc);
	rgn3.CreateRectRgnIndirect(rc);
	rgn3.CombineRgn(&rgn1,&rgn2,RGN_OR);
	this->SetWindowRgn((HRGN)rgn3,TRUE);
	*/
}


LRESULT CDlgNotesAndEidolon::OnCompileUpdateData(WPARAM wp,LPARAM lp)
{
	return 0;
	if( m_pExpression != NULL)
	{
		ExpPropery NameProp;
		NameProp.m_dExpType = m_pExpression->GetExpressType();
		NameProp.m_strName.Format("%s", m_pExpression->GetName());
		NameProp.m_strDescribe.Format("%s",  m_pExpression->GetDescribe());
		NameProp.m_bStyle = m_pExpression->GetStyle();		
		if ( g_pFormulaMan )
			g_pFormulaMan->SendLinkMsg(DiagramMsg_FormulaParamChanged, (LPARAM)&NameProp);
	}


}

void CDlgNotesAndEidolon::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CHSBaseDialog::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	
}

void CDlgNotesAndEidolon::Empty()
{
	if(IsType(EidolonEdit))
	{
		m_pEidolonCtrl.SetWindowText("");
	}
	else if(IsType(Eidolon))
	{
		m_EditEidolon.Empty();
	}
	else if(IsType(Notes))
	{
		m_pEditCtrl.SetWindowText("");
		/*
		this->UpdateData(TRUE);
		if(m_pExpression != NULL)
		{
			CString strText;
			m_pEditCtrl.GetWindowText(strText);
			CMainFrame *pMainWnd = (CMainFrame *)AfxGetMainWnd();
			if( pMainWnd != NULL && pMainWnd->m_pCompileApi != NULL )
			{
				HSOpenExpressUseData data(hxOpenExpress_Notes);
				data.m_strName = &strText;
				data.m_pData   = m_pExpression;
				pMainWnd->m_pCompileApi->OpenExpress(0,(LPARAM)&data);
			}
		}
		*/
	}

	m_pExpression = NULL;
}

CString CDlgNotesAndEidolon::GetText()
{
	CString strText;
	if(IsType(EidolonEdit))
	{
		m_pEidolonCtrl.GetWindowText(strText);
	}
	else if(IsType(Eidolon))
	{		
	}
	else if(IsType(Notes))
	{
		m_pEditCtrl.GetWindowText(strText);
	}
	return strText;
}

void CDlgNotesAndEidolon::PostNcDestroy()
{
	//delete this;
}

void CDlgNotesAndEidolon::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
 	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
 	*pResult = 0;

}
