/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�Bridge.h
* ժ    Ҫ��������ConfigCore�ڲ������ӿ�ʵ����֮����໥����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-22
* ��	 ע��
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"

class CBridge
{
public:
	CBridge(void);
	~CBridge(void);

	static IHsColor* GetHsColor();
	static IHsFont*  GetHsFont();
};
