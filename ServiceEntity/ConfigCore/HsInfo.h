/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：ConfigInterface.h
* 摘    要：资讯配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-20
* 备	 注：
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsInfo :	public IHsInfo
{
public:
	CHsInfo(void);
	~CHsInfo(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// 接口实现部分
public:
	virtual CString GetInfoProvider();   // 获取资讯提供商

private:
	CConfigBase*   m_pConfigBase;    // 文件操作类指针
	CMapWordToPtr  m_CurrColorMap;   // 当前使用的颜色映射表	

	BOOL           m_bInited;        // 是否初始化
	CString        m_strRootPath;    // 逻辑根节点路径 
};
