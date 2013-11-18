/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	ReplyDataManger.h
*	文件标识：
*	摘	  要：	数据引擎应答管理器
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once

struct CReplyDataElement
{
	char*	m_pszData;				//数据项数据指针
	int		m_nSize;				//数据项长度
	BOOL    m_bRemoveData;          //需要手动释放内存

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
	//	构造函数与析构函数
	CReplyDataManger();
	~CReplyDataManger();

	BOOL				AddOneElement(long lSinkID, char* pData, int nDataLen,BOOL bRempve = FALSE);
	BOOL				AddElementHead(char* pszHead, int nHeadLen);
	void				RemoveAllElement();
	int					GetSize();
	int					ReplyToSink(HWND hWndAsync);
};