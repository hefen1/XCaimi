// DlgFormulaEdit.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFormulaEdit.h"
#include "ScriptEditViewFormula.h"
#include "InputPassPwD.H"

#include "DlgEstopPeriod.h"

#include "DlgFormulaMan.h"
#include "DlgNotesAndEidolon.H"
#include "DlgFunctionMan.h"

#include "DlgError.h"
#include "TreeCtrlFormula.h"
#include "Express.h"
#include "VarValue.h"
#include "FunExpresion.h"
#include "GeneralHelper.h"
#include "resource.h"

#define CRTDBG_MAP_ALLOC

#include <stdlib.h>

#include <crtdbg.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaEdit dialog


#define Fomula_Describe_Size 1024

#define IDC_USERPARAM   (WM_USER+1)
#define IDC_BODY		(WM_USER+2)

#define IDC_EDITTREE	(WM_USER+6)

extern HWND g_hParentWnd;

CDlgNotesAndEidolon* CDlgFormulaEdit::m_pDlgEidolon = NULL;
CDlgNotesAndEidolon* CDlgFormulaEdit::m_pDlgNotes = NULL;
CDlgFunctionMan*	 CDlgFormulaEdit::m_pFunctionMan = NULL;
CDlgError* CDlgFormulaEdit::m_pErrorDlg = NULL;
bool CDlgFormulaEdit::bNeedChangeStyle = false;
BOOL CDlgFormulaEdit::bEscMsgOfDlgEdit = FALSE;
extern UINT g_SendDeleteTechIndexMsg;

CDlgFormulaEdit::CDlgFormulaEdit(CWnd* pParent /*=NULL*/)
	: CHSBaseDialog(CDlgFormulaEdit::IDD, pParent)
{
	m_pRichEdit = NULL;

	m_pFormulaMan = (CDlgFormulaMan*)pParent;

	//{{AFX_DATA_INIT(CDlgFormulaEdit)
	m_nMainChart = 1;
	m_CharType = 1;
	//}}AFX_DATA_INIT

	//m_pWnd = this;
	m_pEditTree = NULL;

	m_nExpressType = -1;

	m_dStyle = 0;


	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_MAINFRAME_ACCE), RT_ACCELERATOR);
	m_hAccelTable   = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_MAINFRAME_ACCE));

	Create(CDlgFormulaEdit::IDD,pParent);
}

CDlgFormulaEdit::~CDlgFormulaEdit()
{
	Delete();
}

void CDlgFormulaEdit::DoDataExchange(CDataExchange* pDX)
{
	CHSBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFormulaEdit)
	//DDX_Radio(pDX, IDC_MAINCHART, m_nMainChart);
	DDX_Control(pDX, IDC_PROMTEDIT, m_wndPromte); //by HS
	//	DDX_Control(pDX, IDC_COMBOGROUP, m_wndComBoxEx);//by HS
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBOGROUP, m_wndComBox);
	DDX_Control(pDX, IDC_TABOUT, m_wndTabOut);
}


BEGIN_MESSAGE_MAP(CDlgFormulaEdit, CHSBaseDialog)
	//{{AFX_MSG_MAP(CDlgFormulaEdit)
	ON_BN_CLICKED(IDC_INFUNCTION, OnInfunction)
	ON_BN_CLICKED(IDC_COMPILE, OnCompile)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_MAINCHART, OnMainchart)
	ON_BN_CLICKED(IDC_SUBCHART, OnSubchart)
	ON_BN_CLICKED(IDC_PWD, OnPwd)
	ON_BN_CLICKED(IDC_STOPPERIOD, OnStopperiod)
	ON_BN_CLICKED(IDC_NOTES, OnNotes)
	ON_BN_CLICKED(IDC_EIDOLON, OnEidolon)
	ON_EN_KILLFOCUS(IDC_PWDEDIT, OnKillfocusPwdedit)
	ON_BN_CLICKED(IDC_ERROR, OnError)
	ON_BN_CLICKED(IDC_BUTFUN, OnButfun)
	ON_BN_CLICKED(IDC_BUTEXP, OnButexp)
	ON_BN_CLICKED(IDC_BUTFIND, OnButfind)
	ON_BN_CLICKED(ID_EDIT_REPLACE, OnEditReplace)
	ON_BN_CLICKED(IDC_BUTCOPY, OnButcopy)
	ON_BN_CLICKED(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP

	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_MESSAGE(HX_USER_SENDADDSTR, OnSendAddStr)
	ON_COMMAND(IDC_EDITPARAMLIST,OnEditParam)

	
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, OnComBoxSelChange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABOUT, OnTcnSelchangeTabout)
	ON_NOTIFY(NM_CLICK, IDC_TABOUT, OnNMClickTabout)

	ON_WM_SIZING()

	ON_WM_INITMENUPOPUP()

	ON_COMMAND_RANGE(10000,11000, OnMenu)
	ON_UPDATE_COMMAND_UI_RANGE(10000,11000, OnUpdateMenu)

END_MESSAGE_MAP()

void CDlgFormulaEdit::OnUpdateMenu(CCmdUI* pCmdUI)
{
	if( pCmdUI->m_nID == 10070 )
	{
		// 公式中风格设置:参见 #define HS_EXP_FINANCE 等定义
		pCmdUI->SetCheck( (m_dStyle & HS_EXP_FINANCE)?1:0 );
	}
	else if( pCmdUI->m_nID == 10071 )
	{
		// 公式中风格设置:参见 #define HS_EXP_PROTECTED 等定义
		pCmdUI->SetCheck( (m_dStyle & HS_EXP_PROTECTED)?1:0 );
	}
}

void CDlgFormulaEdit::OnMenu(UINT nID)
{
	/*switch( nID )
	{
	default:
		break;
	}*/
	switch( nID )
	{
	case	10000:		//       MENUITEM "编译公式",                    10000
		{
			OnCompile();
		}
		break;
	case	10001:		//       MENUITEM "关闭",                        10001
		{
			this->OnOK();
		}
		break;
	case	10020:		//       MENUITEM "剪切",                        10020
		{
		}
		break;
	case	10021:		//       MENUITEM "复制",                        10021
		{
		}
		break;
	case	10022:		//       MENUITEM "粘贴",                        10022
		{
		}
		break;
	case	10023:		//       MENUITEM "全选",                        10023
		{
		}
		break;
	case	10024:		//       MENUITEM "插入函数",                    10024
		{
			OnButfun();
		}
		break;
	case	10025:		//       MENUITEM "插入公式",                    10025
		{
			OnButexp();
		}
		break;
	case	10026:		//       MENUITEM "函数管理",                    10026
		{
			OnInfunction();
		}
		break;
	case	10050:		//       MENUITEM "用法注释",                    10050
		{
			OnNotes();
		}
		break;
	case	10051:		//       MENUITEM "参数精灵",                    10051
		{
			OnEidolon();
		}
		break;
	case	10052:		//       MENUITEM "恢复缺省",                    10052
		{
			OnDefault();
		}
		break;
	case	10070:		//       设置公式为财务数据输出
		{
			// 公式中风格设置:参见 #define HS_EXP_FINANCE 等定义
			if( m_dStyle & HS_EXP_FINANCE )
			{
				m_dStyle &= ~HS_EXP_FINANCE;
			}
			else
			{
				m_dStyle |= HS_EXP_FINANCE;
			}
		}
		break;
	case	10071:		//       公式是否为保护的
		{
			// 公式中风格设置:参见 #define HS_EXP_PROTECTED 等定义
			if( m_dStyle & HS_EXP_PROTECTED )
			{
				m_dStyle &= ~HS_EXP_PROTECTED;
			}
			else
			{
				m_dStyle |= HS_EXP_PROTECTED;
			}
		}
		break;
	default:
		break;
	}

}

void CDlgFormulaEdit::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	
    ASSERT(pPopupMenu != NULL);
    // Check the enabled state of various menu items.

    CCmdUI state;
    state.m_pMenu = pPopupMenu;
    ASSERT(state.m_pOther == NULL);
    ASSERT(state.m_pParentMenu == NULL);

    // Determine if menu is popup in top-level menu and set m_pOther to
    // it if so (m_pParentMenu == NULL indicates that it is secondary popup).
    HMENU hParentMenu;
    if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
        state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
    else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
    {
        CWnd* pParent = this;
           // Child windows don't have menus--need to go to the top!
        if (pParent != NULL &&
           (hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
        {
           int nIndexMax = ::GetMenuItemCount(hParentMenu);
           for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
           {
            if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
            {
                // When popup is found, m_pParentMenu is containing menu.
                state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
                break;
            }
           }
        }
    }

    state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
    for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
      state.m_nIndex++)
    {
        state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
        if (state.m_nID == 0)
           continue; // Menu separator or invalid cmd - ignore it.

        ASSERT(state.m_pOther == NULL);
        ASSERT(state.m_pMenu != NULL);
        if (state.m_nID == (UINT)-1)
        {
           // Possibly a popup menu, route to first item of that popup.
           state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
           if (state.m_pSubMenu == NULL ||
            (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
            state.m_nID == (UINT)-1)
           {
            continue;       // First item of popup can't be routed to.
           }
           state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
        }
        else
        {
           // Normal menu item.
           // Auto enable/disable if frame window has m_bAutoMenuEnable
           // set and command is _not_ a system command.
           state.m_pSubMenu = NULL;
           state.DoUpdate(this, FALSE);
        }

        // Adjust for menu deletions and additions.
        UINT nCount = pPopupMenu->GetMenuItemCount();
        if (nCount < state.m_nIndexMax)
        {
           state.m_nIndex -= (state.m_nIndexMax - nCount);
           while (state.m_nIndex < nCount &&
            pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
           {
            state.m_nIndex++;
           }
        }
        state.m_nIndexMax = nCount;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CDlgFormulaEdit message handlers

BOOL CDlgFormulaEdit::OnInitDialog() 
{
	CHSBaseDialog::OnInitDialog();
	
	CRect rect(0,0,0,90);
	//
	CWnd* pWnd = this->GetDlgItem(IDC_PARAMPOS);
	if( pWnd )
	{
		pWnd->GetWindowRect(rect);
		this->ScreenToClient(rect);
	}
	pWnd = this->GetDlgItem(IDC_COMPILE);
	if( pWnd )
	{
		pWnd->ShowWindow(SW_SHOW);
	}
	m_WndGridCtrl.Create(CRect(0,0,0,0), this, IDC_USERPARAM);
	initGridCtrl();

	pWnd = this->GetDlgItem(IDC_EDITPOS);
	if( pWnd )
	{
		pWnd->GetWindowRect(rect);
		this->ScreenToClient(rect);
	}
	m_pRichEdit = new CScriptEditViewFormula;
	m_pRichEdit->Create( NULL, NULL, WS_CHILD | WS_VISIBLE, 
		rect, this, IDC_BODY, NULL);
	m_pRichEdit->MoveWindow(rect);

	m_pRichEdit->m_pExternExpression = m_pFormulaMan->m_pExternExpression;   // 外部表达式表
	m_pRichEdit->m_pExternVariabile  = m_pFormulaMan->m_pExternVariabile;	 // 外部变量表
	m_pRichEdit->m_pExternFunction   = m_pFormulaMan->m_pExternFunction;	 // 外部函数表
	m_pRichEdit->SetKeywordsConstants();									 // 关键字
	CCompileEditView::m_pParentDlg = this;

	// 参数精灵
	m_pDlgEidolon = new CDlgNotesAndEidolon();
	m_pDlgEidolon->Create(this);
	m_pDlgEidolon->SetType(CDlgNotesAndEidolon::EidolonEdit);
	m_pDlgEidolon->SetSize();

	// 注释
	m_pDlgNotes = new CDlgNotesAndEidolon();
	m_pDlgNotes->Create(this);
	m_pDlgNotes->SetType(CDlgNotesAndEidolon::Notes);
	m_pDlgNotes->SetSize();


	m_pErrorDlg = new CDlgError(this,m_pRichEdit);	
	m_pRichEdit->Atatch(&m_pErrorDlg->m_wndList);

	CTabCtrl* pTab = (CTabCtrl*)this->GetDlgItem(IDC_TABOUT);
	if( pTab != NULL )
	{
		pTab->InsertItem(0,_T("编译结果"));
	}

	//CRect rect;
	this->GetWindowRect(rect);
	rect.right  = rect.left + 517;
	rect.bottom = rect.top  + 359;
	this->MoveWindow(rect);
	//SetSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgFormulaEdit::initGridCtrl(int nRow)
{
	if ( m_WndGridCtrl.GetSafeHwnd() )
	{
		try
		{
			m_WndGridCtrl.SetRowCount(nRow+1);
			m_WndGridCtrl.SetColumnCount(6);
			m_WndGridCtrl.SetFixedRowCount(1);
			m_WndGridCtrl.SetFixedColumnCount(1);
			m_WndGridCtrl.SetGridBkColor(RGB(255,255,255));
			m_WndGridCtrl.SetBkColor(RGB(255,255,255));
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return;
		}

		for (int row = 0; row < m_WndGridCtrl.GetRowCount(); row++)
		{
			for (int col = 0; col < m_WndGridCtrl.GetColumnCount(); col++)
			{ 
				GV_ITEM Item;
				Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR;
				Item.row = row;
				Item.col = col;
				Item.crBkClr = RGB(255,255,255);
				//Item.crFgClr = RGB(255,255,255);
				if (row < 1) 
				{
					Item.nFormat = DT_LEFT|DT_WORDBREAK;
					if( col == 0 )
					{
						Item.strText = "";
					}
					else
					{
						Item.strText.Format("%s",col == 1?_T("参数名"):
							col == 2?_T("最小"):
							col == 3?_T("最大"):
							col == 4?_T("当前值"):_T("步长")/*"描述            "*/);//因为暂无描述，故将其去除
					}
				}
				else if (col < 1) 
				{
					Item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					if( col == 0 )
					{
						Item.strText.Format(_T("%d"),row);
					}
				} 
				else 
				{
					Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
					Item.strText = "";
				}
				m_WndGridCtrl.SetItem(&Item);	
 				CRect rect;
 				GetClientRect(rect);
 				m_WndGridCtrl.SetColumnWidth(col,rect.Width()/6-5);
			}
		}
	
		CDC* pDC = this->GetDC();
		if(pDC != NULL)
		{
			m_WndGridCtrl.SetRowHeight(0, pDC->GetTextExtent("A").cy);
			this->ReleaseDC(pDC);
		}
		else
		{
			m_WndGridCtrl.SetRowHeight(0, m_WndGridCtrl.GetRowHeight(0)/3*2);
		}
	}
}
void CDlgFormulaEdit::Delete()
{
	if(m_hAccelTable != NULL)
	{
		DestroyAcceleratorTable(m_hAccelTable);
	}

	if(m_pEditTree != NULL)
	{
		m_pEditTree->DestroyWindow();
		delete m_pEditTree;
		m_pEditTree = NULL;
	}

	if( m_pRichEdit != NULL && !IsBadReadPtr(m_pRichEdit,1))
	{
		if(::IsWindow(m_pRichEdit->m_hWnd))
		{
			m_pRichEdit->DestroyWindow();
		}
		//delete m_pRichEdit;
		m_pRichEdit = NULL;
	}
	if(m_pDlgEidolon != NULL)
	{
		delete m_pDlgEidolon;
		m_pDlgEidolon = NULL;
	}
	if(m_pDlgNotes != NULL)
	{
		delete m_pDlgNotes;
		m_pDlgNotes = NULL;
	}
	if(m_pFunctionMan != NULL)
	{
		delete m_pFunctionMan;
		m_pFunctionMan = NULL;
	}
	if(m_pErrorDlg != NULL)
	{
		delete m_pErrorDlg;
		m_pErrorDlg = NULL;
	}	
	
}

void CDlgFormulaEdit::OnOK() 
{
	OnSave();

	Show(SW_HIDE);



	SetActiveMain(this);
}

void CDlgFormulaEdit::OnCancel() 
{
	if(m_pRichEdit != NULL)
	{
		m_pRichEdit->HideDataTip();
	}
	m_bOkFlag = FALSE;

	Show(SW_HIDE);
	SetActiveMain(this);
}

void CDlgFormulaEdit::Show(int nShow)
{
	if(m_pDlgEidolon != NULL)
	{
		m_pDlgEidolon->ShowWindow(nShow);
	}
	if(m_pDlgNotes != NULL)
	{
		m_pDlgNotes->ShowWindow(nShow);
	}
	if(m_pFormulaMan != NULL)
	{
		m_pFormulaMan->Enable(TRUE);
	}
	if(m_pFunctionMan != NULL)
	{
		m_pFunctionMan->ShowWindow(nShow);
	}
	this->ShowWindow(nShow);
}

BOOL CDlgFormulaEdit::ShowWindow( int nCmdShow )
{
	if( !::IsWindow(this->m_hWnd) )
		return 0;

	if( nCmdShow == SW_HIDE || nCmdShow == SW_MINIMIZE )
		return CHSBaseDialog::ShowWindow(nCmdShow);


 	if( IsIconic() )
 	{
 		return CHSBaseDialog::ShowWindow(SW_RESTORE);
 	}

	if(  IsZoomed() ) // max
		return CHSBaseDialog::ShowWindow(nCmdShow);

	if( ::AnimateWindow(m_hWnd,100,AW_CENTER | AW_ACTIVATE) )
	{
		this->Invalidate();
	}
	else
	{
		return CHSBaseDialog::ShowWindow(nCmdShow);
	}

	return 1;
}

void CDlgFormulaEdit::OnInfunction() 
{
	// TODO: Add your control notification handler code here
	if( m_pFunctionMan == NULL )
	{
		m_pFunctionMan = new CDlgFunctionMan(this,m_pRichEdit->m_pExternFunction,
										  m_pRichEdit->m_pExternExpression,
										  (long*)&m_pFunctionMan);
	}
	if(m_pFunctionMan != NULL)
	{
		m_pFunctionMan->ShowWindow(SW_SHOWNORMAL);
	}
}

void CDlgFormulaEdit::OnCompile() 
{
	if(m_pRichEdit != NULL)
	{
		CString strValue;
		m_pRichEdit->GetRichEditCtrl().GetWindowText(strValue);
		if(strValue == "")
		{
			MessageBox(_T("必须至少有一个表达式，请重试！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
			//m_wndPromte.SetWindowText("公式有几个错误!");
			return;
		}

		m_pRichEdit->Compile(this);
	}
}

void CDlgFormulaEdit::OnSave()
{
	// TODO: Add extra validation here
	//Delete();
	//WRITE_LOG ::OnOK();
	CExpression* pExpresie = NULL;
	char	cOldType = m_cType;
	if(m_pRichEdit != NULL)
	{
		CString strValue;
		m_pRichEdit->GetRichEditCtrl().GetWindowText(strValue);
		if(strValue == "")
		{
			MessageBox(_T("必须至少有一个表达式，请重试！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
			//m_wndPromte.SetWindowText("公式有几个错误!");
			return;
		}		

		CDlgFormulaEdit::bNeedChangeStyle = true;
		pExpresie = (CExpression*)m_pRichEdit->Compile(this);
		if(m_CharType != m_nMainChart)
			::SendMessage(g_hParentWnd,g_SendDeleteTechIndexMsg,0,(LPARAM)pExpresie->GetName().GetBuffer()); //qinhn 20090726 Add 
		CDlgFormulaEdit::bNeedChangeStyle = false;
		if( pExpresie == NULL )
			return;

		if( pExpresie && *(long*)pExpresie == 0 )
			return;

		if( pExpresie && *(long*)pExpresie < 0 )
			pExpresie = NULL;

		m_pRichEdit->HideDataTip();
	}

	m_bOkFlag = TRUE;

//begin by HS 0328
	CString strOk;
	(m_RDDefault.m_strSource).TrimLeft();
	(m_RDDefault.m_strSource).TrimRight();
	m_pRichEdit->GetRichEditCtrl().GetWindowText(strOk);
	strOk.TrimLeft();
	strOk.TrimRight();

	if ( m_RDDefault.m_strSource != strOk && 
		 !strOk.IsEmpty() )
	{
		bool bStore = true;
		m_RDDefault.Serialize(bStore);
	}	

	// 改变公式组
	CDlgFormulaMan* pFomulaman = (CDlgFormulaMan*)m_pFormulaMan;
	if( pFomulaman != NULL)
	{
		CTreeCtrlFormula* pHxTreeCtrl = NULL;

		pHxTreeCtrl = pFomulaman->GetCurTree(pExpresie);
		if( pHxTreeCtrl != NULL )
		{
			HTREEITEM hitemDrag = NULL;
			HTREEITEM hitemDrop = NULL;
			
			CTreeGroup*	pTreeGroup = pHxTreeCtrl->Find(pExpresie);
			if( pTreeGroup != NULL )
			{
				hitemDrag = pTreeGroup->m_hTree;
			}

			CString strName;
			int nNowIndex = this->m_wndComBox.GetCurSel();
			if( nNowIndex != -1 )
			{
				hitemDrop = (HTREEITEM)this->m_wndComBox.GetItemData(nNowIndex);
			}
			else
			{
				this->m_wndComBox.GetWindowText(strName);
			}

			pHxTreeCtrl->ChangItemToGroup(hitemDrag, hitemDrop, strName);
		}
	}
	//end

	if( this->IsType(CDlgFormulaEdit::SpeedNew) && pExpresie != NULL && ::IsWindow(g_hParentWnd) )
	{
		::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
					  HSCurKeyInfo::DragUpdateExpressData2,
					  (LPARAM)pExpresie);
	}

	// by HS add 2006.07.02
	if( pFomulaman )
		pFomulaman->Save();

	Show(SW_HIDE);
	SetActiveMain(this);
}

void CDlgFormulaEdit::OnMainchart() 
{
	// TODO: Add your control notification handler code here
	m_nMainChart = 0;
	
	//CWnd* pWnd = (CWnd*)GetDlgItem(IDC_DEFAXISEDIT);
	//if(pWnd != NULL)
	//{
	//	pWnd->EnableWindow(m_nMainChart);
	//}
	
}

void CDlgFormulaEdit::OnSubchart() 
{
	// TODO: Add your control notification handler code here
	m_nMainChart = 1;
	
	//CWnd* pWnd = (CWnd*)GetDlgItem(IDC_DEFAXISEDIT);
	//if(pWnd != NULL)
	//{
	//	pWnd->EnableWindow(m_nMainChart);
	//}
}

void CDlgFormulaEdit::OnPwd() 
{
	CButton* pBut = (CButton*)GetDlgItem(IDC_PWD);
	if(pBut != NULL)
	{
		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_PWDEDIT);
		if(pWnd != NULL)
		{
			if(!pBut->GetCheck())
			{
				pWnd->SetWindowText("");
			}
			pWnd->EnableWindow(pBut->GetCheck());
		}
	}	
}

void CDlgFormulaEdit::OnStopperiod() 
{
	if(m_pRichEdit != NULL)
	{
		m_pRichEdit->SendMessage(WM_COMMAND,IDC_BANPIERIODS);
	}
}

void CDlgFormulaEdit::OnNotes() 
{
	// TODO: Add your control notification handler code here
	if(m_pDlgNotes != NULL)
	{
		m_pDlgNotes->ShowWindow(SW_SHOWNORMAL);
	}
}

void CDlgFormulaEdit::OnEidolon() 
{
	// TODO: Add your control notification handler code here
	if(m_pDlgEidolon != NULL)
	{
		m_pDlgEidolon->ShowWindow(SW_SHOWNORMAL);
	}
}

long CDlgFormulaEdit::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if( (m_pRichEdit != NULL && 
		 m_pRichEdit->m_pExternExpression == NULL) ||
		(lParam == 0) )
		return 0;

	CExpression* pExp = (CExpression*)lParam;
	if( pExp == NULL )
		return 0;
	
	m_nExpressType = pExp->GetExpressType();

	CString pstrName = pExp->GetName();
	CString strOldName;
	CWnd* pNameEdit = GetDlgItem(IDC_NAMEEDIT);
	if( pNameEdit != NULL )
	{
		pNameEdit->GetWindowText(strOldName);
	}

	pNameEdit->SetWindowText(pstrName);
	m_strOldName = pstrName;
	SetType(CDlgFormulaEdit::Update);

	pNameEdit->EnableWindow(FALSE);

	// 主图/副图
	m_nMainChart = ((pExp->IsStyle(HS_EXP_MAINCHART))?0:1);
	m_CharType = m_nMainChart;
	CWnd* pWnd = GetDlgItem(IDC_MAINCHART);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(!m_nMainChart);

		pWnd->EnableWindow( m_nExpressType == Tech );
	}
	pWnd = GetDlgItem(IDC_SUBCHART);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(m_nMainChart);

		pWnd->EnableWindow( m_nExpressType == Tech );
	}
	
	// 公式中风格设置:参见 #define HS_EXP_FINANCE 等定义
	m_dStyle = pExp->GetStyle();

	// 密码
	pWnd = GetDlgItem(IDC_PWDEDIT); 
	if(pWnd != NULL)
	{
		if( !pExp->GetPWD().IsEmpty() )
		{
			CInputPassPwD dlg(this,_T("请输入公式密码"),pstrName);
			dlg.m_pExp = pExp;
			if( dlg.DoModal() == IDCANCEL )
			{
				if( pNameEdit != NULL )
				{
					pNameEdit->SetWindowText(strOldName);
				}
				return FALSE;
			}
			//pstrName.Format("%s",pExp->GetPWD());
			
			pWnd->EnableWindow(TRUE);
			pWnd->SetWindowText(pExp->GetPWD());
			
			pWnd = GetDlgItem(IDC_PWD);
			if(pWnd != NULL)
			{
				((CButton*)pWnd)->SetCheck(1);
			}
		}
		else
		{
			pWnd->EnableWindow(FALSE);
			pWnd->SetWindowText("");
			
			pWnd = GetDlgItem(IDC_PWD);
			if(pWnd != NULL)
			{
				((CButton*)pWnd)->SetCheck(0);
			}
		}
	}				
	
	pWnd = GetDlgItem(IDC_DESCRIBEEDIT);   // 公式描述
	if(pWnd != NULL)
	{
		pWnd->SetWindowText(pExp->GetDescribe());
	}
	
	if(m_pDlgNotes != NULL)
	{
		m_pDlgNotes->SetNotes(pExp->GetNotes(),pExp->GetName()); // 注释
	}
	
	if( m_pDlgEidolon )       // 参数精灵
	{
		m_pDlgEidolon->SetEidolonEdit(pExp->GetEidolon(),pExp->GetName());
	}

  	
   	if( m_WndGridCtrl.GetSafeHwnd() )
   	{
   		CMapVariabile* pvarsmap = pExp->GetParamVar();
   		CNumericValue* pvalue;
   		if ( pvarsmap && !IsBadReadPtr(pvarsmap,1))
   		{
   			CArray<class CValue*,class CValue*>* parray = pvarsmap->GetObjArray();
   			if ( parray && !IsBadReadPtr(parray, 1))
   			{
   				CString sname;
   				CString strretname;
   				int nrow = m_WndGridCtrl.GetFixedRowCount();  
				int ncolumn;
   				for(;nrow < m_WndGridCtrl.GetRowCount(); nrow++)
   				{
					ncolumn = m_WndGridCtrl.GetFixedColumnCount();
   					for (;ncolumn<m_WndGridCtrl.GetColumnCount(); ncolumn++)
   					{
   						m_WndGridCtrl.SetItemText(nrow,ncolumn,"");
   					}	
   				}
   				nrow = m_WndGridCtrl.GetFixedRowCount();
   				ncolumn = m_WndGridCtrl.GetFixedColumnCount();
   				for(int nobj = 0; parray != NULL && nobj < parray->GetSize(); nobj++)
   				{
   					pvalue = (CNumericValue*)parray->GetAt(nobj);
					ncolumn = m_WndGridCtrl.GetFixedColumnCount();
   					if( !pvalue->IsStyle(HX_EXPRESSNUMBER) )
   						continue;

					CString strvalue;
					CString sName = pvarsmap->GetObjectName(pvalue);			
					m_WndGridCtrl.SetItemText(nrow,1,sName);
					strvalue.Format("%f", pvalue->GetMinValue());
					strvalue = hxSubZero(strvalue);
					m_WndGridCtrl.SetItemText(nrow,2,strvalue);
					strvalue.Format("%f", pvalue->GetMaxValue());
					strvalue = hxSubZero(strvalue);
					m_WndGridCtrl.SetItemText(nrow,3,strvalue);
					strvalue.Format("%f", pvalue->GetValue());
					strvalue = hxSubZero(strvalue);
					m_WndGridCtrl.SetItemText(nrow,4,strvalue);
					strvalue.Format("%f", pvalue->GetTestStepValue());
					strvalue = hxSubZero(strvalue);
					m_WndGridCtrl.SetItemText(nrow,5,strvalue);
   				
   					nrow++;
   				}
   			}
   		}
   		
   		m_WndGridCtrl.Invalidate();
   	}	
	
	m_pRichEdit->GetRichEditCtrl().SetSel( 0, -1);
	m_pRichEdit->GetRichEditCtrl().ReplaceSel(pExp->GetSource(),TRUE);
//	((CScriptEditViewFormula*)m_pRichEdit)->FormatAll();

	///begin by HS 0328
	//	m_strDefault = pExp->GetSource();
	m_RDDefault.m_strName = pExp->GetWholeName();
	m_RDDefault.m_strSource = pExp->GetSource();

	//end

	m_pRichEdit->SetFocus();
	
	SetTitle(pExp->GetExpressType(),pstrName);
	
	if(m_pFormulaMan != NULL)
	{
		/*
		char nExpressType = pExp->GetExpressType();
		nExpressType = nExpressType == Tech?0:
		nExpressType == Exchange?1:
		nExpressType == Condition?2:
		nExpressType == MoreKLine?3:
		nExpressType == ExpressBin?4:5;
		m_pFormulaMan->m_wndTab.SetCurSel(nExpressType);
		*/
		m_pFormulaMan->ShowTree(m_pFormulaMan->GetCurTree(pExp),TRUE);
		m_pFormulaMan->ShowFomulaDlg(this,0,lParam);
	}
	
	this->ShowResult(CDlgFormulaEdit::Translate);

	if( m_pErrorDlg )
	{
		m_pErrorDlg->ClearError();
	}

	return 1;
}

/*************************************************************
* 加入
* 时间：  2003年9月23日
* 描述：  快速新建公式
*************************************************************/
//void CDlgFormulaEdit::SpeedCreateExpress()
//{
//	m_RDDefault.Clear();
//
//	if( (m_pRichEdit != NULL && 
//		 m_pRichEdit->m_pExternExpression == NULL) )
//	{
//		return;
//	}
//
//	if( m_pErrorDlg )
//	{
//		m_pErrorDlg->ClearError();
//	}
//
//	SetType(CDlgFormulaEdit::SpeedNew);
//	SetExType(CDlgFormulaEdit::SpeedNew);
//	SetTitle(Tech,"快速新建");
//	m_nExpressType = Tech;
//	m_nMainChart = 1;
//
//	m_pRichEdit->GetRichEditCtrl().SetWindowText(""); // 公式体
//	CWnd* pNameEdit = GetDlgItem(IDC_NAMEEDIT); // 名称
//	if( pNameEdit != NULL )
//	{
//		pNameEdit->SetWindowText("");
//		pNameEdit->EnableWindow(TRUE);
//		pNameEdit->SetFocus();
//	}
//
//	pNameEdit = GetDlgItem(IDC_SUBCHART); // 主副图
//	if( pNameEdit != NULL )
//	{
//		((CButton*)pNameEdit)->SetCheck(m_nMainChart);
//	}
//
//	CWnd* pWnd = GetDlgItem(IDC_PWDEDIT);   // 密码		
//	if(pWnd != NULL)
//	{
//		pWnd->SetWindowText("");
//	
//		pWnd->EnableWindow(FALSE);
//	}
//	pWnd = GetDlgItem(IDC_PWDCHECK);
//	if(pWnd != NULL)
//	{
//		((CButton*)pWnd)->SetCheck(0);
//	}
//	pWnd = GetDlgItem(IDC_DESCRIBEEDIT);    // 公式描述
//	if(pWnd != NULL)
//	{
//		pWnd->SetWindowText("");
//	}
//
//	if(m_pDlgNotes != NULL)
//	{
//		m_pDlgNotes->SetNotes("",""); // 注释
//	}
//	
//	if( m_pDlgEidolon )       // 参数精灵
//	{
//		m_pDlgEidolon->Empty();
//		m_pDlgEidolon->SetEidolonEdit("","");
//	}
//
//	if(m_WndGridCtrl.GetSafeHwnd())				   // 参数
//	{
//		m_WndGridCtrl.SetParamer(NULL);
//	}
//	
//	if(m_pRichEdit->m_pEstopPeriodDlg != NULL)
//	{
//		m_pRichEdit->m_pEstopPeriodDlg->Empty();
//	}
//}

void CDlgFormulaEdit::OnNewExpress()
{
	if( m_pErrorDlg )
	{
		m_pErrorDlg->ClearError();
	}

	//by HS 0329
	m_RDDefault.Clear();
	//end

	if( (m_pRichEdit != NULL && 
		 m_pRichEdit->m_pExternExpression == NULL) )
		return;

	this->SetType(CDlgFormulaEdit::NewExp);
	m_pRichEdit->GetRichEditCtrl().SetWindowText(""); // 公式体

	CWnd* pNameEdit = GetDlgItem(IDC_NAMEEDIT); // 名称
	if( pNameEdit != NULL )
	{
		pNameEdit->SetWindowText("");
		pNameEdit->EnableWindow(TRUE);
		pNameEdit->SetFocus();
	}

	// 主图/副图
	m_nMainChart = 1;
	CWnd* pWnd = GetDlgItem(IDC_MAINCHART);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(!m_nMainChart);

		pWnd->EnableWindow( m_nExpressType == Tech );
	}
	pWnd = GetDlgItem(IDC_SUBCHART);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(m_nMainChart);

		pWnd->EnableWindow( m_nExpressType == Tech );
	}
	
	m_dStyle = 0;

	pWnd = GetDlgItem(IDC_PWDEDIT);   // 密码		
	if(pWnd != NULL)
	{
		pWnd->SetWindowText("");
		pWnd->EnableWindow(FALSE);
	}
	pWnd = GetDlgItem(IDC_PWDCHECK);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(0);
	}
	pWnd = GetDlgItem(IDC_DESCRIBEEDIT);    // 公式描述
	if(pWnd != NULL)
	{
		pWnd->SetWindowText("");
	}

	if(m_pDlgNotes != NULL)
	{
		m_pDlgNotes->SetNotes("",""); // 注释
	}
	
	if( m_pDlgEidolon )       // 参数精灵
	{
		m_pDlgEidolon->Empty();
		m_pDlgEidolon->SetEidolonEdit("","");
	}

 	if(m_WndGridCtrl.GetSafeHwnd())				   // 参数
 	{
 		//m_WndGridCtrl.emp;
 		int nRow = m_WndGridCtrl.GetFixedRowCount();
 
 		for(;nRow < m_WndGridCtrl.GetRowCount(); nRow++)
 		{
 			int nColumn = m_WndGridCtrl.GetFixedColumnCount();
 			for (;nColumn<m_WndGridCtrl.GetColumnCount(); nColumn++)
 			{		
 				m_WndGridCtrl.SetItemText(nRow,nColumn,"");
 			}
 			
 		}
 	}
	
	if(m_pRichEdit->m_pEstopPeriodDlg != NULL)
	{
		m_pRichEdit->m_pEstopPeriodDlg->Empty();
	}
}

void CDlgFormulaEdit::OnKillfocusPwdedit() 
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_PWDEDIT);
	if( pWnd != NULL )
	{
		CString strFistPWD;
		pWnd->GetWindowText(strFistPWD);
		if( !strFistPWD.IsEmpty() && 
			!m_strFistPWD.Compare(strFistPWD) ) 
			return;

		CWnd* pFocus = this->GetFocus();
		CButton* pBut = (CButton*)GetDlgItem(IDC_PWD);

		if( !strFistPWD.IsEmpty() && pBut != pFocus )
		{	
			CInputPassPwD dlg(this,_T("确认密码"),_T("请将密码重新输入一遍:"));
			dlg.m_strFistPWD = strFistPWD;
			if( dlg.DoModal() == IDCANCEL )
			{			
				pWnd->SetFocus();
				return;
			}
			m_strFistPWD = dlg.m_strFistPWD;

			if( GetDlgItem(IDOK) == pFocus )
			{
				OnOK();
			}
		}
		else
		{
			//Reworded by HS 消除DWJ关于密码保护所提出的问题
			//pWnd->SetWindowText("");
			//if(pBut != NULL)
			//{
			//	pBut->SetFocus();
			//	pBut->SetCheck(0);
			//	//pWnd->EnableWindow(FALSE);
			//}
		}
	}	
}

void CDlgFormulaEdit::OnError() 
{
	if(m_pErrorDlg != NULL)
	{
		m_pErrorDlg->ShowWindow(SW_SHOWNORMAL);
	}
}

BOOL CDlgFormulaEdit::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	// 快键
	if( pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			return FALSE;
		}
		
		if(m_pRichEdit != NULL)
		{
			if(m_pRichEdit->OnReturn(pMsg))
				return TRUE;
			else if ( m_hAccelTable != NULL &&
				::TranslateAccelerator(m_pRichEdit->m_hWnd, m_hAccelTable, pMsg))
			{
				return TRUE;
			}
		}

		if (pMsg->wParam == VK_ESCAPE)
		{
			CDlgFormulaEdit::bEscMsgOfDlgEdit = TRUE;
		}
	}
	else if( pMsg->message == WM_LBUTTONDBLCLK )
	{
		//CWnd* pWnd = GetDlgItem(IDC_PROMTEDIT);
		//if( pWnd != NULL &&
		//	pMsg->hwnd == pWnd->m_hWnd && 
		//	::IsWindow(pWnd->m_hWnd) )
		//{
		//	OnError();
		//	return 0;
		//}
	}
	else if( pMsg->message == WM_LBUTTONDOWN )
	{
		if(m_pRichEdit != NULL)
		{
			m_pRichEdit->ShowOnline(SW_HIDE);
		}
	}

	return CHSBaseDialog::PreTranslateMessage(pMsg);
}

void CDlgFormulaEdit::SetTitle(int nType,CString strName)
{
	CString strText;
	switch(nType)
	{
	case Tech:
		strText.Format("%s-%s",CTreeCtrlFormula::g_strTech,strName);
		break;
	case Exchange:
		strText.Format("%s-%s",CTreeCtrlFormula::g_strExchange,strName);
		break;
	case Strategy: 
		strText.Format("%s-%s",CTreeCtrlFormula::g_strStrategy,strName);
		break;
	case Condition:
		strText.Format("%s-%s",CTreeCtrlFormula::g_strCondition,strName);
		break;
	case MoreKLine:
		strText.Format("%s-%s",CTreeCtrlFormula::g_strColorKLine,strName);
		break;
	default:
		break;
	}
	SetWindowText(strText);
}

void CDlgFormulaEdit::OnButfun() 
{
	
	// TODO: Add your control notification handler code here
	CWnd* pWnd = m_pRichEdit;//this->GetDlgItem(IDC_BUTFUN);
	if(pWnd != NULL)
	{
		CRect rect;
		pWnd->GetWindowRect(rect);
		ShowEditTree(rect,SW_SHOW,CExpression::ExpExternFun);
	}
}

void CDlgFormulaEdit::OnButexp() 
{
	// TODO: Add your control notification handler code here
	CWnd* pWnd = m_pRichEdit;//this->GetDlgItem(IDC_BUTFUN);
	if(pWnd != NULL)
	{
		CRect rect;
		pWnd->GetWindowRect(rect);
		ShowEditTree(rect,SW_SHOW,CExpression::ExpExternExp);
	}
}

void CDlgFormulaEdit::OnButfind() 
{
	// TODO: Add your control notification handler code here
	if(m_pRichEdit == NULL) return;
	m_pRichEdit->SetFocus();
	m_pRichEdit->SendMessage(WM_COMMAND,ID_EDIT_FIND);
}

void CDlgFormulaEdit::OnEditReplace() 
{
	// TODO: Add your control notification handler code here
	if(m_pRichEdit == NULL) return;
	m_pRichEdit->SetFocus();
	m_pRichEdit->SendMessage(WM_COMMAND,ID_EDIT_REPLACE);
}

void CDlgFormulaEdit::OnButcopy() 
{
	// TODO: Add your control notification handler code here
	if(m_pRichEdit == NULL) return;
	m_pRichEdit->SetFocus();
	m_pRichEdit->SendMessage(WM_COMMAND,ID_EDIT_COPY);
}

void CDlgFormulaEdit::OnEditPaste() 
{
	// TODO: Add your control notification handler code here
	if(m_pRichEdit == NULL) return;
	m_pRichEdit->SetFocus();
	m_pRichEdit->SendMessage(WM_COMMAND,ID_EDIT_PASTE);

}

CTreeCtrlFormula* CDlgFormulaEdit::CreateEditTree()
{
	if(m_pEditTree == NULL)
	{
		m_pEditTree = new CTreeCtrlFormula;
		m_pEditTree->SetType(CTreeCtrlFormula::EditFunRef);
		
		if ( !m_pEditTree->CWnd::CreateEx( WS_EX_CLIENTEDGE,WC_TREEVIEW,NULL, 
			WS_POPUP|WS_BORDER|TVS_HASLINES|TVS_LINESATROOT|
			TVS_HASBUTTONS|TVS_SHOWSELALWAYS,//|TVS_SINGLEEXPAND, 
			0,0,300,200,this->m_hWnd,NULL,NULL ) )
		{
			TRACE0("Failed to create workspace view\n");
			return 0;
		}
		
		m_pEditTree->SetImageList(&CDlgFormulaMan::m_expressImages, TVSIL_NORMAL);
		m_pEditTree->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	}
	return m_pEditTree;
}

void CDlgFormulaEdit::ShowEditTree(CRect& RefRect,int nCmdShow,int nType)
{
	CTreeCtrlFormula* pTree = CreateEditTree();
	if(pTree == NULL) return;
	if(pTree->IsWindowVisible())
	{
		return;
	}

	CRect rc;
	rc.left   = RefRect.left + RefRect.Width()/2;
	rc.top    = RefRect.top;
	rc.right  = rc.left + RefRect.Width()/2;
	rc.bottom = rc.top  + RefRect.Height();
	pTree->MoveWindow(rc);
	
	if(nType == CExpression::ExpExternFun)
	{
		if( !m_pEditTree->IsType(CTreeCtrlFormula::EditFunRef) || m_pEditTree->GetCount() <= 0)
		{
			m_pEditTree->DeleteAllItems();
			m_pEditTree->SetType(CTreeCtrlFormula::EditFunRef);
			for(int i = 0; i < 11;i++)
			{
				AddFunByType(CTreeCtrlFormula::g_strFunName[i].m_strName,i,m_pEditTree);
			}
		}
	}
	else if(m_pFormulaMan)
	{
		if( !m_pEditTree->IsType(CTreeCtrlFormula::EditExpRef) || m_pEditTree->GetCount() <= 0)
		{
			m_pEditTree->DeleteAllItems();
			m_pEditTree->SetType(CTreeCtrlFormula::EditExpRef);
			m_pFormulaMan->CopyTree(m_pEditTree);
		}
	}
	
	pTree->SetFocus();
	pTree->ShowWindow(nCmdShow);
}

void CDlgFormulaEdit::AddFunByType(CString strRoot,int nType,CTreeCtrlFormula* pTree)
{
	if( CExpression::m_pExternFunction == NULL || pTree == NULL )
		return;

	POSITION pos = CExpression::m_pExternFunction->GetStartPosition();
	CString	sName;
	CValue* pVal;
	while (pos != NULL)
	{
		CExpression::m_pExternFunction->GetNextAssoc (pos, sName, pVal);
		if( ((CFunExpresion*)pVal)->IsFunType( (FunctionType)nType ) || nType == 0 )
		{
			// 热门板块函数，加入, 2003年11月14日
			if( ((short)((CFunExpresion*)pVal)->GetOperator()) < 0 ) 
			{
				continue;
			}
			pTree->InputFunItem(strRoot,nType,"",(CFunExpresion*)pVal);
		}
	}
}
//
void CDlgFormulaEdit::AddToParamerList(CExpression* pExp)
{
	if(pExp == NULL) 
		return;

	CString strName;
	if ( m_WndGridCtrl.GetSafeHwnd() )
	{
		for(int nRow = m_WndGridCtrl.GetFixedRowCount(); nRow < m_WndGridCtrl.GetRowCount(); nRow++)
		{
			strName = m_WndGridCtrl.GetItemText(nRow,1);
			if( strName.IsEmpty() )
			{		
				return;
			}
			CNumericValue* pValue = new CNumericValue();
			if(pValue == NULL) return ;
			int nColumn = m_WndGridCtrl.GetFixedColumnCount()+1;
			for (;nColumn<m_WndGridCtrl.GetColumnCount(); nColumn++)
			{		
				CString param = m_WndGridCtrl.GetItemText(nRow,nColumn); 
				param = ReplaceNumber(param);
				if (nColumn == 2)
						pValue->SetMinValue(atof(param));
				if (nColumn == 3)
						pValue->SetMaxValue(atof(param));
				if (nColumn == 4)
						pValue->SetValue(atof(param));
				if (nColumn == 5)
						pValue->SetTestStepValue(atof(param));
			}
			pExp->AddParamer(strName,pValue);
		}
	}
	
}
long CDlgFormulaEdit::OnSendAddStr(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(lParam == 0)
		return 0;

	CValue* valoare;
	short nRet;

	switch(wParam)
	{
	case CTreeCtrlFormula::EditFunRef:
		{
			CString str;
			str.Format("%s",*(CString*)lParam);
			nRet = str.Find("-");
			if(nRet != -1)
			{
				str = str.Left(nRet);
			}
			nRet = (short)wParam;
			valoare = GetVariable(str,nRet);
			if( valoare )
			{
				CFunExpresion* pFun = (CFunExpresion*)valoare;
				HSAddStrCompileEdit data;			
				CString strFun;
				data.m_nCharPos = pFun->GetFunDefMode(strFun);
				data.m_strText  = new CString(strFun);

				m_pRichEdit->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&data);
			}
		}
		break;
	case CTreeCtrlFormula::EditExpRef:
		{
			valoare = (CValue*)lParam;
			if( valoare )
			{
				CExpression* pExpression = valoare->GetExp();
				if(pExpression != NULL)
				{
					HSAddStrCompileEdit data;
					data.m_nCharPos = 0;
					data.m_strText  = (CString*)pExpression;

					m_pRichEdit->SendMessage(HX_USER_SENDADDSTR,
						HX_EXPRESSSENTENCE,(LPARAM)&data);
				}
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

CValue* CDlgFormulaEdit::GetVariable(CString strName,short& nRet)
{
	strName.MakeUpper();
	CValue* valoare;

	if(CTreeCtrlFormula::EditFunRef == nRet)
	{
		if ( CExpression::m_pExternFunction != NULL && 
			CExpression::m_pExternFunction->Lookup(strName,valoare) ) // 外部函数表
		{
			nRet = CExpression::ExpExternFun;
			return valoare;
		}
	}
	if(CTreeCtrlFormula::EditExpRef == nRet)
	{
		if ( CExpression::m_pExternExpression != NULL && 
			CExpression::m_pExternExpression->Lookup(strName,valoare) ) // 
		{
			nRet = CExpression::ExpExternExp;
			return valoare;
		}
	}

	return NULL;
}

void CDlgFormulaEdit::OnEditParam()
{
	if(m_WndGridCtrl.GetSafeHwnd())
	{
		m_WndGridCtrl.SetFocus();
	}
}

void CDlgFormulaEdit::OnSize(UINT nType, int cx, int cy) 
{
	CHSBaseDialog::OnSize(nType, cx, cy);
	
	if( abs(cx) + abs(cy) <= 0 ) 
		return;

	SetSize();
}

void CDlgFormulaEdit::SetSize()
{
	CRect rcClient,rcParam;
	GetClientRect(rcClient);

	CWnd* pWnd;
 	if( m_WndGridCtrl.GetSafeHwnd() )
 	{
 		pWnd = this->GetDlgItem(IDC_PARAMPOS);
 		if( pWnd )
 		{
 			pWnd->GetWindowRect(rcParam);
 			this->ScreenToClient(rcParam);
 			m_WndGridCtrl.MoveWindow(rcParam);
 		
 		}
 	}

	if( m_pRichEdit )
	{
		pWnd = this->GetDlgItem(IDC_EDITPOS);
		if( pWnd )
		{
			pWnd->GetWindowRect(rcParam);
			this->ScreenToClient(rcParam);
			
			rcParam.bottom = rcClient.bottom - 40;//80;
			rcParam.right  = rcClient.right;
			m_pRichEdit->MoveWindow(rcParam);

			// bottom
			int nPos = rcParam.top;
			pWnd = this->GetDlgItem(IDC_PROMTEDIT);
			rcParam.right  = rcClient.right;// - 100;
			rcParam.top    = rcParam.bottom + 1;
			rcParam.bottom = rcParam.top + 38;//70;
			pWnd->MoveWindow(rcParam);

			if( m_pErrorDlg )
			{
				m_pErrorDlg->MoveWindow(rcParam);
			}

			pWnd = this->GetDlgItem(IDC_TABOUT);
			rcParam.left   = rcParam.right + 5;
			rcParam.right  = rcParam.left + 90;
			//rcParam.top   -= 65;
			rcParam.bottom = rcParam.top  + 60;
			pWnd->MoveWindow(rcParam);


			/*
			// bottom
			int nPos = rcParam.top;
			pWnd = this->GetDlgItem(IDC_PROMTEDIT);
			rcParam.top    = rcParam.bottom + 5;
			rcParam.bottom = rcParam.top + 90;
			pWnd->MoveWindow(rcParam);

			if( m_pErrorDlg )
			{
				m_pErrorDlg->MoveWindow(rcParam);
			}

			pWnd = this->GetDlgItem(IDC_TABOUT);
			rcParam.left   = rcParam.right + 5;
			rcParam.right  = rcParam.left + 90;
			rcParam.top   -= 65;
			rcParam.bottom = rcParam.top  + 110;
			pWnd->MoveWindow(rcParam);

			// right
			pWnd = this->GetDlgItem(IDC_COMPILE);
			rcParam.top    = nPos;
			rcParam.bottom = rcParam.top   + 22;
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_INFUNCTION);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_BUTFUN);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_BUTEXP);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_NOTES);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_EIDOLON);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);


			pWnd = this->GetDlgItem(IDC_STOPPERIOD);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);

			pWnd = this->GetDlgItem(IDC_DEFAULT);
			rcParam.OffsetRect(0,24);
			pWnd->MoveWindow(rcParam);
			*/

		}
	}
}

void CDlgFormulaEdit::PostNcDestroy()
{
	//delete this;
}

//begin by HS 0328
void CDlgFormulaEdit::OnDefault()
{
	bool bStore = false ;
	CString strDefault = m_RDDefault.m_strSource ;
	m_RDDefault.Serialize(bStore);
	if(m_RDDefault.m_strSource == "")
	{
		m_RDDefault.m_strSource = strDefault;	
	}
	m_pRichEdit->GetRichEditCtrl().SetSel( 0, -1);
	m_pRichEdit->GetRichEditCtrl().ReplaceSel(m_RDDefault.m_strSource,TRUE);

	//m_pRichEdit->GetRichEditCtrl().Se(m_RDDefault.m_strSource);
}

void CDlgFormulaEdit::ReloadDefault::Serialize(bool bStore)
{
	CString strSection  = _T("公式管理");
	CString strFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) ;
	strFileName += "FomulaDefault.cfg";
	
	if( _access(strFileName, 0) != 0 )
	{
		FILE* fp = fopen(strFileName, "w");
		fclose(fp);
	}

	CString strCur = "?????";
	if (bStore)           //存
	{
		m_strSource = hxRelaceReturnKey(m_strSource,"\r\n",&strCur);

		::WritePrivateProfileString(strSection,
			m_strName,m_strSource,strFileName);
	}
	else                   //取
	{
		::GetPrivateProfileString(strSection,m_strName,
			"",m_strSource.GetBuffer(Fomula_Describe_Size),Fomula_Describe_Size, strFileName);
		m_strSource.ReleaseBuffer();
		m_strSource.TrimLeft();
		m_strSource.TrimRight();

		CString strOldCur = "\r\n";
		m_strSource = hxRelaceReturnKey(m_strSource,strCur,&strOldCur);
	}
}


//begin by HS 0329


HBRUSH CDlgFormulaEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CHSBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if( pWnd == (CWnd*)&m_wndPromte )
	{
		CRect rect;
		pWnd->GetClientRect(rect);
		pDC->SetTextColor(RGB(255,0,0));
//		pDC->SetBkColor(g_sDynSaveValue.m_crBackColor);
//		pDC->FillSolidRect(rect,g_sDynSaveValue.m_crBackColor);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
//end

void CDlgFormulaEdit::OnComBoxSelChange()
{
	// TODO: Add your control notification handler code here
	/*
	HTREEITEM hitem = this->m_hComboxItem;

	CDlgFormulaMan* pFomulaman = (CDlgFormulaMan*)GetParent();
	if( pFomulaman != NULL)
	{
		CTreeCtrlFormula* pHxTreeCtrl = pFomulaman->GetCurTree();
		if(pHxTreeCtrl != NULL)
		{
			int nOldIndex = this->m_wndComBox.FindString(
				-1,pHxTreeCtrl->GetItemText(this->m_hComboxItem));
			int nNowIndex = this->m_wndComBox.GetCurSel();
			for( int i = 0; i < fabs(nNowIndex - nOldIndex) ;i++)
			{
				if (nNowIndex > nOldIndex)
				{
					hitem = pHxTreeCtrl->GetNextSiblingItem(hitem);
				}
				else
				{
					hitem = pHxTreeCtrl->GetPrevSiblingItem(hitem);
				}


			}
		}
	}
	this->m_hComboxItem = hitem;
	*/
}

void CDlgFormulaEdit::OnTcnSelchangeTabout(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	/*CTabCtrl* pTab = (CTabCtrl*)this->GetDlgItem(IDC_TABOUT);
	if( pTab == NULL )
		return;

	int nIndex = pTab->GetCurSel();
	if( nIndex == -1 )
		return;*/

	ShowResult(-1);
	
}

void CDlgFormulaEdit::ShowResult(int nIndex)
{
	CTabCtrl* pTab = (CTabCtrl*)this->GetDlgItem(IDC_TABOUT);
	if( pTab == NULL )
		return;

	if( nIndex == -1 )
	{
		nIndex = pTab->GetCurSel();
	}
	else
	{
		int nOldIndex = pTab->GetCurSel();
		if( nOldIndex == nIndex )
			return;
	}

	if( nIndex == -1 )
		return;

	pTab->SetCurSel(nIndex);

	CWnd* pWnd;
	switch(nIndex)
	{
	case CompileResult: // 编译结果
		{
			pWnd = this->GetDlgItem(IDC_PROMTEDIT);
			if( pWnd == NULL )
				return;
			pWnd->ShowWindow(SW_HIDE);

			if( m_pErrorDlg == NULL )
				return;
			m_pErrorDlg->ShowWindow(SW_SHOW);
		}
		break;
	}
}

void CDlgFormulaEdit::OnNMClickTabout(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDlgFormulaEdit::OnSizing(UINT fwSide, LPRECT pRect)
{
	CHSBaseDialog::OnSizing(fwSide, pRect);

	if( (pRect->right - pRect->left ) < 517 )
	{
		pRect->right = pRect->left + 517;
	}

	if( (pRect->bottom - pRect->top ) < 359 )
	{
		pRect->bottom = pRect->top + 359;
	}
}
