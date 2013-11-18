#pragma once

#include "tinyxml.h"

class CMenuTreeItem: public CObject
{
private:
	CString m_strCaption;
    CString m_strLayoutFile;
public:
	CMenuTreeItem(CString strCaption,CString strLayoutFile)
	{
		m_strCaption = strCaption;
		m_strLayoutFile = strLayoutFile;
	}

	CString GetCaption() { return m_strCaption;}
	CString GetLayoutFile() { return m_strLayoutFile;}
};

/////////////////////////////////////////////////////////////////////////////
// CMenuTree 窗口

class CMenuTree : public CTreeCtrl
{
// 构造
public:
	CMenuTree();
	virtual ~CMenuTree();
// 重写
protected:
	//CMapStringToOb    m_MenuItemMap;
	CMapStringToPtr	  m_MenuItemMap;
	CPtrArray		  m_paMenuItem;
	BOOL              m_bBuilding;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
// 实现
public:	
	void BuildMenuTree(CString strMenuFile);
	HTREEITEM GetMenuTreeItem(CString strLayout);
	BOOL IsBuilding() { return m_bBuilding; }
	void BuildMenuTree(HTREEITEM h,TiXmlElement* pMenuElment);
	void Release();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};