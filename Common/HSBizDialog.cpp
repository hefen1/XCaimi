
#include "stdafx.h"
#include ".\HSBizDialog.h"
#include ".\HSWinner.h"
#include ".\WinnerApplication.h"
#include ".\KeyBoardStruct.h"
#include "HSSkinIntf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HHOOK CHSBizDialog::g_hkkMsgBox = NULL;
LRESULT   CALLBACK   CBTProc(INT   nCode,   WPARAM   wParam,   LPARAM   lParam) 
{ 
	HWND     hParentWnd,   hChildWnd;         //   msgbox   is   "child " 
	RECT     rParent,   rChild,   rDesktop; 
	POINT   pCenter,   pStart; 
	INT       nWidth,   nHeight; 

	//   notification   that   a   window   is   about   to   be   activated 
	//   window   handle   is   wParam 
	//if   (nCode   ==   HCBT_ACTIVATE) 
	{ 
		//   set   window   handles 
		hParentWnd   =   GetForegroundWindow(); 
		hChildWnd     =   (HWND)wParam; 

		if((hParentWnd   !=   0)   && 
			(hChildWnd   !=   0)   && 
			(GetWindowRect(GetDesktopWindow(),   &rDesktop)   !=   0)   && 
			(GetWindowRect(hParentWnd,   &rParent)   !=   0)   && 
			(GetWindowRect(hChildWnd,   &rChild)   !=   0)) 
		{ 
			//   calculate   message   box   dimensions 
			nWidth     =   (rChild.right   -   rChild.left); 
			nHeight   =   (rChild.bottom   -   rChild.top); 

			//   calculate   parent   window   center   point 
			pCenter.x   =   rParent.left+((rParent.right 
				-   rParent.left)/2); 
			pCenter.y   =   rParent.top+((rParent.bottom 
				-   rParent.top)/2); 

			//   calculate   message   box   starting   point 
			pStart.x   =   (pCenter.x   -   (nWidth/2)); 
			pStart.y   =   (pCenter.y   -   (nHeight/2)); 

			//   adjust   if   message   box   is   off   desktop 
			if(pStart.x   <   0)   pStart.x   =   0; 
			if(pStart.y   <   0)   pStart.y   =   0; 
			if(pStart.x   +   nWidth   >   rDesktop.right) 
				pStart.x   =   rDesktop.right   -   nWidth; 
			if(pStart.y   +   nHeight   >   rDesktop.bottom) 
				pStart.y   =   rDesktop.bottom   -   nHeight; 

			//   move   message   box 
			MoveWindow(hChildWnd, 
				pStart.x,   pStart.y, 
				nWidth,   nHeight, 
				FALSE); 
		} 
		//   exit   CBT   hook 
		UnhookWindowsHookEx(CHSBizDialog::g_hkkMsgBox); 
	} 
	//   otherwise,   continue   with   any   possible   chained   hooks 
	//else   
		CallNextHookEx(CHSBizDialog::g_hkkMsgBox,   nCode,   wParam,   lParam); 
	return   0; 
}

CLinkageParam::CLinkageParam(BOOL bSendToSameLayout/* = FALSE*/)
{
	m_bSendToSameLayout = bSendToSameLayout;
	m_bPost = FALSE;
}


ILinkageParam* CLinkageParam::Clone()
{
	CLinkageParam *pLinkage = new CLinkageParam();

	pLinkage->m_nMsg = m_nMsg;
	pLinkage->m_bPost = m_bPost;
	pLinkage->m_lFrom = m_lFrom;
	pLinkage->m_bSendToSameLayout = m_bSendToSameLayout;
	POSITION pos;
	CString key,value;
	for( pos = m_valueMap.GetStartPosition(); pos != NULL; )
	{
		m_valueMap.GetNextAssoc( pos, key, value);
		pLinkage->AddParam(key,value);
	}

	return pLinkage;
}

void CLinkageParam::AddParam(CString strFieldName,CString strValue)
{
	m_valueMap.SetAt(strFieldName,strValue);
}

CString CLinkageParam::GetStringField(CString strFieldName)
{
	CString strValue;
	if ( m_valueMap.Lookup(strFieldName,strValue) )
		return strValue;
	else
		return _T("");
}

int CLinkageParam::GetIntegerField(CString strFieldName)
{
	CString strValue;
	if ( m_valueMap.Lookup(strFieldName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atoi(strValue);
	}
	return 0;
}

double CLinkageParam::GetDoubleField(CString strFieldName)
{
	CString strValue;
	if ( m_valueMap.Lookup(strFieldName,strValue) )
	{
		if(!strValue.IsEmpty())
			return atof(strValue);
	}
	return 0;
}

POSITION CLinkageParam::GetStartPosition()
{
	return m_valueMap.GetStartPosition();
}

void CLinkageParam::GetNextAssoc(POSITION& rNextPosition,CString& strParamName,CString& strValue)
{
	return m_valueMap.GetNextAssoc(rNextPosition,strParamName,strValue);
}

void CLinkageParam::Release()
{
	delete this;
}
// CHSBizDialog 对话框

IMPLEMENT_DYNAMIC(CHSBizDialog, CDialogEx)

CHSBizDialog::CHSBizDialog(UINT nID,CWnd* pParent /*=NULL*/,BOOL bFreeSelf/* = TRUE*/)
: CDialogEx(nID, pParent)
{
	m_bFreeSelf = bFreeSelf;
	m_pLayoutPanel = NULL;
	m_bAutoKeyboard = FALSE;
	m_pKeyboard = NULL;
}

CHSBizDialog::~CHSBizDialog()
{
}

INT CHSBizDialog::HSMessageBox(HWND hwnd,   CString strText,   CString strCaption, UINT uType)
{
	g_hkkMsgBox = SetWindowsHookEx(WH_CBT,   &CBTProc,   0, GetCurrentThreadId()); 
	return  ::MessageBox(hwnd,strText,strCaption,uType); 
}

HWND CHSBizDialog::GetHandle()
{
	return m_hWnd;
}

BOOL CHSBizDialog::Init(IMainWnd* pMain,ILayoutPanel* pLayoutPanel,IHSParam* pParam)
{
	m_pMain = pMain;
	m_pLayoutPanel = pLayoutPanel;
	m_pParam = pParam;
	return OnInit();
}

ILayoutPanel* CHSBizDialog::GetLayoutPanel()
{
	return m_pLayoutPanel;
}

ILayoutWindow* CHSBizDialog::GetLayoutWindow()
{
	if(m_pLayoutPanel!=NULL)
		return m_pLayoutPanel->GetLayoutWindow();
	else
		return FALSE;
}


void CHSBizDialog::PostNcDestroy()
{
	if(m_bFreeSelf)
		delete this;
}

void CHSBizDialog::OnOK()
{
	CWnd* pFocus = GetFocus();
	char szClassName[128];
	::GetClassName(pFocus->m_hWnd, szClassName, 128);
	CString strClassName = _T("Button"); 
	if( strClassName == szClassName )
	{
		UINT nId = pFocus->GetDlgCtrlID (); 
		SendMessage (WM_COMMAND,MAKEWPARAM(nId,BN_CLICKED), (LPARAM)pFocus->m_hWnd); 
	}
	else
	{	
		CWnd *pWnd = GetNextDlgTabItem(pFocus,FALSE);
		if(pWnd )
			pWnd->SetFocus();			
	}
}

void CHSBizDialog::OnCancel()
{
}

BOOL CHSBizDialog::OnInit()
{
	/*IHSSkinObj* pHSSkinObj = (IHSSkinObj*) CWinnerApplication::GetObject(OBJ_HSSKIN);
	if (pHSSkinObj)
	{
		pHSSkinObj->LoadSkin("");
		pHSSkinObj->SetActive(TRUE);
		pHSSkinObj->SetSkinFormType(2);
		pHSSkinObj->SkinForm(this->GetSafeHwnd());
	}*/
	return TRUE;
}

void CHSBizDialog::OnShow(BOOL bShow)
{

}

void CHSBizDialog::OnHotKey(UINT nId)
{

}

void CHSBizDialog::OnLinkageMsg(CLinkageParam* pParam)
{

}

void CHSBizDialog::OnWindowFocused()
{
	SetFocus();
}

void CHSBizDialog::OnLayoutStateChange()
{

}

void CHSBizDialog::OnNoOperatTimeout()
{

}

void CHSBizDialog::OnKeyBoardCodeNotify(StockUserInfo* pStkInfo)
{
	
}

BOOL CHSBizDialog::OnKeyBoardNotify(WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BEGIN_MESSAGE_MAP(CHSBizDialog, CDialogEx)
	ON_MESSAGE(WM_BIZWINDOW_NOTIFY, OnBizWindowNotify)
	ON_MESSAGE(WM_BIZWINDOW_LINKAGE, OnLinkageNotify)
	ON_MESSAGE(WM_KEYBOARD_NOTIFY, OnKeyboardNotify)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CHSBizDialog 消息处理程序

LRESULT CHSBizDialog::OnBizWindowNotify(WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch(wParam)
	{
	case BWN_WINDOW_SHOW:
		OnShow( lParam == TRUE);
		break;
	case BWN_HOT_KEY:
		OnHotKey((UINT)lParam);
		break;
	case BWN_FOCUS_WND:
		OnWindowFocused();
		break;
	case BWN_LAYOUT_STATE_CHANGE:
		OnLayoutStateChange();
		break;
	case BWN_ON_OPERAT_TIMEOUT:
		OnNoOperatTimeout();
	case BWN_PARAM_CHANGED:
		OnParamChanged((IHSParam*)lParam);
	}
	return 0;
}

void CHSBizDialog::OnParamChanged(IHSParam* pParam)
{

}

LRESULT CHSBizDialog::OnLinkageNotify(WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLinkageParam* pParam = (CLinkageParam*)wParam; 
	if(pParam)
	{
		OnLinkageMsg(pParam);
		if(pParam->m_bPost == TRUE)
		{
			pParam->Release();
		}
	}
	return 0;
}

LRESULT CHSBizDialog::OnKeyboardNotify(WPARAM wParam, LPARAM lParam)
{
	//跳过已作常用处理的
	if(OnKeyBoardNotify(wParam,lParam))
		return 0;

	if( wParam == HSCurKeyInfo::KeyStock )
	{
		OnKeyBoardCodeNotify((StockUserInfo*)lParam);
	}
	return 0;
}
int CHSBizDialog::SendLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam)
{

	ILayoutWindow* pLayoutWnd = this->GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);
	pLinkageParam->m_nMsg = nMsg;
	pLinkageParam->m_bPost = FALSE;
	pLinkageParam->m_lFrom = (LONG)pLayoutWnd;

	pLayoutWnd->DispatchLinkageMsg(nMsg,(WPARAM)pLinkageParam,0,FALSE);
	return 0;
}

int CHSBizDialog::PostLinkageMsg(UINT nMsg,CLinkageParam* pLinkageParam)
{
	ILayoutWindow* pLayoutWnd = this->GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);

	pLinkageParam->m_nMsg = nMsg;
	pLinkageParam->m_bPost = TRUE;	
	pLinkageParam->m_lFrom = (LONG)pLayoutWnd;

	pLayoutWnd->DispatchLinkageMsg(nMsg,(WPARAM)pLinkageParam,0,TRUE);
	return 0;
}

BOOL CHSBizDialog::RegisterLinkageMsg(UINT nMsg)
{
	ILayoutWindow* pLayoutWnd = GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);
	return pLayoutWnd->RegisterLinkageMsg(nMsg,this);
}

BOOL CHSBizDialog::UnregisterLinkageMsg(UINT nMsg)
{
	ILayoutWindow* pLayoutWnd = GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);
	return pLayoutWnd->UnregisterLinkageMsg(nMsg,this);
}

BOOL CHSBizDialog::RegisterHotKey(UINT id,UINT fsModifiers,UINT vk)
{
	ILayoutWindow* pLayoutWnd = GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);
	return pLayoutWnd->RegisterHotKey(id,fsModifiers,vk,this);
}

BOOL CHSBizDialog::UnregisterHotKey(UINT id,UINT fsModifiers,UINT vk)
{
	ILayoutWindow* pLayoutWnd = GetLayoutWindow();
	ASSERT(pLayoutWnd != NULL);
	return pLayoutWnd->UnregisterHotKey(fsModifiers,vk,this);
}

BOOL CHSBizDialog::IsShow()
{
	return IsWindowVisible();
}

BOOL CHSBizDialog::ActiveSelf()
{
	ASSERT(m_pLayoutPanel!=NULL);
	return m_pLayoutPanel->ActiveBizWindow(this);
}

LRESULT CHSBizDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message==WM_KEYDOWN)   
	{   TCHAR code = (TCHAR)wParam;
		if( code == VK_UP)
		{
			CWnd* pFocus = GetFocus();
			CWnd *pWnd = GetNextDlgTabItem(pFocus,TRUE);
			if(pWnd != pFocus)
			{
				pWnd->SetFocus();
			}
		}
		else if( code == VK_DOWN )
		{
			CWnd* pFocus = GetFocus();
			CWnd *pWnd = GetNextDlgTabItem(pFocus,FALSE);
			if(pWnd != pFocus)
			{
				pWnd->SetFocus();
			}
		}
		
		if( code == VK_RIGHT || code == VK_LEFT || code == VK_UP || code == VK_DOWN  ||  code == VK_TAB 
			|| code == VK_RETURN || code == VK_HOME || code == VK_END)
		{
			CWnd* pFocus = GetFocus();
			if(pFocus)
				::SendMessage(pFocus->m_hWnd,message,wParam,lParam);
			return 0;
		}

		if(code>0)
		{
			if(m_bAutoKeyboard && iswgraph(code) && IsValidateKeyboardCode(code))
			{
				OnOpenKeyboard(wParam);
			}
			else 
				return 0;
		}	
	}
	return __super::WindowProc(message, wParam, lParam);
}

BOOL CHSBizDialog::OnOpenKeyboard(int nCode)
{
	OpenKeyBoard(nCode);
	return TRUE;
}

BOOL CHSBizDialog::IsValidateKeyboardCode(int nCode)
{
	//control是否按下
	if( (::GetKeyState(VK_CONTROL) & 0x8000)  != 0 )
		return FALSE;
	//shift是否按下
	if( (::GetKeyState(VK_SHIFT) & 0x8000)  != 0 )
		return FALSE;
	//alt是否按下
	if( (::GetKeyState(VK_MENU) & 0x8000)  != 0 )
		return FALSE;

	if( nCode == VK_RETURN || nCode == VK_BACK   || nCode == VK_ESCAPE || nCode == VK_TAB   ||
		nCode == VK_SHIFT  || nCode == '*'	     ||  nCode == '/'	   || nCode == '+'	    || 
		nCode == '-'	   || nCode == '.'	     || nCode == '='	   || nCode == VK_SPACE ||
		nCode == VK_PRIOR  || nCode == VK_NEXT   || ( nCode >= VK_F1 && nCode <= VK_F24)    || 
		nCode == VK_DOWN   || nCode == VK_UP     || nCode == VK_RIGHT  || nCode == VK_LEFT)
	{
		return FALSE;
	}
	return TRUE;
}
void CHSBizDialog::OpenKeyBoard(int nCode,HSShowKeyboardInfo* pKeyInfo/*=NULL*/)
{
	if(m_pKeyboard == NULL)
		m_pKeyboard = (IKeyboard*)CWinnerApplication::GetObject(OBJ_KEYBOARD);
	if(m_pKeyboard == NULL)
		return;

	if(pKeyInfo == NULL)
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		HSShowKeyboardInfo keyboardinfo;
		keyboardinfo.m_cStyle = HSShowKeyboardInfo::bottom | HSShowKeyboardInfo::right;
		keyboardinfo.m_oRect = rc;
		m_pKeyboard->ShowKeyboard(nCode,&keyboardinfo,this->m_hWnd);
	}
	else
	{
		m_pKeyboard->ShowKeyboard(nCode,pKeyInfo,this->m_hWnd);
	}
	
}

void CHSBizDialog::OpenKeyBoard(CString strCode,HSShowKeyboardInfo* pKeyInfo/*=NULL*/)
{
	if(m_pKeyboard == NULL)
		m_pKeyboard = (IKeyboard*)CWinnerApplication::GetObject(OBJ_KEYBOARD);
	if(m_pKeyboard == NULL)
		return;

	if(pKeyInfo == NULL)
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		HSShowKeyboardInfo keyboardinfo;
		keyboardinfo.m_cStyle = HSShowKeyboardInfo::bottom | HSShowKeyboardInfo::right;
		keyboardinfo.m_oRect = rc;
		strncpy(keyboardinfo.m_cResever,strCode,
			min(sizeof(keyboardinfo.m_cResever),strCode.GetLength()) );
		m_pKeyboard->ShowKeyboard(0,&keyboardinfo,this->m_hWnd);
	}
	else
	{
		strncpy(pKeyInfo->m_cResever,strCode,
			min(sizeof(pKeyInfo->m_cResever),strCode.GetLength()) );
		m_pKeyboard->ShowKeyboard(0,pKeyInfo,this->m_hWnd);
	}
}

