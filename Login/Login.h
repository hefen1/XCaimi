// Login.h : Login DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

class CLoginApp : public CWinApp
{
public:
	CLoginApp();
	// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
