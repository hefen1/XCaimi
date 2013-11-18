/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsQuoteSys.h
* 摘    要：行情系统配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-29
* 备	 注：
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"



class CHsQuoteSys : public IHsQuoteSys
{
public:
	CHsQuoteSys(void);
	~CHsQuoteSys(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// 实现接口
public:
	virtual BOOL IsSupportLevel2(); // 是否支持Level2
	virtual BOOL IsSupportFuture();//是否支持期货行情
	virtual CString GetQuoteServerInitMarket();
	virtual CString GetFutureServerInitMarket();

private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;
};
