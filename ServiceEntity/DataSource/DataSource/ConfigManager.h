/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ConfigManager.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	����ͳһ����
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/
#pragma once

#include "..\..\..\Common\ConfigInterface.h"
class CConfigManager
{
protected:
	IHsMiscellany*			m_pSysMis;
	BOOL					m_bInitSuccess;
	BOOL					InitEnv();
	//
public:
	//	���캯������������
	CConfigManager();
	~CConfigManager();

	int						SetDownloadDate(CString strDownloadName, CString strDate);
	long					GetInitDateByType(CString strSectionType);
};