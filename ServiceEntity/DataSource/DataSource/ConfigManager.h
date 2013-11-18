/*************************************************************
*	Copyright(c) 2010, 杭州恒生信息技术有限公司
*	All rights reserved.
*
*	文件名称：	ConfigManager.h
*	文件标识：
*	摘	  要：	配置统一管理
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

#include "..\..\..\Common\ConfigInterface.h"
class CConfigManager
{
protected:
	IHsMiscellany*			m_pSysMis;
	BOOL					m_bInitSuccess;
	BOOL					InitEnv();
	//
public:
	//	构造函数与析构函数
	CConfigManager();
	~CConfigManager();

	int						SetDownloadDate(CString strDownloadName, CString strDate);
	long					GetInitDateByType(CString strSectionType);
};