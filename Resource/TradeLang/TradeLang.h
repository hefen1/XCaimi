/*******************************************************
  Դ��������:TradeLang.h
  �������Ȩ:�������ӹɷ����޹�˾
  ϵͳ����:  Ͷ��Ӯ�ҽ�������ն�V1.0
  ģ������:  ������Դ
  ����˵��:  
  ��    ��:  shenglq
  ��������:  20100921
*********************************************************/
// TradeLang.h : TradeLang DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTradeLangApp
// �йش���ʵ�ֵ���Ϣ������� TradeLang.cpp
//

class CTradeLangApp : public CWinApp
{
public:
	CTradeLangApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
