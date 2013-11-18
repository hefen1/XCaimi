/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：ConfigBase.h
* 摘    要：杂项实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-28
* 备	 注：
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsMiscellany : IHsMiscellany
{
public:
	CHsMiscellany(void);
	~CHsMiscellany(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

public:
	virtual  CString GetSectionValue(CString lpszSection);		//按照配置文件格式内容，读取section对应的值，
	virtual  int SetSectionValue(CString lpszSection, CString lpszValue);		//按照配置文件格式内容，传入section对应的值， 


private:
	BOOL  m_bInited;
	CConfigBase*    m_pConfigBase;     // 文件操作类指针
};
