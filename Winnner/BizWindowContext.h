#pragma once

#include "tinyxml.h"

class CBizWindowInfo: public CObject
{
private:
	CString m_strCaption;
	CString m_strLocation;
public:
	CBizWindowInfo(CString strCaption,CString strLocation)
	{
		m_strCaption = strCaption;
		m_strLocation = strLocation;
	}
	CString GetCaption() { return m_strCaption;}
	CString GetLocation() { return m_strLocation;}
};

#define BC_FIXED_CAPTION_ARRAY_LEN  5
class CBizWindowContext :public CObject
{
private:
	CMapWordToOb  m_bizWndMap;
	CMenu         m_designMenu;
	int           m_nBaseId;
	int           GetMenuID();
	CString       m_arrFixedCaption[BC_FIXED_CAPTION_ARRAY_LEN];
public:
	static const CString m_strClassName;
public:
	CBizWindowContext(void);
	~CBizWindowContext(void);

	void BuildDesignMenu(CString strFileName);
	void BuildMenu(CMenu *pMenu,TiXmlElement* pMenuElment);
	CMenu* GetDesignMenu() { return &m_designMenu;}

	int GetBaseMenuID() { return m_nBaseId;}
	int GetSpreateID1() { return m_nBaseId;}
	int GetYSplitActionID() { return m_nBaseId - 1;}
	int GetXSplitActionID() { return m_nBaseId - 2;}
	int GetSpreateID2() { return m_nBaseId - 3;}
	int GetSaveLayoutActionID() { return m_nBaseId - 4;}
 
	CBizWindowInfo* GetBizWndInfo(int nMenuId);
	void EnableSplitAction(BOOL bEnable);
};
