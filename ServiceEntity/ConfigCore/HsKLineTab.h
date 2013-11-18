/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsKLineTab.h
* ժ    Ҫ��K��tab����ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-17
* ��	 ע��
*******************************************************************************/
#pragma once

#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsKLineTab :
	public IHsKLineTab
{
public:
	CHsKLineTab(void);
	~CHsKLineTab(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release();

	virtual CString GetDefaultGroupName();   // ��ȡĬ��ѡ�е�����

	virtual int  GetTechIndexAyCount();                 // ��ȡָ���������
	virtual CString GetTechIndexName(IN int nIndex);    // ��ȡĳһָ���������
	virtual int  GetTechIndex(IN int nIndex, OUT CTechIndexArray& ayTechIndex);          // ��ȡָ����
	virtual int  GetTechTempleteAyCount();              // ��ȡģ���������
	virtual CString GetTechTempleteName(IN int nIndex); // ��ȡĳһģ���������
	virtual int  GetTechTemplete(IN int nIndex, OUT CTechTempleteArray& ayTechTemplete); // ��ȡģ����

private:
	BOOL m_bInited;
	CConfigBase* m_pConfigBase;

	CMapStringToPtr  m_mapIndex;          // ָ����map
	CMapStringToPtr  m_mapTemplete;       // ģ����map
};
