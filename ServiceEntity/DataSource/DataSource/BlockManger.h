/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	BlockManger.h
*	文件标识：
*	摘	  要：	板块管理类
*	
*	当前版本：
*	作	  者：	蒋国宏
*	完成日期：  2010-9-1
*
*	备	  注： 暂时数据引擎不管理板块2010-9-21；李涛
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/

#pragma once

#define MAX_BLOCKNAME					32



class CBlockManger
{
protected:
	//本地缓存市场数据队列
	std::list<BourseInfo*>				m_listBlockInfo;
	//缓存市场数据互斥量
	CMutex								m_mutexBlock;	
public:
	//	构造函数与析构函数
	CBlockManger();
	~CBlockManger();

public:


};