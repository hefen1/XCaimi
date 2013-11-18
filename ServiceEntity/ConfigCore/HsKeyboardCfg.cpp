#include "StdAfx.h"
#include "HsKeyboardCfg.h"
#include "HsFileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CHsFileManager gFileManager; 

CHsKeyboardCfg::CHsKeyboardCfg(void)
{
	m_pKeyboardCfg = NULL;
	m_bInited = FALSE;
}

CHsKeyboardCfg::~CHsKeyboardCfg(void)
{
}

BOOL CHsKeyboardCfg::Release(BOOL bSaveToFile)
{
	if (m_pKeyboardCfg)
	{
		return m_pKeyboardCfg->Release(bSaveToFile);
	}
	return  FALSE;
}

BOOL CHsKeyboardCfg::SaveToFile()
{
	if (m_pKeyboardCfg)
	{
		m_pKeyboardCfg->SaveToFile();
	}
	return TRUE;
}

void CHsKeyboardCfg::Init()
{
	m_pKeyboardCfg = gFileManager.GetKeyboardCfg();

	if ( m_pKeyboardCfg && !::IsBadReadPtr(m_pKeyboardCfg, 1) )
	{
		m_bInited = TRUE;
	}
}
int CHsKeyboardCfg::GetCutShort(CStringArray &ay)
{
	TiXmlNode* pNode = m_pKeyboardCfg->GetNode("//KeyboardCutShort", Default);
	if (pNode)
	{
		TiXmlElement* pChildElement = pNode->FirstChildElement();
		while (pChildElement)
		{
			CString strKey = pChildElement->Attribute("Key");
			ay.Add(strKey);

			CString strKeyName = pChildElement->Attribute("KeyName");
			ay.Add(strKeyName);

			CString strCmd = pChildElement->Attribute("CmdID");
			ay.Add(strCmd);
			
			pChildElement = pChildElement->NextSiblingElement();
		}

		return ay.GetCount();
	}

	return -1;
}

int CHsKeyboardCfg::GetOrderMarket(CStringArray &ay, CString strKey) 
{
	TiXmlNode* pNode = m_pKeyboardCfg->GetNode("//KeyboardOrderType/OrderType", "id", strKey, Default);
	if (pNode)
	{
		TiXmlElement* pChildElement = pNode->FirstChildElement();
		CString strValue = "";
		while (pChildElement)
		{
			strValue = pChildElement->Attribute("MarketType");
			ay.Add(strValue);

			pChildElement = pChildElement->NextSiblingElement();
		}

		return ay.GetCount();
	}
	return -1;
}