// DiagramRTable.h : DiagramRTable DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ConfigInterface.h"
#include "DataSourceDefineEx.h"


// CDiagramRTableApp
// 有关此类实现的信息，请参阅 DiagramRTable.cpp
//

class CDiagramRTableApp : public CWinApp
{
public:
	CDiagramRTableApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
public:
	void   CreateInterface();
public:
	IHsInfo*  m_iInfo;
	IHsColor* m_iColor;
	IHsFont*  m_iFont;
	IDataSourceEx*    m_pDataSource;
	IHsUserDataCfg*   m_iUserDefCfg;    // 用户数据接口（股票标记）
};
