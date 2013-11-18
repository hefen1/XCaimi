/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsGraph.h
* ժ    Ҫ��ͼ���������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-29
* ��	 ע��
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"

class CHsGraph : IHsGraph
{
public:
	CHsGraph(void);
	~CHsGraph(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release(BOOL bSaveToFile = TRUE);
	BOOL SaveToFile();

// �ӿ�ʵ��
public:
	virtual int GetIndexTrendAxisDecimal();		// ָ����ʱͼ����������ʾ��С��λ
	virtual int GetIndexKLineAxisDecimal();		// ָ��K��ͼ����������ʾ��С��λ

	// ��ʱ��K��Tabҳ��ʾ��ָ����Ϣ
	virtual CString GetTabTechIndexDispOrder();	// ָ����ʾ�Ķ���˳��
private:
	BOOL m_bInited;
	CConfigBase *m_pConfigBase;
};
