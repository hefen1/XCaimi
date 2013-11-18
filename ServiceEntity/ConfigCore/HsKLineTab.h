/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsKLineTab.h
* 摘    要：K线tab配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-17
* 备	 注：
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsKLineTab :
	public IHsKLineTab
{
public:
	CHsKLineTab(void);
	~CHsKLineTab(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release();

	virtual CString GetDefaultGroupName();   // 获取默认选中的名称

	virtual int  GetTechIndexAyCount();                 // 获取指标项的数量
	virtual CString GetTechIndexName(IN int nIndex);    // 获取某一指标项的名称
	virtual int  GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex);          // 获取指标项
	virtual int  GetTechTempleteAyCount();              // 获取模板项的数量
	virtual CString GetTechTempleteName(IN int nIndex); // 获取某一模板项的名称
	virtual int  GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete); // 获取模板项

private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;

	CMapStringToPtr  m_mapIndex;          // 指标项map
	CMapStringToPtr  m_mapTemplete;       // 模板项map
};
