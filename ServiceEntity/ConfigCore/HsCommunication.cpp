#include "StdAfx.h"
#include "HsCommunication.h"
#include "HsFileManager.h"
#include "NetEngineInterface.h"
#include "..\..\HelperLib\GeneralHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 


CHsCommunication::CHsCommunication(void)
{
	m_bInited = FALSE;
	m_pConfigBase = NULL;
}

CHsCommunication::~CHsCommunication(void)
{
	Release(TRUE);
}

void CHsCommunication::Init()
{
	m_pConfigBase = gFileManager.GetServerCfg();

	if (m_pConfigBase && !::IsBadReadPtr(m_pConfigBase, 1))
	{
		m_bInited = TRUE;
	}
}

BOOL CHsCommunication::Release( BOOL bSaveToFile /*= TRUE*/ )
{
	if (m_pConfigBase)
	{
		m_pConfigBase->Release(bSaveToFile);
	}

	POSITION pos = m_mapSysServer.GetStartPosition();
	CCommInfoArray* pAy = NULL;
	CString strKey = "";
	
	//////////////////////////////////////////////////////////////////////////
	// 在login析构的时候释放
	// 这里特别要注意
	while (pos)
	{
		m_mapSysServer.GetNextAssoc(pos, strKey, (void*&)pAy);
		if (pAy)
		{
// 			for (int i = 0; i < pAy->GetCount(); i++)
// 			{
// 				CommInfo* pInfo = pAy->GetAt(i);
// 				if (pInfo && !::IsBadReadPtr(pInfo, 1))
// 				{
// 					delete pInfo;
// 					pInfo = NULL;
// 				}
// 			}
			pAy->RemoveAll();
		}
		delete pAy;
	}

	pos = m_mapUserServer.GetStartPosition();
	while (pos)
	{
		m_mapUserServer.GetNextAssoc(pos, strKey, (void*&)pAy);
		if (pAy)
		{
// 			for (int i = 0; i < pAy->GetCount(); i++)
// 			{
// 				CommInfo* pInfo = pAy->GetAt(i);
// 				if (pInfo && !::IsBadReadPtr(pInfo, 1))
// 				{
// 					delete pInfo;
// 					pInfo = NULL;
// 				}
// 			}
			pAy->RemoveAll();
		}
		delete pAy;
	}

	return TRUE;
}

BOOL CHsCommunication::SaveToFile()
{
	if (m_pConfigBase)
	{
		m_pConfigBase->SaveToFile();
	}
	return TRUE;
}

CString CHsCommunication::GetTypeName(Server_type type)
{
	CString str;
	switch(type)
	{
	case CEV_Connect_HQ:
		str = "Quote";
		break;
	case CEV_Connect_QH:
		str = "Future";
		break;
	case CEV_Connect_JY:
		str = "Trade";
		break;
	case CEV_Connect_LEVEL2:
		str = "Level2";
		break;
	case CEV_Connect_CENTER:
		str = "Center";
		break;
	case CEV_Connect_ZX:
		str = "Info";
		break;
	default:
		str = "";
		break;
	}
	return str;
}

Server_type CHsCommunication::GetServerType(CString strType)
{
	Server_type type;
	if (strType.Compare("Quote") == 0)
	{
		type = CEV_Connect_HQ;
	}
	else if (strType.Compare("Future") == 0)
	{
		type = CEV_Connect_QH;
	}
	else if (strType.Compare("Trade") == 0)
	{
		type = CEV_Connect_JY;
	}
	else if (strType.Compare("Level2") == 0)
	{
		type = CEV_Connect_LEVEL2;
	}
	else if (strType.Compare("Center") == 0)
	{
		type = CEV_Connect_CENTER;
	}
	else if (strType.Compare("Info") == 0)
	{
		type = CEV_Connect_ZX;
	}
	else
	{
		type = CEV_Connect_ALL;
	}

	return type;
}

void CHsCommunication::GetServer(Server_type type, TiXmlNode* pNode, CCommInfoArray *pAy)
{
	if (pNode == NULL || ::IsBadReadPtr(pNode, 1) || pAy == NULL)
	{
		return ;
	}
	
	TiXmlNode* pChildNode = pNode->FirstChild();
	while (pChildNode)
	{
		if (strncmp(pChildNode->Value(),"site", sizeof("site")) == 0)
		{
			CString strName = pChildNode->ToElement()->Attribute("name");
			CString strIP = pChildNode->ToElement()->Attribute("IP");
			int port(0);
			pChildNode->ToElement()->Attribute("port", &port);

			CommInfo *pComm = new CommInfo;
			memset(pComm, 0, sizeof(CommInfo));
			pComm->m_cType = type;
			pComm->m_nPort = port;
			strcpy(pComm->m_strServerID, strName.GetBuffer());
			strcpy(pComm->m_strServerIP, strIP.GetBuffer());
			pComm->m_cIsDefault = 0;
			pAy->Add(pComm);
		}
		pChildNode = pChildNode->NextSibling();
	}
}

int CHsCommunication::GetSysServer( Server_type type, CCommInfoArray *pAy, int nCellID)
{
	CString strType = GetTypeName(type);
	if (strType.IsEmpty())
		return -1;
	else 
	{
		if ( type == CEV_Connect_JY)
		{// 如果是交易 则再加上营业部ID
			strType.AppendFormat("%d", nCellID);
		}
	}

	CCommInfoArray* pAyInfo = NULL;
	if (m_mapSysServer.Lookup(strType, (void*&)pAyInfo))
	{
		pAy->Append(*pAyInfo);
		return pAy->GetCount();
	}
	else
	{
		if (type != CEV_Connect_JY)
		{
			CString strPath = "//" + strType;
			TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, SystemDoc);
			if (pNode == NULL)
			{
				pNode = m_pConfigBase->GetNode(strPath, ResourceDoc);
			}
			if (pNode && !::IsBadReadPtr(pNode, 1))
			{
				pAyInfo = new CCommInfoArray;
				GetServer(type, pNode, pAyInfo);
				pAy->Append(*pAyInfo);
				m_mapSysServer.SetAt(strType, (void*)pAyInfo);
			}
			return pAy->GetCount();
		}
		else
		{// 交易要单独处理

			CString strPath = "//Trade/Cell";
			CString strID;
			strID.Format("%d", nCellID);
			TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "id", strID, SystemDoc);
			if (pNode == NULL)
				pNode = m_pConfigBase->GetNode(strPath, "id", strID, ResourceDoc);
			
			if (pNode && !::IsBadReadPtr(pNode, 1))
			{
				pAyInfo = new CCommInfoArray;
				GetServer(type, pNode, pAyInfo);
				pAy->Append(*pAyInfo);
				m_mapSysServer.SetAt(strType, (void*)pAyInfo);
			}
			else
			{// 如果没有配IP 则直接读取默认
				strPath = "//Trade";
				pNode = m_pConfigBase->GetNode(strPath, SystemDoc);
				if (pNode == NULL)
					pNode = m_pConfigBase->GetNode(strPath, ResourceDoc);

				if (pNode && !::IsBadReadPtr(pNode, 1))
				{
					pAyInfo = new CCommInfoArray;
					GetServer(type, pNode, pAy);
					pAy->Append(*pAyInfo);
					m_mapSysServer.SetAt(strType, (void*)pAyInfo);
				}
			}
			return pAy->GetCount();
		}
	}
}

int CHsCommunication::GetUsrServer( Server_type type, CCommInfoArray *pAy, int nCellID)
{
	CString strType = GetTypeName(type);
	if (strType.IsEmpty())
		return -1;
	else 
	{
		if ( type == CEV_Connect_JY)
		{// 如果是交易 则再加上营业部ID
			strType.AppendFormat("%d", nCellID);
		}
	}

	CCommInfoArray* pAyInfo = NULL;
	if (m_mapUserServer.Lookup(strType, (void*&)pAyInfo))
	{
		pAy->Append(*pAyInfo);
		return pAy->GetCount();
	}
	else
	{
		if (type != CEV_Connect_JY)
		{
			CString strPath = "//" + strType;
			TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, UserDoc);
			if (pNode && !::IsBadReadPtr(pNode, 1))
			{
				pAyInfo = new CCommInfoArray;
				GetServer(type, pNode, pAyInfo);
				pAy->Append(*pAyInfo);
				m_mapUserServer.SetAt(strType, (void*)pAyInfo);				
			}
			return pAy->GetCount();
		}
		else
		{// 交易要单独处理

			CString strPath = "//Trade/Cell";
			CString strID;
			strID.Format("%d", nCellID);
			TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, "id", strID, UserDoc);
			if (pNode && !::IsBadReadPtr(pNode, 1))
			{
				pAyInfo = new CCommInfoArray;
				GetServer(type, pNode, pAyInfo);
				pAy->Append(*pAyInfo);
				m_mapUserServer.SetAt(strType, (void*)pAyInfo);
			}
			return pAy->GetCount();
		}
	}
}

int CHsCommunication::GetAllSysServer( CCommInfoArray *pAy, int nCellID)
{
	GetSysServer(CEV_Connect_HQ, pAy);
	GetSysServer(CEV_Connect_QH, pAy);
	GetSysServer(CEV_Connect_JY, pAy, nCellID);
	GetSysServer(CEV_Connect_LEVEL2, pAy);
	GetSysServer(CEV_Connect_ZX, pAy);
	GetSysServer(CEV_Connect_CENTER, pAy);
	return pAy->GetCount();
}

int CHsCommunication::GetAllUsrServer( CCommInfoArray *pAy, int nCellID)
{
	GetUsrServer(CEV_Connect_HQ, pAy);
	GetUsrServer(CEV_Connect_QH, pAy);
	GetUsrServer(CEV_Connect_JY, pAy, nCellID);
	GetUsrServer(CEV_Connect_LEVEL2, pAy);
	GetUsrServer(CEV_Connect_ZX, pAy);
	GetUsrServer(CEV_Connect_CENTER, pAy);
	return pAy->GetCount();
}

void CHsCommunication::SaveUsrServer( Server_type type, CCommInfoArray *pAy, int nCellID)
{
	CString strType = GetTypeName(type);
	if (strType.IsEmpty() || pAy == NULL || ::IsBadReadPtr(pAy, 1))
		return;
	else
	{
		if (type == CEV_Connect_JY)
			strType.AppendFormat("%d", nCellID);
	}
	
	CCommInfoArray ayType; // 从pAy里面过滤出类型为type
	for (int i = 0; i < pAy->GetCount(); i++)
	{
		CommInfo* pInfo = pAy->GetAt(i);
		if (pInfo->m_cType == type)
		{
			ayType.Add(pInfo);
		}
	}
// 	if (ayType.GetCount() == 0)
// 	{
// 		return;
// 	}

	CCommInfoArray ayOld;
	GetUsrServer(type, &ayOld, nCellID);
	if (ayOld.GetCount() == ayType.GetCount())
	{
		CommInfo* pOne = NULL;
		CommInfo* pOther = NULL;
		int i(0);
		for (i = 0; i < ayOld.GetCount(); i++)
		{
			pOne = ayOld.GetAt(i);
			pOther = ayType.GetAt(i);
			if ( strcmp(pOne->m_strServerID, pOther->m_strServerID) || strcmp(pOne->m_strServerIP, pOther->m_strServerIP) )
				break;
		}
		if (i >= ayOld.GetCount())
		{// 如果没有更改 则不保存
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 删除缓存数据
	RemoveSysMap(type, nCellID);
	//////////////////////////////////////////////////////////////////////////

	// 先设置父节点
	TiXmlElement* pElement = NULL;
	if (type == CEV_Connect_JY)
	{// 交易要单独处理
		CString strPath = "//Trade/Cell";
		CString strID;
		strID.Format("%d", nCellID);
		pElement = (TiXmlElement*)m_pConfigBase->GetNode(strPath, "id", strID, UserDoc);
		if (pElement == NULL)
		{
			pElement = (TiXmlElement*)m_pConfigBase->SetNode(strPath, "id", strID);
			pElement->RemoveAttribute("default");
		}
		else
			pElement->Clear();
	}
	else
	{
		CString strPath = "//" + strType;
		pElement = (TiXmlElement*)m_pConfigBase->GetNode(strPath, UserDoc);
		if (pElement == NULL)
		{
			pElement = new TiXmlElement(strType);
			m_pConfigBase->GetRootNode(UserDoc)->LinkEndChild(pElement);
		}
		else
		{
			pElement->Clear();
		}
	}
		
	for (int i = 0; i < ayType.GetCount(); i++)
	{
		CommInfo* pInfo = ayType.GetAt(i);
		if (pInfo->m_cType == type)
		{
			TiXmlElement* pChildElement = new TiXmlElement("site");
			// 服务器名称
			pChildElement->SetAttribute("name", pInfo->m_strServerID);
			// 服务器IP
			pChildElement->SetAttribute("IP", pInfo->m_strServerIP);
			// 服务器端口
			pChildElement->SetAttribute("port", pInfo->m_nPort);

			pElement->LinkEndChild(pChildElement);
		}
	}
	m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
}

void CHsCommunication::SaveAllUsrServer( CCommInfoArray *pAy, int nCellID ) 
{
	if (pAy == NULL || ::IsBadReadPtr(pAy, 1))
	{
		return ;
	}

	SaveUsrServer(CEV_Connect_HQ, pAy);
	SaveUsrServer(CEV_Connect_QH, pAy);
	SaveUsrServer(CEV_Connect_JY, pAy, nCellID);
	SaveUsrServer(CEV_Connect_LEVEL2, pAy);
	SaveUsrServer(CEV_Connect_ZX, pAy);
	SaveUsrServer(CEV_Connect_CENTER, pAy);
}

BOOL CHsCommunication::GetProxy( ProxyInfo *proxy )
{
	if (proxy == NULL || ::IsBadReadPtr(proxy, 1))
		return FALSE;

	CString strPath = "//Proxy";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		TiXmlElement* pElement = pNode->ToElement();
		pElement->Attribute("useChannel",&proxy->m_nUseChannel);
		
		TiXmlElement *pChild = pElement->FirstChildElement();
		if (pChild && !::IsBadReadPtr(pChild, 1))
		{
			// 代理类型
			Proxy_type pType = (Proxy_type)CGeneralHelper::StringToLongSimple(pChild->Attribute("type"));
			// IP
			CString strIP = pChild->Attribute("IP");
			// 端口
			int port = CGeneralHelper::StringToLongSimple(pChild->Attribute("port"));
			// 用户名
			CString strUser = pChild->Attribute("username");
			// 密码
			CString strPWD = pChild->Attribute("userpwd");
			proxy->m_nPort = port;
			strncpy(proxy->m_strProxyIP, strIP.GetBuffer(), sizeof(proxy->m_strProxyIP));
			strncpy(proxy->m_strUser, strUser.GetBuffer(), sizeof(proxy->m_strUser));
			strncpy(proxy->m_strPwd, strPWD.GetBuffer(), sizeof(proxy->m_strPwd));
			proxy->m_nOptions = pType;

			return TRUE;
		}
	}
	return FALSE;
}

void CHsCommunication::SaveProxy( ProxyInfo *proxy )
{
	if (proxy == NULL || ::IsBadReadPtr(proxy, 1))
	{
		return ;
	}

	ProxyInfo oldProxy;
	memset(&oldProxy,0,sizeof(ProxyInfo));
	oldProxy.m_nOptions = NoProxy;
	oldProxy.m_nUseChannel = 0;
	GetProxy(&oldProxy);
	if ( memcmp(&oldProxy, proxy, sizeof(ProxyInfo)) == 0)
	{
		return;
	}

	TiXmlNode* pNode = m_pConfigBase->GetNode("//Proxy");
	if (pNode == NULL)
	{
		TiXmlElement* pElement = new TiXmlElement("Proxy");
		pNode = m_pConfigBase->GetRootNode(UserDoc);
		if (pNode && !::IsBadReadPtr(pNode, 1))
		{
			pNode->LinkEndChild(pElement);
		}
		pNode = pElement;
	}

	m_pConfigBase->SetNodeAttrInt(pNode, "useChannel", proxy->m_nUseChannel);
	TiXmlElement* pElement = new TiXmlElement("addr");
	pNode->LinkEndChild(pNode);
	
	pNode = pElement;
	m_pConfigBase->SetNodeAttrInt(pNode,    "type",     proxy->m_nOptions);
	m_pConfigBase->SetNodeAttrString(pNode, "IP",       proxy->m_strProxyIP);
	m_pConfigBase->SetNodeAttrInt(pNode,    "port",     proxy->m_nPort);
	m_pConfigBase->SetNodeAttrString(pNode, "username", proxy->m_strUser);
	m_pConfigBase->SetNodeAttrString(pNode, "userpwd",  proxy->m_strPwd);

	
}
BOOL CHsCommunication::GetAuto()
{
	CString strPath = "//Switch/AutoChooseBest";
	return m_pConfigBase->GetNodeAttrInt(strPath, "auto");
}

BOOL CHsCommunication::SetAutoChooseBest( UINT au )
{
	if (au == GetAuto())
	{
		return TRUE;
	}

	CString strPath = "//Switch/AutoChooseBest";
	return m_pConfigBase->SetNodeAttrInt(strPath, "auto", au);
}

int CHsCommunication::GetHeartBeat(Server_type type)
{
	CString strType = GetTypeName(type);
	CString strPath = "//" + strType + "/HeartBeat";

	int nHeartBeat = m_pConfigBase->GetNodeAttrInt(strPath, "value", HeartBeat_MIN);
	nHeartBeat = nHeartBeat<HeartBeat_MIN?HeartBeat_MIN:nHeartBeat;
	return nHeartBeat;
}

BOOL CHsCommunication::GetSwitch( Server_type type )
{
	CString strType = GetTypeName(type);
	if (strType.IsEmpty())
	{
		return FALSE;
	}

	CString strPath = "//Switch/" + strType;
	return m_pConfigBase->GetNodeAttrInt(strPath, "value");
}

void CHsCommunication::GetAllSwitch( CMap<Server_type,Server_type,BOOL,BOOL> *pAy )
{
	if (pAy == NULL || ::IsBadReadPtr(pAy, 1))
	{
		return ;
	}

	CString strPath = "//Switch";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath);
	if (pNode && !::IsBadReadPtr(pNode, 1))
	{
		Server_type type;
		BOOL bSwitch;
		TiXmlElement* pChild = pNode->FirstChildElement();
		while (pChild)
		{
			type = GetServerType(pChild->Value());
			if (type != CEV_Connect_ALL)
			{
				pChild->Attribute("value", (int*)&bSwitch);
				pAy->SetAt(type, bSwitch);
			}

			pChild = pChild->NextSiblingElement();
		}
	}
}

CString CHsCommunication::GetDefaultSite( Server_type type, int nCellID)
{
	CString strType = GetTypeName(type);
	if (strType.IsEmpty())
		return "";

	if (type == CEV_Connect_JY)
	{
		CString strPath = "//Trade/Cell";
		CString strID;
		strID.Format("%d", nCellID);
		return m_pConfigBase->GetNodeAttrString(strPath, "default", "", "id", strID);
	}
	else
	{
		CString strPath = "//" + strType;
		return m_pConfigBase->GetNodeAttrString(strPath, "default");
	}
}

void CHsCommunication::GetAllDefaultSite( CMap<Server_type,Server_type,CString,CString> *pAy, int nCellID)
{
	if (pAy == NULL || ::IsBadReadPtr(pAy, 1))
	{
		return ;
	}

	CString strPath = "";
	CString strDefault = "";

	// 行情
	strPath = "//Quote";
	strDefault = m_pConfigBase->GetNodeAttrString(strPath, "default");
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_HQ, strDefault);
	}
	//期货
	strPath = "//Future";
	strDefault = m_pConfigBase->GetNodeAttrString(strPath, "default");
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_QH, strDefault);
	}

	// 交易
	strDefault = GetDefaultSite(CEV_Connect_JY, nCellID);
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_JY, strDefault);
	}

	// Level2
	strPath = "//Level2";
	strDefault = m_pConfigBase->GetNodeAttrString(strPath, "default");
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_LEVEL2, strDefault);
	}

	// 中心端
	strPath = "//Center";
	strDefault = m_pConfigBase->GetNodeAttrString(strPath, "default");
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_CENTER, strDefault);
	}

	// 咨询
	strPath = "//Info";
	strDefault = m_pConfigBase->GetNodeAttrString(strPath, "default");
	if (!strDefault.IsEmpty())
	{
		pAy->SetAt(CEV_Connect_ZX, strDefault);
	}

}

void CHsCommunication::SetDefaultSite(Server_type type, CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID)
{
	CString strName = GetTypeName(type);
	if (strName.IsEmpty() || pSysAy == NULL || pUsrAy == NULL)
	{
		return;
	}

	// 先找到默认站点
	CommInfo* pDefault = NULL;
	for (int i = 0; i < pSysAy->GetCount(); i++)
	{
		if (pSysAy->GetAt(i)->m_cIsDefault == 1 && pSysAy->GetAt(i)->m_cType == type)
		{
			pDefault = pSysAy->GetAt(i);
			break;
		}
	}
	
	for (int i = 0; i < pUsrAy->GetCount(); i++)
	{
		if (pUsrAy->GetAt(i)->m_cIsDefault == 1 && pUsrAy->GetAt(i)->m_cType == type)
		{
			pDefault = pUsrAy->GetAt(i);
			break;
		}
	}

	if (pDefault)
	{
		if (GetDefaultSite(type, nCellID).Compare(pDefault->m_strServerID) == 0)
		{// 默认站点没有改变
			return;
		}

		if (type == CEV_Connect_JY)
		{
			CString strPath = "//Trade/Cell";
			CString strID;
			strID.Format("%d", nCellID);
			m_pConfigBase->SetNodeAttrString(strPath, "default", pDefault->m_strServerID, "id", strID);
		}
		else
		{
			CString strType = GetTypeName(type);
			CString strPath = "//" + strType;
			m_pConfigBase->SetNodeAttrString(strPath, "default", pDefault->m_strServerID);
		}
	}
}

void CHsCommunication::SetAllDefaultSite(CCommInfoArray *pSysAy, CCommInfoArray *pUsrAy, int nCellID)
{
	if (pSysAy == NULL || pUsrAy == NULL)
	{
		return;
	}

	SetDefaultSite(CEV_Connect_HQ, pSysAy, pUsrAy);
	SetDefaultSite(CEV_Connect_JY, pSysAy, pUsrAy, nCellID);
	SetDefaultSite(CEV_Connect_LEVEL2, pSysAy, pUsrAy);
	SetDefaultSite(CEV_Connect_ZX, pSysAy, pUsrAy);
	SetDefaultSite(CEV_Connect_CENTER, pSysAy, pUsrAy);
}

void CHsCommunication::RemoveSysMap( Server_type type, int nCellID )
{
	CString strName = GetTypeName(type);
	if (type == CEV_Connect_JY)
		strName.AppendFormat("%d", nCellID);

	CCommInfoArray* pAy = NULL;
	if (m_mapSysServer.Lookup(strName, (void*&)pAy))
	{
		delete pAy;
		m_mapSysServer.RemoveKey(strName);
	}
}
void CHsCommunication::RemoveUsrMap( Server_type type, int nCellID )
{
	CString strName = GetTypeName(type);
	if (type == CEV_Connect_JY)
		strName.AppendFormat("%d", nCellID);

	CCommInfoArray* pAy = NULL;
	if (m_mapUserServer.Lookup(strName, (void*&)pAy))
	{
		delete pAy;
		m_mapUserServer.RemoveKey(strName);
	}
}

int CHsCommunication::GetSaveAccount()
{
	CString strPath = "//Account/SaveAccount";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value", 0);
}

BOOL CHsCommunication::SetSaveAccount( UINT unSave )
{
	if (unSave == GetSaveAccount())
		return TRUE;
	
	CString strPath = "//Account/SaveAccount";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", unSave);
}

int CHsCommunication::GetHideAccount()
{
	CString strPath = "//Account/HideAccount";
	return m_pConfigBase->GetNodeAttrInt(strPath, "value", 0);
}

BOOL CHsCommunication::SetHideAccount( UINT unHide )
{
	if (unHide == GetHideAccount())
		return TRUE;

	CString strPath = "//Account/HideAccount";
	return m_pConfigBase->SetNodeAttrInt(strPath, "value", unHide);
}

BOOL CHsCommunication::GetLevelAcc(CString &accName, CString &accPwd)
{
	CString strPath = "//Account/Level2Acc";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, Default);
	if (pNode)
	{
		accName = pNode->ToElement()->Attribute("name");
		accPwd  = pNode->ToElement()->Attribute("pwd");
		return TRUE;
	}
	return FALSE;
}

int CHsCommunication::GetAccount( CStringArray& ayAccInfo )
{
	CString strPath = "//Account";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, UserDoc);
	if (pNode)
	{// 查找保存的账号信息
		TiXmlNode* pChild = pNode->FirstChild(AccNodeValue);
		int nCount(0);
		while (pChild)
		{
			TiXmlElement* pElement = pChild->ToElement();
			CString strAcc = pElement->Attribute("name");     // 账号名称
			CString strType = pElement->Attribute("type");    // 账号类型
			CString strCellID = pElement->Attribute("cell");  // 营业厅

			ayAccInfo.Add(strAcc);
			ayAccInfo.Add(strType);
			ayAccInfo.Add(strCellID);
			pChild = pChild->NextSibling(AccNodeValue);
			nCount++;
		}
		return nCount;
	}
	return 0;
}

BOOL CHsCommunication::SaveAccount( CStringArray& ayAccInfo, int nAccCount )
{
	if (nAccCount <= 0)
		return TRUE;
	
	//////////////////////////////////////////////////////////////////////////
	// 确保父节点一定存在
	CString strPath = "//Account";
	TiXmlNode* pNode = m_pConfigBase->SetNode(strPath);
	if (pNode)
	{
		//////////////////////////////////////////////////////////////////////////
		// 删除之前的账号信息节点
		TiXmlNode* pChild = pNode->FirstChild(AccNodeValue);
		while(pChild)
		{
			pNode->RemoveChild(pChild);
			pChild = pNode->FirstChild(AccNodeValue);
		}
		//////////////////////////////////////////////////////////////////////////
		// 添加新的账号信息节点
		for (int i = 0; i < nAccCount; i++)
		{
			TiXmlElement* pElement = new TiXmlElement(AccNodeValue);
			pElement->SetAttribute("name", ayAccInfo.GetAt(i*3));
			pElement->SetAttribute("type", ayAccInfo.GetAt(i*3+1));
			pElement->SetAttribute("cell", ayAccInfo.GetAt(i*3+2));

			pNode->LinkEndChild((TiXmlNode*)pElement);
		}
		m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL CHsCommunication::RemoveAllAccount()
{
	CString strPath = "//Account";
	TiXmlNode* pNode = m_pConfigBase->GetNode(strPath, UserDoc);
	if (pNode == NULL)
		return TRUE;

	TiXmlNode* pChildNode = pNode->FirstChild(AccNodeValue);
	while (pChildNode)
	{
		pNode->RemoveChild(pChildNode);
		pChildNode = pNode->FirstChild(AccNodeValue);
	}
	m_pConfigBase->GetXmlDocument(UserDoc)->SetModified(TRUE);

	return TRUE;
}