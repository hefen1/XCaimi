/*******************************************************
  Դ��������:TradeCommAction.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ���׹�������
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100910
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
