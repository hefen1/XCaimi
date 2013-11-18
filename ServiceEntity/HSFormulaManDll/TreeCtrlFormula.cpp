// TreeCtrlFormula.cpp : implementation file
//

#include "stdafx.h"
#include "TreeCtrlFormula.h"
#include "CompileEditView.h"
#include "InputPassPwD.h"

#include "VarValue.h"
#include "DlgFormulaMan.h"
#include "DlgFormulaEdit.h"
#include "DlgFormulaBrowser.h"
#include "Express.h"
#include "FunExpresion.h"
#include "..\..\Controls\zlib\zlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _SUPPORT_FLOAT_WIN_

HCURSOR g_hDragDataCursor  = NULL;
HCURSOR g_hDragErrorCursor = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlFormula

FunctionNameTable CTreeCtrlFormula::g_strFunName[] =
{
	"所有", // 0
	"行情", // 1
	"时间", // 2
	"引用", // 3
	"逻辑", // 4
	"算术", // 5
	"数学", // 6
	"统计", // 7
	"指标", // 8
	"大盘", // 9
	"常数",  // 10
	"绘图"  // 11
};

// default tree root
CString CTreeCtrlFormula::g_strTech		      = G_STRTECH;
CString CTreeCtrlFormula::g_strCondition      = G_STRCONDITION;
CString CTreeCtrlFormula::g_strExchange       = G_STREXCHANGE;
CString CTreeCtrlFormula::g_strColorKLine     = G_STRCOLORKLINE;
CString CTreeCtrlFormula::g_strRecycleBin     = G_STRRECYCLEBIN;
CString CTreeCtrlFormula::g_strStrategy		  = G_STRSTRATEGY; //20090302 YJT 增加

CString CTreeCtrlFormula::g_strUnionCondition = G_STRUNIONCONDITION;
CString CTreeCtrlFormula::g_strJiben          = G_STRJIBEN;

CString CTreeCtrlFormula::g_strFirstTechLine  = _T("第1条指标线");

CString CTreeCtrlFormula::m_strExpGroup		  = _T("公式组");
CString CTreeCtrlFormula::m_strExpAll		  = _T("所有");
CString CTreeCtrlFormula::m_strExpComm		  = _T("常用");
CString CTreeCtrlFormula::m_strExpUser		  = _T("自编");

CString CTreeCtrlFormula::g_strDefault		  = _T("其它");

CCompileEditView*  CTreeCtrlFormula::m_pCompileEditView = NULL;
CCompileEditView*  CTreeCtrlFormula::m_pHelpView		= NULL;

extern HWND g_hParentWnd;

int IsZipExpressData(const char* szData)
{
	return !strncmp(szData,Express_Header,strlen(Express_Header));
}

char* unzipData(SaveTransZipData* pZipData)
{
	if( pZipData->m_lOrigLen > 0 && pZipData->m_lZipLen > 0 )
	{
		Byte* pTemp = new Byte[pZipData->m_lOrigLen];
		uLong pRetLen = (uLong)pZipData->m_lOrigLen;

		uncompress( pTemp, &pRetLen,
			(Byte*)pZipData->m_cData, 
			(uLong)pZipData->m_lZipLen);

  		if( pRetLen == pZipData->m_lOrigLen )
  		{
  			return (char*)pTemp;
  		}
  		else
  		{
 			delete[] pTemp;
 		}
		
	}

	return NULL;
}

SaveTransZipData* zipData(void *lpszBuffer, long cbBuffer)
{
	Byte *dest = new Byte[cbBuffer];
	memset(dest, 0, cbBuffer);
	uLong destLen = (uLong)cbBuffer;

	Byte *source = new Byte[cbBuffer];
	memcpy(source, lpszBuffer, cbBuffer);
	uLong sourceLen = cbBuffer;

	// 压缩
	if( compress(dest, &destLen, source, sourceLen) != Z_OK )
	{
		delete[] source;
		delete[] dest;
		return NULL;
	}

	SaveTransZipData* pData = new SaveTransZipData;
	pData->m_lOrigLen = cbBuffer;
	pData->m_lZipLen  = (long)destLen;
	pData->m_cData    = new char[destLen];
	memcpy(pData->m_cData,(char*)dest,destLen);

	delete[] source;
	delete[] dest;

	return pData;
}

void CTreeCtrlFormula::Atatch(class CCompileEditView* pCompileEditView,class CCompileEditView* pHelpView)
{
	m_pCompileEditView = pCompileEditView;
	m_pHelpView = pHelpView;
}

CTreeCtrlFormula::CTreeCtrlFormula(CString strName /*= ""*/)
{
	m_pInExpression = NULL;

	m_treeType    = None;
	m_pFormulaMan = NULL;
	m_strName     = strName;
}

CTreeCtrlFormula::~CTreeCtrlFormula()
{	
	Delete();
}

void CTreeCtrlFormula::Delete()
{
	for(int i = 0; i < m_ayTreeGroup.GetSize(); i++)
	{
		delete m_ayTreeGroup.GetAt(i);
	}
	m_ayTreeGroup.RemoveAll();
}

BEGIN_MESSAGE_MAP(CTreeCtrlFormula, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlFormula)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP

//	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
//	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginDrag)

	ON_COMMAND(IDM_NEWGROUP,OnNewGroup)
	ON_COMMAND(IDM_EXPANDGROUP,OnExpandGroup)
	ON_COMMAND(IDM_REDUCEGROUP,OnReduceGroup)

	ON_COMMAND(IDM_NEWEXPRESS,OnNewExpress)	
	ON_COMMAND(IDM_EDITEXPRESS,OnEditExpress)
	ON_COMMAND(IDM_DELEXPRESS,OnDelExpress)
	ON_COMMAND(IDM_DELGROUP,OnDelGroup)

	ON_COMMAND(IDM_DEFAULTPRESS,OnDefaultPress)

	ON_COMMAND(IDM_DELUINION,OnDelUnion)

	ON_WM_KILLFOCUS()

	ON_REGISTERED_MESSAGE(HX_USER_DRAGDATA,OnUserDragData)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlFormula message handlers
LRESULT CTreeCtrlFormula::OnUserDragData(WPARAM wParam, LPARAM lParam)
{
	if(wParam == HSCurKeyInfo::UpdateExpressData)
	{
		CExpression* pExpression = (CExpression*)lParam;
		if(pExpression)
		{
			CTreeGroup* pTreeGroup = Find(pExpression);
			if( pTreeGroup && pTreeGroup->m_hTree ) 
			{
				SelectItem(pTreeGroup->m_hTree);
			}
		}
	}
	return 0;
}
int CTreeCtrlFormula::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	BOOL bSucc = m_CTreeDropTarget.Register(this);

	long lStyle = this->GetStyle();
	if(lStyle & TVS_CHECKBOXES)
	{
		this->SetType(Export);
	}

	COLORREF color = RGB(255,255,255);
	SetBkColor(color);

/*
	if( IsType(CTreeCtrlFormula::AlignWin) )
	{
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
		if(pMain != NULL)
		{
			CWorkspaceWnd*	pWork = pMain->m_wndFunBar.GetWorkspaceWnd();
			if(pWork != NULL)
			{
				CTreeCtrlFormula* pTree = pWork->GetExpressTree();
				if(pTree != NULL)
				{
					CopyTree(pTree);
				}
			}
		}
	}
*/

	return 0;
}

void CTreeCtrlFormula::OnDestroy() 
{
	//SaveLoad(CArchive::store,m_strFileName);
	CTreeCtrl::OnDestroy();	
	// TODO: Add your message handler code here	
}

void CTreeCtrlFormula::FillExpress(CMapVariabile* pExternExpression)
{
	if( CExpression::m_pExternExpression == NULL )
		return;

	DeleteAllItems();
	if( !SaveLoad(CArchive::load) )
	{		
		DeleteAllItems();

		InsertExpressByType(g_strTech,Tech,0);
		InsertExpressByType(g_strCondition,Condition,0);
		InsertExpressByType(g_strExchange,Exchange,0);
		InsertExpressByType(g_strColorKLine,MoreKLine,0);
		InsertExpressByType(g_strRecycleBin,ExpressBin,0);
		InsertExpressByType(g_strStrategy,Strategy,0); //20090302 YJT 增加 Strategy
	}

	if( GetRootTree(g_strTech) == NULL )
	{
		InsertItem(g_strTech, 0, 0);
	}
	if( GetRootTree(g_strCondition) == NULL )
	{
		InsertItem(g_strCondition, 0, 0);
	}
	if( GetRootTree(g_strExchange) == NULL )
	{
		InsertItem(g_strExchange, 0, 0);
	}
	if( GetRootTree(g_strColorKLine) == NULL )
	{
		InsertItem(g_strColorKLine, 0, 0);
	}
	if( GetRootTree(g_strRecycleBin) == NULL )
	{
		InsertItem(g_strRecycleBin, 0, 0);
	}

	HTREEITEM hItem = GetRootItem();
	if(hItem != NULL)
	{
		SelectItem(hItem);
	}
}

void CTreeCtrlFormula::FillExpress(CMapVariabile* pExternExpression,CString strName,char cType,short nImage)
{
	if( CExpression::m_pExternExpression == NULL )
		return;

	DeleteAllItems();
	m_strFileName.Format("%s",strName);
	if( !SaveLoad(CArchive::load) )
	{
		InsertExpressByType(strName,cType,nImage);
	}

	if( GetRootTree(strName) == NULL )
	{
		InsertItem(strName, 0, 0,TVI_ROOT,TVI_LAST);
	}
	if( GetRootTree(g_strRecycleBin) == NULL )
	{
		InsertItem(g_strRecycleBin, 0, 0,TVI_ROOT,TVI_LAST);
	}

	HTREEITEM hItem = GetRootItem();
	if(hItem != NULL)
	{
		SelectItem(hItem);
	}
}

void CTreeCtrlFormula::LoadFromFile(CString strName)
{
	if( !SaveLoad(CArchive::load,strName) )
	{
	}
}

void CTreeCtrlFormula::FillFunction(CMapVariabile* pExternFunction)
{
	if( CExpression::m_pExternFunction == NULL )
		return;
	
	DeleteAllItems();
	Delete();

	for(int i = 0; i < sizeof(g_strFunName)/sizeof(FunctionNameTable);i++)
	{
		InsertFunByType(i);		
	}

	HTREEITEM hItem = GetRootItem();
	if(hItem != NULL)
	{
		SelectItem(hItem);
	}
}

HTREEITEM CTreeCtrlFormula::InsertExpressByType(CString strName,short nType,short nImage)
{
	HTREEITEM hCurItem;
	HTREEITEM hClass = InsertItem(strName, nImage, nImage,TVI_ROOT,TVI_LAST);

	POSITION     pos = CExpression::m_pExternExpression->GetStartPosition();
	CString	     sName,strText;
	CValue*      pVal;
	CExpression* pExpression;
	CExpValue*   pValue;
	CTreeGroup*  pTreeGroup;
	while (pos != NULL)
	{
		CExpression::m_pExternExpression->GetNextAssoc (pos, sName, pVal);
		pValue = (CExpValue*)pVal;
		pExpression = pValue->GetExp();
		if( pExpression != NULL && 
			//!pExpression->IsStyle(HS_EXP_PROTECTED) && // 公式是否为保护的
			pExpression->GetExpressType() == nType )
		{
			strText.Format( "%s", sName);
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				sName.Format("%s-%s",strText,pExpression->GetDescribe());
			}
			nImage   = pExpression->GetImageIndex();
			hCurItem = InsertItem (sName, nImage, nImage,hClass);

			pTreeGroup = new CTreeGroup(pValue/*pExpression*/,hx_GroupItem,hCurItem);
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hCurItem,(DWORD)pTreeGroup);
		}
	}
	return hClass;
}

void CTreeCtrlFormula::InsertFunByType(short nType)
{
	if( CExpression::m_pExternFunction == NULL || 
		nType < 0 || 
		nType > sizeof(g_strFunName)/sizeof(FunctionNameTable) )
		return;

	HTREEITEM hCurItem;
	CString str;
	str =  _T(g_strFunName[nType].m_strName);

	HTREEITEM hClass = InsertItem ((char*)LPCTSTR(str), nType, nType);

	POSITION pos = CExpression::m_pExternFunction->GetStartPosition();
	CString	sName;
	CValue* pVal;
	while (pos != NULL)
	{
		CExpression::m_pExternFunction->GetNextAssoc (pos, sName, pVal);
		if( ((CFunExpresion*)pVal)->IsFunType( (FunctionType)nType ) ||  nType == 0 )
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
			hCurItem = InsertItem (sName, nType, nType,hClass);
			SetItemData(hCurItem,(DWORD)pVal);
		}
	}
}

void CTreeCtrlFormula::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTreeCtrlFormula::SetFunNotes(HTREEITEM hItem)
{
	if(m_pHelpView != NULL && hItem != NULL)
	{
		if( IsType(Function) )
		{
			CFunExpresion* pValue = (CFunExpresion*)GetItemData(hItem);
			if(pValue != NULL && pValue->IsStyle(HX_EXPRESSFUNCTION) )
			{
				CString str = pValue->GetNotes();
				m_pHelpView->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&str);
			}
		}
	}
}

void CTreeCtrlFormula::AddFunToEdit()
{
	if( IsType(CTreeCtrlFormula::Export) )
	{
		HTREEITEM hItem = GetSelectedItem();
		if(hItem != NULL)
		{
			CWnd* pWnd = GetParent();
			if(pWnd != NULL)
			{
				DWORD lParam = this->GetItemData(hItem);
				if( !lParam )
				{
					pWnd->SendMessage(HX_USER_SENDADDSTR,0,lParam);
				}
			}
		}
	}
	else if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) )
	{
		HTREEITEM hItem = GetSelectedItem();
		if(hItem != NULL)
		{
			CWnd* pWnd = GetParent();
			if(pWnd != NULL)
			{
				if( IsType(CTreeCtrlFormula::EditFunRef) )
				{
					CString strName = this->GetItemText(hItem);
					if( !strName.IsEmpty() )
					{
						pWnd->SendMessage(HX_USER_SENDADDSTR,m_treeType,(LPARAM)&strName);
					}
				}
				else
				{
					CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
					if( pTreeGroup != NULL && 
						pTreeGroup->IsStatus(hx_GroupItem) )
					{
						pWnd->SendMessage(HX_USER_SENDADDSTR,m_treeType,(LPARAM)pTreeGroup->m_pData);
					}
				}
			}
		}
	}
	else if( IsType(Function) ) 
	{		
		HTREEITEM hItem = GetSelectedItem();
		if(m_pCompileEditView != NULL && hItem != NULL)
		{
			CFunExpresion* pValue = (CFunExpresion*)GetItemData(hItem);
			if( pValue != NULL && pValue->IsStyle(HX_EXPRESSFUNCTION) )
			{
				HSAddStrCompileEdit data;			
				CString strFun;
				data.m_nCharPos = pValue->GetFunDefMode(strFun);
				data.m_strText  = new CString(strFun);
				
				m_pCompileEditView->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)&data);			
			}
		}
	}
	else if( IsType(Express) ) 
	{
		OnEditExpress();
	}	
}

void CTreeCtrlFormula::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	AddFunToEdit();
	*pResult = 0;
}

void CTreeCtrlFormula::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	/*
	switch( (*pTVKeyDown).wVKey )
	{
	case VK_ESCAPE:
		{
			if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) )
			{
				//this->ShowWindow(SW_HIDE);
				CWnd* pWnd = GetParent();
				if(pWnd != NULL)
				{
					if( ((CDlgFormulaEdit*)pWnd)->m_pRichEdit != NULL )
					{
						((CDlgFormulaEdit*)pWnd)->m_pRichEdit->SetFocus();
					}
				}
			}
		}
		break;
	case VK_RETURN:
		AddFunToEdit();
		break;
	case VK_DELETE:
		if( IsType(CTreeCtrlFormula::Express) )
		{
			if( IsSHIFTpressed() && IsCTRLpressed() )
			{
				HTREEITEM hItem = GetSelectedItem();
				if(hItem == NULL)
				{
					*pResult = 0;
					return;
				}
				
				CString strName = GetItemText(hItem);
				CString strPrompt;
				strPrompt.Format("您真的确定删除\"%s\"？(y/n)",strName);
				int nID = MessageBox(strPrompt,"提示：",
					MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
				if(nID != IDYES)
				{
					*pResult = 0;
					return;
				}
				
				DelAll(hItem,FALSE);
			}
		}
		break;
	}
	*/

	*pResult = 0;
}

void CTreeCtrlFormula::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMTreeView != NULL)
	{
		SetFunNotes( ((*pNMTreeView).itemNew).hItem );

		if(((*pNMTreeView).itemNew).hItem != NULL)
		{
			SelectItem(((*pNMTreeView).itemNew).hItem);
			Select(((*pNMTreeView).itemNew).hItem, TVGN_DROPHILITE);
		}
		
		HTREEITEM hItem = ((*pNMTreeView).itemNew).hItem;
		if( hItem == NULL )
			return;

		if( IsType(CTreeCtrlFormula::Export) )
		{
			CWnd* pWnd = GetParent();
			if(pWnd != NULL)
			{
				// gbq add 20060207
				if( ExPassword(hItem) )
				{
					CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
					pWnd->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)pTreeGroup);
				}
				else		//选回上次
				{
					SelectItem(((*pNMTreeView).itemOld).hItem);
				}
				/*
				// gbq add, 20060207 判断是否需要加密
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
				if( pTreeGroup )
				{
					CExpression* pExp = (CExpression*)pTreeGroup->GetData();
					if( pExp )
					{
						CString pstrName = pExp->GetName();
						if( !pExp->GetPWD().IsEmpty() )
						{
							CInputPassPwD dlg(this,"请输入公式密码",pstrName);
							dlg.m_pExp = pExp;
							if( dlg.DoModal() == IDOK )
								pWnd->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)pTreeGroup);
							else		//选回上次
							{
								SelectItem(((*pNMTreeView).itemOld).hItem);
							}
						}
						else
							pWnd->SendMessage(HX_USER_SENDADDSTR,0,(LPARAM)pTreeGroup);
					}
				}
				*/
				/* old 
				DWORD lParam = this->GetItemData(hItem);
				if( lParam )
				{
					pWnd->SendMessage(HX_USER_SENDADDSTR,0,lParam);
				}
				*/
				// gbq add end;
				//AfxMessageBox("change");
			}
		}
		else if( IsType(CTreeCtrlFormula::HSBrowser) )
		{
			CWnd* pWnd  = this->GetParent();
			if( pWnd != NULL )
			{
				pWnd = pWnd->GetParent();
				if( pWnd )
				{
					CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
					HTREEITEM hParentItem = this->GetParentItem(hItem);
					HTREEITEM hRootItem = GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
					if(pTreeGroup && hParentItem == hRootItem && hRootItem != NULL )
					{
						pWnd->SendMessage(HX_USER_LOADCUR,
							HX_UNION_EXPRESS,(LPARAM)pTreeGroup);
						return;
					}
					
					if( pTreeGroup == NULL || 
						pTreeGroup->IsStatus(hx_OtherData) ||
						!pTreeGroup->IsStatus(hx_GroupItem) )
					{
						//CString strText = GetItemText(hItem);
						pWnd->SendMessage(HX_USER_LOADCUR,
							CTreeCtrlFormula::HSBrowser,(LPARAM)pTreeGroup);

						return;
					}

					pWnd->SendMessage(HX_USER_LOADCUR,
						HX_EXPRESSSENTENCE,(LPARAM)pTreeGroup->GetData());
				}
			}
		}
		else if(m_pFormulaMan != NULL)
		{
			if( !IsEditExpress(hItem) )
			{
				m_pFormulaMan->SendMessage(HX_USER_LOADCUR,0,NULL);
				return;
			}

			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			if( pTreeGroup == NULL || 
				!pTreeGroup->IsStatus(hx_GroupItem) )
			{
				m_pFormulaMan->SendMessage(HX_USER_LOADCUR,0,NULL);
				return;
			}

			m_pFormulaMan->SendMessage(HX_USER_LOADCUR,0,(LPARAM)pTreeGroup->GetData());
		}
	}
	*pResult = 0;
}

HTREEITEM CTreeCtrlFormula::AddItem( HTREEITEM hParent, CString csItemName, 
								HTREEITEM hInsAfter, int iSelImage , int iNonSelImage, long lParam)
{
	HTREEITEM hItem;
	TV_ITEM tvItem;
	TV_INSERTSTRUCT tvIns;

	tvItem.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	int nLength = csItemName.GetLength() ;
	tvItem.pszText = csItemName.GetBuffer(nLength);
	tvItem.cchTextMax = nLength;
	tvItem.lParam = lParam;

	tvItem.iImage = iNonSelImage;
	tvItem.iSelectedImage = iSelImage;
	
	tvIns.item = tvItem;
	tvIns.hInsertAfter = hInsAfter;
	tvIns.hParent = hParent;
	
	hItem = InsertItem(&tvIns);
	
	return (hItem);
}

void CTreeCtrlFormula::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( IsType(CTreeCtrlFormula::AlignWin) )
	{
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;
		SelectItem(hTSelItem);
		Select(hTSelItem, TVGN_DROPHILITE);
		
		CPoint point;
		::GetCursorPos(&point);
		if( MouseDown(0,point) )
		{	
			CWnd* pWnd = WindowFromPoint(point);
			if(pWnd != NULL)
			{				
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hTSelItem);
				if( pTreeGroup != NULL &&
					pTreeGroup->IsStatus(hx_GroupItem) &&
					pTreeGroup->GetData() != NULL)
				{
					::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
						HSCurKeyInfo::DragUpdateExpressData2/*HX_USER_DRAGDATA-2*/,
						(LPARAM)pTreeGroup->GetData());
				}
			}

			pWnd = CWnd::FromHandle(g_hParentWnd);		
			if(pWnd != NULL)
			{
				pWnd->SetFocus();
			}
		}
	}
	else if( IsType(CTreeCtrlFormula::Express) )
	{
		/*
		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		HTREEITEM hTSelItem = pNMTreeView->itemNew.hItem;
		// Highlight selected item
		SelectItem(hTSelItem);
		Select(hTSelItem, TVGN_DROPHILITE);
		
		COleDataSource *poleSourceObj = new COleDataSource ;
		CTreeDropTarget::m_shWndTreeCtrl = m_hWnd;
		// Begin Drag operation
		DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();
		// Remove the highlighting
		SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE,0);
		// If user is moving item by pressing Shift, delete selected item
		if ( dropeffect == DROPEFFECT_MOVE)
			DeleteItem(hTSelItem); 
		delete poleSourceObj;

		// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
		if(pMain != NULL)
		{
			//pMain->CopyTree();
			pMain->CopyToFloatTree();
		}
#endif
		*/
	}
	*pResult = 0;
}

BOOL CTreeCtrlFormula::IsDrag(HTREEITEM hSelectedItem,HTREEITEM hTDropItem)
{
	if( IsType(Express) )
	{
		CTreeGroup* pSrc = (CTreeGroup*)GetItemData(hSelectedItem);
		CTreeGroup* pDes = (CTreeGroup*)GetItemData(hTDropItem);
		if( pSrc != NULL && // 项与组
			pSrc->IsStatus(hx_GroupItem) &&
			(pDes == NULL || (pDes != NULL && pDes->IsStatus(hx_AddGroup))) )
		{
			if( GetParentItem(hSelectedItem) == hTDropItem ) // 同组
			{
				return FALSE;
			}
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}

void CTreeCtrlFormula::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//OnLButtonDown(nFlags, point);
	CTreeCtrl::OnRButtonDown(nFlags, point);
	
	HTREEITEM hHitItem = HitTest( point, &nFlags );
	if(hHitItem == NULL)
		return;

	SelectItem(hHitItem);

	if( IsType(EditFunRef) || IsType(EditExpRef) || 
		IsType(Export) )
		return;

	//if( ItemHasChildren(hHitItem) )
	{
		CMenu menu;	
		if(menu.LoadMenu(IDR_EXPRESSMENU))
		{
			CMenu& popup = *menu.GetSubMenu(0);
			ASSERT(popup.m_hMenu != NULL);

			CTreeGroup* pGroup = NULL;
			BOOL bUnion = TRUE;
			if( IsType(AlignWin) || IsType(CTreeCtrlFormula::HSBrowser) )
			{
				popup.RemoveMenu( 0, MF_BYPOSITION );
				for(int i = 0; i < 4;i++)
				{
					popup.RemoveMenu( 5, MF_BYPOSITION );
				}
				popup.RemoveMenu( IDM_NEWEXPRESS, MF_BYCOMMAND );
				popup.RemoveMenu( IDM_DEFAULTPRESS, MF_BYCOMMAND );
				popup.RemoveMenu( 2, MF_BYPOSITION );//by HS
				popup.RemoveMenu( 2, MF_BYPOSITION );//by HS
				popup.RemoveMenu( IDM_DELUINION, MF_BYCOMMAND );//by HS
				if( hHitItem != NULL )
				{
					pGroup = (CTreeGroup*)GetItemData(hHitItem);				
				}
				
				pGroup = (CTreeGroup*)GetItemData(hHitItem);
				if( pGroup != NULL && pGroup->IsStatus(hx_GroupItem) )
				{
					popup.RemoveMenu( IDM_EXPANDGROUP, MF_BYCOMMAND );
					popup.RemoveMenu( IDM_REDUCEGROUP, MF_BYCOMMAND );
				}
				
				HTREEITEM hParentItem = this->GetParentItem(hHitItem);
				HTREEITEM hRootItem = GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
				if( pGroup && hParentItem == hRootItem )
				{
					bUnion = FALSE;
				}
				else if( pGroup != NULL && !pGroup->IsStatus(hx_GroupItem) )
				{
					popup.RemoveMenu( 2, MF_BYPOSITION );
					popup.RemoveMenu( 2, MF_BYPOSITION );
				}
			}
			else if( IsType(Function) )
			{
				while(popup.GetMenuItemCount() > 3)
				{
					popup.RemoveMenu( 3, MF_BYPOSITION );
				}
				popup.RemoveMenu( IDM_NEWGROUP, MF_BYCOMMAND );
			}
			else if( IsType(Express) )
			{
				if( hHitItem != NULL )
				{					
					pGroup = (CTreeGroup*)GetItemData(hHitItem);
				}
				
				BOOL bEdit = !IsEditExpress(hHitItem);

				if( pGroup != NULL && pGroup->IsStatus(hx_GroupItem) )
				{
					popup.RemoveMenu( IDM_DELGROUP, MF_BYCOMMAND );
					popup.RemoveMenu( IDM_NEWGROUP, MF_BYCOMMAND);

					if( bEdit )
					{
						popup.RemoveMenu( IDM_EDITEXPRESS, MF_BYCOMMAND);
						popup.RemoveMenu( IDM_DEFAULTPRESS, MF_BYCOMMAND);
						popup.RemoveMenu( IDM_EXPANDGROUP, MF_BYCOMMAND);
						popup.RemoveMenu( IDM_REDUCEGROUP, MF_BYCOMMAND);
						popup.RemoveMenu( IDM_NEWEXPRESS, MF_BYCOMMAND);

						popup.RemoveMenu( 0, MF_BYPOSITION );
						popup.RemoveMenu( 0, MF_BYPOSITION );

						popup.RemoveMenu( 1, MF_BYPOSITION );
					}
					else
					{
						CExpression* pExpression = pGroup->GetData();
						if( pExpression != NULL )
						{
							if( pExpression->IsStyle(HS_EXP_DEFAULT_TECH) )
							{
								popup.CheckMenuRadioItem(IDM_DEFAULTPRESS,IDM_DEFAULTPRESS,IDM_DEFAULTPRESS,
									MF_BYCOMMAND);								
							}
						}
					}
				}
				else if( pGroup != NULL && pGroup->IsStatus(hx_AddGroup) )
				{
					for(int i = 0; i < 3;i++)
					{
						popup.RemoveMenu( 5, MF_BYPOSITION );
					}
					popup.RemoveMenu( IDM_DEFAULTPRESS,MF_BYCOMMAND );//by HS
					popup.RemoveMenu( IDM_NEWGROUP, MF_BYCOMMAND );
					popup.RemoveMenu( 5, MF_BYPOSITION );

					if( bEdit )
					{
						popup.RemoveMenu( IDM_NEWEXPRESS, MF_BYCOMMAND);
					}
				}
				else
				{
					while(popup.GetMenuItemCount() > 3)
					{
						popup.RemoveMenu( 3, MF_BYPOSITION );
					}

					if( bEdit )
					{
						popup.RemoveMenu( IDM_NEWGROUP, MF_BYCOMMAND);
					}
				}
			}

			if( bUnion )
			{
				popup.RemoveMenu( IDM_DELUINION, MF_BYCOMMAND );
			}

			::GetCursorPos(&point);
			popup.TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON | /*TPM_BOTTOMALIGN*//*gbq*/ TPM_TOPALIGN,
				point.x, point.y,this);
		}
	}

}

void CTreeCtrlFormula::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	

	HTREEITEM hTSelItem = HitTest( point, &nFlags );
	/*if( hTSelItem != NULL )
	{
		Select( hTSelItem, TVGN_CARET  );
	}*/

	if( IsType(Export) )
	{
		CRect rc;
		if( GetItemRect(hTSelItem,rc,TRUE) )
		{
			CRect rect = rc;
			rc.left  -= 32;
			rc.right = rc.left + 16;
			if(rc.PtInRect(point))
			{
				//AfxMessageBox("check");
				Check(hTSelItem);
				return;
			}
			//rc.right = rect.right;
			//if(rc.PtInRect(point))
			//	AfxMessageBox("test");
		}
	}
	
    CTreeCtrl::OnLButtonDown(nFlags, point);

	if( !IsType(Export) && hTSelItem != NULL)
	{
		CPoint point;
		::GetCursorPos(&point);
		if( MouseDown(0,point) )
		{
			CWnd* pWnd = WindowFromPoint(point);
			if(pWnd == NULL) return;
			
			if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) )
			{
				AddFunToEdit();
			}
			else
			{
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hTSelItem);
				
				if( pTreeGroup != NULL &&
					pTreeGroup->IsStatus(hx_GroupItem) &&
					pTreeGroup->GetData() != NULL &&
					!(pTreeGroup->GetData()->IsStyle(HS_EXP_PROTECTED)) )
				{
					::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
						HSCurKeyInfo::DragUpdateExpressData3/*HX_USER_DRAGDATA-3*/,(LPARAM)pTreeGroup->GetData());
				}
				else
				{
					CString strText = GetItemText(hTSelItem);
					::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
						HSCurKeyInfo::OpenUnionTechChart/*HX_USER_DRAGDATA-101*/,(LPARAM)&strText);
				}
			
				pWnd = CWnd::FromHandle(g_hParentWnd);		
				if(pWnd != NULL)
				{
					pWnd->SetFocus();
				}
			}
		}

		//this->SendMessage(WM_LBUTTONUP,nFlags,MAKELONG(point.x,point.y));
	}	
}

void CTreeCtrlFormula::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	*pResult = 0;
}

void CTreeCtrlFormula::OnNewGroup()
{
    CInputPassPwD dlg(this,_T("新建公式组"),_T("请输入公式组名"),1);
	dlg.m_strCurPWD = _T("新组");
	if( dlg.DoModal() == IDOK )
	{
		HTREEITEM hItem = GetSelectedItem( );
		HTREEITEM hParentItem = hItem;
		hItem = GetParentItem(hItem);
		while( hItem != NULL )
		{
			hParentItem = hItem;
			hItem = GetParentItem(hItem);
		}

		if(hParentItem != NULL)
		{
			if( FindGroupName(dlg.m_strCurPWD) != NULL )
				return;

			HTREEITEM hCurItem = InsertItem(dlg.m_strCurPWD, 2, 2, hParentItem);
			CTreeGroup* pTreeGroup = new CTreeGroup(NULL,hx_AddGroup,hCurItem);
	//		CTreeGroup* pTreeGroup = new CTreeGroup((void* )(dlg.m_strCurPWD.GetBuffer()),hx_AddGroup,hCurItem);
			
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hCurItem,(DWORD)pTreeGroup);

			Expand(hParentItem,TVE_EXPAND);

			// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
			CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
			if(pMain != NULL)
			{
				//pMain->CopyTree();
				pMain->CopyToFloatTree();
			}
#endif
		}
	}
}

void CTreeCtrlFormula::OnExpandGroup()
{
	ExpandGroup(GetSelectedItem(),TVE_EXPAND);
}

void CTreeCtrlFormula::OnReduceGroup()
{
	ExpandGroup(GetSelectedItem(),TVE_COLLAPSE);
}

void CTreeCtrlFormula::OnNewExpress()
{
	if( m_pFormulaMan != NULL )
	{
		m_pFormulaMan->SendMessage(WM_COMMAND,IDD_NEW);
	}

	/*
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if( pMain != NULL )
	{
		CHxCpLangView* pView = pMain->GetCpView();
		if(pView != NULL)
		{
			CCompileEditView* pEditView = pView->GetCompileView();
			if(pEditView != NULL)
			{
				pEditView->SendMessage(WM_COMMAND,IDC_NEWEXPRESS);
			}
		}
		if( pMain->IsIconic() || !pMain->IsWindowVisible() )
		{
			pMain->ShowWindow(SW_SHOWNORMAL);
		}
	}
	*/
}

void CTreeCtrlFormula::OnDelExpress()
{
	//
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
		return;

	BOOL bRecycleBin = FALSE;
	HTREEITEM hItemBin = GetRootTree(g_strRecycleBin);
	HTREEITEM hParentItem = GetParentItem(hItem);
	while(hParentItem != NULL)
	{
		if(hItemBin == hParentItem)
		{
			bRecycleBin = TRUE;
			break;
		}
		hParentItem = GetParentItem(hParentItem);
	}

	CString strName = GetItemText(hItem);
	CString strPrompt;
	strPrompt.Format(_T("您真的确定删除公式\"%s\"？(%s)"),strName,bRecycleBin?_T("公式回收站里的删除将是永久删除"):_T("将删除到公式回收站里"));
	int nID = MessageBox(strPrompt,_T("提示："),
									MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
	if(nID != IDYES)
		return;

	DelAll(hItem,!bRecycleBin);

	// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		//pMain->CopyTree();
		pMain->CopyToFloatTree();
	}
#endif
}

void CTreeCtrlFormula::OnDelGroup() // 删除当前选中组
{
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
		return;
	
	BOOL bRecycleBin = FALSE;
	HTREEITEM hItemBin = GetRootTree(g_strRecycleBin);
	HTREEITEM hParentItem = GetParentItem(hItem);
	while(hParentItem != NULL)
	{
		if(hItemBin == hParentItem)
		{
			bRecycleBin = TRUE;
			break;
		}
		hParentItem = GetParentItem(hParentItem);
	}

	CString strName = GetItemText(hItem);
	CString strPrompt;
	strPrompt.Format(_T("您真的确定删除公式组\"%s\"？(%s)"),strName,bRecycleBin?_T("公式回收站里的删除将是永久删除"):_T("将删除到公式回收站里"));	
	int nID = MessageBox(strPrompt,_T("提示："),
									MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
	if(nID != IDYES)
		return;	

	DelAll(hItem,!bRecycleBin);

	// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		//pMain->CopyTree();
		pMain->CopyToFloatTree();
	}
#endif
}

void CTreeCtrlFormula::DelAll(HTREEITEM hItem,BOOL bRecycleBin)
{
	CString strText = GetItemText(hItem);
	if( GetType(strText) != none )
	{
		return;
	}
	
	// 移到公式回收站
	if(bRecycleBin)
	{
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
		if( pMain != NULL )
		{
			CCompileEditView* pEditView = pMain->GetExpDlg()->m_pRichEdit;
			if(pEditView != NULL)
			{
				CTreeGroup* pTreeGroup = (CTreeGroup*)this->GetItemData(hItem);
				HSTreeToCompileEdit data(pTreeGroup,hItem);
				if( pEditView->SendMessage(HX_USER_DELETECUR,0,(LPARAM)&data) == 2 )
					return;
			}
		}
		
		int nImage = 0;
		int nSelImage = 0;
		GetItemImages(hItem, nImage, nSelImage);	
		HTREEITEM hDestItem = AddItem(GetRootTree(g_strRecycleBin), GetItemText(hItem), TVI_LAST, 
			nImage, nSelImage, GetItemData(hItem));
		AddItem( GetChildItem(hItem),hDestItem,hDestItem);
	}	
	else
	{
		HTREEITEM hChildItem = GetChildItem( hItem );
		while(hChildItem != NULL)
		{
			DelFromArray(hChildItem);
			hChildItem = GetNextSiblingItem( hChildItem );
		}
		DelFromArray(hItem);
	}

	DeleteItem(hItem);
}
/* 删除该级树木所有没有孩子项的组节点 */
void	CTreeCtrlFormula::DelAllNoChildren(HTREEITEM hItem)
{
	if(hItem == NULL)
	{
		hItem = GetRootItem();					
		CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
		while(pTreeGroup && !pTreeGroup->IsStatus(hx_AddGroup))
		{
			hItem = GetChildItem(hItem);
			pTreeGroup = (CTreeGroup*)GetItemData(hItem);
		}
	}

	HTREEITEM hUse = NULL;
	while(hItem)
	{
		if(ItemHasChildren(hItem))
		{
			hItem = GetNextSiblingItem(hItem);
			continue;
		}

		hUse  = hItem;
		hItem = GetNextSiblingItem(hItem);
		
		DelFromArray(hUse);
		DeleteItem(hUse);
	}

	Expand(GetRootItem(), TVE_EXPAND );   
}
void CTreeCtrlFormula::DelFromArray(HTREEITEM hItem,BOOL bDelSource /*= TRUE*/)
{
	// 删除与实际公式的关联
	CTreeGroup* pCurGroup = (CTreeGroup*)GetItemData(hItem);
	if( pCurGroup == NULL )
		return;
	
	CTreeGroup* pTreeGroup;
	for(int i = 0; i < m_ayTreeGroup.GetSize(); i++)
	{
		pTreeGroup = m_ayTreeGroup.GetAt(i);
		if( pTreeGroup == pCurGroup )
		{
			if( IsUsed(pTreeGroup) )
				return;

			if(bDelSource && pTreeGroup->GetData() != NULL)
			{		
				CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
				if( pMain != NULL )
				{
					CCompileEditView* pEditView = pMain->GetExpDlg()->m_pRichEdit;
					if(pEditView != NULL)
					{
						HSTreeToCompileEdit data(pTreeGroup,hItem);
						if( pEditView->SendMessage(HX_USER_DELETECUR,1,(LPARAM)&data) == 2 )
							return;
					}
				}
			}

			delete pTreeGroup;
			m_ayTreeGroup.RemoveAt(i);
			return;
		}
	}	
}

void CTreeCtrlFormula::OnDefaultPress()
{
	HTREEITEM hItem = GetSelectedItem();
	if( hItem == NULL )
		return;
	
	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) )
		return;

	CExpression* pExpression = pTreeGroup->GetData();
	if(pExpression == NULL)
		return;

	if( pExpression->IsStyle(HS_EXP_DEFAULT_TECH) )
	{
		pExpression->RemoveStyle(HS_EXP_DEFAULT_TECH);
//		SetItemImage(hItem,pExpression->GetImageIndex(),pExpression->GetImageIndex());
		UINT nState = GetItemState(hItem,TVIF_STATE|TVIS_BOLD);
		nState &= (~TVIS_BOLD);
		SetItemState(hItem, nState, TVIF_STATE|TVIS_BOLD);
		CDlgFormulaMan::SetDefaultTech(pExpression,'-');
	}
	else
	{
		pExpression->AddStyle(HS_EXP_DEFAULT_TECH);
//		SetItemImage(hItem,pExpression->GetImageIndex(),pExpression->GetImageIndex());
		SetItemState(hItem, TVIS_BOLD, TVIS_BOLD);
		CDlgFormulaMan::SetDefaultTech(pExpression,'+');
	}
	
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		pMain->CopyTree();
	}
}

void CTreeCtrlFormula::OnEditExpress()
{
	HTREEITEM hItem = GetSelectedItem();
	if( hItem == NULL )
		return;
	
	if( !IsEditExpress(hItem) )
	{
		return;
	}

	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if( IsType(CTreeCtrlFormula::HSBrowser) )
	{
		CWnd* pWnd  = this->GetParent();
		if( pWnd != NULL )
		{
			pWnd = pWnd->GetParent();
			if( pWnd != NULL )
			{
				HTREEITEM hParentItem = this->GetParentItem(hItem);
				HTREEITEM hRootItem = GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
				if( pTreeGroup && hParentItem == hRootItem )
				{
					pWnd->SendMessage(HX_USER_LOADCUR,
						HX_UNION_EXPRESS,(LPARAM)pTreeGroup);
					return;
				}

				if( pTreeGroup == NULL || 
					pTreeGroup->IsStatus(hx_OtherData) ||
					!pTreeGroup->IsStatus(hx_GroupItem) )
				{
					pWnd->SendMessage(HX_USER_LOADCUR,
						CTreeCtrlFormula::HSBrowser,(LPARAM)pTreeGroup);
					return;
				}
/*
				if( (pTreeGroup == NULL) || 
					!pTreeGroup->IsStatus(hx_GroupItem) )
				{
					CString strText = GetItemText(hItem);
					pWnd->SendMessage(HX_USER_LOADCUR,
						CTreeCtrlFormula::HSBrowser,(LPARAM)&strText);
					return;
				}
*/				
				pWnd->SendMessage(HX_USER_LOADCUR,CCompileEditView::UpdateExpress,
						(LPARAM)pTreeGroup->GetData());
			}
		}
	}
	else if( m_pFormulaMan != NULL )
	{
		if( (pTreeGroup == NULL)			|| 
			(pTreeGroup->GetData() == NULL) ||
			!pTreeGroup->IsStatus(hx_GroupItem) )
			return;

		m_pFormulaMan->SendMessage(HX_USER_LOADCUR,CCompileEditView::UpdateExpress,
			(LPARAM)pTreeGroup->GetData());
	}
}

void CTreeCtrlFormula::ExpandGroup(HTREEITEM hCurItem,UINT nCode)
{
	if( hCurItem == NULL )
	{
		HTREEITEM hRootItem = GetRootItem();
		while(hRootItem != NULL)
		{
			Expand(hRootItem,nCode);

			ExpandGroup(hRootItem,nCode);
			hRootItem = GetNextSiblingItem( hRootItem );
		}
	}
	else
	{
		if( hCurItem != NULL )
		{
			Expand(hCurItem,nCode);

			HTREEITEM hChildItem = GetChildItem( hCurItem );
			while(hChildItem != NULL)
			{
				Expand(hChildItem,nCode);

				ExpandGroup(hChildItem,nCode);
				hChildItem = GetNextSiblingItem( hChildItem );
			}
		}		
	}
}

HTREEITEM CTreeCtrlFormula::FindGroupName(CString strName,BOOL bPrompt /*= TRUE*/,
										  HTREEITEM hRootItem /*= NULL*/)
{


	HTREEITEM hItemBin = NULL;
	if( hRootItem == NULL )
	{
		hRootItem = GetRootItem();
		hItemBin  = GetRootTree(g_strRecycleBin);
	}

	while( hRootItem != NULL )
	{
		if( hRootItem == hItemBin )
		{
			hRootItem = GetNextSiblingItem( hRootItem );
			continue;
		}
		
		if( !GetItemText(hRootItem).CompareNoCase(strName) )
		{
			if( bPrompt )
			{
				CString strPrompt;
				strPrompt.Format(_T("\"%s\"组名已经存在..."),strName);
				AfxMessageBox(strPrompt);
			}
			return hRootItem;
		}
		else
		{
			HTREEITEM hItem = GetChildItem( hRootItem );
			if( hItem )
			{
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
				if( pTreeGroup && pTreeGroup->IsStatus(hx_AddGroup) )
				{
					hItem = FindGroupName(strName,bPrompt,hItem);
					if( hItem != NULL )
						return hItem;
				}
			}
		}

		hRootItem = GetNextSiblingItem( hRootItem );
	}

	//HTREEITEM hItem = GetSelectedItem( );
	//HTREEITEM hParentItem = hItem;
	//hItem = GetParentItem(hItem);
	//while( hItem != NULL )
	//{
	//	hParentItem = hItem;
	//	hItem = GetParentItem(hItem);
	//}
	//
	//if( hParentItem != NULL )
	//{
	//	HTREEITEM hItem = GetChildItem( hParentItem );
	//	while(hItem != NULL)
	//	{
	//		// begin by HS
	//		//	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	//		if( CString(GetItemText(hItem)).CompareNoCase(strName) == 0 )
	//		{
	//			if(bPrompt)
	//			{
	//				CString strPrompt;
	//				strPrompt.Format("\"%s\"组名已经存在...",strName);
	//				AfxMessageBox(strPrompt);
	//			}
	//			return hItem;
	//		}
	//		hItem = GetNextSiblingItem( hItem );
	//		/*if( pTreeGroup != NULL && 
	//		pTreeGroup->GetData() != NULL &&
	//		!pTreeGroup->GetData()->GetName().CompareNoCase(strName) )
	//		{
	//		if(bPrompt)
	//		{
	//		CString strPrompt;
	//		strPrompt.Format("\"%s\"组名已经存在...",strName);
	//		AfxMessageBox(strPrompt);
	//		}
	//		return hItem;
	//		
	//		hItem = GetNextSiblingItem( hItem );}*/
	//		//end
	//	}
	//}

	return NULL;
}


HTREEITEM CTreeCtrlFormula::FindNameInGroup(CString strName,HTREEITEM hParentItem)
{
	if( hParentItem != NULL )
	{
		CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hParentItem);
		HTREEITEM hItem = NULL;
		if(pTreeGroup != NULL)
		{
			if( pTreeGroup->IsStatus(hx_GroupItem) )
				hItem = hParentItem;
		}
		while(hItem != NULL)
		{
			pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->GetData() != NULL &&
				!pTreeGroup->GetData()->GetName().CompareNoCase(strName) )
			{
				return hItem;
			}
			hItem = GetNextSiblingItem( hItem );
		}

		hItem = GetChildItem( hParentItem );
		while(hItem != NULL)
		{
			pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->GetData() != NULL &&
				!pTreeGroup->GetData()->GetName().CompareNoCase(strName) )
			{
				return hItem;
			}
			hItem = GetNextSiblingItem( hItem );
		}
	}
	else
	{
		return NULL;
	}
	return NULL;//FindNameInGroup(strName,GetParentItem(hParentItem));
}

BOOL CTreeCtrlFormula::AddItem(CString strName,CMapVariabile* pExternExpression)
{
	if( CExpression::m_pExternExpression == NULL )
		return FALSE;

	HTREEITEM hParentItem = NULL;
	HTREEITEM hItem = GetSelectedItem( );
	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem) )
		hParentItem = hItem;
	else
	{
		hItem = GetParentItem(hItem);
		hParentItem = hItem;
		pTreeGroup = (CTreeGroup*)GetItemData(hItem);
		while( !(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem)) )
		{				
			hItem = GetParentItem(hItem);
			if(hItem != NULL)
			{
				hParentItem = hItem;
				pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			}
		}
	}
	
	if(hParentItem != NULL)
	{
		CValue* pVal;
		if ( !pExternExpression->Lookup(strName,pVal) ) // 外部变量表
			return FALSE;
		
		hItem = FindNameInGroup(strName,hItem);

		CString strText;
		CExpression* pExpression;
		CExpValue* pValue = (CExpValue*)pVal;
		pExpression = pValue->GetExp();
		if( pExpression != NULL )
		{
			strText.Format( "%s", strName);
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				strName.Format("%s-%s",strText,pExpression->GetDescribe());
			}

			if( hItem != NULL)
			{
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem); 
				if( pTreeGroup != NULL &&   
					pTreeGroup->GetData() != NULL &&
					pTreeGroup->GetData()->GetName().CompareNoCase(strText) )
				{
					SetItemText(hItem,strName);
					return FALSE;  // 已经存在
				}
			}
			
			HTREEITEM hCurItem = InsertItem(strName, 1, 1,hParentItem);
			pTreeGroup = new CTreeGroup(pValue/*pExpression*/,hx_GroupItem,hCurItem);
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hCurItem,(DWORD)pTreeGroup);
	
		}
		
		Expand(hParentItem,TVE_EXPAND);

		return TRUE;
	}

	return FALSE;
}

BOOL CTreeCtrlFormula::AddItem(CString strName,CExpValue* pValue,CMapVariabile* pExternExpression)
{
	if( CExpression::m_pExternExpression == NULL )
		return FALSE;

	HTREEITEM hParentItem = NULL;
	HTREEITEM hItem = GetSelectedItem( );
	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem) )
		hParentItem = hItem;
	else
	{
		hItem = GetParentItem(hItem);
		hParentItem = hItem;
		pTreeGroup = (CTreeGroup*)GetItemData(hItem);
		while( !(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem)) )
		{				
			hItem = GetParentItem(hItem);
			if(hItem != NULL)
			{
				hParentItem = hItem;
				pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			}
		}
	}
	
	if(hParentItem != NULL)
	{
		CString strText;
		if( pValue != NULL )
		{
			CExpression* pExpression = pValue->GetExp();
			strText.Format( "%s", strName);
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				strName.Format("%s-%s",strText,pExpression->GetDescribe());
			}

			HTREEITEM hCurItem = InsertItem(strName, 1, 1,hParentItem);
			pTreeGroup = new CTreeGroup(pValue/*pExpression*/,hx_GroupItem,hCurItem); // ??
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hCurItem,(DWORD)pTreeGroup);
			this->SelectItem(hCurItem);
		}

		Expand(hParentItem,TVE_EXPAND);

		// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
		if(pMain != NULL)
		{
			//pMain->CopyTree();
			pMain->CopyToFloatTree();
		}
#endif
		return TRUE;
	}

	return FALSE;
}

HTREEITEM CTreeCtrlFormula::InputItem(CString strRootName,int nType,CString strSub,CExpValue* pValue,
								 HTREEITEM hItem /*= NULL*/,DWORD dStyle /*= 0*/)
{
	HTREEITEM hParentItem = (hItem != NULL?hItem:GetRootTree(strRootName));
	if(hParentItem == NULL)
	{
		hParentItem = InsertItem(strRootName, CExpression::HS_Exp_Root, CExpression::HS_Exp_Root,TVI_ROOT,TVI_LAST);
		SetItemState(hParentItem, TVIS_BOLD, TVIS_BOLD);
	}

	HTREEITEM hSub = NULL;
	if( !strSub.IsEmpty() )
	{
		hSub = GetTreeItem(strSub,hParentItem);
		if(hSub == NULL && hSub != hParentItem)
		{
			hSub = InsertItem(strSub, CExpression::HS_Exp_Group, CExpression::HS_Exp_Group, hParentItem);
			SetItemState(hSub, TVIS_BOLD, TVIS_BOLD);
			CTreeGroup* pTreeGroup = new CTreeGroup(NULL,hx_AddGroup,hSub);
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hSub,(DWORD)pTreeGroup);
		}
	}
	if(hSub != NULL)
	{
		hParentItem = hSub;
	}
	if(hParentItem != NULL)
	{
		CString strText,strName;
		if( pValue != NULL )
		{
			CExpression* pExpression = pValue->GetExp();
// 			CMapVariabile* pval =  pExpression->GetParamVar();
// 			CArray<CValue*, CValue*>* pValue =  pval->GetObjArray();
// 			CNumericValue* pNum = (CNumericValue*)pValue->GetAt(0);
// 			double max = pNum->GetMaxValue();
			strText.Format( "%s", pExpression->GetName() );
			if( /*!IsType(CTreeCtrlFormula::Export) &&*/ !pExpression->GetDescribe().IsEmpty() )
			{
				strText.Format("%s  %s",pExpression->GetName(),pExpression->GetDescribe());
			}

			HTREEITEM hCurItem = InsertItem(strText, CExpression::GetImgIndex(nType),
				CExpression::GetImgIndex(nType),hParentItem);
			//
			if( (IsType(CTreeCtrlFormula::Export) || (dStyle & TVIS_BOLD)) && 
				 CExpression::m_pExternExpression != NULL )
			{
				CValue* valoare;
				if( CExpression::m_pExternExpression->Lookup( pExpression->GetName(),
					pExpression->GetExpressType(),valoare ) )
				{
					SetItemState(hCurItem, TVIS_BOLD, TVIS_BOLD);
				}
			}
			//
			CTreeGroup* pTreeGroup = new CTreeGroup(pValue/*pExpression*/,hx_GroupItem,hCurItem);
			m_ayTreeGroup.Add(pTreeGroup);
			SetItemData(hCurItem,(DWORD)pTreeGroup);

			if( pExpression && 
				pExpression->IsStyle(HS_EXP_DEFAULT_TECH) )
			{
			    SetItemState(hCurItem, TVIS_BOLD, TVIS_BOLD);
			}

			return hCurItem;
		}

		return hParentItem;
	}

	return NULL;
}

HTREEITEM CTreeCtrlFormula::InputFunItem(CString strRootName,int nImg,CString strSub,CFunExpresion* pValue,
									HTREEITEM hItem /*= NULL*/)
{
	HTREEITEM hParentItem = (hItem != NULL?hItem:GetRootTree(strRootName));
	if(hParentItem == NULL)
	{
		hParentItem = InsertItem(strRootName, 0, 0);
		SetItemState(hParentItem, TVIS_BOLD, TVIS_BOLD);
	}

	HTREEITEM hSub = NULL;
	if( !strSub.IsEmpty() )
	{
		hSub = GetTreeItem(strSub,hParentItem);
		if(hSub == NULL && hSub != hParentItem)
		{
			hSub = InsertItem(strSub, 2, 2, hParentItem);
			SetItemState(hSub, TVIS_BOLD, TVIS_BOLD);
			//SetItemData(hSub,(DWORD)NULL);
		}
	}
	if(hSub != NULL)
	{
		hParentItem = hSub;
	}
	if(hParentItem != NULL)
	{
		CString strText,strName;
		if( pValue != NULL )
		{
			strText.Format( "%s", pValue->GetFunName() );
			if( !pValue->GetDescribe().IsEmpty() )
			{
				strName.Format("%s-%s",strText,pValue->GetDescribe());
				strText = strName;
			}

			HTREEITEM hCurItem = InsertItem(strText, 4, 4,hParentItem);
			//SetItemData(hCurItem,(DWORD)pValue);

			return hCurItem;
		}

		return hParentItem;
	}

	return NULL;
}

HTREEITEM CTreeCtrlFormula::AddItem(CString strName,char cType,
						  HTREEITEM hItem,CMapVariabile* pExternExpression)
{	
	//
	if( CExpression::m_pExternExpression == NULL )
		return NULL;

	HTREEITEM hParentItem = NULL;
	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem) )
		hParentItem = hItem;
	else
	{
		hItem = GetParentItem(hItem);
		hParentItem = hItem;
		pTreeGroup = (CTreeGroup*)GetItemData(hItem);
		while( !(pTreeGroup == NULL || !pTreeGroup->IsStatus(hx_GroupItem)) )
		{				
			hItem = GetParentItem(hItem);
			if(hItem != NULL)
			{
				hParentItem = hItem;
				pTreeGroup = (CTreeGroup*)GetItemData(hItem);
			}
		}
	}
	
	if(hParentItem != NULL)
	{
		CValue* pVal;
		if ( !pExternExpression->Lookup(strName,pVal) ) // 外部变量表
			return NULL;
		
		HTREEITEM hFindItem = FindNameInGroup(strName,hItem);

		CString strText;
		CExpression* pExpression;
		CExpValue* pValue = (CExpValue*)pVal;
		pExpression = pValue->GetExp();
		HTREEITEM hCurItem = NULL;
		if( pExpression != NULL )
		{
			strText.Format( "%s", strName);
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				strName.Format("%s-%s",strText,pExpression->GetDescribe());
			}
			
			if(hFindItem != NULL)
			{
				if( GetItemText(hFindItem).CompareNoCase(strName) )
				{
					SetItemText(hFindItem,strName);
				}
				pExpression->SetExpressType(cType);

				hCurItem = hFindItem;
			}
			else
			{				
				hCurItem = InsertItem(strName, 1, 1,hParentItem);
				pExpression->SetExpressType(cType);
				pTreeGroup = new CTreeGroup(pValue/*pExpression*/,hx_GroupItem,hCurItem);
				m_ayTreeGroup.Add(pTreeGroup);
				SetItemData(hCurItem,(DWORD)pTreeGroup);
			}
		}
		
		Expand(hParentItem,TVE_EXPAND);

		// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
		CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
		if(pMain != NULL)
		{
			//pMain->CopyTree();
			pMain->CopyToFloatTree();
		}
#endif
		return hCurItem;
	}

	return NULL;
}

BOOL CTreeCtrlFormula::UpDateItem(CString strName,HTREEITEM hItem,CMapVariabile* pExternExpression)
{
	// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		if(pMain->GetIndividualInfo() != NULL)
		{
			CTreeCtrlFormula* pTree = pMain->GetIndividualInfo();
			if(pTree != NULL && pTree != this )
			{
				pTree->UpDateItem(strName,hItem,pExternExpression);
			}
		}
	}
#endif
	// 
	if( CExpression::m_pExternExpression == NULL )
		return FALSE;

	if(hItem != NULL)
	{
		CValue* pVal;
		if ( !pExternExpression->Lookup(strName,pVal) ) // 外部变量表
			return FALSE;
		
		HTREEITEM hFindItem = FindNameInGroup(strName,hItem);
		if( hFindItem == NULL ) // 没找到,则添加
		{
			return FALSE;
		}

		CString strText;
		CExpression* pExpression;
		CExpValue* pValue = (CExpValue*)pVal;
		pExpression = pValue->GetExp();
		if( pExpression != NULL )
		{
			strText.Format( "%s", strName);
			if( !pExpression->GetDescribe().IsEmpty() )
			{
				strName.Format("%s-%s",strText,pExpression->GetDescribe());
			}
			SetItemText(hItem,strName);
		}
		Expand(hItem,TVE_EXPAND);
		return TRUE;
	}
	return FALSE;
}

BOOL CTreeCtrlFormula::UpdateTreeItemData(CValue* pOldVal,CValue* pNewVal)
{
	// 浮动窗口
#ifdef _SUPPORT_FLOAT_WIN_
	CDlgFormulaMan* pMain = (CDlgFormulaMan*)AfxGetMainWnd();
	if(pMain != NULL)
	{
		if(pMain->GetIndividualInfo() != NULL)
		{
			CTreeCtrlFormula* pTree = pMain->GetIndividualInfo();
			if(pTree != NULL && pTree != this)
			{
				pTree->UpdateTreeItemData(pOldVal,pNewVal);
			}
		}
	}
#endif

	//
	CExpression* pOldExpression = ((CExpValue*)pOldVal)->GetExp();
	CExpression* pNewExpression = ((CExpValue*)pNewVal)->GetExp();

	pNewExpression->SetExpressType(pOldExpression->GetExpressType());

	CTreeGroup*  pTreeGroup;
	for(int i = 0; i < m_ayTreeGroup.GetSize(); i++)
	{
		pTreeGroup = m_ayTreeGroup.GetAt(i);
		if( pTreeGroup->GetData() == pOldExpression )
		{
			pTreeGroup->m_pData = pNewVal;
		}
	}
	return TRUE;
}

void CTreeCtrlFormula::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
	
	if( IsType(CTreeCtrlFormula::AlignWin) )
	{
		HTREEITEM hHitItem = HitTest( point, &nFlags );
		if(hHitItem != NULL)
		{
			SelectItem(hHitItem);
			
			//	
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hHitItem);
			if( pTreeGroup != NULL &&
				pTreeGroup->IsStatus(hx_GroupItem) &&
				pTreeGroup->GetData() != NULL)
			{
				::SendMessage(g_hParentWnd,HX_USER_DRAGDATA, HSCurKeyInfo::PreSynchronization, 0);
				::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
					HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/,(LPARAM)pTreeGroup->GetData());
			}
			else
			{
		  // 指标组合图，现在不使用。

			}
			CWnd* pWnd = CWnd::FromHandle(g_hParentWnd);
			if(pWnd != NULL)
			{
				pWnd->SetFocus();
			}
		}
	}
	else if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) ||
			 IsType(CTreeCtrlFormula::Export) )
	{
		HTREEITEM hHitItem = HitTest( point, &nFlags );
		if(hHitItem != NULL)
		{
			SelectItem(hHitItem);
		}

		//AddFunToEdit();
	}
}

void CTreeCtrlFormula::AddItem(HTREEITEM hSrcTItem,HTREEITEM hDestTItem,HTREEITEM hSrcDestTItem )
{
	int nImage;
	int nSelImage;
	while (	(hSrcTItem != NULL) && (hSrcTItem != hSrcDestTItem)) 
	{
		CString csItem = GetItemText(hSrcTItem);
		GetItemImages(hSrcTItem, nImage, nSelImage);
		
		LPARAM param = GetItemData(hSrcTItem);
		
		HTREEITEM hDestChildItem = AddItem(hDestTItem, csItem, TVI_LAST, 
											nImage, nSelImage, param);
		HTREEITEM hSrcChildItem = GetChildItem(hSrcTItem);
		
		if(hSrcChildItem != NULL) 
		{
			AddItem(hSrcChildItem, hDestChildItem,hSrcDestTItem);
		}
		
		hSrcTItem = GetNextSiblingItem(hSrcTItem);
		hDestTItem = GetParentItem(hDestChildItem);
	}	
}

void CTreeCtrlFormula::GetItemImages(HTREEITEM hSrcTItem, int &nSelItemImage, int &nNonSelItemImage)
{	
	CImageList *pImageList = GetImageList(TVSIL_NORMAL);
	if (pImageList == NULL )
	{
		nSelItemImage =0;
		nNonSelItemImage = 0;
	}
	else
	{
		if ( GetImageList(TVSIL_NORMAL) == NULL )
		{
			SetImageList(pImageList, TVSIL_NORMAL);
		}
		GetItemImage(hSrcTItem, nSelItemImage, nNonSelItemImage);
	}
}

void CTreeCtrlFormula::Serialize(CArchive& ar,int& nCountRef)
{
	HTREEITEM hRootItem;
	if (ar.IsStoring())
	{
		hRootItem = GetRootItem();

		while(hRootItem != NULL)
		{
			LoadItemData status;
			status.strName.Format("%s",GetItemText(hRootItem));			
			status.nExpressType = GetType(status.strName);

			status.nStatus = hx_RootGroup;
			GetItemImages(hRootItem, status.nSelItemImage, status.nNonSelItemImage);

			if( IsType(Export) ) // checkbox tree
			{
				if( TreeViewGetCheckState(hRootItem) )
				{
					status.Serialize(ar);
					nCountRef ++;
				}
				else if( IsSubCheck(hRootItem) )
				{
					status.Serialize(ar);
					nCountRef ++;
				}
			}
			else
			{
				status.Serialize(ar);
				nCountRef ++;
			}

			Save(ar,hRootItem,&status,nCountRef);

			hRootItem = GetNextSiblingItem( hRootItem );
		}
	}
	else
	{
	}
}

HTREEITEM CTreeCtrlFormula::Read(HTREEITEM hParentItem,LoadItemData& status,int nCopy /*= CopyNone*/,
								 DWORD dwStyle /*= 0*/)
{
	CExpValue* pValue = status.pExpValue;
	if( !pValue && 
		!(status.nStatus == hx_RootGroup || status.nStatus == hx_AddGroup) )
	{
		return NULL;
	}

	CString strSub,strRootName;
	if(status.nStatus == hx_RootGroup)
	{
		strRootName = status.strName;
	}
	else if( status.nStatus == hx_AddGroup)
	{
		strSub = status.strName;
	}

	if( IsType(CTreeCtrlFormula::Export) )
	{
		if( m_pInExpression != NULL && pValue)
		{
			CString strName;
			CExpression* pExpression = pValue->GetExp();
			if(pExpression != NULL)
			{
				strName = pExpression->GetName();
			}
			if( !strName.IsEmpty() )
			{
				CValue* valoare = NULL;
				if( m_pInExpression->Lookup(strName,
					pExpression->GetExpressType(),valoare) ) // over
				{
					((CExpValue*)valoare)->Clean();
					((CExpValue*)valoare)->Serialize(pValue->GetExp());
					
					if(pValue != NULL) // 
					{
						delete pValue;
						pValue = NULL;
					}
					pValue = (CExpValue *)valoare;  // 
				}
				else
				{
					//(*m_pInExpression)[strName] = pValue;
					m_pInExpression->AddExpress(strName,pValue);
				}
			}
		}
		else
		{
			if(pValue != NULL)
			{
				delete pValue;
				pValue = NULL;
			}
		}
	}
	else
	{
		if( CExpression::m_pExternExpression != NULL && pValue)
		{
			CString strName;
			CExpression* pExpression = pValue->GetExp();
			if(pExpression != NULL)
			{
				strName = pExpression->GetName();
			}
			if( !strName.IsEmpty() )
			{
				CValue* valoare = NULL;
				if( CExpression::m_pExternExpression->Lookup(strName,
					pExpression->GetExpressType(),valoare) ) // over
				{
					((CExpValue*)valoare)->Clean();
					((CExpValue*)valoare)->Serialize(pValue->GetExp());
					
					if(pValue != NULL) // 
					{
						delete pValue;
						pValue = NULL;
					}
					pValue = (CExpValue *)valoare;  //

					//CExpression::m_pExternExpression->RemoveKey(strName);
				}
				else
				{
					//(*CExpression::m_pExternExpression)[strName] = pValue;
					CExpression::m_pExternExpression->AddExpress(strName,pValue);
				}
			}
		}
		else
		{
			if(pValue != NULL)
			{
				delete pValue;
				pValue = NULL;
			}
		}
	}
	
	return InputItem(strRootName,status.nExpressType,strSub,pValue,hParentItem,
		IsType(CTreeCtrlFormula::Export)?TVIS_BOLD:0);

}

HTREEITEM CTreeCtrlFormula::Load(HTREEITEM hParentItem,LoadItemData& status,int nCopy,DWORD dwStyle /*= 0*/)
{
	CString strName = status.strName;

	CValue* pVal;
	BOOL bFind = CExpression::m_pExternExpression->Lookup(strName,(status).nExpressType,pVal);
	if( !bFind )
	{
		if( !(status.nStatus == hx_RootGroup || status.nStatus == hx_AddGroup) )
		{
			return NULL;
		}
	}

	HTREEITEM hCurItem;
	if ( CExpression::m_pExternExpression == NULL || 
		 !bFind ) // 外部变量表
	{
		hCurItem = InsertItem(status.strName, status.nSelItemImage, status.nNonSelItemImage,hParentItem);
		CTreeGroup* pTreeGroup = new CTreeGroup(NULL,status.nStatus,hCurItem);
		m_ayTreeGroup.Add(pTreeGroup);
		SetItemData(hCurItem,(DWORD)pTreeGroup);
		return hCurItem;
	}
		
	CString strText;
	CExpression* pExpression;
	CExpValue* pValue = (CExpValue*)pVal;
	pExpression = pValue->GetExp();
	int nImage;
	if( pExpression != NULL )
	{
		// 公式属性
		if( (nCopy & 0xFFFF) > 0 && !pExpression->IsStyle(nCopy & 0xFFFF) )
		{
			return NULL;
		}
		// 是否为指定风格，否则为NULL
		if( dwStyle > 0 && !pExpression->IsStyle(dwStyle) )
		{
			return NULL;
		}

		strText.Format( "%s", strName);
		if( !pExpression->GetDescribe().IsEmpty() )
		{
			strName.Format("%s-%s",strText,pExpression->GetDescribe());
		}

		nImage   = CExpression::GetImgIndex(pExpression->GetExpressType());
		if(nImage != 0)
		{
			status.nSelItemImage = status.nNonSelItemImage = nImage;
		}

		hCurItem = InsertItem(strName, status.nSelItemImage, status.nNonSelItemImage,hParentItem);
		CTreeGroup* pTreeGroup = new CTreeGroup(pValue/*pExpression*/,status.nStatus,hCurItem);
		m_ayTreeGroup.Add(pTreeGroup);
		SetItemData(hCurItem,(DWORD)pTreeGroup);
		if (status.state != 0 && status.stateMask != 0)
		{
		   SetItemState(hCurItem, status.stateMask,status.state);
		}

		return hCurItem;
	}
	
	return NULL;
}

void CTreeCtrlFormula::Save(CArchive& ar,HTREEITEM hRootItem,LoadItemData* status,int& nCountRef)
{
	if (ar.IsStoring()) // save
	{
		if( hRootItem != NULL )
		{
			HTREEITEM hChildItem = GetChildItem( hRootItem );
			while(hChildItem != NULL)
			{
				status->nStatus = hx_AddGroup;
				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hChildItem);
				if( pTreeGroup != NULL && 
					pTreeGroup->IsStatus(hx_GroupItem) &&
					pTreeGroup->GetData() != NULL )
				{
					status->pExpValue = (CExpValue*)pTreeGroup->m_pData;
					status->nStatus	  = pTreeGroup->m_nStatus;
					status->strName.Format("%s",pTreeGroup->GetData()->GetName());

					pTreeGroup->GetData()->SetExpressType(status->nExpressType);
				}
				else
				{
					status->pExpValue = NULL;
					status->strName.Format("%s",GetItemText(hChildItem));
				}				
				GetItemImages(hRootItem, status->nSelItemImage, status->nNonSelItemImage);
				
				//
				if( IsType(Export) )
				{
					if( pTreeGroup != NULL && 
						pTreeGroup->IsStatus(hx_GroupItem) )
					{
						if( TreeViewGetCheckState(hChildItem) )
						{
							status->Serialize(ar);
							nCountRef ++;
						}
					}
					else if( IsSubCheck(hChildItem) )
					{
						status->Serialize(ar);
						nCountRef ++;
					}
				}
				else
				{
					status->Serialize(ar);
					nCountRef ++;
				}

				//
				Save(ar,hChildItem,status,nCountRef);

				hChildItem = GetNextSiblingItem( hChildItem );
			}
		}
	}
	else
	{
	
	}
}

BOOL CTreeCtrlFormula::SaveLoad(UINT nMode,CString strFileName/*= NULL*/,
						   CDlgFormulaMan* pFormulaMan /*= NULL*/,DWORD dStyle /*= 0*/)
{
	CFile fp;
	if(strFileName.IsEmpty())
	{
		strFileName =  CGeneralHelper::GetSpecifyPath(CGeneralHelper::SETTING_PATH) +m_strFileName+".exp";
	}

	CString strTempName;

	BOOL bOpen = FALSE;
	if(nMode == CArchive::store) // 存储
	{
		if(IsType(CTreeCtrlFormula::AlignWin))
			return FALSE;

		if(this->GetCount() <= 0)
			return FALSE;

		CString strTempPath;
		GetTempPath (MAX_PATH, strTempPath.GetBuffer (MAX_PATH));
		strTempPath.ReleaseBuffer();
				
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
			head.m_strHead = m_strName;
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
		
		ULONGLONG lLen = fp.GetLength();
		if(lLen > 0)
		{
			char* pData = new char[lLen];
			fp.Read(pData,lLen);
			fp.Close();

			SaveTransZipData* pTransZipData = zipData(pData,lLen);
			if( pTransZipData )
			{
				CFile curFile;
				if( curFile.Open( strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary|CFile::modeNoTruncate ))
				{
					curFile.SetLength(0);
					curFile.Write(pTransZipData,sizeof(SaveTransZipData));
					curFile.Write(pTransZipData->m_cData,pTransZipData->m_lZipLen);
					curFile.Close();
				}

				pTransZipData->Free();
				delete pTransZipData;
			}
			else
			{
				CFile curFile;
				if( curFile.Open( strFileName, CFile::modeReadWrite | CFile::shareDenyNone | CFile::typeBinary|CFile::modeNoTruncate  ) )
				{
					curFile.SetLength(0);
					curFile.Write(pData,lLen);
					curFile.Close();
				}				
			}
			
			delete[] pData;
		}		
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

		SaveTransZipData ZipData;
		if( fp.Read(&ZipData,sizeof(ZipData)) == sizeof(ZipData) )
		{
			if( !strncmp(ZipData.m_szHeader,Express_Header,strlen(Express_Header)) )
			{
				bOpen = 0;

				int nLen = fp.GetLength();
				nLen -= sizeof(ZipData);
				if( nLen <= 0 )				
					return FALSE;

				ZipData.m_cData = new char[nLen];
				if( fp.Read(ZipData.m_cData,nLen) == nLen )
				{
					fp.Close();

					CString strTempPath;
					GetTempPath (MAX_PATH, strTempPath.GetBuffer (MAX_PATH));
					strTempPath.ReleaseBuffer();
					
					GetTempFileName (strTempPath, _T("hsExp"), 0, strTempName.GetBuffer (MAX_PATH));
					strTempName.ReleaseBuffer ();

					char* punzipData = unzipData(&ZipData);
					if( punzipData &&
						fp.Open( strTempName, CFile::typeBinary | CFile::shareDenyNone | CFile::modeCreate |
									  CFile::modeReadWrite, &fileEx) )
					{
						fp.Write(punzipData,ZipData.m_lOrigLen);
						fp.Close();

						bOpen = fp.Open( strTempName, CFile::typeBinary | 
							CFile::shareDenyNone | CFile::modeReadWrite, &fileEx);
						delete[] punzipData;
					}					
				}		

				ZipData.Free();

			}
			else
			{
				fp.SeekToBegin();
			}
		}

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

			CTreeCtrlFormula* pTree = NULL;
			if(pFormulaMan != NULL)
			{
				pTree = pFormulaMan->GetCurTree(head.m_strHead);
				if(pTree == NULL)
					return FALSE;
			}
			else
			{
				pTree = this;
			}

			HTREEITEM hRootItem = NULL;
			HTREEITEM hParentItem = NULL;

			if(head.m_lCount < 0)
			{
				return FALSE;
			}
			
			CExpValue* pExpValue;
			CExpression* pExpress;

/*// gbq 
static CFile *file = NULL;
if(file == NULL)
{
	CString s;
	int i = 0;
	int p = 0;
	p = strFileName.Find("\\", i);
	while( p > 0 )
	{
		i = p;
		p = strFileName.Find("\\", i+1);
	}
	s = strFileName.Mid(i + 1);
	s += "express.txt";
	s = "c:\\" + s;
	file = new CFile(s, CFile::modeCreate |CFile::modeWrite);
}
static CString str;
str = "";
*/
			for(int i = 0; i < head.m_lCount; i++)
			{
				LoadItemData status;
				status.Serialize(ar);

				pExpValue = status.pExpValue;
				if(pExpValue != NULL)
				{
					pExpress = pExpValue->GetExp();
/*
// gbq			
str += pExpress->GetName();
str += "{  \r\n";
str += pExpress->GetNotes();
str += "\r\n";
str += "} \r\n";
*/
				}
				else
				{
					pExpress = NULL;
				}

				// 临时引入
				if( dStyle & HS_EXP_TEMPIN )
				{
					if(pExpress != NULL)
					{
						pExpress->AddStyle(dStyle & HS_EXP_TEMPIN);
					}
				}

				// def fun
				if( pExpress != NULL && 
					pExpress->IsStyle(HS_EXP_DEFAULT_TECH) && 
					CDlgFormulaMan::m_pOpeDefTechFun != NULL &&
					!IsType(Export) )
				{
					(*CDlgFormulaMan::m_pOpeDefTechFun)(pExpValue,FALSE,NULL,'+');
				}

				//
				if( status.nStatus == hx_RootGroup )	 // 根
				{
					hParentItem = hRootItem = pTree->Read(NULL,status);
				}
				else if( status.nStatus == hx_AddGroup ) // 组
				{
					hParentItem = pTree->Read(hRootItem,status);
				}
				else // 项
				{
					pTree->Read(hParentItem,status);
				}
			}
/*
file->Write((void*)str.GetBuffer(), str.GetLength());
file->Close();
delete file;
file = NULL;
*/
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

		fp.Close();
	}

	// delete
	if( !strTempName.IsEmpty() )
	{
		TRY
		{
			CFile::Remove( strTempName );
		}
		CATCH( CFileException, e )
		{
		}
		END_CATCH
	}

	return TRUE;

}

HTREEITEM CTreeCtrlFormula::GetRootTree(CString strName)
{
	if( strName.IsEmpty() )
		strName = m_strName;

	HTREEITEM hRootItem = GetRootItem();
	CString strText;
	while(hRootItem != NULL)
	{
		strText = GetItemText(hRootItem);
		strText.Trim();
		if( !strText.CompareNoCase(_T(strName)) )
		{
			return hRootItem;
		}
		hRootItem = GetNextSiblingItem( hRootItem );
	}
	return NULL;
}

void CTreeCtrlFormula::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	if( IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::Export) )
	{
		if( ((*pTVDispInfo).item).hItem != NULL && ((*pTVDispInfo).item).pszText != NULL )
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData( ((*pTVDispInfo).item).hItem );
			if( pTreeGroup == NULL )
			{
				*pResult = 1;
				return;
			}
			if( pTreeGroup->IsStatus(hx_GroupItem) && IsType(CTreeCtrlFormula::Export) && m_pInExpression == NULL )
			{
				*pResult = 1;
				return;
			}
			{
				if( pTreeGroup->IsStatus(hx_GroupItem) ) // 公式
				{
					CExpression* pExpress = pTreeGroup->GetData();
					if(pExpress != NULL)
					{
						CEdit* pEdit = (CEdit*)GetEditControl();
						if(pEdit != NULL)
						{
							pEdit->SetWindowText(pExpress->GetName());
						}
						//((*pTVDispInfo).item).pszText = (char*)LPCSTR(pExpress->GetName());
					}
				}
			}
		}
	}

	*pResult = 0;
}

void CTreeCtrlFormula::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	if(IsType(AlignWin) ||  IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::Export) )
	{
		if( ((*pTVDispInfo).item).hItem != NULL && ((*pTVDispInfo).item).pszText != NULL )
		{
			CString strNewName = ((*pTVDispInfo).item).pszText;
			strNewName.MakeUpper();

			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData( ((*pTVDispInfo).item).hItem );
			if( pTreeGroup != NULL )
			{
				int nState = 0;
				if( pTreeGroup->IsStatus(hx_AddGroup) ) // 组
				{
					nState = 1;	
				}
				else if( pTreeGroup->IsStatus(hx_GroupItem) ) // 公式
				{
					CExpValue* pValue = (CExpValue*)pTreeGroup->m_pData;
					if( pValue != NULL )
					{
						CMapVariabile* pExternExpression = (IsType(CTreeCtrlFormula::Express)?CExpression::m_pExternExpression:m_pInExpression);
						nState = pValue->ReName(strNewName,pExternExpression);
						if( !((nState == CExpValue::Succ ) || (nState == CExpValue::SysExist)) )
							nState = 0;
					}
				}

				if(nState)
				{
					if( pTreeGroup->IsStatus(hx_GroupItem) )
					{
						CExpression* pExpress = pTreeGroup->GetData();
						if(pExpress != NULL)
						{
							strNewName.Format("%s  %s",pExpress->GetName(),pExpress->GetDescribe());
						}
					}

					SetItemText( ((*pTVDispInfo).item).hItem,strNewName );

					if( nState == CExpValue::Succ )
					{
						nState = GetItemState( ((*pTVDispInfo).item).hItem,TVIF_STATE|TVIS_BOLD );
						nState &= (~TVIS_BOLD);
						SetItemState( ((*pTVDispInfo).item).hItem, nState, TVIF_STATE|TVIS_BOLD );
					}
				}
			}
		}
	}

	*pResult = 0;
}

BOOL CTreeCtrlFormula::IsUsed(HTREEITEM hRootItem,CTreeGroup* pCurGroup)
{
	if( hRootItem != NULL )
	{
		HTREEITEM hChildItem = GetChildItem( hRootItem );
		while(hChildItem != NULL)
		{
			if( pCurGroup->Compare((CTreeGroup*)GetItemData(hChildItem)) )
				return TRUE;
						
			if( IsUsed(hChildItem,pCurGroup) )
				return TRUE;
			hChildItem = GetNextSiblingItem( hChildItem );			
		}
	}
	return FALSE;
}

BOOL CTreeCtrlFormula::IsUsed(CTreeGroup* pCurGroup)
{
	HTREEITEM hRecycleBin = GetRootTree(g_strRecycleBin);
	HTREEITEM hRootItem = GetRootItem();
	while(hRootItem != NULL && hRootItem != hRecycleBin )
	{
		if( pCurGroup->Compare((CTreeGroup*)GetItemData(hRootItem)) )
			return TRUE;

		if( IsUsed(hRootItem, pCurGroup) )
			return TRUE;

		hRootItem = GetNextSiblingItem( hRootItem );
	}
	return FALSE;
}

HTREEITEM CTreeCtrlFormula::GetCurSavePos(HTREEITEM hCurItem)
{
	HTREEITEM hSelItem = GetSelectedItem();
	if(hCurItem == NULL)
	{
		return hSelItem;
	}

	HTREEITEM hSelSaveItem = hSelItem;

	HTREEITEM hSelParent = NULL;
	while( hSelItem != NULL )
	{
		hSelParent = hSelItem;
		hSelItem   = GetParentItem(hSelItem);
	}
	if( hSelParent == hCurItem && hSelParent != NULL )
	{
		CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hSelSaveItem);
		if(pTreeGroup == NULL || pTreeGroup->IsStatus(hx_AddGroup) )
		{
			return hSelSaveItem;
		}
		else
		{
			hSelSaveItem = GetParentItem(hSelSaveItem);
			if(hSelSaveItem != NULL)
			{
				return hSelSaveItem;
			}
		}
	}
	return hCurItem;
}

BOOL CTreeCtrlFormula::MouseDown(UINT nFlags, CPoint point)
{
	if(g_hDragDataCursor == NULL)
	{
		g_hDragDataCursor = AfxGetApp()->LoadCursor(IDR_DRAGDATA);
	}
	if(g_hDragErrorCursor == NULL)
	{
		g_hDragErrorCursor = AfxGetApp()->LoadCursor(IDC_NODROP);
	}

	//
	CPoint pt = point;
	this->ScreenToClient(&pt);
	HTREEITEM   hitemDrag  = HitTest(pt, &nFlags);
	HTREEITEM   hitemDrop  = NULL;
	HTREEITEM   hitem;

	if(hitemDrag == NULL) return FALSE;

	CRect  rect;
	

	CRect HSRect; //by HS 020401
	
	CTreeGroup* pTreeGroup = NULL;
	if( IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::AlignWin) )
	{
		pTreeGroup = (CTreeGroup*)GetItemData(hitemDrag);
		if( pTreeGroup != NULL &&
			!pTreeGroup->IsStatus(hx_GroupItem) )
		{
			return FALSE;
		}
	
		if( !GetItemRect(hitemDrag,rect,TRUE) )
			return FALSE;
		this->ClientToScreen(&rect);

		
		HSRect = rect; //by HS 020401
			
		if( !rect.PtInRect(point) )
		{
			return FALSE;
		}
	}

	CImageList* pImageList = NULL;
	if( IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::AlignWin) )
	{
		pImageList = CreateDragImage(hitemDrag);
	}
/*	if(pImageList != NULL) 
	{
		pImageList->DragShowNolock(TRUE);
		pImageList->SetDragCursorImage(0, CPoint(0, 0));
		pImageList->BeginDrag(0, CPoint(0,0));
		pImageList->DragMove(pt);
		pImageList->DragEnter(this, pt);
	}
*/	GetWindowRect(rect);

	::SetCapture(m_hWnd);

	int     curX = 0,curY = 0;
	int     xDiff = 0,yDiff = 0;
		
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != this)
			break;

		switch (msg.message) 
		{
		case WM_MOUSEMOVE:
			{
				if(msg.wParam == MK_LBUTTON)
				{
					::GetCursorPos(&point);

					//by HS 20020401
					if( HSRect.PtInRect(point))
					{
						break;
					}
					if(pImageList != NULL) 
					{
						pImageList->DragShowNolock(TRUE);
						pImageList->SetDragCursorImage(0, CPoint(0, 0));
						pImageList->BeginDrag(0, CPoint(0,0));
						pImageList->DragMove(pt);
						pImageList->DragEnter(this, pt);
					}
					//end
					if( rect.PtInRect(point) )
					{
						//::SetCursor(g_hDragErrorCursor);
					}
					else
					{
						::SetCursor(g_hDragDataCursor);
					}
					
					if( pImageList != NULL && 
						( IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::AlignWin) ) )
					{
						pt = point;
						ScreenToClient(&pt);			
						pImageList->DragMove(pt);
						if ( (hitem = HitTest(pt, &nFlags)) != NULL )
						{
							pImageList->DragLeave(this);
							
							pTreeGroup = (CTreeGroup*)GetItemData(hitem);
							if( pTreeGroup != NULL &&
								pTreeGroup->IsStatus(hx_AddGroup) )
							{
								SelectDropTarget(hitem);
							}
							//
							HTREEITEM hVisible;
							if( curY < (int)(short)HIWORD(msg.lParam) )
							{
								hVisible = GetNextVisibleItem(hitem);
							}
							else
							{
								hVisible = GetFirstVisibleItem();
							}
							if(hVisible != NULL)
							{
								EnsureVisible(hVisible);
							}

							//
							hitemDrop = hitem;
							pImageList->DragEnter(this, pt);
						}
					}

					curX = (int)(short)LOWORD(msg.lParam);
					curY = (int)(short)HIWORD(msg.lParam);			
					xDiff = curX - point.x;
					yDiff = curY - point.y;
				}
				else
				{
					goto ExitLoop;
				}
			}
	        break;
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
            goto ExitLoop;
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
    ::ReleaseCapture();
	
	if(pImageList != NULL)
	{
		pImageList->DragLeave(this);
		pImageList->EndDrag();
		delete pImageList;
		pImageList = NULL;
	}
	
	if(xDiff == 0 || yDiff == 0)
		return FALSE;

	::GetCursorPos(&point);
	if( rect.PtInRect(point) )
	{
		if( IsType(CTreeCtrlFormula::Express) || IsType(CTreeCtrlFormula::AlignWin) )
		{
			if(hitemDrop == NULL) return FALSE;
			
			pTreeGroup = (CTreeGroup*)GetItemData(hitemDrop);
			if( pTreeGroup == NULL ||
				!pTreeGroup->IsStatus(hx_AddGroup) )
			{
				return FALSE;
			}
			
			if ( (hitemDrag != hitemDrop) && 
				!IsChildNodeOf(hitemDrop, hitemDrag) &&
				GetParentItem(hitemDrag) != hitemDrop )
			{
				TransferItem(hitemDrag, hitemDrop);
				DeleteItem(hitemDrag);
			}
			else
			{
				MessageBeep(0);
			}
		}

		return FALSE;
	}

	return TRUE;
}

BOOL CTreeCtrlFormula::TransferItem(HTREEITEM hitemDrag, HTREEITEM hitemDrop)
{
	TV_INSERTSTRUCT     tvstruct;
	TCHAR               sztBuffer[128];
	HTREEITEM           hNewItem, hFirstChild;

		// avoid an infinite recursion situation
	tvstruct.item.hItem = hitemDrag;
	tvstruct.item.cchTextMax = 127;
	tvstruct.item.pszText = sztBuffer;
	tvstruct.item.mask = TVIF_STATE | TVIF_PARAM | TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	GetItem(&tvstruct.item);  // get information of the dragged element

	tvstruct.hParent = hitemDrop;
	tvstruct.hInsertAfter = TVI_SORT;
	tvstruct.item.mask = TVIF_STATE | TVIF_PARAM | TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;//TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	hNewItem = InsertItem(&tvstruct);
	this->SelectItem(hNewItem);

	while ((hFirstChild = GetChildItem(hitemDrag)) != NULL)
	{
		TransferItem(hFirstChild, hNewItem);  // recursively transfer all the items
		DeleteItem(hFirstChild);			  // delete the first child and all its children
	}
	return TRUE;
}

BOOL CTreeCtrlFormula::ChangItemToGroup(HTREEITEM hitemDrag, HTREEITEM hitemDrop,
								   CString strInsertName /*= ""*/,
								   DWORD dwMask /*= TVE_EXPAND*/)
{
	if( hitemDrag == NULL )
		return FALSE;

	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hitemDrag);
	if( pTreeGroup == NULL || 
		!pTreeGroup->IsStatus(hx_GroupItem) ) // not item
	{
		return FALSE;
	}

	if( hitemDrop != NULL )
	{
		pTreeGroup = (CTreeGroup*)GetItemData(hitemDrop);
		if( pTreeGroup == NULL || 
			!pTreeGroup->IsStatus(hx_AddGroup) ) // not group
		{
			return FALSE;
		}

		HTREEITEM hParentItem = this->GetParentItem(hitemDrag);
		if( hParentItem == hitemDrop )
		{
			return TRUE;
		}
	}
	else
	{
		if( strInsertName.IsEmpty() )
			return FALSE;
		hitemDrop = this->NewGroup(strInsertName);
		if( hitemDrop == NULL )
			return FALSE;
	}

	TV_INSERTSTRUCT     tvstruct;
	TCHAR               sztBuffer[128];
	HTREEITEM           hNewItem;

	tvstruct.item.hItem      = hitemDrag;
	tvstruct.item.cchTextMax = 127;
	tvstruct.item.pszText    = sztBuffer;
	tvstruct.item.mask		 = TVIF_STATE | TVIF_PARAM | TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	GetItem(&tvstruct.item);

	tvstruct.hParent	  = hitemDrop;
	tvstruct.hInsertAfter = TVI_SORT;
	tvstruct.item.mask    = TVIF_STATE | TVIF_PARAM | TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;//TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	
	hNewItem = InsertItem(&tvstruct); // add new
	pTreeGroup = (CTreeGroup*)(((tvstruct).item).lParam);
	if( pTreeGroup )
	{
		pTreeGroup->m_hTree = hNewItem;
	}

	this->SelectItem(hNewItem);       
	
	DeleteItem(hitemDrag); // del old
	
	if( dwMask & TVE_EXPAND )
	{
		Expand(hNewItem,TVE_EXPAND);
	}

	return TRUE;
}

BOOL CTreeCtrlFormula::IsChildNodeOf(HTREEITEM hitemChild, HTREEITEM hitemSuspectedParent)
{
	do
	{
		if (hitemChild == hitemSuspectedParent)
			break;
	}
	while ((hitemChild = GetParentItem(hitemChild)) != NULL);

	return (hitemChild != NULL);
}

char CTreeCtrlFormula::GetType(HTREEITEM& hCurItem)
{
	char cType = none;
	if(hCurItem != NULL)
	{
		cType = GetType(GetItemText(hCurItem));
		if( cType != none )
		{
			return cType;
		}
		HTREEITEM hParentItem = GetParentItem(hCurItem);
		while(hParentItem)
		{
			cType = GetType(GetItemText(hParentItem));
			if( cType != none )
			{
				hCurItem = hParentItem;
				return cType;
			}
			hParentItem = GetParentItem(hParentItem);
		}
	}
	return none;
}

char CTreeCtrlFormula::GetType()
{
	if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strTech) )
	{
		return Tech;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strCondition) )
	{
		return Condition;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strExchange) )
	{
		return Exchange;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strStrategy) ) //20090302 YJT 增加 Strategy
	{
		return Strategy;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strColorKLine) )
	{
		return MoreKLine;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strUnionCondition) )
	{
		return Union;
	}
	else if( !m_strName.CompareNoCase(CTreeCtrlFormula::g_strRecycleBin) )
	{
		return ExpressBin;
	}
	return none;
}

char CTreeCtrlFormula::GetType(CString strName)
{
	if( !strName.CompareNoCase(CTreeCtrlFormula::g_strTech) )
	{
		return Tech;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strCondition) )
	{
		return Condition;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strExchange) )
	{
		return Exchange;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strStrategy) ) //20090302 YJT 增加 Strategy
	{
		return Strategy;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strColorKLine) )
	{
		return MoreKLine;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strUnionCondition) )
	{
		return Union;
	}
	else if( !strName.CompareNoCase(CTreeCtrlFormula::g_strRecycleBin) )
	{
		return ExpressBin;
	}
	return none;
}

BOOL CTreeCtrlFormula::IsEditExpress(HTREEITEM hHitItem)
{
	if( hHitItem == NULL )
	{
		return FALSE;
	}

	CTreeGroup* pGroup = (CTreeGroup*)GetItemData(hHitItem);
	if( pGroup != NULL && pGroup->IsStatus(hx_GroupItem) )
	{
		hHitItem = this->GetParentItem(hHitItem);
		if( hHitItem != NULL )
		{
			HTREEITEM hParentHitItem = this->GetParentItem(hHitItem);
			if( hParentHitItem )
			{
				hHitItem = hParentHitItem;
			}
		}
	}
	else if( pGroup != NULL && pGroup->IsStatus(hx_AddGroup) )
	{
		hHitItem = this->GetParentItem(hHitItem);
	}
	
	if( hHitItem != NULL )
	{
		char cType = this->GetType(GetItemText(hHitItem));
		if((cType == none || cType > MoreKLine ) && cType != Strategy ) //20090302 YJT 增加 Strategy
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CTreeCtrlFormula::DeleteTree()
{
	Delete();
	if( IsType(CTreeCtrlFormula::AlignWin) )
	{
		HTREEITEM hRootItem = GetRootItem();
		HTREEITEM hRootItemDel;
		CString strText;
		while( hRootItem != NULL )
		{
			strText.Format("%s",GetItemText(hRootItem));			
			if( GetType(strText) == none )
			{
				hRootItem = GetNextSiblingItem( hRootItem );
				continue;
			}
			hRootItemDel = hRootItem;
			hRootItem = GetNextSiblingItem( hRootItem );
			DeleteItem(hRootItemDel);
		}
	}
	else
	{
		DeleteAllItems();
	}
}

void CTreeCtrlFormula::CopyTree(CTreeCtrlFormula* pTree,int nCopy/* = CopyNone*/,DWORD dwStyle /*= 0*/)
{
	if( pTree == NULL )
		return;

	/*if( pTree->IsType( CTreeCtrlFormula::HSBrowser ) )
	{
	}
	else */if( ((nCopy & CopyNone)   == CopyNone || 
		 (nCopy & CopyDelete) == CopyDelete ) && 
		!pTree->IsType(CTreeCtrlFormula::Express) )
		return;

	if( (nCopy & CopyDelete) == CopyDelete )
	{
		DeleteTree();
	}
	
	HTREEITEM hRootItem = pTree->GetRootItem();
	HTREEITEM hDesRootItem;
	while(hRootItem != NULL)
	{
		LoadItemData status;
		status.strName.Format("%s",pTree->GetItemText(hRootItem));			
		status.nExpressType = pTree->GetType(status.strName);
		if(status.nExpressType == ExpressBin)
		{
			hRootItem = pTree->GetNextSiblingItem( hRootItem );
			continue;
		}

		status.nStatus = hx_RootGroup;
		pTree->GetItemImages(hRootItem, status.nSelItemImage, status.nNonSelItemImage);

		if( (nCopy & CopyLoad) == CopyLoad )
		{
			hDesRootItem = Read(NULL,status,nCopy,dwStyle);
		}
		else
		{
	//		if( (nCopy & CopyNotGroup) == CopyNotGroup ) // 不添加组
	//		{
	//			hDesRootItem = NULL;
	//		}
	//		else
			if(status.nExpressType != none && (nCopy & CopyNoRoot))
			{
				hDesRootItem = NULL;
			}
			else
			{
				hDesRootItem = Load(NULL,status,nCopy,dwStyle);
			}
		}
		Copy(pTree,hRootItem,&status,hDesRootItem,nCopy,dwStyle);
		
		hRootItem = pTree->GetNextSiblingItem( hRootItem );
	}	
}

void CTreeCtrlFormula::LoadTree(CDlgFormulaMan* pFormulaMan,WORD wStyle)
{
	if( !pFormulaMan || this->GetCount() <= 0 )
		return;
	
	HTREEITEM hRootItem = GetRootItem();
	while(hRootItem != NULL)
	{
		LoadItemData status;
		status.strName.Format("%s",GetItemText(hRootItem));			
		status.nExpressType = GetType(status.strName);
		if(status.nExpressType == ExpressBin)
		{
			hRootItem = GetNextSiblingItem( hRootItem );
			continue;
		}

		status.strName.Empty();

		status.nStatus = hx_RootGroup;
		GetItemImages(hRootItem, status.nSelItemImage, status.nNonSelItemImage);
		
		if( IsType(Export) ) // checkbox tree
		{
			if( wStyle & EXTERN_EXPRESS_AUTO_SETUP ) // // 自动安装公式
			{
				pFormulaMan->AddExpress(&status,wStyle);
			}
			else if( TreeViewGetCheckState(hRootItem) )
			{
				pFormulaMan->AddExpress(&status,wStyle);
			}
			else if( IsSubCheck(hRootItem) )
			{
				pFormulaMan->AddExpress(&status,wStyle);
			}
		}
		else
		{
			pFormulaMan->AddExpress(&status,wStyle);
		}

		Load(pFormulaMan,wStyle,hRootItem,&status);
		
		hRootItem = GetNextSiblingItem( hRootItem );
	}
}

void CTreeCtrlFormula::Load(CDlgFormulaMan* pFormulaMan,WORD wStyle,HTREEITEM hRootItem,LoadItemData* status)
{
	if( hRootItem != NULL )
	{
		HTREEITEM hChildItem = GetChildItem( hRootItem );
		HTREEITEM hParentItem;
		while(hChildItem != NULL)
		{
			status->nStatus = hx_AddGroup;
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hChildItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_GroupItem) &&
				pTreeGroup->GetData() != NULL )
			{
				status->nStatus = pTreeGroup->m_nStatus;
				hParentItem = GetParentItem(hChildItem);
				if( hParentItem != NULL )
				{
					status->strName.Format("%s",GetItemText(hParentItem));
				}
				else
				{
					status->strName.Format("%s",pTreeGroup->GetData()->GetName());
				}
				pTreeGroup->GetData()->SetExpressType(status->nExpressType);
			}
			else
			{
				status->strName.Format("%s",GetItemText(hChildItem));
			}				
			GetItemImages(hRootItem, status->nSelItemImage, status->nNonSelItemImage);

			//
			if( pTreeGroup != NULL )
			{
				status->pExpValue = (CExpValue*)pTreeGroup->m_pData;
			}
			else
			{
				status->pExpValue = NULL;
			}

			if( IsType(Export) ) // checkbox tree
			{
				if( wStyle & EXTERN_EXPRESS_AUTO_SETUP ) // // 自动安装公式
				{
					pFormulaMan->AddExpress(status,wStyle);
				}
				else if( TreeViewGetCheckState(hChildItem) )
				{
					pFormulaMan->AddExpress(status,wStyle);
				}
				else if( IsSubCheck(hChildItem) )
				{
					pFormulaMan->AddExpress(status,wStyle);
				}
			}
			else
			{
				pFormulaMan->AddExpress(status,wStyle);
			}
			
			//
			Load(pFormulaMan,wStyle,hChildItem,status);
			hChildItem = GetNextSiblingItem( hChildItem );
		}
	}
}

void CTreeCtrlFormula::Copy(CTreeCtrlFormula* pTree,HTREEITEM hRootItem,LoadItemData* status,
					   HTREEITEM hDesRootItem,int nCopy,DWORD dwStyle /*= 0*/)
{
	if( hRootItem != NULL )
	{
		HTREEITEM hChildItem  = pTree->GetChildItem( hRootItem );
		HTREEITEM hParentItem = NULL;
		while(hChildItem != NULL)
		{
			status->nStatus = hx_AddGroup;
			CTreeGroup* pTreeGroup = (CTreeGroup*)pTree->GetItemData(hChildItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_GroupItem) &&
				pTreeGroup->GetData() != NULL )
			{
				status->nStatus = pTreeGroup->m_nStatus;
				status->strName.Format("%s",pTreeGroup->GetData()->GetName());

				pTreeGroup->GetData()->SetExpressType(status->nExpressType);
				status->state = pTree->GetItemState(hChildItem, status->stateMask);
			}
			else
			{
				status->strName.Format("%s",pTree->GetItemText(hChildItem));
			}				
			pTree->GetItemImages(hChildItem, status->nSelItemImage, status->nNonSelItemImage);

			if( (nCopy & CopyLoad) == CopyLoad )
			{
				if( pTreeGroup != NULL )
				{
					status->pExpValue = (CExpValue*)pTreeGroup->m_pData;
				}
				else
				{
					status->pExpValue = NULL;
				}
				if( status->nStatus == hx_AddGroup ) // 组
				{
					hParentItem = Read(hDesRootItem,*status,nCopy,dwStyle);
				}
				else // 项
				{
					Read(hDesRootItem,*status,nCopy,dwStyle);
				}
			}			
			else
			{				
				if( status->nStatus == hx_AddGroup ) // 组
				{
					if( (nCopy & CopyNotGroup) == CopyNotGroup ) // 不添加组
					{
						hParentItem = hDesRootItem;
					}
					else
					{
						hParentItem = Load(hDesRootItem,*status,nCopy,dwStyle);
					}
				}
				else // 项
				{
					Load(hDesRootItem,*status,nCopy,dwStyle);
				}
			}
			
			Copy(pTree,hChildItem,status,hParentItem,nCopy,dwStyle);
			
			hChildItem = pTree->GetNextSiblingItem( hChildItem );
		}
	}
}

void CTreeCtrlFormula::AddTreeGroupToMenu(UINT nID,CMenu& popup)
{
	CString strParentName,strName;
	HTREEITEM hRootItem = GetRootItem();
	while(hRootItem != NULL)
	{
		strParentName.Format("%s",GetItemText( hRootItem ));

		if( !strParentName.CompareNoCase(CTreeCtrlFormula::g_strRecycleBin) )
		{
			hRootItem = GetNextSiblingItem( hRootItem );
			continue;
		}

		HTREEITEM hChildItem = GetChildItem( hRootItem );
		while( hChildItem != NULL )
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hChildItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_AddGroup) )
			{
				strName.Format("%s:%s",strParentName,GetItemText( hChildItem ));
				popup.AppendMenu(MF_STRING,nID++,strName);
			}

			hChildItem = GetNextSiblingItem( hChildItem );
		}

		hRootItem = GetNextSiblingItem( hRootItem );

		nID += 100;
	}
}

HTREEITEM CTreeCtrlFormula::GetTreeItem(CString strRefName,HTREEITEM pRootTree)
{
	BOOL bFind = (pRootTree == NULL);
	if(bFind)
		pRootTree = GetRootItem();
	CString strText;
	while( pRootTree != NULL )
	{
		if( !GetItemText(pRootTree).CompareNoCase(CTreeCtrlFormula::g_strRecycleBin) )
		{
			pRootTree = GetNextSiblingItem( pRootTree );
			continue;
		}

		HTREEITEM hChildItem = GetChildItem( pRootTree );
		while( hChildItem != NULL )
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hChildItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_AddGroup) )
			{
				strText = GetItemText(hChildItem);
				if( !strText.CompareNoCase(strRefName) )
				{
					return hChildItem;
				}
			}
			hChildItem = GetNextSiblingItem( hChildItem );
		}
		if(bFind)
			pRootTree = GetNextSiblingItem( pRootTree );
		else
			break;
	}
	return NULL;
}

void CTreeCtrlFormula::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTreeCtrl::OnChar(nChar, nRepCnt, nFlags);
}

DWORD CTreeCtrlFormula::GetItemData(HTREEITEM hItem)
{
	if( !::IsWindow(m_hWnd) )
		return 0;

	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_PARAM;

	if( !::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&item) )
		return 0;

	return (DWORD)item.lParam;
}

BOOL CTreeCtrlFormula::OneItemInOtherItem(HTREEITEM hCurItem,HTREEITEM hDesItem)
{
	if( hCurItem == hDesItem )
		return TRUE;
	HTREEITEM hChildItem = GetChildItem( hDesItem );
	while( hChildItem != NULL )
	{
		if(hChildItem == hCurItem)
			return TRUE;

		if(OneItemInOtherItem(hCurItem,hChildItem))
			return TRUE;

		hChildItem = GetNextSiblingItem( hChildItem );
	}
	return FALSE;
}

void CTreeCtrlFormula::Save(HTREEITEM hRootItem,int nExpressType)
{
	if( hRootItem != NULL )
	{
		HTREEITEM hChildItem = GetChildItem( hRootItem );
		while(hChildItem != NULL)
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hChildItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_GroupItem) &&
				pTreeGroup->GetData() != NULL )
			{
				pTreeGroup->GetData()->SetExpressType(nExpressType);
			}
			Save(hChildItem,nExpressType);
			
			hChildItem = GetNextSiblingItem( hChildItem );
		}
	}
}

void CTreeCtrlFormula::Save(int nExpressType,void* pData)
{
    SaveLoad(CArchive::store);
}

BOOL CTreeCtrlFormula::IsTree(CString strName)
{
	return !m_strName.CompareNoCase(strName);
}

void CTreeCtrlFormula::OnKillFocus( CWnd* pNewWnd )
{
	if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) )
	{
		this->ShowWindow(SW_HIDE);
	}

	CTreeCtrl::OnKillFocus( pNewWnd );
}

BOOL CTreeCtrlFormula::TreeViewSetCheckState(HTREEITEM hItem, BOOL fCheck)
{
    TVITEM tvItem;

    tvItem.mask = TVIF_HANDLE | TVIF_STATE;
    tvItem.hItem = hItem;
    tvItem.stateMask = TVIS_STATEIMAGEMASK;

    /*Image 1 in the tree view check box image list is the
    unchecked box. Image 2 is the checked box.*/
    
    tvItem.state = INDEXTOSTATEIMAGEMASK( fCheck );

    return TreeView_SetItem(m_hWnd, &tvItem);
}

BOOL CTreeCtrlFormula::TreeViewGetCheckState( HTREEITEM hItem)
{
    TVITEM tvItem;

    // Prepare to receive the desired information.
    tvItem.mask = TVIF_HANDLE | TVIF_STATE;
    tvItem.hItem = hItem;
    tvItem.stateMask = TVIS_STATEIMAGEMASK;

    // Request the information.
    TreeView_GetItem(m_hWnd, &tvItem);

    // Return zero if it's not checked, or nonzero otherwise.
    return ((BOOL)(tvItem.state >> 12) - 1);
}

HTREEITEM CTreeCtrlFormula::InsertItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hItem = CTreeCtrl::InsertItem( lpInsertStruct );
	return hItem;
}

HTREEITEM CTreeCtrlFormula::InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
						int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
						HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hItem = CTreeCtrl::InsertItem( nMask, lpszItem, nImage,
						nSelectedImage, nState, nStateMask, lParam,
						hParent, hInsertAfter);
	return hItem;
}

HTREEITEM CTreeCtrlFormula::InsertItem(LPCTSTR lpszItem, HTREEITEM hParent /*= TVI_ROOT*/,
						 HTREEITEM hInsertAfter/* = TVI_LAST*/)
{
	HTREEITEM hItem = CTreeCtrl::InsertItem(lpszItem, hParent,hInsertAfter);
	return hItem;
}

HTREEITEM CTreeCtrlFormula::InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
						 HTREEITEM hParent /*= TVI_ROOT*/, HTREEITEM hInsertAfter /*= TVI_SORT*/)
{
	HTREEITEM hItem = CTreeCtrl::InsertItem(lpszItem, nImage, nSelectedImage,hParent, hInsertAfter );
	//TreeViewSetCheckState(hItem,TRUE);
	return hItem;
}

void CTreeCtrlFormula::CheckParent(HTREEITEM hCurItem)
{
	if( hCurItem != NULL )
	{
		int nCode;
		int nCount = 0;
		int nCheckCount = 0;
		int nWaitCheck = 0;
		IsSiblingItemCheck(hCurItem,nCount,nCheckCount,nWaitCheck);
		if(nCheckCount == 0 && nWaitCheck == 0)
		{
			nCode = 1;
		}
		else if(nCount == nCheckCount)
		{
			nCode = 2;
		}
		else
		{
			nCode = 3;
		}

		HTREEITEM hParenItem = GetParentItem( hCurItem );
		if( hParenItem != NULL )
		{
			TreeViewSetCheckState(hParenItem,nCode);
			//hParenItem = GetParentItem( hParenItem );

			if(hParenItem != NULL)
			{
				HTREEITEM hItem = GetParentItem(hParenItem);
				if(hItem != NULL)
				{
					hItem = GetChildItem(hItem);
					CheckParent(hItem);
				}
			}
		}
	}		
}

void CTreeCtrlFormula::Check(HTREEITEM hCurItem, UINT nCode)
{
	if( hCurItem == NULL )
	{
		HTREEITEM hRootItem = GetRootItem();
		while(hRootItem != NULL)
		{
			TreeViewSetCheckState(hRootItem,nCode);

			Check(hRootItem,nCode);
			hRootItem = GetNextSiblingItem( hRootItem );
		}
	}
	else
	{
		if( hCurItem != NULL )
		{
			// gbq add, 20060207下面这句
			if( nCode == 1 || ExPassword(hCurItem) )
				TreeViewSetCheckState(hCurItem,nCode);

			HTREEITEM hChildItem = GetChildItem( hCurItem );
			while(hChildItem != NULL)
			{
				// gbq modify, 20060208, 注释下面三句
				// gbq add, 20060207下面这句
				//if( ExPassword(hCurItem) || nCode == 0 )
				//	TreeViewSetCheckState(hChildItem,nCode);

				Check(hChildItem,nCode);
				hChildItem = GetNextSiblingItem( hChildItem );
			}
		}
	}
}

int CTreeCtrlFormula::IsSiblingItemCheck(HTREEITEM hCurItem,int& nCount,int& nCheckCount,int& nWaitCheck)
{
	BOOL bCheck = 0;
	//
	while( hCurItem != NULL )
	{
		nCount++;
		bCheck = TreeViewGetCheckState(hCurItem);
		if( bCheck == 1 )
		{
			nCheckCount++;
		}
		else if( bCheck == 2 )
		{
			nWaitCheck++;
		}
		
		hCurItem = GetNextSiblingItem(hCurItem);
		return IsSiblingItemCheck(hCurItem,nCount,nCheckCount,nWaitCheck);
	}

	return nCheckCount;
}


BOOL CTreeCtrlFormula::IsSubCheck( HTREEITEM hCurItem )
{
	if( hCurItem != NULL )
	{
		if ( TreeViewGetCheckState( hCurItem ) )
			return 1;
		
		HTREEITEM hChildItem = GetChildItem( hCurItem );
		while(hChildItem != NULL)
		{
			if( TreeViewGetCheckState( hChildItem ) )
				return 1;
			hChildItem = GetNextSiblingItem( hChildItem );
			
			return IsSubCheck(hChildItem);
		}
	}		

	return 0;
}

void CTreeCtrlFormula::Check(HTREEITEM hTSelItem)
{
	BOOL bCheck = TreeViewGetCheckState(hTSelItem);
	switch(bCheck)
	{
	case 0:
		bCheck = 2;
		break;
	case 1:
		bCheck = 1;
		break;
	case 2:
		bCheck = 2;
		break;
	}
	
	Check(hTSelItem,bCheck);

	hTSelItem = GetParentItem(hTSelItem);
	hTSelItem = GetChildItem(hTSelItem);
	CheckParent(hTSelItem);
}

void CTreeCtrlFormula::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CWnd* pWnd = DYNAMIC_DOWNCAST(CWnd, GetParent());
	if(pWnd && (nChar <= VK_SPACE || nChar >= VK_SELECT)
		&& nChar != VK_RETURN && nChar != VK_DELETE)
	{ 
		pWnd->SendMessage(WM_KEYDOWN,nChar,MAKELONG(nRepCnt,nFlags));
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);	
		return;
	}

	switch( nChar )
	{
	case VK_SPACE:
		{
			HTREEITEM hItem = GetSelectedItem();
			if(hItem != NULL)
			{
				Check(hItem);
			}
		}
		return;
	case VK_F2:
		EditLabel(GetSelectedItem());
		break;
	case VK_ESCAPE:
		{
			if( IsType(CTreeCtrlFormula::EditFunRef) || IsType(CTreeCtrlFormula::EditExpRef) )
			{
				//this->ShowWindow(SW_HIDE);
				CWnd* pWnd = GetParent();
				if(pWnd != NULL)
				{
					if( ((CDlgFormulaEdit*)pWnd)->m_pRichEdit != NULL )
					{
						((CDlgFormulaEdit*)pWnd)->m_pRichEdit->SetFocus();
					}
				}
			}
		}
		break;
	case VK_RETURN:
		{
			if( IsType(CTreeCtrlFormula::AlignWin) )
			{
 				HTREEITEM hItem = GetSelectedItem();

				CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
				if( pTreeGroup != NULL &&
					pTreeGroup->IsStatus(hx_GroupItem) &&
					pTreeGroup->GetData() != NULL)
				{
					::SendMessage(g_hParentWnd,HX_USER_DRAGDATA, HSCurKeyInfo::PreSynchronization, 0);
					::SendMessage(g_hParentWnd,HX_USER_DRAGDATA,
						HSCurKeyInfo::UpdateExpressData/*HX_USER_DRAGDATA-1*/,(LPARAM)pTreeGroup->GetData());					
				}
				else
				{
					UINT nState = GetItemState(hItem, TVIF_STATE);
					UINT nCode = TVE_EXPAND;

					if(nState & TVIS_EXPANDED)
					{
						nCode = TVE_COLLAPSE;
					}

					Expand(hItem, nCode);
					EnsureVisible(hItem);	
				}		
			}
			else
			{
				AddFunToEdit();
			}
		}
		break;
	case VK_DELETE:
		if( IsType(CTreeCtrlFormula::Express) )
		{
			if( IsSHIFTpressed() && IsCTRLpressed() )
			{
				HTREEITEM hItem = GetSelectedItem();
				if(hItem == NULL)
				{
					return;
				}
				
				CString strName = GetItemText(hItem);
				CString strPrompt;
				strPrompt.Format(_T("您真的确定删除\"%s\"？(y/n)"),strName);
				int nID = MessageBox(strPrompt,_T("提示："),
					MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
				if(nID != IDYES)
				{
					return;
				}
				
				DelAll(hItem,FALSE);
			}
		}
		break;
	case VK_LEFT:
	case VK_RIGHT:
		if( m_pFormulaMan != NULL && m_pFormulaMan->Key(nChar) )
			return;
		break;
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);	
}

long CTreeCtrlFormula::OperatorTree(DWORD dOperator,HTREEITEM hChildItem /*= NULL*/) 
{
	if(dOperator == 0) return -1;

	HTREEITEM hCurItem;
	if(hChildItem == NULL)
	{
		hCurItem = GetRootItem();
	}
	else
	{
		hCurItem = GetChildItem(hChildItem);
	}
	long lRet = 0;
	while(hCurItem != NULL)
	{
		if(dOperator & EXTERN_EXPRESS_SAME_NAME)
		{
			CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hCurItem);
			if( pTreeGroup != NULL && 
				pTreeGroup->IsStatus(hx_GroupItem) )
			{
				if( IsType(Export) && 					
					( (dOperator & EXTERN_EXPRESS_AUTO_SETUP) || TreeViewGetCheckState(hCurItem)) )
				{					
					CExpression* pExpress = pTreeGroup->GetData();
					if( pExpress != NULL )
					{
						CValue* valoare;
						if( CExpression::m_pExternExpression->Lookup( pExpress->GetName(),pExpress->GetExpressType(),valoare ) )
						{
							Expand(hCurItem,TVE_EXPAND);
							Select(hCurItem,TVGN_CARET);
							Select(hCurItem,TVGN_FIRSTVISIBLE);
							//SelectItem(hCurItem);
							return EXTERN_EXPRESS_SAME_NAME;
						}
					}
				}
			}
		}

		lRet = OperatorTree(dOperator,hCurItem);
		if( lRet )
		{
			return lRet;
		}
		
		hCurItem = GetNextSiblingItem( hCurItem );
	}

	return 0;
}

CTreeGroup*	CTreeCtrlFormula::Find(CExpression* pExpression,DWORD dMask/* = 0*/)
{
	CTreeGroup* pTreeGroup;
	HTREEITEM   hTree;
	for( int i = 0; i < m_ayTreeGroup.GetSize(); i++ )
	{
		pTreeGroup = m_ayTreeGroup.GetAt(i);
		if( pTreeGroup != NULL && 
			pTreeGroup->GetData() == pExpression )
		{
			if( dMask & TVE_EXPAND )
			{
				hTree = pTreeGroup->m_hTree;
				if(hTree != NULL)
					hTree = this->GetParentItem(hTree);

				if( hTree != NULL )
				{
					Expand(hTree,TVE_EXPAND);
					EnsureVisible(pTreeGroup->m_hTree);
					SelectItem(pTreeGroup->m_hTree);
				}
			}

			return pTreeGroup;
		}
	}

	return NULL;
}

void CTreeCtrlFormula::Show(CExpression* pExpression,DWORD dMask)
{
	CTreeGroup* pTreeGroup;
	HTREEITEM   hTree;
	CExpression* pCurExpression;
	for( int i = 0; i < m_ayTreeGroup.GetSize(); i++ )
	{
		pTreeGroup = m_ayTreeGroup.GetAt(i);
		if( pTreeGroup != NULL )
		{
			pCurExpression = pTreeGroup->GetData();
		
			if( pCurExpression != NULL &&
				(pCurExpression == pExpression || pExpression == NULL ) )
			{
				hTree = pTreeGroup->m_hTree;
				if(hTree != NULL)
					hTree = this->GetParentItem(hTree);

				if(hTree != NULL)
				{
					Expand(hTree,TVE_EXPAND);
					EnsureVisible(pTreeGroup->m_hTree);
					SelectItem(pTreeGroup->m_hTree);
					//Select(pTreeGroup->m_hTree,TVGN_CARET);
				}
				return;
			}
		}
	}
}

void CTreeCtrlFormula::OnDelUnion()
{
	if( !IsType(CTreeCtrlFormula::HSBrowser) )
		return;

	HTREEITEM hItem = GetSelectedItem();
	if( hItem == NULL )
		return;
	
	UnionSelCondition* pTreeGroup = (UnionSelCondition*)GetItemData(hItem);

	CWnd* pWnd  = this->GetParent();
	if( pWnd != NULL )
	{
		pWnd = pWnd->GetParent();
		if( pWnd != NULL )
		{
			HTREEITEM hParentItem = this->GetParentItem(hItem);
			HTREEITEM hRootItem   = GetRootTree(CTreeCtrlFormula::g_strUnionCondition);
			if( pTreeGroup && hParentItem == hRootItem )
			{
				CValue* valoare;
				if( CExpression::m_pExternExpression->Lookup( pTreeGroup->m_strName,
					Union,valoare ) )
				{
					delete valoare;
				}
				else
				{
					delete pTreeGroup;
				}

				DeleteItem(hItem);
				return;
			}
		}
	}
}

HTREEITEM CTreeCtrlFormula::NewGroup(CString strGroupName, bool bprompt /*= TRUE*/)
{
	HTREEITEM hParentItem = this->GetRootTree("");
	if( hParentItem != NULL )
	{
		HTREEITEM hFindItem = FindGroupName(strGroupName,false);
		if( hFindItem != NULL )
			return hFindItem;

		HTREEITEM hCurItem	   = InsertItem(strGroupName, 2, 2, hParentItem);
		CTreeGroup* pTreeGroup = new CTreeGroup(NULL,hx_AddGroup,hCurItem);
	
		m_ayTreeGroup.Add(pTreeGroup);
		SetItemData(hCurItem,(DWORD)pTreeGroup);

		return hCurItem;
	}
	
	return NULL;
}

long CTreeCtrlFormula::GetHasExpressType(long* pJiben /*= NULL*/,long* pUnion /*= NULL*/)
{
	long lExpressType    = 0;

	if( pJiben ) *pJiben = 0;
	if( pUnion ) *pUnion = 0;

	if( pJiben && GetRootTree(g_strJiben) != NULL )
	{
		*pJiben = JiBenMian;
	}
	if( pUnion && GetRootTree(g_strUnionCondition) != NULL )
	{
		*pUnion = Union;
	}

	if( GetRootTree(g_strTech) != NULL )
	{
		lExpressType |= Tech;
	}
	if( GetRootTree(g_strCondition) != NULL )
	{
		lExpressType |= Condition;
	}
	if( GetRootTree(g_strExchange) != NULL )
	{
		lExpressType |= Exchange;
	}
	if( GetRootTree(g_strColorKLine) != NULL )
	{
		lExpressType |= MoreKLine;
	}
	if( GetRootTree(g_strStrategy) != NULL ) //20090302 YJT 增加 Strategy
	{
		lExpressType |= Strategy;
	}

	return lExpressType;
}

extern CMapStringToPtr* g_pTableHeadData;
void CTreeCtrlFormula::InitCWData(/*CComboBox* pBox,*/CString strName) 
{
	if( g_pTableHeadData )
	{
		void* pValue;
		//	strName = GB_GUBEN_NAME;
		//	CString strName = Cj_GUBEN_NAME;
		CMapWordToPtr* pMap;
		if( g_pTableHeadData->Lookup(strName,pValue) )
		{
			pMap = (CMapWordToPtr*)pValue;
			if( !pMap->IsEmpty() )
			{
				TableColProperty* pCol;
				POSITION pos = pMap->GetStartPosition();
				WORD wKey;
				while(pos)
				{
					pMap->GetNextAssoc(pos, wKey, pValue);
					if(pValue != NULL)
					{
						pCol = (TableColProperty*)pValue;

						CTreeCtrlFormula* pCurTree = this;
						if( pCurTree == NULL )
							return ;
						HTREEITEM hRootItem = pCurTree->GetRootTree(CTreeCtrlFormula::g_strJiben);
						if( hRootItem == NULL )
							return ;
						HTREEITEM hItem;
						hItem = pCurTree->InsertItem( pCol->strName,CExpression::HS_Exp_Condition,
							CExpression::HS_Exp_Condition,hRootItem);
						CTreeGroup* pTreeGroup = new CTreeGroup(hx_OtherData,(DWORD)wKey);
						pCurTree->SetItemData(hItem,(DWORD)pTreeGroup);
					}
				}
			}
		}
	}
}

void CTreeCtrlFormula::OnMouseMove(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnMouseMove(nFlags, point); 
	
	if(IsWindowVisible() && IsType(CTreeCtrlFormula::AlignWin))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
}

void CTreeCtrlFormula::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef _HS_COMPLIE_INCLUDE_INFO_MODULE_ /* 支持资讯模块的编译开关 */
	if(IsType(CTreeCtrlFormula::AlignWin))
	{
		SendMessage(WM_KEYDOWN, nChar,MAKELONG(0,0xFFFF));
		return;
	}

	CTreeCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
#else	
	CTreeCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
#endif /*_HS_COMPLIE_INCLUDE_INFO_MODULE_*/
}

// gbq add 20060207, 检查密码
BOOL CTreeCtrlFormula::ExPassword(HTREEITEM hItem)
{
	CTreeGroup* pTreeGroup = (CTreeGroup*)GetItemData(hItem);
	if( pTreeGroup )
	{
		CExpression* pExp = (CExpression*)pTreeGroup->GetData();
		if( pExp )
		{
			CString pstrName = pExp->GetName();
			if( !pExp->GetPWD().IsEmpty() )
			{
				CInputPassPwD dlg(this,_T("请输入公式密码"),pstrName);
				dlg.m_pExp = pExp;
				if( dlg.DoModal() == IDOK )		//密码输入正确
					return TRUE;
				else							//密码输入错误
					return FALSE;
			}
			else				//不需要密码
				return TRUE;
		}
		else					//不是公式
			return TRUE;
	}
	return TRUE;
}
