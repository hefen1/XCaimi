/*******************************************************************************
* Copyright (c)2010, ���ݺ������ӹɷ����޹�˾
* All rights reserved.
*
* �ļ����ƣ�HsTabCfg.h
* ժ    Ҫ��Tabҳǩ�ӿ�ʵ����
*
* ��ǰ�汾��
* ��    �ߣ�����
* ������ڣ�2010-12-24
* ��	 ע��
*******************************************************************************/

#pragma once
#include "ConfigInterface.h"
#include "ConfigBase.h"
#include "ConfigStruct.h"

class CHsTabCfg : public IHsTabCfg
{
public:
	CHsTabCfg(void);
	~CHsTabCfg(void);

	void Init();
	BOOL IsInit() {return m_bInited;}

public:
	BOOL Release(BOOL bSaveToFile = FALSE);
	BOOL SaveToFile();

	// pNode������<Group>�ڵ�ĸ��ڵ� ��ȡ����<Group>�ڵ��name����ֵ
	long GetTabGroups(TiXmlNode* pNode, CStringArray& ayTabGroup);
	// pNode������<Item>�ڵ�ĸ��ڵ� ��ȡ����
	long GetTabList(TiXmlNode* pNode, CTabItemArray& ayTabItem);
public:
	virtual CString		GetDefaultTabGroup();							// ��ȡĬ����ʾ�ı�ǩҳ������
	virtual int 		GetDefaultTabItemID();						    // ��ȡĬ��ѡ�еı�ǩҳID
	
	virtual long		GetAllTabGroups(CStringArray& ayTabGroup);	    // ��ȡ���б�ǩҳ������
	virtual long		GetSysTabGroups(CStringArray& ayTabGroup);   	// ��ȡϵͳĬ�ϱ�ǩҳ������
	virtual long		GetUserTabGroups(CStringArray& ayTabGroup);  	// ��ȡ�û��Զ����ǩҳ������
	virtual long		GetUserDefMenuID(int nID);

	virtual long		GetTabList(CString strGroupName, CTabItemArray& ayTabItem);	 // ��ȡTab�б�
	virtual long		GetSysTabList(CString strGroupName,CTabItemArray& ayTabItem);// ��ȡϵͳTab�б�
	virtual BOOL		SetTabList(CString strGroupName, CTabItemArray* ayTabItem);	 // ����ָ������Tab�б�
	virtual BOOL		DelTabList(CString strGroupName);				// ɾ��ָ����ǩҳ����

private:
	CConfigBase*   m_pConfigBase;    // �ļ�������ָ��
	CMapStringToPtr m_mapTabGroup;   // ��ǩҳ��

	BOOL     m_bInited;          //�Ƿ��ʼ��
};
