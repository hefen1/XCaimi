/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�CKeyboardCfg.h
* ժ    Ҫ���ȼ���дʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2011-01-12
* ��	 ע��
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"


class CHsKeyboardCfg : IHsKeyboardCfg
{
public:
	CHsKeyboardCfg(void);
	~CHsKeyboardCfg(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

	virtual int GetOrderMarket(CStringArray &ay, CString strKey);  //��ȡ�г�����
	virtual int GetCutShort(CStringArray &ay) ; 
private:
	CConfigBase*  m_pKeyboardCfg;
	BOOL m_bInited;
};
