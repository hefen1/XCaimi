// MenuTreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Transaction.h"
#include "MenuTreeDlg.h"
#include "..\..\HelperLib\GeneralHelper.h"
#include "TradeConfigObjIntf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMenuTreeDlg 对话框
#define  IDC_BUTTON_BEGIN		1000
#define  IDC_BUTTON_END			1049
#define  HS_IDW_MENUTRUE		1050
#define  HS_IDW_MENUTRUE_END	1099

CAccountMenuTree::CAccountMenuTree()
{
	m_pAccount = NULL;
}

CAccountMenuTree::~CAccountMenuTree()
{
	for (int i = 0; i < m_MenuTreeList.GetCount(); i++)
	{
		CMenuTree* pTree = m_MenuTreeList.GetAt(i);
		pTree->Release();
	}
	m_MenuTreeList.RemoveAll();
	for (int i = 0; i < m_ButtonList.GetCount(); i++)
	{
		delete m_ButtonList.GetAt(i);
	}
	m_ButtonList.RemoveAll();
}



IMPLEMENT_DYNAMIC(CMenuTreeDlg, CHSBizDialog)

CMenuTreeDlg::CMenuTreeDlg(CWnd* pParent /*=NULL*/)
	: CHSBizDialog(CMenuTreeDlg::IDD, pParent)
{
	m_CurrAccountMenuTree = NULL;
}

CMenuTreeDlg::~CMenuTreeDlg()
{
	m_pAccountMng->UnRegAccountChange(this);
	
	for (int i = 0; i < m_AccountMenuTreeList.GetCount(); i++)
	{
		delete m_AccountMenuTreeList.GetAt(i);
	}
	m_AccountMenuTreeList.RemoveAll();
}

void CMenuTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMenuTreeDlg, CHSBizDialog)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_BEGIN, IDC_BUTTON_END, &CMenuTreeDlg::OnButtonClicked)
	ON_NOTIFY_RANGE(NM_CLICK, HS_IDW_MENUTRUE,HS_IDW_MENUTRUE_END, &CMenuTreeDlg::OnNMClickMenuTree)
	//ON_NOTIFY_RANGE(TVN_SELCHANGED, HS_IDW_MENUTRUE, HS_IDW_MENUTRUE_END,  &CMenuTreeDlg::OnMenuItemChange)
	ON_NOTIFY_RANGE(NM_SETFOCUS, HS_IDW_MENUTRUE, HS_IDW_MENUTRUE_END, &CMenuTreeDlg::OnNMSetfocusTree)
	ON_NOTIFY_RANGE(NM_KILLFOCUS, HS_IDW_MENUTRUE, HS_IDW_MENUTRUE_END, &CMenuTreeDlg::OnNMKillfocusTree)
END_MESSAGE_MAP()


// CMenuTreeDlg 消息处理程序

BOOL CMenuTreeDlg::OnInitDialog()
{
	CHSBizDialog::OnInitDialog();
	//CString strMenuFile = CGeneralHelper::GetAppPath() + _T("config\\trading_menu.xml");
	//TiXmlDocument m_doc;
	//if( m_doc.LoadFile(strMenuFile))
	//{
		/*ITradeLang* pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
		m_ImageList.Create(16, 16, ILC_COLOR24 ,0, 4);
		m_ImageList.SetBkColor(RGB(255, 255, 255));
		for (int i = 1; i < 9; i++)
		{
			HICON h = pTradeLang->LoadHICON(i);
			m_ImageList.Add(h);
		}*/
		
		/*CFont font;
		LOGFONT stFont;
		CDC *pDC = this->GetDC();
		memset(&stFont,0,sizeof(LOGFONT));
		stFont.lfHeight = MulDiv(20,pDC->GetDeviceCaps(LOGPIXELSY),72);//设置字体高度为12
		stFont.lfWeight = FW_NORMAL;
		stFont.lfCharSet = GB2312_CHARSET;
		strcpy(stFont.lfFaceName,"宋体");//设置字样
		font.CreateFontIndirect(&stFont);*/
		//CFont* pFont = GetDlgItem(IDC_BT_FONT)->GetFont();		
		////TiXmlElement* pRoot = m_doc.FirstChildElement("Trading");
		////TiXmlElement* pTradingMenu = pRoot->FirstChildElement("TradingMenu");
		//m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
		//TiXmlElement* pTradingMenu = m_pTradeConfigObj->GetFunctionTree("1");
		//TiXmlElement* pElem = pTradingMenu->FirstChildElement("Item");

		//m_nLastConIndex = 0;
		//while(pElem != NULL)
		//{	
		//	CMenuTree* pTree = new CMenuTree();
		//	pTree->Create(WS_TABSTOP | WS_CHILD | TVS_SHOWSELALWAYS
		//		| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_DISABLEDRAGDROP,
		//		CRect(0, 0, 0, 0), this, HS_IDW_MENUTRUE + m_nLastConIndex);
		//	pTree->SetImageList(&m_ImageList, TVSIL_NORMAL);
		//	pTree->BuildMenuTree(0, pElem->FirstChildElement("Item"));
		//	pTree->SelectItem(pTree->GetRootItem());
		//	m_paMenuTree.Add(pTree);
		//	CString strCaption = pElem->Attribute(_T("caption"));
		//	CHSButton* pButton = new CHSButton();
		//	pButton->Create(strCaption, WS_VISIBLE | WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON, 
		//		CRect(0, 0, 0, 0), this, IDC_BUTTON_BEGIN + m_nLastConIndex);			
		//	pButton->DrawFocus(FALSE);
		//	pButton->SetFont(pFont, TRUE);
		//	//pButton->SetTextFont(9, "宋体");			
		//	m_paButton.Add(pButton);
		//	pElem = pElem->NextSiblingElement();
		//	m_nLastConIndex++;	
		//}
		//m_pCurrButton = (CHSButton *)m_paButton.GetAt(0);
		//m_pCurrButton->SetForeColor(0x0000FF);
		//m_pCurrMenuTree = (CMenuTree *)m_paMenuTree.GetAt(0);
		//m_pCurrMenuTree->ShowWindow(SW_SHOW);
	//}	
	ITradeLang* pTradeLang = (ITradeLang*) CWinnerApplication::GetObject(OBJ_TRADELANG);
	m_ImageList.Create(16, 16, ILC_COLOR24 ,0, 4);
	m_ImageList.SetBkColor(RGB(255, 255, 255));
	for (int i = 1; i < 9; i++)
	{
		HICON h = pTradeLang->LoadHICON(i);
		m_ImageList.Add(h);
	}
	m_pTradeCoreObj = (ITradeCoreObj*) CWinnerApplication::GetObject(OBJ_TRADECORE);
	m_pAccountMng = m_pTradeCoreObj->GetAccountMng();
	m_pAccountMng->RegAccountChange(this);
	m_pTradeConfigObj = (ITradeConfigObj*)CWinnerApplication::GetObject(OBJ_TRADECONFIG);
	m_nLastConIndex = 0;
	/*NewMenuTree(m_pAccountMng->GetCurrentAccount(atTrade));
	m_CurrAccountMenuTree = m_AccountMenuTreeList.GetAt(0);
	m_CurrAccountMenuTree->m_pCurrMenuTree->ShowWindow(SW_SHOW);
	for (int i = 0; i < m_CurrAccountMenuTree->m_ButtonList.GetCount(); i++)
	{
		m_CurrAccountMenuTree->m_ButtonList.GetAt(i)->ShowWindow(SW_SHOW);
	}*/
	return TRUE;
}


void CMenuTreeDlg::OnSize(UINT nType, int cx, int cy)
{
	CHSBizDialog::OnSize(nType, cx, cy);
	OnSizeEx();
}

BOOL CMenuTreeDlg::OnInit()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (__super::OnInit())
	{
		NewMenuTree(m_pAccountMng->GetCurrentAccount(atTrade));
		m_CurrAccountMenuTree = m_AccountMenuTreeList.GetAt(0);
		m_CurrAccountMenuTree->m_pCurrMenuTree->ShowWindow(SW_SHOW);
		for (int i = 0; i < m_CurrAccountMenuTree->m_ButtonList.GetCount(); i++)
		{
			m_CurrAccountMenuTree->m_ButtonList.GetAt(i)->ShowWindow(SW_SHOW);
		}
		OutputDebugString("[CMenuTreeDlg::OnShow] enter...\n");
		RegisterLinkageMsg(TRADE_MESSAGE_OPENLAYOUT);
		RegisterHotKey(1,MOD_CONTROL|MOD_ALT,'D');
		return TRUE;
	}
	else
		return FALSE;
	
}

void CMenuTreeDlg::OnShow(BOOL bShow)
{
	if(bShow == FALSE)
		return;
	OutputDebugString("[CMenuTreeDlg::OnShow] enter...\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	OpenLayOut();
}

void CMenuTreeDlg::OnLinkageMsg(CLinkageParam* pParam)
{
	switch (pParam->m_nMsg)
	{
	case TRADE_MESSAGE_OPENLAYOUT:
		{
			CString sLayOut = pParam->GetStringField("layout");
			CHSParam HSParam;
			POSITION pos;
			CString sKey,sValue;
			for( pos = pParam->GetStartPosition(); pos != NULL; )
			{
				pParam->GetNextAssoc(pos, sKey, sValue);
				HSParam.AddParam(sKey,sValue);
			}
			OpenLayOut(sLayOut, &HSParam);
			break;
		}		
	}	
}

void CMenuTreeDlg::OnNMClickMenuTree(UINT Id,NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenuTree* pTree = m_CurrAccountMenuTree->m_pCurrMenuTree;
	if( pTree->IsBuilding())
		return;

	TVHITTESTINFO ht ; 
	GetCursorPos(&ht.pt) ; 
	pTree->ScreenToClient(&ht.pt) ; 
	ht.flags = TVHT_ABOVE ; 

	HTREEITEM hItem = pTree->HitTest(&ht);
	if(!pTree->ItemHasChildren(hItem))
	{
		ILayoutWindow* pLayOut = this->GetLayoutWindow();
		if(pLayOut != NULL && hItem != NULL)
		{
			CMenuTreeItem* pItem = (CMenuTreeItem*)(pTree->GetItemData(hItem));
			CString strCaption = pItem->GetCaption();
			CString strLayout = pItem->GetLayoutFile();
			/*CMenuTreeItem* pMenuItem = m_pCurrMenuTree->GetMenuTreeItem(strLayout);
			if(pMenuItem)
			{
				pLayOut->OpenLayout(_T("TradeWorkspace"),pMenuItem->GetLayoutFile());
				SendLingLayoutChangeMsg(strCaption);
			}*/
			IAccount* pAccount = m_CurrAccountMenuTree->GetAccount();
			CString strOwner;
			strOwner.Format(_T("%d"),pAccount->GetID());
			pLayOut->OpenLayout(_T("TradeWorkspace"), strLayout,strOwner);
			SendLingLayoutChangeMsg(strCaption);
		}
	}
	*pResult = 0;
}

void CMenuTreeDlg::OnMenuItemChange(UINT Id, NMHDR *pNMHDR, LRESULT *pResult)
{
	OutputDebugString("[CMenuTreeDlg::OnMenuItemChange] enter...\n");
	HTREEITEM h = m_CurrAccountMenuTree->m_pCurrMenuTree->GetSelectedItem();
	if( !m_CurrAccountMenuTree->m_pCurrMenuTree->ItemHasChildren(h) )
	{
		OpenLayOut();
	}
	*pResult = 0;
}

void CMenuTreeDlg::OnNMSetfocusTree(UINT Id, NMHDR *pNMHDR, LRESULT *pResult)
{
	OutputDebugString("[CMenuTreeDlg::OnNMSetfocusTree] enter...\n");
}

void CMenuTreeDlg::OnNMKillfocusTree(UINT Id, NMHDR *pNMHDR, LRESULT *pResult)
{
	OutputDebugString("[CMenuTreeDlg::OnNMKillfocusTree] enter...\n");
}

void CMenuTreeDlg::SendLingLayoutChangeMsg(const CString &strCaption)
{
	//CString sParam = "action_name=";
	//sParam += strCaption;
	CLinkageParam param;
	param.AddParam(PARAM_ACTIONNAME,strCaption);
	this->SendLinkageMsg(TRADE_MESSAGE_ACTIONNAME, &param);
}


void CMenuTreeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CMenuTreeDlg::OnButtonClicked(UINT Id)
{
	CMenuTree* pTree = m_CurrAccountMenuTree->m_pCurrMenuTree;
	pTree->ShowWindow(SW_HIDE);
	//pTree = m_CurrAccountMenuTree->m_MenuTreeList.GetAt(Id - IDC_BUTTON_BEGIN);
	pTree = m_MenuTreeList.GetAt(Id - IDC_BUTTON_BEGIN);
	pTree->ShowWindow(SW_SHOW);
	m_CurrAccountMenuTree->m_pCurrMenuTree = pTree;

	m_CurrAccountMenuTree->m_pCurrButton->SetForeColor(0x000000);
	m_CurrAccountMenuTree->m_pCurrButton->RedrawWindow();
	//CHSButton* pButton = m_CurrAccountMenuTree->m_ButtonList.GetAt(Id - IDC_BUTTON_BEGIN);
	CHSButton* pButton = m_ButtonList.GetAt(Id - IDC_BUTTON_BEGIN);
	pButton->SetForeColor(0x0000FF);
	m_CurrAccountMenuTree->m_pCurrButton = pButton;
	OpenLayOut();
}

void CMenuTreeDlg::OpenLayOut()
{
	CMenuTree* pTree = m_CurrAccountMenuTree->m_pCurrMenuTree;
	if( pTree->IsBuilding())
		return;
	ILayoutWindow* pLayOut = this->GetLayoutWindow();
	if(pLayOut)
	{
		//HTREEITEM h = m_pCurrMenuTree->GetRootItem();
		HTREEITEM h = pTree->GetSelectedItem();
		CMenuTreeItem* pItem = (CMenuTreeItem*)(pTree->GetItemData(h));
		CString strCaption = pItem->GetCaption();
		CString strLayout = pItem->GetLayoutFile();
		/*CMenuTreeItem* pMenuItem = m_pCurrMenuTree->GetMenuTreeItem(strLayout);
		if(pMenuItem)
		{
			pLayOut->OpenLayout(_T("TradeWorkspace"),pMenuItem->GetLayoutFile());
			SendLingLayoutChangeMsg(strCaption);
		}*/
		IAccount* pAccount = m_CurrAccountMenuTree->GetAccount();
		CString strOwner;
		strOwner.Format(_T("%d"),pAccount->GetID());
		pLayOut->OpenLayout(_T("TradeWorkspace"), strLayout,strOwner);
		SendLingLayoutChangeMsg(strCaption);
	}
}

void CMenuTreeDlg::OpenLayOut(CString sLayOut, IHSParam* pParam /* = NULL */)
{
	ILayoutWindow* pLayOut = this->GetLayoutWindow();
	if(pLayOut)
	{
		CMenuTree* pTree = m_CurrAccountMenuTree->m_pCurrMenuTree;
		HTREEITEM h = pTree->GetMenuTreeItem(sLayOut);
		CMenuTreeItem* pMenuItem = (CMenuTreeItem*)pTree->GetItemData(h);
		if(pMenuItem)
		{

			IAccount* pAccount = m_CurrAccountMenuTree->GetAccount();
			CString strOwner;
			strOwner.Format(_T("%d"),pAccount->GetID());
			pLayOut->OpenLayout(_T("TradeWorkspace"), sLayOut,strOwner, pParam);
			CString strCaption = pMenuItem->GetCaption();
			SendLingLayoutChangeMsg(strCaption);
		}
		pTree->SelectItem(h);
	}
}

void CMenuTreeDlg::NewMenuTree(IAccount* pAccount)
{
	CAccountMenuTree* pAccountMenuTree = new CAccountMenuTree();
	pAccountMenuTree->SetAccount(pAccount);
	CFont* pFont = GetDlgItem(IDC_BT_FONT)->GetFont();		
	TiXmlElement* pTradingMenu = m_pTradeConfigObj->GetFunctionTree(pAccount->GetCellNo());
	TiXmlElement* pElem = pTradingMenu->FirstChildElement("Item");
	while(pElem != NULL)
	{	
		CMenuTree* pTree = new CMenuTree();
		pTree->Create(WS_TABSTOP | WS_CHILD | TVS_SHOWSELALWAYS
			| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_DISABLEDRAGDROP,
			CRect(0, 0, 0, 0), this, HS_IDW_MENUTRUE + m_nLastConIndex);
		pTree->SetImageList(&m_ImageList, TVSIL_NORMAL);
		pTree->BuildMenuTree(0, pElem->FirstChildElement("Item"));
		pTree->SelectItem(pTree->GetRootItem());
		pAccountMenuTree->m_MenuTreeList.Add(pTree);
		m_MenuTreeList.Add(pTree);
		CString strCaption = pElem->Attribute(_T("caption"));
		CHSButton* pButton = new CHSButton();
		pButton->Create(strCaption, WS_TABSTOP | WS_CHILD | BS_PUSHBUTTON, 
			CRect(0, 0, 0, 0), this, IDC_BUTTON_BEGIN + m_nLastConIndex);			
		//pButton->DrawFocus(FALSE);
		pButton->SetFont(pFont, TRUE);
		pAccountMenuTree->m_ButtonList.Add(pButton);
		m_ButtonList.Add(pButton);
		pElem = pElem->NextSiblingElement();
		m_nLastConIndex++;	
	}
	pAccountMenuTree->m_pCurrMenuTree = pAccountMenuTree->m_MenuTreeList.GetAt(0);
	pAccountMenuTree->m_pCurrButton = pAccountMenuTree->m_ButtonList.GetAt(0);
	pAccountMenuTree->m_pCurrButton->SetForeColor(0x0000FF);
	m_AccountMenuTreeList.Add(pAccountMenuTree);
}

void CMenuTreeDlg::OnAccountChange(IAccount* lpValue, EAccountChange nChangeType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (lpValue && (lpValue->GetAccountType() == atTrade))
	{
		switch (nChangeType)
		{
		case acChange:
			  {
				  m_CurrAccountMenuTree->m_pCurrMenuTree->ShowWindow(SW_HIDE);
				  for (int j = 0; j < m_CurrAccountMenuTree->m_ButtonList.GetCount(); j++)
				  {
					  m_CurrAccountMenuTree->m_ButtonList.GetAt(j)->ShowWindow(SW_HIDE);
				  }
				  for (int i = 0; i < m_AccountMenuTreeList.GetCount(); i++)
				  {
					 CAccountMenuTree* pAccountMenuTree = m_AccountMenuTreeList.GetAt(i);
					  if (pAccountMenuTree->GetAccount() == lpValue)
					  {
						  m_CurrAccountMenuTree = pAccountMenuTree;
						  m_CurrAccountMenuTree->m_pCurrMenuTree->ShowWindow(SW_SHOW);
						  for (int j = 0; j < m_CurrAccountMenuTree->m_ButtonList.GetCount(); j++)
						  {
							  m_CurrAccountMenuTree->m_ButtonList.GetAt(j)->ShowWindow(SW_SHOW);
						  }	
						  OpenLayOut();
						  break;
					  }
				  }
			  }
			  break;
		case acAdd:
			  {
				  NewMenuTree(lpValue);
				  OnSizeEx();
			  }
			break;
		case acDel:
			  {
				  /*for (int i = 0; i < m_AccountMenuTreeList.GetCount(); i++)
				  {
					  CAccountMenuTree* pAccountMenuTree = m_AccountMenuTreeList.GetAt(i);
					  if (pAccountMenuTree->GetAccount() == lpValue)
					  {
						  delete pAccountMenuTree;
						  m_AccountMenuTreeList.RemoveAt(i);
						  break;
					  }
				  }*/
			  }
			  break;
		}
	}
}

void CMenuTreeDlg::OnAccountStatus(IAccount* lpValue)
{

}

void CMenuTreeDlg::OnSizeEx()
{
	CRect rcTmp;
	GetClientRect(rcTmp);
	int nWith = rcTmp.Width(); 
	int nHeight = rcTmp.Height();
	for (int i = 0; i< m_AccountMenuTreeList.GetCount(); i++)
	{
		CAccountMenuTree* pAccountMenuTree = m_AccountMenuTreeList.GetAt(i);
		for (int j = 0; j < pAccountMenuTree->m_MenuTreeList.GetCount(); j++)
		{
			CMenuTree* pTmp = pAccountMenuTree->m_MenuTreeList.GetAt(j) ;
			pTmp->MoveWindow(0, 24, nWith, nHeight - 24);
			//CRect rcList(rcTmp.left, rcTmp.top + 24 , rcTmp.right, rcTmp.bottom);
			//pTmp->MoveWindow(&rcList);
		}
		int nCount = (int)pAccountMenuTree->m_ButtonList.GetCount();
		if ((nWith > 0) && (nCount > 0))
		{
			int nButtonWith = (int)( nWith / nCount);
			for (int j = 0; j < nCount; j++)
			{
				CHSButton* pTmp = pAccountMenuTree->m_ButtonList.GetAt(j);
				pTmp->MoveWindow(nButtonWith * j , 0, nButtonWith, 24);
			}
		}
	}	

}
void CMenuTreeDlg::OnHotKey(UINT nId)
{
	if (1 == nId)
	{
		IMainWnd* pMain = GetMain();		
		pMain->ShowPanel("TradeDebugView",SP_SHOW);
	}
}