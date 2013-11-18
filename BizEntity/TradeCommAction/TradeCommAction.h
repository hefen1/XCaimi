/*******************************************************
  源程序名称:TradeCommAction.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  交易公共功能
  功能说明:  
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
// TradeCommAction.h : main header file for the TradeCommAction DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTradeCommActionApp
// See TradeCommAction.cpp for the implementation of this class
//

class CTradeCommActionApp : public CWinApp
{
public:
	CTradeCommActionApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
