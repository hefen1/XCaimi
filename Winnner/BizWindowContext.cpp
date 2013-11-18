#include "StdAfx.h"
#include "BizWindowContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const CString CBizWindowContext::m_strClassName = _T("CBizWindowContext");

CBizWindowContext::CBizWindowContext(void)
{
	m_arrFixedCaption[0] = _T("");
	m_arrFixedCaption[1] = _T("垂直分割");
	m_arrFixedCaption[2] = _T("水平分割");
	m_arrFixedCaption[3] = _T("");
	m_arrFixedCaption[4] = _T("保存布局");
}

CBizWindowContext::~CBizWindowContext(void)
{
	POSITION pos;
	WORD key;
	CBizWindowInfo* pTemp;
	for( pos = m_bizWndMap.GetStartPosition(); pos != NULL; )
	{
		m_bizWndMap.GetNextAssoc( pos, key, (CObject*&)pTemp );
		if(pTemp)
			delete pTemp;
	}
	m_bizWndMap.RemoveAll();
}

void CBizWindowContext::BuildDesignMenu(CString strFileName)
{
	if(m_designMenu.m_hMenu)
		return;

	TiXmlDocument mxlDoc;
	if( mxlDoc.LoadFile(strFileName) != true )
		return;

	m_designMenu.CreatePopupMenu();

	TiXmlElement* pRoot = mxlDoc.FirstChildElement(_T("BizWindows"));
	CString strBaseID = pRoot->Attribute(_T("base_id"));
	m_nBaseId = atoi(strBaseID);
	if(m_nBaseId == 0)
		m_nBaseId = 100;
	
	CMenu subMenu;
	subMenu.CreateMenu();
	m_designMenu.AppendMenu( MF_POPUP, (UINT_PTR)subMenu.m_hMenu,_T("插入窗口"));
	TiXmlElement* pMenuElment = pRoot->FirstChild("Item")->ToElement();
	BuildMenu(&subMenu,pMenuElment);
	subMenu.Detach();

	m_designMenu.AppendMenu(MF_SEPARATOR, GetSpreateID1(),m_arrFixedCaption[0]);
	m_designMenu.AppendMenu(MF_BYCOMMAND|MF_STRING, GetYSplitActionID(),m_arrFixedCaption[1]);
	m_designMenu.AppendMenu(MF_BYCOMMAND|MF_STRING, GetXSplitActionID(),m_arrFixedCaption[2]);
	m_designMenu.AppendMenu(MF_SEPARATOR, GetSpreateID2(),m_arrFixedCaption[3]);
	m_designMenu.AppendMenu(MF_BYCOMMAND|MF_STRING, GetSaveLayoutActionID(),m_arrFixedCaption[4]);
}

void CBizWindowContext::BuildMenu(CMenu *pMenu,TiXmlElement* pMenuElment)
{
	TiXmlElement* pElem = pMenuElment;
	while(pElem != NULL)
	{
		CString strCaption = pElem->Attribute(_T("caption"));
		CString strLocation = pElem->Attribute(_T("location"));
		CBizWindowInfo *pWndInfo = new CBizWindowInfo(strCaption,strLocation);
		int nID = GetMenuID();
		m_bizWndMap.SetAt(nID,pWndInfo);
	
		if(pElem->NoChildren())
		{
			pMenu->AppendMenu(MF_BYCOMMAND|MF_STRING, nID, pWndInfo->GetCaption());
		}
		else
		{
			TiXmlElement* pChildElem = pElem->FirstChild("Item")->ToElement();
			CMenu subMenu;
			subMenu.CreateMenu();
			pMenu->AppendMenu( MF_POPUP, (UINT_PTR)subMenu.m_hMenu,pWndInfo->GetCaption());
			BuildMenu(&subMenu,pChildElem);
			subMenu.Detach();
		}
		pElem=pElem->NextSiblingElement();
	}
}

int CBizWindowContext::GetMenuID()
{
	return m_nBaseId + m_bizWndMap.GetCount() + 1;
}

CBizWindowInfo* CBizWindowContext::GetBizWndInfo(int nMenuId)
{
	CBizWindowInfo *pInfo = NULL;
	m_bizWndMap.Lookup(nMenuId,(CObject*&)pInfo);
	return pInfo;
}

void CBizWindowContext::EnableSplitAction(BOOL bEnable)
{
	MENUITEMINFO info;
	memset(&info,0,sizeof(info));
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_STATE;             // get the state of the menu item
	int nPosition = 0;
	for(int i = 0; i <= 2; i++)
	{
		int nId = m_nBaseId - i;
		if(bEnable)
		{
			if( !m_designMenu.GetMenuItemInfo(nId, &info) ) 
			{
				UINT nFlags = m_arrFixedCaption[i].IsEmpty() ? MF_SEPARATOR: MF_BYCOMMAND|MF_STRING;
				m_designMenu.InsertMenu(nPosition++,nFlags, nId,m_arrFixedCaption[i]);
			}
		}
		else
		{
			if( m_designMenu.GetMenuItemInfo(nId, &info) ) 
				m_designMenu.RemoveMenu(nId,MF_BYCOMMAND);
		}
		
	}	
}