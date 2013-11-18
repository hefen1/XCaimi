/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsUserDataCfg.h
* ժ    Ҫ�������ж�д�û���������ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-29
* ��	 ע����ΪЧ����Ҫ���ڳ�ʼ����ʱ���ֱ�ӽ����ݵ���
*******************************************************************************/
#pragma once
#include "ConfigInterface.h"
#include "ConfigStruct.h"
#include "ConfigBase.h"

class CHsUserDataCfg : IHsUserDataCfg
{
public:
	CHsUserDataCfg(void);
	~CHsUserDataCfg(void);
	void Init();
	BOOL IsInit() {return m_bInited;}

	BOOL Release();
	BOOL SaveToFile();

private:
	void LoadFileData(); // ���ļ���ȡ���뵽map�� 
// ʵ�ֽӿ�
public:
	virtual const HsCodeMark* SearchMark(IN CString& strKeyWord); //���ݹؼ��ֲ��ұ������
	virtual BOOL DelMark(IN CString& strKeyWord);		 // ɾ��ָ�����
	virtual BOOL SetMark(IN const HsCodeMark& CodeMark); // �趨���
private:
	BOOL         m_bInited;
	BOOL         m_bSave;
	CConfigBase* m_pConfigBase;

//	CCodeMarkArray	m_ayCodeMark;
	CCodeMarkMap	m_mapCodeMark;
};
