#include "StdAfx.h"
#include "LinkageMsgManager.h"

#include "..\Common\HSWinner.h"
#include "..\Common\HSMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLinkageMsgManager::CLinkageMsgManager(void)
{
}

CLinkageMsgManager::~CLinkageMsgManager(void)
{
	OutputDebugString(_T("[CLinkageMsgManager::~CLinkageMsgManager] freeing...\n"));
	POSITION pos;
	WORD key;
	CObject* pObj = NULL;
	for( pos = m_msgMap.GetStartPosition(); pos != NULL; )
	{
		m_msgMap.GetNextAssoc( pos, key, pObj);
		if(pObj)
		{
			delete (CBizWindowsArray*)pObj;
		}
	}
	m_msgMap.RemoveAll();
}


BOOL CLinkageMsgManager::RegisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd)
{
	CBizWindowsArray  *pBizWndArray = NULL;
	if( !m_msgMap.Lookup(nMsg,(CObject*&)pBizWndArray) )
	{
		pBizWndArray = new CBizWindowsArray();
		m_msgMap.SetAt(nMsg,pBizWndArray);
	}	

	for(int i= 0 ; i < pBizWndArray->GetCount(); i++)
	{
		IBizWindow* pBiz = pBizWndArray->GetAt(i);
		if(pBiz == pBizWnd)
			return TRUE;
	}
	
	pBizWndArray->Add(pBizWnd);
	return TRUE;
} 

BOOL CLinkageMsgManager::UnregisterLinkageMsg(UINT nMsg,IBizWindow *pBizWnd)
{
	CBizWindowsArray*  pBizWndArray = NULL;
	if( !m_msgMap.Lookup(nMsg,(CObject*&)pBizWndArray) )
	{
		return TRUE;
	}	
	for(int i = 0; i < pBizWndArray->GetCount(); i++ )
	{
		IBizWindow* p = pBizWndArray->GetAt(i);
		if( p  == pBizWnd )
		{
			pBizWndArray->RemoveAt(i);
			return TRUE;
		}
	}

	return TRUE;
}

int CLinkageMsgManager::DispatchLinkageMsg(UINT nMsg,WPARAM wParam,LPARAM lParam,BOOL bPost)
{
	CBizWindowsArray*  pBizWndArray = NULL;
	if( !m_msgMap.Lookup(nMsg,(CObject*&)pBizWndArray) )
	{
		return -1;
	}	

	ILinkageParam* pLinkageParam = (ILinkageParam*)wParam;
	for(int i = 0 ; i < pBizWndArray->GetCount(); i++)
	{
		IBizWindow* p = pBizWndArray->GetAt(i);
		if(bPost)
		{
			ILinkageParam* pLinkageClone = pLinkageParam->Clone();
			if(pLinkageClone->IsSendToSameLayout() )
			{
				LONG lFrom = pLinkageClone->GetFrom();
				LONG lTarget = (LONG)p->GetLayoutPanel()->GetLayoutWindow();
				if( lFrom == lTarget)
					::PostMessage(p->GetHandle(),WM_BIZWINDOW_LINKAGE,(WPARAM)pLinkageClone,lParam);
			}
			else
				::PostMessage(p->GetHandle(),WM_BIZWINDOW_LINKAGE,(WPARAM)pLinkageClone,lParam);
		}
		else
		{

			ILinkageParam* pLinkageParam = (ILinkageParam*)wParam;
			ILinkageParam* pLinkageClone = pLinkageParam->Clone();
			if(pLinkageClone->IsSendToSameLayout() )
			{
				LONG lFrom = pLinkageClone->GetFrom();
				LONG lTarget = (LONG)p->GetLayoutPanel()->GetLayoutWindow();
				if( lFrom == lTarget)
					::SendMessage(p->GetHandle(),WM_BIZWINDOW_LINKAGE,(WPARAM)pLinkageClone,lParam);
			}
			else
				::SendMessage(p->GetHandle(),WM_BIZWINDOW_LINKAGE,(WPARAM)pLinkageClone,lParam);
		}
	}
	return 0;
}