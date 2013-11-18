/*************************************************************
*	Copyright(c) 2010, ���ݺ�����Ϣ�������޹�˾
*	All rights reserved.
*
*	�ļ����ƣ�	ReplyDataManger.h
*	�ļ���ʶ��
*	ժ	  Ҫ��	��������Ӧ�������
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

struct CReplyDataElement
{
	char*	m_pszData;				//����������ָ��
	int		m_nSize;				//�������
	BOOL    m_bRemoveData;          //��Ҫ�ֶ��ͷ��ڴ�

	CReplyDataElement()
	{
		memset(this, 0, sizeof(CReplyDataElement));
	}
	~CReplyDataElement()
	{
		if ( m_bRemoveData && m_pszData && !IsBadReadPtr(m_pszData,1))
			delete[] m_pszData;
			
	}
};

struct CSinksReply
{
	long							m_lSinkID;
	std::list<CReplyDataElement*>	m_listData;
	long							m_lTotalLen;
	UINT							m_uType;
};

class CReplyDataManger
{
protected:
	char*							m_pszHead;
	int								m_nHeadLen;
	std::list<CSinksReply*>			m_listSinkReply;
public:
	//	���캯������������
	CReplyDataManger();
	~CReplyDataManger();

	BOOL				AddOneElement(long lSinkID, char* pData, int nDataLen,BOOL bRempve = FALSE);
	BOOL				AddElementHead(char* pszHead, int nHeadLen);
	void				RemoveAllElement();
	int					GetSize();
	int					ReplyToSink(HWND hWndAsync);
};