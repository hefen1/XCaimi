/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	MapPreSubscrition.h
*	文件标识：
*	摘	  要：	订阅预处理类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注：	由于map中只会比较指针值，而不是去比较指针内容，所以做此扩展
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
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
	//	构造函数与析构函数
	CMapPreSubscrition();
	~CMapPreSubscrition();
protected:
	std::list<CPreSubscritionCodeInfo*>		m_listData;
public:
	//设置值
	void							SetAt(CodeInfo* pInfo, BOOL bCodeAlreadySet, BOOL bCheckExist = FALSE);
	BOOL							RemoveKey(CodeInfo* pInfo);
	void							RemoveAll();
	int								GetSize();
	BOOL							CopyCodeInfo(CodeInfo* pInfoCopyed, int nSize);
	int								GetUnSetSubList(std::list<CodeInfo* >&list);
};