// HSMenuBar.cpp : 实现文件
//

#include "stdafx.h"
#include "Winner.h"
#include "MainFrm.h"
#include "HSMenuBar.h"
#include "HSAboutDlg.h"
#include "..\..\HelperLib\GeneralHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CMenuInfo::OnMenuMsg()
{
	if(m_handle == NULL)
	{
		m_handle = CWinnerApplication::GetModuleHandle(m_strModuleName + _T(".dll"));
		if(m_handle)
		{
			FARPROC proc = NULL;
			proc = GetProcAddress(m_handle,"OnMenuMessage");
			m_pOnMenuMsg = reinterpret_cast<OnMenuMessage>(proc);
		}
	}
	if(m_pOnMenuMsg)
		m_pOnMenuMsg(m_strModuleName + _T(".") + m_strMsgId);
}

int CHSTopButton::GetImageHorzMargin() const 
{
	if(m_sizeImage.cx != 0)
		return 0;
	else
		return __super::GetImageHorzMargin();
}
int CHSTopButton::GetVertMargin() const 
{ 
	if(m_sizeImage.cx != 0)
		return 0;
	else
		return 3;
}
void CHSTopButton::OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState)
{
	CString strText;
	GetWindowText(strText);
	if( strText.IsEmpty())
		rectClient.InflateRect(2,2);
	__super::OnDrawBorder(pDC,rectClient,uiState);
}

void CHSTopButton::ChangeImg(CString strImgFile)
{
	CImage img;
	img.Load(strImgFile);
	if( !img.IsNull() )
	{
		HBITMAP hBitmap = img.Detach();
		this->SetImage(hBitmap);
	}
}

// CHSMenuBar

IMPLEMENT_DYNAMIC(CHSMenuBar, CMFCMenuBar)

CHSMenuBar::CHSMenuBar()
{
	m_nMenuIdSeed = USER_MENUID_BEGIN;
}

CHSMenuBar::~CHSMenuBar()
{
	RemoveCaptionButtons();
	for(int i = 0; i < m_arrExternalInfo.GetCount(); i++ )
	{
		TopBtnInfo *pInfo = m_arrExternalInfo.GetAt(i);
		delete pInfo;
	}

	POSITION pos;
    int nKey;
	CMenuInfo* pMenuInfo = NULL;
	for( pos = m_menuInfoMap.GetStartPosition(); pos != NULL; )
	{
		m_menuInfoMap.GetNextAssoc( pos, nKey, pMenuInfo);
		if(pMenuInfo)
		{
			delete pMenuInfo;
		}
	}
	m_menuInfoMap.RemoveAll();
}

#define MENUBAR_IDC_CLOSE     1
#define MENUBAR_IDC_MAX       2
#define MENUBAR_IDC_MIN       3

#define MENUBAR_IDC_QUOTE     4
#define MENUBAR_IDC_TRADE     5
#define MENUBAR_IDC_INFO      6

BEGIN_MESSAGE_MAP(CHSMenuBar, CMFCMenuBar)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CHSMenuBar 消息处理程序

void CHSMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();
	ASSERT(hWndMain);
	DWORD dwStyle = GetWindowLong(hWndMain, GWL_STYLE);
	UINT nId = (dwStyle & WS_MAXIMIZE)? SC_RESTORE: SC_MAXIMIZE;
	CHSTopButton* pBtn = (CHSTopButton*)GetDlgItem(MENUBAR_IDC_MAX);
	if(pBtn)
	{
		CString strTemp = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);
		CString strFile = (dwStyle & WS_MAXIMIZE)?strTemp + _T("max_main.bmp"): strTemp + _T("restore_main.bmp");
		pBtn->ChangeImg(strFile);
	}
	::PostMessage(hWndMain, WM_SYSCOMMAND, nId, 0);
}

void CHSMenuBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	int iHit = HitTest(point);
	if ( iHit < 0 )
	{
		CWnd *pWnd = AfxGetMainWnd();
		if(!pWnd->IsZoomed())
			pWnd->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	}
	else
	{
		CMFCMenuBar::OnLButtonDown(nFlags, point);
	}
}

void CHSMenuBar::RemoveCaptionButtons()
{
	for (int i = 0; i < m_arrButtons.GetSize(); i++)
	{
		delete m_arrButtons[i];
	}
	m_arrButtons.RemoveAll();
}

void CHSMenuBar::SetCaptionButtons()
{
	RemoveCaptionButtons();
	CString strImgDir = CGeneralHelper::GetSpecifyPath(CGeneralHelper::FACE_PATH);

	for(int i = 0 ; i < m_arrExternalInfo.GetCount(); i++ )
	{
		TopBtnInfo* pInfo = m_arrExternalInfo.GetAt(i);
		AddNewCaptionButton(pInfo->strCaption,strImgDir + pInfo->strImg,pInfo->m_nID);
	}

	AddNewCaptionButton(_T(""),strImgDir + _T("min_main.bmp"),MENUBAR_IDC_MIN);

	CString strFile;
	HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();
	if(hWndMain == NULL)
	{
		CWinnerApp* pApp = DYNAMIC_DOWNCAST(CWinnerApp, AfxGetApp());
		CRect rectNormal(CPoint(0, 0), CSize(0, 0));
		int nFlags = 0;
		int nShowCmd = SW_MAXIMIZE;
		if (pApp != NULL)
		{
			pApp->LoadWindowPlacement(rectNormal, nFlags, nShowCmd);
		}
		strFile = (nShowCmd == SW_MAXIMIZE)?strImgDir + _T("restore_main.bmp"): strImgDir + _T("max_main.bmp");
	}
	else
	{
		DWORD dwStyle = GetWindowLong(hWndMain, GWL_STYLE);
		strFile = (dwStyle & WS_MAXIMIZE)?strImgDir + _T("restore_main.bmp"): strImgDir + _T("max_main.bmp");
	}
	
	AddNewCaptionButton(_T(""),strFile,MENUBAR_IDC_MAX);
	AddNewCaptionButton(_T(""),strImgDir + _T("close_main.bmp"),MENUBAR_IDC_CLOSE);	
}

void CHSMenuBar::AddNewCaptionButton(CString strCaption,CString strMapFile,UINT nID)
{
	CHSTopButton *pBtn = new CHSTopButton();
	CRect rc;
	rc.SetRectEmpty();
	pBtn->Create(strCaption,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,rc,this,nID);
	CImage img;
	img.Load(strMapFile);
	if( !img.IsNull() )
	{
		HBITMAP hBitmap = img.Detach();
		pBtn->SetImage(hBitmap);
		pBtn->SetWindowText(_T(""));
	}
	else
	{
		pBtn->EnableMenuFont();
	}
	pBtn->SetAutorepeatMode();
	pBtn->SizeToContent();
	m_arrButtons.Add(pBtn);
}


int CHSMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMFCMenuBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString strMenuFile = CGeneralHelper::GetSpecifyPath(CGeneralHelper::SYSCONFIG_PATH) 
		+ _T("sysconfig.xml");
	LoadMenu(strMenuFile);	

	SetCaptionButtons();

	return 0;
}

void CHSMenuBar::OnSize(UINT nType, int cx, int cy)
{
	CMFCMenuBar::OnSize(nType, cx, cy);
	int nX = cx;
	for(int i = m_arrButtons.GetSize() - 1; i >=0 ;i--)
	{
		CHSTopButton* pBtn = m_arrButtons[i];
		CRect rc;
		pBtn->GetWindowRect(&rc);
		if(i == m_arrButtons.GetSize() - 4)
			nX = nX - rc.Width() - 16;
		else
			nX = nX - rc.Width() - 3;
		pBtn->MoveWindow(nX,0,rc.Width(),rc.Height());
	}
}

BOOL CHSMenuBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = wParam;
	if(nID == MENUBAR_IDC_CLOSE || nID == MENUBAR_IDC_MAX || nID == MENUBAR_IDC_MIN
		|| nID == MENUBAR_IDC_QUOTE || nID == MENUBAR_IDC_TRADE || nID == MENUBAR_IDC_INFO)	
	{
		OnTopBtnClick(nID);
		return TRUE;
	}

	return CMFCMenuBar::OnCommand(wParam, lParam);
}

void CHSMenuBar::OnTopBtnClick(UINT nID)
{
	HWND hWndMain = AfxGetMainWnd()->GetSafeHwnd();
	if(nID == MENUBAR_IDC_CLOSE)
		::PostMessage(hWndMain, WM_SYSCOMMAND, SC_CLOSE, 0);
	else if (nID == MENUBAR_IDC_MAX )
	{
		CPoint pt(0,0);
		OnLButtonDblClk(NULL,pt);
	}
	else if (nID == MENUBAR_IDC_MIN )
	{
		::PostMessage(hWndMain, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}
	else if( nID == MENUBAR_IDC_QUOTE )
	{		
		CWinnerApp* pApp = (CWinnerApp*)AfxGetApp();
		CMainFrame* pMain = (CMainFrame*)pApp->m_pMainWnd;
		pMain->OpenPanel(PN_QUOTEDIAGRAM);
	}
	else if( nID == MENUBAR_IDC_TRADE)
	{
		CWinnerApp* pApp = (CWinnerApp*)AfxGetApp();
		CMainFrame* pMain = (CMainFrame*)pApp->m_pMainWnd;
		if(pMain->IsTradeLogin())
		{
			if( pMain->IsPanelShow(PN_TRADE) )
				pMain->HidePanel(PN_TRADE);
			else
				pMain->OpenPanel(PN_TRADE);
		}
		else
		{
			LoginRet lb = pApp->DoLogin(LoginType_OnlyTrade);
			if(lb == LoginRet_Trade || lb == LoginRet_QuoteAndTrade)
			{
				pMain->OpenPanel(PN_TRADE);
			}
		}
	}
	else if( nID == MENUBAR_IDC_INFO)
	{

	}
	::SetFocus(hWndMain);
}

void CHSMenuBar::LoadMenu(CString strMenuFile)
{
	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(strMenuFile))
		return;

	TiXmlElement* pRoot = xmldoc.FirstChildElement("SysConfig");
	TiXmlElement* pMenuElment = pRoot->FirstChild("MainMenu")->ToElement();

	CMenu menu;
	menu.CreateMenu();
	BuildMenu(&menu,pMenuElment->FirstChild("Item")->ToElement());
	CreateFromMenu(menu.m_hMenu,TRUE,TRUE);
	menu.Detach();

	TiXmlElement* pMenuBtn = pRoot->FirstChild("MenuButtons")->ToElement();
	if(pMenuBtn)
	{
		TiXmlElement* pMenuBtnItem = pMenuBtn->FirstChildElement(_T("Item"));
		while(pMenuBtnItem)
		{
			TopBtnInfo* pBtnInfo = new TopBtnInfo();
			m_arrExternalInfo.Add(pBtnInfo);
			CString strID = pMenuBtnItem->Attribute(_T("id"));
			pBtnInfo->m_nID = atoi(strID);
			pBtnInfo->strCaption = pMenuBtnItem->Attribute(_T("caption"));
			pBtnInfo->strImg = pMenuBtnItem->Attribute(_T("img"));
			pMenuBtnItem = pMenuBtnItem->NextSiblingElement(); 
		}	
	}
}

BOOL CHSMenuBar::BuildMenu(CMenu* pMenu,TiXmlElement* pMenuElment)
{
	TiXmlElement* pElem = pMenuElment;
	while(pElem != NULL)
	{
		CMenuInfo *pMenuInfo = new CMenuInfo();
		LoadMenuItemInfo(pElem,pMenuInfo);
		m_menuInfoMap.SetAt(pMenuInfo->m_nId,pMenuInfo);

		if(pMenuInfo->m_bVisable == FALSE)
		{
			pElem=pElem->NextSiblingElement();
			continue;
		}

		if(pElem->NoChildren())
		{
			if(pMenuInfo->m_nId == 0)
				pMenu->AppendMenu(MF_SEPARATOR, pMenuInfo->m_nId, pMenuInfo->m_strCaption);
			else
				pMenu->AppendMenu(MF_BYCOMMAND|MF_STRING, pMenuInfo->m_nId, pMenuInfo->m_strCaption);
			
		}
		else
		{
			TiXmlElement* pChildElem = pElem->FirstChild("Item")->ToElement();
			CMenu subMenu;
			subMenu.CreateMenu();
			pMenu->AppendMenu( MF_POPUP, (UINT_PTR)subMenu.m_hMenu, pMenuInfo->m_strCaption);
			BuildMenu(&subMenu,pChildElem);
			subMenu.Detach();
		}
		pElem=pElem->NextSiblingElement();
	}
	return TRUE;
}

int  CHSMenuBar::CreateMenuId()
{
	int nId = m_nMenuIdSeed;
	m_nMenuIdSeed++;
	return nId;
}

BOOL CHSMenuBar::LoadMenuItemInfo(TiXmlElement *pElem,CMenuInfo* pInfo)
{
	if(pElem == NULL || pInfo == NULL)
		return FALSE;

	//菜单id
	pInfo->m_nId = CreateMenuId();
	//菜单标题
	pInfo->m_strCaption = pElem->Attribute(_T("caption")); 
	//菜单是否可见
	CString strVisable = pElem->Attribute(_T("visable"));
	if(strVisable == _T("false"))
	    pInfo->m_bVisable = FALSE;
	else
		pInfo->m_bVisable = TRUE;

	CString strAction = pElem->Attribute(_T("action")); 
	CString strPlugin,strMsgId;
	int nPos = strAction.Find('.');
	//菜单所在模块名称
	pInfo->m_strModuleName = strAction.Left(nPos);
	//菜单msg id
	pInfo->m_strMsgId = strAction.Right(strAction.GetLength() - nPos - 1);
	return TRUE;
}

void CHSMenuBar::OnMenuMsg(int nId)
{
	CMenuInfo* pInfo = NULL;
	if ( nId == 115 )
	{
		CHSAboutDlg dlg;
		dlg.DoModal();
	}
	if(m_menuInfoMap.Lookup(nId,pInfo))
	{
		CWinnerApplication::FormatLog(DEBUG_LOG_LEVEL,LN_WINNER,
			_T("[CHSMenuBar::OnMenuMsg]menu msg: %d-%s:action=%s.%s"),
			pInfo->m_nId,pInfo->m_strMsgId,pInfo->m_strModuleName,pInfo->m_strMsgId);
		pInfo->OnMenuMsg();
	}
	else
	{
		CWinnerApplication::FormatLog(ERROR_LOG_LEVEL,LN_WINNER,
			_T("[CHSMenuBar::OnMenuMsg]menu info not found.id=%d"),
			nId);	
	}
}

CString CHSMenuBar::GetActionID(int nID)
{
	CMenuInfo* pInfo = NULL;
	CString str;
	if(m_menuInfoMap.Lookup(nID,pInfo))
	{
		str.Format(_T("%s.%s"), pInfo->m_strModuleName,pInfo->m_strMsgId);
	}
	return str;
}
