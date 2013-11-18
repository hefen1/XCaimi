/*******************************************************************************
* Copyright (c)2010, 杭州恒生电子股份有限公司
* All rights reserved.
*
* 文件名称：HsGraph.h
* 摘    要：图形相关配置实现类
*
* 当前版本：
* 作    者：王超
* 完成日期：2010-12-29
* 备	 注：
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsGraph : IHsGraph
{
public:
	CHsGraph(void);
	~CHsGraph(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// 接口实现
public:
	virtual int GetIndexTrendAxisDecimal();		// 指数分时图，纵坐标显示的小数位
	virtual int GetIndexKLineAxisDecimal();		// 指数K线图，纵坐标显示的小数位

	// 分时，K线Tab页显示的指标信息
	virtual CString GetTabTechIndexDispOrder();	// 指标显示的队列顺序
private:
	BOOL m_bInited;
	CConfigBase *m_pConfigBase;
};
