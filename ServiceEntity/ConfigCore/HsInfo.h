/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�ConfigInterface.h
* ժ    Ҫ����Ѷ����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-20
* ��	 ע��
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsInfo :	public IHsInfo
{
public:
	CHsInfo(void);
	~CHsInfo(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// �ӿ�ʵ�ֲ���
public:
	virtual CString GetInfoProvider();   // ��ȡ��Ѷ�ṩ��

private:
	CConfigBase*   m_pConfigBase;    // �ļ�������ָ��
	CMapWordToPtr  m_CurrColorMap;   // ��ǰʹ�õ���ɫӳ���	

	BOOL           m_bInited;        // �Ƿ��ʼ��
	CString        m_strRootPath;    // �߼����ڵ�·�� 
};
