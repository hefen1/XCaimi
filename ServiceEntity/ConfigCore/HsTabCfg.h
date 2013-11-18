/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsTabCfg.h
* 摘    要：Tab页签接口实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-24
* 备	 注：
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsTabCfg : public IHsTabCfg
{
public:
	CHsTabCfg(void);
	~CHsTabCfg(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

public:
	BOOL Release(BOOL bSaveToFile = FALSE);
	BOOL SaveToFile();

	// pNode是所有<Group>节点的父节点 获取所有<Group>节点的name属性值
	long GetTabGroups(TiXmlNode* pNode, CStringArray& ayTabGroup);
	// pNode是所有<Item>节点的父节点 获取所有
	long GetTabList(TiXmlNode* pNode, CTabItemArray& ayTabItem);
public:
	virtual CString		GetDefaultTabGroup();							// 获取默认显示的标签页组名称
	virtual int 		GetDefaultTabItemID();						    // 获取默认选中的标签页ID
	
	virtual long		GetAllTabGroups(CStringArray& ayTabGroup);	    // 获取所有标签页组名称
	virtual long		GetSysTabGroups(CStringArray& ayTabGroup);   	// 获取系统默认标签页组名称
	virtual long		GetUserTabGroups(CStringArray& ayTabGroup);  	// 获取用户自定义标签页组名称
	virtual long		GetUserDefMenuID(int nID);

	virtual long		GetTabList(CString strGroupName, CTabItemArray& ayTabItem);	 // 获取Tab列表
	virtual long		GetSysTabList(CString strGroupName,CTabItemArray& ayTabItem);// 获取系统Tab列表
	virtual BOOL		SetTabList(CString strGroupName, CTabItemArray* ayTabItem);	 // 设置指定分类Tab列表
	virtual BOOL		DelTabList(CString strGroupName);				// 删除指定标签页分类

private:
	CConfigBase*   m_pConfigBase;    // 文件操作类指针
	CMapStringToPtr m_mapTabGroup;   // 标签页组

	BOOL     m_bInited;          //是否初始化
};
