// DlgFunctionMan.cpp : implementation file
//

#include "stdafx.h"
#include "DlgFunctionMan.h"

#include "TreeCtrlFormula.h"
#include "DlgFormulaEdit.h"
#include "CompileEditView.h"
#include "FunExpresion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctionMan dialog


CDlgFunctionMan::CDlgFunctionMan(CWnd* pParent /*=NULL*/,CMapVariabile*  pExternFunction /*= NULL*/,
						   CMapVariabile*  pExternExpression /*= NULL*/,
						   long*  pThis /*= NULL*/)
	: CDialog(CDlgFunctionMan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFunctionMan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	/*m_pThis = pThis;*/

	m_pParent = (CDlgFormulaEdit*)pParent;
	m_pExternFunction   = pExternFunction;		 // 外部函数表
	m_pExternExpression = pExternExpression;     // 外部表达式表
	
	//m_bGripper = FALSE;

	//m_pWnd = this;
	Create(CDlgFunctionMan::IDD,pParent);

}


void CDlgFunctionMan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDlgFunctionMan)
	DDX_Control(pDX, IDD_NOTES, m_wndNotes);
	DDX_Control(pDX, IDD_TYPE, m_wndTypeList);
	DDX_Control(pDX, IDD_FUNLIST, m_wndListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFunctionMan, CDialog)
	//{{AFX_MSG_MAP(CDlgFunctionMan)
	ON_BN_CLICKED(IDD_NEW, OnNew)
	ON_BN_CLICKED(IDD_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_NOTIFY(LVN_ITEMCHANGED, IDD_TYPE, OnItemchangedType)
	ON_NOTIFY(LVN_ITEMCHANGED, IDD_FUNLIST, OnItemchangedFunlist)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	
	ON_MESSAGE(USERDEF_UPDATEDATA,OnUserDefUpdateData)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFunctionMan message handlers

BOOL CDlgFunctionMan::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wndTypeList.Init();

	int i;
	for(i = 0; i < 12;i++)
	{
		m_wndTypeList.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									  i,CTreeCtrlFormula::g_strFunName[i].m_strName+_T("函数"),
									  0,0,i,0);
		//m_wndTypeList.InsertItem(i,CTreeCtrlFormula::g_strFunName[i].m_strName+"函数",i);
	}	

	LV_COLUMN lvcolumn;
	CString str;

	for(i = 0; i < 1; i++)
	{
		str.Format("%s",_T("请选择函数："));

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = (char*)LPCTSTR(str);
		lvcolumn.iSubItem = i;
		lvcolumn.cx = 280;
		m_wndListCtrl.InsertColumn(i, &lvcolumn);
	}

	m_wndListCtrl.Init();

	AddFunByType(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFunctionMan::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	//this->ShowWindow(SW_HIDE);
}

void CDlgFunctionMan::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	//this->ShowWindow(SW_HIDE);
}

void CDlgFunctionMan::OnNew() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgFunctionMan::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgFunctionMan::OnInsert() 
{
	int nSel = m_wndListCtrl.GetSelectionMark();
	if( nSel != -1 )
	{
		CObject* pValue = (CObject*)m_wndListCtrl.GetItemData(nSel);
		OnUserDefUpdateData(0,(LPARAM)pValue);
	}
}

void CDlgFunctionMan::OnItemchangedType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(pNMListView != NULL && (*pNMListView).uNewState )
	{
		if( (*pNMListView).iItem != -1 )
		{
			AddFunByType((*pNMListView).iItem);
		}
	}

	/*
	// load current
	int nIndex = m_wndTypeList.GetSelectionMark( );
	if( nIndex == 1 )
	{
		AddExpressByType(0);
	}
	else
	{
		if(nIndex > 0)
			nIndex--;
		AddFunByType(nIndex);
	}
	*/
	*pResult = 0;
}

void CDlgFunctionMan::AddExpressByType(short nType)
{
	if( m_pExternExpression == NULL )
		return;

	m_wndListCtrl.DeleteAllItems();

	POSITION     pos = m_pExternExpression->GetStartPosition();
	CString	     sName,strText;
	CValue*      pVal;
	CExpression* pExpression;
	CExpValue*   pValue;

	int			 nCurItem = 0;
	while (pos != NULL)
	{
		m_pExternExpression->GetNextAssoc (pos, sName, pVal);
		pValue = (CExpValue*)pVal;
		pExpression = pValue->GetExp();
		if( pExpression != NULL && 
			(nType == 0 || pExpression->GetExpressType() == nType) )
		{			
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				strText.Format( "%s", sName);
				sName.Format("%s-%s",strText,pExpression->GetDescribe());
			}
			
			m_wndListCtrl.InsertItem (nCurItem,sName,nType);
			m_wndListCtrl.SetItemData (nCurItem,(LPARAM)pVal);
			nCurItem ++;	
		}
	}

}

void CDlgFunctionMan::AddFunByType(short nType)
{
	if( m_pExternFunction == NULL || 
		nType < 0 || 
		nType > 11 )
		return;

	m_wndListCtrl.DeleteAllItems();

	POSITION pos = m_pExternFunction->GetStartPosition();
	CString	sName;
	CValue* pVal;
	int     nCurItem = 0;
	while (pos != NULL)
	{
		m_pExternFunction->GetNextAssoc (pos, sName, pVal);
		if( ((CFunExpresion*)pVal)->IsFunType( (FunctionType)nType ) || nType == 0 )
		{
			// 热门板块函数，加入, 2003年11月14日
			if( ((short)((CFunExpresion*)pVal)->GetOperator()) < 0 ) 
			{
				continue;
			}
			if( !((CFunExpresion*)pVal)->GetDescribe().IsEmpty() )
			{
				sName.Format("%s-%s", 
					((CFunExpresion*)pVal)->GetFunName(),
					((CFunExpresion*)pVal)->GetDescribe());
			}
			m_wndListCtrl.InsertItem (LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									  nCurItem,sName,0,0,nType,(LPARAM)pVal);
			nCurItem ++;
		}
	}
}

long CDlgFunctionMan::OnUserDefUpdateData(WPARAM wParam /*= 0*/, LPARAM lParam /*= 0*/)
{
	if(lParam == 0 || m_pParent == NULL)
		return 0;

	CObject* pData = (CObject*)lParam;
	if( AfxIsMemoryBlock(pData,sizeof(CFunExpresion)) )
	{
		//CFunExpresion* pFun = DYNAMIC_DOWNCAST(CFunExpresion,pData);
		CFunExpresion* pFun = (CFunExpresion*)pData;
		if( pFun != NULL && 
			pFun->IsStyle(HX_EXPRESSFUNCTION) )
		{
			HSAddStrCompileEdit data;			
			CString strFun;
			data.m_nCharPos = pFun->GetFunDefMode(strFun);
			data.m_strText  = new CString(strFun);

			m_pParent->m_pRichEdit->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&data);			
		}
	}
	else if( AfxIsMemoryBlock(pData,sizeof(CExpValue)) )
	{
		//CExpValue* pExp = DYNAMIC_DOWNCAST(CExpValue,pData);
		CExpValue* pExp = (CExpValue*)pData;
		if( pExp != NULL && 
			pExp->IsStyle(HX_EXPRESSSENTENCE) )
		{
			CExpression* pExpression = pExp->GetExp();
			if(pExpression != NULL)
			{
				HSAddStrCompileEdit data;
				CString strFun;
				data.m_nCharPos = 0;
				data.m_strText  = (CString*)pExpression;

				m_pParent->m_pRichEdit->SendMessage(HX_USER_SENDADDSTR,
					HX_EXPRESSSENTENCE,(LPARAM)&data);
				// ??? data析构时删除空间会否出现问题, 已在另一边处理,不过我认为在这里处理会更明显
				// data.m_strText = NULL;// HS 2003-5-7 加入
			}
		}
	}
	
	return 0;
}


void CDlgFunctionMan::OnItemchangedFunlist(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if(m_wndNotes.m_hWnd != NULL)
	{
		if(pNMListView != NULL && (*pNMListView).uNewState )
		{
			if( (*pNMListView).iItem != -1 )
			{
				CObject* pData = (CObject*)m_wndListCtrl.GetItemData((*pNMListView).iItem);
				if( AfxIsMemoryBlock(pData,sizeof(CFunExpresion)) )
				{
					//CFunExpresion* pFun = DYNAMIC_DOWNCAST(CFunExpresion,pData);
					CFunExpresion* pFun = (CFunExpresion*)pData;
					if( pFun != NULL && 
						pFun->IsStyle(HX_EXPRESSFUNCTION) )
					{
						m_wndNotes.SetWindowText(pFun->GetNotes());
					}
				}
				else if( AfxIsMemoryBlock(pData,sizeof(CExpValue)) )
				{
					//CExpValue* pExp = DYNAMIC_DOWNCAST(CExpValue,pData);
					CExpValue* pExp = (CExpValue*)pData;
					if( pExp != NULL && 
						pExp->IsStyle(HX_EXPRESSSENTENCE) )
					{
						CExpression* pExpression = pExp->GetExp();
						if(pExpression != NULL)
						{
							m_wndNotes.SetWindowText(pExpression->GetSource());
						}
					}
				}
				else
				{
					m_wndNotes.SetWindowText("");
				}
			}
		}
	}

	*pResult = 0;
}

BOOL CDlgFunctionMan::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_RBUTTONDOWN )
	{
		if( pMsg->hwnd == m_wndNotes.m_hWnd && 
			::IsWindow(m_wndNotes.m_hWnd) )
		{
			int nStartChar;
			int nEndChar;
			m_wndNotes.GetSel( nStartChar, nEndChar );
			if( nStartChar == nEndChar )
			{
				m_wndNotes.SetSel(0,-1);
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFunctionMan::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( abs(cx) + abs(cy) <= 0 )
		return;
}

void CDlgFunctionMan::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	//delete this;
}
