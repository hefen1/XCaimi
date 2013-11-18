/*******************************************************
  源程序名称:TradeLang.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易资源
  功能说明:  
  作    者:  shenglq
  开发日期:  20100921
*********************************************************/
// TradeLang.h : TradeLang DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTradeLangApp
// 有关此类实现的信息，请参阅 TradeLang.cpp
//

class CTradeLangApp : public CWinApp
{
public:
	CTradeLangApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
