/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�ConfigBase.h
* ժ    Ҫ������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-28
* ��	 ע��
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsMiscellany : IHsMiscellany
{
public:
	CHsMiscellany(void);
	~CHsMiscellany(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

public:
	virtual  CString GetSectionValue(CString lpszSection);		//���������ļ���ʽ���ݣ���ȡsection��Ӧ��ֵ��
	virtual  int SetSectionValue(CString lpszSection, CString lpszValue);		//���������ļ���ʽ���ݣ�����section��Ӧ��ֵ�� 


private:
	BOOL  m_bInited;
	CConfigBase*    m_pConfigBase;     // �ļ�������ָ��
};
