/*******************************************************
  源程序名称:TradeStock.h
  软件著作权:恒生电子股份有限公司
  系统名称:  投资赢家金融理财终端V1.0
  模块名称:  证券交易
  功能说明:  
  作    者:  shenglq
  开发日期:  20100910
*********************************************************/
// TradeStock.h : main header file for the TradeStock DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTradeStockApp
// See TradeStock.cpp for the implementation of this class
//

class CTradeStockApp : public CWinApp
{
public:
	CTradeStockApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
