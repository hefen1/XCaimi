// CompileEditView.cpp : implementation file
//

#include "stdafx.h"
#include "CompileEditView.h"

#include "InputPassPwD.h"
#include "..\..\Controls\YtControl\GridCtrl\GridCtrl.h"
#include "DlgEstopPeriod.h"
#include "TreeCtrlFormula.h"


#include "DlgFormulaEdit.h"
#include "DlgFormulaMan.h"
#include "DlgNotesAndEidolon.H"

#include "DlgError.h"
#include "DlgFormulaEdit.h"
#include "FunExpresion.h"
#include "OnlineList.h"
#include "VarValue.h"
#include "HSMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompileEditView

//#ifdef _AFXDLL
//AFX_EXTENSION_MODULE *  g_pExtensionModule = NULL;
//#endif

#define CMainFrame CDlgFormulaMan 

/*#define IDC_BUTTONADD   (WM_USER + 100)*/
#define IDC_BUTTONPOWER (WM_USER + 200)

/*
#ifdef _AFXDLL 
UINT HX_USER_COMPILEDATA = ::RegisterWindowMessage(HS_COMPILE_DATA);
UINT HX_USER_UPDATEDATA  = ::RegisterWindowMessage(HS_COMPILE_UPDATEDATA);
UINT HX_USER_DRAGDATA    = ::RegisterWindowMessage(HS_COMPILE_DRAGDATA);
#endif
*/
extern UINT g_SendReLoadTabItemMsg;
extern UINT g_ReLoadTabItem;
extern UINT g_SendDeleteTechIndexMsg;


extern CDlgFormulaMan* g_pFormulaMan;
extern HWND g_hParentWnd;
extern BOOL g_bIsExeFile;
BOOL		g_bExpressUpdate = TRUE;

CMenu		g_pSaveMenu;
BOOL		m_bCreateNew = TRUE;

CDlgFormulaEdit* CCompileEditView::m_pParentDlg = NULL;

CRichEditDocEx::CRichEditDocEx(CView* pView /*= NULL*/) 
:m_pView(pView) 
{
}

CRichEditDocEx::~CRichEditDocEx() 
{ 
}

CRichEditCntrItem* CRichEditDocEx::CreateClientItem(REOBJECT* preo /*= NULL*/) const
{
	return NULL;
}

IMPLEMENT_DYNCREATE(CCompileEditView, CRichEditView)

CCompileEditView::CCompileEditView()
{
	m_pListCtrl = NULL;
	m_pwndDlgMainBar = NULL;
	m_pVarGridCtrl = NULL;

	m_pPopupMenu   = NULL;
	m_nCompileEdit = COMPILE;

	m_pExternExpression = NULL;
	m_pExternVariabile  = NULL;
	m_pExternFunction   = NULL;

	m_hExpressInTreePos = NULL;

	m_pCurEditTreeGroup = NULL;

	m_pEstopPeriodDlg = NULL;	

// 	m_pDatatip = NULL;

	m_bArrowCur = FALSE;

	//
	m_pOnlineList = NULL;
	m_bInputChar  = FALSE;

	//
}

CCompileEditView::~CCompileEditView()
{	

	if(m_pDocument != NULL)
	{
		delete m_pDocument;
		m_pDocument = NULL;
	}
	if(m_pOnlineList != NULL)
	{
		delete m_pOnlineList;
		m_pOnlineList = NULL;
	}
	if(m_pPopupMenu != NULL)
	{
		m_pPopupMenu->DestroyMenu();
		delete m_pPopupMenu;
		m_pPopupMenu = NULL;
	}

	/*
	if( m_pExternExpression != NULL)
	{
		SaveLoadMap(TRUE,*m_pExternExpression);
	}
	*/
	/*
	if(g_bExpressUpdate)
	{
		m_pExternExpression = DeleteVarsMap(m_pExternExpression,g_bExpressUpdate);
		m_pExternVariabile  = DeleteVarsMap(m_pExternVariabile,g_bExpressUpdate);
		m_pExternFunction   = DeleteVarsMap(m_pExternFunction,g_bExpressUpdate);
	}
	*/

	if(m_pEstopPeriodDlg != NULL)
	{
		delete m_pEstopPeriodDlg;
		m_pEstopPeriodDlg = NULL;
	}

}


BEGIN_MESSAGE_MAP(CCompileEditView, CRichEditView)
	//{{AFX_MSG_MAP(CCompileEditView)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP

	ON_WM_MOUSEACTIVATE( )

	ON_COMMAND(IDC_NOTE, OnNote)
	ON_COMMAND(IDC_NUMEN, OnNumen)
	ON_COMMAND(IDC_BANPIERIODS, OnBanpieriods)
	ON_COMMAND(IDC_COMPILE, OnCompile)
	ON_COMMAND(IDC_OK, OnOk)
	ON_COMMAND(IDC_NEWEXPRESS, OnNewExpress)
	ON_COMMAND(IDC_EXPRESSOUT, OnExpressOut)
	ON_COMMAND(IDC_EXPRESSIN, OnExpressIn)
	ON_COMMAND(IDC_ENGTOCHINESE, OnEngToChinese)		

	ON_COMMAND_RANGE(IDC_BUTTONADD,IDC_BUTTONPOWER,OnSymbolMsg)

	ON_MESSAGE(HX_USER_COMPILE_INFO, AddCompileInfotoList)
	ON_MESSAGE(HX_USER_SENDADDSTR, OnSendAddStr)
	ON_MESSAGE(HX_USER_LOADCUR, LoadCurExp)
	ON_MESSAGE(HX_USER_DELETECUR, DeleteCurExp)
	
	ON_COMMAND_RANGE(SAVEMENU_BEGIN,SAVEMENU_END,SaveDataMenu)

	ON_COMMAND_RANGE(FunOper_QUOTES_Begin,FunOper_Constant_End,OnFunListMsg)

	ON_COMMAND(IDM_PROPERTY,OnProperty)

	ON_COMMAND(ID_EDIT_COPY,OnCopy)

	ON_COMMAND(IDC_EDITONLINEHELP,OnEditOnlineHelp)
//	ON_COMMAND(IDC_EDITRETURN,OnReturn)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompileEditView message handlers

int CCompileEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDocument = new CRichEditDocEx(this);

	Initialize();
	COLORREF color = RGB(255,255,255);
	GetRichEditCtrl().SetBackgroundColor(FALSE, color/*globalData.crEditBkClr*/);

	return 0;
}

BOOL CCompileEditView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	dwStyle |= ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN | 
               WS_BORDER | WS_VSCROLL | WS_HSCROLL;// | WS_TABSTOP | ES_OEMCONVERT;
	//dwStyle |= ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CCompileEditView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if( IsOnlineShow() )
	{
		m_pOnlineList->ShowWindow(SW_HIDE);
	}
	
	if(IsCompileEdit(COMPILE)) 
	{
		if(m_pPopupMenu == NULL)
		{
		/*	CResManager resmanager;*/

			m_pPopupMenu = new CMenu;
			if( !m_pPopupMenu->LoadMenu(IDR_EDITMENU) )
			{
				delete m_pPopupMenu;
				m_pPopupMenu = NULL;
			}
		}
		
		CMenu& pPopup = *m_pPopupMenu->GetSubMenu(0);
		if(pPopup.m_hMenu != NULL)
		{
			m_bArrowCur = TRUE;

			long nStartChar;
			long nEndChar;
			GetRichEditCtrl().GetSel( nStartChar, nEndChar );
			
			nEndChar = ((nStartChar != nEndChar)?MF_ENABLED:MF_DISABLED|MF_GRAYED); // selected
			
			pPopup.EnableMenuItem(ID_EDIT_CUT,MF_BYCOMMAND | nEndChar);
			pPopup.EnableMenuItem(ID_EDIT_COPY,MF_BYCOMMAND | nEndChar);

			ClientToScreen(&point);

			pPopup.TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
				point.x, point.y,this);
		}
	}

	CRichEditView::OnRButtonDown(nFlags, point);
}

void CCompileEditView::OnNote() 
{
	if(IsCompileEdit(COMPILE))
	{
	}
}

void CCompileEditView::OnNumen() 
{
	if(IsCompileEdit(COMPILE))
	{
	}
	// TODO: Add your command handler code here
	
}

void CCompileEditView::OnBanpieriods() 
{
	if(IsCompileEdit(COMPILE))
	{
		if(m_pEstopPeriodDlg == NULL)
		{
			m_pEstopPeriodDlg = new CDlgEstopPeriod(this);			
		}

		m_pEstopPeriodDlg->Empty();
		m_pEstopPeriodDlg->ShowWindow(SW_SHOW);
	}
}

void CCompileEditView::OnCompile()
{
	if(IsCompileEdit(COMPILE))
	{
		Compile(m_pParentDlg);
	}
}

long CCompileEditView::Compile(CDlgFormulaEdit* pDlg, LPARAM lParam /*= 0*/)
{
	if(m_pListCtrl != NULL)
	{ 
		m_pListCtrl->DeleteAllItems();
	}

	if( m_pParentDlg == NULL && pDlg != NULL )
	{
		m_pParentDlg = pDlg;
	}
	if( pDlg == NULL )
		return 0;

	this->SetFocus();

	CString strValue;
	GetRichEditCtrl().GetWindowText(strValue);
	if(strValue.IsEmpty()) 
		return -1;

	CString strName;
	CWnd* pNameEdit = pDlg->GetDlgItem(IDC_NAMEEDIT); // 名称
	if( pNameEdit != NULL )
	{
		pNameEdit->GetWindowText(strName);
	}

	if(strName.IsEmpty())
	{
		MessageBox(_T("请输入名称！"),_T("提示"),MB_ICONINFORMATION);
		if(pNameEdit != NULL)
		{
			pNameEdit->SetFocus();
		}
		return FALSE;
	}

	strName.TrimLeft();
	strName.TrimRight();
	strName.MakeUpper();
	CValue* pValoare = NULL;
	if( pDlg->IsType(CDlgFormulaEdit::Update) )
	{
		if( strName.CompareNoCase(pDlg->m_strOldName) &&
			m_pExternExpression->Lookup(strName,pDlg->m_nExpressType,pValoare) )
		{
			CString strPrompt;
			strPrompt.Format(_T("公式 \"%s\" 已经存在，请改名保存"),strName);
			MessageBox(strPrompt,_T("提示"),MB_ICONINFORMATION);
			pNameEdit->SetFocus();
			((CEdit*)pNameEdit)->SetSel(0,-1);
			return 0;
		}

		if( m_pExternExpression->Lookup(pDlg->m_strOldName,pDlg->m_nExpressType,pValoare) )
		{
		}
	}
	else if ( m_pExternExpression->Lookup(strName,pDlg->m_nExpressType,pValoare) )
	{
		if( pDlg->IsType(CDlgFormulaEdit::NewExp) )
		{
			CString strPrompt;
			strPrompt.Format(_T("公式 \"%s\" 已经存在，请改名保存"),strName);
			MessageBox(strPrompt,_T("提示"),MB_ICONINFORMATION);
			pNameEdit->SetFocus();
			((CEdit*)pNameEdit)->SetSel(0,-1);
			return 0;
		}
	}
	
	CExpression* pExpresie = new CExpression(m_pExternExpression,
		m_pExternVariabile,m_pExternFunction);

	if( pDlg)
	{
		pDlg->AddToParamerList(pExpresie);
	}

	//CWnd* pPromptWnd = m_pParentDlg->GetDlgItem(IDC_PROMTEDIT);
	//CWnd* pBtError = m_pParentDlg->GetDlgItem(IDC_ERROR);
	CWnd* pPromptWnd = m_pParentDlg->m_pErrorDlg;
	CString strErroText;

	nodErrorInfo info(0,0,0,strName);
	int nErrorCount = pExpresie->ChangeExpression(strValue);
	if( nErrorCount == 0 )
	{
		strErroText = _T("公式编译（测试）通过!");
		CTreeCtrlFormula* pHxTreeCtrl = NULL;	

		BOOL bAdd = FALSE;
		CExpValue* pValue = NULL;
		if(pValoare != NULL)
		{
			pValue = (CExpValue*)pValoare;
			CExpression* pOldExpress = pValue->GetExp();
			if(pOldExpress != NULL)
			{
				pExpresie->SetExpressType(pDlg->m_nExpressType);
				//qinhn 20090729 Add 保证在修改编译公式时，其"常用指标"的属性不变
				if (pOldExpress->IsStyle(HS_EXP_DEFAULT_TECH))
				{
					pExpresie->AddStyle(HS_EXP_DEFAULT_TECH);
				}
				//qinhn 20090729 Add End
			}
			pValue->Clean();

			if( pDlg->IsType(CDlgFormulaEdit::Update) &&
				strName.CompareNoCase(pDlg->m_strOldName) )
			{
				m_pExternExpression->RemoveKey(pDlg->m_strOldName,pDlg->m_nExpressType);
				bAdd = TRUE;
			}
		}
		else
		{
			pExpresie->SetExpressType(pDlg->m_nExpressType);
		}

		if(pValue == NULL)
		{
			pValue = new CExpValue;
			bAdd   = TRUE;
		}

		pValue->SetValue(pExpresie);
		pExpresie->SetName(strName);
		//qinhn 20090727 modify  设置类型，只编译保存则类型不变
		if (CDlgFormulaEdit::bNeedChangeStyle )
			pExpresie->AddStyle( ((!pDlg->m_nMainChart)?HS_EXP_MAINCHART:HS_EXP_SUBCHART) );
		else
			pExpresie->AddStyle(((!pDlg->m_CharType)?HS_EXP_MAINCHART:HS_EXP_SUBCHART));
		pExpresie->AddStyle( HS_EXP_OUTTYPE(pDlg->m_dStyle) );
		CDlgFormulaEdit::bNeedChangeStyle = false;
		//qinhn 20090717 modify End

		if( bAdd )
		{
			m_pExternExpression->AddExpress(strName,pValue);
		}

		CString strText;
		CWnd* pWnd = pDlg->GetDlgItem(IDC_DESCRIBEEDIT);    // 公式描述
		if(pWnd != NULL)
		{
			pWnd->GetWindowText(strText);
			pExpresie->SetDescribe(strText);
		}
		pWnd = pDlg->GetDlgItem(IDC_PWDEDIT);        // 密码
		if(pWnd != NULL && !(pWnd->GetStyle() & WS_DISABLED) )
		{
			pWnd->GetWindowText(strText);
			pExpresie->SetPWD(strText);
		}
		
		if(pDlg->m_pDlgNotes != NULL)  // 注释
		{
			strText = pDlg->m_pDlgNotes->GetText();
			pExpresie->SetNotes(strText); 
		}
		
		if( pDlg->m_pDlgEidolon )       // 参数精灵
		{
			strText = pDlg->m_pDlgEidolon->GetText();
			pExpresie->SetEidolon(strText);
		}

		if( m_pEstopPeriodDlg != NULL )
		{
			*pExpresie->GetEstopPeriod()   = m_pEstopPeriodDlg->GetEstopPeriod();   // 禁用周期
			*pExpresie->GetDefaultPeriod() = m_pEstopPeriodDlg->GetDefaultPeriod(); // 缺省周期
		}

		// 更新树
		if( pDlg->m_pFormulaMan != NULL )
		{
			pHxTreeCtrl = pDlg->m_pFormulaMan->GetCurTree((WORD)pDlg->m_nExpressType);
			if(pHxTreeCtrl != NULL)
			{
				//pExpresie->SetExpressType(pHxTreeCtrl->GetType());

				if(pValoare != NULL)
				{	
					//pHxTreeCtrl->UpdateTreeItemData(pValoare,pValue);
				}
				else
				{
					if(pDlg != NULL)
					{
						pDlg->SetTitle(pExpresie->GetExpressType(),strName);
					}

					pHxTreeCtrl->AddItem(strName,pValue,m_pExternExpression);

					if( pDlg->IsType(CDlgFormulaEdit::NewExp) )
					{
						pDlg->m_strOldName = strName;
						pExpresie->AddStyle(HS_EXP_USEREDIT);
						pDlg->SetType(CDlgFormulaEdit::Update | pDlg->IsType(CDlgFormulaEdit::SpeedNew));
					}
				}
			}
		}

		// 更新绘图窗口
		ExpPropery NameProp;
		NameProp.m_dExpType = pExpresie->GetExpressType();
		NameProp.m_strName.Format("%s", pExpresie->GetName());
		NameProp.m_strDescribe.Format("%s",  pExpresie->GetDescribe());
		NameProp.m_bStyle = pExpresie->GetStyle();
		if ( g_pFormulaMan )
			g_pFormulaMan->SendLinkMsg(DiagramMsg_FormulaParamChanged,(LPARAM)&NameProp);

		if(pPromptWnd != NULL)
		{
			pPromptWnd->SendMessage(HX_USER_COMPILE_INFO,0,0);
			//pPromptWnd->SetWindowText(strErroText);
		}

		m_pParentDlg->ShowResult(CDlgFormulaEdit::CompileResult);
		::SendMessage(g_hParentWnd,g_SendReLoadTabItemMsg,0,0); //qinhn 20090721 Add 重新载入T指标ab页，防止点修改公式对话框里面的"保存"后出错
		
		return (long)pExpresie;
	}
	else
	{
		/*strErroText = "公式有几个错误!";
		if(pBtError != NULL)
		{
			pBtError->EnableWindow(TRUE);
		}
		if(pPromptWnd != NULL)
		{
			pPromptWnd->SetWindowText(strErroText);
		}*/

		//AddCompileInfotoList(11,(LPARAM)&info);


		m_pParentDlg->ShowResult(CDlgFormulaEdit::CompileResult);

		delete pExpresie;
	}

	return 0;
}

void CCompileEditView::OnOk() 
{
	/*
	CString strValue;
	GetRichEditCtrl().GetWindowText(strValue);
	if(strValue.IsEmpty()) 
		return;

	if( !Compile() )
		return;
	SaveLoadMap(TRUE,*m_pExternExpression);

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		CHxCpLangView* pView = pMain->GetCpView();
		if(pView != NULL)
		{
			if( ::IsWindow(pView->m_wndDlgToolBar.m_hWnd) )
			{
				CWnd* pWnd = pView->m_wndDlgToolBar.GetDlgItem(IDC_OK);
				if(pWnd != NULL)
				{
					CRect rect;
					pWnd->GetWindowRect(rect);

					if(CreateSaveMenu())
					{
						CMenu& popup = *g_pSaveMenu.GetSubMenu(0);
						popup.TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
												   rect.left, rect.bottom,this);
					}
				}
			}
		}
	}
	*/
}

void CCompileEditView::OnSymbolMsg(UINT nID)
{
	if(IsCompileEdit(COMPILE))
	{
		CString strSymbol;
		switch(nID)
		{
		case IDC_BUTTONADD:     
			strSymbol = "+";
			break;
		case IDC_BUTTONADD+1:
			strSymbol = "-";
			break;
		case IDC_BUTTONADD+2:
			strSymbol = "*";
			break;
		case IDC_BUTTONADD+3:
			strSymbol = "/";
			break;
		case IDC_BUTTONADD+4:
			strSymbol = "(";
			break;
		case IDC_BUTTONADD+5:
			strSymbol = ")";
			break;
		case IDC_BUTTONADD+6:
			strSymbol = "=";
			break;
		case IDC_BUTTONADD+7:
			strSymbol = "!";
			break;
		case IDC_BUTTONADD+8:
			strSymbol = ".";
			break;
		case IDC_BUTTONADD+9:
			strSymbol = ":=";
			break;
		case IDC_BUTTONADD+10:
			strSymbol = ":";
			break;
		case IDC_BUTTONADD+11:
			strSymbol = "^";
			break;
		}
		CString strRep;
		strRep.Format(" %s ",strSymbol);
		this->GetRichEditCtrl().ReplaceSel(strRep,TRUE);
		this->SetFocus();
	}
}

long CCompileEditView::AddCompileInfotoList(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(m_pParentDlg == NULL)
		return 0;

	CWnd* pPromptWnd = m_pParentDlg->GetDlgItem(IDC_PROMTEDIT);
	if(pPromptWnd == NULL)
		return 0;

	CWnd* pBtError = m_pParentDlg->GetDlgItem(IDC_ERROR);

	nodErrorInfo* pError = (nodErrorInfo*)lParam;
	int nCount = GetErrorCount();
	CString strText;
	if(pError != NULL)
	{
		/*
		if(wParam == 10)
		{
		}
		else if(wParam == 11)
		{
		}
		else if(wParam == 12)
		{
		}
		else*/
		{			
			if(pBtError != NULL)
			{
				pBtError->EnableWindow(TRUE);
			}
			if(nCount == 0 && pError->m_nBegin >= 0 && pError->m_nEnd >= 0)
			{
				strText = _T("公式有几个错误!");
				pPromptWnd->SetWindowText(strText);
			}
		}		
		if(CDlgFormulaEdit::m_pErrorDlg != NULL)
		{
			CDlgFormulaEdit::m_pErrorDlg->SendMessage(HX_USER_COMPILE_INFO, wParam, lParam);
		}
	}
	else
	{
		strText = _T("公式编译（测试）通过!");
		if(pBtError != NULL)
		{
			pBtError->EnableWindow(FALSE);
		}
		pPromptWnd->SetWindowText(strText);
	}
	
	return 0;
	/*
	if(m_pParentDlg == NULL)
		return;

	CWnd* pPromptWnd = m_pParentDlg->GetDlgItem(IDC_PROMTEDIT);
	if(pPromptWnd != NULL)
	{
		CString strText;
		nodErrorInfo* pError = (nodErrorInfo*)lParam;
		int nCount = GetErrorCount();
		if(pError != NULL)
		{	
			if(wParam == 10)
			{
				strText.Format("%s",pError->GetError());
			}
			else if(wParam == 11)
			{
				strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount,0);
			}
			else if(wParam == 12)
			{
				strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount>0?nCount-1:nCount,0);
			}
			else
			{
				strText.Format("错误号:%i;错误信息:%s (%s)错误行列:%i-%i",pError->m_nError,
					hxRelaceReturnKey(pError->GetError(),"\r\n"),hxGetErrorInfoByErrorNum(pError->m_nError),pError->m_nBegin,pError->m_nEnd);
				
				if(nCount == 1 && pError->m_nBegin >= 0 && pError->m_nEnd >= 0)
				{
					this->GetRichEditCtrl().SetSel(pError->m_nBegin,pError->m_nEnd);
				}
			}
		}
		else
		{
			strText = "公式编译（测试）通过!";
		}

//		CString strOldText;
//		pPromptWnd->GetWindowText(strOldText);
		pPromptWnd->SetWindowText(strText);
	}
	*/


	/*
	if(m_pListCtrl == NULL)
		return;

	CString strText;
	short nImage;
	nodErrorInfo* pError = (nodErrorInfo*)lParam;
	int nCount = m_pListCtrl->GetItemCount();
	if(pError != NULL)
	{	
		if(wParam == 10)
		{
			nImage = 2;
			strText.Format("%s",pError->GetError());
		}
		else if(wParam == 11)
		{
			nImage = 2;
			strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount,0);
		}
		else if(wParam == 12)
		{
			nImage = 2;
			strText.Format("%s - %i 错误,%i 警告",pError->GetError(),nCount>0?nCount-1:nCount,0);
		}
		else
		{
			nImage = 0;
			strText.Format("错误号:%i;错误信息:%s (%s)错误行列:%i-%i",pError->m_nError,
				hxRelaceReturnKey(pError->GetError(),"\r\n"),hxGetErrorInfoByErrorNum(pError->m_nError),pError->m_nBegin,pError->m_nEnd);

			if(nCount == 0 && pError->m_nBegin >= 0 && pError->m_nEnd >= 0)
			{
				this->GetRichEditCtrl().SetSel(pError->m_nBegin,pError->m_nEnd);
			}
		}
	}
	else
	{
		strText = "公式编译（测试）通过!";
		nImage = 1;
	}	
	m_pListCtrl->InsertItem(nCount,strText,nImage);
	*/
}

void CCompileEditView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CRichEditView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CCompileEditView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( IsCompileEdit(COMPILE) )
	{
		switch(nChar)
		{
		case VK_F7:  // 编译
			this->OnCompile();
			break;
		case VK_F2:
			return;
		case VK_F3:
			return;
		case VK_F5:
			return;
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
		case VK_UP:
		case VK_DOWN:
			if( IsOnlineShow() )
			{
				m_pOnlineList->SetFocus();
				if(nChar != VK_RETURN)
				{
					m_pOnlineList->SendMessage(WM_KEYDOWN,nChar,MAKELONG(nRepCnt, nFlags));
				}
				return;
			}
			break;
		}
	}
	
	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCompileEditView::InitSysFunction() // 初始化已定义的函数
{
	if(g_bExpressUpdate)
	{
		::InitSysFunction(m_pExternFunction);
	}
}


long CCompileEditView::OnSendAddStr(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(lParam == NULL) 
		return 0;
		
	if( IsCompileEdit(COMPILE) )
	{
		HSAddStrCompileEdit* pData = (HSAddStrCompileEdit*)lParam;
		CString strRep;
		long nStartChar;
		long nEndChar;
		GetRichEditCtrl().GetSel( nStartChar, nEndChar );
		if(wParam == HX_EXPRESSSENTENCE)
		{
			CExpression* pExpression = (CExpression*)pData->m_strText;
			pData->m_strText = NULL;
						
			strRep.Format(_T("\n\n{ 插入%s公式体开始 }\n\n %s \n\n{ 插入%s公式体结束 }\n"),
				pExpression->GetName(),pExpression->GetSource(),pExpression->GetName());
			GetRichEditCtrl().SetSel (-1,-1); // 插入到最后
			GetRichEditCtrl().ReplaceSel(strRep,TRUE);

			if( m_pParentDlg->m_WndGridCtrl.GetSafeHwnd() )
			{
				//m_pParentDlg->m_WndGridCtrl.SetParamer(pExpression->GetParamVar(),TRUE);

				CMapVariabile* pVarsMap = pExpression->GetParamVar();
				int nRow = m_pParentDlg->m_WndGridCtrl.GetFixedRowCount();
				if ( pVarsMap ==NULL )
				{
					for(;nRow < m_pParentDlg->m_WndGridCtrl.GetRowCount(); nRow++)
					{
						m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,1,"");
						m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,2,"");
						m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,3,"");
						m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,4,"");
						m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,5,"");
					}
				}
				CNumericValue* pValue;
				CArray<class CValue*,class CValue*>* pArray = pVarsMap->GetObjArray();
				CString sName;
				CString strRetName;
				for(int nObj = 0; pArray != NULL && nObj < pArray->GetSize(); nObj++)
				{
					pValue = (CNumericValue*)pArray->GetAt(nObj);
					if( !pValue->IsStyle(HX_EXPRESSNUMBER) )
						continue;
					CString strValue;
					sName = pVarsMap->GetObjectName(pValue);			
					strRetName.Format("%s",sName);
					m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,1,strRetName);
					strValue.Format("%d", pValue->GetMinValue());
					m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,2,strValue);
					strValue.Format("%d", pValue->GetMaxValue());
					m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,3,strValue);
					strValue.Format("%d", pValue->GetValue());
					m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,4,strValue);
					strValue.Format("%d", pValue->GetTestStepValue());
					m_pParentDlg->m_WndGridCtrl.SetItemText(nRow,5,strValue);
		
					nRow++;
				}
				m_pParentDlg->m_WndGridCtrl.Invalidate();
			}
		}
		else
		{
			strRep.Format("%s",*pData->m_strText);
			GetRichEditCtrl().ReplaceSel(strRep,FALSE);
			GetRichEditCtrl().SetSel( nStartChar+pData->m_nCharPos, nStartChar+pData->m_nCharPos );
		}		
		SetFocus();
	}
	else if( IsCompileEdit(HELP) )
	{
		CString* str = (CString*)lParam;

		GetRichEditCtrl().SetWindowText(*str);
		int nCount = GetRichEditCtrl().GetLineCount();
		CDC* pDC = GetRichEditCtrl().GetDC();
		if(pDC != NULL)
		{
			nCount = pDC->GetTextExtent("A").cy * (nCount + 1);
			GetRichEditCtrl().ReleaseDC(pDC);
		}

		CSplitterWnd* pWnd = (CSplitterWnd*)this->GetParent();
		if( pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)) )
		{	
			CRect rect;
			pWnd->GetClientRect(rect);
			int nHeight = rect.Height() - nCount;
			if(nHeight < 0) nHeight = rect.Height();
			pWnd->SetRowInfo(0,nHeight,0);
			pWnd->SetRowInfo(1,nCount,0);
			pWnd->RecalcLayout();
		}
	}
	return 0;
}

void CCompileEditView::Initialize() 
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof(PARAFORMAT);
	pf.dwMask = PFM_TABSTOPS ;
	pf.cTabCount = MAX_TAB_STOPS;
	for( int itab = 0 ; itab < pf.cTabCount  ; itab++ )
		pf.rgxTabs[itab] = (itab + 1) * 1440/5 ;

	SetParaFormat( pf );

	CHARFORMAT2A cfDefault;
	GetRichEditCtrl().GetDefaultCharFormat( cfDefault );
	
	//cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_BOLD | CFE_PROTECTED; 
	cfDefault.dwMask = CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT |
		CFM_PROTECTED | CFM_SIZE | CFM_FACE | CFM_CHARSET;
		
		//CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED ;
	//cfDefault.yHeight = 200;
	cfDefault.bCharSet = GB2312_CHARSET;//GB2312_CHARSET|ANSI_CHARSET|CHINESEBIG5_CHARSET|SYMBOL_CHARSET|DEFAULT_CHARSET|SYMBOL_CHARSET|OEM_CHARSET;//0xEE; 
	strcpy(cfDefault.szFaceName, _T("宋体"));  // Courier New // 宋体

	this->SetCharFormat(cfDefault);

	GetRichEditCtrl().SetDefaultCharFormat(cfDefault);
	GetRichEditCtrl().SetWordCharFormat(cfDefault);	

	GetRichEditCtrl().SetEventMask(ENM_CHANGE | ENM_SELCHANGE | ENM_PROTECTED);
}

void CCompileEditView::SetCompileEdit(int nCompileEdit)
{
	m_nCompileEdit = nCompileEdit;	
}

void CCompileEditView::OnFunListMsg(UINT nID)
{
	if( m_pPopupMenu == NULL || 
		m_pPopupMenu->m_hMenu == NULL ) 
		return;

	CMenu& pPopupMenu = *m_pPopupMenu->GetSubMenu(0);	
	if(pPopupMenu.m_hMenu == NULL)
		return;

	CMenu& pFunMenu = *pPopupMenu.GetSubMenu(0);
	if(pFunMenu.m_hMenu == NULL)
		return;

	CString strText;
	pFunMenu.GetMenuString(nID,strText,MF_BYCOMMAND);

	long nStartChar;
	long nEndChar;
	this->GetRichEditCtrl().GetSel( nStartChar, nEndChar );

	CString strRep;
	strRep.Format(" %s ",strText);
	this->GetRichEditCtrl().ReplaceSel(strRep,TRUE);
	int nIndex = strRep.Find("(");
	if( nIndex != -1 )
	{
		nIndex += 2;
		this->GetRichEditCtrl().SetSel( nStartChar+nIndex, nStartChar+nIndex);
	}
}

BOOL CCompileEditView::SaveLoadMap(BOOL bSave,CMapVariabile& ayVarMap,CString strFileName /*= ""*/)
{
	/*
	CComboBox* pBox = NULL;
	if( !bSave && m_pwndDlgMainBar != NULL )  // 公式名称加入到combox
	{
		pBox = (CComboBox*)m_pwndDlgMainBar->GetDlgItem(IDC_NAMECOMBO);
	}
	*/

	BOOL bRet = ::SaveLoadMap(bSave,m_pExternExpression,
						   	  m_pExternVariabile,m_pExternFunction,NULL,NULL);

	if( !bSave )
	{
		::SendMessage( g_hParentWnd,HX_USER_COMPILEDATA,10000,0);
	}

	return bRet;
}

void CCompileEditView::Init(short nType)
{	
	if(g_bExpressUpdate)
	{
		SaveLoadMap(FALSE,*m_pExternExpression); // 装入公式
	}
	else if(m_pExternExpression != NULL)
	{
		CComboBox* pBox = NULL;
		if(m_pwndDlgMainBar != NULL)  // 公式名称加入到combox
		{
			pBox = (CComboBox*)m_pwndDlgMainBar->GetDlgItem(IDC_NAMECOMBO);
		}
		POSITION pos = m_pExternExpression->GetStartPosition();
		while (pos != NULL)
		{
			CString sName;
			CValue *pVal;
			m_pExternExpression->GetNextAssoc (pos, sName, pVal);
			if(pVal != NULL)
			{
				if(pBox != NULL)
				{
					pBox->AddString(sName);
				}
			}
		}
	}


	// 设定关键字及常量
	SetKeywordsConstants();	

}

long CCompileEditView::DeleteCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	HSTreeToCompileEdit* pppData = (HSTreeToCompileEdit*)lParam;
	if(pppData == NULL)
		return 0;

	CTreeGroup* pTreeGroup = (CTreeGroup*)pppData->m_pName;
	if(pTreeGroup != NULL)
	{
		CExpression* pExpress = pTreeGroup->GetData();
		if(pExpress != NULL)
		{
			CString strName;
			strName.Format("%s",pExpress->GetName());

			CValue* pValoare;
			if ( m_pExternExpression->Lookup(strName,pExpress->GetExpressType(),pValoare) )
			{
				if( !pExpress->GetPWD().IsEmpty() ) // 有密码
				{
					CInputPassPwD dlg(this,_T("请输入公式密码"),strName);
					dlg.m_pExp = pExpress;
					if( dlg.DoModal() == IDCANCEL )
					{
						return 2;
					}
				}
				if( wParam )
				{
					m_pExternExpression->RemoveKey(strName,pExpress->GetExpressType());
					delete pValoare;
				}
			}

			if(m_pwndDlgMainBar != NULL)
			{
				CComboBox* pCombox = (CComboBox*)m_pwndDlgMainBar->GetDlgItem(IDC_NAMECOMBO);
				if(pCombox != NULL)
				{
					int nIndex = pCombox->FindString( 0, strName );
					if(nIndex != -1)
					{
						pCombox->DeleteString( nIndex );
					}
				}
			}
		}
	}
	else
	{
	}

	if(m_hExpressInTreePos == (HTREEITEM)pppData->m_pData)
	{
		OnNewExpress();
	}

	return 0;
}

long CCompileEditView::LoadCurExp(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	return 0;
}

void CCompileEditView::InitData()
{
	if( m_pExternExpression != NULL ||
		m_pExternVariabile  != NULL || 
		m_pExternFunction   != NULL )
		return;
		
	CArray<CMapVariabile*,CMapVariabile*>* ayData = NULL;
	if( g_hParentWnd && 
		(ayData = (CArray<CMapVariabile*,CMapVariabile*>*)::SendMessage( g_hParentWnd,HX_USER_COMPILEDATA,0,0)) != NULL )
	{
		if( ayData != NULL && ayData->GetSize() >= 3 ) 
		{
			m_pExternExpression = ayData->GetAt(0);  // 外部表达式表
			m_pExternVariabile  = ayData->GetAt(1);	 // 外部变量表
			m_pExternFunction   = ayData->GetAt(2);	 // 外部函数表			

			::SetHxDataSource( (CHSDataSource*)ayData->GetAt(3),
				(CArray<ColorProperty*,ColorProperty*>*)ayData->GetAt(4) );
		}
	}
	else // error!!!????
	{
		m_pExternExpression = new CMapVariabile;     // 外部表达式表
		m_pExternVariabile  = new CMapVariabile;	 // 外部变量表
		m_pExternFunction   = new CMapVariabile;	 // 外部函数表	
		
	}

	InitSysFunction();

//	m_pExternExpression = DeleteVarsMap(m_pExternExpression,g_bIsExeFile);
//	m_pExternVariabile  = DeleteVarsMap(m_pExternVariabile,g_bIsExeFile);
//	m_pExternFunction   = DeleteVarsMap(m_pExternFunction,g_bIsExeFile);

}

void CCompileEditView::OnNewExpress()
{
	/*
	if(m_pwndDlgMainBar == NULL)
		return;

	m_hExpressInTreePos = NULL;
	m_pCurEditTreeGroup = NULL;
	m_bCreateNew = TRUE;
		
	GetRichEditCtrl().SetWindowText(""); // 公式体

	CWnd* pCombox = m_pwndDlgMainBar->GetDlgItem(IDC_NAMECOMBO); // 名称
	if(pCombox != NULL)
	{
		pCombox->SetWindowText("");
		pCombox->SetFocus();		
	}
	CWnd* pWnd = m_pwndDlgMainBar->GetDlgItem(IDC_PWDEDIT); // 密码		
	if(pWnd != NULL)
	{
		pWnd->SetWindowText("");
		pWnd->EnableWindow(FALSE);
	}
	pWnd = m_pwndDlgMainBar->GetDlgItem(IDC_PWDCHECK);
	if(pWnd != NULL)
	{
		((CButton*)pWnd)->SetCheck(0);
	}
	pWnd = m_pwndDlgMainBar->GetDlgItem(IDC_NAMEEXPRESSION); // 公式描述
	if(pWnd != NULL)
	{
		pWnd->SetWindowText("");
	}

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if( pMain->m_wndNotesEditBar )       // 注释
	{
		pMain->m_wndNotesEditBar.GetEdit()->SetWindowText("");
	}
	
	if( pMain->m_wndEidolonEditBar )       // 参数精灵
	{
		pMain->m_wndEidolonEditBar.GetEdit()->SetWindowText("");
	}
	if(m_pVarGridCtrl != NULL)
	{
		m_pVarGridCtrl->SetParamer(NULL);
	}
	
	if(m_pEstopPeriodDlg != NULL)
	{
		m_pEstopPeriodDlg->Empty();
	}
	*/
}

void CCompileEditView::SetKeywordsConstants()
{
	if( m_pExternFunction == NULL ) 
		return;

	CString strKey;
	CString strCur;

	POSITION pos = m_pExternFunction->GetStartPosition();
	CString	sName;
	CValue* pVal;
	while (pos != NULL)
	{
		m_pExternFunction->GetNextAssoc(pos, sName, pVal);
		if( ((CFunExpresion*)pVal)->GetFunType() )
		{
			strCur.Format(" %s ",sName);
			strKey += strCur;
		}
	}

	AddKeywords(strKey);
//	AddConstants(str);
}

void CCompileEditView::SaveDataMenu(UINT nID)
{
	if(g_pSaveMenu.m_hMenu == NULL)
		return;

	CString strComm;
	g_pSaveMenu.GetMenuString(nID,strComm,MF_BYCOMMAND);
	if(strComm.IsEmpty())
		return;

	int nPos = strComm.Find(":");
	if( nPos != -1 )
	{
		strComm = strComm.Mid(nPos+1);
	}
	if(strComm.IsEmpty())
		return;

	if(!IsCompileEdit(COMPILE))
		return;

	CTreeCtrlFormula* pHxTreeCtrl = NULL;

	HTREEITEM hFloatTreeItem = NULL;
	CTreeCtrlFormula* pFloatTree = NULL;
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		if(pMain->GetIndividualInfo() != NULL)
		{
			pFloatTree = pMain->GetIndividualInfo();
		}
	}

	HTREEITEM hTreeItem = NULL;
	char	  cType = 0;
	
	nID -= SAVEMENU_BEGIN;
	if(nID >= 0 && nID < 100)
	{
		cType = Tech;
		if(pHxTreeCtrl != NULL)
		{				
			hTreeItem = pHxTreeCtrl->GetRootTree(CTreeCtrlFormula::g_strTech);//		 = "技术指标";
			hTreeItem = pHxTreeCtrl->GetTreeItem(strComm,hTreeItem);
		}
		if(pFloatTree != NULL)
		{
			hFloatTreeItem = pFloatTree->GetRootTree(CTreeCtrlFormula::g_strTech);//		 = "技术指标";
			hFloatTreeItem = pFloatTree->GetTreeItem(strComm,hFloatTreeItem);
		}
	}
	if( nID >= 100 && nID < 200 )
	{
		cType = Condition;
		if(pHxTreeCtrl != NULL)
		{
			hTreeItem = pHxTreeCtrl->GetRootTree(CTreeCtrlFormula::g_strCondition);//		 = "技术指标";				
			hTreeItem = pHxTreeCtrl->GetTreeItem(strComm,hTreeItem);
		}
		if(pFloatTree != NULL)
		{
			hFloatTreeItem = pFloatTree->GetRootTree(CTreeCtrlFormula::g_strCondition);//		 = "技术指标";
			hFloatTreeItem = pFloatTree->GetTreeItem(strComm,hFloatTreeItem);
		}
	}
	if( nID >= 200 && nID < 300 )
	{
		cType = Exchange;
		if(pHxTreeCtrl != NULL)
		{
			hTreeItem = pHxTreeCtrl->GetRootTree(CTreeCtrlFormula::g_strExchange);//		 = "技术指标";				
			hTreeItem = pHxTreeCtrl->GetTreeItem(strComm,hTreeItem);
		}
		if(pFloatTree != NULL)
		{
			hFloatTreeItem = pFloatTree->GetRootTree(CTreeCtrlFormula::g_strExchange);//		 = "技术指标";
			hFloatTreeItem = pFloatTree->GetTreeItem(strComm,hFloatTreeItem);
		}
	}
	if( nID >= 300 && nID < 400 )
	{
		cType = MoreKLine;
		if(pHxTreeCtrl != NULL)
		{
			hTreeItem = pHxTreeCtrl->GetRootTree(CTreeCtrlFormula::g_strColorKLine);//		 = "技术指标";				
			hTreeItem = pHxTreeCtrl->GetTreeItem(strComm,hTreeItem);
		}
		if(pFloatTree != NULL)
		{
			hFloatTreeItem = pFloatTree->GetRootTree(CTreeCtrlFormula::g_strColorKLine);//		 = "技术指标";
			hFloatTreeItem = pFloatTree->GetTreeItem(strComm,hFloatTreeItem);
		}
	}		
	if( nID >= 400 && nID < 500 )
	{
		cType = ExpressBin;
		if(pHxTreeCtrl != NULL)
		{
			hTreeItem = pHxTreeCtrl->GetRootTree(CTreeCtrlFormula::g_strRecycleBin);//		 = "技术指标";				
			hTreeItem = pHxTreeCtrl->GetTreeItem(strComm,hTreeItem);
		}
		if(pFloatTree != NULL)
		{
			hFloatTreeItem = pFloatTree->GetRootTree(CTreeCtrlFormula::g_strRecycleBin);//		 = "技术指标";
			hFloatTreeItem = pFloatTree->GetTreeItem(strComm,hFloatTreeItem);
		}
	}

	CString strName;
	CComboBox* pBox = NULL;
	if(m_pwndDlgMainBar != NULL)
	{
		pBox = (CComboBox*)m_pwndDlgMainBar->GetDlgItem(IDC_NAMECOMBO);
		if(pBox != NULL)
		{			
			pBox->GetWindowText(strName);
			strName.MakeUpper();
			if( pBox->FindString(0,strName) != CB_ERR )
			{
				CValue* valoare;
				if ( m_pExternExpression->Lookup(strName,valoare) ) // 外部表达式表
				{
				}
			}
			else
			{
				pBox->AddString(strName);
			}
		}
	}
	

	if( pHxTreeCtrl != NULL && hTreeItem != NULL && !strName.IsEmpty())
	{
		m_hExpressInTreePos = pHxTreeCtrl->AddItem(strName,cType,hTreeItem/*pHxTreeCtrl->GetCurSavePos(hTreeItem)*/,m_pExternExpression);
	}

	// 浮动窗口	
	if( pFloatTree != NULL && hFloatTreeItem != NULL && !strName.IsEmpty())
	{
		pFloatTree->AddItem(strName,cType,hFloatTreeItem/*pFloatTree->GetCurSavePos(hFloatTreeItem)*/,m_pExternExpression);
	}

	::SendMessage( g_hParentWnd,HX_USER_UPDATEDATA,0,(LPARAM)&strName );
}

void CCompileEditView::OnExpressOut()
{
}

void CCompileEditView::OnExpressIn()
{
}

void CCompileEditView::OnEngToChinese()
{	
}

BOOL CCompileEditView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	return 0;
}

BOOL CCompileEditView::CreateSaveMenu()
{
	if(g_pSaveMenu.m_hMenu != NULL)
		g_pSaveMenu.DestroyMenu();
	
	if( !g_pSaveMenu.LoadMenu(IDR_SAVEMENU) )
		return FALSE;

	CTreeCtrlFormula* pTree = NULL;

	if( pTree == NULL )
		return FALSE;
	
	CMenu& popup = *g_pSaveMenu.GetSubMenu(0);
	if( popup.m_hMenu == NULL )
		return FALSE;

	popup.RemoveMenu(0,MF_BYPOSITION);
	pTree->AddTreeGroupToMenu(SAVEMENU_BEGIN,popup);

	return TRUE;
}

int CCompileEditView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
	if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
		return nResult;   // frame does not want to activate
/*
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame != NULL)
	{
		// eat it if this will cause activation
		ASSERT(pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame));

		// either re-activate the current view, or set this view to be active
		CView* pView = pParentFrame->GetActiveView();
		HWND hWndFocus = ::GetFocus();
		if (pView == this &&
			m_hWnd != hWndFocus && !::IsChild(m_hWnd, hWndFocus))
		{
			// re-activate this view
			OnActivateView(TRUE, this, this);
		}
		else
		{
			// activate this view
			pParentFrame->SetActiveView(this);
		}
	}
*/
	return nResult;
}

void CCompileEditView::OnProperty()
{
	
}

void CCompileEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRichEditView::OnMouseMove(nFlags, point);

	if ( !nFlags && IsDataTip() )
	{
		CString strTitle;
		CString strText = GetTipText(strTitle,nFlags,point);
		if(!strText.IsEmpty())
		{
			//m_pDatatip->On(TRUE);
			//ClientToScreen(&point);
			//if (::GetCursorPos(&point))
			//{
				//m_pDatatip->SetOffset(8,8);
			/*	m_pDatatip->Set(point,strTitle,strText,30,5);*/
		/*	m_pDatatip.Show(strTitle,strText);*/
			//}
		}
		else
		{
			//m_pDatatip->Hide();
		}
	}
	/*
	CWnd* pWnd = GetFocus();
	if(pWnd != NULL && pWnd != m_pDatatip)
	{
		this->SetFocus();
	}
	*/
}

CString CCompileEditView::GetTipText(CString& strTitle,UINT nFlags, CPoint point,
									 int* nBegin /*= NULL*/,int* nEnd /*= NULL*/) 
{
	return _T("没有帮助!");
}

CString	CCompileEditView::GetKeyHelp(CString strKey)
{
	CString strRet;
	strKey.MakeUpper();
	CValue* valoare;
	if ( m_pExternFunction != NULL && 
		 m_pExternFunction->Lookup(strKey,valoare) )
	{
		CFunExpresion* pFun = (CFunExpresion*)valoare;
		strRet.Format("%s",pFun->GetNotes());
	}

//	else if ( m_pExternExpression != NULL && 
//			  m_pExternExpression->Lookup(strKey,valoare) )
//	{
//		CExpression* pExpresie = (CExpression*)valoare;
//		strRet.Format("%s",pExpresie->GetNotes());
//	}
	
	return strRet;
}

void CCompileEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pOnlineList != NULL)
	{
		if( m_pOnlineList->IsWindowVisible() )
		{
			m_pOnlineList->ShowWindow(SW_HIDE);
		}
	}
	
	//
	m_bArrowCur = FALSE;

	/*if( m_pDatatip != NULL &&
		m_pDatatip->m_hWnd)
	{
		m_pDatatip->Hide( );
	}*/
	CRichEditView::OnLButtonDown(nFlags, point);
}

void CCompileEditView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	/*if( m_pDatatip != NULL &&
		m_pDatatip->m_hWnd )
	{
		m_pDatatip->Hide( );
	}*/
	CRichEditView::OnLButtonUp(nFlags, point);
}

void CCompileEditView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRichEditView::OnLButtonDblClk(nFlags, point);
}

void CCompileEditView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pOnlineList != NULL)
	{
		if( m_pOnlineList->IsWindowVisible() )
		{
			m_pOnlineList->ShowWindow(SW_HIDE);
		}
	}

	/*if( m_pDatatip != NULL &&
		m_pDatatip->m_hWnd )
	{
		m_pDatatip->Hide( );
	}*/
	CRichEditView::OnRButtonUp(nFlags, point);
}

void CCompileEditView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(m_pOnlineList != NULL)
	{
		if( m_pOnlineList->IsWindowVisible() )
		{
			m_pOnlineList->ShowWindow(SW_HIDE);
		}
	}

	/*if( m_pDatatip != NULL &&
		m_pDatatip->m_hWnd)
	{
		m_pDatatip->Hide( );
	}*/
	CRichEditView::OnRButtonDblClk(nFlags, point);
}

CString	CCompileEditView::FindWord(CString strText,int& nStart,int nCount)
{
	if( strText.IsEmpty() || nStart > nCount )
		return "";

	CString strLeft  = strText.Left(nStart);
	CString strRight = strText.Mid(nStart);

	strLeft  = CutWord(strLeft,TRUE);
	nStart -= strLeft.GetLength();
	strRight = CutWord(strRight,FALSE);

	return strLeft+strRight;
}

CString	CCompileEditView::CutWord(CString strText,BOOL bLeft)
{
	int nBegin,nEnd;
	int nPos;
	CString strRet;
	if(bLeft)
	{
		nEnd   = strText.GetLength();
		nBegin = nEnd - 1;
		nEnd = 0;
		while ( (nBegin >= 0) &&
				((nPos = IsChinese(strText[nBegin],strText,nBegin,TRUE)) ||
				  isalnum(strText[nBegin])) )
		{
			if( nPos == 2 )
				break;
			nBegin--;
			nEnd++;
		}
		strRet = strText.Right(nEnd);
	}
	else
	{
		nBegin   = 0;
		nEnd = strText.GetLength();
		while ( (nBegin < nEnd) &&
			    ((nPos = IsChinese(strText[nBegin],strText,nBegin,TRUE)) ||
			    isalnum(strText[nBegin]) || 
				(strText[nBegin] == '.') )  )
		{
			if( nPos == 2 )
				break;
			nBegin++;
		}
		if( nBegin < 0 ) nBegin = 0;
		strRet = strText.Mid(0,nBegin);
	}
	
	return strRet;
}

void CCompileEditView::HideDataTip()
{

/*	m_pDatatip.Hide();*/
}

void CCompileEditView::OnCopy()
{
	GetRichEditCtrl().Copy();
}

BOOL CCompileEditView::IsDataTip()
{


	return TRUE;
}

BOOL CCompileEditView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bArrowCur) 
		::SetCursor(AfxGetApp()->LoadStandardCursor (IDC_ARROW));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
	
	return TRUE;
}

COnlineList* CCompileEditView::CreateOnlineList()
{
	if(m_pOnlineList == NULL)
	{
		m_pOnlineList = new COnlineList;

		if ( !m_pOnlineList->CWnd::CreateEx( /*WS_EX_TOPMOST|*/WS_EX_DLGMODALFRAME,WC_LISTVIEW,NULL,
			WS_POPUP|WS_BORDER|LVS_REPORT|LVS_SINGLESEL|LVS_SORTASCENDING|LVS_NOSORTHEADER, 
			0,0,200,170,this->GetRichEditCtrl().m_hWnd,NULL,NULL ) )
		{
			return 0;
		}
	}
	return m_pOnlineList;
}

void CCompileEditView::ShowOnlineList(CPoint& point,int nCmdShow,CString strText /*= ""*/,
									  BOOL bEdit /*= FALSE*/,
									  int nBegin,int nEnd)
{
	COnlineList* pOnline = CreateOnlineList();
	if(pOnline == NULL) return;

	if( !bEdit )
	{
		CString strTitle;
		strText = GetTipText(strTitle,MK_RBUTTON,point,&nBegin,&nEnd);
	}

	if( pOnline->AddToOnlineList(strText,bEdit,nBegin,nEnd) )
	{
		CRect rect,rc;
		pOnline->GetWindowRect(rect);
		
		ClientToScreen(&point);
		rc.left = point.x;
		rc.top  = point.y;
		rc.right = rc.left + rect.Width();
		rc.bottom = rc.top + rect.Height();
		pOnline->MoveWindow(rc);
		
		if( !pOnline->IsWindowVisible() )
		{
			pOnline->ShowWindow(nCmdShow);
		}
	}
	else
	{
		if( pOnline->IsWindowVisible() )
		{
			pOnline->ShowWindow(SW_HIDE);
		}
	}
}

BOOL CCompileEditView::IsOnlineShow()
{
    if(m_pOnlineList != NULL)
	{
		return m_pOnlineList->IsWindowVisible();
	}
	return FALSE;
}

BOOL CCompileEditView::OnReturn(MSG* pMsg)
{
	if( IsOnlineShow() )
	{
		if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			m_pOnlineList->SetFocus();
			m_pOnlineList->SendMessage(WM_KEYDOWN,VK_RETURN,0);
			return TRUE;
		}
	}
	return FALSE;
}

void CCompileEditView::OnEditOnlineHelp()
{
	CPoint point = GetRichEditCtrl().GetCaretPos() ;

	CClientDC dc((CWnd*)&GetRichEditCtrl());
	CSize size = dc.GetTextExtent("A");
	point.y += size.cy;


	ShowOnlineList(point,SW_SHOW);
}

void CCompileEditView::OnEdit(int nBegin,int nEnd,CString strText,BOOL bEdit /*= FALSE*/)
{
	CPoint point = GetRichEditCtrl().GetCaretPos();
	
	CClientDC dc((CWnd*)&GetRichEditCtrl());
	CSize size = dc.GetTextExtent("A");
	point.y += size.cy;

	ShowOnlineList(point,SW_SHOW,strText,bEdit,nBegin,nEnd);	
}

void CCompileEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if( m_pParentDlg )
	{
		m_pParentDlg->ShowResult(CDlgFormulaEdit::Translate);
	}

	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_ESCAPE:
	case VK_RETURN:
	case VK_BACK:
		if( !IsOnlineShow() )
		{
			break;
		}
	default:
		m_bInputChar = TRUE;
		break;
	}

	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CCompileEditView::ShowOnline(int nCmd,BOOL bForce /*= FALSE*/)
{
	if(m_pOnlineList != NULL)
	{
		if( bForce )
		{
			m_pOnlineList->ShowWindow(nCmd);
		}
		else if(nCmd == SW_HIDE && IsOnlineShow() )
		{
			CPoint pt;
			GetCursorPos(&pt);
			CRect rect;
			m_pOnlineList->GetWindowRect(rect);
			if( !rect.PtInRect(pt) )
			{
				m_pOnlineList->ShowWindow(nCmd);
			}
			m_bInputChar = FALSE;
		}
	}
}

HRESULT CCompileEditView::QueryAcceptData( LPDATAOBJECT lpdataobj, CLIPFORMAT FAR * lpcfFormat,
									 DWORD dwReco, BOOL bReally, HGLOBAL hMetaFile )
{
	/*
	ASSERT(lpcfFormat != NULL);
	if (!bReally) // not actually pasting
		return S_OK;
	// if direct pasting a particular native format allow it
	if (IsRichEditFormat(*lpcfFormat))
		return S_OK;

	COleDataObject dataobj;
	dataobj.Attach(lpdataobj, FALSE);
	// if format is 0, then force particular formats if available
	if (*lpcfFormat == 0 && (m_nPasteType == 0))
	{
		if (dataobj.IsDataAvailable((CLIPFORMAT)_oleData.cfRichTextAndObjects)) // native avail, let richedit do as it wants
			return S_OK;
		else if (dataobj.IsDataAvailable((CLIPFORMAT)_oleData.cfRichTextFormat))
		{
			*lpcfFormat = (CLIPFORMAT)_oleData.cfRichTextFormat;
			return S_OK;
		}
		else if (dataobj.IsDataAvailable(CF_TEXT))
		{
			*lpcfFormat = CF_TEXT;
			return S_OK;
		}
	}
	// paste OLE formats

	DoPaste(dataobj, *lpcfFormat, hMetaPict);
	return S_FALSE;
	*/
	OnEditProperties();
	return S_FALSE;
}
