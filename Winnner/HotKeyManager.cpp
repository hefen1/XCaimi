#include "StdAfx.h"
#include "..\Common\HSMessage.h"
#include "HotKeyManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHotKeyManager::CHotKeyManager(void)
{
}

CHotKeyManager::~CHotKeyManager(void)
{
	OutputDebugString(_T("[CHotKeyManager::~CHotKeyManager] freeing...\n"));
	POSITION pos;
	CString key;
	CHotKeyInfoArray* pArray = NULL;
	for( pos = m_hotKeyMap.GetStartPosition(); pos != NULL; )
	{
		m_hotKeyMap.GetNextAssoc( pos, key, (CObject*&)pArray);
		if(pArray)
		{
			for(int i = 0; i < pArray->GetCount(); i++)
			{
				HotKeyInfo *pInfo = pArray->GetAt(i);
				if(pInfo)
					delete pInfo;
			}
			delete (CBizWindowsArray*)pArray;
		}
	}
	m_hotKeyMap.RemoveAll();


	HotKeyInfo* pNotifyInfo = NULL;
	for( pos = m_noOperatNotifyMap.GetStartPosition(); pos != NULL; )
	{
		m_noOperatNotifyMap.GetNextAssoc( pos, key, (CObject*&)pNotifyInfo);
		if(pNotifyInfo)
		{
			delete pNotifyInfo;
		}
	}
	m_noOperatNotifyMap.RemoveAll();
}

HWND CHotKeyManager::GetFrameHandle(HWND hChild)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	POSITION pos = pMainWnd->m_inlineWndMap.GetStartPosition();
	CString strKey;
	CWnd* pFrameWnd = NULL;
	while( pos != NULL)
	{
		pMainWnd->m_inlineWndMap.GetNextAssoc(pos,strKey,(CObject*&)pFrameWnd);
		HWND hFrame = hChild;
		while(hFrame)
		{
			if(pFrameWnd->m_hWnd == hFrame)
			{
				return pFrameWnd->m_hWnd;
			}
			hFrame = ::GetParent(hFrame);
		}
	}

	pos = pMainWnd->m_dockWndMap.GetStartPosition();
	pFrameWnd = NULL;
	while( pos != NULL)
	{
		pMainWnd->m_dockWndMap.GetNextAssoc(pos,strKey,(CObject*&)pFrameWnd);
		HWND hFrame = hChild;
		while(hFrame && pFrameWnd)
		{
			if(pFrameWnd->m_hWnd == hFrame)
			{
				return pFrameWnd->m_hWnd;
			}
			hFrame = ::GetParent(hFrame);
		}
	}

	pos = pMainWnd->m_popWndMap.GetStartPosition();
	pFrameWnd = NULL;
	while( pos != NULL)
	{
		pMainWnd->m_popWndMap.GetNextAssoc(pos,strKey,(CObject*&)pFrameWnd);
		HWND hFrame = hChild;
		while(hFrame && pFrameWnd)
		{
			if(pFrameWnd->m_hWnd == hFrame)
			{
				return pFrameWnd->m_hWnd;
			}
			hFrame = ::GetParent(hFrame);
		}
	}
	return NULL;
}

BOOL CHotKeyManager::RegisterHotKey(UINT id,UINT fsModifiers,UINT vk,IBizWindow *pBizWnd)
{
	if(id == HK_NOOPERATNOTIFY)
	{
		CString key;
		key.Format(_T("biz_wnd:%x"),pBizWnd);
		HotKeyInfo* pInfo = NULL; 
		if(!m_noOperatNotifyMap.Lookup(key,(CObject*&)pInfo))
		{
			HotKeyInfo *pNewInfo = new HotKeyInfo;
			pNewInfo->m_nId = id;
			pNewInfo->m_fsModifiers = NULL;
			pNewInfo->m_vk = vk;
			pNewInfo->m_pBizWnd = pBizWnd;
			pNewInfo->m_hFrame = GetFrameHandle(pBizWnd->GetHandle());
			m_noOperatNotifyMap.SetAt(key,(CObject*)pNewInfo);
		}
		else
		{
			pInfo->m_vk = vk;
		}
	}
	else
	{
		CString key;
		key.Format(_T("%x_%x"),fsModifiers,vk);

		CHotKeyInfoArray  *pHotKeyInfoArray = NULL;
		if( !m_hotKeyMap.Lookup(key,(CObject*&)pHotKeyInfoArray) )
		{
			pHotKeyInfoArray = new CHotKeyInfoArray();
			m_hotKeyMap.SetAt(key,pHotKeyInfoArray);
		}	

		for(int i= 0 ; i < pHotKeyInfoArray->GetCount(); i++)
		{
			HotKeyInfo* pInfo = pHotKeyInfoArray->GetAt(i);
			if(pInfo->m_pBizWnd == pBizWnd)
				return TRUE;
		}

		HotKeyInfo *pNewInfo = new HotKeyInfo;
		pNewInfo->m_nId = id;
		pNewInfo->m_fsModifiers = fsModifiers;
		pNewInfo->m_vk = vk;
		pNewInfo->m_pBizWnd = pBizWnd;
		pNewInfo->m_hFrame = GetFrameHandle(pBizWnd->GetHandle());
		pHotKeyInfoArray->Add(pNewInfo);
	}
	return TRUE;
}

BOOL CHotKeyManager::UnregisterHotKey(UINT fsModifiers,UINT vk,IBizWindow *pBizWnd)
{
	CString keyNotify;
	keyNotify.Format(_T("biz_wnd:%x"),pBizWnd);
	HotKeyInfo* pInfo = NULL; 
	if(m_noOperatNotifyMap.Lookup(keyNotify,(CObject*&)pInfo))
	{
		if(pInfo->m_nId == HK_NOOPERATNOTIFY) 
		{
			delete pInfo;
			m_noOperatNotifyMap.RemoveKey(keyNotify);
			return TRUE;
		}
	}
	
	CString key;
	key.Format(_T("%x_%x"),fsModifiers,vk);

	CHotKeyInfoArray  *pHotKeyInfoArray = NULL;
	if( !m_hotKeyMap.Lookup(key,(CObject*&)pHotKeyInfoArray) )
	{
		return TRUE;
	}	

	for(int i = 0; i < pHotKeyInfoArray->GetCount(); i++ )
	{
		HotKeyInfo* p = pHotKeyInfoArray->GetAt(i);
		if( p->m_pBizWnd  == pBizWnd )
		{
			HotKeyInfo* pInfo = pHotKeyInfoArray->GetAt(i);
			pHotKeyInfoArray->RemoveAt(i);
			if(pInfo)
				delete pInfo;
			return TRUE;
		}
	}

	return TRUE;
}


BOOL CHotKeyManager::OnHotKey(UINT fsModifiers,UINT vk)
{
	BOOL bHasProcessed = FALSE;
	CString key;
	key.Format(_T("%x_%x"),fsModifiers,vk);	

	CHotKeyInfoArray  *pHotKeyInfoArray = NULL;
	if( !m_hotKeyMap.Lookup(key,(CObject*&)pHotKeyInfoArray) )
	{
		return FALSE;
	}

	OutputDebugString("[CHotKeyManager::OnHotKey] " + key + " is press \n");

	HWND hFocus = ::GetFocus();
	HWND hFocusFrame = GetFrameHandle(hFocus);

	for(int i = 0; i < pHotKeyInfoArray->GetCount(); i++ )
	{
		bHasProcessed = TRUE;
		HotKeyInfo* p = pHotKeyInfoArray->GetAt(i);
		ASSERT(p != NULL);
		ASSERT(p->m_pBizWnd != NULL);
		if(p->m_hFrame == hFocusFrame)
			::SendMessage(p->m_pBizWnd->GetHandle(),WM_BIZWINDOW_NOTIFY,BWN_HOT_KEY,p->m_nId);
	}

	return bHasProcessed;
}

BOOL CHotKeyManager::OnNoOperatTimeout(UINT nTimeout)
{	
	HotKeyInfo* pNotifyInfo = NULL;
	POSITION pos;
	CString key;
	for( pos = m_noOperatNotifyMap.GetStartPosition(); pos != NULL; )
	{
		m_noOperatNotifyMap.GetNextAssoc( pos, key, (CObject*&)pNotifyInfo);
		if(pNotifyInfo && nTimeout > pNotifyInfo->m_vk)
		{
			::SendMessage(pNotifyInfo->m_pBizWnd->GetHandle(),WM_BIZWINDOW_NOTIFY,BWN_ON_OPERAT_TIMEOUT,pNotifyInfo->m_nId);
		}
	}
	return TRUE;
}

BOOL CHotKeyManager::NeedNoOperatTimeoutNotify()
{
	return m_noOperatNotifyMap.GetCount() > 0;
}