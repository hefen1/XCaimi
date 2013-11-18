// DiagramFoundation.h : DiagramFoundation DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "..\..\..\Common\ConfigInterface.h"
#include "..\..\..\Common\DataSourceDefineEx.h"


// CDiagramFoundationApp
// 有关此类实现的信息，请参阅 DiagramFoundation.cpp
//

class CDiagramFoundationApp : public CWinApp
{
public:
	CDiagramFoundationApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	IHsColor* m_iColor;
	IHsFont*  m_iFont;
	IDataSourceEx* m_pDataSource;
};
