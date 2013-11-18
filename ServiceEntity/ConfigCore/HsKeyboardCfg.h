/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：CKeyboardCfg.h
* 摘    要：热键读写实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2011-01-12
* 备	 注：
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"


class CHsKeyboardCfg : IHsKeyboardCfg
{
public:
	CHsKeyboardCfg(void);
	~CHsKeyboardCfg(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

	virtual int GetOrderMarket(CStringArray &ay, CString strKey);  //获取市场排序
	virtual int GetCutShort(CStringArray &ay) ; 
private:
	CConfigBase*  m_pKeyboardCfg;
	BOOL m_bInited;
};
