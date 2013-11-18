/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	MapPreSubscrition.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	����Ԥ������
*	
*	��ǰ�汾��
*	��	  �ߣ�	������
*	������ڣ�  2010-9-1
*
*	��	  ע��	����map��ֻ��Ƚ�ָ��ֵ��������ȥ�Ƚ�ָ�����ݣ�����������չ
*	�޸ļ�¼��
*	��	  �ߣ�
*	�޸����ڣ�
*	�޸����ݣ�
*
****************************************************************/

#pragma once

struct CPreSubscritionCodeInfo 
{
	CodeInfo	m_codeinfo;
	BOOL		m_bAlreadySet;

	CPreSubscritionCodeInfo()
	{
		memset(this, 0, sizeof(CPreSubscritionCodeInfo));
	}
};

class CMapPreSubscrition
{
public:
	//	���캯������������
	CMapPreSubscrition();
	~CMapPreSubscrition();
protected:
	std::list<CPreSubscritionCodeInfo*>		m_listData;
public:
	//����ֵ
	void							SetAt(CodeInfo* pInfo, BOOL bCodeAlreadySet, BOOL bCheckExist = FALSE);
	BOOL							RemoveKey(CodeInfo* pInfo);
	void							RemoveAll();
	int								GetSize();
	BOOL							CopyCodeInfo(CodeInfo* pInfoCopyed, int nSize);
	int								GetUnSetSubList(std::list<CodeInfo* >&list);
};