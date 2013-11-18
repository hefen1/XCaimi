/*************************************************************
*	Copyright(c) 2010, 杭州恒生股份有限公司
*	All rights reserved.
*
*	文件名称：	CDiagramComm.h
*	文件标识：
*	摘	  要：  图元共同使用的一些定义
*	
*	当前版本：
*	作	  者：	王超
*	完成日期：  2011-3-21
*
*	备	  注：
*	修改记录：
*	作	  者：
*	修改日期：
*	修改内容：
*
****************************************************************/
#pragma once
#include "..\..\..\Common\QuoteComm\QuoteDefine.h"

#define INFO_PATH_KEY_F10	    "%sF10资讯路径"   // F10资讯
#define INFO_PATH_KEY_TREND		"%s分时资讯路径"  // 分时资讯
#define INFO_PATH_KEY_TECH		"%s盘后资讯路径"  // 盘后资讯
#define INFO_PATH_KEY_REPORT	"%s报表资讯路径"  // 报表资讯
#define INFO_PATH_KEY_SPEC		"%s特色资讯路径"  // 特色资讯

#define HTMLFORMAT_PATH         "InfoMine.template" // 存放html格式串文件的文件名
#define HTMLCONTENT_PATH        "InfoMine.show"   // 用于存放需要显示的html文件

//////////////////////////////////////////////////////////////////////////
// html格式替换关键词
#define HTML_REPLACE_TITLE      "ARTICLE_TITLE"      
#define HTML_REPLACE_CONTENT    "ARTICLE_CONTENT"
#define HTML_REPLACE_AUTHOR     "ARTICLE_AUTHOR"
#define HTML_REPLACE_TIME       "ARTICLE_TIME"
#define HTML_REPLACE_ORIGIN		"ARTICLE_ORIGIN"
//////////////////////////////////////////////////////////////////////////

#define GET_SRVTYPE_BYNAME(name)	((!name.CompareNoCase(SECRET_TREE_INI_FILE) \
	|| !name.CompareNoCase(BASE_INFO_TREE_INI_FILE)) ? CEV_Connect_ZX : CEV_Connect_HQ)