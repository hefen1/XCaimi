/*******************************************************
  Դ��������:TradeStock.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ֤ȯ����
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100910
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
